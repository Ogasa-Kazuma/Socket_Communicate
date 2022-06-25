
#include "ServerManager.h"



//////////////////////////// Initialization /////////////////////////

ServerManager :: ServerManager(ClientManager* clientManager, DataManager* dataManager, ServerController* serverController){

    this -> clientManager = clientManager;
    this -> dataManager = dataManager;
    this -> serverController = serverController;

}

ServerManager :: ~ServerManager(){
    
    //閉じないとOS上に残るかも
    close(this -> sock);
    close(this -> sock_client);
    //メモリ開放
    /*
    delete this -> clientManager;
    delete this -> dataManager;
    delete this -> serverController;
    */

}

int ServerManager :: Init(){
    

    this -> sock_details = {0};
    //ここのアンパサンドらへんどうだろ
    this -> sock = BindedSocket(&(this -> sock_details), "../test.unix-socket");
    return this -> sock;
}

/////////////////////////////////////////////////////////////////////////




///////////////////////// Transportation ////////////////////////


int ServerManager :: Accept(sock_info* sock_details){
    int size_sock_details = sizeof(sock_info);
    this -> sock_client = accept(this -> sock, (struct sockaddr*)&sock_details, (socklen_t *)&size_sock_details);
    return this -> sock_client;
}

int ServerManager :: CreateSock(sock_info* sock_details, const char* path){

    int sock = 0;

    if((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return -1;
    }

    sock_details -> sun_family = AF_UNIX;
    strcpy(sock_details -> sun_path, path);

    return sock;
}

int ServerManager :: Bind(int sock, sock_info* sock_details){

    remove(sock_details -> sun_path);

    if(bind(sock, (struct sockaddr*)sock_details, sizeof(sock_info)) == -1){
        perror("bind");
        close(sock);
        exit(1);
    }

    return sock;
}


int ServerManager :: BindedSocket(sock_info* sock_details, const char* path){
    //const char*にしたのがC++での変更点
    int sock = this -> CreateSock(sock_details, path);
    sock = this -> Bind(sock, sock_details);
    return sock;

}

////////////////////////////////////////////////////////////////////////////





///////////////////////// System Log ////////////////////////////////////
void ServerManager :: LoginSyslog(){
    char message[100];
    memset(message, '\0', sizeof(message));
    strncpy(message, this -> gid_logined_user, sizeof(this -> gid_logined_user));
    strcat(message, " : login");
    this -> clientManager -> Syslog(message);

}

void ServerManager :: LogoutSyslog(){
    char message[100];
    memset(message, '\0', sizeof(message));
    strncpy(message, this -> gid_logined_user, sizeof(this -> gid_logined_user));
    strcat(message, " : logout");
    this -> clientManager -> Syslog(message);

}

void ServerManager :: UpdateSyslog(char* type, char* value){
    char message[100];
    memset(message, '\0', sizeof(message));
    strncpy(message, this -> gid_logined_user, sizeof(this -> gid_logined_user));
    strcat(message, " : update");
    this -> clientManager -> Syslog(message);
} 
////////////////////////////////////////////////////////////////////////////







//////////////////// message to client ////////////////////////////////////


void ServerManager :: InvalidRequestMessageToClient(){
//パラメータや要求がおかしいとクライアントに伝える
    char invalid_message[100] = "invalid request";
    this -> SendToClient(invalid_message);    
}

void ServerManager :: SendToClient(char* response){
    write(this -> sock_client, response, MAX_BUFF_SIZE);
}


////////////////////////////////////////////////////////////////////////









/////////////////// Login Status //////////////////////////////////////////

void ServerManager :: InitGid(){
//すべてNULL文字にする
    for(int i = 0; i < sizeof(this -> gid_logined_user); i++){
        this -> gid_logined_user[i] = '\0';

    }

}

///////////////////////////////////////////////////////////////////////////




//////////////// Response /////////////////////////////////////////////////
void ServerManager :: MakeResponse(char* request, char* response){

    
    if(request[HEADER_COMMAND] == COMMAND_CLIENT_LOGIN){
        this -> Login(request, response);
    }
    else if(request[HEADER_COMMAND] == COMMAND_CLIENT_LOGOUT){
        this -> Logout(response);
    }
    else if(request[HEADER_COMMAND] == COMMAND_CLIENT_READ){
        this -> InspectResponse(request, response);
    }
    else if(request[HEADER_COMMAND] == COMMAND_CLIENT_UPDATE){
        this -> UpdateResponse(request, response);
    }
    else{
        this -> InvalidRequestMessageToClient();
    }

}

bool ServerManager :: IsUserLogined(){
//ログイン済みか判定
//ログインしていないなら、GIDの値はすべて0
    for(int i = 0; i < sizeof(this -> gid_logined_user); i++){
        if(!(this -> gid_logined_user[i] == '\0') && !(this -> gid_logined_user[i] == 0)){
            return true;
        }
    }
    return false;
}

void ServerManager :: ShowClientStatus(const char* message){
//サーバー側プロセスにクライアントの状態とメッセージを表示する
    char write_buff[100];
    memset(write_buff, '\0', sizeof(write_buff));
    strncpy(write_buff, this -> gid_logined_user, sizeof(this -> gid_logined_user));
    strcat(write_buff, message);
    write(1, write_buff, sizeof(write_buff));
}

void ServerManager :: Login(char* request, char* response){
    
    //ログインの可否と更新の可否をクライアントに通知
    this -> clientManager -> LoginResponse(request, response, this -> gid_logined_user);
    this -> SendToClient(response);

    //ログイン成功ならシステムログへの記録と、サーバー側プロセスのUIに表示
    if(this -> IsUserLogined()){
        this -> LoginSyslog();
        this -> ShowClientStatus(" : ログイン\n");
    }

}



void ServerManager :: UpdateResponse(char* request, char* response){

    char value_to_register_db[SIZE_BODY_DATA];
    for(int i = 0; i < sizeof(value_to_register_db); i++){
        value_to_register_db[i] = request[i + SIZE_HEADER];
    }
    
    //今回の仕様ではmoneyで固定
    this -> dataManager -> Update(this -> gid_logined_user, "money", value_to_register_db);
    this -> ShowClientStatus(" : 預金額を更新しました\n");    

}



void ServerManager :: InspectResponse(char* request, char* response){

    const char type_request = request[HEADER_DATA_TYPE];


    if(type_request == COMMAND_CLIENT_READ_NAME){
        this -> dataManager -> Get(this -> gid_logined_user, "name", response);
        this -> SendToClient(response);
    }
    else if(type_request == COMMAND_CLIENT_READ_BIRTHDAY){
        this -> dataManager -> Get(this -> gid_logined_user, "birth", response);
        this -> SendToClient(response);
    }
    else if(type_request == COMMAND_CLIENT_READ_BALANCE){
        this -> dataManager -> Get(this -> gid_logined_user, "money", response);
        this -> SendToClient(response);
    }
    else{
        this -> InvalidRequestMessageToClient();
    }


}



void ServerManager :: Logout(char* response){
    
    //GIDを初期化する前にシステムログとサーバー側UIに出力
    this -> LogoutSyslog();
    this -> ShowClientStatus(" : ログアウト\n");
    
    //GIDの初期化
    this -> InitGid();
    
    //ログアウトユーザーとの通信を終了
    this -> flag_exit_communicate = true;
    close(this -> sock_client);
    
}

//////////////////////////////////////////////////////////////////////









/////////////////////////// Communicate to Client //////////////////

int ServerManager :: Communicate(){

    char buff_rcv[MAX_BUFF_SIZE];
    char buff_res[MAX_BUFF_SIZE];

    this -> flag_exit_communicate = false;

    while(1){
        if(this -> flag_exit_communicate){
            break;
        }

            
        //メモリ初期化しないと、同じリクエストが同時にめちゃくちゃ来まくってるみたいになる
        memset(buff_rcv, '\0', sizeof(buff_rcv));
        memset(buff_res, '\0', sizeof(buff_res));
        //クライアントサーバーからの受信待ち
        int result_read = read(this -> sock_client, buff_rcv, sizeof(buff_rcv));

        if(!(result_read == -1)){
            this -> MakeResponse(buff_rcv, buff_res);
        }
    }
    return 0;
}



void ServerManager :: Receive(){
    //UIを表示したあとに通信開始
    this -> NavigateUser();

    while(1){

        this -> SetNonBlocking(this -> sock);
        //標準入力をノンブロッキングモードに
        this -> SetNonBlocking(0);

        //接続要求の受け入れ            
        listen(this -> sock, this -> max_client_accept);
        this -> sock_client = Accept(&sock_details);
        
        this -> SetNonBlocking(this -> sock_client);

        //いずれかのクライアントと繋がっていればデータのやりとり！
        if(this -> sock_client >= 0){
            int result_communicate = Communicate();
        }
            
        //終了判定（キー入力）
        if(NeedExit()){
            break;
        }
            
    }
     
}
/////////////////////////////////////////////////////////////////////


//////////////////// Nonblocking mode ////////////
void ServerManager :: SetNonBlocking(int fd){
    int flag = 1;
    ioctl(fd, FIONBIO, &flag);
}
//////////////////////////////////////////

/////////////// User Interface /////////////////////////////////

void ServerManager :: NavigateUser(){
    if(!this -> hasNavigated){
        //printfのみが後回しにされる！
        //printfの代わりにwrite関数ならすぐ実行された！
        const char write_buff[100] = "exit : 終了, ps : サーバー状況\n";
        write(1, write_buff, 100);
        this -> hasNavigated = true;
    }
}


bool ServerManager :: NeedExit(){

        char read_buff[20];
        memset(read_buff, '\0', 10);
        //標準入力バッファに格納されたキーボード入力を読み取る
        read(0,read_buff, 10);

        if(strcmp(read_buff, "exit\n") == 0){
            return true;
        }

        return false;
}

////////////////////////////////////////