#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <csignal>
#include "utils.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Errors.hpp"
#include <map>

#include <stdlib.h>
#define TRUE   1  
#define FALSE  0  

//class Client;

class Server 
{
    private:
        int _portNumber;
        std::string _password;
        int  client_socket[5];
        std::map<int, Client*> Clients;
        std::map<std::string, Channel*> Channels;
        fd_set readfds;
        int indexClient;
    public:
        Server();
        Server(int portNumber, std::string password);
static void signalHandler(int signal);
        int getPortNumber();
        void setPortNumber(int portNumber);

        std::string getPassword();
        void setPassword(std::string password);
        void executeCommand(std::string cmd, int index);
        void createServer();


        void pass_cmd(std::string cmd, int index);
        void nick_cmd(std::string cmd, int index);
        void user_cmd(std::string cmd, int index);
        void quit_cmd(std::string cmd, int index);
        void privmsg_cmd(std::string cmd, int index);
        void join_cmd(std::string cmd, int index);
        void kick_cmd(std::string cmd, int index);
        void mode_cmd(std::string cmd, int index);
         
        int onlineClientsFD(int index);
        int isClientFull(int index);
        void createOrRegister(int index, int fd);
};

#endif