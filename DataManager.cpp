#include "DataManager.h"


DataManager :: DataManager(const char* DBfileName){
    
}

void DataManager :: Update(char* gid, const char* type, char* value){
    printf("update request received");
}

bool DataManager :: canLogin(char* gid, char* pin){
    return true;
}

void DataManager :: Get(char* gid, const char* type, char* value_recv_buff){
    if(strcmp(type, "name") == 0){
        strncpy(value_recv_buff, "name", 5);
    }
    else if(strcmp(type, "birth") == 0){
        strncpy(value_recv_buff, "bir", 5);
    }
    else if(strcmp(type, "money") == 0){
        strncpy(value_recv_buff, "kane", 5);
    }
   
}