#include "Errors.hpp"

const std::string  ERR_NEEDMOREPARAMS(const std::string& command)
{ 
    return std::string("461 "  + command + " : Not enough parameters\n"); 
}

const std::string ERR_PASSWDMISMATCH(const std::string& command)
{
    return std::string("464 "  + command + " : Password incorrect\n");
}

const std::string ERR_PASSISALREADYSET(const std::string& command)
{
    return std::string("000 "  + command + " : Password is already set\n");
}

const std::string ERR_NICKCOLLISION(const std::string& command)
{
    return std::string("436 "  + command + " : Nickname collision KILL\n");
}

const std::string ERR_ALREADYREGISTRED(const std::string& command)
{
    return std::string("462 "  + command + " : You may not reregister\n");
}

const std::string ERR_NICKNAMEINUSE(const std::string& command, const std::string& name)
{
    return std::string("433 "  + command + " " + name +" : Nickname is already in use\n");
}

const std::string ERR_USERISOFLINE(const std::string& command, const std::string& name)
{
    return std::string("001 "  + command  +" : User " + name + " is ofline\n");
}






