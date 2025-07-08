#include <iostream>

#include "NeosContentPackager/packager.h"
#include "NeosContentPackager/reader.h"

using namespace NeosContentPackager;


void packageVector()
{
    packageFiles()
    .cutFilePath(0)
    .target_files({"D:/SoftwareDevelopment/Librarys/NCP/development/tests/files/femboy.png", "D:/SoftwareDevelopment/Librarys/NCP/development/tests/files/potato.png"})
    .output("examplePackage", 1);
}

void packageList()
{
    packageFiles()
    .cutFilePath(0)
    .target_fileList("D:/SoftwareDevelopment/Librarys/NCP/development/tests/files/list.txt")
    .output("examplePackage", 1);
}

void read()
{
    NCPK_READER reader;
    reader.mount("examplePackage.ncpk");

    auto data = reader.readFile("D:\\SoftwareDevelopment\\Librarys\\NCP\\development\\tests\\files\\femboy.png");

    if(!data.empty())
    {
        std::cout << "1";
        std::ofstream outFile("image.png", std::ios::binary);
        outFile.write(data.data(), data.size());
        outFile.close();
    }
}

int main()
{
    //packageList();
    read();
}