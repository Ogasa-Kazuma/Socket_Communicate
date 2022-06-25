#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


class DataManager{

    private:
        char* DBfileName;

    public:
        DataManager(const char* DBfileName);
        void Update(char* gid, const char* type, char* value);
        bool canLogin(char* gid, char* pin);
        void Get(char* gid, const char* type, char* value_recv_buff);

};