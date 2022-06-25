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
#include "ClientManager.h"
#include "common.h"



using namespace std;

typedef struct sockaddr_un sock_info;


class ServerManager{

    private:
        //変数
        ClientManager* clientManager;
        sock_info sock_details = {0};
        bool hasNavigated;
        const int exitCode = 2;
        const int max_client_accept = 5;
        char gid_logined_user[size_gid];
        int flag_exit_communicate = false;

        //メソッド
        bool NeedExit();
        int sock;
        int sock_client;
        int CreateSock(sock_info* sock_details, const char* path);
        int Bind(int sock, sock_info* sock_details);
        int BindedSocket(sock_info* sock_details, const char* path);
        int Accept(sock_info* sock_details);
        void ReadRequest(char* destination, char* source, int length, int offset);
        int Communicate();
        void SetNonBlocking(int fd);
        void NavigateUser();
        bool IsDataCame(char* buff);
        char JudgeRequest(char* header);
        void MakeResponse(char* request, char* response);
        void InspectResponse(char* request, char* response);
        void UpdateResponse(char* request, char* response);
        void InitGid();
        void Logout(char* response);
        void LoginSyslog();
        void LogoutSyslog();
        void UpdateSyslog(char* type, char* value);
        void SendToClient(char* response);
        void InvalidRequestMessageToClient();

    public:
        ServerManager(ClientManager* clientManager);
        ~ServerManager();
        int Init();
        void Receive();
};