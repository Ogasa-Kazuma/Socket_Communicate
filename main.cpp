#include "ServerManager.h"
#include "ClientManager.h"


int main(void){

    ClientManager* clientManager = new ClientManager();
    ServerManager sManager(clientManager);
    sManager.Init();
    sManager.Receive();
    return 0;
}