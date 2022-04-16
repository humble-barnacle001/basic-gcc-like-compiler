#include <fstream>
#include <vector>

class FileReadWrite
{
public:
    void prepareRead(std::ifstream &file, const char *fileName);
    void prepareWrite(std::ofstream &file, const char *fileName);
};
