#ifndef _NCPK_FORMAT_H_
#define _NCPK_FORMAT_H_

#include <cstdint>

#define NCPK_FORMAT_VERSION 1;

namespace NeosContentPackager 
{
    namespace lib
    {
        #pragma pack(push, 1)

        struct NCPK_HEADER
        {
            //char compression_type[8] = {0};
            char ID[5] = { "NCPK" };

            int16_t format_version = 1;
            int16_t content_version = 1;

            uint32_t file_count = 0;

            uint64_t table_offset = 0;
        };

        struct NCPK_ENTRY
        {
            char virtual_path[255]  = {0};

            uint8_t is_compressed = 0; // Treated as bool, fixs some issues I previously had

            uint32_t size = 0;
            uint32_t compressed_size = 0;

            uint64_t offset = 0;
        };

        #pragma pack(pop)
    }
}

#endif /*_NCP_FORMAT_H_*/