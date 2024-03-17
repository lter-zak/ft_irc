#include "Server.hpp"
#include "utils.hpp"

Server::Server(){}

Server::Server(int portNumber, std::string password)
{
    _password = password;
    _portNumber = portNumber;
    indexClient = 0;

}

int Server::getPortNumber(){ return _portNumber;}      
void Server::setPortNumber(int portNumber){ _portNumber = portNumber;}

std::string Server::getPassword() { return _password;}
void Server::setPassword(std::string password){_password = password;}



void Server::createServer()
{
    int opt = TRUE;   
    int master_socket , addrlen , new_socket  ,  
          max_clients = 5 , activity, i , valread , sd;   
    int max_sd;   
    struct sockaddr_in address;   
    char buffer[1025];  

    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   

    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( getPortNumber());   
         
    //bind the socket to localhost port 8888  
    if (::bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0 )   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", getPortNumber());   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    { 
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         

//---------------------------------------------------------------------------------------

    while(TRUE)   
    {   
        //clear the socket set  
        FD_ZERO(&readfds);   
     
         //add master socket to set  
        FD_SET(master_socket, &readfds); 

        max_sd = master_socket;   
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {       
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0){
                FD_SET( sd , &readfds);   
            }   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd; 
        }   
    
        activity = select(max_sd + 1, &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error\n");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  

               
        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //send new connection greeting message  
            if( send(new_socket, "You are connected to server\n", 29, 0) != 29 )   
            {   
                perror("send");   
            }   
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
            if(i == 5){
                send(new_socket, "LIMIT\n", 7, 0);
                close(new_socket);
            }
            
        }   

    
        //else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
 
            if (FD_ISSET( sd , &readfds))   
            {   
                std::memset(buffer, '\0', sizeof(buffer));
                valread = recv( sd , buffer, 1024, 0);
                if (valread  > 0)   
                {     
                    if(onlineClientsFD(sd) == -1)
                    {
                        std::cout<<"AY SRA HETEM"<<std::endl;
                        Clients.insert(std::pair<int, Client*>(indexClient, new Client("", "", sd)));
                        indexClient++;
                    } 

                }   
                else if(valread==0)
                {    
                    client_socket[i] = 0;
                    for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
                    {
                        if(it->second->getOnlineFD()==sd && it->second->guest ==true) // online chi u guesta merjic hanuma
                        {
                            delete  Clients[it->first];// cjjnjel es  toxy, voncor te 
                            close(sd);
                            Clients.erase(it->first);
                            break;
                        } 
                    }
                    buffer[valread] = '\0';   
                } 
                executeCommand(buffer, sd);

                for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
                {
                    if(it->second->getOnlineFD()==-1 && it->second->guest ==true) // online chi u guesta merjic hanuma
                    {
                        delete Clients[it->first];// cjjnjel es  toxy, voncor te 
                        Clients.erase(it->first);
                        break;

                    } 
                    if(it->second->guest ==true)
                    {
                        if(isClientFull(sd)!=-1 ) 
                        { 
                            createOrRegister(isClientFull(sd), sd);
                        }  
                    }
                }
                
                std::cout<<"-----------------------------------------------"<<std::endl;
                std::cout<<"| PASS | GUEST  | OFD | INDEX | CHCOUNT | NICK  "<<std::endl;
                for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
                {
                    std::cout<<"| "<<it->second->getPass()<<"    | "<<it->second->guest<<"      | "<<it->second->getOnlineFD()<<"   | "<<it->first<< "     | "<<it->second->ChannelCount <<"       | "<<it->second->getNickname() <<std::endl;
                }
            
                std::cout<<"----------------------------------------------- "<<std::endl;
                std::cout<<"size=============>" <<Clients.size()<<std::endl;
                std::cout<<"ChannelSize=============>" <<Channels.size()<<std::endl;

                for (std::map<std::string, Channel*>::iterator it =  Channels.begin(); it !=  Channels.end(); ++it) 
                {
                     for(std::vector<int>::iterator it1 = Channels[it->first]->adminIDs.begin(); it1 != Channels[it->first]->adminIDs.end();++it1)
                         {
                         std::cout<<*it1<<" ";
                            }
                    std::cout<<" AID ]"<<"" <<"[Max "<<it->second->ChatLimit<<" ]";
                    std::cout<<it->first<<": ";
                        for (unsigned long i = 0; i < it->second->clients.size(); i++)
                        std::cout<<it->second->clients[i]<<" ";
                    std::cout<<std::endl;
                }
            //   system("leaks ./ircserv");
            }   
        }
    }   
}


void Server::executeCommand(std::string cmd, int index)
{
    if(!cmd.compare(0,4, "PASS"))
        pass_cmd(cmd, onlineClientsFD(index));
    else if (!cmd.compare(0,4, "NICK"))
        nick_cmd(cmd, onlineClientsFD(index)); // 2rd argumenty mapi meji arajin indexna darnum
    else if (!cmd.compare(0,4, "USER"))
        user_cmd(cmd, onlineClientsFD(index));
    else if (!cmd.compare(0,4, "QUIT"))
        quit_cmd(cmd, onlineClientsFD(index));
    else if (!cmd.compare(0,7, "PRIVMSG"))
        privmsg_cmd(cmd, onlineClientsFD(index));
    else if (!cmd.compare(0,4, "JOIN"))
        join_cmd(cmd, onlineClientsFD(index));   
    else if (!cmd.compare(0,4, "KICK"))
        kick_cmd(cmd, onlineClientsFD(index));
    else if (!cmd.compare(0,4, "MODE"))
        mode_cmd(cmd, onlineClientsFD(index));
    std::cout<<"11111111111111111111"<<std::endl;
}





int Server::onlineClientsFD(int index)
{
    for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
    {
        if(it->second->getOnlineFD()==index)
        {
            return it->first;
        } 
    }
    return -1;
}


int Server::isClientFull(int index)
{
    for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
    {
        if(it->second->getPass()==true && it->second->getNickname()!="" && it->second->getUsername()!="" && it->second->getOnlineFD()==index)
        {
            it->second->guest = false;
            return it->first;
        } 
    }
    return -1;
}

void Server::createOrRegister(int index, int fd)
{

 for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it) 
 {
        if(it->first!=index && it->second->getNickname()==Clients[index]->getNickname() && 
            it->second->getUsername()==Clients[index]->getUsername() && it->second->getOnlineFD()!=fd)
        {
            if(it->second->getOnlineFD()!=-1)
            {
                sendMyMsg(Clients[index]->getOnlineFD(),  "You are already login, try register\n");
                Clients[index]->guest = true;
                Clients[index]->setNickname("");
                Clients[index]->setUsername("");
            } 
            else if(it->second->getOnlineFD()==-1)
            {
                sendMyMsg(Clients[index]->getOnlineFD(),  "Welcome Back\n");
                it->second->setOnlineFD(fd);
                delete Clients[index];// cjjnjel es  toxy, voncor te 
                Clients.erase(index);
            } 
            return;
        }  
       if(it->first==index && it->second->getNickname()==Clients[index]->getNickname() 
                && it->second->getUsername()== Clients[index]->getUsername() && it->second->getOnlineFD() == fd)
        {
              sendMyMsg(Clients[index]->getOnlineFD(),  "You are succesfully registered\n");
            return;
        }  
        if(it->second->getNickname()==Clients[index]->getNickname() && it->first != index && it->second->getOnlineFD() != fd)
        {
            Clients[index]->guest = true;
            Clients[index]->setNickname("");
            sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NICKCOLLISION("NICK"));// try login or register
            return;
        }
    }
}