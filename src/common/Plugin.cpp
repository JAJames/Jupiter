/**
 * Copyright (C) 2014-2021 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#include <cstring>
#include <cstdio>
#include <string_view>

#if defined _WIN32
#include <Windows.h>
#else // _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include "Plugin.h"
#include <memory>
#include "Functions.h"
#include "String.hpp"

using namespace Jupiter::literals;
using namespace std::literals;

#if defined _WIN32
constexpr char directory_character = '\\';
#else
constexpr char directory_character = '/';
#endif // _WIN32

struct dlib {
	dlib() = default;

	// Ensure the lib handle doesn't get duplicated / prevent double free
	dlib(const dlib&) = delete;
	dlib(dlib&&) = delete;
	dlib& operator=(const dlib&) = delete;
	dlib& operator=(dlib&&) = delete;

#if defined _WIN32
	HMODULE lib = nullptr;
#else // _WIN32
	void* lib = nullptr;
#endif // _WIN32
	~dlib();
};

dlib::~dlib() {
	if (dlib::lib != nullptr) {
#if defined _WIN32
		FreeLibrary(dlib::lib);
#else // _WIN32
		dlclose(dlib::lib);
#endif // _WIN32
	}
}

#if defined _WIN32
const char module_file_extension[]{ ".dll" };
#else // _WIN32
const char module_file_extension[]{ ".so" };
#endif // _WIN32

const Jupiter::ReferenceString config_file_extension = ".ini"_jrs;

Jupiter::StringS plugins_directory = "Plugins"_jrs + directory_character;
Jupiter::StringS plugin_configs_directory = "Configs"_jrs + directory_character;

std::vector<Jupiter::Plugin*> g_plugins; // Array of weak pointers to plugin instances generally stored in static memory
std::vector<Jupiter::Plugin*>& Jupiter::plugins = g_plugins;
std::vector<std::unique_ptr<dlib>> g_libList;

/** Jupiter::Plugin Implementation */

Jupiter::Plugin::Plugin() {
}

Jupiter::Plugin::~Plugin() {
	for (auto itr = g_plugins.begin(); itr != g_plugins.end(); ++itr) {
		if (*itr == this) {
			g_plugins.erase(itr);
			break;
		}
	}
}

// Instance Functions

bool Jupiter::Plugin::shouldRemove() const {
	return _shouldRemove;
}

const Jupiter::ReadableString &Jupiter::Plugin::getName() const {
	return name;
}

Jupiter::Config &Jupiter::Plugin::getConfig() {
	return config;
}

bool Jupiter::Plugin::initialize() {
	return true;
}

void Jupiter::Plugin::OnPostInitialize() {
}

// Static Functions

void Jupiter::Plugin::setDirectory(const Jupiter::ReadableString &dir) {
	if (plugins_directory.set(dir) != 0 && plugins_directory.get(plugins_directory.size() - 1) != directory_character) {
		plugins_directory += directory_character;
	}
}

const Jupiter::ReadableString &Jupiter::Plugin::getDirectory() {
	return plugins_directory;
}

void Jupiter::Plugin::setConfigDirectory(const Jupiter::ReadableString &dir) {
	if (plugin_configs_directory.set(dir) != 0 && plugin_configs_directory.get(plugin_configs_directory.size() - 1) != directory_character) {
		plugin_configs_directory += directory_character;
	}
}

const Jupiter::ReadableString &Jupiter::Plugin::getConfigDirectory() {
	return plugin_configs_directory;
}

Jupiter::Plugin *Jupiter::Plugin::load(const Jupiter::ReadableString &pluginName) {
	std::string file = static_cast<std::string>(plugins_directory) + static_cast<std::string>(pluginName) + module_file_extension;
	std::unique_ptr<dlib> dPlug{ std::make_unique<dlib>() };

	// Load the library
#if defined _WIN32
	dPlug->lib = LoadLibraryA(file.c_str());
#else // _WIN32
	dPlug->lib = dlopen(file.c_str(), RTLD_NOW);
#endif // _WIN32
	if (dPlug->lib == nullptr) {
		fprintf(stderr, "Error: Unable to load plugin file \"%s\" (File failed to load)" ENDL, file.c_str());
		return nullptr;
	}

	Plugin* weak_plugin;
	{
		// Get the "getPlugin" function
		typedef Jupiter::Plugin *(*func_type)(void);
#if defined _WIN32
		func_type func = (func_type)GetProcAddress(dPlug->lib, "getPlugin");
#else // _WIN32
		func_type func = (func_type)dlsym(dPlug->lib, "getPlugin");
#endif // _WIN32
		if (func == nullptr) {
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Invalid plugin)" ENDL, file.c_str());
			return nullptr;
		}

		// Get the plugin
		weak_plugin = func();
		if (weak_plugin == nullptr) {
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to initialize)" ENDL, file.c_str());
			return nullptr;
		}

		// Initialize the plugin
		weak_plugin->name.set(pluginName);
		weak_plugin->config.read(plugin_configs_directory + pluginName + config_file_extension);
		weak_plugin->initialize();
	}
	{
		// Get and execute the "load" function if it exists
		typedef bool(*func_type)(void);
#if defined _WIN32
		func_type func = (func_type)GetProcAddress(dPlug->lib, "load");
#else // _WIN32
		func_type func = (func_type)dlsym(dPlug->lib, "load");
#endif // _WIN32
		if (func != nullptr && func() == false) {
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to load)" ENDL, file.c_str());
			return nullptr;
		}
	}

	g_libList.push_back(std::move(dPlug));
	g_plugins.push_back(weak_plugin);

	return weak_plugin;
}

bool Jupiter::Plugin::free(size_t index) {
	if (index < g_plugins.size()) {
		// Do not free() the plugin; plugin gets free'd either by FreeLibrary() during static memory deallocation or unload.
		g_plugins.erase(g_plugins.begin() + index);
		auto dPlugItr = g_libList.begin() + index;
		std::unique_ptr<dlib> dPlug = std::move(*dPlugItr);
		g_libList.erase(dPlugItr);

		typedef void(*func_type)(void);
#if defined _WIN32
		func_type unload_func = (func_type)GetProcAddress(dPlug->lib, "unload");
#else // _WIN32
		func_type unload_func = (func_type)dlsym(dPlug->lib, "unload");
#endif // _WIN32
		if (unload_func != nullptr) {
			unload_func();
		}
		return true;
	}
	return false;
}

bool Jupiter::Plugin::free(Jupiter::Plugin *plugin) {
	if (plugin == nullptr) {
		return false;
	}

	for (size_t index = 0; index != g_plugins.size(); ++index) {
		if (g_plugins[index] == plugin) {
			return Jupiter::Plugin::free(index);
		}
	}

	return false;
}

bool Jupiter::Plugin::free(const Jupiter::ReadableString &pluginName) {
	if (pluginName == nullptr) {
		return false;
	}

	for (size_t index = 0; index != g_plugins.size(); ++index) {
		if (pluginName.matchi(g_plugins[index]->getName())) {
			return Jupiter::Plugin::free(index);
		}
	}

	return false;
}

Jupiter::Plugin *Jupiter::Plugin::get(size_t index) {
	if (index < g_plugins.size()) {
		return g_plugins[index];
	}

	return nullptr;
}

Jupiter::Plugin *Jupiter::Plugin::get(const Jupiter::ReadableString &pluginName) {
	for (const auto& plugin : g_plugins) {
		if (pluginName.matchi(plugin->getName())) {
			return plugin;
		}
	}

	return nullptr;
}

// Event Implementations

int Jupiter::Plugin::think() {
	return 0;
}

int Jupiter::Plugin::OnRehash() {
	config.reload();
	return 0;
}

bool Jupiter::Plugin::OnBadRehash(bool removed) {
	_shouldRemove = removed;
	return false;
}

// Event Placeholders

void Jupiter::Plugin::OnConnect(Jupiter::IRC::Client *) {
	return;
}

void Jupiter::Plugin::OnDisconnect(Jupiter::IRC::Client *) {
	return;
}

void Jupiter::Plugin::OnReconnectAttempt(Jupiter::IRC::Client *, bool) {
	return;
}

void Jupiter::Plugin::OnRaw(Jupiter::IRC::Client *, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnNumeric(Jupiter::IRC::Client *, long int, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnError(Jupiter::IRC::Client *, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnChat(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnNotice(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnServerNotice(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnCTCP(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnAction(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnInvite(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnJoin(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnPart(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnNick(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnKick(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnQuit(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnMode(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &) {
	return;
}

void Jupiter::Plugin::OnThink(Jupiter::IRC::Client *) {
	return;
}

void Jupiter::Plugin::OnGenericCommandAdd(Jupiter::GenericCommand &) {
}

void Jupiter::Plugin::OnGenericCommandRemove(Jupiter::GenericCommand &) {
}
