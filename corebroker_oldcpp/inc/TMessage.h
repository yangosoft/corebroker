/* 
 * File:   TMessage.h
 * Author: yango
 *
 * Created on 18 de julio de 2014, 21:58
 */

#ifndef TMESSAGE_H
#define	TMESSAGE_H
#include <string>
#include <stdint.h>

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
    uint32_t getDestination() const;

    void setFrom(uint32_t from);
    uint32_t getFrom() const;
    
    void setName(const std::string& msgName);
    std::string getName() const;

    void setRawData(const std::string& rawData);
    std::string getRawData();

    static TMessage fromRawData(const std::string& rawData, bool& parsedOk);
    static TMessage fromFile(const std::string& path, bool& parsedOk);

    bool parseData();


    bool readValue(const std::string& key, std::string& value);
    bool readValue(const std::string& key, uint32_t& value);

    bool writeValue(const std::string& key, std::string& value);
    bool writeValue(const std::string& key, uint32_t& value);

    std::string toString() const;


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

