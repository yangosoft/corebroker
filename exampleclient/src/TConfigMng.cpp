/* 
 * File:   TConfigMng.cpp
 * Author: daerobotics
 * 
 * Created on 8 de julio de 2014, 12:27
 */



#include <string>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include "file/inc/tfile.h"
#include <vector>
#include <iostream>
#include <ostream>
#include <iterator>
#include <sstream>


#include "TConfigMng.h"
#include <sys/stat.h>

using namespace std;

TConfigMng::TConfigMng(const std::string& filename)
{
    Json::Reader r;
    Json::Value val;

    m_filename = filename;


    std::string s = "{ \"pepe\": \"hola\" }";
    bool result = r.parse(s, val, false);
    std::cout << "SUCCESS? " << result << std::endl;

    Json::Value defaultVal;
    std::cout << val.get("pepe", defaultVal).asString() << std::endl;
    m_parseCorrect = false;
}

bool TConfigMng::readConfiguration()
{
    if (false == TFile::fileExists(m_filename))
    {
        return false;
    }

    TFile myfile(m_filename);
    std::vector<std::string> vLines;
    bool ok = myfile.readByLine(vLines);

    if (true == ok)
    {
        std::stringstream res;
        std::copy(vLines.begin(), vLines.end(), std::ostream_iterator<std::string>(res, "\n"));
        m_jsonDoc = res.str();
        std::cout << "Internal rep: " << m_jsonDoc << std::endl;

        m_parseCorrect = m_jsonReader.parse(m_jsonDoc, m_jsonRep, false);
        std::cout << "SUCCESS? " << m_parseCorrect << std::endl;

        Json::Value defaultVal;
        std::cout << "Puerto: " << m_jsonRep.get("port", defaultVal).asString() << std::endl;
        std::cout << "Broker id: " << m_jsonRep.get("broker_id", defaultVal).asUInt() << std::endl;
    }

    return ok;
}

bool TConfigMng::getParamValue(const std::string& key, uint32_t& value)
{
    if (false == m_parseCorrect)
    {
        return false;
    }

    if (false == m_jsonRep.isMember(key))
    {
        return false;
    }

    Json::Value defaultVal;

    value = m_jsonRep.get(key, defaultVal).asUInt();
    return false;
}

bool TConfigMng::getParamValue(const std::string& /*key*/, std::string& /*value*/)
{
    if (false == m_parseCorrect)
    {
        return false;
    }

    return false;
}

TConfigMng::TConfigMng(const TConfigMng&/* orig*/)
{
}

TConfigMng::~TConfigMng()
{
}

