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
    bool readMessage(can_frame& message) const;
    
    bool sendMessage(const can_frame& message ) const;
    
    bool isReady() const;
    
    void closeCAN();
    
    virtual ~TCan();
    TCan(const TCan& orig) = delete;
    
private:
    
    std::string m_port;
    int m_fdSocket;
    
    bool m_opened;
};

#endif	/* TCAN_H */

