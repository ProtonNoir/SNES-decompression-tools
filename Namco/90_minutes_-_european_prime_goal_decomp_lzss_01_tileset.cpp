#include <iostream>
#include <fstream>

using namespace std;

unsigned int decompress (unsigned int rom_position,FILE *file_in,FILE *file_out);

int main(int argc, char **argv)
{
    unsigned int rom_pos = 0x13e621;
    //unsigned int rom_pos = 0x19b867;
    //unsigned int rom_pos = 0x19b9b9;

    FILE *f1;
    FILE *f2;

    f1 = fopen("90 Minutes - European Prime Goal (E) [!].smc", "rb");
    f2 = fopen("90 Minutes - European Prime Goal (E) [!].bin", "w+b");


    decompress(rom_pos,f1,f2);

    cout << hex << " ROM POSITION : " << (int)(rom_pos) << endl;

    fclose(f1);
    fclose(f2);

    return 0;
}

unsigned int decompress(unsigned int rom_pos_f,FILE *file1,FILE *file2)
{
    // const unsigned int BUFFER_SIZE=0x2000;
    unsigned int rpos;
    unsigned int bpos;


    rpos=rom_pos_f;
    bpos=0;

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

        cout << hex << " BYTE CODE ctrl = " << (int) ctrl;
        cout << hex << " bpos = " << (int) bpos;

        if (ctrl==0x00) break;

        while (ctrl>0x01)
        {

            if ((ctrl&test_bit)==test_bit)
            {
                chr=fgetc(file1);
                rpos++;
                cout << hex << " LITERAL ";
                cout << hex << " = "<< (int)chr << endl;
                fseek(file2,bpos,SEEK_SET);
                fputc(chr,file2);
                bpos++;
            }

            else

            {
                cout << hex << " LZ ";
                lz1=fgetc(file1);
                rpos++;
                lz2=fgetc(file1);
                rpos++;

                lz_len=lz1;
                cout << hex << " lz_len = "<< lz_len;

                lz_off=bpos+lz2;
                if (lz_off>bpos) lz_off=lz_off-0x100;
                if (lz_off==bpos) lz_off=bpos-0x100;
                cout << hex << " lz_off = "<< lz_off << endl;

                for (i=0; i<lz_len; i++)
                {
                    fseek(file2,lz_off+i,SEEK_SET);
                    chr=fgetc(file2);
                    fseek(file2,bpos+i,SEEK_SET);
                    fputc(chr,file2);
                }
                bpos+=lz_len;
            }
            ctrl=ctrl>>1;
        }
    }

    return rpos;
}

