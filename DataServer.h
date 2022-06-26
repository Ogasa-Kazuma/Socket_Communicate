
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

class DataServer{

    private:
        const char* dbFileName;
        int SplitElements(char* str, char* return_value, int offset);

    public:
        DataServer(const char* dbFileName);
        ~DataServer();
        int Update(const char* gid, const char* type, char* value);
        void Get(char* return_value, const char* gid, const char* type);
        bool canLogin(const char* gid, const char* pin);
};