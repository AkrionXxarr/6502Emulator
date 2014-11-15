#include <iostream>
#include <fstream>
using namespace std;

#include "6502_Processor.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <rom name>" << endl;
        return 0;
    }

    CPU c;
    ifstream romFile;
    romFile.open(argv[1], (ifstream::in | ifstream::binary));

    if (!romFile.is_open()) { return 0; }

    u16 curByte = 0;
    u16 curAddr = 0x0200;

    for (;;)
    {
        curByte = romFile.get();
        if (romFile.eof()) { break; }
        else if (curAddr >= 0xAFFF) { return 0; }
        c.ram->Write(curAddr++, u8(curByte));
    }
    
    romFile.close();

    c.Run();
}