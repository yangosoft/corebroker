#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include "TConfigMng.h"
#include "TSocketServer.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    TConfigMng configMng("config.json");


    pthread_mutex_t m_mutex;
    pthread_mutex_init(&m_mutex, NULL);


    pthread_mutex_lock(&m_mutex);
    pthread_mutex_unlock(&m_mutex);


        TSocketServer server(5000);
        server.start();




        while (server.getStatus() != SS_ERROR)
        {
            //std::cout << "DONE" << std::endl;
            sleep(1);
        }

   

    return 0;
}
