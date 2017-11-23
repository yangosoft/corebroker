/* 
 * File:   TSocketServer.h
 * Author: daerobotics
 *
 * Created on 8 de julio de 2014, 14:46
 */

#ifndef TSOCKETSERVER_H
#define	TSOCKETSERVER_H

#include <stdint.h>
#include "TNodeClient.h"
#include <list>
#include "IThread.h"
#include <thread>

typedef  std::list<TNodeClient*> TNodeList;

class TSocketServer : public IThread {
public:
    TSocketServer(uint32_t port);
    TSocketServer(const TSocketServer& orig);

    std::list<TNodeClient*>& getNodeList();
    




    void operator()();
    virtual void Start();
    virtual void Pause();
    virtual void Stop();
    virtual ~TSocketServer();

private:

    uint32_t m_port;
    TNodeList m_lstNodes;
    std::list<std::thread*> m_lstThreads;
    
    uint32_t m_connectedNodeId;
    
    


};

#endif	/* TSOCKETSERVER_H */

