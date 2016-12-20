/**
 * Copyright (C) 2014-2016 Jessica James.
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

#if defined _WIN32
#include <Windows.h>
#else // _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include "Plugin.h"
#include "Functions.h"
#include "ArrayList.h"
#include "CString.h"
#include "String.h"

using namespace Jupiter::literals;

#if defined _WIN32
constexpr char directory_character = '\\';
#else
constexpr char directory_character = '/';
#endif // _WIN32

struct dlib
{
	Jupiter::Plugin *plugin = nullptr;
#if defined _WIN32
	HMODULE lib = nullptr;
#else // _WIN32
	void *lib = nullptr;
#endif // _WIN32
	~dlib();
};

dlib::~dlib()
{
	if (dlib::lib != nullptr)
	{
#if defined _WIN32
		FreeLibrary(dlib::lib);
#else // _WIN32
		dlclose(dllib::lib);
#endif // _WIN32
	}
}

#if defined _WIN32
const Jupiter::ReferenceString module_file_extension = ".dll"_jrs;
#else // _WIN32
const Jupiter::ReferenceString module_file_extension = ".so"_jrs;
#endif // _WIN32

const Jupiter::ReferenceString config_file_extension = ".ini"_jrs;

Jupiter::StringS plugins_directory = "Plugins"_jrs + directory_character;
Jupiter::StringS plugin_configs_directory = "Configs"_jrs + directory_character;

Jupiter::ArrayList<Jupiter::Plugin> _plugins;
Jupiter::ArrayList<Jupiter::Plugin> *Jupiter::plugins = &_plugins;
Jupiter::ArrayList<dlib> _libList;

/** Jupiter::Plugin Implementation */

Jupiter::Plugin::Plugin()
{
}

Jupiter::Plugin::~Plugin()
{
	for (size_t index = 0; index != _plugins.size(); ++index)
	{
		if (_plugins.get(index) == this)
		{
			_plugins.remove(index);
			break;
		}
	}
}

// Instance Functions

bool Jupiter::Plugin::shouldRemove() const
{
	return Jupiter::Plugin::_shouldRemove;
}

const Jupiter::ReadableString &Jupiter::Plugin::getName() const
{
	return Jupiter::Plugin::name;
}

Jupiter::Config &Jupiter::Plugin::getConfig()
{
	return Jupiter::Plugin::config;
}

bool Jupiter::Plugin::initialize()
{
	return true;
}

void Jupiter::Plugin::OnPostInitialize()
{
}

// Static Functions

void Jupiter::Plugin::setDirectory(const Jupiter::ReadableString &dir)
{
	if (plugins_directory.set(dir) != 0 && plugins_directory.get(plugins_directory.size() - 1) != directory_character)
		plugins_directory += directory_character;
}

const Jupiter::ReadableString &Jupiter::Plugin::getDirectory()
{
	return plugins_directory;
}

void Jupiter::Plugin::setConfigDirectory(const Jupiter::ReadableString &dir)
{
	if (plugin_configs_directory.set(dir) != 0 && plugin_configs_directory.get(plugin_configs_directory.size() - 1) != directory_character)
		plugin_configs_directory += directory_character;
}

const Jupiter::ReadableString &Jupiter::Plugin::getConfigDirectory()
{
	return plugin_configs_directory;
}

Jupiter::Plugin *Jupiter::Plugin::load(const Jupiter::ReadableString &pluginName)
{
	Jupiter::CStringS file = plugins_directory + pluginName + module_file_extension;
	dlib *dPlug = new dlib();

	// Load the library
#if defined _WIN32
	dPlug->lib = LoadLibraryA(file.c_str());
#else // _WIN32
	dPlug->lib = dlopen(file.c_str(), RTLD_LAZY);
#endif // _WIN32
	if (dPlug->lib == nullptr)
	{
		fprintf(stderr, "Error: Unable to load plugin file \"%s\" (File failed to load)" ENDL, file.c_str());
		goto fail;
	}

	{
		// Get the "getPlugin" function
		typedef Jupiter::Plugin *(*func_type)(void);
#if defined _WIN32
		func_type func = (func_type)GetProcAddress(dPlug->lib, "getPlugin");
#else // _WIN32
		func_type func = (func_type)dlsym(dPlug->lib, "getPlugin");
#endif // _WIN32
		if (func == nullptr)
		{
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Invalid plugin)" ENDL, file.c_str());
			goto fail;
		}

		// Get the plugin
		dPlug->plugin = func();
		if (dPlug->plugin == nullptr)
		{
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to initialize)" ENDL, file.c_str());
			goto fail;
		}

		// Initialize the plugin
		dPlug->plugin->name.set(pluginName);
		dPlug->plugin->config.read(plugin_configs_directory + pluginName + config_file_extension);
		dPlug->plugin->initialize();
	}
	{
		// Get and execute the "load" function if it exists
		typedef bool(*func_type)(void);
#if defined _WIN32
		func_type func = (func_type)GetProcAddress(dPlug->lib, "load");
#else // _WIN32
		func_type func = (func_type)dlsym(dPlug->lib, "load");
#endif // _WIN32
		if (func != nullptr && func() == false)
		{
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to load)" ENDL, file.c_str());
			goto fail;
		}
	}

	_libList.add(dPlug);
	_plugins.add(dPlug->plugin);

	return dPlug->plugin;

fail:
	delete dPlug;
	return nullptr;
}

bool Jupiter::Plugin::free(size_t index)
{
	if (index < _plugins.size())
	{
		// Do not free() the plugin; plugin gets free'd by FreeLibrary().
		_plugins.remove(index);
		dlib *dPlug = _libList.remove(index);

		typedef void(*func_type)(void);
#if defined _WIN32
		func_type func = (func_type)GetProcAddress(dPlug->lib, "unload");
#else // _WIN32
		func_type func = (func_type)dlsym(dPlug->lib, "unload");
#endif // _WIN32
		if (func != nullptr) func();

		delete dPlug;
		return true;
	}
	return false;
}

bool Jupiter::Plugin::free(Jupiter::Plugin *plugin)
{
	if (plugin == nullptr)
		return false;

	for (size_t index = 0; index != _plugins.size(); ++index)
		if (_plugins.get(index) == plugin)
			return Jupiter::Plugin::free(index);

	return false;
}

bool Jupiter::Plugin::free(const Jupiter::ReadableString &pluginName)
{
	if (pluginName == nullptr)
		return false;

	for (size_t index = 0; index != _plugins.size(); ++index)
		if (pluginName.matchi(_plugins.get(index)->getName()))
			return Jupiter::Plugin::free(index);

	return false;
}

Jupiter::Plugin *Jupiter::Plugin::get(size_t index)
{
	if (index < _plugins.size())
		return _plugins.get(index);

	return nullptr;
}

Jupiter::Plugin *Jupiter::Plugin::get(const Jupiter::ReadableString &pluginName)
{
	Jupiter::Plugin *plugin;
	for (size_t index = 0; index != _plugins.size(); ++index)
	{
		plugin = _plugins.get(index);
		if (pluginName.matchi(plugin->getName()))
			return plugin;
	}

	return nullptr;
}

// Event Implementations

int Jupiter::Plugin::think()
{
	return 0;
}

int Jupiter::Plugin::OnRehash()
{
	Jupiter::Plugin::config.reload();
	return 0;
}

bool Jupiter::Plugin::OnBadRehash(bool removed)
{
	Jupiter::Plugin::_shouldRemove = removed;
	return false;
}

// Event Placeholders

void Jupiter::Plugin::OnConnect(Jupiter::IRC::Client *)
{
	return;
}

void Jupiter::Plugin::OnDisconnect(Jupiter::IRC::Client *)
{
	return;
}

void Jupiter::Plugin::OnReconnectAttempt(Jupiter::IRC::Client *, bool)
{
	return;
}

void Jupiter::Plugin::OnRaw(Jupiter::IRC::Client *, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnNumeric(Jupiter::IRC::Client *, long int, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnError(Jupiter::IRC::Client *, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnChat(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnNotice(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnServerNotice(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnCTCP(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnAction(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnInvite(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnJoin(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnPart(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnNick(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnKick(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnQuit(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnMode(Jupiter::IRC::Client *, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::Plugin::OnThink(Jupiter::IRC::Client *)
{
	return;
}

void Jupiter::Plugin::OnGenericCommandAdd(Jupiter::GenericCommand &)
{
}

void Jupiter::Plugin::OnGenericCommandRemove(Jupiter::GenericCommand &)
{
}
