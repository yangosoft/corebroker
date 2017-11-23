/* 
 * File:   TSocketClient.h
 * Author: daerobotics
 *
 * Created on 14 de julio de 2014, 11:43
 */

#ifndef TSOCKETCLIENT_H
#define	TSOCKETCLIENT_H

#include "IThread.h"

#include <string>
#include <stdint.h>

#include <boost/asio.hpp>

#include "TNode.h"

class TSocketClient : public IThread {
public:
    TSocketClient(const std::string& IP, uint16_t port );
    
   
    
    bool connect();
    
    
    virtual void operator()();
    virtual void Start();
    virtual void Pause();
    virtual void Stop();
    virtual ~TSocketClient();
    
    TNode& getNode();
    
private:



    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::socket m_socket;
    
    std::thread* m_nodeThread;
    
    std::string m_host;
    uint16_t                 m_port;
    
    TNode       *m_node;

};

#endif	/* TSOCKETCLIENT_H */

