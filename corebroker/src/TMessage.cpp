/* 
 * File:   TMessage.cpp
 * Author: daerobotics
 * 
 * Created on 8 de julio de 2014, 15:42
 */

#include "TMessage.h"
#include <iostream>
#include <sstream>

TMessage::TMessage(const std::string& messageName, uint32_t from, uint32_t to)
{
    m_from = from;
    m_to = to;
    m_parseCorrect = false;
    m_name = messageName;

}

/*TMessage::TMessage(TMessage&& orig)
{
    m_from = orig.m_from;
    m_to   = orig.m_to;
    m_name = orig.m_name;
    m_parseCorrect = orig.m_parseCorrect;
    m_rawData = orig.m_rawData;
    m_jsonReader = orig.m_jsonReader;
    m_jsonRep = orig.m_jsonRep;
}

TMessage& TMessage::operator= (const TMessage& orig)
{
    m_from = orig.m_from;
    m_to   = orig.m_to;
    m_name = orig.m_name;
    m_parseCorrect = orig.m_parseCorrect;
    m_rawData = orig.m_rawData;
    m_jsonReader = orig.m_jsonReader;
    m_jsonRep = orig.m_jsonRep;
    return *this;
}

TMessage& TMessage::operator= (TMessage&& orig)
{
    m_from = orig.m_from;
    m_to   = orig.m_to;
    m_name = orig.m_name;
    m_parseCorrect = orig.m_parseCorrect;
    m_rawData = orig.m_rawData;
    m_jsonReader = orig.m_jsonReader;
    m_jsonRep = orig.m_jsonRep;
    return *this;
}*/


TMessage TMessage::fromRawData(const std::string& rawData, bool& parsedOk)
{
    std::stringstream ss(rawData);
    std::string item;

    bool insert = false;
    bool ok = false;
    std::string data = "";

    while (std::getline(ss, item, '\n'))
    {
        if (M_SENTINEL_BEGIN == item)
        {
            std::cout << " FOUND SENITEL BEGIN" << std::endl;
            ok = true;
            insert = true;
            continue;
        }


        if (M_SENTINEL_END == item)
        {
            std::cout << " FOUND SENITEL END" << std::endl;
            ok = ok & true;
            break;
        }

        if (true == insert)
        {
            data = data + "\n" + item;
        }


    }

    std::cout << " PRE PARSED \n" << data << std::endl;
    TMessage m("error", 0, 0);
    m.setRawData(data);
    ok = ok & m.parseData();
    parsedOk = ok;

    std::cout << " * FROM " << m.getFrom() << std::endl;
    std::cout << " * TO " << m.getDestination() << std::endl;


    return m;
}

void TMessage::setRawData(const std::string& rawData)
{
    m_rawData = rawData;
}

bool TMessage::parseData()
{

    


    bool result = m_jsonReader.parse(m_rawData, m_jsonRep, false);
    std::cout << "SUCCESS? " << result << std::endl;

    if (true == result)
    {
        Json::Value defaultVal;


        std::cout << m_jsonRep.get("message_name", defaultVal).asString() << std::endl;


        m_name = m_jsonRep.get("message_name", defaultVal).asString();


        std::cout << "MEsasge from: " << m_jsonRep.get("message_from", defaultVal).asString() << std::endl;


        m_from = m_jsonRep.get("message_from", defaultVal).asUInt();


        std::cout << "Mesasge to: " << m_jsonRep.get("message_to", defaultVal).asString() << std::endl;


        m_to = m_jsonRep.get("message_to", defaultVal).asUInt();



    }



    return result;
}

uint32_t TMessage::getFrom()
{
    return m_from;
}

uint32_t TMessage::getDestination()
{
    return m_to;
}

bool TMessage::readValue(const std::string& key, std::string& value)
{

     Json::Value defaultVal;

    std::cout << m_jsonRep.get(key, defaultVal).asString() << std::endl;

    value = m_jsonRep.get(key, value).asString();
    
    return true;
}

TMessage::TMessage(const TMessage& orig)
{
    m_from = orig.m_from;
    m_to = orig.m_to;
    m_name = orig.m_name;
    m_parseCorrect = orig.m_parseCorrect;
    m_rawData = orig.m_rawData;
    m_jsonReader = orig.m_jsonReader;
    m_jsonRep = orig.m_jsonRep;
}

TMessage::~TMessage()
{
}

