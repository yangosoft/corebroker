/* 
 * File:   TNodeClient.cpp
 * Author: daerobotics
 * 
 * Created on 17 de julio de 2014, 12:31
 */

#include <iostream>
#include <unistd.h>
#include "TNodeClient.h"
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

TNodeClient::TNodeClient(tcp::socket& socket) : m_socket(socket), TNode(socket)
{
    std::cout << "I'm socket: " << m_socket.remote_endpoint().address().to_string() << std::endl;
    m_nodeId = 0;
    //m_nodeStatus = NODE_DISCONNECTED;
    m_nodeStatus = NODE_CONNECTED;

    //Inicializar mutex
    pthread_mutex_init(&m_mutex, NULL);
}

void TNodeClient::EnqueueMsg(const TMessage& msg)
{
    TMessage m(msg);

    m_lstMessages.push_back(&m);
}

void TNodeClient::operator()()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::string message = "PEPE\n";
    boost::system::error_code ignored_error;

    boost::array<char, 128> inputBuffer;

    //Leer el nodeID con el que se presenta este módulo
    size_t len = m_socket.read_some(boost::asio::buffer(inputBuffer), ignored_error);

    if (len > 0)
    {
        std::cout << " DATA FROM SOCKET: " << std::endl;
        std::cout.write(inputBuffer.data(), len);
        std::string raw(inputBuffer.data());

        bool ok;
        TMessage msg = TMessage::fromRawData(raw, ok);

        if (false == ok)
        {
            m_nodeStatus = NODE_REJECTED;
            std::cout << __FUNCTION__ << " node is rejected" << std::endl;
        } else
        {
            //Validate 

            m_nodeId = msg.getFrom();
            ok = msg.readValue(std::string("node_name"), m_nodeName);

            std::cout << __FUNCTION__ << " node(" << m_nodeId << ") : " << m_nodeName << std::endl;



            std::string msgLogin = "#begin\n{ \"message_name\": \"ANSWER_MSG_LOGIN\",\n";
            msgLogin += "\"message_from\": " + boost::lexical_cast<std::string>(m_nodeId) + " ,\n";
            msgLogin += "\"message_to\": 1001 ,\n";
            msgLogin += "\"status\": \"OK\" }\n#end\n";
            m_nodeStatus = NODE_ACCEPTED;
            boost::system::error_code error;


            size_t len = m_socket.write_some(boost::asio::buffer(msgLogin), error);

            if (error)
            {
                std::cout << "ERROR sending answer login: " << error.message() << std::endl;
                m_nodeStatus = NODE_REJECTED;
            }


        }
    }



    //If es valido...
    //m_nodeStatus = NODE_ACCEPTED;
    //else
    //no valido
    //m_nodeStatus = NODE_REJECTED;




    while (1)
    {
        pthread_mutex_lock(&m_mutex);
        //std::cout << __FUNCTION__ << " sleep " << std::endl;
        std::cout << __PRETTY_FUNCTION__ << " " << m_nodeId << " ]> IM LOGGED? " << (m_nodeStatus == NODE_ACCEPTED) << std::endl;



        //boost::asio::write(m_socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
        // m_socket.write_some(boost::asio::buffer(message), ignored_error);

        //        if (ignored_error)
        //        {
        //            m_nodeStatus = NODE_DISCONNECTED;
        //            std::cout << "Client disconnected!" << std::endl;
        //            m_socket.close();
        //            pthread_mutex_unlock(&m_mutex);
        //            break;
        //        }
        std::chrono::milliseconds dura(5000);
        std::this_thread::sleep_for(dura);
        pthread_mutex_unlock(&m_mutex);
        //sleep(5);
    }

}

void TNodeClient::Pause()
{

}

void TNodeClient::Start()
{

}

void TNodeClient::Stop()
{

}

uint32_t TNodeClient::getNodeId()
{
    return m_nodeId;
}

void TNodeClient::setNodeId(uint32_t nodeId)
{
    std::cout << __FUNCTION__ << ": " << nodeId << std::endl;
    m_nodeId = nodeId;
}

TNodeClient::~TNodeClient()
{
}


