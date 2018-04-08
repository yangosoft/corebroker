#include <iostream>

#include <unistd.h>
#include <mutex>


#include "TConfigMng.h"
#include "TSocketServer.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    TConfigMng configMng("config.json");
    std::mutex m_mutex;

    TSocketServer server(5000);
    server.start();


    while (server.getStatus() != SS_STATUS::SS_ERROR)
    {
            //std::cout << "DONE" << std::endl;
            sleep(1);
            
    }


    return 0;
}
