#include "tfile.h"


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
    struct stat buf{};
    return (stat(file.c_str(), &buf) == 0);
}

bool TFile::readByLine(std::vector<std::string>& vLines)
{
    if (false == fileExists(m_filename))
    {
        return false;
    }
    vLines.clear();

    std::ifstream myfile(m_filename.c_str());



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

