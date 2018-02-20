#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const uint32_t WINDOW_SIZE = 0x1000;
const uint32_t DATA_SIZE = 0x20000;
const uint8_t TEST_BIT = 0x80;
#define MAX_RAW_SIZE 31
#define MAX_MATCH_LENGTH 33
#define MAX_LONG_MATCH_LENGTH 33

using namespace std;

int main(int argc, char **argv)
{
    uint8_t in_buf[DATA_SIZE];
    uint8_t out_buf[DATA_SIZE];

    int32_t in_pos = 0;
    int32_t out_pos = 0;

    int32_t offset = 0x0a564f;

    uint8_t h1,h2;
    uint16_t comp_size;

    FILE *in_file;
    FILE *out_file;

    in_file = fopen("Flintstones, The - The Treasure of Sierra Madrock (U).smc", "rb");
    out_file = fopen("Flintstones, The - The Treasure of Sierra Madrock (U)_0x0a564f.bin", "w+b");

    fseek  (in_file,offset,SEEK_SET);
    h1=fgetc(in_file);
    offset++;
    h2=fgetc(in_file);
    offset++;

    comp_size=(h2<<8)|h1;
    cout << hex << "\nSize of decompressed data: " << (int)comp_size;

    fseek(in_file,offset,SEEK_SET);
    fread(in_buf,sizeof(uint8_t),DATA_SIZE,in_file);
    fclose(in_file);

    cout << hex << "\nStarting decompression...";

    while(out_pos<comp_size)
    {

        uint8_t ctrl;
        uint8_t lz1,lz2,lz3;
        uint16_t lz;

        uint16_t lz_len=0;
        uint16_t lz_off=0;
        uint16_t lz_dist=0;



        cout << hex << "\nInput position = [" << setw(4) << (int) in_pos << "]";
        ctrl=in_buf[in_pos++];
        cout << hex << "\tBytecode = [" << setw(4) << (int)ctrl << "]";


        for (uint16_t i=0; i<8; i++)

        {

            if ((ctrl&TEST_BIT)!=TEST_BIT)
            {
                cout << hex << "\nLITERAL = ["<< (int)in_buf[in_pos] << "]";
                cout << hex << "\tout_pos = ["<< (int)out_pos << "]";
                out_buf[out_pos++]=in_buf[in_pos++];

            }

            else

            {
                cout << hex << "\nLZ";

                lz1=in_buf[in_pos++];

                cout << hex << " LZ1 : " << (int)lz1;


                if (lz1>=0xc0)
                {


                    if (lz1>=0xf8)
                    {

                         lz2=in_buf[in_pos++];
                        lz=((lz1<<8)|lz2);

                        cout << hex << " lz = "<< (int)lz << endl;

                        if (lz>=0xff80)

                        {

                            lz_len=(((lz&0x7f)>>1)+0x11)&0xff;
                             cout << hex << " lz_len = " << (int)lz_len << endl;

                            lz_dist=((lz&0x03)+1)&0xff;
                             cout << hex << "\tlz_dist = ["<< (int)lz_dist << "]";
                              lz_off=out_pos-lz_dist;
                             cout << hex << " lz_off = " << (int)lz_off << endl;
                             cout << hex << "\tout_pos = ["<< (int)out_pos << "]";


                        }

                          else
                          {
 cout << hex << " DUMMY "<< endl;
 lz3=in_buf[in_pos++];
   cout << hex << " lz3 = "<< (int)lz3 << endl;
  lz_len=((lz>>6)&0x1f)+4;
   cout << hex << " lz_len = " << (int)lz_len << endl;
  //lz_dist=(((lz2&0x03)<<8)&lz3)+1;
  lz_dist=(((lz2&0x3f)<<8)|lz3)+1;
  
  
  cout << hex << "\tlz_dist = ["<< (int)lz_dist << "]";
                              lz_off=out_pos-lz_dist;
                             cout << hex << " lz_off = " << (int)lz_off << endl;
                             cout << hex << "\tout_pos = ["<< (int)out_pos << "]";
 
 
 
 
 
 
 cout << hex << "\tout_pos = ["<< (int)out_pos << "]";
                        	}

                    }

                    else
                    {

                        lz2=in_buf[in_pos++];

                        lz_len=((lz1&0x3c)>>2)+3;
                        cout << hex << "\tlz_len = ["<< (int)lz_len << "]";
                        lz_dist=lz2+1+((lz1&0x03)<<8);
                        cout << hex << "\tlz_dist = ["<< (int)lz_dist << "]";
                        lz_off=out_pos-lz_dist;
                        cout << hex << "\tlz_off = ["<< (int)lz_off << "]";

                        cout << hex << "\tout_pos = ["<< (int)out_pos << "]";


                    }


                }


                else
                {
                    lz_len=((lz1>>5)&0x07)+2;
                    cout << hex << "\tlz_1>>5 = ["<< (int)((lz1>>5)&0x03) << "]";
                    cout << hex << "\tlz_len = ["<< (int)lz_len << "]";
                    lz_dist=lz1&0x1f;
                    cout << hex << "\tlz_dist = ["<< (int)lz_dist << "]";
                    lz_off=out_pos-lz_dist-1;
                    cout << hex << "\tlz_off = ["<< (int)lz_off << "]";
                }

                for (int j=0; j<lz_len; j++)

                {

                    out_buf[out_pos++]=out_buf[lz_off++];


                }

            }

            ctrl<<=1;
        }

    }



    cout << hex << "End of compression at" << (int)(offset+in_pos+1);

    fwrite(out_buf,1,out_pos,out_file);
    fclose(out_file);

    return 0;

}
