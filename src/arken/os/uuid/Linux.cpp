#include <fstream>
#include <arken/base>

string os::uuid()
{
    char * buffer = new char[37];

    std::ifstream file;
    file.open("/proc/sys/kernel/random/uuid");
    file.read(buffer, 36);
    file.close();
    buffer[36] = '\0';

    return string( std::move(buffer), 36 );
}
