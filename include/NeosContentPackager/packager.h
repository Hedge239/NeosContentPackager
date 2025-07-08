#ifndef _NCPK_PACKER_H_
#define _NCPK_PACKER_H_

#include <NeosContentPackager/lib/enumerations.h>
#include <NeosContentPackager/lib/format.h>

#include <string>
#include <vector>

namespace NeosContentPackager
{
    class NCPK_PACKAGER
    {
        // Encrption move to its own files, since its done after packing anyway
        lib::enumerations::compressionFlag _compressionFlag;
        int _compressionBias;
        bool _compress;

        std::string _pathList;
        std::vector<std::string> _pathsDirect;
        int _pathCutOff = 0;

        void _packageFile(std::ofstream& outputStream, std::string& filePath, std::vector<lib::NCPK_ENTRY>& entires);
        void _generatePackage(const std::string& outputdir, int contentVersion);

        public:
        NCPK_PACKAGER& doCompression(lib::enumerations::compressionFlag type, int threshold);

        NCPK_PACKAGER& cutFilePath(int range);

        NCPK_PACKAGER& target_fileList(const std::string& dirToList);
        NCPK_PACKAGER& target_files(const std::vector<std::string> dirTofiles);

        void output(const std::string& outputDir, int contentVersion);
    };

    NCPK_PACKAGER packageFiles();
}

#endif /*_NCPK_PACKER*/