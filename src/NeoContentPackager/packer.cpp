#include "NeoContentPackager/packer.h"
#include "NeoContentPackager/lib/format.h"

#include <string>
#include <fstream>
#include <vector>

namespace NeoContentPacker  // Interal
{
    void NCP_PACKER::_packFiles(const std::string& outputdir)
    {
        // Gentlemen, I may not have a brain but I have a idea.
        // Instead of loading it all to memory then writeing in bulk like many of the documents I found,
        // Were going to write the files as we go, so only one file is in memory at once. Then steal the moon
        std::ifstream target_list(_fileListPath);
        std::ofstream target_ouput(outputdir + ".ncpk", std::ios::binary);
        if(!target_list.is_open()) { printf("[NCP_PACKER] [ERROR] Faild to open FileList\n"); return; }
        if(!target_ouput.is_open()) { printf("[NCP_PACKER] [ERROR] Faild to create package\n"); return; }

        // Header Creation
        NCPK_header header = {};
        header.file_count = 0;
        header.entryTable_offset = 0;
        header.format_version = 1;
        header.content_version =  1;

        // Load into memory & write files individually
        std::vector<NCPK_fileEntry> entries;
        std::string filePath;

        while(std::getline(target_list, filePath))
        {
            std::ifstream targetFile(filePath, std::ios::ate | std::ios::binary);
            if(!targetFile.is_open()) { printf("[NCP_PACKER] [ERROR] Faild to open file: %s\n", filePath.c_str()); return; }

            // Create Entry
            NCPK_fileEntry fileEntry = {};
            fileEntry.uncompressedSize = (uint32_t)targetFile.tellg();
            fileEntry.offset = (uint32_t)target_ouput.tellp();
            fileEntry.isCompressed = (fileEntry.uncompressedSize > _compressionBias && _compress == true);

            // Read to memory
            targetFile.seekg(0);
            std::vector<char> fileData;
            fileData.resize(fileEntry.uncompressedSize);
            targetFile.read(fileData.data(), fileEntry.uncompressedSize);

            // Compress
            if(fileEntry.isCompressed)
            {
                // TODO
            }else 
            {
                fileEntry.compressedSize = fileEntry.uncompressedSize;
                target_ouput.write(fileData.data(), fileEntry.uncompressedSize);
            }

            // Add to entires
            printf("[NCP_PACKER] [STATUS] Written: %s [%s]\n", filePath.c_str(), std::to_string(fileEntry.compressedSize).c_str());

            entries.push_back(fileEntry);
            targetFile.close();
        }

        // Write fileTable
        header.entryTable_offset = (uint32_t)target_ouput.tellp();
        for(const auto& entry : entries) { target_ouput.write(reinterpret_cast<const char*>(&entry), sizeof(NCPK_fileEntry)); }

        // Write Header
        header.file_count = (uint32_t)entries.size();
        target_ouput.write(reinterpret_cast<const char*>(&header), sizeof(NCPK_header));

        printf("[NCP_PACKER] [COMPLETION] Created package : %s.ncpk\n", outputdir.c_str());
        target_ouput.close();
    }
}

namespace NeoContentPacker  // External
{
    NCP_PACKER& NCP_PACKER::encryptHeader(int key) {_encryptionKey = key; return *this;}
    NCP_PACKER& NCP_PACKER::compressFiles(int threshold) {_compress = true; _compressionBias = threshold; return *this;}

    NCP_PACKER& NCP_PACKER::target_fileList(const std::string& list) {_fileListPath = list; return *this;}
    
    void NCP_PACKER::output(const std::string& dir) {_packFiles(dir);}
}

NeoContentPacker::NCP_PACKER NeoContentPacker::createPack() {return NeoContentPacker::NCP_PACKER();}