![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)


# Now checkout 

CHANNEL M.   | client | syntax
:----------- | :----: | :-
&nbsp;       |        | &nbsp;
__oper__     | irssi  | `oper user1 OPER_PW`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- o__    |        | grand Operator role to someone 
&nbsp;       | irssi  | `/mode #chan +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       | nc     | 
&nbsp;       |        | 
&nbsp;       |        | &nbsp;
__+/- k__    | &nbsp; | set token/key for room
&nbsp;       | irssi  | `/mode #room +k <key>`
&nbsp;       | &nbsp; | :green_circle: works fine w/ a key
&nbsp;       | &nbsp; | :red_circle: still available w/o a key
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- l__    | &nbsp; | set limit
&nbsp;       | irssi  | `/mode #room +l 1`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- b__    | &nbsp; | ban someone (#room cannot send to channel)
&nbsp;       | irssi  | `/mode #room +b user1`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;


USER MODE    | client | syntax
:----------- | :----: | :-
&nbsp;       |        | &nbsp;
__+/- i__    |        | toggle *invisible* for user
&nbsp;       | irssi  | `/mode user1 +i`
&nbsp;       | &nbsp; | :red_circle: _modified, nothing happens_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;



COMMAND      | client | syntax
:----------- | :----- | :-
&nbsp;       | &nbsp; | &nbsp;
Login        | nc     | `$ nc e1r2p3 6677`
&nbsp;       |        | `PASS password`
&nbsp;       |        | `NICK nick`
&nbsp;       |        | `USER user`
&nbsp;       | &nbsp; | &nbsp;
__/DCC__     | irssi  | `/dcc send <~@~user> /mnt/nfs/homes/USR/DIR/log1`
&nbsp;       |        | `/dcc <list\|get>`
&nbsp;       | &nbsp; | _inside channel: user still in server_
&nbsp;       | &nbsp; | _outside channel: user disconnected_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/kill__    | irssi | `/oper user1 OPER_PW`, `/kill someone`
&nbsp;       | &nbsp; | in channel: _user still in server_
&nbsp;       | &nbsp; | out of channel: _user disconnected_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/kick__    | &nbsp; | _must be done inside a channel_
&nbsp;       | irssi  | `/oper user1 OPER_PW` 
&nbsp;       |        | `/kick user2`
&nbsp;       |        | `user2`: only allowed to `/quit` `/exit` once kicked
&nbsp;       |        | cannot `/part`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/mode__    | &nbsp; | _channel mode_
&nbsp;       | irssi  | `/oper user1 OPER_PW`
&nbsp;       |        | `/mode #room +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/part__    | irssi  | `/part <#room>`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/topic__   | &nbsp; | `/topic` / `topic <topic>`
&nbsp;       | &nbsp; | :red_circle: _Not working_
&nbsp;       | &nbsp; | _topic updated only at re-joining_
&nbsp;       | &nbsp; | _topic unchanged at quiting_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/restart__ | irssi  | `/oper user1 OPER_PW`
&nbsp;       |        | `/restart`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/notice__  | irssi  | `/notice @xqin hello, world`
&nbsp;       |        | `/notice #room hello, world` :yellow_circle:
&nbsp;       | &nbsp; | _by default a private msg to oneself ..._
&nbsp;       | &nbsp; | _and not to the entire channel (to prevent flooding)_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/notice__  | nc     | `PRIVMSG #room <msg>`
&nbsp;       | &nbsp; | &nbsp;
__/say__     | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/say something`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/names__   | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/n` or `/name`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;



# Now checking
- [ ] ~Do-the-f*ing-Math Bot~
- [ ] ~Weekday-guessing Bot~
- Use netcat ---> `nc localhost [port]`
  - `$ nc <Seat> <Port>`
  - `PASS !@#$` - `NICK qxnx` - `USER qxnx * * qxnx`
  - `JOIN #room` - `PART`
  - `/part` `/join` ---> lowercase = NO
- [ ] Date & Time Bot
- [ ] Read EVAL.pdf again
- [:white_check_mark:] Find all RPL ( --- Home assignment)
- [:white_check_mark:] /NOTICE | notice
  - `/notice #room hello, world` (not for irssi) --> nc `PRIVMSG #room <msg>`
  - `/notice @n2 hello, world`
- [:white_check_mark:] /DCC | dcc
  - *Update* [:white_check_mark:] - qxia successfully sent a _Fmt.hpp_ to nuo
  - `/dcc send <~@~User> /mnt/nfs/homes/login/Downloads/Great.in`
  - `/dcc <list|get>`
- [:white_check_mark:] XCQ after ***get_input_from_client_sfd (sfd)***
- [:white_check_mark:] Candle 7
- [:white_check_mark:] Modify all for loops
- [:white_check_mark:] Re-add french MOTD


# "roi" (User)
- [:white_check_mark:] `+r` | set myself to `restricted` - avoid messages from unidentified users
- [:white_check_mark:] `+o` | set operator | Syntax <kbd> $ /mode #ROOM +o USER </kbd>
- [:white_check_mark:] `+i` 


# "maltvonkb" (Channel)
- [ ] `+m` | set moderation |  `/mode #chan +m`
- [ ] `+a` | auto-voice | +a users are auto-granted voice | `/mode #chan +a user` 
- [x] `+l` | set limit | `/oper ...` ` /mode #ROOM +l 2`
- [ ] `+t` | topic protection | `/mode #chan +t `
- [ ] `+v` | give _user_ a voice in a moderated room |  `/mode #chan +v user`
- [ ] `+o` | give someone _Operator_ role | `/mode #chan +o user`
- [ ] `+n` | no external messages |  `/mode #chan +n`
- [ ] `+k` | :red_circle::red_circle::red_circle: set chan key | `/oper ...` ` /mode #ROOM +k KEY`   
- [x] `+b` | ban someone | `/oper ...` ` /mode #ROOM +b USER` 


# Command List 
- [x] oper
- [ ] privmsg
- [ ] ping
  - dropped /pong (incompatible w/ irssi)
- [ ] part
- [ ] topic
- [x] quit
- [x] kick
- [x] kill
- [x] restart
- [x] n/name/names (builtin)


# Command List II
- [x] TOPIC works fine
  - `/topic` 
  - `/topic <new_topic>` (anyone can set topic)
- [x] RESTART works fine
  - `/oper user1 ####` 
  - `/restart`
  - <kbd> ALT + LEFT Arrow </kbd> 
- [x] KILL works fine
  - `/oper user1 ####`
  - `/kill n2` 
- [x] OPER, MODE + KICK works fine
  - `/oper user1 ####`
  - `/mode #room +o n2`
  - `/kick user1` (n2) 



# IRC Mysteries
- `anonymous!anonymous@anonymous`
  - The 1st "anonymous" refers to the user's nickname
  - The 2nd "anonymous" refers to the username
  - The 3rd "anonymous" refers to the hostname
  - eg. `john!johndoe@example.com`
- Valid port: `(port >= 6660 && port <= 6669) || port == 6679 || port == 6697`
  - The standard port for IRC connection is 6667
  - More often, IRC networks use a range instead of just one
  - A typical range is [6660, 6669] or [6660, 7000]
  - 6679 and 6697 are used for secure IRC connections
    - 6697 = standard port for SSL-encrypted IRC connections
    - 6679 = an alternative port for SSL connections
  - [*, 1024] = "well-known ports" reserved for specific services or applications
- Check the very beginning: from freenode/main to 
  - **Server	MY_SERVER( port, password ) ;** `
  - **MY_SERVER.connect_to_server() ;** 
- MY_SERVER.connect_to_server() ;
  - `1` **this->m_server_socket = Server::new_socket() ;**
    - **int sockt = socket(AF_INET, SOCK_STREAM, 0);** // create socket
    - **setsockopt(sockt, SOL_SOCKET, SO_REUSEADDR, & (int) tmp, sizeof( (int) tmp)) ;** // set up socket // ? opt??
    - **fcntl(sockt, F_SETFL, O_NONBLOCK)** // set socket to non-blocking mode
      - m_server.sin_addr.s_addr = INADDR_ANY
      - m_server.sin_port = htons( (unsigned short) (std::strtoul( m_port.c_str(), NULL, 0 )))
      - m_server.sin_family = AF_INET
    - **bind(sockt, (const struct sockaddr * )& m_server, sizeof(m_server))** // binding socket
    - **listen( sockt, MAX_CLIENTS /* 10 */ )** // listening on socket
  - _"listening..."_
  - `2` **while SERVER == alive**
  `handle SIGINT` `FD_ZERO(& rfds)` `FD_SET( m_server_socket, & rfds )`
  find `max_sockt_fd` (highest file descriptor) for `select`
  - **activity = select( Highest_Socket_FD + 1, & rfds, None, None, None)**
    - **if (FD_ISSET(this->m_server_socket, & rfds)**
      - **Server::new_connection()** wait for new conns
    - else
      - **buf = receive_msg(g_server_client_socket[ i ])** loop through G_Client_Sockets and receive msg/cmds
    - Here we XCQ the any command carried in the input
  - ...





<!--

# Basic checks
- Check poll() (or equivalent) 
  - only one
  - called every time before accept, read, write etc.
  - no errno
- Verify fnctl()
  - use of fcntl(fd, F_SETFL, O_NONBLOCK)

# Networking 1
- Use 'nc'
  - connect to the server
  - send commands
  - the server answers back
- From now on, use our reference IRC client
  - try multiple clients
  - test with `IRSSI` and `nc` at the same time
- Join a channel
   - messages are sent to all

# Networking 2
- Use `nc` to send partial commands
... What are partial commands ? (🟡)
  - the server answers correctly
  - other connections run fine
- Unexpectedly kill a client
  - check if server still works and can still get new client
- (🟡) Kill a `nc` with half of a command sent
- Stop a client (^-Z)
  - Flood the channel using another client
    - Server should not hang
  - Check leak


# Client side
- Use `nc` to perform
  - authenticate (🟡)
  - set nick
  - set username
  - join chan
- Use `irssi` to perform
  - authenticate
  - set nick
  - set username
  - join chan
- Run `/PRIVMSG` with different parameters
- Run `/NOTICE` with different parameters
    - work with different parameters
- Check that a regular user cannot do operator actions
- Check that a ___chanop___ can 
- (🟡) All the channel operations should be tested  


# File transfer
- File transfer with `irssi` (🟡)

# A small bot
- ___Todo___

-->
