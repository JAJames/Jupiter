/**
 * Copyright (C) 2013-2015 Justin James.
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
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _IRC_NUMERICS_H_HEADER
#define _IRC_NUMERICS_H_HEADER

/**
* These numerics are in large part found here: https://www.alien.net.au/irc/irc2numerics.html
* Not all numerics from the above list are included (generally conflicting numerics such as RPL_MAP).
*/

/**
* To enable the shorter definitions (those without IRC_ in front), define "SHORT_IRC_MACROS" prior to including this file.
*/

#if !defined SKIP_IRC_MACROS

#define IRC_RPL_WELCOME				1 /** RFC2812: Post-registration welcome message. */
#define IRC_RPL_YOURHOST			2 /** RFC2812: Post-registration. Your host is this server and I am running some daemon */
#define IRC_RPL_CREATED				3 /** RFC2812: Post-registration. This was was created at some point in time */
#define IRC_RPL_MYINFO				4 /** RFC2812: Post-registration. <server_name> <version> <user_modes> <chan_modes> */
#define IRC_RPL_BOUNCEOLD			5	/** RFC2812/DEPRECATED: 005 is rarely used as a bounce indicator, but was defined in RFC2812. */
#define IRC_RPL_ISUPPORT			5 /** Used to indicate what a server supports. Does not appear in any RFC, but was drafted in 2004. */
#define IRC_RPL_PROTOCTL			IRC_RPL_ISUPPORT /** Use IRC_RPL_ISUPPORT instead */
#define IRC_RPL_SNOMASK				8 /** Server notice mask */
#define IRC_RPL_STATMEMTOT			9 /** I have no idea what this does. */
#define IRC_RPL_BOUNCE				10 /** Bounce server message. Different syntax from IRC_RPL_BOUNCEOLD */
#define IRC_RPL_STATMEM				10 /** I have no idea what this does. */
#define IRC_RPL_YOURCOOKIE			14 /** I have no idea what this does. */
#define IRC_RPL_YOURID				42 /** Informs you of your unique ID. */
#define IRC_RPL_SAVENICK			43 /** Your nick was force-changed due to a nick collision */
#define IRC_RPL_ATTEMPTINGJUNC		50 /** I have no idea what this does. */
#define IRC_RPL_ATTEMPTINGREROUTE	51 /** I have no idea what this does. */
#define IRC_RPL_TRACELINK			200 /** RFC1459: "If the TRACE message is destined for another server, all intermediate servers must return a IRC_RPL_TRACELINK reply to indicate that the TRACE passed through it and where its going next."*/
#define IRC_RPL_TRACECONNECTING 	201 /** RFC1459 */
#define IRC_RPL_TRACEHANDSHAKE		202 /** RFC1459 */
#define IRC_RPL_TRACEUNKNOWN 		203 /** RFC1459 */
#define IRC_RPL_TRACEOPERATOR 		204 /** RFC1459 */
#define IRC_RPL_TRACEUSER 			205 /** RFC1459 */
#define IRC_RPL_TRACESERVER			206 /** RFC1459 */
#define IRC_RPL_TRACESERVICE 		207 /** RFC2812 */
#define IRC_RPL_TRACENEWTYPE		208 /** RFC1459 */
#define IRC_RPL_TRACECLASS			209 /** RFC2812 */
#define IRC_RPL_TRACERECONNECT		210 /** RFC2812 */
#define IRC_RPL_STATS				210 /** I have no idea what this does. */
#define IRC_RPL_STATSLINKINFO		211 /** RFC1459 */
#define IRC_RPL_STATSCOMMANDS		212 /** RFC1459 */
#define IRC_RPL_STATSCLINE			213 /** RFC1459 */
#define IRC_RPL_STATSNLINE			214 /** RFC1459 */
#define IRC_RPL_STATSILINE			215 /** RFC1459 */
#define IRC_RPL_STATSKLINE			216 /** RFC1459 */
#define IRC_RPL_STATSQLINE			217 /** RFC1459 */
#define IRC_RPL_STATSYLINE			218 /** RFC1459 */
#define IRC_RPL_ENDOFSTATS			219 /** RFC1459 */
#define IRC_RPL_UMODEIS				221 /** RFC1459 */
#define IRC_RPL_SERVICEINFO			231 /** RFC1459 */
#define IRC_RPL_ENDOFSERVICES		232 /** RFC1459 */
#define IRC_RPL_SERVICE				233 /** RFC1459 */
#define IRC_RPL_SERVLIST			234 /** RFC2812 */
#define IRC_RPL_STATSVERBOSE		236 /** I have no idea what this does. */
#define IRC_RPL_STATSENGINE			237 /** I have no idea what this does. */
#define IRC_RPL_STATSIAUTH			239 /** I have no idea what this does. */
#define IRC_RPL_STATSLLINE			241 /** RFC1459 */
#define IRC_RPL_STATSUPTIME			242 /** RFC1459 */
#define IRC_RPL_STATSOLINE			243 /** RFC1459 */
#define IRC_RPL_STATSHLINE			244 /** RFC1459 */
#define IRC_RPL_STATSSLINE			245 /** I have no idea what this does. */
#define IRC_RPL_STATSPING			246 /** RFC2812 */
#define IRC_RPL_STATSCONN			250 /** I have no idea what this does. */
#define IRC_RPL_LUSERCLIENT			251 /** RFC1459 */
#define IRC_RPL_LUSEROP				252 /** RFC1459 */
#define IRC_RPL_LUSERUNKNOWN		253 /** RFC1459 */
#define IRC_RPL_LUSERCHANNELS		254 /** RFC1459 */
#define IRC_RPL_LUSERME				255 /** RFC1459 */
#define IRC_RPL_ADMINME				256 /** RFC1459 */
#define IRC_RPL_ADMINLOC1			257 /** RFC1459 */
#define IRC_RPL_ADMINLOC2			258 /** RFC1459 */
#define IRC_RPL_ADMINEMAIL			259 /** RFC1459 */
#define IRC_RPL_TRACELOG			261 /** RFC1459 */
#define IRC_RPL_TRACEEND			262 /** RFC2812 */
#define IRC_RPL_TRYAGAIN			263 /** RFC2812 */
#define IRC_RPL_LOCALUSERS			265 /** I have no idea what this does. */
#define IRC_RPL_CURRENT_LOCAL IRC_RPL_LOCALUSERS /** Use IRC_RPL_GLOBALUSERS */
#define IRC_RPL_GLOBALUSERS			266 /** I have no idea what this does. */
#define IRC_RPL_CURRENT_GLOBAL IRC_RPL_GLOBALUSERS /** Use IRC_RPL_GLOBALUSERS */
#define IRC_RPL_START_NETSTAT		267 /** I have no idea what this does. */
#define IRC_RPL_NETSTAT				268 /** I have no idea what this does. */
#define IRC_RPL_END_NETSTAT			269 /** I have no idea what this does. */
#define IRC_RPL_PRIVS				270 /** I have no idea what this does. */
#define IRC_RPL_SILELIST			271 /** I have no idea what this does. */
#define IRC_RPL_ENDOFSILELIST		272 /** I have no idea what this does. */
#define IRC_RPL_NOTIFY				273 /** I have no idea what this does. */
#define IRC_RPL_VCHANEXIST			276 /** I have no idea what this does. */
#define IRC_RPL_VCHANLIST			277 /** I have no idea what this does. */
#define IRC_RPL_VCHANHELP			278 /** I have no idea what this does. */
#define IRC_RPL_GLIST				280 /** I have no idea what this does. */
#define IRC_RPL_CHANINFO_KICKS		296 /** I have no idea what this does. */
#define IRC_RPL_END_CHANINFO		299 /** I have no idea what this does. */
#define IRC_RPL_NONE				300 /** RFC1459 */
#define IRC_RPL_AWAY				301 /** RFC1459 */
#define IRC_RPL_USERHOST			302 /** RFC1459 */
#define IRC_RPL_ISON				303 /** RFC1459 */
#define IRC_RPL_TEXT				304 /** I have no idea what this does. Supposedly deprecated? */
#define IRC_RPL_UNAWAY				305 /** RFC1459 */
#define IRC_RPL_NOWAWAY				306 /** RFC1459 */
#define IRC_RPL_WHOISUSER			311 /** RFC1459 */
#define IRC_RPL_WHOISSERVER			312 /** RFC1459 */
#define IRC_RPL_WHOISOPERATOR		313 /** RFC1459 */
#define IRC_RPL_WHOWASUSER			314 /** RFC1459 */
#define IRC_RPL_ENDOFWHO			315 /** RFC1459 */
#define IRC_RPL_WHOISCHANOP			316 /** RFC1459 */
#define IRC_RPL_WHOISIDLE			317 /** RFC1459 */
#define IRC_RPL_ENDOFWHOIS			318 /** RFC1459 */
#define IRC_RPL_WHOISCHANNELS		319 /** RFC1459 */
#define IRC_RPL_WHOISVIRT			320 /** I have no idea what this does. */
#define IRC_RPL_WHOIS_HIDDEN		320 /** I have no idea what this does. */
#define IRC_RPL_WHOISSPECIAL		320 /** I have no idea what this does. */
#define IRC_RPL_LISTSTART			321 /** RFC1459 */
#define IRC_RPL_LIST				322 /** RFC1459 */
#define IRC_RPL_LISTEND				323 /** RFC1459 */
#define IRC_RPL_CHANNELMODEIS		324 /** RFC1459 */
#define IRC_RPL_UNIQOPIS			325 /** RFC2812 */
#define IRC_RPL_NOCHANPASS			326 /** I have no idea what this does. */
#define IRC_RPL_CHPASSUNKNOWN		327 /** I have no idea what this does. */
#define IRC_RPL_CHANNEL_URL			328 /** I have no idea what this does. */
#define IRC_RPL_CREATIONTIME		329 /** I have no idea what this does. */
#define IRC_RPL_NOTOPIC				331 /** RFC1459 */
#define IRC_RPL_TOPIC				332 /** RFC1459 */
#define IRC_RPL_TOPICWHOTIME		333 /** I have no idea what this does. */
#define IRC_RPL_BADCHANPASS			339 /** I have no idea what this does. */
#define IRC_RPL_USERIP				340 /** I have no idea what this does. */
#define IRC_RPL_INVITING			341 /** RFC1459 */
#define IRC_RPL_SUMMONING			342 /** RFC1459 */
#define IRC_RPL_INVITED				345 /** I have no idea what this does. */
#define IRC_RPL_INVITELIST			346 /** RFC2812 */
#define IRC_RPL_ENDOFINVITELIST		347 /** RFC2812 */
#define IRC_RPL_EXCEPTLIST			348 /** RFC2812 */
#define IRC_RPL_ENDOFEXCEPTLIST		349 /** RFC2812 */
#define IRC_RPL_VERSION				351 /** RFC1459 */
#define IRC_RPL_WHOREPLY			352 /** RFC1459 */
#define IRC_RPL_NAMREPLY			353 /** RFC1459 */
#define IRC_RPL_WHOSPCRPL			354 /** I have no idea what this does. */
#define IRC_RPL_NAMREPLY_			355 /** I have no idea what this does. */
#define IRC_RPL_KILLDONE			361 /** RFC1459 */
#define IRC_RPL_CLOSING				362 /** RFC1459 */
#define IRC_RPL_CLOSEEND			363 /** RFC1459 */
#define IRC_RPL_LINKS				364 /** RFC1459 */
#define IRC_RPL_ENDOFLINKS			365 /** RFC1459 */
#define IRC_RPL_ENDOFNAMES			366 /** RFC1459 */
#define IRC_RPL_BANLIST				367 /** RFC1459 */
#define IRC_RPL_ENDOFBANLIST		368 /** RFC1459 */
#define IRC_RPL_ENDOFWHOWAS			369 /** RFC1459 */
#define IRC_RPL_INFO				371 /** RFC1459 */
#define IRC_RPL_MOTD				372 /** RFC1459 */
#define IRC_RPL_INFOSTART			373 /** RFC1459 */
#define IRC_RPL_ENDOFINFO			374 /** RFC1459 */
#define IRC_RPL_MOTDSTART			375 /** RFC1459 */
#define IRC_RPL_ENDOFMOTD			376 /** RFC1459 */
#define IRC_RPL_YOUREOPER			381 /** RFC1459 */
#define IRC_RPL_REHASHING			382 /** RFC1459 */
#define IRC_RPL_YOURESERVICE		383 /** RFC2812 */
#define IRC_RPL_MYPORTIS			384 /** RFC1459 */
#define IRC_RPL_NOTOPERANYMORE		385 /** You are no longer a server operator */
#define IRC_RPL_ALIST				388 /** I have no idea what this does. */
#define IRC_RPL_ENDOFALIST			389 /** I have no idea what this does. */
#define IRC_RPL_TIME				391 /** RFC1459: TIME command response. <server> [possible extension data] :<time string>*/
#define IRC_RPL_USERSSTART			392 /** RFC1459 */
#define IRC_RPL_USERS				393 /** RFC1459 */
#define IRC_RPL_ENDOFUSERS			394 /** RFC1459 */
#define IRC_RPL_NOUSERS				395 /** I have no idea what this does. */
#define IRC_RPL_HOSTHIDDEN			396 /** Your hostname is now hidden. */
#define IRC_ERR_UNKNOWNERROR		400 /** An unknown error occured. */
#define IRC_ERR_NOSUCHNICK			401 /** RFC1459 */
#define IRC_ERR_NOSUCHSERVER		402 /** RFC1459 */
#define IRC_ERR_NOSUCHCHANNEL		403 /** RFC1459 */
#define IRC_ERR_CANNOTSENDTOCHAN	404 /** RFC1459 */
#define IRC_ERR_TOOMANYCHANNELS		405 /** RFC1459 */
#define IRC_ERR_WASNOSUCHNICK		406 /** RFC1459 */
#define IRC_ERR_TOOMANYTARGETS		407 /** RFC1459 */
#define IRC_ERR_NOSUCHSERVICE		408 /** RFC2812 */
#define IRC_ERR_NOORIGIN			409 /** RFC1459 */
#define IRC_ERR_NORECIPIENT			411 /** RFC1459 */
#define IRC_ERR_NOTEXTTOSEND		412 /** RFC1459 */
#define IRC_ERR_NOTOPLEVEL			413 /** RFC1459 */
#define IRC_ERR_WILDTOPLEVEL		414 /** RFC1459 */
#define IRC_ERR_BADMASK				415 /** RFC2812 */
#define IRC_ERR_TOOMANYMATCHES		416 /** I have no idea what this does. */
#define IRC_ERR_QUERYTOOLONG		416 /** I have no idea what this does. */
#define IRC_ERR_LENGTHTRUNCATED		419 /** I have no idea what this does. */
#define IRC_ERR_UNKNOWNCOMMAND		421 /** RFC1459 */
#define IRC_ERR_NOMOTD				422 /** RFC1459 */
#define IRC_ERR_NOADMININFO			423 /** RFC1459 */
#define IRC_ERR_FILEERROR			424 /** RFC1459 */
#define IRC_ERR_NOOPERMOTD			425 /** There is no operator MOTD. */
#define IRC_ERR_TOOMANYAWAY			429 /** I have no idea what this does. */
#define IRC_ERR_EVENTNICKCHANGE		430 /** Can not change nickname due to channel event. */
#define IRC_ERR_NONICKNAMEGIVEN		431 /** RFC1459: You didn't specify a nickname. */
#define IRC_ERR_ERRONEOUSNICKNAME	432 /** RFC1459: Nickname is reserved or otherwise invalid. */
#define IRC_ERR_ERRONEUSNICKNAME	IRC_ERR_ERRONEOUSNICKNAME /** Use IRC_ERR_EERONEOUSNICKNAME instead. */
#define IRC_ERR_NICKNAMEINUSE		433 /** RFC1459: Already used nickname was passed to NICK */
#define IRC_ERR_NICKCOLLISION		436 /** RFC1459: A nickname collision was detected. */
#define IRC_ERR_UNAVAILRESOURCE		437 /** RFC2812: Target is temporarily unreachable. */
#define IRC_ERR_BANNICKCHANGE		437 /** "Ban nick change" */
#define IRC_ERR_TARGETTOOFAST		439 /** I have no idea what this does. */
#define IRC_RPL_INVTOOFAST			IRC_ERR_TARGETTOOFAST
#define IRC_RPL_MSGTOOFAST			IRC_ERR_TARGETTOOFAST
#define IRC_ERR_SERVICESDOWN		440 /** I have no idea what this does. */
#define IRC_ERR_USERNOTINCHANNEL	441 /** RFC1459 */
#define IRC_ERR_NOTONCHANNEL		442 /** RFC1459 */
#define IRC_ERR_USERONCHANNEL		443 /** RFC1459 */
#define IRC_ERR_NOLOGIN				444 /** RFC1459 */
#define IRC_ERR_SUMMONDISABLED		445 /** RFC1459 */
#define IRC_ERR_USERSDISABLED		446 /** RFC1459 */
#define IRC_ERR_NONICKCHANGE		447 /** I have no idea what this does. */
#define IRC_ERR_NOTIMPLEMENTED		449 /** I have no idea what this does. */
#define IRC_ERR_NOTREGISTERED		451 /** RFC1459: You need to be registered. */
#define IRC_ERR_IDCOLLISION			452 /** I have no idea what this does. */
#define IRC_ERR_NICKLOST			453 /** I have no idea what this does. */
#define IRC_ERR_HOSTILENAME			455 /** I have no idea what this does. */
#define IRC_ERR_ACCEPTFULL			456 /** I have no idea what this does. */
#define IRC_ERR_ACCEPTEXIST			457 /** I have no idea what this does. */
#define IRC_ERR_ACCEPTNOT			458 /** I have no idea what this does. */
#define IRC_ERR_NOHIDING			459 /** I have no idea what this does. */
#define IRC_ERR_NOTFORHALFOPS		460 /** I have no idea what this does. */
#define IRC_ERR_NEEDMOREPARAMS		461 /** RFC1459: Command requires more parameters. */
#define IRC_ERR_ALREADYREGISTERED	462 /** RFC1459: You're already registered. */
#define IRC_ERR_NOPERMFORHOST		463 /** RFC1459: Your host is blocked. */
#define IRC_ERR_PASSWDMISMATCH		464 /** RFC1459: Incorrect or missing server password. */
#define IRC_ERR_YOUREBANNEDCREEP	465 /** RFC1459: You're banned. */
#define IRC_ERR_YOUWILLBEBANNED		466 /** RFC1459: You will soon be banned. */
#define IRC_ERR_KEYSET				467 /** RFC1459: A key for this channel has already been set. */
#define IRC_ERR_LINKSET				469 /** I have no idea what this does. */
#define IRC_ERR_CHANNELISFULL		471 /** RFC1459: That channel is full. */
#define IRC_ERR_UNKNOWNMODE			472 /** RFC1459: That mode is unknown. */
#define IRC_ERR_INVITEONLYCHAN		473 /** RFC1459: That channel is invite-only. */
#define IRC_ERR_BANNEDFROMCHAN		474 /** RFC1459: You're banned from that channel. */
#define IRC_ERR_BADCHANNELKEY		475 /** RFC1459: Channel key was missing/incorrect. */
#define IRC_ERR_BADCHANMASK			476 /** RFC2812: That channel mask is invalid. */
#define IRC_ERR_NOCHANMODES			477 /** RFC2812: Channel does not support mode changes. */
#define IRC_ERR_BANLISTFULL			478 /** RFC2812: Channel access/ban list is full. */
#define IRC_ERR_BADCHANNAME			479 /** I have no idea what this does. */
#define IRC_ERR_LINKFAIL			479 /** I have no idea what this does. */
#define IRC_ERR_NOPRIVILEGES		481 /** RFC1459: You do not have the neccessary privledges. */
#define IRC_ERR_CHANOPRIVSNEEDED	482 /** RFC1459: You do not have the neccessary channel privledges. */
#define IRC_ERR_CANTKILLSERVER		483 /** RFC1459: You can not KILL a server. */
#define IRC_ERR_RESTRICTED			484 /** RFC2812: This connection is "restricted" */
#define IRC_ERR_UNIQOPRIVSNEEDED	485 /** RFC2812: That mode requires "channel creator" privledges. */
#define IRC_ERR_TSLESSCHAN			488 /** I have no idea what this does. */
#define IRC_ERR_NOOPERHOST			491 /** RFC1459: You can not become a server operator. */
#define IRC_ERR_NOSERVICEHOST		492 /** RFC1459 */
#define IRC_ERR_NOFEATURE			493 /** I have no idea what this does. */
#define IRC_ERR_BADFEATURE			494 /** I have no idea what this does. */
#define IRC_ERR_BADLOGTYPE			495 /** I have no idea what this does. */
#define IRC_ERR_BADLOGSYS			496 /** I have no idea what this does. */
#define IRC_ERR_BADLOGVALUE			497 /** I have no idea what this does. */
#define IRC_ERR_ISOPERLCHAN			498 /** I have no idea what this does. */
#define IRC_ERR_CHANOWNPRIVNEEDED	499 /** I have no idea what this does. */
#define IRC_ERR_UMODEUNKNOWNFLAG	501 /** RFC1459: This user mode flag is not recognized. */
#define IRC_ERR_USERSDONTMATCH		502 /** RFC1459: You can not mody/view other users' modes. */
#define IRC_ERR_GHOSTEDCLIENT		503 /** I have no idea what this does. */
#define IRC_ERR_USERNOTONSERV		504 /** I have no idea what this does. */
#define IRC_ERR_SILELISTFULL		511 /** I have no idea what this does. */
#define IRC_ERR_TOOMANYWATCH		512 /** I have no idea what this does. */
#define IRC_ERR_BADPING				513 /** Invalid PING response. */
#define IRC_ERR_NEEDPONG			IRC_ERR_BADPING /** Use IRC_ERR_BADPING instead. */
#define IRC_ERR_BADEXPIRE			515 /** I have no idea what this does. */
#define IRC_ERR_DONTCHEAT			516 /** I have no idea what this does. */
#define IRC_ERR_DISABLED			517 /** I have no idea what this does. */
#define IRC_ERR_WHOSYNTAX			522 /** I have no idea what this does. */
#define IRC_ERR_WHOLIMEXCEED		523 /** I have no idea what this does. */
#define IRC_ERR_REMOTEPFX			525 /** I have no idea what this does. */
#define IRC_ERR_PFXUNROUTABLE		526 /** I have no idea what this does. */
#define IRC_ERR_BADHOSTMASK			550 /** I have no idea what this does. */
#define IRC_ERR_HOSTUNAVAIL			551 /** I have no idea what this does. */
#define IRC_ERR_USINGSLINE			552 /** I have no idea what this does. */
#define IRC_RPL_LOGON				600 /** I have no idea what this does. */
#define IRC_RPL_LOGOFF				601 /** I have no idea what this does. */
#define IRC_RPL_WATCHOFF			602 /** I have no idea what this does. */
#define IRC_RPL_WATCHSTAT			602 /** I have no idea what this does. */
#define IRC_RPL_NOWON				604 /** I have no idea what this does. */
#define IRC_RPL_NOWOFF				605 /** I have no idea what this does. */
#define IRC_RPL_WATCHLIST			606 /** I have no idea what this does. */
#define IRC_RPL_ENDOFWATCHLIST		607 /** I have no idea what this does. */
#define IRC_RPL_WATCHCLEAR			608 /** I have no idea what this does. */
#define IRC_RPL_ISLOCOP				611 /** I have no idea what this does. */
#define IRC_RPL_ISNOTOPER			612 /** I have no idea what this does. */
#define IRC_RPL_ENDOFISOPER			613 /** I have no idea what this does. */
#define IRC_RPL_DCCLIST				618 /** I have no idea what this does. */
#define IRC_RPL_OMOTDSTART			624 /** I have no idea what this does. */
#define IRC_RPL_OMOTD				625 /** I have no idea what this does. */
#define IRC_RPL_ENDOFO				626 /** I have no idea what this does. */
#define IRC_RPL_SETTINGS			630 /** I have no idea what this does. */
#define IRC_RPL_ENDOFSETTINGS		631 /** I have no idea what this does. */
#define IRC_RPL_TRACEROUTE_HOP		660 /** I have no idea what this does. */
#define IRC_RPL_TRACEROUTE_START	661 /** I have no idea what this does. */
#define IRC_RPL_MODECHANGEWARN		662 /** I have no idea what this does. */
#define IRC_RPL_CHANREDIR			663 /** I have no idea what this does. */
#define IRC_RPL_SERVMODEIS			664 /** I have no idea what this does. */
#define IRC_RPL_OTHERUMODEIS		665 /** I have no idea what this does. */
#define IRC_RPL_ENDOF_GENERIC		666 /** I have no idea what this does. */
#define IRC_RPL_WHOWASDETAILS		667 /** I have no idea what this does. */
#define IRC_RPL_STARTTLS			670 /** Proceed with SSL/TLS initialization */
#define IRC_RPL_WHOISSECURE			671 /** User is using a secure connection. */
#define IRC_RPL_UNKNOWNMODES		672 /** I have no idea what this does. */
#define IRC_RPL_CANNOTSETMODES		673 /** I have no idea what this does. */
#define IRC_RPL_LUSERSTAFF			678 /** I have no idea what this does. */
#define IRC_RPL_TIMEONSERVERIS		679 /** I have no idea what this does. */
#define IRC_RPL_NETWORKS			682 /** I have no idea what this does. */
#define IRC_RPL_YOURLANGUAGEIS		687 /** I have no idea what this does. */
#define IRC_RPL_LANGUAGE			688 /** I have no idea what this does. */
#define IRC_RPL_WHOISSTAFF			689 /** I have no idea what this does. */
#define IRC_RPL_WHOISLANGUAGE		690 /** I have no idea what this does. */
#define IRC_ERR_STARTTLS			691 /** An error occured setting up SSL/TLS server-side */
#define IRC_RPL_MODLIST				702 /** I have no idea what this does. */
#define IRC_RPL_ENDOFMODLIST		703 /** I have no idea what this does. */
#define IRC_RPL_HELPSTART			704 /** I have no idea what this does. */
#define IRC_RPL_HELPTXT				705 /** I have no idea what this does. */
#define IRC_RPL_ENDOFHELP			706 /** I have no idea what this does. */
#define IRC_RPL_ETRACEFULL			708 /** I have no idea what this does. */
#define IRC_RPL_ETRACE				709 /** I have no idea what this does. */
#define IRC_RPL_KNOCK				710 /** I have no idea what this does. */
#define IRC_RPL_KNOCKDLVR			711 /** I have no idea what this does. */
#define IRC_RPL_TOOMANYKNOCK		712 /** I have no idea what this does. */
#define IRC_RPL_CHANOPEN			713 /** I have no idea what this does. */
#define IRC_RPL_KNOCKONCHAN			714 /** I have no idea what this does. */
#define IRC_RPL_KNOCKDISABLED		715 /** I have no idea what this does. */
#define IRC_RPL_TARGUMODEG			716 /** I have no idea what this does. */
#define IRC_RPL_TARGNOTIFY			717 /** I have no idea what this does. */
#define IRC_RPL_UMODEGMSG			718 /** I have no idea what this does. */
//#define IRC_RPL_OMOTDSTART			720 /** I have no idea what this does. */
//#define IRC_RPL_OMOTD				721 /** I have no idea what this does. */
//#define IRC_RPL_ENDOFOMOTD			722 /** I have no idea what this does. */
#define IRC_RPL_NOPRIVS				723 /** I have no idea what this does. */
#define IRC_RPL_TESTMARK			724 /** I have no idea what this does. */
#define IRC_RPL_TESTLINE			725 /** I have no idea what this does. */
#define IRC_RPL_NOTESTLINE			726 /** I have no idea what this does. */
#define IRC_RPL_XINFO				771 /** I have no idea what this does. */
#define IRC_RPL_XINFOSTART			773 /** I have no idea what this does. */
#define IRC_RPL_XINFOEND			774 /** I have no idea what this does. */
#define IRC_RPL_CANNOTDOCOMMAND		972 /** I have no idea what this does. */
#define IRC_RPL_CANNOTCHANGEUMODE	973 /** I have no idea what this does. */
#define IRC_RPL_CANNOTCHANGECHANMODE	974 /** I have no idea what this does. */
#define IRC_RPL_CANNOTCHANGESERVERMODE	975 /** I have no idea what this does. */
#define IRC_RPL_CANNOTSENDTONICK	976 /** I have no idea what this does. */
#define IRC_RPL_UNKNOWNSERVERMODE	977 /** I have no idea what this does. */
#define IRC_RPL_SERVERMODELOCK		979 /** I have no idea what this does. */
#define IRC_RPL_BADCHARENCODING		980 /** I have no idea what this does. */
#define IRC_RPL_TOOMANYLANGUAGES	981 /** I have no idea what this does. */
#define IRC_RPL_NOLANGUAGE			982 /** I have no idea what this does. */
#define IRC_RPL_TEXTTOOSHORT		983 /** I have no idea what this does. */
#define IRC_RPL_NUMERIC_ERR			999 /** I have no idea what this does. */

#if defined SHORT_IRC_MACROS

#define RPL_WELCOME				1 /** RFC2812: Post-registration welcome message. */
#define RPL_YOURHOST			2 /** RFC2812: Post-registration. Your host is this server and I am running some daemon */
#define RPL_CREATED				3 /** RFC2812: Post-registration. This was was created at some point in time */
#define RPL_MYINFO				4 /** RFC2812: Post-registration. <server_name> <version> <user_modes> <chan_modes> */
#define RPL_BOUNCEOLD			5	/** RFC2812/DEPRECATED: 005 is rarely used as a bounce indicator, but was defined in RFC2812. */
#define RPL_ISUPPORT			5 /** Used to indicate what a server supports. Does not appear in any RFC, but was drafted in 2004. */
#define RPL_PROTOCTL			RPL_ISUPPORT /** Use RPL_ISUPPORT instead */
#define RPL_SNOMASK				8 /** Server notice mask */
#define RPL_STATMEMTOT			9 /** I have no idea what this does. */
#define RPL_BOUNCE				10 /** Bounce server message. Different syntax from RPL_BOUNCEOLD */
#define RPL_STATMEM				10 /** I have no idea what this does. */
#define RPL_YOURCOOKIE			14 /** I have no idea what this does. */
#define RPL_YOURID				42 /** Informs you of your unique ID. */
#define RPL_SAVENICK			43 /** Your nick was force-changed due to a nick collision */
#define RPL_ATTEMPTINGJUNC		50 /** I have no idea what this does. */
#define RPL_ATTEMPTINGREROUTE	51 /** I have no idea what this does. */
#define RPL_TRACELINK			200 /** RFC1459: "If the TRACE message is destined for another server, all intermediate servers must return a RPL_TRACELINK reply to indicate that the TRACE passed through it and where its going next."*/
#define RPL_TRACECONNECTING 	201 /** RFC1459 */
#define RPL_TRACEHANDSHAKE		202 /** RFC1459 */
#define RPL_TRACEUNKNOWN 		203 /** RFC1459 */
#define RPL_TRACEOPERATOR 		204 /** RFC1459 */
#define RPL_TRACEUSER 			205 /** RFC1459 */
#define RPL_TRACESERVER			206 /** RFC1459 */
#define RPL_TRACESERVICE 		207 /** RFC2812 */
#define RPL_TRACENEWTYPE		208 /** RFC1459 */
#define RPL_TRACECLASS			209 /** RFC2812 */
#define RPL_TRACERECONNECT		210 /** RFC2812 */
#define RPL_STATS				210 /** I have no idea what this does. */
#define RPL_STATSLINKINFO		211 /** RFC1459 */
#define RPL_STATSCOMMANDS		212 /** RFC1459 */
#define RPL_STATSCLINE			213 /** RFC1459 */
#define RPL_STATSNLINE			214 /** RFC1459 */
#define RPL_STATSILINE			215 /** RFC1459 */
#define RPL_STATSKLINE			216 /** RFC1459 */
#define RPL_STATSQLINE			217 /** RFC1459 */
#define RPL_STATSYLINE			218 /** RFC1459 */
#define RPL_ENDOFSTATS			219 /** RFC1459 */
#define RPL_UMODEIS				221 /** RFC1459 */
#define RPL_SERVICEINFO			231 /** RFC1459 */
#define RPL_ENDOFSERVICES		232 /** RFC1459 */
#define RPL_SERVICE				233 /** RFC1459 */
#define RPL_SERVLIST			234 /** RFC2812 */
#define RPL_STATSVERBOSE		236 /** I have no idea what this does. */
#define RPL_STATSENGINE			237 /** I have no idea what this does. */
#define RPL_STATSIAUTH			239 /** I have no idea what this does. */
#define RPL_STATSLLINE			241 /** RFC1459 */
#define RPL_STATSUPTIME			242 /** RFC1459 */
#define RPL_STATSOLINE			243 /** RFC1459 */
#define RPL_STATSHLINE			244 /** RFC1459 */
#define RPL_STATSSLINE			245 /** I have no idea what this does. */
#define RPL_STATSPING			246 /** RFC2812 */
#define RPL_STATSCONN			250 /** I have no idea what this does. */
#define RPL_LUSERCLIENT			251 /** RFC1459 */
#define RPL_LUSEROP				252 /** RFC1459 */
#define RPL_LUSERUNKNOWN		253 /** RFC1459 */
#define RPL_LUSERCHANNELS		254 /** RFC1459 */
#define RPL_LUSERME				255 /** RFC1459 */
#define RPL_ADMINME				256 /** RFC1459 */
#define RPL_ADMINLOC1			257 /** RFC1459 */
#define RPL_ADMINLOC2			258 /** RFC1459 */
#define RPL_ADMINEMAIL			259 /** RFC1459 */
#define RPL_TRACELOG			261 /** RFC1459 */
#define RPL_TRACEEND			262 /** RFC2812 */
#define RPL_TRYAGAIN			263 /** RFC2812 */
#define RPL_LOCALUSERS			265 /** I have no idea what this does. */
#define RPL_CURRENT_LOCAL RPL_LOCALUSERS /** Use RPL_GLOBALUSERS */
#define RPL_GLOBALUSERS			266 /** I have no idea what this does. */
#define RPL_CURRENT_GLOBAL RPL_GLOBALUSERS /** Use RPL_GLOBALUSERS */
#define RPL_START_NETSTAT		267 /** I have no idea what this does. */
#define RPL_NETSTAT				268 /** I have no idea what this does. */
#define RPL_END_NETSTAT			269 /** I have no idea what this does. */
#define RPL_PRIVS				270 /** I have no idea what this does. */
#define RPL_SILELIST			271 /** I have no idea what this does. */
#define RPL_ENDOFSILELIST		272 /** I have no idea what this does. */
#define RPL_NOTIFY				273 /** I have no idea what this does. */
#define RPL_VCHANEXIST			276 /** I have no idea what this does. */
#define RPL_VCHANLIST			277 /** I have no idea what this does. */
#define RPL_VCHANHELP			278 /** I have no idea what this does. */
#define RPL_GLIST				280 /** I have no idea what this does. */
#define RPL_CHANINFO_KICKS		296 /** I have no idea what this does. */
#define RPL_END_CHANINFO		299 /** I have no idea what this does. */
#define RPL_NONE				300 /** RFC1459 */
#define RPL_AWAY				301 /** RFC1459 */
#define RPL_USERHOST			302 /** RFC1459 */
#define RPL_ISON				303 /** RFC1459 */
#define RPL_TEXT				304 /** I have no idea what this does. Supposedly deprecated? */
#define RPL_UNAWAY				305 /** RFC1459 */
#define RPL_NOWAWAY				306 /** RFC1459 */
#define RPL_WHOISUSER			311 /** RFC1459 */
#define RPL_WHOISSERVER			312 /** RFC1459 */
#define RPL_WHOISOPERATOR		313 /** RFC1459 */
#define RPL_WHOWASUSER			314 /** RFC1459 */
#define RPL_ENDOFWHO			315 /** RFC1459 */
#define RPL_WHOISCHANOP			316 /** RFC1459 */
#define RPL_WHOISIDLE			317 /** RFC1459 */
#define RPL_ENDOFWHOIS			318 /** RFC1459 */
#define RPL_WHOISCHANNELS		319 /** RFC1459 */
#define RPL_WHOISVIRT			320 /** I have no idea what this does. */
#define RPL_WHOIS_HIDDEN		320 /** I have no idea what this does. */
#define RPL_WHOISSPECIAL		320 /** I have no idea what this does. */
#define RPL_LISTSTART			321 /** RFC1459 */
#define RPL_LIST				322 /** RFC1459 */
#define RPL_LISTEND				323 /** RFC1459 */
#define RPL_CHANNELMODEIS		324 /** RFC1459 */
#define RPL_UNIQOPIS			325 /** RFC2812 */
#define RPL_NOCHANPASS			326 /** I have no idea what this does. */
#define RPL_CHPASSUNKNOWN		327 /** I have no idea what this does. */
#define RPL_CHANNEL_URL			328 /** I have no idea what this does. */
#define RPL_CREATIONTIME		329 /** I have no idea what this does. */
#define RPL_NOTOPIC				331 /** RFC1459 */
#define RPL_TOPIC				332 /** RFC1459 */
#define RPL_TOPICWHOTIME		333 /** I have no idea what this does. */
#define RPL_BADCHANPASS			339 /** I have no idea what this does. */
#define RPL_USERIP				340 /** I have no idea what this does. */
#define RPL_INVITING			341 /** RFC1459 */
#define RPL_SUMMONING			342 /** RFC1459 */
#define RPL_INVITED				345 /** I have no idea what this does. */
#define RPL_INVITELIST			346 /** RFC2812 */
#define RPL_ENDOFINVITELIST		347 /** RFC2812 */
#define RPL_EXCEPTLIST			348 /** RFC2812 */
#define RPL_ENDOFEXCEPTLIST		349 /** RFC2812 */
#define RPL_VERSION				351 /** RFC1459 */
#define RPL_WHOREPLY			352 /** RFC1459 */
#define RPL_NAMREPLY			353 /** RFC1459 */
#define RPL_WHOSPCRPL			354 /** I have no idea what this does. */
#define RPL_NAMREPLY_			355 /** I have no idea what this does. */
#define RPL_KILLDONE			361 /** RFC1459 */
#define RPL_CLOSING				362 /** RFC1459 */
#define RPL_CLOSEEND			363 /** RFC1459 */
#define RPL_LINKS				364 /** RFC1459 */
#define RPL_ENDOFLINKS			365 /** RFC1459 */
#define RPL_ENDOFNAMES			366 /** RFC1459 */
#define RPL_BANLIST				367 /** RFC1459 */
#define RPL_ENDOFBANLIST		368 /** RFC1459 */
#define RPL_ENDOFWHOWAS			369 /** RFC1459 */
#define RPL_INFO				371 /** RFC1459 */
#define RPL_MOTD				372 /** RFC1459 */
#define RPL_INFOSTART			373 /** RFC1459 */
#define RPL_ENDOFINFO			374 /** RFC1459 */
#define RPL_MOTDSTART			375 /** RFC1459 */
#define RPL_ENDOFMOTD			376 /** RFC1459 */
#define RPL_YOUREOPER			381 /** RFC1459 */
#define RPL_REHASHING			382 /** RFC1459 */
#define RPL_YOURESERVICE		383 /** RFC2812 */
#define RPL_MYPORTIS			384 /** RFC1459 */
#define RPL_NOTOPERANYMORE		385 /** You are no longer a server operator */
#define RPL_ALIST				388 /** I have no idea what this does. */
#define RPL_ENDOFALIST			389 /** I have no idea what this does. */
#define RPL_TIME				391 /** RFC1459: TIME command response. <server> [possible extension data] :<time string>*/
#define RPL_USERSSTART			392 /** RFC1459 */
#define RPL_USERS				393 /** RFC1459 */
#define RPL_ENDOFUSERS			394 /** RFC1459 */
#define RPL_NOUSERS				395 /** I have no idea what this does. */
#define RPL_HOSTHIDDEN			396 /** Your hostname is now hidden. */
#define ERR_UNKNOWNERROR		400 /** An unknown error occured. */
#define ERR_NOSUCHNICK			401 /** RFC1459 */
#define ERR_NOSUCHSERVER		402 /** RFC1459 */
#define ERR_NOSUCHCHANNEL		403 /** RFC1459 */
#define ERR_CANNOTSENDTOCHAN	404 /** RFC1459 */
#define ERR_TOOMANYCHANNELS		405 /** RFC1459 */
#define ERR_WASNOSUCHNICK		406 /** RFC1459 */
#define ERR_TOOMANYTARGETS		407 /** RFC1459 */
#define ERR_NOSUCHSERVICE		408 /** RFC2812 */
#define ERR_NOORIGIN			409 /** RFC1459 */
#define ERR_NORECIPIENT			411 /** RFC1459 */
#define ERR_NOTEXTTOSEND		412 /** RFC1459 */
#define ERR_NOTOPLEVEL			413 /** RFC1459 */
#define ERR_WILDTOPLEVEL		414 /** RFC1459 */
#define ERR_BADMASK				415 /** RFC2812 */
#define ERR_TOOMANYMATCHES		416 /** I have no idea what this does. */
#define ERR_QUERYTOOLONG		416 /** I have no idea what this does. */
#define ERR_LENGTHTRUNCATED		419 /** I have no idea what this does. */
#define ERR_UNKNOWNCOMMAND		421 /** RFC1459 */
#define ERR_NOMOTD				422 /** RFC1459 */
#define ERR_NOADMININFO			423 /** RFC1459 */
#define ERR_FILEERROR			424 /** RFC1459 */
#define ERR_NOOPERMOTD			425 /** There is no operator MOTD. */
#define ERR_TOOMANYAWAY			429 /** I have no idea what this does. */
#define ERR_EVENTNICKCHANGE		430 /** Can not change nickname due to channel event. */
#define ERR_NONICKNAMEGIVEN		431 /** RFC1459: You didn't specify a nickname. */
#define ERR_ERRONEOUSNICKNAME	432 /** RFC1459: Nickname is reserved or otherwise invalid. */
#define ERR_ERRONEUSNICKNAME	ERR_ERRONEOUSNICKNAME /** Use ERR_EERONEOUSNICKNAME instead. */
#define ERR_NICKNAMEINUSE		433 /** RFC1459: Already used nickname was passed to NICK */
#define ERR_NICKCOLLISION		436 /** RFC1459: A nickname collision was detected. */
#define ERR_UNAVAILRESOURCE		437 /** RFC2812: Target is temporarily unreachable. */
#define ERR_BANNICKCHANGE		437 /** "Ban nick change" */
#define ERR_TARGETTOOFAST		439 /** I have no idea what this does. */
#define RPL_INVTOOFAST			ERR_TARGETTOOFAST
#define RPL_MSGTOOFAST			ERR_TARGETTOOFAST
#define ERR_SERVICESDOWN		440 /** I have no idea what this does. */
#define ERR_USERNOTINCHANNEL	441 /** RFC1459 */
#define ERR_NOTONCHANNEL		442 /** RFC1459 */
#define ERR_USERONCHANNEL		443 /** RFC1459 */
#define ERR_NOLOGIN				444 /** RFC1459 */
#define ERR_SUMMONDISABLED		445 /** RFC1459 */
#define ERR_USERSDISABLED		446 /** RFC1459 */
#define ERR_NONICKCHANGE		447 /** I have no idea what this does. */
#define ERR_NOTIMPLEMENTED		449 /** I have no idea what this does. */
#define ERR_NOTREGISTERED		451 /** RFC1459: You need to be registered. */
#define ERR_IDCOLLISION			452 /** I have no idea what this does. */
#define ERR_NICKLOST			453 /** I have no idea what this does. */
#define ERR_HOSTILENAME			455 /** I have no idea what this does. */
#define ERR_ACCEPTFULL			456 /** I have no idea what this does. */
#define ERR_ACCEPTEXIST			457 /** I have no idea what this does. */
#define ERR_ACCEPTNOT			458 /** I have no idea what this does. */
#define ERR_NOHIDING			459 /** I have no idea what this does. */
#define ERR_NOTFORHALFOPS		460 /** I have no idea what this does. */
#define ERR_NEEDMOREPARAMS		461 /** RFC1459: Command requires more parameters. */
#define ERR_ALREADYREGISTERED	462 /** RFC1459: You're already registered. */
#define ERR_NOPERMFORHOST		463 /** RFC1459: Your host is blocked. */
#define ERR_PASSWDMISMATCH		464 /** RFC1459: Incorrect or missing server password. */
#define ERR_YOUREBANNEDCREEP	465 /** RFC1459: You're banned. */
#define ERR_YOUWILLBEBANNED		466 /** RFC1459: You will soon be banned. */
#define ERR_KEYSET				467 /** RFC1459: A key for this channel has already been set. */
#define ERR_LINKSET				469 /** I have no idea what this does. */
#define ERR_CHANNELISFULL		471 /** RFC1459: That channel is full. */
#define ERR_UNKNOWNMODE			472 /** RFC1459: That mode is unknown. */
#define ERR_INVITEONLYCHAN		473 /** RFC1459: That channel is invite-only. */
#define ERR_BANNEDFROMCHAN		474 /** RFC1459: You're banned from that channel. */
#define ERR_BADCHANNELKEY		475 /** RFC1459: Channel key was missing/incorrect. */
#define ERR_BADCHANMASK			476 /** RFC2812: That channel mask is invalid. */
#define ERR_NOCHANMODES			477 /** RFC2812: Channel does not support mode changes. */
#define ERR_BANLISTFULL			478 /** RFC2812: Channel access/ban list is full. */
#define ERR_BADCHANNAME			479 /** I have no idea what this does. */
#define ERR_LINKFAIL			479 /** I have no idea what this does. */
#define ERR_NOPRIVILEGES		481 /** RFC1459: You do not have the neccessary privledges. */
#define ERR_CHANOPRIVSNEEDED	482 /** RFC1459: You do not have the neccessary channel privledges. */
#define ERR_CANTKILLSERVER		483 /** RFC1459: You can not KILL a server. */
#define ERR_RESTRICTED			484 /** RFC2812: This connection is "restricted" */
#define ERR_UNIQOPRIVSNEEDED	485 /** RFC2812: That mode requires "channel creator" privledges. */
#define ERR_TSLESSCHAN			488 /** I have no idea what this does. */
#define ERR_NOOPERHOST			491 /** RFC1459: You can not become a server operator. */
#define ERR_NOSERVICEHOST		492 /** RFC1459 */
#define ERR_NOFEATURE			493 /** I have no idea what this does. */
#define ERR_BADFEATURE			494 /** I have no idea what this does. */
#define ERR_BADLOGTYPE			495 /** I have no idea what this does. */
#define ERR_BADLOGSYS			496 /** I have no idea what this does. */
#define ERR_BADLOGVALUE			497 /** I have no idea what this does. */
#define ERR_ISOPERLCHAN			498 /** I have no idea what this does. */
#define ERR_CHANOWNPRIVNEEDED	499 /** I have no idea what this does. */
#define ERR_UMODEUNKNOWNFLAG	501 /** RFC1459: This user mode flag is not recognized. */
#define ERR_USERSDONTMATCH		502 /** RFC1459: You can not mody/view other users' modes. */
#define ERR_GHOSTEDCLIENT		503 /** I have no idea what this does. */
#define ERR_USERNOTONSERV		504 /** I have no idea what this does. */
#define ERR_SILELISTFULL		511 /** I have no idea what this does. */
#define ERR_TOOMANYWATCH		512 /** I have no idea what this does. */
#define ERR_BADPING				513 /** Invalid PING response. */
#define ERR_NEEDPONG			ERR_BADPING /** Use ERR_BADPING instead. */
#define ERR_BADEXPIRE			515 /** I have no idea what this does. */
#define ERR_DONTCHEAT			516 /** I have no idea what this does. */
#define ERR_DISABLED			517 /** I have no idea what this does. */
#define ERR_WHOSYNTAX			522 /** I have no idea what this does. */
#define ERR_WHOLIMEXCEED		523 /** I have no idea what this does. */
#define ERR_REMOTEPFX			525 /** I have no idea what this does. */
#define ERR_PFXUNROUTABLE		526 /** I have no idea what this does. */
#define ERR_BADHOSTMASK			550 /** I have no idea what this does. */
#define ERR_HOSTUNAVAIL			551 /** I have no idea what this does. */
#define ERR_USINGSLINE			552 /** I have no idea what this does. */
#define RPL_LOGON				600 /** I have no idea what this does. */
#define RPL_LOGOFF				601 /** I have no idea what this does. */
#define RPL_WATCHOFF			602 /** I have no idea what this does. */
#define RPL_WATCHSTAT			602 /** I have no idea what this does. */
#define RPL_NOWON				604 /** I have no idea what this does. */
#define RPL_NOWOFF				605 /** I have no idea what this does. */
#define RPL_WATCHLIST			606 /** I have no idea what this does. */
#define RPL_ENDOFWATCHLIST		607 /** I have no idea what this does. */
#define RPL_WATCHCLEAR			608 /** I have no idea what this does. */
#define RPL_ISLOCOP				611 /** I have no idea what this does. */
#define RPL_ISNOTOPER			612 /** I have no idea what this does. */
#define RPL_ENDOFISOPER			613 /** I have no idea what this does. */
#define RPL_DCCLIST				618 /** I have no idea what this does. */
#define RPL_OMOTDSTART			624 /** I have no idea what this does. */
#define RPL_OMOTD				625 /** I have no idea what this does. */
#define RPL_ENDOFO				626 /** I have no idea what this does. */
#define RPL_SETTINGS			630 /** I have no idea what this does. */
#define RPL_ENDOFSETTINGS		631 /** I have no idea what this does. */
#define RPL_TRACEROUTE_HOP		660 /** I have no idea what this does. */
#define RPL_TRACEROUTE_START	661 /** I have no idea what this does. */
#define RPL_MODECHANGEWARN		662 /** I have no idea what this does. */
#define RPL_CHANREDIR			663 /** I have no idea what this does. */
#define RPL_SERVMODEIS			664 /** I have no idea what this does. */
#define RPL_OTHERUMODEIS		665 /** I have no idea what this does. */
#define RPL_ENDOF_GENERIC		666 /** I have no idea what this does. */
#define RPL_WHOWASDETAILS		667 /** I have no idea what this does. */
#define RPL_STARTTLS			670 /** Proceed with SSL/TLS initialization */
#define RPL_WHOISSECURE			671 /** User is using a secure connection. */
#define RPL_UNKNOWNMODES		672 /** I have no idea what this does. */
#define RPL_CANNOTSETMODES		673 /** I have no idea what this does. */
#define RPL_LUSERSTAFF			678 /** I have no idea what this does. */
#define RPL_TIMEONSERVERIS		679 /** I have no idea what this does. */
#define RPL_NETWORKS			682 /** I have no idea what this does. */
#define RPL_YOURLANGUAGEIS		687 /** I have no idea what this does. */
#define RPL_LANGUAGE			688 /** I have no idea what this does. */
#define RPL_WHOISSTAFF			689 /** I have no idea what this does. */
#define RPL_WHOISLANGUAGE		690 /** I have no idea what this does. */
#define ERR_STARTTLS			691 /** An error occured setting up SSL/TLS server-side */
#define RPL_MODLIST				702 /** I have no idea what this does. */
#define RPL_ENDOFMODLIST		703 /** I have no idea what this does. */
#define RPL_HELPSTART			704 /** I have no idea what this does. */
#define RPL_HELPTXT				705 /** I have no idea what this does. */
#define RPL_ENDOFHELP			706 /** I have no idea what this does. */
#define RPL_ETRACEFULL			708 /** I have no idea what this does. */
#define RPL_ETRACE				709 /** I have no idea what this does. */
#define RPL_KNOCK				710 /** I have no idea what this does. */
#define RPL_KNOCKDLVR			711 /** I have no idea what this does. */
#define RPL_TOOMANYKNOCK		712 /** I have no idea what this does. */
#define RPL_CHANOPEN			713 /** I have no idea what this does. */
#define RPL_KNOCKONCHAN			714 /** I have no idea what this does. */
#define RPL_KNOCKDISABLED		715 /** I have no idea what this does. */
#define RPL_TARGUMODEG			716 /** I have no idea what this does. */
#define RPL_TARGNOTIFY			717 /** I have no idea what this does. */
#define RPL_UMODEGMSG			718 /** I have no idea what this does. */
//#define RPL_OMOTDSTART			720 /** I have no idea what this does. */
//#define RPL_OMOTD				721 /** I have no idea what this does. */
//#define RPL_ENDOFOMOTD			722 /** I have no idea what this does. */
#define RPL_NOPRIVS				723 /** I have no idea what this does. */
#define RPL_TESTMARK			724 /** I have no idea what this does. */
#define RPL_TESTLINE			725 /** I have no idea what this does. */
#define RPL_NOTESTLINE			726 /** I have no idea what this does. */
#define RPL_XINFO				771 /** I have no idea what this does. */
#define RPL_XINFOSTART			773 /** I have no idea what this does. */
#define RPL_XINFOEND			774 /** I have no idea what this does. */
#define RPL_CANNOTDOCOMMAND		972 /** I have no idea what this does. */
#define RPL_CANNOTCHANGEUMODE	973 /** I have no idea what this does. */
#define RPL_CANNOTCHANGECHANMODE	974 /** I have no idea what this does. */
#define RPL_CANNOTCHANGESERVERMODE	975 /** I have no idea what this does. */
#define RPL_CANNOTSENDTONICK	976 /** I have no idea what this does. */
#define RPL_UNKNOWNSERVERMODE	977 /** I have no idea what this does. */
#define RPL_SERVERMODELOCK		979 /** I have no idea what this does. */
#define RPL_BADCHARENCODING		980 /** I have no idea what this does. */
#define RPL_TOOMANYLANGUAGES	981 /** I have no idea what this does. */
#define RPL_NOLANGUAGE			982 /** I have no idea what this does. */
#define RPL_TEXTTOOSHORT		983 /** I have no idea what this does. */
#define RPL_NUMERIC_ERR			999 /** I have no idea what this does. */

#endif // SHORT_IRC_MACROS

#endif // SKIP_IRC_MACROS

#endif // _IRC_NUMERICS_H_HEADER