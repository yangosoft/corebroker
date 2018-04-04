#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include "TConfigMng.h"

#include "TClientNode.h"

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




        std::cout << "Trying to connect socket client" << std::endl;
        TClientNode client("localhost", 5000);
        
        
        std::string path = "test.json";
        if( argc > 2  )
        {
            uint32_t nodeId = atoi(argv[2]);
            std::cout << "Set nodeId " << nodeId << std::endl;
            client.setNodeId(nodeId);
            path = "test2.json";
            
        }
        
        bool connected = client.connect();
        

        std::cout << "Client connected? " << connected << std::endl;
        connected = client.login();
        std::cout << "Login? " << connected << std::endl;
        
        bool ok;
        
        
        
        
        TMessage m = TMessage::fromFile(path, ok);
        
        if(ok == true)
        {
            client.sendMessage(m);
            
        }
        
        while (client.getStatus() != SC_STATUS::SC_STOP)
        {
            m = client.readMessage(ok);
            if( ok )
            {
                std::cout << "NEW MESSAGE " << m.getName() << " from " << m.getFrom() << std::endl;
                std::cout << " DO SOMETHING WITH IT ;)" << std::endl;
                sleep(20);
                
                m = TMessage::fromFile(path, ok);
        
                if(ok == true)
                {
                    client.sendMessage(m);

                }
                
                
            }
            //std::cout << "DONE" << std::endl;
            sleep(1);
        }
        


    return 0;
}
