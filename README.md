# Jupiter
Primarily developed in C++, Jupiter is an open-source multi-purpose library initially intended for the purpose of creating IRC bots, but is by no means restricted to IRC bots.
Jupiter has been used to expedite the production of numerous projects ranging from ZIP code verifiers, to game administration systems, to game leaderboards and web APIs.

## Strings
Jupiter contains a number of deprecated string classes, which are deprecated in favor of std::string_view and std::string.
These will be removed in the future, with any useful string utilities being replaced by equivalents in jessilib.

## Sockets
Jupiter includes a Socket wrapper, allowing for simple interaction with sockets, while eliminating all of the platform-dependant code.
Jupiter sockets are also IP agnostic, allowing for compatibility with IPv4, IPv6, and whatever the future may hold.

### Secure Sockets
Jupiter also supports SSL/TLS, using the OpenSSL library, which is the only external code Jupiter depends on. The SecureSocket implementation simplifies usage of SSL, and easily allows for non-SSL sockets to switch to SSL after initialization using C++11 move constructor.

## Plugins
Jupiter supports dynamic plugins, allowing for event-driven programming, without modifying the library. Plugins can easily be loaded and unloaded as a user desires. All plugins are rehashable, allowing for settings to be reloaded without a doing a full unload-load sequence.
Plugins can do as little as adding a console command, or as much as adding support for a protocol or creating a game administration system.

Events:
<ul>
	<li>think()</li>
	<li>OnRehash()</li>
	<li>OnBadRehash()</li>
	<li>shouldRemove()</li>
	<li>OnConnect()</li>
	<li>OnDisconnect()</li>
	<li>OnReconnectAttempt()</li>
	<li>OnRaw()</li>
	<li>OnNumeric()</li>
	<li>OnError()</li>
	<li>OnChat()</li>
	<li>OnNotice()</li>
	<li>OnServerNotice()</li>
	<li>OnCTCP()</li>
	<li>OnAction()</li>
	<li>OnInvite()</li>
	<li>OnJoin()</li>
	<li>OnPart()</li>
	<li>OnNick()</li>
	<li>OnKick()</li>
	<li>OnQuit()</li>
	<li>OnMode()</li>
	<li>OnThink()</li>
</ul>

Pure Virtual Functions:
<ul>
	<li>getName() - Returns the name of the plugin.</li>
</ul>
