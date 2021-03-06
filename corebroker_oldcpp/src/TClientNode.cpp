/* 
 * File:   TClientNode.cpp
 * Author: daerobotics
 * 
 * Created on 21 de julio de 2014, 13:11
 */

#include "TClientNode.h"

#include "INode.h"

#include <msg/msgdef.h>

#include <unistd.h>

#include <iostream>
#include <sstream>

TClientNode::TClientNode(const std::string& host, uint32_t port, bool nonBlocking) : m_client(host, port)
{
    m_nodeStatus = NODE_DISCONNECTED;
    m_nodeId = THIS_NODE_ID;
    m_nonBlocking = nonBlocking;
    m_thisThread = NULL;
    pthread_mutex_init(&m_mutex , NULL);
    if (true == m_nonBlocking)
    {
        
        
        
        m_threadStatus = TH_PAUSE;
        int ret = pthread_create(m_thisThread, NULL, &TClientNode::run_helper, this);

        if (ret < 0)
        {
            std::cout << "could not create thread" << std::endl;

        }

        


    }


}

bool TClientNode::connect()
{
    bool ret = m_client.tryConnect();

    if (true == ret)
    {
        m_nodeStatus = NODE_CONNECTED;
    }

    return ret;
}

bool TClientNode::login()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::stringstream ss;
    //if connected
    if (NODE_CONNECTED != m_nodeStatus)
    {
        return (NODE_ACCEPTED == m_nodeStatus);
    }


    std::string msgLogin = M_SENTINEL_BEGIN + "\n" + "{\n";
    msgLogin += "\"message_name\": \"" + MSG_LOGIN_NAME + "\",\n";
    ss << m_nodeId;
    msgLogin += "\"message_from\": \"" + ss.str() + "\",\n";
    ss.str("");

    ss << ((uint32_t) 1000);

    msgLogin += "\"message_to\": \"" + ss.str() + "\"\n";

    msgLogin += "}\n";
    msgLogin += M_SENTINEL_END + "\n";


    bool ok;
    std::cout << " Trying to create a msg..." << std::endl;
    TMessage m = TMessage::fromRawData(msgLogin, ok);

    if (false == ok)
    {
        std::cout << "Cannot parse message" << std::endl;
        std::cout << "_________________" << std::endl;
        std::cout << msgLogin << std::endl;
        std::cout << "_________________" << std::endl;

    }


    ok = m_client.writeData(m);
    if (false == ok)
    {
        std::cout << "Cannot send message" << std::endl;
    }

    //Read login response

    TMessage answerMsg = m_client.readMessage(ok);
    if (false == ok)
    {
        std::cout << "Cannot read message" << std::endl;
    }

    std::cout << "MESSAGE NAME: '" << answerMsg.getName() << "'" << std::endl;

    if (answerMsg.getName() == MSG_LOGIN_ANSWER_NAME)
    {
        std::cout << "Is a ANSWER LOGIN!" << std::endl;
        //Check if ok
        std::string key = "correct";
        std::string value;
        ok = answerMsg.readValue(key, value);

        if (false == ok)
        {
            std::cout << "Cannot read 'correct'" << std::endl;

        } else
        {
            std::cout << " * correct = " << value << std::endl;
            ok = ok & (value == "1");
            m_nodeStatus = NODE_ACCEPTED;
            
            if ( true == m_nonBlocking )
            {
                m_threadStatus = TH_START;
            }

        }

    } else
    {
        ok = false;
    }


    return ok;
}

uint32_t TClientNode::getNodeId()
{
    return m_nodeId;
}

void TClientNode::setNodeId(uint32_t nodeId)
{
    std::cout << " * Set node id: " << nodeId << std::endl;
    m_nodeId = nodeId;
}

bool TClientNode::sendMessage(const TMessage& msg)
{
    bool ok = false;

    if (NODE_ACCEPTED == m_nodeStatus)
    {

        ok = m_client.writeData(msg);

    }
    return ok;
}

TMessage TClientNode::readMessage(bool &ok)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    TMessage m("error", 0, 0);
    ok = false;
    if (NODE_ACCEPTED == m_nodeStatus)
    {
        
        if( false == m_nonBlocking )
        {
            m = m_client.readMessage(ok);
        }else
        {
            if ( m_lstMessages.size() > 0 )
            {
                
                pthread_mutex_lock(&m_mutex);
                ok = true;
                m = *(m_lstMessages.front());
                m_lstMessages.pop_front();
                pthread_mutex_unlock(&m_mutex);
            }
            
                
            
        }

    }
    return m;
}

void TClientNode::operator()()
{
    TMessage m("error", 0, 0);
    bool ok = false;
    
    while (TH_STOP != m_threadStatus)
    {
        while (TH_PAUSE == m_threadStatus)
        {
             //std::cout << __PRETTY_FUNCTION__ << " <-> PAUSED " << std::endl;

            sleep(5);
        }

        if( NODE_ACCEPTED == m_nodeStatus )
        {
            m = m_client.readMessage(ok);
            if ( true == ok )
            {
                std::cout << "Added new message to queue!" << std::endl;
                //Lock queue
                pthread_mutex_lock(&m_mutex);
                m_lstMessages.push_back(&m);
                pthread_mutex_unlock(&m_mutex);
            }
            
        }


        std::cout << __PRETTY_FUNCTION__ << std::endl;
        sleep(1);


    }

}

bool TClientNode::isBlocking()
{
    return (false == m_nonBlocking ) ;
}

void* TClientNode::run()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    (*this)();
    std::cout << __PRETTY_FUNCTION__ << ": end run! " << std::endl;
    return 0;
}

void TClientNode::pause()
{
    m_internalStatus = SC_PAUSE;
}

void TClientNode::start()
{
    m_internalStatus = SC_LISTENING;
}

void TClientNode::stop()
{
    m_internalStatus = SC_STOP;
}

int32_t TClientNode::getStatus()
{
    return m_internalStatus;
}

TClientNode::~TClientNode()
{
}

