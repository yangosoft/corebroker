/* 
 * File:   TClientNode.h
 * Author: dzayas
 *
 * Created on 21 de julio de 2014, 13:11
 */

#ifndef TCLIENTNODE_H
#define	TCLIENTNODE_H

#include "IThread.h"
#include "TSocketClient.h"

#include <string>
#include <stdint.h>



class TClientNode : public IThread
{
    static const uint32_t THIS_NODE_ID = 1001;
    
public:
    
    
    TClientNode(const std::string& host, uint32_t port, bool nonBlocking = false);
    
    
    int32_t getStatus();
    
    bool connect();
    
    bool login();
    
    bool sendMessage(const TMessage& msg);
    TMessage readMessage(bool &ok);
    
    bool isBlocking();
    
    uint32_t getNodeId();
    void setNodeId(uint32_t nodeId);
    
    virtual void operator()();
    virtual void start();
    virtual void pause();
    virtual void stop();
    
    virtual ~TClientNode();
private:

    TSocketClient m_client;
    
    bool m_nonBlocking;
    
    std::string m_host;
    uint16_t m_port;
    
    int32_t m_fdSocket;
    pthread_t *m_thisThread;
    int32_t m_threadStatus;
    
    std::list<TMessage*> m_lstMessages;

    uint32_t m_nodeId;
    int32_t m_nodeStatus;

    std::string m_nodeName;
    int32_t m_internalStatus;


    pthread_mutex_t m_mutex;
    
    
    
    void *run( );
    
    
    
    
    static void *run_helper(void *context)
    {
        return ((TClientNode *)context)->run();
    }
    
    
};

#endif	/* TCLIENTNODE_H */

