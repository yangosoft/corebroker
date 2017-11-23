/* 
 * File:   main.cpp
 * Author: daerobotics
 *
 * Created on 8 de julio de 2014, 11:14
 */

#include <cstdlib>
#include <stdint.h>
#include <thread>
#include <iostream>

#include "inc/TConfigMng.h"
#include "inc/TSocketServer.h"

#include "inc/TSocketClient.h"

#include "inc/TNode.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{

    TConfigMng cfg("/tmp/config.json");
    cfg.readConfiguration();


    uint32_t port;
    bool success = cfg.getParamValue(std::string("port"), port);
    port = 5000;

std::chrono::milliseconds dura(1000);
    if (argc == 1)
    {
        TSocketServer server(port);

        std::thread thread1(std::ref(server));

        
        std::this_thread::sleep_for(dura);
        
        std::this_thread::sleep_for(dura);
        
        thread1.join();
        
       /* TNodeList lstNodes = server.getNodeList();
        TNodeList::const_iterator it = lstNodes.begin();
        for (; it != lstNodes.end(); it++)
        {
            std::cout << "NODE: " << (*it)->getNodeId() << std::endl;
        }*/
    }

    if (argc > 1)
    {

        TSocketClient client("localhost", 5000);
        bool c = client.connect();
        std::cout << "Client connected!!" << std::endl;
        if ( true == c)
        {
            TNode& n = client.getNode();
            std::cout << "GOT NODE" << std::endl;

            n.setNodeId(1001);
            n.doLogin();
        }

        std::thread thread2(std::ref(client));


        /*std::this_thread::sleep_for(dura);
        TSocketClient client2("localhost", 5000);
        if (client2.connect())
        {
            TNode& n = client2.getNode();
            std::cout << "GOT NODE" << std::endl;

            n.setNodeId(1002);
            n.doLogin();
        }

        std::thread thread3(std::ref(client2));*/

        thread2.join();
       

    }

    while (1);

   //thread1.join();

    return 0;
}

