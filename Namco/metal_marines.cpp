#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

using namespace std;

unsigned int decompress (unsigned int rom_position,FILE *file_in,FILE *file_out);
bool communicate = false;

int main(int argc, char *argv[])
{
    unsigned int rom_pos;
    cout << "Metal Marines Decompressor\n";
    if(argc<=1)
        {                                                                        //No value... handle with standard adress
            rom_pos = 0x072755;
            //rom_pos = 0x072D88;
            //rom_pos = 0x0738b5;
            //rom_pos = 0x070d7c;
            //rom_pos = 0x07211f;
            //rom_pos = 0x060000; //Gameover Screen
            //rom_pos = 0x010000; //MAP Tiles
            //unsigned int rom_pos = 0x072755; //Set romposition of compressed data
            //unsigned int rom_pos = 0x06FE05; //Set romposition of compressed data
            //unsigned int rom_pos = 0x07590E; //Set romposition of compressed data
            //unsigned int rom_pos = 0x047DCA; //Set romposition of compressed data
            //unsigned int rom_pos = 0x068FAA; // Planet Intro (Mode7?)
            //unsigned int rom_pos = 0x06D6E3; // The earth
            //unsigned int rom_pos = 0x074E9D; // Graphics Intro
            //unsigned int rom_pos = 0x077BEB; // Something from the intro
        }

    else
        {                                                                        //get value
            sscanf(argv[1],"%X",&rom_pos);
        }

    FILE *f1;
    FILE *f2;

// get the char* using &writable[0] or &*writable.begin()
    f1 = fopen("mm.smc", "rb");
    f2 = fopen("mm-decompressed.smc", "w+b");

    decompress(rom_pos,f1,f2);

    fclose(f1);
    fclose(f2);

    return 0;
}

unsigned int decompress(unsigned int rom_pos_f,FILE *file1,FILE *file2)
{
    // const unsigned int BUFFER_SIZE=0x2000;
    unsigned int rpos;      //ROM Position
    unsigned int bpos=0;    //New File Position
    unsigned int totalcount = 0;
    double goodratio=0;

    rpos=rom_pos_f;
    cout << hex << "\n---------\nSTART ROM POSITION : 0x" << (int)(rpos);

    // for (i=0; i<sizeof(buf1); i++) buf1[i]=0x00;
    //  for (i=0; i<sizeof(buf2); i++) buf2[i]=0x00;

    //while (bpos_f<BUFFER_SIZE)
    while (rpos)

    {
        unsigned char chr;
        unsigned char ctrl;

        unsigned short lz_len;
        unsigned short lz_off;

        unsigned short lz1,lz2;
        unsigned char test_bit = 0x01;
        unsigned int i;


        fseek(file1,rpos,SEEK_SET);
        ctrl=fgetc(file1);
        rpos++;

        if(communicate==true)   cout << "\nBYTE CODE   0b" << std::bitset<8>(ctrl) << endl;
        if(communicate==true)   cout << "bpos      = 0x" << hex << bpos;

        if (ctrl==0x00) break; // Decompress everything?

        while (ctrl>0x01)
        {
        //As long ctrl greater than 0x01 go on.
        //->0x01 terminates

        if ((ctrl&test_bit)==test_bit)  // IF LSB 1 then write byte (uncompressed)
        {
            chr=fgetc(file1);
            rpos++;
            if(communicate==true)   cout << "\nBytevalue  = 0x";
            if(communicate==true)   cout << hex << (int)chr << endl;
            if(communicate==true)   cout << "ROM-Adress = 0x" << hex << (int)rpos << endl;
            fseek(file2,bpos,SEEK_SET);
            fputc(chr,file2);
            bpos++;
            totalcount++;
        }

        else                            // Else decompress
        {
            if(communicate==true)   cout << "\n----LZ----" << endl;
            lz1=fgetc(file1);       //first (half) Char defines length
            rpos++;
            lz2=fgetc(file1);       //second (half) Char defines ?
            rpos++;

            if(lz1 == 0) {          //Specialcase for Metal-Marines -> thanks to ManakoAT
                    lz_len = 256;
            }
            else{
                    lz_len=lz1;
            }

            if(communicate==true)   cout << "lz_length = ";
            if(communicate==true)   cout << dec << lz_len;
            if(communicate==true)   cout << " Byte" << endl;
            lz_off=bpos+lz2;                                 // Definiere LZ-byteposition + (halben)char
            if (lz_off>bpos) lz_off=lz_off-0x100;            // Wenn LZ-Byteposition größer als Dateiposition dann gehe 1byte zurück
            if (lz_off==bpos) lz_off=bpos-0x100;             // Wenn Datei-,  und LZ-Byteposition gleich sind, dann gehe von der Dateipostion 1Byte zurück
            if(communicate==true)   cout << hex << "lz_offset = 0x"<< (int)lz_off << endl;
            for (i=0; i<lz_len; i++)
                {
                    fseek(file2,lz_off+i,SEEK_SET);
                    chr=fgetc(file2);
                    fseek(file2,bpos+i,SEEK_SET);
                    fputc(chr,file2);
                }
            bpos+=lz_len;
            totalcount++;
            }

            ctrl=ctrl>>1;                                       // push one bit to the right and fills with 0
            if(communicate==true)   cout << "CTRL: 0b" << std::bitset<8>(ctrl);
        }
        if(communicate==true)   cout << " -> END!" << endl;
    }
    cout << hex << "\n  END ROM POSITION : 0x" << (int)(rpos) << "\n---------" << endl;
    cout << "TOTAL: " << dec << (int)totalcount << " Bytes read." << endl;
    cout << "TOTAL: " << dec << (int)bpos << " Bytes written." << "\n---------" << endl;
    goodratio=double(bpos)/totalcount;
    cout << "Ratio: " << dec << (double)goodratio << "\n---------" << endl;
    return rpos;
}
