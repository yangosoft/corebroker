/* 
 * File:   INode.h
 * Author: dzayas
 *
 * Created on 21 de julio de 2014, 14:58
 */

#ifndef INODE_H
#define	INODE_H

#include <cstdint>

#ifdef	__cplusplus
extern "C" {
#endif
    
    

enum class TNodeStatus: int32_t { 
    NODE_CONNECTED,
    NODE_ACCEPTED,
    NODE_REJECTED,
    NODE_DISCONNECTED
};



#ifdef	__cplusplus
}
#endif

#endif	/* INODE_H */

