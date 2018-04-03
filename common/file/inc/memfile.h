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
#include <cstdint>

class MemFile
{
public:
    explicit MemFile(const std::string& filename);
    
    
    uint8_t*  readAsBuffer() const;
    
private:
    std::string m_filename;
    
    
};

#endif
