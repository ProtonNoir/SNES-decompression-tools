#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

unsigned int decompress (unsigned int rom_position,FILE *file_in,FILE *file_out);

int main(int argc, char *argv[])
{
    unsigned int rom_pos;
    cout << "Metal Marines Decompressor\n";
    if(argc<=1)
        {                                                                        //Keinen Wert empfangen
            rom_pos = 0x072755;
            //rom_pos = 0x072D88;
            //rom_pos = 0x0738b5;
            //rom_pos = 0x070d7c;
            //rom_pos = 0x07211f;
            //rom_pos = 0x060000; //Gameover Screen
        }
    else
        {                                                                        //Wert erhalten
            sscanf(argv[1],"%X",&rom_pos);
        }

    FILE *f1;
    FILE *f2;

    f1 = fopen("mm.smc", "rb");
    f2 = fopen("mm.bin", "w+b");

    unsigned int endrom_pos = decompress(rom_pos,f1,f2);
    cout << hex << "\n---------\nEND ROM POSITION : 0x" << (int)(endrom_pos) << "\n---------" << endl;
    fclose(f1);
    fclose(f2);

    return 0;
}

unsigned int decompress(unsigned int rom_pos_f,FILE *file1,FILE *file2)
{
    // const unsigned int BUFFER_SIZE=0x2000;
    unsigned int rpos;      //ROM Position
    unsigned int bpos=0;    //Neue Datei Position
    unsigned int totalcount = 0;
    double goodratio=0;

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

            if ((ctrl&test_bit)==test_bit)  // Wenn LSB 1 ist dann schreibe den Bytewert (unkomprimiert)
            {
                chr=fgetc(file1);
                rpos++;
                cout << "\nBytewert  = 0x";
                cout << hex << (int)chr << endl;
                fseek(file2,bpos,SEEK_SET);
                fputc(chr,file2);
                bpos++;
                totalcount++;
            }

            else                            // Ansonsten Dekomprimiere

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

                lz_off=bpos+lz2;                                 // Definiere LZ-byteposition + (halben)char
                if (lz_off>bpos) lz_off=lz_off-0x100;            // Wenn LZ-Byteposition größer als Dateiposition dann gehe 1byte zurück
                if (lz_off==bpos) lz_off=bpos-0x100;             // Wenn Datei-,  und LZ-Byteposition gleich sind, dann gehe von der Dateipostion 1Byte zurück
                cout << hex << "lz_offset = 0x"<< (int)lz_off << endl;

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

            ctrl=ctrl>>1;                                       // Schiebt alle Bits 1mal nach Rechts und füllt mit 0 auf
            cout << "CTRL: 0b" << std::bitset<8>(ctrl);
        }
        cout << " -> ENDE!" << endl;

    }
    cout << "\n\n\n\nEs wurden insgesamt: " << dec << (int)bpos << " Bytes geschrieben." << endl;
    cout << "Es wurden insgesamt: " << dec << (int)totalcount << " Bytes gelesen." << endl;
    goodratio=double(bpos)/totalcount;
    cout << "Ratio: " << dec << (double)goodratio << endl;
    return rpos;
}
