/* 
 * File:   TSocketClient.h
 * Author: yango
 *
 * Created on 18 de julio de 2014, 23:30
 */

#ifndef TSOCKETCLIENT_H
#define	TSOCKETCLIENT_H

#include <cstdint>
#include <list>
#include <mutex>
#include <string>
#include <thread>

#include "IThread.h"
#include "TMessage.h"

enum class SC_STATUS { SC_ERROR, SC_LISTENING, SC_PAUSE, SC_STOP };

class TSocketClient : public IThread {
public:
    TSocketClient(const std::string& IP, uint16_t port);



    bool tryConnect();

    SC_STATUS getStatus() const;
    
    
    bool writeData(const TMessage& m);
    ssize_t writeData(const char *data, uint32_t size);

    TMessage readMessage(bool &ok);
    
    void giveReadSocketControl();
    
    void operator()() override;
    void start() override;
    void pause() override;
    void stop() override;
    ~TSocketClient() override = default;


private:

    std::string m_host;
    uint16_t m_port;
    
    int32_t m_fdSocket;
    
    bool m_initialized;
    std::thread m_thisThread;
    ITHREAD_STATUS m_threadStatus;
    
    std::list<TMessage*> m_lstMessages;

    uint32_t m_nodeId;

    std::string m_nodeName;
    SC_STATUS m_internalStatus;



    
    
    
    void *run( );
    
    bool checkLogin(const std::string& strMsg);
    
    
    static void *run_helper(void *context)
    {
        TSocketClient *n = static_cast<TSocketClient *>(context);
        return n->run();
    }

};

#endif	/* TSOCKETCLIENT_H */

