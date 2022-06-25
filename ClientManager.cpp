#include "ClientManager.h"



ClientManager :: ClientManager(DataManager* dataManager, ServerController* serverController){
    this -> dataManager = dataManager;
    this -> serverController = serverController;
}


void ClientManager :: LoginResponse(char* request, char* response, char* gid_logined_user){
    
    char gid[SIZE_GID];
    char pin[SIZE_PIN];

    //リクエストからGIDとPINを抽出
    for(int i = 0; i< sizeof(gid); i++){
        gid[i] = request[i + SIZE_HEADER];
    }
    for(int j = 0; j < sizeof(pin); j++){
        pin[j] = request[j + SIZE_HEADER + sizeof(gid)];
    }

    //ログインの可否とデータ更新の可否
    bool canLogin = this -> dataManager -> canLogin(gid, pin);
    bool canUpdate = this -> serverController -> canUpdate();

    //レスポンスの生成
    if(canLogin){
        response[0] = COMMAND_SERVER_LOGIN_OK;
        strncpy(gid_logined_user, gid, sizeof(gid));
    }
    else{
        response[0] = COMMAND_SERVER_LOGIN_NG;
    }
    if(canUpdate){
        response[1] = COMMAND_SERVER_LOGIN_OK_RW;
    }
    else{
        response[1] = COMMAND_SERVER_LOGIN_OK_RO;
    }

} 


void ClientManager :: Syslog(const char* message){
    syslog(LOG_INFO, "%s", message);
}