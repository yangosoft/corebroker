/* 
 * File:   tcan.h
 * Author: dzayas
 *
 * Created on 20 de agosto de 2014, 8:03
 */

#ifndef TCAN_H
#define	TCAN_H

#include <string>

#include <linux/can.h>
#include <linux/can/raw.h>

class TCan {
    
    
public:
    TCan();
    bool openCAN(const std::string& port);
    
    //Blocks until a message is read
    bool readMessage(can_frame& message);
    
    bool sendMessage(const can_frame& message );
    
    bool isReady();
    
    void closeCAN();
    
    virtual ~TCan();
    
    
private:
    
    
    
    
    std::string m_port;
    int m_fdSocket;
    
    bool m_opened;
    
    
    TCan(const TCan& orig);

};

#endif	/* TCAN_H */

