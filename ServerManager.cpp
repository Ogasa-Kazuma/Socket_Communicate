
#include "ServerManager.h"

ServerManager :: ServerManager(ClientManager* clientManager){


}

ServerManager :: ~ServerManager(){
    close(this -> sock);
    close(this -> sock_client);
}

int ServerManager :: Init(){
    

    this -> sock_details = {0};
    //ここのアンパサンドらへんどうだろ
    this -> sock = BindedSocket(&(this -> sock_details), "../test.unix-socket");
    return this -> sock;
}

int ServerManager :: Accept(sock_info* sock_details){
    int size_sock_details = sizeof(sock_info);
    this -> sock_client = accept(this -> sock, (struct sockaddr*)&sock_details, (socklen_t *)&size_sock_details);
    return this -> sock_client;
}


void ServerManager :: ReadRequest(char* destination, char* source, int length, int offset){
        for(int i = 0; i < length; i++){
            destination[i] = source[i + offset];
        }
        //destination[length] = NULL;
}



bool ServerManager :: IsDataCame(char* buff){
    for(int i = 0; i < sizeof(buff); i++){
        if(!buff[i] == 0 && !buff[i] == '\0'){
            return true;
        }
    }
    return false;
}


void ServerManager :: UpdateResponse(char* request, char* response){

    char value_to_register_db[body_separate];
    for(int i = 0; i < sizeof(value_to_register_db); i++){
        value_to_register_db[i] = request[i + size_header];
    }
    strcpy(response, "upd");
    

}



void ServerManager :: MakeResponse(char* request, char* response){

    char header[size_header];
    memset(header, '\0', sizeof(header));

    this -> ReadRequest(header, request, size_header, 0);

    if(header[0] == flag_login){
        //this -> clientManager ->
    }
    else if(header[0] == flag_logout){
        this -> Logout(response);
    }
    else if(header[0] == flag_inspect){
        this -> InspectResponse(request, response);
    }
    else if(header[0] == flag_update){
        this -> UpdateResponse(request, response);
    }

}


void ServerManager :: InitGid(){
    for(int i = 0; i < sizeof(this -> gid_logined_user); i++){
        this -> gid_logined_user[i] = '\0';
    }
}


void ServerManager :: LoginSyslog(){
    char message[size_gid];
    strncpy(message, this -> gid_logined_user, size_gid);
    strcat(message, " : login");
    this -> clientManager -> Syslog(message);

}

void ServerManager :: LogoutSyslog(){
    char message[size_gid];
    strncpy(message, this -> gid_logined_user, size_gid);
    strcat(message, " : logout");
    this -> clientManager -> Syslog(message);

}


void ServerManager :: Logout(char* response){

    this -> LogoutSyslog();
    this -> InitGid();
    strcpy(response, "out\n");
    this -> flag_exit_communicate = true;
    write(this -> sock_client, response, temp_buff_size);

    const char write_buff[100] = "ログアウト成功";
    write(1, write_buff, sizeof(write_buff));
    
    close(this -> sock_client);
}


void ServerManager :: UpdateSyslog(char* type, char* value){

} 


void ServerManager :: InspectResponse(char* request, char* response){

    const char type_request = request[1];
    if(type_request == type_name){
        strcpy(response, "kam");
    }
    else if(type_request == type_birth){
        strcpy(response, "bir");
    }
    else if(type_request == type_money){
        strcpy(response, "kane");
    }

    write(1, response, sizeof(response));


}






int ServerManager :: Communicate(){

    char buff_rcv[MAX_BUFF_SIZE];
    char buff_res[MAX_BUFF_SIZE];

    this -> flag_exit_communicate = false;

    while(1){
        if(this -> flag_exit_communicate){
            break;
        }

        

        printf("start Communicate\n");
        
        //メモリ初期化しないと、同じリクエストが同時にめちゃくちゃ来まくってるみたいになる
        memset(buff_rcv, '\0', sizeof(buff_rcv));
        memset(buff_res, '\0', sizeof(buff_res));
        //クライアントサーバーからの受信待ち
        int result_read = read(this -> sock_client, buff_rcv, sizeof(buff_rcv));

        if(!(result_read == -1)){

            buff_rcv[0] = flag_logout;
            buff_rcv[1] = 0;
            this -> MakeResponse(buff_rcv, buff_res);
        }



        //何もクライアントから送られてきてないとき、ずっとwriteで送信し返されそうだけど
        else if(!(strcmp(buff_rcv, "\0") == 0)){
            //MAX_BUFF_SIZEでいいのか？
            write(this -> sock_client, buff_res, 5);
        }
    }
    return 0;
}


void ServerManager :: SetNonBlocking(int fd){
    int flag = 1;
    ioctl(fd, FIONBIO, &flag);
}

void ServerManager :: NavigateUser(){
    if(!this -> hasNavigated){
        //printfのみが後回しにされる！
        //printfの代わりにwrite関数ならすぐ実行された！
        const char write_buff[100] = "exit : 終了, ps : サーバー状況\n";
        write(1, write_buff, 100);
        this -> hasNavigated = true;
    }
}

void ServerManager :: Receive(){
    
    while(1){

        this -> NavigateUser();   
   
        //UIを表示したあとに通信開始
        if(this -> hasNavigated){
            this -> SetNonBlocking(this -> sock);
            //標準入力をノンブロッキングモードに
            this -> SetNonBlocking(0);

            //5, は接続可能なクライアント数なのでハードコーディングはやめる!            
            listen(this -> sock, this -> max_client_accept);
            this -> sock_client = Accept(&sock_details);
            
            this -> SetNonBlocking(this -> sock_client);

            //いずれかのクライアントと繋がっていればデータのやりとり！
            if(this -> sock_client >= 0){
                int result_communicate = Communicate();
                //通信中にサーバ終了キーが押されたらそのままプロセス終了！
                if(result_communicate == this -> exitCode){
                    break;
                }
            }
              
            //終了判定（キー入力）
            if(NeedExit()){
                break;
            }
            
        }
        
               
    }
     
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