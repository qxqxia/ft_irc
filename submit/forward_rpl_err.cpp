#include "Freenode.hpp"

std::string Get_RPL_ERR(int code, Server *serv, User *user, std::string arg1, std::string arg2)
{
    std::stringstream ss;
    ss << code;
    std::string code_in_string = ss.str();
    if (code < 10)
        code_in_string.insert(0, 2, '0');
    else if (code < 100)
        code_in_string.insert(0, 1, '0');

    std::string res;

    if (user)
    {
        res += ":" + serv->get_server_name() + " " + code_in_string + " " + user->get_nickname() + " ";
    }
    else
    {
        res += ":" + serv->get_server_name() + " " + code_in_string + " " + "*" + " ";
    }
    switch (code)
    {
        case 001:
            res += RPL_WELCOME(user->get_nickname(), user->get_username(), user->get_hostname());
			break ;
		case 002:
            res += RPL_YOURHOST;
			break ;
        case 003:
            res += RPL_CREATED;
			break ;
        case 004:
            res += RPL_MYINFO;
			break ;
        case 005:
            res += RPL_BOUNCE(serv->get_port());
			break ;
        case 221:
            res += RPL_UMODEIS(arg1);
			break ;
        case 324:
            res += RPL_CHANNELMODEIS(arg1, arg2);
			break ;
        case 353:
            res += RPL_NAMREPLY(arg1, arg2);
			break ;
        case 366:
            res += RPL_ENDOFNAMES(arg1);
			break ;
        case 367:
            res += RPL_BANLIST(arg1, arg2);
			break ;
        case 368:
            res += RPL_ENDOFBANLIST(arg1);
			break ;
        case 381:
            res += RPL_YOUREOPER;
			break ;
        case 401:
            res += ERR_NOSUCHNICK(arg1);
			break ;
        case 403:
            res += ERR_NOSUCHCHANNEL(arg1);
			break ;
        case 404:
            res += ERR_CANNOTSENDTOCHAN(arg1);
			break ;
        case 405:
            res += ERR_TOOMANYCHANNELS(arg1);
            break ;
        case 431:
            res += ERR_NONICKNAMEGIVEN;
			break ;
        case 432:
            res += ERR_ERRONEUSNICKNAME(arg1);
			break ;
        case 433:
            res += ERR_NICKNAMEINUSE(arg1);
			break ;
        case 441:
            res += ERR_USERNOTINCHANNEL(arg1, arg2);
			break ;
        case 442:
            res += ERR_NOTONCHANNEL(arg1);
			break ;
        case 461:
            res += ERR_NEEDMOREPARAMS(arg1);
			break ;
        case 464:
            res += ERR_PASSWDMISMATCH;
			break ;
        case 471:
            res += ERR_CHANNELISFULL(arg1);
			break ;
        case 472:
            res += ERR_UNKNOWNMODE(arg1, arg2);
            break ;
        // case 473:
        //     res += ERR_INVITEONLYCHAN(arg1);
		// 	break ;
        case 474:
            res += ERR_BANNEDFROMCHAN(arg1);
			break ;
        case 475:
            res += ERR_BADCHANNELKEY(arg1);
			break ;
        case 481:
            res += ERR_NOPRIVILEGES;
            break ;
        case 482:
            res += ERR_CHANOPRIVSNEEDED(arg1);
			break ;
        case 484:
            res += ERR_RESTRICTED;
			break ;
        case 501:
            res += ERR_UMODEUNKNOWNFLAG;
			break ;
        case 669:
            res += BOT_NOCOMMAMD(arg1);
            break;
		default:
			/**/;
    }

    // std::cout << CYAN "(dbg) Get_RPL_ERR: " << res << std::endl;
    // std::cout << YELLOW "(dbg)(Get_RPL_ERR): " RESET << (
    //     (res.empty()) ? "(None)" : res
    // ) << nl;

    return res;
}
