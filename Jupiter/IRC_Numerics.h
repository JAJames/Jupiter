/**
 * Copyright (C) 2013-2017 Jessica James.
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

#if !defined _IRC_NUMERICS_H_HEADER
#define _IRC_NUMERICS_H_HEADER

#include <cstdint>

/**
* These numerics are in large part found here: https://www.alien.net.au/irc/irc2numerics.html
* Not all numerics from the above list are included (generally conflicting numerics such as RPL_MAP).
*/

namespace Jupiter
{
	namespace IRC
	{
		typedef uint_fast16_t NumericType;

		namespace Reply
		{
			constexpr NumericType WELCOME = 1; /** RFC2812: Post-registration welcome message. */
			constexpr NumericType YOURHOST = 2; /** RFC2812: Post-registration. Your host is this server and I am running some daemon */
			constexpr NumericType CREATED = 3; /** RFC2812: Post-registration. This was was created at some point in time */
			constexpr NumericType MYINFO = 4; /** RFC2812: Post-registration. <server_name> <version> <user_modes> <chan_modes> */
			constexpr NumericType BOUNCEOLD = 5; /** RFC2812/DEPRECATED: 005 is rarely used as a bounce indicator, but was defined in RFC2812. */
			constexpr NumericType ISUPPORT = 5; /** Used to indicate what a server supports. Does not appear in any RFC, but was drafted in 2004. */
			constexpr NumericType SNOMASK = 8; /** Server notice mask */
			constexpr NumericType STATMEMTOT = 9; /** I have no idea what this does. */
			constexpr NumericType BOUNCE = 10; /** Bounce server message. Different syntax from IRC_RPL_BOUNCEOLD */
			constexpr NumericType STATMEM = 10; /** I have no idea what this does. */
			constexpr NumericType YOURCOOKIE = 14; /** I have no idea what this does. */
			constexpr NumericType YOURID = 42; /** Informs you of your unique ID. */
			constexpr NumericType SAVENICK = 43; /** Your nick was force-changed due to a nick collision */
			constexpr NumericType ATTEMPTINGJUNC = 50; /** I have no idea what this does. */
			constexpr NumericType ATTEMPTINGREROUTE = 51; /** I have no idea what this does. */

			constexpr NumericType TRACELINK = 200; /** RFC1459: "If the TRACE message is destined for another server, all intermediate servers must return a IRC_RPL_TRACELINK reply to indicate that the TRACE passed through it and where its going next."*/
			constexpr NumericType TRACECONNECTING = 201; /** RFC1459 */
			constexpr NumericType TRACEHANDSHAKE = 202; /** RFC1459 */
			constexpr NumericType TRACEUNKNOWN = 203; /** RFC1459 */
			constexpr NumericType TRACEOPERATOR = 204; /** RFC1459 */
			constexpr NumericType TRACEUSER = 205; /** RFC1459 */
			constexpr NumericType TRACESERVER = 206; /** RFC1459 */
			constexpr NumericType TRACESERVICE = 207; /** RFC2812 */
			constexpr NumericType TRACENEWTYPE = 208; /** RFC1459 */
			constexpr NumericType TRACECLASS = 209; /** RFC2812 */
			constexpr NumericType TRACERECONNECT = 210; /** RFC2812 */
			constexpr NumericType STATS = 210; /** I have no idea what this does. */
			constexpr NumericType STATSLINKINFO = 211; /** RFC1459 */
			constexpr NumericType STATSCOMMANDS = 212; /** RFC1459 */
			constexpr NumericType STATSCLINE = 213; /** RFC1459 */
			constexpr NumericType STATSNLINE = 214; /** RFC1459 */
			constexpr NumericType STATSILINE = 215; /** RFC1459 */
			constexpr NumericType STATSKLINE = 216; /** RFC1459 */
			constexpr NumericType STATSQLINE = 217; /** RFC1459 */
			constexpr NumericType STATSYLINE = 218; /** RFC1459 */
			constexpr NumericType ENDOFSTATS = 219; /** RFC1459 */
			constexpr NumericType UMODEIS = 221; /** RFC1459 */
			constexpr NumericType SERVICEINFO = 231; /** RFC1459 */
			constexpr NumericType ENDOFSERVICES = 232; /** RFC1459 */
			constexpr NumericType SERVICE = 233; /** RFC1459 */
			constexpr NumericType SERVLIST = 234; /** RFC2812 */
			constexpr NumericType STATSVERBOSE = 236; /** I have no idea what this does. */
			constexpr NumericType STATSENGINE = 237; /** I have no idea what this does. */
			constexpr NumericType STATSIAUTH = 239; /** I have no idea what this does. */
			constexpr NumericType STATSLLINE = 241; /** RFC1459 */
			constexpr NumericType STATSUPTIME = 242; /** RFC1459 */
			constexpr NumericType STATSOLINE = 243; /** RFC1459 */
			constexpr NumericType STATSHLINE = 244; /** RFC1459 */
			constexpr NumericType STATSSLINE = 245; /** I have no idea what this does. */
			constexpr NumericType STATSPING = 246; /** RFC2812 */
			constexpr NumericType STATSCONN = 250; /** I have no idea what this does. */
			constexpr NumericType LUSERCLIENT = 251; /** RFC1459 */
			constexpr NumericType LUSEROP = 252; /** RFC1459 */
			constexpr NumericType LUSERUNKNOWN = 253; /** RFC1459 */
			constexpr NumericType LUSERCHANNELS = 254; /** RFC1459 */
			constexpr NumericType LUSERME = 255; /** RFC1459 */
			constexpr NumericType ADMINME = 256; /** RFC1459 */
			constexpr NumericType ADMINLOC1 = 257; /** RFC1459 */
			constexpr NumericType ADMINLOC2 = 258; /** RFC1459 */
			constexpr NumericType ADMINEMAIL = 259; /** RFC1459 */
			constexpr NumericType TRACELOG = 261; /** RFC1459 */
			constexpr NumericType TRACEEND = 262; /** RFC2812 */
			constexpr NumericType TRYAGAIN = 263; /** RFC2812 */
			constexpr NumericType LOCALUSERS = 265; /** I have no idea what this does. */
			constexpr NumericType GLOBALUSERS = 266; /** I have no idea what this does. */
			constexpr NumericType START_NETSTAT = 267; /** I have no idea what this does. */
			constexpr NumericType NETSTAT = 268; /** I have no idea what this does. */
			constexpr NumericType END_NETSTAT = 269; /** I have no idea what this does. */
			constexpr NumericType PRIVS = 270; /** I have no idea what this does. */
			constexpr NumericType SILELIST = 271; /** I have no idea what this does. */
			constexpr NumericType ENDOFSILELIST = 272; /** I have no idea what this does. */
			constexpr NumericType NOTIFY = 273; /** I have no idea what this does. */
			constexpr NumericType VCHANEXIST = 276; /** I have no idea what this does. */
			constexpr NumericType VCHANLIST = 277; /** I have no idea what this does. */
			constexpr NumericType VCHANHELP = 278; /** I have no idea what this does. */
			constexpr NumericType GLIST = 280; /** I have no idea what this does. */
			constexpr NumericType CHANINFO_KICKS = 296; /** I have no idea what this does. */
			constexpr NumericType END_CHANINFO = 299; /** I have no idea what this does. */

			constexpr NumericType NONE = 300; /** RFC1459 */
			constexpr NumericType AWAY = 301; /** RFC1459 */
			constexpr NumericType USERHOST = 302; /** RFC1459 */
			constexpr NumericType ISON = 303; /** RFC1459 */
			constexpr NumericType TEXT = 304; /** I have no idea what this does. Supposedly deprecated? */
			constexpr NumericType UNAWAY = 305; /** RFC1459 */
			constexpr NumericType NOWAWAY = 306; /** RFC1459 */
			constexpr NumericType WHOISUSER = 311; /** RFC1459 */
			constexpr NumericType WHOISSERVER = 312; /** RFC1459 */
			constexpr NumericType WHOISOPERATOR = 313; /** RFC1459 */
			constexpr NumericType WHOWASUSER = 314; /** RFC1459 */
			constexpr NumericType ENDOFWHO = 315; /** RFC1459 */
			constexpr NumericType WHOISCHANOP = 316; /** RFC1459 */
			constexpr NumericType WHOISIDLE = 317; /** RFC1459 */
			constexpr NumericType ENDOFWHOIS = 318; /** RFC1459 */
			constexpr NumericType WHOISCHANNELS = 319; /** RFC1459 */
			constexpr NumericType WHOISVIRT = 320; /** I have no idea what this does. */
			constexpr NumericType WHOIS_HIDDEN = 320; /** I have no idea what this does. */
			constexpr NumericType WHOISSPECIAL = 320; /** I have no idea what this does. */
			constexpr NumericType LISTSTART = 321; /** RFC1459 */
			constexpr NumericType LIST = 322; /** RFC1459 */
			constexpr NumericType LISTEND = 323; /** RFC1459 */
			constexpr NumericType CHANNELMODEIS = 324; /** RFC1459 */
			constexpr NumericType UNIQOPIS = 325; /** RFC2812 */
			constexpr NumericType NOCHANPASS = 326; /** I have no idea what this does. */
			constexpr NumericType CHPASSUNKNOWN = 327; /** I have no idea what this does. */
			constexpr NumericType CHANNEL_URL = 328; /** I have no idea what this does. */
			constexpr NumericType CREATIONTIME = 329; /** I have no idea what this does. */
			constexpr NumericType NOTOPIC = 331; /** RFC1459 */
			constexpr NumericType TOPIC = 332; /** RFC1459 */
			constexpr NumericType TOPICWHOTIME = 333; /** I have no idea what this does. */
			constexpr NumericType BADCHANPASS = 339; /** I have no idea what this does. */
			constexpr NumericType USERIP = 340; /** I have no idea what this does. */
			constexpr NumericType INVITING = 341; /** RFC1459 */
			constexpr NumericType SUMMONING = 342; /** RFC1459 */
			constexpr NumericType INVITED = 345; /** I have no idea what this does. */
			constexpr NumericType INVITELIST = 346; /** RFC2812 */
			constexpr NumericType ENDOFINVITELIST = 347; /** RFC2812 */
			constexpr NumericType EXCEPTLIST = 348; /** RFC2812 */
			constexpr NumericType ENDOFEXCEPTLIST = 349; /** RFC2812 */
			constexpr NumericType VERSION = 351; /** RFC1459 */
			constexpr NumericType WHOREPLY = 352; /** RFC1459 */
			constexpr NumericType NAMREPLY = 353; /** RFC1459 */
			constexpr NumericType WHOSPCRPL = 354; /** I have no idea what this does. */
			constexpr NumericType NAMREPLY_ = 355; /** I have no idea what this does. */
			constexpr NumericType KILLDONE = 361; /** RFC1459 */
			constexpr NumericType CLOSING = 362; /** RFC1459 */
			constexpr NumericType CLOSEEND = 363; /** RFC1459 */
			constexpr NumericType LINKS = 364; /** RFC1459 */
			constexpr NumericType ENDOFLINKS = 365; /** RFC1459 */
			constexpr NumericType ENDOFNAMES = 366; /** RFC1459 */
			constexpr NumericType BANLIST = 367; /** RFC1459 */
			constexpr NumericType ENDOFBANLIST = 368; /** RFC1459 */
			constexpr NumericType ENDOFWHOWAS = 369; /** RFC1459 */
			constexpr NumericType INFO = 371; /** RFC1459 */
			constexpr NumericType MOTD = 372; /** RFC1459 */
			constexpr NumericType INFOSTART = 373; /** RFC1459 */
			constexpr NumericType ENDOFINFO = 374; /** RFC1459 */
			constexpr NumericType MOTDSTART = 375; /** RFC1459 */
			constexpr NumericType ENDOFMOTD = 376; /** RFC1459 */
			constexpr NumericType YOUREOPER = 381; /** RFC1459 */
			constexpr NumericType REHASHING = 382; /** RFC1459 */
			constexpr NumericType YOURESERVICE = 383; /** RFC2812 */
			constexpr NumericType MYPORTIS = 384; /** RFC1459 */
			constexpr NumericType NOTOPERANYMORE = 385; /** You are no longer a server operator */
			constexpr NumericType ALIST = 388; /** I have no idea what this does. */
			constexpr NumericType ENDOFALIST = 389; /** I have no idea what this does. */
			constexpr NumericType TIME = 391; /** RFC1459: TIME command response. <server> [possible extension data] :<time string>*/
			constexpr NumericType USERSSTART = 392; /** RFC1459 */
			constexpr NumericType USERS = 393; /** RFC1459 */
			constexpr NumericType ENDOFUSERS = 394; /** RFC1459 */
			constexpr NumericType NOUSERS = 395; /** I have no idea what this does. */
			constexpr NumericType HOSTHIDDEN = 396; /** Your hostname is now hidden. */


			constexpr NumericType LOGON = 600; /** I have no idea what this does. */
			constexpr NumericType LOGOFF = 601; /** I have no idea what this does. */
			constexpr NumericType WATCHOFF = 602; /** I have no idea what this does. */
			constexpr NumericType WATCHSTAT = 602; /** I have no idea what this does. */
			constexpr NumericType NOWON = 604; /** I have no idea what this does. */
			constexpr NumericType NOWOFF = 605; /** I have no idea what this does. */
			constexpr NumericType WATCHLIST = 606; /** I have no idea what this does. */
			constexpr NumericType ENDOFWATCHLIST = 607; /** I have no idea what this does. */
			constexpr NumericType WATCHCLEAR = 608; /** I have no idea what this does. */
			constexpr NumericType ISLOCOP = 611; /** I have no idea what this does. */
			constexpr NumericType ISNOTOPER = 612; /** I have no idea what this does. */
			constexpr NumericType ENDOFISOPER = 613; /** I have no idea what this does. */
			constexpr NumericType DCCLIST = 618; /** I have no idea what this does. */
			constexpr NumericType OMOTDSTART = 624; /** I have no idea what this does. */
			constexpr NumericType OMOTD = 625; /** I have no idea what this does. */
			constexpr NumericType ENDOFO = 626; /** I have no idea what this does. */
			constexpr NumericType SETTINGS = 630; /** I have no idea what this does. */
			constexpr NumericType ENDOFSETTINGS = 631; /** I have no idea what this does. */
			constexpr NumericType TRACEROUTE_HOP = 660; /** I have no idea what this does. */
			constexpr NumericType TRACEROUTE_START = 661; /** I have no idea what this does. */
			constexpr NumericType MODECHANGEWARN = 662; /** I have no idea what this does. */
			constexpr NumericType CHANREDIR = 663; /** I have no idea what this does. */
			constexpr NumericType SERVMODEIS = 664; /** I have no idea what this does. */
			constexpr NumericType OTHERUMODEIS = 665; /** I have no idea what this does. */
			constexpr NumericType ENDOF_GENERIC = 666; /** I have no idea what this does. */
			constexpr NumericType WHOWASDETAILS = 667; /** I have no idea what this does. */
			constexpr NumericType STARTTLS = 670; /** Proceed with SSL/TLS initialization */
			constexpr NumericType WHOISSECURE = 671; /** User is using a secure connection. */
			constexpr NumericType UNKNOWNMODES = 672; /** I have no idea what this does. */
			constexpr NumericType CANNOTSETMODES = 673; /** I have no idea what this does. */
			constexpr NumericType LUSERSTAFF = 678; /** I have no idea what this does. */
			constexpr NumericType TIMEONSERVERIS = 679; /** I have no idea what this does. */
			constexpr NumericType NETWORKS = 682; /** I have no idea what this does. */
			constexpr NumericType YOURLANGUAGEIS = 687; /** I have no idea what this does. */
			constexpr NumericType LANGUAGE = 688; /** I have no idea what this does. */
			constexpr NumericType WHOISSTAFF = 689; /** I have no idea what this does. */
			constexpr NumericType WHOISLANGUAGE = 690; /** I have no idea what this does. */

			constexpr NumericType MODLIST = 702; /** I have no idea what this does. */
			constexpr NumericType ENDOFMODLIST = 703; /** I have no idea what this does. */
			constexpr NumericType HELPSTART = 704; /** I have no idea what this does. */
			constexpr NumericType HELPTXT = 705; /** I have no idea what this does. */
			constexpr NumericType ENDOFHELP = 706; /** I have no idea what this does. */
			constexpr NumericType ETRACEFULL = 708; /** I have no idea what this does. */
			constexpr NumericType ETRACE = 709; /** I have no idea what this does. */
			constexpr NumericType KNOCK = 710; /** I have no idea what this does. */
			constexpr NumericType KNOCKDLVR = 711; /** I have no idea what this does. */
			constexpr NumericType TOOMANYKNOCK = 712; /** I have no idea what this does. */
			constexpr NumericType CHANOPEN = 713; /** I have no idea what this does. */
			constexpr NumericType KNOCKONCHAN = 714; /** I have no idea what this does. */
			constexpr NumericType KNOCKDISABLED = 715; /** I have no idea what this does. */
			constexpr NumericType TARGUMODEG = 716; /** I have no idea what this does. */
			constexpr NumericType TARGNOTIFY = 717; /** I have no idea what this does. */
			constexpr NumericType UMODEGMSG = 718; /** I have no idea what this does. */
			//constexpr NumericType OMOTDSTART = 720; /** I have no idea what this does. */
			//constexpr NumericType OMOTD = 721; /** I have no idea what this does. */
			//constexpr NumericType ENDOFOMOTD = 722; /** I have no idea what this does. */
			constexpr NumericType NOPRIVS = 723; /** I have no idea what this does. */
			constexpr NumericType TESTMARK = 724; /** I have no idea what this does. */
			constexpr NumericType TESTLINE = 725; /** I have no idea what this does. */
			constexpr NumericType NOTESTLINE = 726; /** I have no idea what this does. */
			constexpr NumericType XINFO = 771; /** I have no idea what this does. */
			constexpr NumericType XINFOSTART = 773; /** I have no idea what this does. */
			constexpr NumericType XINFOEND = 774; /** I have no idea what this does. */

			constexpr NumericType CANNOTDOCOMMAND = 972; /** I have no idea what this does. */
			constexpr NumericType CANNOTCHANGEUMODE = 973; /** I have no idea what this does. */
			constexpr NumericType CANNOTCHANGECHANMODE = 974; /** I have no idea what this does. */
			constexpr NumericType CANNOTCHANGESERVERMODE = 975; /** I have no idea what this does. */
			constexpr NumericType CANNOTSENDTONICK = 976; /** I have no idea what this does. */
			constexpr NumericType UNKNOWNSERVERMODE = 977; /** I have no idea what this does. */
			constexpr NumericType SERVERMODELOCK = 979; /** I have no idea what this does. */
			constexpr NumericType BADCHARENCODING = 980; /** I have no idea what this does. */
			constexpr NumericType TOOMANYLANGUAGES = 981; /** I have no idea what this does. */
			constexpr NumericType NOLANGUAGE = 982; /** I have no idea what this does. */
			constexpr NumericType TEXTTOOSHORT = 983; /** I have no idea what this does. */
			constexpr NumericType NUMERIC_ERR = 999; /** I have no idea what this does. */
		}
		namespace Error
		{
			constexpr NumericType UNKNOWNERROR = 400; /** An unknown error occured. */
			constexpr NumericType NOSUCHNICK = 401; /** RFC1459 */
			constexpr NumericType NOSUCHSERVER = 402; /** RFC1459 */
			constexpr NumericType NOSUCHCHANNEL = 403; /** RFC1459 */
			constexpr NumericType CANNOTSENDTOCHAN = 404; /** RFC1459 */
			constexpr NumericType TOOMANYCHANNELS = 405; /** RFC1459 */
			constexpr NumericType WASNOSUCHNICK = 406; /** RFC1459 */
			constexpr NumericType TOOMANYTARGETS = 407; /** RFC1459 */
			constexpr NumericType NOSUCHSERVICE = 408; /** RFC2812 */
			constexpr NumericType NOORIGIN = 409; /** RFC1459 */
			constexpr NumericType NORECIPIENT = 411; /** RFC1459 */
			constexpr NumericType NOTEXTTOSEND = 412; /** RFC1459 */
			constexpr NumericType NOTOPLEVEL = 413; /** RFC1459 */
			constexpr NumericType WILDTOPLEVEL = 414; /** RFC1459 */
			constexpr NumericType BADMASK = 415; /** RFC2812 */
			constexpr NumericType TOOMANYMATCHES = 416; /** I have no idea what this does. */
			constexpr NumericType QUERYTOOLONG = 416; /** I have no idea what this does. */
			constexpr NumericType LENGTHTRUNCATED = 419; /** I have no idea what this does. */
			constexpr NumericType UNKNOWNCOMMAND = 421; /** RFC1459 */
			constexpr NumericType NOMOTD = 422; /** RFC1459 */
			constexpr NumericType NOADMININFO = 423; /** RFC1459 */
			constexpr NumericType FILEERROR = 424; /** RFC1459 */
			constexpr NumericType NOOPERMOTD = 425; /** There is no operator MOTD. */
			constexpr NumericType TOOMANYAWAY = 429; /** I have no idea what this does. */
			constexpr NumericType EVENTNICKCHANGE = 430; /** Can not change nickname due to channel event. */
			constexpr NumericType NONICKNAMEGIVEN = 431; /** RFC1459: You didn't specify a nickname. */
			constexpr NumericType ERRONEOUSNICKNAME = 432; /** RFC1459: Nickname is reserved or otherwise invalid. */
			constexpr NumericType NICKNAMEINUSE = 433; /** RFC1459: Already used nickname was passed to NICK */
			constexpr NumericType NICKCOLLISION = 436; /** RFC1459: A nickname collision was detected. */
			constexpr NumericType UNAVAILRESOURCE = 437; /** RFC2812: Target is temporarily unreachable. */
			constexpr NumericType BANNICKCHANGE = 437; /** "Ban nick change" */
			constexpr NumericType TARGETTOOFAST = 439; /** I have no idea what this does. */
			constexpr NumericType SERVICESDOWN = 440; /** I have no idea what this does. */
			constexpr NumericType USERNOTINCHANNEL = 441; /** RFC1459 */
			constexpr NumericType NOTONCHANNEL = 442; /** RFC1459 */
			constexpr NumericType USERONCHANNEL = 443; /** RFC1459 */
			constexpr NumericType NOLOGIN = 444; /** RFC1459 */
			constexpr NumericType SUMMONDISABLED = 445; /** RFC1459 */
			constexpr NumericType USERSDISABLED = 446; /** RFC1459 */
			constexpr NumericType NONICKCHANGE = 447; /** I have no idea what this does. */
			constexpr NumericType NOTIMPLEMENTED = 449; /** I have no idea what this does. */
			constexpr NumericType NOTREGISTERED = 451; /** RFC1459: You need to be registered. */
			constexpr NumericType IDCOLLISION = 452; /** I have no idea what this does. */
			constexpr NumericType NICKLOST = 453; /** I have no idea what this does. */
			constexpr NumericType HOSTILENAME = 455; /** I have no idea what this does. */
			constexpr NumericType ACCEPTFULL = 456; /** I have no idea what this does. */
			constexpr NumericType ACCEPTEXIST = 457; /** I have no idea what this does. */
			constexpr NumericType ACCEPTNOT = 458; /** I have no idea what this does. */
			constexpr NumericType NOHIDING = 459; /** I have no idea what this does. */
			constexpr NumericType NOTFORHALFOPS = 460; /** I have no idea what this does. */
			constexpr NumericType NEEDMOREPARAMS = 461; /** RFC1459: Command requires more parameters. */
			constexpr NumericType ALREADYREGISTERED = 462; /** RFC1459: You're already registered. */
			constexpr NumericType NOPERMFORHOST = 463; /** RFC1459: Your host is blocked. */
			constexpr NumericType PASSWDMISMATCH = 464; /** RFC1459: Incorrect or missing server password. */
			constexpr NumericType YOUREBANNEDCREEP = 465; /** RFC1459: You're banned. */
			constexpr NumericType YOUWILLBEBANNED = 466; /** RFC1459: You will soon be banned. */
			constexpr NumericType KEYSET = 467; /** RFC1459: A key for this channel has already been set. */
			constexpr NumericType LINKSET = 469; /** I have no idea what this does. */
			constexpr NumericType CHANNELISFULL = 471; /** RFC1459: That channel is full. */
			constexpr NumericType UNKNOWNMODE = 472; /** RFC1459: That mode is unknown. */
			constexpr NumericType INVITEONLYCHAN = 473; /** RFC1459: That channel is invite-only. */
			constexpr NumericType BANNEDFROMCHAN = 474; /** RFC1459: You're banned from that channel. */
			constexpr NumericType BADCHANNELKEY = 475; /** RFC1459: Channel key was missing/incorrect. */
			constexpr NumericType BADCHANMASK = 476; /** RFC2812: That channel mask is invalid. */
			constexpr NumericType NOCHANMODES = 477; /** RFC2812: Channel does not support mode changes. */
			constexpr NumericType BANLISTFULL = 478; /** RFC2812: Channel access/ban list is full. */
			constexpr NumericType BADCHANNAME = 479; /** I have no idea what this does. */
			constexpr NumericType LINKFAIL = 479; /** I have no idea what this does. */
			constexpr NumericType NOPRIVILEGES = 481; /** RFC1459: You do not have the neccessary privledges. */
			constexpr NumericType CHANOPRIVSNEEDED = 482; /** RFC1459: You do not have the neccessary channel privledges. */
			constexpr NumericType CANTKILLSERVER = 483; /** RFC1459: You can not KILL a server. */
			constexpr NumericType RESTRICTED = 484; /** RFC2812: This connection is "restricted" */
			constexpr NumericType UNIQOPRIVSNEEDED = 485; /** RFC2812: That mode requires "channel creator" privledges. */
			constexpr NumericType TSLESSCHAN = 488; /** I have no idea what this does. */
			constexpr NumericType NOOPERHOST = 491; /** RFC1459: You can not become a server operator. */
			constexpr NumericType NOSERVICEHOST = 492; /** RFC1459 */
			constexpr NumericType NOFEATURE = 493; /** I have no idea what this does. */
			constexpr NumericType BADFEATURE = 494; /** I have no idea what this does. */
			constexpr NumericType BADLOGTYPE = 495; /** I have no idea what this does. */
			constexpr NumericType BADLOGSYS = 496; /** I have no idea what this does. */
			constexpr NumericType BADLOGVALUE = 497; /** I have no idea what this does. */
			constexpr NumericType ISOPERLCHAN = 498; /** I have no idea what this does. */
			constexpr NumericType CHANOWNPRIVNEEDED = 499; /** I have no idea what this does. */

			constexpr NumericType UMODEUNKNOWNFLAG = 501; /** RFC1459: This user mode flag is not recognized. */
			constexpr NumericType USERSDONTMATCH = 502; /** RFC1459: You can not mody/view other users' modes. */
			constexpr NumericType GHOSTEDCLIENT = 503; /** I have no idea what this does. */
			constexpr NumericType USERNOTONSERV = 504; /** I have no idea what this does. */
			constexpr NumericType SILELISTFULL = 511; /** I have no idea what this does. */
			constexpr NumericType TOOMANYWATCH = 512; /** I have no idea what this does. */
			constexpr NumericType BADPING = 513; /** Invalid PING response. */
			constexpr NumericType BADEXPIRE = 515; /** I have no idea what this does. */
			constexpr NumericType DONTCHEAT = 516; /** I have no idea what this does. */
			constexpr NumericType DISABLED = 517; /** I have no idea what this does. */
			constexpr NumericType WHOSYNTAX = 522; /** I have no idea what this does. */
			constexpr NumericType WHOLIMEXCEED = 523; /** I have no idea what this does. */
			constexpr NumericType REMOTEPFX = 525; /** I have no idea what this does. */
			constexpr NumericType PFXUNROUTABLE = 526; /** I have no idea what this does. */
			constexpr NumericType BADHOSTMASK = 550; /** I have no idea what this does. */
			constexpr NumericType HOSTUNAVAIL = 551; /** I have no idea what this does. */
			constexpr NumericType USINGSLINE = 552; /** I have no idea what this does. */

			constexpr NumericType STARTTLS = 691; /** An error occured setting up SSL/TLS server-side */
		}
	}
}

#endif // _IRC_NUMERICS_H_HEADER