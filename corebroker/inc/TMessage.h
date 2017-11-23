/* 
 * File:   TMessage.h
 * Author: daerobotics
 *
 * Created on 8 de julio de 2014, 15:42
 */

#ifndef TMESSAGE_H
#define	TMESSAGE_H

#include <stdint.h>
#include <string>
#include <json/json.h>

static const std::string M_SENTINEL_BEGIN = "#begin";
static const std::string M_SENTINEL_END = "#end";
    




class TMessage {
public:
    TMessage(const std::string& messageName, uint32_t from, uint32_t to);
    TMessage(const TMessage& orig);
    
//    TMessage& operator=( const TMessage& other );
//    TMessage( TMessage&& other );
//    TMessage& operator=( TMessage&& other );
    
    void setDestination(uint32_t to);
    uint32_t getDestination();

    void setFrom(uint32_t from);
    uint32_t getFrom();

    void setRawData(const std::string& rawData);

    static TMessage fromRawData(const std::string& rawData, bool& parsedOk);
    
    bool parseData();

    
    bool readValue(const std::string& key, std::string& value);
    bool readValue(const std::string& key, uint32_t& value);
    
    bool writeValue(const std::string& key, std::string& value);
    bool writeValue(const std::string& key, uint32_t& value);
    
    std::string toString();


    virtual ~TMessage();
private:
    

    uint32_t m_from;
    uint32_t m_to;
    std::string m_name;

    bool m_parseCorrect;

    std::string m_rawData;


    Json::Reader m_jsonReader;
    Json::Value m_jsonRep;



};

#endif	/* TMESSAGE_H */

