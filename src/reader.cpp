#include "NeosContentPackager/reader.h"
#include "NeosContentPackager/lib/format.h"

#include <cstring>
#include <fstream>

namespace NeosContentPackager  // external
{
    void NCPK_READER::mount(const std::string& path)
    {
        std::ifstream packageStream(path, std::ios::binary);
        if(!packageStream.is_open()) { printf("[NCP_READER] [ERROR] Faild to open package : %s\n", path.c_str()); return; }
        
        _packagePath = path;

        // Get Header
        lib::NCPK_HEADER header;
        packageStream.seekg(-sizeof(lib::NCPK_HEADER), std::ios::end);
        packageStream.read(reinterpret_cast<char*>(&header), sizeof(header));

        // Validate file
        if(strcmp("NCPK", header.ID) != 0) { printf("[NCP_READER] [ERROR] Not a valid package : %s\n", path.c_str()); packageStream.close(); return; }
        printf("[NCP_READER] [STATUS] Successfully opened : %s\n", path.c_str());

        // Seek fileTable
        packageStream.seekg(header.table_offset, std::ios::beg);
        for(int i = 0; i < header.file_count; ++i)
        {
            lib::NCPK_ENTRY entry;
            packageStream.read(reinterpret_cast<char*>(&entry), sizeof(entry));
            _vPathMap[entry.virtual_path] = entry;

            printf("[NCP_READER] [STATUS] Added vPath : %s\n", entry.virtual_path);
        }

        printf("[NCP_READER] [SUCCESS] Mounted : %s\n", path.c_str());
        packageStream.close();
        return;
    }

    void NCPK_READER::unmount()
    {
        if(!_packagePath.empty()) { printf("[NCP_READER] [ERROR] Faild to unmount : nothing\n"); return; }
        
        _packagePath.clear();
        _vPathMap.clear();

        printf("[NCP_READER] [SUCCESS] Unmounted Package\n");
        return;
    }

    std::vector<char> NCPK_READER::readFile(const std::string& vPath)
    {
        std::ifstream packageStream(_packagePath, std::ios::binary); if(!packageStream.is_open()) { printf("[NCP_READER] [ERROR] Faild to open package : %s\n", _packagePath.c_str()); return {}; }
        auto vFile = _vPathMap.find(vPath); if(vFile == _vPathMap.end()) {printf("[NCP_READER] [ERROR] Invalid VirtualPath : %s\n", vPath.c_str()); return {};}
        
        lib::NCPK_ENTRY& entry = vFile->second;
        std::vector<char> buffer(entry.compressed_size);

        packageStream.seekg(entry.offset, std::ios::beg);
        packageStream.read(buffer.data(), entry.compressed_size);

        if(entry.is_compressed)
        {
            //TODO
        }

        packageStream.close();
        return buffer;
    }

    std::unique_ptr<std::istream> NCPK_READER::streamFile(const std::string& vPath)
    {
        if(_packagePath.empty()) { printf("[NCP_READER] [ERROR] No Package Defined : %s\n", _packagePath.c_str()); return {}; }
        auto vFile = _vPathMap.find(vPath); if(vFile == _vPathMap.end()) {printf("[NCP_READER] [ERROR] Invalid VirtualPath : %s\n", vPath.c_str()); return {};}

        lib::NCPK_ENTRY& entry = vFile->second;

        std::unique_ptr<std::ifstream> packageStream = std::make_unique<std::ifstream>(_packagePath, std::ios::binary);
        if(!packageStream->is_open()) { printf("[NCP_READER] [ERROR] Faild to open package : %s\n", _packagePath.c_str()); return {}; }

        packageStream->seekg(entry.offset, std::ios::beg);

        return packageStream;
    }
}