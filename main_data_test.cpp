#include "DataServer.h"

int main(void){

    DataServer* dataServer = new DataServer("data3.txt");
    char money_value[SIZE_BODY_DATA];
    memset(money_value, '\0', sizeof(money_value));
    strncpy(money_value, "9999999993億8800万0000", SIZE_BODY_DATA);
    
    dataServer -> Update("0000000001", "money", money_value);
    //テスト項目
    //1.gid, typeなどに終端文字を含めずUpdateを呼び出した場合 : 今のままだとメモリ系のエラー(セグメンテーションフォールトとかコアダンプ)
    //2.各項目に大きな値(SIZE_BODY_DATAより多い文字数) を入力した場合 : 上記と同じエラー(strcpyではなくstr「n」cpy使うとかで対策)
    //3.データベースファイルを、存在しないファイルにしたらどうなるか　: エラー
    //4.データベースにGIDとかの項目をあらかじめ書いていなかったら : 
    //5.moneyの更新の値(money_value)の途中に「end」とか「'\0'」とか「'\n'」を入れたらどうなるか
    //上記は、私がコーディング中に「これやられたらやばいな」と思ったもので、対策はしてません





    //Getを試す
    char result[SIZE_DATA_LINE];

    //Getが受け取る文字列は終端文字がきちんと含まれてる前提
    dataServer -> Get(result, "0000000001", "name");
    printf("取得した値を表示\n");
    //typeに"money"を指定したら、gidに対応したmoneyの値が表示される
    printf("%s\n", result);




    //ログイン可否の判断機能を試す
    bool canLogin = dataServer -> canLogin("0000000001", "0002");
    printf("%d\n", canLogin);





    return 0;
}