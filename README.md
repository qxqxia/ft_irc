![](https://img.shields.io/badge/C++-Internet&nbsp;Relay&nbsp;Chat-red.svg?style=flat&logo=c%2B%2B)


# User mode

USER         | client | syntax
:----------- | :----: | :-
&nbsp;       |
__+/- i__    |        | toggle *invisible* a user 
&nbsp;       | irssi  | `/mode user1 +i`
&nbsp;       | &nbsp; | :red_circle: _modified, nothing happens_ (to be deprecated)
&nbsp;       | &nbsp; | &nbsp;
__+/- r__    |        | restrict mode
&nbsp;       | irssi  | `/mode user2 -r`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;

# Channel mode

CHANNEL      | client | syntax
:----------- | :----: | :-
&nbsp;
__oper__     | irssi  | `oper user1 OPER_PASS`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- m__    |        | channel moderation
&nbsp;       | irssi  | `/mode #chan +m`
&nbsp;       |        | `/say something` (user2 can't talk)
&nbsp;       | nc     | 
&nbsp;       |        | 
&nbsp;       |
__+/- v__    |        | give someone a voice in a moderated room
&nbsp;       | irssi  | `/mode #chan +v user`
&nbsp;       |        | `/say something else` (now user2 can talk)
&nbsp;       | nc     | 
&nbsp;       |        | 
&nbsp;       |
__+/- a__    |        | anonymous mode (to be deprecated)
&nbsp;       | irssi  | `/mode #channel +a`
&nbsp;       |        | :red_circle: `-a` does not work
&nbsp;       | nc     | 
&nbsp;       |        | 
&nbsp;       |
__+/- l__    | &nbsp; | set limit
&nbsp;       | irssi  | `/mode #room +l 1`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- t__    | &nbsp; | topic settable by channel operator only
&nbsp;       | irssi  | `/mode #room +t`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- o__    |        | grand operator role to someone 
&nbsp;       | irssi  | `/mode #chan +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       | nc     | 
&nbsp;       |        | 
&nbsp;       |
__+/- n__    |        | (deprecated)
&nbsp;       |        | 
&nbsp;       |
__+/- k__    | &nbsp; | set token/key for room (to be deprecated)
&nbsp;       | irssi  | `/mode #room +k <key>`
&nbsp;       | &nbsp; | :green_circle: works fine w/ a key
&nbsp;       | &nbsp; | :red_circle: still available w/o a key
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;
__+/- b__    | &nbsp; | ban someone (#room cannot send to channel)
&nbsp;       | irssi  | `/mode #room +b user1`
&nbsp;       | nc     | 
&nbsp;       |        | &nbsp;


# Command mode

COMMAND      | client | syntax
:----------- | :----: | :-
&nbsp;
Login        | nc     | `$ nc e1r2p3 6677`
&nbsp;       |        | `PASS password`
&nbsp;       |        | `NICK nick`
&nbsp;       |        | `USER usr * * Usr`
&nbsp;       | &nbsp; | &nbsp;
__/DCC__     | irssi  | `/dcc send <~@~user> /mnt/nfs/homes/USR/DIR/log1`
&nbsp;       |        | `/dcc <list\|get>`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/kill__    | irssi | `/oper user1 OPER_PASS`, `/kill someone`
&nbsp;       | &nbsp; | ::: in channel: _user still in server_
&nbsp;       | &nbsp; | ::: out of channel: _user disconnected_
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/kick__    | &nbsp; | _must be done inside a channel_
&nbsp;       | irssi  | `/oper user1 OPER_PASS` 
&nbsp;       |        | `/kick user2`
&nbsp;       |        | `user2`: only allowed to `/quit` `/exit` once kicked
&nbsp;       |        | cannot `/part`
&nbsp;       | nc     | `OPER user1 OPER_PASS`
&nbsp;       |        | `KICK #room nuo misbehaving`
&nbsp;       | &nbsp; | &nbsp;
__/mode__    | &nbsp; | _channel mode_
&nbsp;       | irssi  | `/oper user1 OPER_PASS`
&nbsp;       |        | `/mode #room +o user1`
&nbsp;       |        | `/kick user2` (user1)
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/part__    | irssi  | `/part <#room>`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/topic__   | &nbsp; | `/topic` | `topic <topic>`
&nbsp;       | &nbsp; | :red_circle: _Not working_
&nbsp;       | &nbsp; | _::: topic updated only at re-joining_
&nbsp;       | &nbsp; | _::: topic unchanged at quiting_
&nbsp;       | nc     | `TOPIC #room :no`
&nbsp;       |        | `TOPIC #room :yes`
&nbsp;       | &nbsp; | &nbsp;
__/restart__ | irssi  | `/oper user1 OPER_PASS`
&nbsp;       |        | `/restart`
&nbsp;       | nc     | 
&nbsp;       | &nbsp; | &nbsp;
__/notice__  | irssi  | `/notice @xqin hello, world`
&nbsp;       |        | `/notice #room hello, world` :yellow_circle:
&nbsp;       | &nbsp; | _::: by default a private msg to oneself ..._
&nbsp;       | &nbsp; | _::: and not to the entire channel (to prevent flooding)_
&nbsp;       | nc     | `PRIVMSG #room <msg>`
&nbsp;       | &nbsp; | &nbsp;
__/say__     | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/say something`
&nbsp;       | &nbsp; | &nbsp;
__/names__   | &nbsp; | (IRSSI built-in)
&nbsp;       | irssi  | `/n` or `/name`
&nbsp;       | nc     | no `NAME #room` for nc :yellow_circle:
&nbsp;       | &nbsp; | &nbsp;
PRIVMSG      | nc     | `PRIVMSG #room hello world`
&nbsp;       |        | `PRIVMSG nuo hi, how r u？`
&nbsp;       | &nbsp; | &nbsp;


