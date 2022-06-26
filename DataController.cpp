
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

            FILE *fp_write;
            
            //r+ は読み書き両方のモード
            fp_write = fopen(dataFileName, "r+");

            while(1){

                //id読み取り用配列（一応毎回初期化する)    
                char id[SIZE_GID + 1];
                memset(id, '\0', sizeof(id));

                //ファイルからGIDを取得
                for(int i = 0; i < SIZE_GID; i++){
                    id[i] = fgetc(fp_write);

                //終了判定 データベースの最終行は「end」の文字列
                //ゴリ押しif文感満載
                    if(id[0] == 'e' && id[1] == 'n' && id[2] == 'd'){
                        printf("end");
                        return 0;
                    }
                }

                //終端文字を追加
                id[SIZE_GID + 1] = '\0';



                //GIDが一致する行があれば更新
                if(strcmp(id, gid) == 0){
                    //カンマはgid,pin,name,birth,と4つ出てきている
                    const int num_canma = 4;

                    //moneyの文字のところまで移動
                    fseek(fp_write, num_canma + SIZE_PIN + (2 * SIZE_BODY_DATA), SEEK_CUR);
                    for(int j = 0; j < SIZE_BODY_DATA; j++){
                        //文字ずつ上書きしていく
                        fputc(value[j], fp_write);
                    }
                    //アンダーバー、カンマ、改行文字を飛び越えたいのでfseekで3文字分の移動
                    //データは85Byteずつ3つ来る。余る1Byteはnullじゃなくてアンダーバーで埋めてる
                    fseek(fp_write, 3, SEEK_CUR);
                }

                //GIDが一致していない場合、次の行に飛ぶ
                else{
                    fseek(fp_write, SIZE_DATA_LINE - SIZE_GID, SEEK_CUR);
                }
        
                
                
        }
        fclose(fp_write);
        return 0;
    }







    int main(void){


        char value_update[SIZE_BODY_DATA];
        const char* gid = "0000000001";
        const char* type = "money";
        strcpy(value_update, "ABCDEFG");


        //Getが受け取る文字列は終端文字がきちんと含まれてる前提
        Update("data3.txt", gid, type, value_update);


    	return 0;
    }
