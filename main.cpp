#include "ServerManager.h"
#include "ClientManager.h"
#include "ServerController.h"
#include "DataManager.h"


int main(void){

    ClientManager* clientManager = new ClientManager();
    DataManager* dataManager = new DataManager("data.txt");
    ServerController* serverController = new ServerController(7, 19);
    ServerManager sManager(clientManager, dataManager, serverController);
    sManager.Init();
    sManager.Receive();
    return 0;
}