#include <cstdio>
#include <iostream>
#include <fstream> // for std::fstream
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	std::fstream file;
    file.open("Program_Executed.txt", std::ios::out | std::ios::binary | std::ios::ate);
    file.write("IWASHERE", 25);
    file.flush();
    file.close();
    while(1)
    {
        sleep(5);
    }
    return 0;
}