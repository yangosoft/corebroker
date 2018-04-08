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
#include "INode.h"

#include <string>
#include <stdint.h>



class TClientNode : public IThread
{
   constexpr static const uint32_t THIS_NODE_ID = 1001;
    
public:
    
    
    TClientNode(const std::string& host, uint32_t port, bool nonBlocking = false);
    
    
    SC_STATUS getStatus() const;
    
    bool connect();
    
    bool login();
    
    bool sendMessage(const TMessage& msg);
    TMessage readMessage(bool &ok);
    
    bool isBlocking();
    
    uint32_t getNodeId() const;
    void setNodeId(uint32_t nodeId);
    
    void operator()() override;
    void start() override;
    void pause() override;
    void stop() override;
    
    ~TClientNode() override = default;
private:

    TSocketClient m_client;
    
    bool m_nonBlocking;
    
    std::string m_host;
    uint16_t m_port;
    
    int32_t m_fdSocket;
    pthread_t *m_thisThread;
    ITHREAD_STATUS m_threadStatus;
    
    std::list<TMessage*> m_lstMessages;

    uint32_t m_nodeId;
    TNodeStatus m_nodeStatus;

    std::string m_nodeName;
    SC_STATUS m_internalStatus;


    std::mutex m_mutex;
    
    
    
    void *run( );
    
    
    
    
    static void *run_helper(void *context)
    {
        TClientNode *n = static_cast<TClientNode *>(context);
        return n->run();
    }
    
    
};

#endif	/* TCLIENTNODE_H */

