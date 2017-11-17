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

enum SS_STATUS { SS_ERROR, SS_LISTENING, SS_PAUSE, SS_STOP };

class TSocketServer : public IThread {
public:

    TSocketServer(uint32_t port, TClientNode* defaultGw  = NULL);
   
    void routeMessage(const TMessage& msg);
    
    uint32_t getNodeId();
    void setNodeId(uint32_t nodeId);
    
    void operator()();
    virtual void start();
    virtual void pause();
    virtual void stop();
    virtual ~TSocketServer();
    
    int32_t getStatus();
    
private:
    uint32_t m_port;
    uint32_t m_nodeId;
    pthread_t *m_thisThread;
    std::list<int> m_lstThreads;
    std::list<TRemoteNode*> m_lstNodes;
    int32_t m_internalStatus;
    
    pthread_mutex_t m_mutexRoute;
    
    
    TClientNode* m_defaultGw;
    
    
    void readMessagesFromGw();
    
    
    void *run( );
      
    
    static void *run_helper(void *context)
    {
        return ((TSocketServer *)context)->run();
    }
};

#endif	/* TSOCKETSERVER_H */

