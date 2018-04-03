#include "memfile.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <cstdio>
#include <unistd.h>


#include <iostream>
#include <fstream>
#include <vector>
#include <system_error>



MemFile::MemFile(const std::string& filename)
{
    m_filename = filename;
}



uint8_t* MemFile::readAsBuffer() const
{
    auto fd = open(m_filename.c_str(), O_RDONLY, 0600);
	if (fd == -1) {
		auto ec = std::error_code( errno, std::system_category() );
		throw std::system_error( ec );
	}
    
	auto len = lseek( fd, 0, SEEK_END );
	if ( len == -1 ) {
		close( fd );
        auto ec = std::error_code( errno, std::system_category() );
		throw std::system_error( ec );
	}
	

	auto map = mmap( nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close( fd );
        auto ec = std::error_code( errno, std::system_category() );
		throw std::system_error( ec );
	}
	auto _data = reinterpret_cast<uint8_t*>(map);

	close( fd );
    return _data; 
}

