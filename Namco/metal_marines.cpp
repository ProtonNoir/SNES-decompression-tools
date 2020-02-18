#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

unsigned int decompress (unsigned int rom_position,FILE *file_in,FILE *file_out);

int main(int argc, char **argv)
{
    cout << "Metal Marines Decompressor\n";
    unsigned int rom_pos = 0x072755;

    FILE *f1;
    FILE *f2;

    f1 = fopen("mm.smc", "rb");
    f2 = fopen("mm.bin", "w+b");


    decompress(rom_pos,f1,f2);

    fclose(f1);
    fclose(f2);

    return 0;
}

unsigned int decompress(unsigned int rom_pos_f,FILE *file1,FILE *file2)
{
    // const unsigned int BUFFER_SIZE=0x2000;
    unsigned int rpos;      //ROM Position
    unsigned int bpos=0;    //Neue Datei Position


    rpos=rom_pos_f;
    cout << hex << "ROM POSITION : 0x" << (int)(rpos) << endl;

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

        //cout << hex << "\n BYTE CODEctrl = " << (int) ctrl;
        cout << "\nBYTE CODE   0b" << std::bitset<8>(ctrl) << endl;
        cout << "bpos      = 0x" << hex << bpos;

        if (ctrl==0x00) break;
        while (ctrl>0x01)
        {
        //Solange ctrl größer als 0x01 ist mach weiter
        //->0x01 ist Terminierungszeichen

            if ((ctrl&test_bit)==test_bit) // Wenn ctrl ungerade ist; Schreibe direkten Wert
            {
                chr=fgetc(file1);
                rpos++;
                cout << "\nBytewert  = 0x";
                cout << hex << (int)chr << endl;
                fseek(file2,bpos,SEEK_SET);
                fputc(chr,file2);
                bpos++;
            }

            else    // Wenn ctrl gerade ist; Dekomprimiere

            {
                cout << "\n----LZ----" << endl;
                lz1=fgetc(file1);       //erster (halber) Char definiert Länge
                rpos++;
                lz2=fgetc(file1);       //zweiter (halber CHar) definiert
                rpos++;

                lz_len=lz1;
                cout << "lz_laenge = ";
                cout << dec << lz_len;
                cout << " Byte" << endl;

                lz_off=bpos+lz2;                                 // Definiere byteposition + (halben)char
                if (lz_off>bpos) lz_off=lz_off-0x100;            // Wenn
                if (lz_off==bpos) lz_off=bpos-0x100;
                cout << hex << "lz_offset = 0x"<< (int)lz_off << endl;

                for (i=0; i<lz_len; i++)
                {
                    fseek(file2,lz_off+i,SEEK_SET);
                    chr=fgetc(file2);
                    fseek(file2,bpos+i,SEEK_SET);
                    fputc(chr,file2);
                }
                bpos+=lz_len;
            }

            ctrl=ctrl>>1; //shifts right and adds either 0s, if value is an unsigned type
            cout << "CTRL: 0b" << std::bitset<8>(ctrl);
        }
        cout << " -> ENDE!" << endl;
    }

    return rpos;
}
