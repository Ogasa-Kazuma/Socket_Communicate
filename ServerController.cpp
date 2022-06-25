#include "ServerController.h"


ServerController :: ServerController(int lower_time, int upper_time){
    this -> lower_time = lower_time;
    this -> upper_time = upper_time;
}

bool ServerController :: canUpdate(){
    return true;
}