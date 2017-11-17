/* 
 * File:   IThread.h
 * Author: yango
 *
 * Created on 18 de julio de 2014, 19:40
 */

#ifndef ITHREAD_H
#define	ITHREAD_H

enum ITHREAD_STATUS { TH_START, TH_STOP, TH_PAUSE  };


class IThread {
public:
 
  
    IThread();
    virtual void operator()() = 0;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void stop()  = 0;
    virtual ~IThread()   = 0;

private:

};

#endif	/* ITHREAD_H */

