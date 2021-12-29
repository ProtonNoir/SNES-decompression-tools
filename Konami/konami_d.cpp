#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#define WINDOW_SIZE 0x400
#define DATA_SIZE 0x10000

int32_t check_pos(int32_t);

using namespace std;

int main(int argc, char **argv)
{

    uint16_t oldM1,oldM2;
    uint8_t in_buf[DATA_SIZE];
    uint8_t out_buf[DATA_SIZE];
    uint8_t win_buf[WINDOW_SIZE];

    uint32_t comp_size;
    int32_t in_pos;
    int32_t out_pos;
    int32_t buf_pos;
    uint16_t cnt;
    uint8_t ctrl;
    uint8_t chr;

    cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << ":: KONAMI SNES DECOMPRESSOR                                              ::" << endl;
    cout << ":: Version: 1.02                                                         ::" << endl;
    cout << ":: Date: 2014.10.21                                                      ::" << endl;
    cout << ":: Author: proton                                                        ::" << endl;
    cout << ":: Usage: konami_d.exe [ROM_file] [offset] [game_type]                   ::" << endl;
    cout << ":: Example of usage:                                                     ::" << endl;
    cout << ":: konami_d.exe \"Adventures of Batman & Robin, The (U).smc\" 0xF8000 1    ::" << endl;
    cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    FILE *in_file;
    FILE *f2;

    char *offset;
    char *type;

    uint32_t t;
    uint32_t pos;


    in_file = fopen(argv[1], "rb");
    if (in_file==NULL)
    {
        fputs ("File error",stderr);
        exit (1);
    }

    offset = argv[2];
    if (offset==NULL)
    {
        fputs ("Offset error",stderr);
        exit (1);
    }

    type = argv[3];
    if (type==NULL)
    {
        fputs ("Type error",stderr);
        exit (1);
    }

    sscanf (offset, "%x", &pos);
    sscanf (type, "%x", &t);


    fseek (in_file, pos, SEEK_SET);
    oldM1 = fgetc(in_file);
    pos++;
    oldM2 = fgetc(in_file);
    pos++;

    oldM2=oldM2 << 8;
    comp_size = (oldM1|oldM2)&0x7fff;

    //cout << hex << "Razmer upakovannyh dannyh: " << comp_size << endl;

    cout << hex << "Game type: " << (int)t;
    cout << hex << "\nSize of data to be decompressed: " << (int)comp_size;

    if (comp_size > DATA_SIZE) return 0;

    for (int i=0; i<DATA_SIZE; i++)
    {
        in_buf[i]=out_buf[i]=win_buf[i]=0x00;
    }

    fseek  (in_file,pos,SEEK_SET);
    fread(in_buf,sizeof(unsigned char),comp_size-2,in_file);
    fclose(in_file);

    in_pos=0;
    out_pos=0;
    buf_pos=0;
    ctrl=0;
    cnt=0;
    int a=0;


    cout << hex << "\nStarting decompression...";

    while (in_pos<comp_size-2&&out_pos<DATA_SIZE)
    {


        cout << hex << "\nInput position = [" << setw(4) << (int) in_pos << "]";
        if (!(in_buf[in_pos]&0x80))
        {
        cout << hex << "\tBytecode = [" << setw(2) << (int)in_buf[in_pos] << (int)in_buf[in_pos+1] << "]";
        }
        else
        {
        cout << hex << "\tBytecode = [" << setw(4) << (int)in_buf[in_pos] << "]";
        }
        ctrl=(in_buf[in_pos++]>>5);



        switch (ctrl)
        {

        case (0x04):
            cout << " RAW (0x81 - 0x9F)";
            cnt = in_buf[in_pos-1] & 0x1f;
            //cout << hex << "\tcnt: " << (int) cnt;


            for (int i=0; i<cnt; i++)
            {
                out_buf[out_pos++]=in_buf[in_pos];
                win_buf[buf_pos++]=in_buf[in_pos];
                in_pos++;
                if(buf_pos>WINDOW_SIZE)
                {
                    buf_pos=buf_pos-WINDOW_SIZE;
                }
            }

            //cout << hex << "\tout_pos: " << (int) out_pos << endl;



            break;

        case (0x05):
            cout << " RLE_A0 (0xA0 - 0xBF)";
            cnt = (in_buf[in_pos-1] & 0x1f) + 2;
            //cout << hex << "\tcnt: " << (int) cnt;

            for (int i=0; i<cnt; i++)
            {
                chr = in_buf[in_pos++];
                win_buf[buf_pos++]=0x00;
                buf_pos=check_pos(buf_pos);
                out_buf[out_pos++]=0x00;

                win_buf[buf_pos++]=chr;
                buf_pos=check_pos(buf_pos);
                out_buf[out_pos++]=chr;
            }
            //cout << hex << "\tout_pos: " << (int) out_pos << endl;
            break;

        case (0x06):
            cout << " RLE_C0 (0xC0 - 0xDF)";
            cnt = (in_buf[in_pos-1] & 0x1f) + 2;
            chr = in_buf[in_pos++];
            for (int i=0; i<cnt; i++)
            {
                out_buf[out_pos++]=chr;
                win_buf[buf_pos++]=chr;
                if(buf_pos>WINDOW_SIZE)
                {
                    buf_pos=(buf_pos-WINDOW_SIZE)+a;
                }
            }
            //cout << hex << "\tout_pos: " << (int) out_pos << endl;
            break;

        case (0x07):
            cout << " RLE_E0 (0xE0 - 0xFF)";
            //cout << "\tt=" << (int)t;

            if(t==0)
            {
                chr = in_buf[in_pos-1];
                cnt = (chr & 0x1f)+2;
                for (int i=0; i<cnt; i++)
                {
                    win_buf[buf_pos++]=0x00;
                    if(buf_pos>WINDOW_SIZE)
                    {
                        buf_pos=(buf_pos-WINDOW_SIZE)+a;
                    }
                    out_buf[out_pos++]=0x00;

                }
            }

            else
            {
                if (in_buf[in_pos-1] != 0xff)
                {
                    cnt = (in_buf[in_pos-1] & 0x1f)+2;
                  //  cout << hex << "\tcnt: " << (int) cnt;
                    for (int i=0; i<cnt; i++)
                    {
                        win_buf[buf_pos++]=0x00;
                        if(buf_pos>WINDOW_SIZE)
                        {
                            buf_pos=(buf_pos-WINDOW_SIZE)+a;
                        }
                        out_buf[out_pos++]=0x00;

                        //buf_pos++;
                    }
                }
                else
                {
                    chr = in_buf[in_pos++];
                    cnt = (chr & 0xff)+2;
                    for (int i=0; i<cnt; i++)
                    {
                        win_buf[buf_pos++]=0x00;
                        if(buf_pos>WINDOW_SIZE)
                        {
                            buf_pos=(buf_pos-WINDOW_SIZE)+a;
                        }
                        out_buf[out_pos++]=0x00;

                    }

                }
            }
           // cout << hex << "\tout_pos: " << (int) out_pos << endl;
            break;

        default:
            cout << " LZ (0x00 - 0x7F)";
            unsigned short lz1,lz2,lz_len,lz_off;
            int16_t win;
        //    cout << hex << "...LZ-Copy...";
            lz1=in_buf[in_pos-1];
            lz2=in_buf[in_pos++];
            win=out_pos-0x400;
            if (win<=0) win=0;
         //   cout << hex << "windows_start=" << (int)win << endl;
        //    cout << hex << "lz1=" << (int)lz1 << "\tlz2=" << (int)lz2 << endl;
            lz_len = (lz1 >> 2) + 2;
       //     cout << hex << "lz_len=" << (int)lz_len;
            lz1=lz1<<8;
            lz_off=(lz1|lz2)&0x3ff;
       //     cout << hex << "lz_ofin_file=" << (int)lz_off << endl;
            lz_off=((lz_off - 0x3df)&0x3ff);

       //     cout << hex << "lz_off2=" << (int)lz_off << endl;

            int cur=out_pos-0x400;

       //     cout << hex << "cur=" << (int)cur << endl;

            for (int i=0; i<lz_len; i++)
            {

       //         cout << hex << "buf_pos=" << (int)buf_pos;
                lz_off=check_pos(lz_off);
                win_buf[buf_pos++]=win_buf[lz_off];
                buf_pos=check_pos(buf_pos);
                out_buf[out_pos++]=win_buf[lz_off];
                lz_off++;
                lz_off=check_pos(lz_off);
            }

        //    cout << hex << "\tout_pos: " << (int) out_pos << endl;

            break;

        }

    }


    cout << hex << "\nDecompressed data size: " << (int)out_pos;

    f2 = fopen("decomp.bin", "wb");
    fwrite(out_buf,1,out_pos,f2);
    fclose(f2);

    return 0;

}

int32_t check_pos(int32_t cur_pos)
{
    if(cur_pos>=WINDOW_SIZE)
    {
        cur_pos=cur_pos-WINDOW_SIZE;
    }
    return cur_pos;
};

int32_t check_buf(int32_t index)
{
    if(index>WINDOW_SIZE)
    {
        index=0;
    }
    return index;
};


