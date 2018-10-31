/* 
 * File:   TMessage.cpp
 * Author: yango
 * 
 * Created on 18 de julio de 2014, 21:58
 */

#include "TMessage.h"
#include "file/inc/tfile.h"



#include <iostream>
#include <iterator>
#include <iterator>
#include <sstream>
#include <vector>


TMessage::TMessage(const std::string& messageName, uint32_t from, uint32_t to)
{
    m_from = from;
    m_to = to;
    m_parseCorrect = false;
    m_name = messageName;
}



void TMessage::setFrom(uint32_t from)
{
    m_from = from;
}

void TMessage::setDestination(uint32_t to)
{
    m_to = to;
}



TMessage TMessage::fromFile(const std::string& path, bool& parsedOk)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    TMessage m("error", 0, 0);
    parsedOk = false;
    
    if( false == TFile::fileExists(path) )
    {
     return m;
    }
    
    TFile tfile(path);
    std::vector<std::string> vLines;
    bool ok = tfile.readByLine(vLines);
    
    if ( true == ok )
    {
        std::stringstream res;
        std::copy(vLines.begin(), vLines.end(), std::ostream_iterator<std::string>(res, "\n"));
        std::string raw = res.str();
        
        m = TMessage::fromRawData(raw,parsedOk);
    }
    
    parsedOk = ok;
    
    return m;
}

TMessage TMessage::fromRawData(const std::string& rawData, bool& parsedOk)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << " *rawData: " << rawData << std::endl;
    if (true == rawData.empty())
    {
        TMessage m("error", 0, 0);

        parsedOk = false;
        return m;
    }

    std::stringstream ss(rawData);
    std::string item;

    bool insert = false;
    bool ok = false;
    std::string data;

    bool firstLine = true;
    while (std::getline(ss, item, '\n'))
    {
        if( true == firstLine)
        {
            if ( item == "{" )
            {
                std::cout << "Message without sentinels!" << std::endl;
                ok = true;
                insert = true;
            }
            firstLine = false;
        }
        
        
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
    ok = ok && m.parseData();
    parsedOk = ok;

    std::cout << " * FROM " << m.getFrom() << std::endl;
    std::cout << " * TO " << m.getDestination() << std::endl;


    return m;
}

void TMessage::setRawData(const std::string& rawData)
{
    m_rawData = rawData;
}

void TMessage::setName(const std::string& msgName)
{
    m_name = msgName;
}

std::string TMessage::getName() const
{
    return m_name;
}

bool TMessage::parseData()
{


    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << "m_rawData:\n" << m_rawData << std::endl;

    bool result = m_jsonReader.parse(m_rawData, m_jsonRep, false);
    std::cout << " * SUCCESS? " << result << std::endl;

    if (true == result)
    {
        Json::Value defaultVal;


        std::cout << m_jsonRep.get("message_name", defaultVal).asString() << std::endl;


        m_name = m_jsonRep.get("message_name", defaultVal).asString();


        std::cout << "MEsasge from: " << m_jsonRep.get("message_from", defaultVal).asString() << std::endl;

        std::istringstream reader(m_jsonRep.get("message_from", defaultVal).asString());
        
        reader >> m_from;
        
        //m_from = m_jsonRep.get("message_from", defaultVal).asUInt();
        

        std::cout << "Mesasge to: '" << m_jsonRep.get("message_to", defaultVal).asString() << "'" << std::endl;
        reader.clear();
        reader.str("");
        
        reader.str(m_jsonRep.get("message_to", defaultVal).asString());
        reader >> m_to;
        if ( 0 == m_to )
        {
            std::cout << " ERROR PARSING DESTINATION. Using atoi" << std::endl;
            m_to = std::stoi(m_jsonRep.get("message_to", defaultVal).asString());
            
            std::cout << " DEstination " << m_to << std::endl;
            
        }
    }
    return result;
}

uint32_t TMessage::getFrom() const
{
    return m_from;
}

uint32_t TMessage::getDestination() const
{
    return m_to;
}

bool TMessage::readValue(const std::string& key, std::string& value)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    Json::Value defaultVal;
    std::cout << m_jsonRep.get(key, defaultVal).asString() << std::endl;

    value = m_jsonRep.get(key, defaultVal).asString();
    std::cout << " * key = value " <<  key << " = " << value << std::endl ;
    return true;
}


bool TMessage::readValue(const std::string& key, uint32_t& value)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    

    Json::Value defaultVal;
    std::cout << m_jsonRep.get(key, defaultVal).asUInt() << std::endl;

    value = m_jsonRep.get(key, defaultVal).asUInt();
    std::cout << " * key = value " <<  key << " = " << value << std::endl ;
    return true;
}


bool TMessage::writeValue(const std::string& key, uint32_t value)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    m_jsonRep[key] = value;
    return true;
}

bool TMessage::writeValue(const std::string& key, std::string& value)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    m_jsonRep[key] = value;
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

std::string TMessage::getRawData()
{
    return m_rawData;
}

std::string TMessage::toString() const
{
    std::string out = M_SENTINEL_BEGIN + "\n";
    out = out + m_jsonRep.toStyledString();
    out = out + "\n";
    out = out + M_SENTINEL_END;

    return out;
}


