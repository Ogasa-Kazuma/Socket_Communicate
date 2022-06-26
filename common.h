#pragma once








////////////////// クライアント - サーバー間通信 ////////////////////////////////

//送受信のデータ量
//header(2) + GID(10) + PIN(4) + body(256)
#define MAX_BUFF_SIZE 272

//ヘッダー（要求を表す部分)が何バイト目か
#define HEADER_COMMAND 0
//ヘッダー (データの項目を表す部分)が何バイト目か
#define HEADER_DATA_TYPE 1

#define SIZE_HEADER  2
#define SIZE_GID  10
#define SIZE_PIN  4

#define SIZE_BODY_DATA 85


#define COMMAND_CLIENT_UPDATE 2

#define COMMAND_CLIENT_READ 3
#define COMMAND_CLIENT_READ_NAME 0
#define COMMAND_CLIENT_READ_BIRTHDAY 1
#define COMMAND_CLIENT_READ_BALANCE 2

#define COMMAND_CLIENT_LOGOUT 4
#define COMMAND_CLIENT_LOGIN 5
//////////////////////////////////////////////////




////////////////////// ログイン & モード ////////////////////////////////

//ログインの可否
#define COMMAND_SERVER_LOGIN_NG 0
#define COMMAND_SERVER_LOGIN_OK 1

//更新の可否
#define COMMAND_SERVER_LOGIN_OK_RW 0
#define COMMAND_SERVER_LOGIN_OK_RO 1

//////////////////////////////////////////////////////////////////////





///////////////// データ管理 //////////////////////////////////////////

//1行あたりのカンマの数
#define NUM_CANMA 5

//1行あたりの改行文字の個数
#define NUM_NEWLINE_CHAR 1

//データベースファイルの一行あたりの文字数 ヘッダーは含まない、カンマと改行文字をプラス
#define SIZE_DATA_LINE (MAX_BUFF_SIZE - SIZE_HEADER + NUM_CANMA + NUM_NEWLINE_CHAR)

///////////////////////////////////////////////////////////////////////
