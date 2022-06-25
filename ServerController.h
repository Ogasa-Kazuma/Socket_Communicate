#pragma once

class ServerController{

    private:
        int lower_time;
        int upper_time;

    public:
        ServerController(int lower_time, int upper_time);
        bool canUpdate();

};