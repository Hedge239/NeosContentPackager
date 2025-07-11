#ifndef _NCPK_READER_H_
#define _NCPK_READER_H_

#include "NeosContentPackager/lib/format.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace NeosContentPackager
{
    class NCPK_READER
    {
        std::string _packagePath;
        std::unordered_map<std::string, lib::NCPK_ENTRY> _vPathMap;

        public:
        void mount(const std::string& path);
        void unmount();

        std::vector<char> readFile(const std::string& virtual_path);
        std::unique_ptr<std::istream> streamFile(const std::string& virtual_path);
    };
}

#endif /*_NCPK_READER_H_*/