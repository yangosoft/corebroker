/* 
 * File:   TSocketServer.h
 * Author: yango
 *
 * Created on 18 de julio de 2014, 19:52
 */

#ifndef TSOCKETSERVER_H
#define	TSOCKETSERVER_H



#include <stdint.h>
#include <list>

#include "IThread.h"
#include "TRemoteNode.h"
#include "TSocketClient.h"
#include "TClientNode.h"
#include "TMessage.h"
#include <pthread.h>
#include <mutex>

enum class SS_STATUS { SS_ERROR, SS_LISTENING, SS_PAUSE, SS_STOP };

class TSocketServer : public IThread {
public:

    TSocketServer(uint32_t port, TClientNode* defaultGw  = nullptr);
   
    void routeMessage(const TMessage& msg);
    
    uint32_t getNodeId() const;
    void setNodeId(uint32_t nodeId);
    
    void operator()() override;
    void start() override;
    void pause() override;
    void stop() override;
    ~TSocketServer() override = default;
    
    SS_STATUS getStatus() const;
    
private:
    uint32_t m_port;
    uint32_t m_nodeId;
    pthread_t *m_thisThread;
    std::list<int> m_lstThreads;
    std::list<TRemoteNode*> m_lstNodes;
    SS_STATUS m_internalStatus;
    
    std::mutex m_mutexRoute;
    
    
    TClientNode* m_defaultGw;
    
    
    void readMessagesFromGw();
    
    
    void *run( );
      
    
    static void *run_helper(void *context)
    {
        TSocketServer *n = static_cast<TSocketServer *>(context);
        return n->run();
    }
};

#endif	/* TSOCKETSERVER_H */

