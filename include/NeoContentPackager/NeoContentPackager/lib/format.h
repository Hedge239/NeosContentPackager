#ifndef _NCPK_FORMAT_H_
#define _NCPK_FORMAT_H_

#include <cstdint>

namespace NeoContentPacker
{
    #pragma pack(push, 1)

    struct NCPK_header
    {
        char format_label[5] = {"NCPK"};

        int16_t format_version = 1;
        int16_t content_version  = 0;

        uint32_t file_count = 0;

        uint64_t entryTable_offset = 0;
    };

    struct NCPK_fileEntry
    {
        char relativePath[255] = {0};

        uint8_t isCompressed = 0;

        uint32_t uncompressedSize = 0;
        uint32_t compressedSize = 0;

        uint64_t offset = 0;
    };

    #pragma pack(pop)
}

#endif /*_NCPK_FORMAT_H_*/