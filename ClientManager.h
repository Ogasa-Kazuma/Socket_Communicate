#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>

#include "common.h"
#include "DataManager.h"
#include "ServerController.h"

class ClientManager{

    private:
        DataManager* dataManager;
        ServerController* serverController;

    public:
        ClientManager(DataManager* dataManager, ServerController* serverController);
        void LoginResponse(char* request, char* response, char* gid_logined_user);  
        void Syslog(const char* message);


};