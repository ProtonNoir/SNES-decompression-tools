#include <iostream>
#include <fstream>

void print(int argc, char** argv)
{
    using namespace std;

    cout << "GHOUL PATROL (LUCASARTS) SNES COMPRESSOR/DECOMPRESSOR" << endl;
    cout << "Version: alpha" << endl;
    cout << "Date: 2015.01.XX" << endl;
    cout << "Author: proton" << endl;
    cout << "Usage to decompress:" << endl;
    cout << "ghoul_patrol.exe d [input_file] [output_file] [offset]" << endl;
    cout << "Usage to compress:" << endl;
    cout << "1) input file to output" << endl;
    cout << "ghoul_patrol.exe c [input_file] [output_file]" << endl;
    cout << "2) input file and insert it in romfile to offset" << endl;
    cout << "ghoul_patrol.exe c [input_filename] [rom_filename] [offset]" << endl;
    cout << "Example of usage:" << endl;
    cout << "ghoul_patrol.exe d \"Ghoul Patrol (U).smc\" \"output.bin\" 0x0B0000" << endl;
}
