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

class ClientManager{

    private:

    public:
        ClientManager();
        void MakeResponse(char* request, char* response);
        void ReadRequest(char* destination, char* source, int length, int offset);
        void Syslog(const char* message);

};