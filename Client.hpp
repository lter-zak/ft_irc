#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>



class Client 
{
    private:
        std::string _nickname;
        std::string _username;
        int _onlineFD;
        bool _pass;
    public:
        Client(std::string, std::string, int fd);
        bool guest;
        std::string getUsername();
        void setUsername(std::string);
        void setNickname(std::string);
        std::string getNickname();
        bool getPass();
        void setPass(bool);
        int getOnlineFD();
        void setOnlineFD(int fd);
       
};

#endif