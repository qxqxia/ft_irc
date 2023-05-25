![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)


# Subject change:

![](https://cdn.discordapp.com/attachments/774304974862549032/1103301987082195106/image.png)


# Todos


__moderated room__ :: to retry m mode 
- (todo)


__a tiny bot__ :: to try with partner
- (todo)


__Makefile__ :: add usage
- [ ] Run server side:
  - `$ ./ircserv <port> <pass>`
- [ ] Run client side:
  - Irssi - `$ ./irssi` then `/connect <host> <port> <pass>`
  - Netcat - `$ nc <hostip> <port>` `PASS _`




---


# User mode
<kbd> {[+|-] |r|o} </kbd>

USER modes                | syntax
------------------------: | :----
&nbsp;                    |
**r** - Restrict mode     |
`irssi`                   | `/mode user2 -r`
&nbsp;                    |
**o** - Activate OP role  |
`irssi`                   | `/oper user1 OPER_PASS`
&nbsp;                    |


---


# Channel mode

CHANNEL modes             | syntax
------------------------: | :----
&nbsp;                    |
__m mode__                | channel moderation
`irssi`                   | `/mode #chan +m`
&nbsp;                    | `/say something` (user2 can't talk)
&nbsp;                    |
__v mode__                | voice in +m room 
`irssi`                   | `/mode #chan +v user`
&nbsp;                    | `/say something else` (now user2 can talk)
&nbsp;                    |
__b mode__                | activate banlist
`irssi`                   | `/mode #room +b user1`
&nbsp;                    | will display `#room cannot send to channel` to user banned
&nbsp;                    |
__l mode__                | set limit
`irssi`                   |
&nbsp;                    |`/mode #room +l 1`
&nbsp;                    |
__o mode__                | promote user to +o
`irssi`                   | `/mode #chan +o user1`
&nbsp;                    |`/kick user2` (user1)
&nbsp;                    |

---


# Commands


Commands                  | syntax
------------------------: | :----
Login                     | login
`nc`                      | `$ nc e1r2p3 6677`
&nbsp;                    | `PASS password`
&nbsp;                    | `NICK nick`
&nbsp;                    | `USER usr * * Usr`
&nbsp;                    |
__/DCC__                  | send files
`irssi`                   | `/dcc send <~@~user> /mnt/nfs/homes/USR/DIR/log1`
&nbsp;                    | `/dcc <list\|get>`
&nbsp;                    |
__/kill__                 | make someone leave Channel or Server
`irssi`                   | `/oper user1 OPER_PASS`, `/kill someone`
&nbsp;                    | ___in___ channel: _user parts chan, but will stay in server_
&nbsp;                    | ___ex___ channel: _user disconnected_
&nbsp;                    |
__/kick__                 | ___kicking___ must be done inside a room
`irssi`                   | `/oper user1 OPER_PASS` 
&nbsp;                    | `/kick user2`
&nbsp;                    | `user2`: only allowed to `/quit` `/exit` once kicked
&nbsp;                    | cannot `/part`
`nc`                      | `OPER user1 OPER_PASS`
&nbsp;                    | `KICK #room nuo misbehaving`
&nbsp;                    |
__/mode__                 | enbale channel mode 
`irssi`                   | `/oper user1 OPER_PASS`
&nbsp;                    | `/mode #room +o user1`
&nbsp;                    | `/kick user2` (user1)
&nbsp;                    |
__/part__                 |
`irssi` :heavy_check_mark:| `/part #room` . the <kbd>#</kbd> is indispensable
&nbsp;                    | `/part #42,#43,#44,#45`
&nbsp;                    |
`nc` :heavy_check_mark:   | `PART #42,#43,#44`
&nbsp;                    | > `:nuo!nuo@* PART #42` `...#43` `...#44`
&nbsp;                    |
__/restart__              | 
`irssi`                   | `/oper user1 OPER_PASS`
&nbsp;                    | `/restart`
&nbsp;                    |
__/notice__               | 
`irssi`                   | `/notice @xqin hello, world`
&nbsp;                    | `/notice #room hello, world` :yellow_circle:
&nbsp;                    | _::: by default a private msg to oneself ..._
&nbsp;                    | _::: and not to the entire channel (to prevent flooding)_
`nc`                      | `PRIVMSG #room <msg>`
&nbsp;                    |
__CAP LS__                | _list the capabilities supported by server_ 
`nc`                      | 
&nbsp;                    |
__/say__                  | (IRSSI built-in)
`irssi`                   | `/say something`
&nbsp;                    |
__/names__                | (IRSSI built-in)
&nbsp;                    | 
`irssi`                   | `/n` or `/name`
&nbsp;                    | no `NAME #room` for nc :yellow_circle:
&nbsp;                    |
__PRIVMSG__               |
`nc`                      | `PRIVMSG #room hello world`
&nbsp;                    | `PRIVMSG nuo hi, how r u？`
&nbsp;                    |


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
