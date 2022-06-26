
#include "DataServer.h"


DataServer :: DataServer(const char* dbFileName){
    this -> dbFileName = dbFileName;

}

DataServer :: ~DataServer(){

}

int DataServer :: Update(const char* gid, const char* type, char* value){

            FILE *database;
            
            //r+ は読み書き両方のモード
            database = fopen(this -> dbFileName, "r+");

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








    void DataServer :: Get(char* return_value, const char* gid, const char* type){

        FILE *fp; // FILE型構造体
        //データファイルの名前
           
        char str[SIZE_DATA_LINE];
    
        fp = fopen(this -> dbFileName, "r"); // ファイルを開く。失敗するとNULLを返す。


        
            //fgetsは1行ずつデータを読み取る。1行分のデータがstrに格納される
        while(fgets(str, SIZE_DATA_LINE, fp) != NULL) {
            //printf("%s", str);
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
            int offset1 = this -> SplitElements(str, id, 0);
            //nameの抽出
            int offset2 = this -> SplitElements(str, pin, offset1);
            //nameの抽出
            int offset3 = this -> SplitElements(str, name, offset2);
            //birthの抽出
            int offset4 = this -> SplitElements(str, birth, offset3);
            //預金額の抽出
            int offset5 = this -> SplitElements(str, money, offset4);
        

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
    
    fclose(fp); // ファイルを閉じる

}




    bool DataServer :: canLogin(const char* gid, const char* pin){

        FILE *fp; // FILE型構造体
           
        char str[SIZE_DATA_LINE];
    
        fp = fopen(this -> dbFileName, "r"); // ファイルを開く。失敗するとNULLを返す。
        
            //fgetsは1行ずつデータを読み取る。1行分のデータがstrに格納される
        while(fgets(str, SIZE_DATA_LINE, fp) != NULL) {
            //printf("%s", str);
            //データファイルの最後は「end」なのでそこで終了
            if(strcmp(str, "end\n") == 0){
                fclose(fp); // ファイルを閉じる
                return false;
                break;
            }
            
            char id[SIZE_GID + 1];
            char pin_db[SIZE_PIN + 1];
            char name[SIZE_BODY_DATA + 1];
            char birth[SIZE_BODY_DATA + 1];
            char money[SIZE_BODY_DATA + 1];
            
            //idの抽出
            int offset1 = this -> SplitElements(str, id, 0);
            //nameの抽出
            int offset2 = this -> SplitElements(str, pin_db, offset1);
            //nameの抽出
            int offset3 = this -> SplitElements(str, name, offset2);
            //birthの抽出
            int offset4 = this -> SplitElements(str, birth, offset3);
            //預金額の抽出
            int offset5 = this -> SplitElements(str, money, offset4);
        

            //なにの値を返すべきか判断
            if(strcmp(id, gid) == 0 && strcmp(pin, pin_db) == 0){
                fclose(fp); // ファイルを閉じる
                return true;
            }

        }
    return false;
}








int DataServer :: SplitElements(char* str, char* return_value, int offset){
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

        //ここが実行されることは想定してない
        return -1;
}
