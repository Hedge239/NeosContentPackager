#ifndef _NCPK_PACKER_H_
#define _NCPK_PACKER_H_

#include <string>

namespace NeoContentPacker
{
    class NCP_PACKER
    {
        int _encryptionKey;
        int _compressionBias;

        bool _compress;

        std::string _fileListPath;

        void _packFiles(const std::string& outputdir);

        public:
        NCP_PACKER& encryptHeader(int key);
        NCP_PACKER& compressFiles(int threshold);

        NCP_PACKER& target_fileList(const std::string& fileList); // Provide a file with a list of files inside

        void output(const std::string& outDir);
    };

    NCP_PACKER createPack();
}

#endif /*_NCPK_PACKER_H_*/