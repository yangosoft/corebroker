/* 
 * File:   tcan.cpp
 * Author: daerobotics
 * 
 * Created on 20 de agosto de 2014, 8:03
 */

#include "tcan.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>


#include <stdint.h>
#include <iostream>

TCan::TCan()
{
    m_opened = false;

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

    if (bind(m_fdSocket, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {

        return false;
    }

    m_opened = true;
    return true;
}

bool TCan::sendMessage(const can_frame& message)
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
        return false;
    }

    return true;

}

bool TCan::readMessage(can_frame& message)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;


    if (false == m_opened)
    {
        return false;
    }


    ssize_t recvbytes = 0;

    recvbytes = read(m_fdSocket, &message, sizeof (struct can_frame));
    if (recvbytes > -1)
    {




    } else
    {
        return false;
    }


    return true;
}

bool TCan::isReady()
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

/*int send_port(struct can_frame *frame)
{
    int retval;
   retval = write(m_fdSocket, frame, sizeof(struct can_frame));
    if (retval != sizeof(struct can_frame))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

// this is just an example, run in a thread 
void read_port()
{
    struct can_frame frame_rd;
    int recvbytes = 0;

    read_can_port = 1;
    while(read_can_port)
    {
        struct timeval timeout = {1, 0};
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(soc, &readSet);

        if (select((m_fdSocket + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (!read_can_port)
            {
                break;
            }
            if (FD_ISSET(m_fdSocket, &readSet))
            {
                recvbytes = read(m_fdSocket, &frame_rd, sizeof(struct can_frame));
                if(recvbytes)
                {
                    printf(“dlc = %d, data = %s\n”, frame_rd.can_dlc,frame_rd.data);

                }
            }
        }

    }

}

int close_port()
{
    close(m_fdSocket);
    return 0;
}



 */




TCan::TCan(const TCan& orig)
{

}

TCan::~TCan()
{
    closeCAN();
}

