/* 
 * File:   TRemoteNode.h
 * Author: yango
 *
 * Created on 18 de julio de 2014, 21:45
 */

#ifndef TREMOTENODE_H
#define	TREMOTENODE_H

#include <cstdint>
#include "IThread.h"
#include "TMessage.h"
#include "INode.h"
#include <pthread.h>
#include <list>
#include <string>
#include <mutex>

static const uint32_t SECONDS_KEEPALIVE = 30;

//
class TSocketServer;

class TRemoteNode : public IThread {
public:
    TRemoteNode(int32_t fdSocket, TSocketServer& owner);
    
    TNodeStatus getNodeStatus() const;
    
    void EnqueueMsg(const TMessage& msg);


    uint32_t getNodeId() const;
    void setNodeId(uint32_t nodeId);
    
    
   
    
    bool writeData(const TMessage& m);
    ssize_t writeData(const char *data, uint32_t size);
    
    
    void operator()() override;
    void start() override;
    void pause() override;
    void stop() override;
    ~TRemoteNode() override = default;
private:
    
    int32_t m_fdSocket;
    pthread_t *m_thisThread;
    ITHREAD_STATUS m_threadStatus;
    
    std::mutex m_mutexQueueMessages;
    
    std::list<TMessage*> m_lstMessages;

    uint32_t m_nodeId;
    TNodeStatus m_nodeStatus;
    std::string m_nodeName;

    TSocketServer& m_owner;

    std::mutex m_mutex;
    
    void *run( );
    
    bool checkLogin(const std::string& strMsg);
    
    bool sendLoginAnswer(bool accepted);
    
    
    static void *run_helper(void *context)
    {
        TRemoteNode *n = static_cast<TRemoteNode *>(context);
        return n->run();
    }

};

#endif	/* TREMOTENODE_H */

