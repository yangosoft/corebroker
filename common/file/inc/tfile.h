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
    using StringList = std::vector<std::string>;
    TFile(const std::string& filename);
    
    bool open();
    
    static bool fileExists(const std::string& file);
    
    bool readByLine(StringList& vLines ) const;
    
    bool writeLines(const StringList& vLines, bool append = false ) const;
    bool writeData(const char* buffer, const size_t size, bool append = false ) const;
    
    
private:
    std::string m_filename;
    
    
};

#endif
