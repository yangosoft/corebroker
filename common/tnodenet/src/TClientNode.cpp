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
#include <chrono>
#include <thread>

TClientNode::TClientNode(const std::string& host, uint32_t port, bool nonBlocking) : m_client(host, port)
{
    m_nodeStatus = TNodeStatus::NODE_DISCONNECTED;
    m_nodeId = THIS_NODE_ID;
    m_nonBlocking = nonBlocking;
    m_thisThread = nullptr;
    
    if (true == m_nonBlocking)
    {
        m_threadStatus = ITHREAD_STATUS::TH_PAUSE;
        int ret = pthread_create(m_thisThread, nullptr, &TClientNode::run_helper, this);

        if (ret < 0)
        {
            throw std::logic_error("could not create thread ");

        }
    }
}

bool TClientNode::connect()
{
    bool ret = m_client.tryConnect();

    if (true == ret)
    {
        m_nodeStatus = TNodeStatus::NODE_CONNECTED;
    }

    return ret;
}

bool TClientNode::login()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::stringstream ss;
    //if connected
    if (TNodeStatus::NODE_CONNECTED != m_nodeStatus)
    {
        return (TNodeStatus::NODE_ACCEPTED == m_nodeStatus);
    }


    std::string msgLogin = M_SENTINEL_BEGIN + "\n" + "{\n";
    msgLogin += "\"message_name\": \"" + MSG_LOGIN_NAME + "\",\n";
    ss << m_nodeId;
    msgLogin += "\"message_from\": \"" + ss.str() + "\",\n";
    ss.str("");

    uint32_t id = 1000;
    
    ss << id;

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
            ok = ok && (value == "1");
            m_nodeStatus = TNodeStatus::NODE_ACCEPTED;
            
            if ( true == m_nonBlocking )
            {
                m_threadStatus = ITHREAD_STATUS::TH_START;
            }

        }

    } else
    {
        ok = false;
    }


    return ok;
}

uint32_t TClientNode::getNodeId() const
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

    if (TNodeStatus::NODE_ACCEPTED == m_nodeStatus)
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
    if (TNodeStatus::NODE_ACCEPTED == m_nodeStatus)
    {
        
        if( false == m_nonBlocking )
        {
            m = m_client.readMessage(ok);
        }else
        {
            if ( ! m_lstMessages.empty() )
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                ok = true;
                m = *(m_lstMessages.front());
                m_lstMessages.pop_front();
            }
        }

    }
    return m;
}

void TClientNode::operator()()
{
    TMessage m("error", 0, 0);
    bool ok = false;
    
    while (ITHREAD_STATUS::TH_STOP != m_threadStatus)
    {
        while (ITHREAD_STATUS::TH_PAUSE == m_threadStatus)
        {
             //std::cout << __PRETTY_FUNCTION__ << " <-> PAUSED " << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        if( TNodeStatus::NODE_ACCEPTED == m_nodeStatus )
        {
            m = m_client.readMessage(ok);
            if ( true == ok )
            {
                std::cout << "Added new message to queue!" << std::endl;
                //Lock queue
                std::lock_guard<std::mutex> lock(m_mutex);
                m_lstMessages.push_back(&m);
            }
        }

        std::cout << __PRETTY_FUNCTION__ << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    return nullptr;
}

void TClientNode::pause()
{
    m_internalStatus = SC_STATUS::SC_PAUSE;
}

void TClientNode::start()
{
    m_internalStatus = SC_STATUS::SC_LISTENING;
}

void TClientNode::stop()
{
    m_internalStatus = SC_STATUS::SC_STOP;
}

SC_STATUS TClientNode::getStatus() const
{
    return m_internalStatus;
}



