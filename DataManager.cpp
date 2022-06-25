    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
     
    #define N 256 // 1行の最大文字数(バイト数)


    int SplitElements(char* str, char* return_value, int offset){
                //要素の抜き出し
                for(int i = 0; i < N; i++){
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

  
 
 



    void Update(char* return_value, const char* gid, const char* type){

        FILE *fp; // FILE型構造体
        //データファイルの名前
            char fname[] = "data.txt";
            char str[N];
        
            fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
            FILE *fp_write = fopen("data2.txt", "r+");

            if(fp == NULL) {
                printf("%s file not open!\n", fname);
                
            }

            //ファイルがあればファイル読み取り
            else{
                //fgetsは1行ずつデータを読み取る。1行分のデータがstrに格納される
                while(fgets(str, N, fp) != NULL) {
                    printf("%s", str);
                    //データファイルの最後は「end」なのでそこで終了
                    if(strcmp(str, "end\n") == 0){
                        printf("end!\n");
                        break;
                    }

                    printf("%c\n", str[0]);
                    //str[0] = 'a';
                    //fputs(str, fp_write);
                    
                    char id[N];
                    char pin[N];
                    char name[N];
                    char birth[N];
                    char money[N];
                    
                    //idの抽出
                    int offset1 = SplitElements(str, id, 0);
                    //nameの抽出
                    int offset2 = SplitElements(str, pin, offset1);
                    //birthの抽出
                    int offset3 = SplitElements(str, name, offset2);
                    //預金額の抽出
                    int offset4 = SplitElements(str, birth, offset3);
                    //預金額の抽出
                    int offset5 = SplitElements(str, money, offset4);
                    
                    //取得できた各要素を表示
                    printf("%s\n", id);
                    printf("%s\n", pin);
                    printf("%s\n", name);
                    printf("%s\n", birth);
                    printf("%s\n", money);

                    char str_cpy[N];
                    strcpy(str_cpy, str);

                    char type_cpy[N];
                    strcpy(type_cpy, type);

                    //なにの値を返すべきか判断
                    
                    if(strcmp(id, gid) == 0){
                        
                        char rewrite_buff[N];
                        int count_canma = 0;
                        int first_index = 0;
                        int j = 0;
                        for(int i = 0; i < sizeof(str_cpy); i++){
                            /*
                            if(count_canma == 2){
                                if(first_index == 0){
                                    first_index = i;
                                }
                                else{
                                    rewrite_buff[i] = type[i - first_index - 1];
                                }
                                
                            }
                            */
                            
                            if(!(count_canma == 2)){
                                rewrite_buff[i] = str_cpy[i];
                            }
                            else{
                                //if(j < strlen(type_cpy)){
                                    rewrite_buff[i] = 'a';//type_cpy[j];
                                //}
                  
                                
                                ///j += 1; 
                            }
                            
                            if(str_cpy[i] == ','){
                                count_canma =  count_canma + 1;
                                rewrite_buff[i] = str_cpy[i];
                            }
                            
                        }
                        rewrite_buff[sizeof(str_cpy) + 1] = 'a';
                        fputs(rewrite_buff, fp_write);
                    }
                    else{
                        fputs(str, fp_write);
                    } 
                    
    
                }
            }

            fclose(fp); // ファイルを閉じる
            fclose(fp_write);
    }







    int main(void) {

        int num = 0;
        int dev = (num / 10) % 10;
        printf("%d\n", dev);

        


        char result[N];
        const char* gid = "0070211111";
        const char* type = "money";


        //Getが受け取る文字列は終端文字がきちんと含まれてる前提
        Update(result, gid, type);

    	return 0;
    }