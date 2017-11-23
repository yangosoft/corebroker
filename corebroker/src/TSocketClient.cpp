/* 
 * File:   TSocketClient.cpp
 * Author: daerobotics
 * 
 * Created on 14 de julio de 2014, 11:43
 */

#include "TSocketClient.h"
#include <boost/lexical_cast.hpp>

#include <iostream>

#include <thread>

using boost::asio::ip::tcp;

TSocketClient::TSocketClient(const std::string& IP, uint16_t port) : m_socket(m_ioService)
{
    
    m_host = IP;
    m_port = port;
    m_node = NULL;
    m_nodeThread = NULL;

}

bool TSocketClient::connect()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    boost::asio::ip::tcp::resolver resolver(m_ioService); // 2
    boost::asio::ip::tcp::resolver::query query(m_host, boost::lexical_cast<std::string>(m_port)); // 3
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end; // 4



    m_socket = boost::asio::ip::tcp::socket(m_ioService);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end) // 5
    {
        
        m_socket.close();
        m_socket.connect(*endpoint_iterator++, error);
    }
    if (error)
    {// 6
        std::cout << __PRETTY_FUNCTION__ << " error conneting:" << error.message() << std::endl  ;
        return false;
    }
    
    m_node = new TNode(m_socket);
    
    m_nodeThread = new std::thread( std::ref( *m_node ) );
    

//    boost::array<char, 128> outputBuffer;
//    
//    std::string h = "HELLO WORLD!";
//    
//    size_t len = m_socket.write_some(boost::asio::buffer(h), error);
    


    return true;


}

void TSocketClient::operator()()
{
    
    while(1)
    {
           std::cout << __PRETTY_FUNCTION__ << std::endl;
           std::chrono::milliseconds dura(5000);
           std::this_thread::sleep_for(dura);
    }

}

void TSocketClient::Start()
{

}

void TSocketClient::Pause()
{

}

void TSocketClient::Stop()
{

}

TNode& TSocketClient::getNode()
{
    return *m_node;
}

TSocketClient::~TSocketClient()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    //Delete de NODE
}

