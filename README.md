![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)


# Todos:

__Makefile__ :: add usage
- [ ] Run server side:
  - `$ ./ircserv <port> <pass>`
- [ ] Run client side:
  - `$ ./irssi` then `/connect <host> <port> <pass>` or\
  - `$ nc <hostip> <port> <pass>`

__command__ :: `CAP LS` 
- [ ] using nc
- [ ] using irssi


---


# Q&A

### `netcat`
- if no password is entered using
  - will show msg :: "You need to enter a pass!"
  - will close client :: `close( this.m_socket_incoming ) ;`
- if nothing follows a `NICK ` or nick is taken
  - will send RPL `ERR_ERRONEUSNICKNAME`
  - will close client


### NAMES, `/names`, `/n`

- on connection to a channel, w/o explicitly sending /NAMES
  - the client (IRSSI) sends an initial NAMES command to retrieve a list of users in the current channel
  - this is the first thing the client does, even if we don't have the NAMES command
- on receiving a NAMES command from client (IRSSI)
  - Even though our server does not implement the NAMES command, Irssi stills function normally and display the user list because it ignores the error message returned by the server


### Valid port

- `[6660, 6669]`, `6679`, `6697`
- standard port: `6667` (standard port for IRC connection)
- typical range: `[6660, 6669]` or `[6660, 7000]`
- for secure connections
  - 6697 = standard port for SSL-encrypted IRC connections
  - 6679 = an alternative port for SSL connections
- `[*, 1024]` = "well-known ports" reserved for specific services or applications


### About user

anonymous!anonymous@anonymous
- "anonymous" (1st one) refers to the user's Nickname
- "!anonymous" refers to Username
- "@anonymous" refers to Hostname

eg. `john!johndoe@example.com`


### Separator set

- `:` - Used to indicate the start of the message body or the text of a message
- `!` - Used to separate a user's nickname from their username or hostname
- `@` - Used to separate a user's username from their hostname
- `#` - Used to indicate a channel name
- `&` - Used to indicate a channel name for networks that support both channels and groups
- `+` - Used to indicate a mode change to grant a user voice in a channel
- `%` - Used to indicate a mode change to grant a user operator status in a channel
- `.` - Used in some commands to separate the server name from the command or argument


---


# User mode

USER         | client | syntax
:----------- | :----: | :-
&nbsp;       |
__+/- i__    |        | toggle *invisible* a user 
&nbsp;       | irssi  | `/mode user1 +i`
&nbsp;       | &nbsp; | :red_circle: _modified, nothing happens_ (to be deprecated)
&nbsp;       |        | 
__+/- r__    |        | restrict mode
&nbsp;       | irssi  | `/mode user2 -r`
&nbsp;       | 


---


# Channel mode

CHANNEL      | client | syntax
:----------- | :----: | :-
&nbsp;
__oper__     | irssi  | `oper user1 OPER_PASS`
&nbsp;       |        | 
__+/- m__    |        | channel moderation
&nbsp;       | irssi  | `/mode #chan +m`
&nbsp;       |        | `/say something` (user2 can't talk)
&nbsp;       |        | 
__+/- v__    |        | give someone a voice in a moderated room
&nbsp;       | irssi  | `/mode #chan +v user`
&nbsp;       |        | `/say something else` (now user2 can talk)
&nbsp;       |        | 
__+/- a__    |        | anonymous mode (to be deprecated)
&nbsp;       | irssi  | `/mode #channel +a`
&nbsp;       |        | :red_circle: `-a` does not work
&nbsp;       |        | 
__+/- l__    | &nbsp; | set limit
&nbsp;       | irssi  | `/mode #room +l 1`
&nbsp;       |        |
__+/- t__    | &nbsp; | topic settable by channel operator only
&nbsp;       | irssi  | `/mode #room +t`
&nbsp;       |        | 
__+/- o__    |        | grand operator role to someone 
&nbsp;       | irssi  | `/mode #chan +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       |        | 
__+/- n__    |        | (deprecated)
&nbsp;       |        | 
__+/- k__    | &nbsp; | set token/key for room (to be deprecated)
&nbsp;       | irssi  | `/mode #room +k <key>`
&nbsp;       | &nbsp; | :green_circle: works fine w/ a key
&nbsp;       | &nbsp; | :red_circle: still available w/o a key
&nbsp;       |
__+/- b__    | &nbsp; | ban someone (#room cannot send to channel)
&nbsp;       | irssi  | `/mode #room +b user1`
&nbsp;       |


---


# Commands

COMMAND      | client | syntax
:----------- | :----: | :-
&nbsp;
Login        | nc     | `$ nc e1r2p3 6677`
&nbsp;       |        | `PASS password`
&nbsp;       |        | `NICK nick`
&nbsp;       |        | `USER usr * * Usr`
&nbsp;       |        | 
__/DCC__     | irssi  | `/dcc send <~@~user> /mnt/nfs/homes/USR/DIR/log1`
&nbsp;       |        | `/dcc <list\|get>`
&nbsp;       |        | 
__/kill__    | irssi  | `/oper user1 OPER_PASS`, `/kill someone`
&nbsp;       | &nbsp; | ::: in channel: _user still in server_
&nbsp;       | &nbsp; | ::: out of channel: _user disconnected_
&nbsp;       |        |
__/kick__    | &nbsp; | _must be done inside a channel_
&nbsp;       | irssi  | `/oper user1 OPER_PASS` 
&nbsp;       |        | `/kick user2`
&nbsp;       |        | `user2`: only allowed to `/quit` `/exit` once kicked
&nbsp;       |        | cannot `/part`
&nbsp;       | nc     | `OPER user1 OPER_PASS`
&nbsp;       |        | `KICK #room nuo misbehaving`
&nbsp;       | 
__/mode__    | &nbsp; | _channel mode_
&nbsp;       | irssi  | `/oper user1 OPER_PASS`
&nbsp;       |        | `/mode #room +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       |
__/part__    | irssi  | `/part <#room>`
&nbsp;       |        |
__/topic__   | &nbsp; | `/topic` | `topic <topic>`
&nbsp;       | &nbsp; | :red_circle: _Not working_
&nbsp;       | &nbsp; | _::: topic updated only at re-joining_
&nbsp;       | &nbsp; | _::: topic unchanged at quiting_
&nbsp;       | nc     | `TOPIC #room :no`
&nbsp;       |        | `TOPIC #room :yes`
&nbsp;       | 
__/restart__ | irssi  | `/oper user1 OPER_PASS`
&nbsp;       |        | `/restart`
&nbsp;       | 
__/notice__  | irssi  | `/notice @xqin hello, world`
&nbsp;       |        | `/notice #room hello, world` :yellow_circle:
&nbsp;       | &nbsp; | _::: by default a private msg to oneself ..._
&nbsp;       | &nbsp; | _::: and not to the entire channel (to prevent flooding)_
&nbsp;       | nc     | `PRIVMSG #room <msg>`
&nbsp;       | 
__CAP LS__   | nc     | _list the capabilities supported by server_ 
&nbsp;       | 
__/say__     | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/say something`
&nbsp;       | 
__/names__   | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/n` or `/name`
&nbsp;       | nc     | no `NAME #room` for nc :yellow_circle:
&nbsp;       | 
PRIVMSG      | nc     | `PRIVMSG #room hello world`
&nbsp;       |        | `PRIVMSG nuo hi, how r u？`
&nbsp;       | 


