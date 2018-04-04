/* 
 * File:   tfile.h
 * Author: dzayas
 *
 * Created on 8 de julio de 2014, 12:27
 */

#ifndef TFILE_H
#define	TFILE_H

#include <string>
#include <vector>

class TFile
{
public:
    TFile(const std::string& filename);
    
    bool open();
    
    static bool fileExists(const std::string& file);
    
    bool readByLine(std::vector<std::string>& vLines );
    
private:
    std::string m_filename;
    
    
};

#endif
