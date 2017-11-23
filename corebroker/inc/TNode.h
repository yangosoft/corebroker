/* 
 * File:   TNode.h
 * Author: daerobotics
 *
 * Created on 8 de julio de 2014, 15:20
 */

#ifndef TNODE_H
#define	TNODE_H

#include <boost/asio.hpp>
#include <stdint.h>
#include <TMessage.h>
#include <list>

#include <pthread.h>

#include "IThread.h"

using boost::asio::ip::tcp;

static const uint32_t SECONDS_KEEPALIVE = 30;

enum TNodeStatus { 
    NODE_CONNECTED,
    NODE_ACCEPTED,
    NODE_REJECTED,
    NODE_DISCONNECTED
};


class TNode : public IThread {
public:
    TNode(tcp::socket& socket);



    virtual void EnqueueMsg(const TMessage& msg);

    
    virtual uint32_t getNodeId();
    virtual void setNodeId(uint32_t nodeId);
    
    bool doLogin();

    virtual void operator()();

    virtual void Start();
    virtual void Pause();
    virtual void Stop();

    virtual ~TNode();

private:


    tcp::socket& m_socket;
    std::list<TMessage*> m_lstMessages;
    
    uint32_t m_nodeId;
    TNodeStatus m_nodeStatus;
    std::string m_nodeName;
    
    
    pthread_mutex_t m_mutex;
    std::chrono::seconds m_lastTimestampSent;
    
    bool sendKeepAlive();

};

#endif	/* TNODE_H */

