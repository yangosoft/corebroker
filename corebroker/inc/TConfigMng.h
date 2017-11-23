/* 
 * File:   TConfigMng.h
 * Author: daerobotics
 *
 * Created on 8 de julio de 2014, 12:27
 */

#ifndef TCONFIGMNG_H
#define	TCONFIGMNG_H

#include <string>
#include <json/json.h>
#include <stdint.h>

class TConfigMng {
public:
    TConfigMng(const std::string& filename);

    bool readConfiguration();


    virtual ~TConfigMng();
    
    bool getParamValue(const std::string& key, std::string& value);
    bool getParamValue(const std::string& key, uint32_t& value);
    
    
private:

    std::string m_filename;
    std::string m_jsonDoc;

    bool m_parseCorrect;

    Json::Reader m_jsonReader;
    Json::Value m_jsonRep;

    TConfigMng(const TConfigMng& orig);


};

#endif	/* TCONFIGMNG_H */

