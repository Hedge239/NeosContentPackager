#include "NeosContentPackager/packager.h"
#include "NeosContentPackager/lib/format.h"
#include "NeosContentPackager/lib/enumerations.h"

#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

namespace NeosContentPackager   // Internal
{
    void NCPK_PACKAGER::_packageFile(std::ofstream& outputStream, std::string& filePath, std::vector<lib::NCPK_ENTRY>& entires)
    {
        // Gentlemen, I may not have a brain but I have a idea.
        // Instead of loading it all to memory then writeing in bulk like many of the documents I found,
        // Were going to write the files as we go, so only one file is in memory at once. Then we steal the moon!!
        std::ifstream targetStream(filePath, std::ios::ate | std::ios::binary);
        if(!targetStream.is_open()) { printf("[NCP_PACKER] [ERROR] Faild to open file: %s\n", filePath.c_str()); return; }

        // Create Entry
        lib::NCPK_ENTRY fileEntry = {};
        fileEntry.size = (uint32_t)targetStream.tellg();
        fileEntry.offset = (uint32_t)outputStream.tellp();
        fileEntry.is_compressed = (fileEntry.size > _compressionBias && _compress == true);

        // Get Reative path
        if(_pathCutOff != 0)
        {
            size_t posistion = filePath.size();
            for(int i = 0; i < _pathCutOff; ++i)
            {
                posistion = filePath.find_last_of("\\/", posistion - 1);
                if(posistion == std::string::npos) { printf("[NCP_PACKER] [ERROR] FilePath Cutoff To high!: %s\n", filePath.c_str()); return; }
            }

            std::strncpy(fileEntry.virtual_path, filePath.substr(posistion == std::string::npos ? 0 : posistion + 1).c_str(), sizeof(fileEntry.virtual_path) - 1);
        }else 
        {
            std::strncpy(fileEntry.virtual_path, filePath.c_str(), sizeof(fileEntry.virtual_path) - 1);
        }

        // Read to memory
        targetStream.seekg(0);

        std::vector<char> fileData;
        fileData.resize(fileEntry.size);
        targetStream.read(fileData.data(), fileEntry.size);

        if(fileEntry.is_compressed)
        {
            // TODO LATER, calls likes COMPRESSION::COMPRESS() or somthin
        }else 
        {
            fileEntry.compressed_size = fileEntry.size;
            outputStream.write(fileData.data(), fileEntry.size);
        }

        // Add entire to list
        printf("[NCP_PACKER] [STATUS] Written: %s | vPath: %s | Size: [%s]\n", filePath.c_str(), fileEntry.virtual_path, std::to_string(fileEntry.size).c_str());
        entires.push_back(fileEntry);
        targetStream.close();
    }

    void NCPK_PACKAGER::_generatePackage(const std::string& outputdir, int contentVersion)
    {
        std::ofstream outputStream(outputdir + ".ncpk", std::ios::binary);
        if(!outputStream.is_open()) { printf("[NCPK_PACKAGER] [ERROR] Faild to create package at: %s\n", outputdir.c_str()); return; }

        // Load, compress, and write to package, and create entry
        std::vector<lib::NCPK_ENTRY> entires;
        std::string filePath;

        if(!_pathsDirect.empty())   // Vector
        {
            for(const std::string& path : _pathsDirect)
            {
                filePath = path;
                _packageFile(outputStream, filePath, entires);
            }
        }

        if(std::filesystem::exists(_pathList))  // FileList.extension or none, idk
        {
            std::ifstream fileListStream(_pathList);
            if(!fileListStream.is_open()) { printf("[NCP_PACKER] [ERROR] Faild to open FileList: %s\n", _pathList.c_str()); return; }

            while(std::getline(fileListStream, filePath)) { _packageFile(outputStream, filePath, entires); }
        }

        // Create Header
        lib::NCPK_HEADER header = {};
        header.file_count = 0;
        header.table_offset = 0;
        header.format_version = NCPK_FORMAT_VERSION;
        header.content_version = contentVersion;

        // TODO: Set compression

        // Write fileTable/fileCount
        header.table_offset = (uint32_t)outputStream.tellp();
        for(const auto& entry : entires) { outputStream.write(reinterpret_cast<const char*>(&entry), sizeof(lib::NCPK_ENTRY)); }

        header.file_count = (uint32_t)entires.size();
        outputStream.write(reinterpret_cast<const char*>(&header), sizeof(lib::NCPK_HEADER));

        printf("[NCP_PACKAGER] [COMPLETION] Created package : %s.ncpk\n", outputdir.c_str());
        outputStream.close();
    }
}

namespace NeosContentPackager   // External
{
    // This is why wide screen rules
    NCPK_PACKAGER& NCPK_PACKAGER::doCompression(lib::enumerations::compressionFlag type, int threshold) {_compress = true; _compressionFlag = type; _compressionBias = threshold; return *this;}
    NCPK_PACKAGER& NCPK_PACKAGER::cutFilePath(int range) {_pathCutOff = range; return *this;}
    NCPK_PACKAGER& NCPK_PACKAGER::target_fileList(const std::string& dirToList) {_pathList = dirToList; return *this;}
    NCPK_PACKAGER& NCPK_PACKAGER::target_files(const std::vector<std::string> dirTofiles) {_pathsDirect = dirTofiles; return *this;}

    void NCPK_PACKAGER::output(const std::string& outputDir, int contentVersion) {_generatePackage(outputDir, contentVersion);}
}

NeosContentPackager::NCPK_PACKAGER NeosContentPackager::packageFiles() {return NeosContentPackager::NCPK_PACKAGER();}