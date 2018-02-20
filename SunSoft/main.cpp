#include <iostream>
#include <fstream>
#include "compress_podw.h"

using namespace std;


int main(int argc, char **argv)
{
    uint8_t in_buf[DATA_SIZE]={0};
    uint8_t out_buf[DATA_SIZE]={0};

    int32_t in_pos = 0;
    int32_t out_pos = 0;


  //int32_t off = 0;
    int32_t offset = 0;

    char *off;

   // pos_t	pos;

    uint8_t h1,h2;
    uint16_t comp_size, file_size;

cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
cout << ":: GHOUL PATROL (LUCASARTS) SNES DECOMPRESSOR                              ::" << endl;
cout << ":: Version: alpha                                                        ::" << endl;
cout << ":: Date: 2014.11.11                                                      ::" << endl;
cout << ":: Author: proton                                                        ::" << endl;
cout << ":: Usage: ghoul_patrol_d.exe [input_file] [output_file] [offset]        ::" << endl;
cout << ":: Example of usage:                                                     ::" << endl;
cout << ":: ghoul_patrol_d.exe \"Final Fight 3 (U).smc\" \"output.bin\" 0x0B0000     ::" << endl;
cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

  //  if (argc < 3) {
    //	fprintf(stderr, "Parameters error");
        //exit(-1);
//}

    FILE *in_file;
    FILE *out_file;

//in_file = fopen(argv[1], "rb");
//if (in_file==NULL){fputs ("Input file error",stderr); exit (1);}

//out_file = fopen(argv[2], "wb");
//if (out_file==NULL){fputs ("Output file error",stderr); exit (1);}

//off = argv[3];
//if (off==NULL){fputs ("Offset error",stderr); exit (1);}

//sscanf (off, "%x", &offset);


//DECOMPRESSION

//in_file = fopen("Ghoul Patrol (U).smc", "rb");
//out_file = fopen("Ghoul Patrol (U)_0x0b0000.bin", "w+b");
//offset=0x0b0000;

/*
in_file = fopen("Ghoul Patrol (U)_0x0b0000_comp.bin", "rb");
out_file = fopen("Ghoul Patrol (U)_0x0b0000_decomp.bin", "w+b");

offset=0x0;
    //fseek  (in_file,offset,SEEK_SET);
    fseek  (in_file,offset,SEEK_SET);
    h1=fgetc(in_file);
    offset++;
    h2=fgetc(in_file);
    offset++;

    comp_size=(h2<<8)|h1;
    cout << hex << "\nSize of data to be decompressed: " << (int)comp_size;
    cout << hex << "\nOffset: " << (int)offset;

    fseek(in_file,offset,SEEK_SET);
    fread(in_buf,sizeof(uint8_t),comp_size,in_file);
    fclose(in_file);

    cout << hex << "\nStarting decompression...";

    out_pos=decompress(in_pos, in_buf, out_pos, out_buf, comp_size);

    fwrite(out_buf,sizeof(uint8_t),out_pos,out_file);
    fclose(out_file);
    return 0;
*/


    //COMPRESSION


    in_file = fopen("Pirates of Dark Water, The (U)_norm.bin", "rb");
    out_file = fopen("Pirates of Dark Water, The (U)_norm_comp.bin", "wb");
    fseek  (in_file,0,SEEK_END);
    file_size=ftell(in_file);

    cout << hex << "Size of data to be compressed: " << (int)file_size << endl;

    fseek  (in_file,0,SEEK_SET);
    fread(in_buf,sizeof(uint8_t),file_size,in_file);
    fclose(in_file);

    cout << hex << "Starting compression...";
    out_pos=compress(in_buf, out_buf, file_size);

    cout << hex << "Size of compressed data: " << (int)(out_pos) << endl;

    fwrite(out_buf,1,out_pos,out_file);
    fclose(out_file);
    return 0;





}


