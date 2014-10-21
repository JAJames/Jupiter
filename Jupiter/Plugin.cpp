/**
 * Copyright (C) 2014 Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <cstring>
#include <cstdio>

#if defined _WIN32
#include <Windows.h>
#define DIR_CHR '\\'
#define DEFAULT_PLUGINS_DIRECTORY "Plugins\\"
#else // _WIN32
#include <dlfcn.h>
#define DIR_CHR '/'
#define DEFAULT_PLUGINS_DIRECTORY "Plugins/"
#endif // _WIN32

#include "Plugin.h"
#include "Functions.h"
#include "ArrayList.h"
#include "CString.h"
#include "String.h"

Jupiter::StringS pluginDir = DEFAULT_PLUGINS_DIRECTORY;

Jupiter::ArrayList<Jupiter::Plugin> _plugins;
Jupiter::ArrayList<Jupiter::Plugin> *Jupiter::plugins = &_plugins;
struct dlib;
Jupiter::ArrayList<dlib> _libList;

Jupiter::Plugin::~Plugin()
{
	for (size_t i = 0; i != _plugins.size(); i++)
	{
		if (_plugins.get(i) == this)
		{
			_plugins.remove(i);
			break;
		}
	}
}

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

void Jupiter::setPluginDirectory(const Jupiter::ReadableString &dir)
{
	if (pluginDir.set(dir) != 0 && pluginDir.get(pluginDir.size() - 1) != DIR_CHR) pluginDir += DIR_CHR;
}

const Jupiter::ReadableString &Jupiter::getPluginDirectory()
{
	return pluginDir;
}

#if defined _WIN32
#define MODULE_FILE_EXTENSION ".dll"
#else // _WIN32
#define MODULE_FILE_EXTENSION ".so"
#endif // _WIN32

Jupiter::Plugin *Jupiter::loadPlugin(const Jupiter::ReadableString &pluginName)
{
	return Jupiter::loadPluginFile(Jupiter::CStringS::Format("%.*s%.*s" MODULE_FILE_EXTENSION, pluginDir.size(), pluginDir.ptr(), pluginName.size(), pluginName.ptr()).c_str());
}

Jupiter::Plugin *Jupiter::loadPluginFile(const char *file)
{
	dlib *dPlug = new dlib();

	// Load the library
#if defined _WIN32
	dPlug->lib = LoadLibraryA(file);
#else // _WIN32
	dPlug->lib = dlopen(file, RTLD_LAZY);
#endif // _WIN32
	if (dPlug->lib == nullptr)
	{
		fprintf(stderr, "Error: Unable to load plugin file \"%s\" (File failed to load)" ENDL, file);
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
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Invalid plugin)" ENDL, file);
			goto fail;
		}

		dPlug->plugin = func();
		if (dPlug->plugin == nullptr)
		{
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to initialize)" ENDL, file);
			goto fail;
		}
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
			fprintf(stderr, "Error: Unable to load plugin file \"%s\" (Plugin failed to load)" ENDL, file);
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

bool Jupiter::freePlugin(size_t index)
{
	if (index < _plugins.size())
	{
		// Do not free() the plugin; plugin gets free'd by FreeLibrary().
		_plugins.remove(index);
		dlib *dPlug = _libList.remove(index);

		typedef void (*func_type)(void);
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

bool Jupiter::freePlugin(Jupiter::Plugin *plugin)
{
	if (plugin == nullptr) return false;
	for (size_t i = 0; i != _plugins.size(); i++) if (_plugins.get(i) == plugin) return Jupiter::freePlugin(i);
	return false;
}

bool Jupiter::freePlugin(const Jupiter::ReadableString &pluginName)
{
	if (pluginName == nullptr) return false;
	for (size_t i = 0; i != _plugins.size(); i++) if (pluginName.matchi(_plugins.get(i)->getName())) return Jupiter::freePlugin(i);
	return false;
}

Jupiter::Plugin *Jupiter::getPlugin(size_t index)
{
	if (index < _plugins.size()) return _plugins.get(index);
	return nullptr;
}

Jupiter::Plugin *Jupiter::getPlugin(const Jupiter::ReadableString &pluginName)
{
	Jupiter::Plugin *p;
	for (size_t i = 0; i != _plugins.size(); i++)
	{
		p = _plugins.get(i);
		if (pluginName.matchi(p->getName())) return p;
	}
	return nullptr;
}

// Event Placeholders

int Jupiter::Plugin::think()
{
	return 0;
}

int Jupiter::Plugin::OnRehash()
{
	return 0;
}

bool Jupiter::Plugin::OnBadRehash(bool removed)
{
	Jupiter::Plugin::_shouldRemove = removed;
	return false;
}

bool Jupiter::Plugin::shouldRemove()
{
	return Jupiter::Plugin::_shouldRemove;
}

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