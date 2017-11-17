/* 
 * File:   TSocketClient.cpp
 * Author: yango
 * 
 * Created on 18 de julio de 2014, 23:30
 */

#include "TSocketClient.h"
#include <iostream>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#include <string.h>
#include <iostream>


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

TSocketClient::TSocketClient(const std::string& IP, uint16_t port)
{

    m_host = IP;
    m_port = port;

    m_threadStatus = SC_PAUSE;




}

bool TSocketClient::tryConnect()
{
    std::cout <<  __PRETTY_FUNCTION__ << std::endl;
    int sockfd = 0;




    struct sockaddr_in serv_addr;

    //Puntero a esctructura hostent que nos permitirá 
    //resolver un nombre de host a IP
    struct hostent *he;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error : Could not create socket \n" << std::endl;
        return false;
    }

    memset(&serv_addr, '0', sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);

    // Debemos resolver el nombre del host a una IP
    he = gethostbyname(m_host.c_str());
    if (he == NULL)
    {
        std::cout << "Error gethostbyname()" << std::endl;
        return false;
    }

    serv_addr.sin_addr = *((struct in_addr *) he->h_addr);



    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
    {

        std::cout << "Error connecting socket" << std::endl;
        return false;
    }


    m_fdSocket = sockfd;

    m_thisThread = new pthread_t;

    int ret = pthread_create(m_thisThread, NULL, &TSocketClient::run_helper, this);

    if (ret < 0)
    {
        printf("could not create thread");

    }

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
            m_threadStatus = SC_STOP;
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


    while (SC_STOP != m_threadStatus)
    {
        while (SC_PAUSE == m_threadStatus)
        {
             //std::cout << __PRETTY_FUNCTION__ << " <-> PAUSED " << std::endl;

            sleep(5);
        }




        std::cout << __PRETTY_FUNCTION__ << std::endl;
        sleep(1);


    }

}

void* TSocketClient::run()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    (*this)();
    std::cout << __PRETTY_FUNCTION__ << ": end run! " << std::endl;
    return 0;
}

void TSocketClient::pause()
{
    m_internalStatus = SC_PAUSE;
}

void TSocketClient::start()
{
    m_internalStatus = SC_LISTENING;
}

void TSocketClient::stop()
{
    m_internalStatus = SC_STOP;
}

int32_t TSocketClient::getStatus()
{
    return m_internalStatus;
}

TSocketClient::~TSocketClient()
{
}

