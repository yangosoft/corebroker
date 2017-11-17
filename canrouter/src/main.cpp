#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <unistd.h>



#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdint.h>


#include "tcan.h"
#include "TConfigMng.h"
#include "TClientNode.h"
#include "TMessage.h"
#include "msg/msgdef.h"

//#include "cpplog/cpplog.hpp"

#include <list>
#include <sstream>


using namespace std;
//using namespace cpplog;

#define LOG_INFO(X) std::cout 
#define LOG_ERROR(X) std::cout
#define OstreamLogger

std::list<TMessage> lstIncomingMessages;
pthread_mutex_t mutexIncomingMessages;
TCan can;
//OstreamLogger log(std::cout);
/**
 * Given a TMessage it's sent through CAN
 * @param message
 */
void sendMessageToCAN(TMessage& message)
{

    LOG_INFO(log) << __PRETTY_FUNCTION__ << std::endl;
    if (MSG_CAN_NAME == message.getName())
    {
        LOG_INFO(log) << " * Sending message through CAN" << std::endl;

        uint32_t canId;
        bool ok = message.readValue("can_id", canId);
        if (false == ok)
        {
            LOG_ERROR(log) << " * ERROR can_id not found" << std::endl;
            return;
        }

        can_frame frame;
        frame.can_id = canId;

        //Payload
        for (uint8_t i = 0; i < CAN_MAX_DLEN; i++)
        {

            ostringstream ss;
            ss << i;
            std::string key = "data" + ss.str();
            uint32_t data = 0;
            ok = message.readValue(key, data);

            if (false == ok)
            {
                LOG_ERROR(log) << " * ERROR " << key << "not found" << std::endl;
                return;
            }

            frame.data[i] = (uint8_t) data;







        }


        ok = can.sendMessage(frame);
        if (false == ok)
        {
            LOG_ERROR(log) << " * ERROR sending frame!" << std::endl;
            return;
        }






    }


}

/**
 * Given a can_frame it's sent to the node network
 * @param frame
 */
void sendFrameToNode(TClientNode& client, can_frame& frame)
{
    LOG_INFO(log) << __PRETTY_FUNCTION__ << std::endl;
    bool ok;
    TMessage sMessage = TMessage::fromFile("canmessage.json", ok);
    if (false == ok)
    {
        LOG_ERROR(log) << " * ERROR reading canmessage.json" << std::endl;
        return;
    }

    sMessage.writeValue("can_id", frame.can_id);
    for (uint8_t i = 0; i < CAN_MAX_DLEN; i++)
    {

        ostringstream ss;
        ss << i;
        std::string key = "data" + ss.str();
        sMessage.writeValue(key, frame.data[i]);

    }

    client.sendMessage(sMessage);


}

void *clientNodeReader(void *pClientNode)
{

    //TClientNode* client = (TClientNode*) pClientNode;



    if (NULL == pClientNode)
    {
        LOG_ERROR(log) << "ERROR pCLIENT IS NULL" << std::endl;
        return 0;
    }
    TClientNode* client = static_cast<TClientNode*> (pClientNode);

    if (NULL == client)
    {
        LOG_ERROR(log) << "ERROR CLIENT IS NULL" << std::endl;
        return 0;
    }

    bool ok = false;

    while (SC_STOP != client->getStatus())
    {
        TMessage sMessage = client->readMessage(ok);

        if (true == ok)
        {
            LOG_INFO(log) << "New message received" << std::endl;
            LOG_INFO(log) << sMessage.toString() << std::endl;

            //lock
            //            pthread_mutex_lock(&mutexIncomingMessages);
            //            lstIncomingMessages.push_back(sMessage);
            //            pthread_mutex_unlock(&mutexIncomingMessages);
            //unlock

            sendMessageToCAN(sMessage);

        }


    }


    return 0;


}

/*
 * 
 */
int main(int argc, char** argv)
{
    
#ifdef MODULE_NAME
    LOG_INFO(log) << MODULE_NAME << " - " << VERSION_STRING  << std::endl;
#endif






    TClientNode client("localhost", 5000);
    client.setNodeId(1006);


    pthread_t thNodeReader;
    pthread_mutex_init(&mutexIncomingMessages, NULL);


    bool connected = client.connect();

    if (connected)
    {
        LOG_INFO(log) << "Connected to localhost!" << std::endl;
    } else
    {
        LOG_ERROR(log) << "Cannot connect" << std::endl;
        return -1;
    }

    bool loggedIn = client.login();

    if (loggedIn)
    {
        LOG_INFO(log) << "Logged in!" << std::endl;
    } else
    {
        LOG_ERROR(log) << "Cannot loggin" << std::endl;
        return -1;
    }


    bool ok = false;
    TMessage sMessage = TMessage::fromFile("canmessage.json", ok);

    if (false == ok)
    {
        LOG_ERROR(log) << "Cannot read canmessage.json" << std::endl;
        return -1;
    }





    can.openCAN("can0");

    if (can.isReady())
    {

        LOG_INFO(log) << "CAN READY!" << std::endl;

        int ret = pthread_create(&thNodeReader, NULL, clientNodeReader, (void*) &client);

        if (ret < 0)
        {
            LOG_ERROR(log) << "Error al crear el hilo" << std::endl;
            return -1;
        }









        can_frame message;
        while (1)
        {

            bool b = can.readMessage(message);
            LOG_INFO(log) << "Readed? " << b << std::endl;
            LOG_INFO(log) << "ID: " << message.can_id << " ";

            sMessage.writeValue("can_id", message.can_id);

            sMessage.writeValue("data0", message.data[0]);
            sMessage.writeValue("data1", message.data[1]);
            sMessage.writeValue("data2", message.data[2]);
            sMessage.writeValue("data3", message.data[3]);
            sMessage.writeValue("data4", message.data[4]);
            sMessage.writeValue("data5", message.data[5]);
            sMessage.writeValue("data6", message.data[6]);
            sMessage.writeValue("data7", message.data[7]);



            for (uint8_t i = 0; i < CAN_MAX_DLEN; i++)
            {
                LOG_INFO(log) << std::hex << message.data[i] << " ";



            }
            LOG_INFO(log) << std::endl;

            sendFrameToNode(client, message);

            message.can_id = 0xAA;
            message.data[0] = 0x10;
            message.data[1] = 0x11;
            message.data[2] = 0x12;

            b = can.sendMessage(message);
            LOG_INFO(log) << "Sent? " << b << std::endl;




        }



    } else
    {
        LOG_INFO(log) << "Port not ready" << std::endl;
        return -1;
    }

    void** ret;
    pthread_join(thNodeReader, ret);



    return 0;
}
