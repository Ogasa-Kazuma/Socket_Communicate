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
#include "DataManager.h"
#include "ServerController.h"
#include "common.h"



using namespace std;

typedef struct sockaddr_un sock_info;


class ServerManager{

    private:
        //変数
        ClientManager* clientManager;
        DataManager* dataManager;
        ServerController* serverController;

        sock_info sock_details = {0};
        int sock;
        int sock_client;
        bool hasNavigated;
        const int exitCode = 2;
        const int max_client_accept = 5;
        char gid_logined_user[SIZE_GID];
        int flag_exit_communicate = false;

        //メソッド
        

        int CreateSock(sock_info* sock_details, const char* path);
        int Bind(int sock, sock_info* sock_details);
        int BindedSocket(sock_info* sock_details, const char* path);
        int Accept(sock_info* sock_details);
        
        int Communicate();
        void SetNonBlocking(int fd);
        void NavigateUser();
        
        char JudgeRequest(char* header);
        void MakeResponse(char* request, char* response);
        void InspectResponse(char* request, char* response);
        void UpdateResponse(char* request, char* response);
        void InitGid();
        void Login(char* request, char* response);
        void Logout(char* response);
        void LoginSyslog();
        void LogoutSyslog();
        void UpdateSyslog();
        void SendToClient(char* response);
        void InvalidRequestMessageToClient();
        bool IsUserLogined();
        void ShowClientStatus(const char* message);
        bool ReadStandardInput();

    public:
        ServerManager(ClientManager* clientManager, DataManager* dataManager, ServerController* serverController);
        ~ServerManager();
        int Init();
        void Receive();
};