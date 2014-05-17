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

Jupiter::CStringS pluginDir = DEFAULT_PLUGINS_DIRECTORY;

Jupiter::ArrayList<Jupiter::Plugin> _plugins;
Jupiter::ArrayList<Jupiter::Plugin> *Jupiter::plugins = &_plugins;
struct dlib;
Jupiter::ArrayList<dlib> _libList;
Jupiter::ArrayList<dlib> *libList = &_libList;

Jupiter::Plugin::~Plugin()
{
	for (int i = _plugins.size(); i >= 0; i--) if (_plugins.get(i) == this) _plugins.remove(i);
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

void Jupiter::setPluginDirectory(const char *dir)
{
	if (dir == nullptr) pluginDir = DEFAULT_PLUGINS_DIRECTORY;
	else
	{
		pluginDir = dir;
		if (pluginDir.size() != 0 && dir[pluginDir.size() - 1] != DIR_CHR) pluginDir += DIR_CHR;
	}
}

const char *Jupiter::getPluginDirectory()
{
	return pluginDir.c_str();
}

Jupiter::Plugin *Jupiter::loadPlugin(const char *pluginName)
{
#if defined _WIN32
	char *fileName = new char[strlen(pluginName) + pluginDir.size() + 5];
	sprintf(fileName, "%s%s.dll", pluginDir.c_str(), pluginName);
#else // _WIN32
	char *fileName = new char[strlen(pluginName) + pluginDirLen + 4];
	sprintf(fileName, "%s%s.so", pluginDir.c_str(), pluginName);
#endif // _WIN32
	return Jupiter::loadPluginFile(fileName);
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
	if (dPlug->lib == nullptr) goto fail;

	// Get the "getPlugin" function
	typedef Jupiter::Plugin *(*func_type)(void);
#if defined _WIN32
	func_type func = (func_type) GetProcAddress(dPlug->lib, "getPlugin");
#else // _WIN32
	func_type func = (func_type)dlsym(dPlug->lib, "getPlugin");
#endif // _WIN32
	if (func == nullptr) goto fail;

	dPlug->plugin = func();
	if (dPlug->plugin == nullptr) goto fail;

	libList->add(dPlug);
	_plugins.add(dPlug->plugin);

	return dPlug->plugin;

fail:
	delete dPlug;
	return nullptr;
}

bool Jupiter::freePlugin(unsigned int index)
{
	if (index < _plugins.size())
	{
		// Do not free() the plugin; plugin gets free'd by FreeLibrary().
		_plugins.remove(index);
		dlib *dPlug = libList->remove(index);

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
	for (int i = _plugins.size() - 1; i >= 0; i--) if (_plugins.get(i) == plugin) return Jupiter::freePlugin(i);
	return false;
}

bool Jupiter::freePlugin(const char *pluginName)
{
	if (pluginName == nullptr) return false;
	for (int i = _plugins.size() - 1; i >= 0; i--) if (strmatchi(pluginName, _plugins.get(i)->getName())) return Jupiter::freePlugin(i);
	return false;
}

Jupiter::Plugin *Jupiter::getPlugin(unsigned int index)
{
	if (index < _plugins.size()) return _plugins.get(index);
	return nullptr;
}

Jupiter::Plugin *Jupiter::getPlugin(const char *pluginName)
{
	Jupiter::Plugin *p;
	for (int i = _plugins.size() - 1; i >= 0; i--)
	{
		p = _plugins.get(i);
		if (strmatchi(pluginName, p->getName())) return p;
	}
	return nullptr;
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

int Jupiter::Plugin::OnRehash()
{
	return 0;
}

void Jupiter::Plugin::OnRaw(Jupiter::IRC::Client *, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnNumeric(Jupiter::IRC::Client *, long int, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnError(Jupiter::IRC::Client *, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnChat(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnNotice(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnServerNotice(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnCTCP(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnAction(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnInvite(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnJoin(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnPart(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnNick(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnKick(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnQuit(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnMode(Jupiter::IRC::Client *, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::Plugin::OnThink(Jupiter::IRC::Client *)
{
	return;
}

int Jupiter::Plugin::think()
{
	return 0;
}