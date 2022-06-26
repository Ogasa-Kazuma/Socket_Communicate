  #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "common.h"






    void AddData(char* newline, const char* value, bool needMatch256Byte){
        char buff[SIZE_BODY_DATA + 1];

        for(int i = 0; i < (SIZE_BODY_DATA + 1); i++){

            if(i == SIZE_BODY_DATA){
                buff[i] = ',';
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
            strncat(newline, "_", 1);
            
        }
        else{
            strncat(newline, buff, SIZE_BODY_DATA + 1);
        }
        
    }




    int main(void){

        FILE *fp;
        char fname[] = "data3.txt";    
        fp = fopen(fname, "r+");
        char standardItems[SIZE_DATA_LINE];
        strcpy(standardItems, "id,pin,name,birth,money");
        for(int i = strlen(standardItems); i < SIZE_DATA_LINE; i++){
            standardItems[i] = ' ';
        }
        standardItems[SIZE_DATA_LINE - 2] = ',';
        standardItems[SIZE_DATA_LINE - 1] = '\n';
        fputs(standardItems, fp);
        printf("%c\n", standardItems[SIZE_DATA_LINE - 2]);
        printf("%c\n", standardItems[SIZE_DATA_LINE - 1 ]);
        

        char newline[SIZE_DATA_LINE];

        const char* gid = "1234567891";
        const char* pin = "1234";
        const char* name = "Kazuma";
        const char* birth = "19990406";
        const char* money = "123456789123456789";

        strncat(newline, gid, SIZE_GID);
        strncat(newline, ",", 1);
        strncat(newline, pin, SIZE_PIN);
        strncat(newline, ",", 1);


        AddData(newline, name, false);
        AddData(newline, birth, false);
        AddData(newline, money, true);

        
        strcat(newline, "\n");
        fputs(newline, fp);

        fputs("end\n", fp);
    





    	return 0;
    }
