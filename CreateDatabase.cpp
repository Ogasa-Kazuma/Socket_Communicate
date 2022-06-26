  #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "common.h"






    void AddData(char* newline, const char* value, bool needMatch256Byte){
        char buff[SIZE_BODY_DATA + 1];

        for(int i = 0; i < (SIZE_BODY_DATA + 1); i++){

            if(i == SIZE_BODY_DATA && !needMatch256Byte){
                buff[i] = ',';
            }
            else if(i == SIZE_BODY_DATA && needMatch256Byte){
                buff[i] = '_';
            }
            else if(i < strlen(value)){
                buff[i] = value[i];
            }
            else if(i >= strlen(value)){
                buff[i] = ' ';
            }
        }
        if(needMatch256Byte){
            strncat(newline, buff, SIZE_BODY_DATA + 1);
            strncat(newline, ",", 1);
            
        }
        else{
            strncat(newline, buff, SIZE_BODY_DATA + 1);
        }
        
    }

    void MakeHeaderLine(FILE* fp, char* standardItems){
        //データベースの最初の行を作成する
        
        //項目とカンマを最初に文字列に追加
        strcpy(standardItems, "id,pin,name,birth,money");
        //一行のデータ量は固定（固定長にしないと更新プログラムと合わなくなる)
        for(int i = strlen(standardItems); i < SIZE_DATA_LINE; i++){
            //何もないところは空白で埋める
            standardItems[i] = ' '; 
        }
        //カンマと改行
        standardItems[SIZE_DATA_LINE - 2] = ',';
        standardItems[SIZE_DATA_LINE - 1] = '\n';
    
    }

    void MakeUserDataLine(char* newline, const char* gid, const char* pin, const char* name, const char* birth, const char* money){
        strncat(newline, gid, SIZE_GID);
        strncat(newline, ",", 1);
        strncat(newline, pin, SIZE_PIN);
        strncat(newline, ",", 1);


        AddData(newline, name, false);
        AddData(newline, birth, false);
        AddData(newline, money, true);

        strcat(newline, "\n");
    }


    int main(void){

        //ファイルの準備
        FILE *fp;
        //ファイルがない場合は自分の手で新規作成する必要があります
        char fname[] = "data3.txt";    
        fp = fopen(fname, "r+");

        //最初の項目の行を生成
        char standardItems[SIZE_DATA_LINE];
        MakeHeaderLine(fp, standardItems);
        fputs(standardItems, fp);


        
        //ユーザー情報の新しい行を追加する//////////////////
        char newline[SIZE_DATA_LINE];
        memset(newline, '\0', sizeof(newline));

        const char* gid = "1234567891";
        const char* pin = "1234";
        const char* name = "Kazuma";
        const char* birth = "19990406";
        const char* money = "123456789123456789";

        MakeUserDataLine(newline, gid, pin, name, birth, money);
        fputs(newline, fp);
        ///////////////////////////////////////////



        //ユーザー2
        char newline2[SIZE_DATA_LINE];
        memset(newline2, '\0', sizeof(newline2));

        const char* gid2 = "0000000001";
        const char* pin2 = "0002";
        const char* name2 = "Somen";
        const char* birth2 = "20220622";
        const char* money2 = "0010000100";

        MakeUserDataLine(newline2, gid2, pin2, name2, birth2, money2);
        fputs(newline2, fp);



        //ファイルの最後　ここを変更すると更新プログラムと合わなくなる
        fputs("end\n", fp);
    
    	return 0;
    }
