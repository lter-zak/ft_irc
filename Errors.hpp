#ifndef  ERRORS_HPP
# define ERRORS_HPP

#include <iostream>

const std::string  ERR_NEEDMOREPARAMS(const std::string& command);
 
const std::string ERR_PASSWDMISMATCH(const std::string& command);

const std::string ERR_PASSISALREADYSET(const std::string& command);

const std::string ERR_NICKCOLLISION(const std::string& command);

const std::string ERR_ALREADYREGISTRED(const std::string& command);

const std::string ERR_NICKNAMEINUSE(const std::string& command, const std::string& name);

const std::string ERR_USERISOFLINE(const std::string& command, const std::string& name);
#endif