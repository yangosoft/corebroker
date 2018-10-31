/* 
 * File:   tcan.cpp
 * Author: daerobotics
 * 
 * Created on 20 de agosto de 2014, 8:03
 */

#include "tcan.h"
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>


#include <cstdint>
#include <iostream>
#include <stdexcept>

TCan::TCan(): m_opened(false)
{
    
}

bool TCan::openCAN(const std::string& port)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    struct ifreq ifr;
    struct sockaddr_can addr;

    m_port = port;
    m_opened = false;

    /* open socket */
    m_fdSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_fdSocket < 0)
    {
        return false;
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port.c_str());
    

    if (ioctl(m_fdSocket, SIOCGIFINDEX, &ifr) < 0)
    {
        return false;
    }

    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(m_fdSocket, F_SETFL, O_NONBLOCK);

    if (bind(m_fdSocket, reinterpret_cast<struct sockaddr *>( &addr ), sizeof (addr)) < 0)
    {
        return false;
    }

    m_opened = true;
    return true;
}

bool TCan::sendMessage(const can_frame& message) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if (false == m_opened)
    {
        return false;
    }

    ssize_t retval;
    retval = write(m_fdSocket, &message, sizeof (struct can_frame));
    if (retval != sizeof (struct can_frame))
    {
        /* TODO change to exceptions and avoid returning booleans as error codes.
        std::logic_error err("Cannot send the CAN message " + message.can_id );
        throw err;
        */
        return false;
    }

    return true;

}

bool TCan::readMessage(can_frame& message) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;


    if (false == m_opened)
    {
        return false;
    }


    ssize_t recvbytes = 0;

    recvbytes = read(m_fdSocket, &message, sizeof (struct can_frame));
    if (recvbytes <= -1)
    {
        //no data
        return false;
    }


    return true;
}

bool TCan::isReady() const
{
    return m_opened;
}

void TCan::closeCAN()
{
    if (m_opened)
    {
        close(m_fdSocket);
    }

    m_opened = false;
}


TCan::~TCan()
{
    closeCAN();
}

