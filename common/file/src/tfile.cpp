#include "tfile.h"


#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>

TFile::TFile(const std::string& filename)
{
    m_filename = filename;
}

bool TFile::fileExists(const std::string& file)
{

    struct stat buf
    {
    };
    return (stat(file.c_str(), &buf) == 0);
}

bool TFile::readByLine(StringList& vLines) const
{
    if (false == fileExists(m_filename))
    {
        return false;
    }
    vLines.clear();

    std::ifstream myfile(m_filename);


    std::string line;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            vLines.push_back(line);
        }
        myfile.close();
    }

    return true;
}

bool TFile::writeLines(const StringList& vLines, bool append) const
{
    std::ofstream myfile;

    try
    {
        if (append == false)
        {
            myfile.open(m_filename);
        }
        else
        {
            myfile.open(m_filename, std::ios::app);
        }


        std::for_each(vLines.begin(), vLines.end(), [&myfile](auto s) {
            myfile << s << std::endl;
        });
    }
    catch(std::exception ex)
    {
        return false;
    }


    myfile.close();
    return true;
}

bool TFile::writeData(const char* buffer, const size_t size, bool append) const
{
    std::ofstream myfile;

    try
    {
        if (append == false)
        {
            myfile.open(m_filename, std::ios::binary );
        }
        else
        {
            myfile.open(m_filename, std::ios::app |std::ios::binary);
        }

        myfile.write(buffer,size);
        
    }
    catch(std::exception ex)
    {
        return false;
    }


    myfile.close();
    return true;
}

