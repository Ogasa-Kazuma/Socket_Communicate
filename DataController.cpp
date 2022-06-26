
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "common.h"
     
    


    int SplitElements(char* str, char* return_value, int offset){
                //要素の抜き出し
                for(int i = 0; i < SIZE_DATA_LINE; i++){
                    //カンマで要素の分かれ目を判断する
                    if(str[i + offset] == ','){
                        //返す値に終端文字を入れてあげる
                        return_value[i] = '\0';
                        //これは「すでに何文字目まで読み込んだか」を表す
                        //人間からみた「1文字目」はコンピュータから見ると「0文字目」なので1を足す
                        return (i + offset + 1);
                    }
                    return_value[i] = str[i + offset]; 
                }
    }

  

 



    int Update(const char* dataFileName, const char* gid, const char* type, char* value){

            FILE *database;
            
            //r+ は読み書き両方のモード
            database = fopen(dataFileName, "r+");

            while(1){

                //id読み取り用配列（一応毎回初期化する)
                //終端文字分のプラス 1    
                char id[SIZE_GID + 1];
                memset(id, '\0', sizeof(id));

                //ファイルからGIDを取得
                for(int i = 0; i < SIZE_GID; i++){
                    // 1文字ずつ読み取り
                    id[i] = fgetc(database);

                //終了判定 データベースの最終行は「end」の文字列
                //ゴリ押しif文感満載
                    if(id[0] == 'e' && id[1] == 'n' && id[2] == 'd'){
                        printf("end");
                        //ファイルを閉じて終了
                        fclose(database);
                        return 0; //なぜかbreakでうまくいかない
                    }
                }

                //終端文字を追加
                id[SIZE_GID + 1] = '\0';

                //GIDが一致する行があれば更新
                if(strcmp(id, gid) == 0){
                    //カンマはgid,pin,name,birth,と4つ出てきている
                    const int num_canma = 4;

                    //moneyの項目のところまで移動
                    fseek(database, num_canma + SIZE_PIN + (2 * SIZE_BODY_DATA), SEEK_CUR);
                    
                    //moneyの更新（上書き）開始
                    for(int j = 0; j < SIZE_BODY_DATA; j++){
                        // 1文字ずつ上書きしていく
                        fputc(value[j], database);
                    }

                    //上書き終了後
                    /*
                    　アンダーバー、カンマ、改行文字を飛び越えたいのでfseekで3文字分の移動
                    　次の行の先頭に移動する
                    　データは85Byteずつ3つ来る。余る1Byteはnullじゃなくてアンダーバーで埋めてる
                    */
                    fseek(database, 3, SEEK_CUR);
                }

                //GIDが一致していない場合、次の行に飛ぶ
                else{
                    fseek(database, SIZE_DATA_LINE - SIZE_GID, SEEK_CUR);
                }

                            
        }
        //
        fclose(database);
        return 0;
    }





    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
     




  

 



    void Get(char* return_value, const char* gid, const char* type){

        FILE *fp; // FILE型構造体
        //データファイルの名前
            char fname[] = "data3.txt";
            char str[SIZE_DATA_LINE];
        
            fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
            if(fp == NULL) {
                printf("%s file not open!\n", fname);
                
            }

            //ファイルがあればファイル読み取り
            else{
                //fgetsは1行ずつデータを読み取る。1行分のデータがstrに格納される
                while(fgets(str, SIZE_DATA_LINE, fp) != NULL) {
                    printf("%s", str);
                    //データファイルの最後は「end」なのでそこで終了
                    if(strcmp(str, "end\n") == 0){
                        //printf("end!\n");
                        break;
                    }
                    
                    char id[SIZE_GID + 1];
                    char pin[SIZE_PIN + 1];
                    char name[SIZE_BODY_DATA + 1];
                    char birth[SIZE_BODY_DATA + 1];
                    char money[SIZE_BODY_DATA + 1];
                    
                    //idの抽出
                    int offset1 = SplitElements(str, id, 0);
                    //nameの抽出
                    int offset2 = SplitElements(str, pin, offset1);
                    //nameの抽出
                    int offset3 = SplitElements(str, name, offset2);
                    //birthの抽出
                    int offset4 = SplitElements(str, birth, offset3);
                    //預金額の抽出
                    int offset5 = SplitElements(str, money, offset4);
                

                    //なにの値を返すべきか判断
                    if(strcmp(id, gid) == 0){
                        //名前を返す
                        if(strcmp(type, "name") == 0){
                            strcpy(return_value, name);
                        }
                        else if(strcmp(type, "birth") == 0){
                            //生年月日を返す
                            strcpy(return_value, birth);
                        }
                        else if(strcmp(type, "money") == 0){
                            //預金額を返す
                            strcpy(return_value, money);
                        }
                    } 
    
                }
            }

            fclose(fp); // ファイルを閉じる

    }






    int main(void){


        char money_value[SIZE_BODY_DATA];
        const char* gid = "0000000001";
        const char* type = "money";
        strcpy(money_value, "43億8800万0000"); //漢字でも43.8800.0000でも大丈夫

        Update("data3.txt", gid, type, money_value);

        //テスト項目
        //1.gid, typeなどに終端文字を含めずUpdateを呼び出した場合 : 今のままだとメモリ系のエラー(セグメンテーションフォールトとかコアダンプ)
        //2.各項目に大きな値(SIZE_BODY_DATAより多い文字数) を入力した場合 : 上記と同じエラー(strcpyではなくstr「n」cpy使うとかで対策)
        //3.データベースファイルを、存在しないファイルにしたらどうなるか　: エラー
        //4.データベースにGIDとかの項目をあらかじめ書いていなかったら : 
        //5.moneyの更新の値(money_value)の途中に「end」とか「'\0'」とか「'\n'」を入れたらどうなるか
        //上記は、私がコーディング中に「これやられたらやばいな」と思ったもので、対策はしてません


        char result[SIZE_DATA_LINE];
        const char* gid3 = "0000000001";
        const char* type3 = "money";


        //Getが受け取る文字列は終端文字がきちんと含まれてる前提
        Get(result, gid3, type3);
        printf("取得した値を表示\n");
        //typeに"money"を指定したら、gidに対応したmoneyの値が表示される
        printf("%s\n", result);



    	return 0;
    }
