/* 
 * File:   TSocketServer.cpp
 * Author: yango
 * 
 * Created on 18 de julio de 2014, 19:52
 */

#include "TSocketServer.h"
#include "TRemoteNode.h"

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

static const uint32_t NODE_ID = 1000;

//Esta es la implementación de la función
void *new_connection(void *p)
{
  //Cada hilo tiene su propia pila
  char data_buffer[1024];
  
  int fd_client_sck = *(int*)p;
  
  printf("Cliente con file descriptor: %d\n",fd_client_sck);
  
  
  int read_len  = read(fd_client_sck, data_buffer, 1024);
	
  printf("Recibidos %d bytes\n", read_len);
	
  int i = 0;
  for ( i = 0 ; i < read_len ; i++ )
  {
    printf("%02x ", data_buffer[i]);
  }
        
  printf("\n");
        
  memset(data_buffer, '0', sizeof(data_buffer)); 
	
  data_buffer[0] = 0xF3; // Dato arbritario a enviar
	
  //Enviamos 1 byte
  write(fd_client_sck, data_buffer, 1);
	
  //Cerramos el socket de este cliente en particular
  close(fd_client_sck);
  
  //Liberamos memoria!
  free(p);
  
  
  return 0;
}



TSocketServer::TSocketServer(uint32_t port, TClientNode* defaultGw) 
{
    m_port = port;
    m_internalStatus = SS_PAUSE;
    m_defaultGw = defaultGw;
    m_nodeId = NODE_ID;
     
    if (NULL != m_defaultGw)
    {
        bool connected = m_defaultGw->connect();
        if ( false == connected )
        {
            std::cout << " * Default gw cannot be connected " << std::endl;
        }else
        {
            connected = m_defaultGw->login();
            if ( false == connected )    
            {
                std::cout << " * Default gw is not logged in" << std::endl;
            }else
            {
                std::cout << " * Default gw is logged in!" << std::endl;
            }
        }
    }
    
    
    m_thisThread = new pthread_t;
    pthread_mutex_init(&m_mutexRoute , NULL);
    int ret = pthread_create( m_thisThread , NULL ,  &TSocketServer::run_helper ,  this);
	
	if( ret < 0)
	{
		printf("could not create thread");
		
	}
    
}



void TSocketServer::setNodeId(uint32_t nodeId)
{
    m_nodeId = nodeId;
}

uint32_t TSocketServer::getNodeId()
{
    return m_nodeId;
}


void* TSocketServer::run()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    (*this)();
    std::cout << __PRETTY_FUNCTION__ << ": end run! "<< std::endl;
    return 0;
}


void TSocketServer::pause()
{
    m_internalStatus = SS_PAUSE;
}

void TSocketServer::start()
{
    m_internalStatus = SS_LISTENING;
}

void TSocketServer::stop()
{
    m_internalStatus = SS_STOP;
}

void TSocketServer::operator ()()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    while(SS_LISTENING != m_internalStatus)
    {
        std::cout << " WAITING FOR LISTENING" << std::endl;
        sleep(100);
    }
    
    int fd_sck 	 	= 0;
    int fd_client_sck   = 0;
    
    //Estructura sockaddr_in
    //contiene el tipo de socket que queremos abrir SOCK_STREAM (TCP)
    //el puerto
    //la IP (INADDR_ANY) escuchamos en cualquier interfaz de red
    struct sockaddr_in serv_addr; 
    
    if ( NULL == m_defaultGw )
    {
    
        fd_sck = socket(AF_INET, SOCK_STREAM, 0);
    }else
    {
        //Socket is non blocking!
        fd_sck = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0);
               
    }
    
    
    
    //Ponemos a 0 la memoria (en C/C++ tú eres el responsable de limpiar la memoria)
    memset(&serv_addr, '0', sizeof(serv_addr));
    

    
    //Rellenamos la estructura
    serv_addr.sin_family 	= AF_INET;
    serv_addr.sin_addr.s_addr 	= htonl(INADDR_ANY); //Importantísimo usar la familia  de funciones hton (hardware to network) 
    serv_addr.sin_port 		= htons( m_port ); 

    //bind "enlaza" la estructura al descriptor de fichero 
    int status = bind(fd_sck, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    if ( 0 != status )
    {
 
        std::cout << "Something went wrong in bind" << std::endl;
        m_internalStatus = SS_ERROR;
        return;
    }

    
    //aquí realmente ponemos a escuchar el socket (aceptamos hasta 10 clientes en espera)
    status = listen(fd_sck, 10); 
    if ( 0 != status )
    {
        
        std::cout << "Something went wrong in listen" << std::endl;
        m_internalStatus = SS_ERROR;
        return;
    }
    
    
    while(m_internalStatus != SS_STOP)
    {
        
             
        
	
        fd_client_sck = accept(fd_sck, (struct sockaddr*)NULL, NULL); 

	if( -1 != fd_client_sck )
        {
	
            TRemoteNode *n = new TRemoteNode(fd_client_sck, *this);
        
            n->start();
            m_lstNodes.push_back(n);
        }
        
        
        //Check incoming messages
        
        readMessagesFromGw();                        
        
           
        
        
        
        
	
     }
    
    //Join all threads
    
}

/**
 * When a message cames to this function all the connected nodes
 * recieves a copy. It's a broadcaster
 * @param msg message to broadcast
 */

void TSocketServer::routeMessage(const TMessage& msg)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    pthread_mutex_lock (&m_mutexRoute);
    
    if( msg.getDestination() == m_nodeId )
    {
        std::cout << " Message for me " << std::endl;
        
        
    }else
    {
    
    
    
    
    
    
    
        std::list<TRemoteNode*>::iterator it;
        bool found = false;
        for( it = m_lstNodes.begin(); it != m_lstNodes.end(); it++ )
        {
            TRemoteNode* n = (*it);
            std::cout << " MSG " << msg.getDestination() << " vs node " << n->getNodeId() << std::endl;
            if ( n->getNodeId() == msg.getDestination()  )
            {
                std::cout << "Found destination node!" << std::endl;
                found = true;
                n->EnqueueMsg(msg);
                break;
            }






        }

        if ( false == found )
        {
            std::cout << "MESSAGE DESTINATION TO DEFAULT GW..." << std::endl;
            if ( NULL != m_defaultGw )
            {
                bool sent = m_defaultGw->sendMessage(msg);

                if ( false == sent )
                {
                    std::cout << " * Message cannot be sent through default gw" << std::endl;
                }else
                {
                    std::cout << " * Message sent! " << std::endl;
                }


            }


        }
    }
    
    //For each node in node list
    //  if nodeId == message.destination
    //       node->enqueue
    
    
    pthread_mutex_unlock (&m_mutexRoute);
}


void TSocketServer::readMessagesFromGw()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if ( NULL == m_defaultGw )
    {
        return;
    }
    
    if( true == m_defaultGw->isBlocking() )
    {
            std::cout << "Default gw client is blocking!!!!" << std::endl;
            return;
    }
    
    bool ok;
    TMessage m = m_defaultGw->readMessage(ok);
    
    if( true == ok )
    {
        std::cout << " * New message from gateway" << std::endl;
        std::cout << m.toString() << std::endl;
        //Check if destination from this message is one of 
        //m_lstNodes
        std::list<TRemoteNode*>::iterator it ;
        
        for ( it = m_lstNodes.begin() ; it != m_lstNodes.end(); it++ )
        {
            TRemoteNode* n = *(it);
            
            if ( n->getNodeId() == m.getDestination() )
            {
                std::cout << "  * Destination found! " << std::endl;
                n->EnqueueMsg(m);
                return;
            }
        }
        
        
    }
        
}


int32_t TSocketServer::getStatus()
{
    return m_internalStatus;
}

TSocketServer::~TSocketServer() {
}

