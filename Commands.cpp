
# include "Server.hpp"

void Server::pass_cmd(std::string cmd, int index)// stex indexy clineti mapi arajinna
{  
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (Clients.find(index) != Clients.end()  )
    {
         if (splited.size() != 2)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    }  
    if(splited[1] != _password && Clients[index]->getPass() == false)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSWDMISMATCH(splited[0]));
        return ; 
    }
    if(splited[1] == _password  && Clients[index]->getPass() == true)
    {
         sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSISALREADYSET(splited[0]));
            return ;
    } 
    else if(Clients[index]->getPass() == true)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_PASSISALREADYSET(splited[0]));
        return ;
    }
    Clients[index]->setPass(true);         
   }  
    //    else if(Clients.find(index) != Clients.end() ) {
    //         sendMyMsg(Clients[index]->getOnlineFD(),  "You already have password\n");
    //    }                                                                                     
}



void Server::nick_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 2)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    }  

        if(isClientFull(Clients[index]->getOnlineFD())!=-1)
        {
             for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
            {
                if(it->second->getNickname() == splited[1])
                {
                    sendMyMsg(Clients[index]->getOnlineFD(), ERR_NICKNAMEINUSE(splited[0], splited[1]));
                    return;
                } 
            }
        }
        Clients[index]->setNickname(splited[1]);
    
}


void Server::user_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 2) // --------------------------------- 5
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0])); 
        return ;
    }  

    if(Clients[index]->getUsername() == "" )
    {
        Clients[index]->setUsername(splited[1]);
    } 
    else 
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_ALREADYREGISTRED(splited[0]));
        return ;
    }
}



void Server::quit_cmd(std::string cmd, int index)
{
    std::vector<std::string>  splited =  split(cmd, ' '); 
    if (splited.size() != 1)
    {
        sendMyMsg(Clients[index]->getOnlineFD(), ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    } 
    for(int i = 0; i < 5; i++)
    {
        if(client_socket[i]==Clients[index]->getOnlineFD())
        {
            client_socket[i] = 0;
            close(Clients[index]->getOnlineFD());
            Clients[index]->setOnlineFD(-1);
        } 
    }
}



 void Server::privmsg_cmd(std::string cmd, int index)
 {



    if(isClientFull(Clients[index]->getOnlineFD())==-1 )
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
        return;
    }
    std::vector<std::string>  splited =  split(cmd, ' '); 
    std::string msg = "";
    if(Clients[index]->getNickname() == splited[1])
    {
        return;
    }   
    for(unsigned long i = 2; i < splited.size(); i++)
    {
        msg = msg+splited[i]+" ";
    }
    if (splited.size() < 3)
    {
        sendMyMsg(Clients[index]->getOnlineFD(),  ERR_NEEDMOREPARAMS(splited[0]));
        return ;
    } 

    for (std::map<int, Client*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
    {
        if(it->second->getNickname() == splited[1] && it->second->getOnlineFD() != Clients[index]->getOnlineFD())
        {
            if(it->second->getOnlineFD()==-1){
                sendMyMsg(Clients[index]->getOnlineFD(),  ERR_USERISOFLINE(splited[0], splited[1]));
                return;
            }
            sendMyMsg(it->second->getOnlineFD(), Clients[index]->getNickname() + ": " + msg + "\n");
            return;
        } 
    }



    //for (std::map<std::string, Channel*>::iterator it =  Channels.begin(); it !=  Channels.end(); ++it) 
  //  {
        // if(it->first==splited[1])
        // {
        //     for (unsigned long i = 0; i < it->second->clients.size(); i++)
        //     {
        //         if(it->second->clients[i]!=index)
        //         sendMyMsg(Clients[it->second->clients[i]]->getOnlineFD(), Clients[index]->getNickname() + ": "+msg+"\n");
        //     }
        // } 
        // return;
   // }
    sendMyMsg(Clients[index]->getOnlineFD(),  "Dont exist "+splited[1]+" user or channel\n");
 }


  void Server::join_cmd(std::string cmd, int index)
  {
        // petqa tenal usera te che, nax petqa user lini
        if(isClientFull(Clients[index]->getOnlineFD())==-1 )
        {
            sendMyMsg(Clients[index]->getOnlineFD(),  "You dont have account, the first register or log in\n");
            return;
        }
        std::vector<std::string>  splited =  split(cmd, ' '); 
        if (splited.size() !=2)
        {
            sendMyMsg(Clients[index]->getOnlineFD(),  "Incoorect parameters count\n");
            return ;
        } 
        if(Channels.find(splited[1])==Channels.end())
        {
            Channels.insert(std::pair<std::string, Channel*>(splited[1], new Channel()));
            Channels[splited[1]]->adminID = index;
            (Channels[splited[1]]->clients).push_back(index);
            sendMyMsg(Clients[index]->getOnlineFD(), "You are created "+splited[1]+" channel\n");
         
        } 
        else 
        {
            for (unsigned long i = 0; i <  Channels[splited[1]]->clients.size(); i++)
            {
                if(Channels[splited[1]]->clients[i]==index)
                {
                    sendMyMsg(Clients[index]->getOnlineFD(), "You  already joined "+splited[1]+" channel\n");
                    return;
                }
            }
            (Channels[splited[1]]->clients).push_back(index);
            sendMyMsg(Clients[index]->getOnlineFD(), "You joined "+splited[1]+" channel\n");
        }
  }