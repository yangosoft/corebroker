/* 
 * File:   TSocketServer.cpp
 * Author: daerobotics
 * 
 * Created on 8 de julio de 2014, 14:46
 */

#include "TSocketServer.h"
#include <iostream>
#include <boost/asio.hpp>
#include <stdint.h>
#include <thread>

using boost::asio::ip::tcp;



TSocketServer::TSocketServer(uint32_t port)
{
    std::cout << __FUNCTION__ << std::endl;
    this->m_port = port;
    m_connectedNodeId = 0;
}

void TSocketServer::operator()() 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), m_port));

        for (;;)
        {
            tcp::socket*  socket = new tcp::socket(io_service);
            acceptor.accept(*socket);
            
            TNodeClient* n = new TNodeClient(*socket);
            n->setNodeId(m_connectedNodeId);
            m_connectedNodeId++;
            
            
            m_lstNodes.push_back(n);
            
            
            std::cout << "Accepted client " << socket->remote_endpoint().address().to_string() << std::endl;
            //std::string message = "hello\n";
            std::thread* thread1 = new std::thread( std::ref( *n ) );
            m_lstThreads.push_back( thread1 );
            //boost::system::error_code ignored_error;
            //boost::asio::write(socket, boost::asio::buffer(message),   boost::asio::transfer_all(), ignored_error);
            //socket.close();
            
        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}

void TSocketServer::Pause()
{
    std::cout << __FUNCTION__ << std::endl;
}

void TSocketServer::Start()
{
    std::cout << __FUNCTION__ << std::endl;
    
}

void TSocketServer::Stop()
{
    std::cout << __FUNCTION__ << std::endl;
}




TSocketServer::TSocketServer(const TSocketServer& orig)
{
    std::cout << __FUNCTION__ << std::endl;
}

TNodeList& TSocketServer::getNodeList()
{
    return m_lstNodes;
}

TSocketServer::~TSocketServer()
{
     std::list<std::thread*>::iterator it;
     std::list<TNodeClient*>::iterator jt;
     for( jt = m_lstNodes.begin(); jt != m_lstNodes.end(); jt++ )
     {
         (*jt)->Stop();
     }
     
     for( it = m_lstThreads.begin(); it != m_lstThreads.end(); it++ )
     {
         
         (*it)->join();
     }
     
     //TODO: FALTA DELETE de ambas listas
     
}

