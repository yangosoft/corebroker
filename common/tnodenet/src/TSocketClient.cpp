/*
 * File:   TSocketClient.cpp
 * Author: yango
 *
 * Created on 18 de julio de 2014, 23:30
 */

#include "TSocketClient.h"
#include <iostream>



#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <ctime>
#include <pthread.h>
#include <cstring>
#include <iostream>


#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>

#include <chrono>
#include <thread>

TSocketClient::TSocketClient(const std::string& IP, uint16_t port): 
        m_host(IP),
        m_port(port), 
        m_initialized(false),
        m_thisThread(&TSocketClient::run, this),
        m_threadStatus(ITHREAD_STATUS::TH_PAUSE)
{
}

bool TSocketClient::tryConnect()
{
    std::cout <<  __PRETTY_FUNCTION__ << std::endl;
    int sockfd = 0;




    struct sockaddr_in serv_addr{};

    //Puntero a esctructura hostent que nos permitirá
    //resolver un nombre de host a IP
    struct hostent *he;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error : Could not create socket \n" << std::endl;
        return false;
    }

    

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);

    // Debemos resolver el nombre del host a una IP
    he = gethostbyname(m_host.c_str());
    if (he == nullptr)
    {
        std::cout << "Error gethostbyname()" << std::endl;
        return false;
    }

   
    serv_addr.sin_addr = *(reinterpret_cast<struct in_addr *>(he->h_addr));
    
    if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof (serv_addr)) < 0)
    {
        std::cout << "Error connecting socket" << std::endl;
        return false;
    }

    m_fdSocket = sockfd;
    m_initialized = true;

    return true;
}

ssize_t TSocketClient::writeData(const char *data, uint32_t size)
{
    //PAUSE m_thread
    ssize_t n = write(m_fdSocket, data, size);

    return n;
}

bool TSocketClient::writeData(const TMessage& m)
{
    std::string buffer = m.toString();
    ssize_t size = buffer.length();
    ssize_t sizeSent = writeData(buffer.c_str(), size);

    return ( sizeSent == size);

}

TMessage TSocketClient::readMessage(bool &ok)
{
    std::cout <<  __PRETTY_FUNCTION__ << std::endl;
    ok = false;
    char buffer[256];
    bool messageFound = false;
    TMessage m("", 0, 0);
    std::string strBuffer;
    while (false == messageFound)
    {

        ssize_t len = read(m_fdSocket, buffer, sizeof (buffer));
        if (len <= 0)
        {
            //Client disconnected

            std::cout << "Client " << m_fdSocket << " disconnected" << std::endl;
            m_threadStatus = ITHREAD_STATUS::TH_STOP;
            close(m_fdSocket);
            break;

        }

        strBuffer = strBuffer + std::string(buffer);
        memset(buffer, '\0', sizeof (buffer));

        if ((strBuffer.find(M_SENTINEL_BEGIN) != std::string::npos) &&
                (strBuffer.find(M_SENTINEL_END) != std::string::npos))
        {
            std::cout << " BUFFER: " << strBuffer << std::endl;
            m = TMessage::fromRawData(strBuffer,messageFound);



            strBuffer = "";
        }
    }
    ok = messageFound;
    return m;

}

void TSocketClient::operator()()
{

    while(!m_initialized)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    while (ITHREAD_STATUS::TH_STOP != m_threadStatus)
    {
        while (ITHREAD_STATUS::TH_PAUSE == m_threadStatus)
        {
            //std::cout << __PRETTY_FUNCTION__ << " <-> PAUSED " << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        std::cout << __PRETTY_FUNCTION__ << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

void* TSocketClient::run()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    (*this)();
    std::cout << __PRETTY_FUNCTION__ << ": end run! " << std::endl;
    return nullptr;
}

void TSocketClient::pause()
{
    m_internalStatus = SC_STATUS::SC_PAUSE;
}

void TSocketClient::start()
{
    m_internalStatus = SC_STATUS::SC_LISTENING;
}

void TSocketClient::stop()
{
    m_internalStatus = SC_STATUS::SC_STOP;
}

SC_STATUS TSocketClient::getStatus() const
{
    return m_internalStatus;
}



