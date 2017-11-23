/* 
 * File:   TNodeClient.h
 * Author: daerobotics
 *
 * Created on 17 de julio de 2014, 12:31
 * 
 * This class represents a remote node client an acts as a proxy
 */

#ifndef TNODECLIENT_H
#define	TNODECLIENT_H

#include "TNode.h"

class TNodeClient : public TNode {
public:
    TNodeClient(tcp::socket& socket);
    TNodeClient(const TNodeClient& orig);



    virtual void EnqueueMsg(const TMessage& msg);


    virtual uint32_t getNodeId();
    virtual void setNodeId(uint32_t nodeId);


    virtual void operator()();

    virtual void Start();
    virtual void Pause();
    virtual void Stop();


    virtual ~TNodeClient();
private:
    tcp::socket& m_socket;
    std::list<TMessage*> m_lstMessages;

    uint32_t m_nodeId;
    TNodeStatus m_nodeStatus;
    std::string m_nodeName;


    pthread_mutex_t m_mutex;

};

#endif	/* TNODECLIENT_H */

