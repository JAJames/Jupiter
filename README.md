# Jupiter
Primarily developed in C++, Jupiter is an open-source omni-library initially intended for the purpose of creating IRC bots, but is by no means restricted to IRC bots.
Jupiter has been used to expediate the production of numerous projects ranging from ZIP code verifiers, to game administration systems.

## Strings
Jupiter contains several efficient string implementations, all of which have the simple goals of simplifying string management and boosting performance.
All of the string implementations are easily adaptable to data types other than simple characters. Adapting these implementations is as simple as a type definition.

### "Loose" Strings
Loose strings (i.e: String_Loose or CString_Loose) grow by doubling their buffer size whenever they run out of buffer space, and generally allocate more space than needed. This allows for fast append() operations.

### "Strict" Strings
Strict strings (i.e: String_Strict or CString_Strict) always allocate the bare minimum amount of space necessary, in order to conserve memory. However, these still do not shrink their buffer size automatically, allowing for fast truncate() operations.

### Shift Strings
Shift strings allow shifting of the base position of a string, eliminating the need to generate substrings just to remove the beginning of a string.

### Reference Strings
A reference string is, as the name implies, a reference to a string. It contains no internal buffer, copies no data, and never modifies the base string. While you can shift the base of the string, or truncate elements from the end of the string, you can not modify the actual contents of the string.
These are useful not only for referencing string literals, but also for parsing data, as it entirely eliminates the need to constantly copy substrings just to operate on a single token.

## Sockets
Jupiter includes a Socket wrapper, allowing for simple interaction with sockets, while eliminating all of the platform-dependant code.
Jupiter sockets are also IP agnostic, allowing for compatibility with IPv4, IPv6, and whatever the future may hold.

### Secure Sockets
Jupiter also supports SSL/TLS, using the OpenSSL library, which is the only external code Jupiter depends on. The SecureSocket implementation simplifies usage of SSL, and easily allows for non-SSL sockets to switch to SSL after initialization using C++11 move constructor.

## Plugins
Jupiter supports dynamic plugins, allowing for event-driven programming, without modifying the library. Plugins can easily be loaded and unloaded as a user desires. All plugins are rehashable, allowing for settings to be reloaded without a doing a full unload-load sequence.
Plugins can do as little as adding a console command, or as much as creating a game administration system.

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
