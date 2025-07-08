#include "NeosContentPackager/reader.h"
#include "NeosContentPackager/lib/format.h"

#include <cstring>

namespace NeosContentPackager  // external
{
    void NCPK_READER::mount(const std::string& path)
    {
        _packageStream.open(path, std::ios::binary);
        if(!_packageStream.is_open()) { printf("[NCP_READER] [ERROR] Faild to open package : %s\n", path.c_str()); return; }
        
        // Get Header
        lib::NCPK_HEADER header;
        _packageStream.seekg(-sizeof(lib::NCPK_HEADER), std::ios::end);
        _packageStream.read(reinterpret_cast<char*>(&header), sizeof(header));

        // Validate file
        if(strcmp("NCPK", header.ID) != 0) { printf("[NCP_READER] [ERROR] Not a valid package : %s\n", path.c_str()); _packageStream.close(); return; }
        printf("[NCP_READER] [STATUS] Successfully opened : %s\n", path.c_str());

        // Seek fileTable
        _packageStream.seekg(header.table_offset, std::ios::beg);
        for(int i = 0; i < header.file_count; ++i)
        {
            lib::NCPK_ENTRY entry;
            _packageStream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
            _vPathMap[entry.virtual_path] = entry;

            printf("[NCP_READER] [STATUS] Added vPath : %s", entry.virtual_path);
        }

        printf("[NCP_READER] [SUCCESS] Mounted : %s\n", path.c_str());
        return;
    }

    void NCPK_READER::unmount()
    {
        if(!_packageStream.is_open()) { printf("[NCP_READER] [ERROR] Faild to unmount : nothing"); return; }
        
        _packageStream.close();
        _vPathMap.clear();

        printf("[NCP_READER] [SUCCESS] Unmounted Package");
        return;
    }

    std::vector<char> NCPK_READER::readFile(const std::string& vPath)
    {
        auto vFile = _vPathMap.find(vPath);
        if(vFile == _vPathMap.end()) {printf("[NCP_READER] [ERROR] Invalid VirtualPath : %s\n", vPath.c_str()); return {};}
        
        lib::NCPK_ENTRY& entry = vFile->second;

        std::vector<char> buffer(entry.compressed_size);
        _packageStream.seekg(entry.offset, std::ios::beg);
        _packageStream.read(buffer.data(), entry.compressed_size);

        if(entry.is_compressed)
        {
            //TODO
        }

        return buffer;
    }
}