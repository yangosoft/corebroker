/* 
 * File:   TNode.cpp
 * Author: daerobotics
 * 
 * Created on 8 de julio de 2014, 15:20
 */

#include <iostream>
#include <unistd.h>
#include "TNode.h"
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

TNode::TNode(tcp::socket& socket) : m_socket(socket)
{
    std::cout << "I'm socket: " << m_socket.remote_endpoint().address().to_string() << std::endl;
    m_nodeId = 0;
    //m_nodeStatus = NODE_DISCONNECTED;
    m_nodeStatus = NODE_CONNECTED;
    
    m_lastTimestampSent = std::chrono::seconds(std::time(NULL));

    //Inicializar mutex
    pthread_mutex_init(&m_mutex, NULL);
}

void TNode::EnqueueMsg(const TMessage& msg)
{
    TMessage m(msg);

    m_lstMessages.push_back(&m);
}

bool TNode::doLogin()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if (NODE_ACCEPTED == m_nodeStatus)
    {
        std::cout << "Already connected!" << std::endl;
        return true;
    }

    if (NODE_CONNECTED != m_nodeStatus)
    {
        std::cout << "NOT CONNECTED" << std::endl;
        return false;
    }

    std::string msgLogin = "#begin\n{ \"message_name\": \"MSG_LOGIN\",\n";
    msgLogin += "\"message_from\": " + boost::lexical_cast<std::string>(m_nodeId) + " ,\n";
    msgLogin += "\"message_to\": 1000 }\n#end\n";

    boost::system::error_code error;


    size_t len = m_socket.write_some(boost::asio::buffer(msgLogin), error);

    if (error)
    {
        std::cout << "ERROR sending msg login: " << error.message() << std::endl;
    }


    std::cout << "I'm node " << m_nodeId << std::endl;
    
    
    //WAIT FOR RESPONSE
    
    boost::array<char, 128> inputBuffer;
    boost::system::error_code ignored_error;
    len = m_socket.read_some(boost::asio::buffer(inputBuffer), ignored_error);

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

            std::cout << "Response from " << msg.getFrom() << std::endl;
            std::string status;
            ok = msg.readValue(std::string("status"), status);

            std::cout << "Status: " << status << std::endl;
            
            if("OK" == status)
            {
                m_nodeStatus = NODE_ACCEPTED;
            }else
            {
                m_nodeStatus = NODE_REJECTED;
            }
            
            
            return true;


        }
    }
    
    
    
    
    
    return false;
}

void TNode::operator()()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    boost::system::error_code ignored_error;

    boost::array<char, 128> inputBuffer;

    
    //If es valido...
    //m_nodeStatus = NODE_ACCEPTED;
    //else
    //no valido
    //m_nodeStatus = NODE_REJECTED;


    std::cout << __PRETTY_FUNCTION__ << " while! " << std::endl;

    while (true)
    {
        std::cout << __PRETTY_FUNCTION__ << " locked!!!!" << std::endl;
        pthread_mutex_lock(&m_mutex);
        //std::cout << __FUNCTION__ << " sleep " << std::endl;
        std::cout << __PRETTY_FUNCTION__ <<  ": " <<  m_nodeId << " ]> IM LOGGED? " << (m_nodeStatus == NODE_ACCEPTED) << std::endl;

        std::chrono::seconds unix_timestamp = std::chrono::seconds(std::time(NULL));
        
        std::chrono::seconds diffScs = ( unix_timestamp - m_lastTimestampSent );
        if ( diffScs.count() > SECONDS_KEEPALIVE )
        {
            std::cout << __PRETTY_FUNCTION__ << " time to send keepalive" << std::endl;
            m_lastTimestampSent = unix_timestamp;
        }
        

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

void TNode::Pause()
{

}

void TNode::Start()
{

}

void TNode::Stop()
{

}

uint32_t TNode::getNodeId()
{
    return m_nodeId;
}

void TNode::setNodeId(uint32_t nodeId)
{
    std::cout << __FUNCTION__ << ": " << nodeId << std::endl;
    m_nodeId = nodeId;
}

bool TNode::sendKeepAlive()
{
    if( NODE_ACCEPTED != m_nodeStatus )
    {
        return false;
    }
    
    
    std::string msgLogin = "#begin\n{ \"message_name\": \"MSG_KEEP_ACK\",\n";
    msgLogin += "\"message_from\": " + boost::lexical_cast<std::string>(m_nodeId) + " ,\n";
    msgLogin += "\"message_to\": 1000 }\n#end\n";

    boost::system::error_code error;


    size_t len = m_socket.write_some(boost::asio::buffer(msgLogin), error);

    if (error)
    {
        std::cout << "ERROR sending msg login: " << error.message() << std::endl;
        return false;
    }

    
    return true;
}

TNode::~TNode()
{
}

