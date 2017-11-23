/* 
 * File:   IThread.h
 * Author: daerobotics
 *
 * Created on 9 de julio de 2014, 10:35
 */

#ifndef ITHREAD_H
#define	ITHREAD_H

#include <thread>

class IThread {
public:

    
    virtual void operator()() = 0;
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual ~IThread(){};
private:
    //IThread(const IThread& other);

    // virtual IThread& operator=(const IThread& other);
};

#endif	/* ITHREAD_H */

