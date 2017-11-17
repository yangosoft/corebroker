/* 
 * File:   TRemoteNode.cpp
 * Author: yango
 * 
 * Created on 18 de julio de 2014, 21:45
 */

#include "TRemoteNode.h"
#include "msg/msgdef.h"

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sstream>

#include "TSocketServer.h"

TRemoteNode::TRemoteNode(int32_t fdSocket, TSocketServer& owner) :m_owner(owner)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    m_fdSocket = fdSocket;
    m_nodeId = 0;
    m_threadStatus = TH_PAUSE;
    m_thisThread = new pthread_t;
    pthread_mutex_init(&m_mutexQueueMessages , NULL);
    int ret = pthread_create(m_thisThread, NULL, &TRemoteNode::run_helper, this);

    if (ret < 0)
    {
        std::cout << "Cannot create thread!" << std::endl;

    }
}

uint32_t TRemoteNode::getNodeId()
{
    return m_nodeId;
}

void TRemoteNode::setNodeId(uint32_t nodeId)
{
    m_nodeId = nodeId;
}

void TRemoteNode::EnqueueMsg(const TMessage& msg)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    pthread_mutex_lock(&m_mutexQueueMessages);
    std::cout <<  " * Locked! " << std::endl;
    
    TMessage *m = new TMessage(msg);
    m_lstMessages.push_back(m);
    
    writeData(msg);
    
    
    
    
    
    pthread_mutex_unlock(&m_mutexQueueMessages);
}

void *TRemoteNode::run()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    m_nodeStatus = NODE_CONNECTED;
    (*this)();
   std::cout << __PRETTY_FUNCTION__ << " end run " << std::endl;
    return 0;
}


int32_t TRemoteNode::getNodeStatus()
{
    return m_nodeStatus;
}


bool TRemoteNode::checkLogin(const std::string& strMsg)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout <<  " * " << strMsg << std::endl;
    bool ok;
    TMessage m = TMessage::fromRawData(strMsg,ok);
    
    if( ok )
    {
        
        if ( m.getName() == MSG_LOGIN_NAME)
        {
            std::cout << m.getFrom() << " -> " << m.getDestination() << " - " << m.getName() << std::endl;
            m_nodeId = m.getFrom();
            
        }else
        {
            std::cout << "MESSAGE NOT LOGIN " << m.getName() << std::endl;
        }
        
        
        
        
        
        return ok;
    }
    
    return false;
}

void TRemoteNode::operator()()
{
    char buffer[2048];
    std::string strBuffer;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    bool ok = false;
    
    /* WAIT FOR LOGIN */
    while(NODE_ACCEPTED != m_nodeStatus) 
    {
        //std::cout << "Waiting login" << std::endl;
        ssize_t len = read(m_fdSocket,buffer, sizeof(buffer));
        if( len <= 0 )
        {
            //Client disconnected
            m_nodeStatus = NODE_DISCONNECTED;
            std::cout << "Client " << m_fdSocket << " disconnected" << std::endl;
            m_threadStatus = TH_STOP;
            close(m_fdSocket);
            break;
            
        }
        strBuffer = strBuffer + std::string(buffer);
        memset(buffer, '\0', sizeof(buffer)); 
        
        if( (strBuffer.find(M_SENTINEL_BEGIN) != std::string::npos ) &&
                (strBuffer.find(M_SENTINEL_END) != std::string::npos ))
        {
            std::cout << " BUFFER: " << strBuffer << std::endl;
            //Posible login message
            if ( true == checkLogin(strBuffer) )
            {
                std::cout << "NODE IS ACCEPTED!" << std::endl;
                m_nodeStatus = NODE_ACCEPTED;
            }
            strBuffer = "";
        }
        
        
    }
    
    
    
    
    if ( NODE_DISCONNECTED != m_nodeStatus )
    {
        sendLoginAnswer( (NODE_ACCEPTED == m_nodeStatus)  );
    }
    
    
    while( TH_STOP != m_threadStatus )
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if( TH_PAUSE == m_threadStatus )
        {
            std::cout << " NODE IS PAUSED! " << std::endl;
            sleep(10);
            
            continue;
        }
        std::cout << " * NODE DOES NOTHING " << std::endl;
         memset(buffer, '\0', sizeof(buffer)); 
        ssize_t len = read(m_fdSocket,buffer, sizeof(buffer));
        if( len == 0 )
        {
            //Client disconnected
            m_nodeStatus = NODE_DISCONNECTED;
            std::cout << "Client " << m_fdSocket << " disconnected" << std::endl;
            m_threadStatus = TH_STOP;
            close(m_fdSocket);
            break;
            
        }
        
        
        //std::cout << "MESSAGE: " + std::string(buffer) << std::endl;
        strBuffer = strBuffer + std::string(buffer);
        if( (strBuffer.find(M_SENTINEL_BEGIN) != std::string::npos ) &&
                (strBuffer.find(M_SENTINEL_END) != std::string::npos ))
        {
            std::cout << " BUFFER: " << strBuffer << std::endl;
            //Posible  message
            
            TMessage nm = TMessage::fromRawData(strBuffer,ok);
            
            if ( true == ok )
            {
                std::cout << "NEW MESSAGE!!!! " << nm.getName() << " from " << nm.getFrom() << " to " << nm.getDestination() << std::endl;
                
                
                if( nm.getDestination() == m_nodeId )
                {
                    std::cout  << " * This message is for me! " << std::endl;
                }else
                {
                
                    m_owner.routeMessage(nm);
                }
                
                
                
                //Send a message
//                nm.setFrom(1000);
//                nm.setDestination(m_nodeId);
//                ok = writeData(nm);
//                if ( true == ok )
//                {
//                    std::cout << "Sent a message!" << std::endl;
//                }
            }
            
            strBuffer = "";
        }
        
        if( m_lstMessages.size() > 0 )
        {
            pthread_mutex_lock(&m_mutexQueueMessages);
            TMessage *m = m_lstMessages.front();
            m_lstMessages.pop_front();
            if (NULL != m)
            {
                std::cout << "MESSAGE FROM OUTSIDE: " << m->getName()  << " from " << m->getFrom() << std::endl;
            }
            
            pthread_mutex_unlock(&m_mutexQueueMessages);   
        }
        
    }
}

bool TRemoteNode::sendLoginAnswer(bool accepted)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::stringstream ss;
    std::string msgLogin = M_SENTINEL_BEGIN;
            
    msgLogin += "\n{\n";
    msgLogin += "\"message_name\": \""+ MSG_LOGIN_ANSWER_NAME +"\",\n";
    
    ss.str("");
    ss << 1000;
    
    msgLogin += "\"message_from\": "+ ss.str()  +",\n";
    ss.str("");
    ss << m_nodeId ;
    
    msgLogin += "\"message_to\": "+ ss.str()  +",\n";
    
    
    
    msgLogin += "\"correct\": "+ (accepted? std::string("1"):std::string("0"))  +"\n";
            
    std::cout << " * Node is accepted? " << accepted << std::endl;
    
    msgLogin += "}\n";
    msgLogin += M_SENTINEL_END;
    
    bool ok;
    TMessage m = TMessage::fromRawData(msgLogin,ok);
    
    if( false == ok )
    {
        std::cout << "Cannot parse message" << std::endl;
    }
    
    
    ok = writeData(m);
    if( false == ok )
    {
        std::cout << "Cannot send message" << std::endl;
    }
    
    
    
    return ok;
}

ssize_t TRemoteNode::writeData(const char *data, uint32_t size)
{

    //PAUSE m_thread
    ssize_t n = write(m_fdSocket, data, size);

    return n;
}

bool TRemoteNode::writeData(const TMessage& m)
{
    std::string buffer = m.toString();
    ssize_t size = buffer.length();
    ssize_t sizeSent = writeData(buffer.c_str(), size);

    return ( sizeSent == size);

}




void TRemoteNode::pause()
{
    m_threadStatus = TH_PAUSE;
}

void TRemoteNode::start()
{
 m_threadStatus = TH_START;
}

void TRemoteNode::stop()
{
    m_threadStatus = TH_STOP;
}

TRemoteNode::~TRemoteNode()
{
}

