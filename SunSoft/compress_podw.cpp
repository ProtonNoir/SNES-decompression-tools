#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include "compress_podw.h"

using namespace std;

struct pos_t
{
    uint16_t in, out, win;
};

uint32_t check_pos(uint32_t cur_pos);

struct lz_t
{
    uint16_t length, offset;
};

lz_t search_lz (int, uint8_t*, int);

//uint32_t write_lz(uint32_t read_pos, uint8_t* write_buf, uint32_t write_pos, lz_t lz);
uint16_t write_lz (uint8_t *write_buf, uint16_t write_pos, lz_t lz);

//uint32_t write_lit(uint32_t &read_pos, uint8_t* read_buf, uint32_t &write_pos, uint8_t* write_buf, uint32_t &win_pos, uint8_t* win_buf);
uint32_t write_lit(uint32_t &read_pos, uint8_t* read_buf, uint32_t &write_pos, uint8_t* write_buf);
uint16_t write_lz (uint32_t &read_pos, uint8_t *write_buf, uint16_t write_pos, lz_t lz);

void write_bit(uint8_t &codebyte, uint8_t &bitcount, uint8_t bit);

uint32_t compress(uint8_t* in_buf, uint8_t* out_buf, size_t in_size)
{
   // uint8_t win_buf[WINDOW_SIZE]={0};
    uint8_t bit;
    uint16_t cnt;
    uint32_t in_pos,out_pos,head_pos;
    //,buf_pos;
    uint8_t ctrl;
    unsigned char h1,h2;
    lz_t lz, lz_alt;

    in_pos=0;
    out_pos=5;
    head_pos=4;
   // buf_pos=0xfee;

    ctrl=0;
    cnt=0;
    bit=0;

    cout << hex << "Starting compression...";
    cout << hex << "\nInput position = [" << setw(4) << (int) in_pos << "]";

    //write_lit(in_pos, in_buf, out_pos, out_buf, win_pos, win_buf);
    write_lit(in_pos, in_buf, out_pos, out_buf);
    write_bit(ctrl, bit, 1);
    cout << hex << "bit=" << (int)bit;

    while (in_pos<in_size)
    {
        cout << hex << "\nInput position = [" << setw(4) << (int) in_pos << "]";

        lz=search_lz(in_pos,in_buf,in_size);
        lz_alt=search_lz(in_pos+1,in_buf,in_size);

        if (lz_alt.length>lz.length && lz_alt.length>=MIN_MATCH_LENGTH)
        {
            write_lit(in_pos, in_buf, out_pos, out_buf);
            write_bit(ctrl, bit, 1);
            cout << hex << "bit=" << (int)bit;
            cout << "\t Optimization";
        }
        else
        {
            if (lz.length<MIN_MATCH_LENGTH)
            {
                write_lit(in_pos, in_buf, out_pos, out_buf);
                write_bit(ctrl, bit, 1);
                cout << hex << "bit=" << (int)bit;
            }
            else
            {
                out_pos+=write_lz(in_pos, out_buf, out_pos, lz);
                write_bit(ctrl, bit, 0);
                //in_pos+=lz.length;
                cout << hex << "bit=" << (int)bit;
            }


        }

        if (bit==8)

        {
            out_buf[head_pos]=ctrl;
            ctrl=0x00;
            bit=0;
            head_pos=out_pos++;
            cnt++;
            cout << hex << "\tBytecode = [" << setw(2) << (int) ctrl << "]" << endl;
            cout << hex << "\tBytecode_count = [" << setw(2) << (int) cnt << "]" << endl;
            cout << hex << "\tout_pos = [" << setw(2) << (int) out_pos << "]" << "\thead_pos = [" << setw(2) << (int) head_pos << "]" <<endl;
        }
    }


    if (bit<8)

    {
        int bit_shift;
        bit_shift=8-bit;
        ctrl=ctrl>>bit_shift;
        out_buf[head_pos]=ctrl;

        cout << hex << "\tBytecode = [" << setw(2) << (int) ctrl << "]" << endl;
        cout << hex << "\tBytecode_count = [" << setw(2) << (int) cnt << "]" << endl;
        cout << hex << "\tBit_count = [" << setw(2) << (int) bit << "]" << endl;

    }

    cout << hex << "Size of compressed data: " << (int)(out_pos) << endl;

    //cnt=(cnt<<3)+bit;
    //cnt=out_pos-2;
    //h1=cnt&0xff;
    //h2=(cnt&0xff00)>>8;

    //cnt=(cnt<<3)+bit;
    cnt=in_size;
    h1=cnt&0xff;
    h2=(cnt&0xff00)>>8;

    out_buf[0]=h1;
    out_buf[1]=h2;

    return out_pos;
}


lz_t search_lz (int pos, uint8_t* buffer, int inputsize)
{
    lz_t variant;
    variant.length=0;
    variant.offset=0;

    int lz_off=0;
    int lz_len=0;
    int match=0;

    int win;
    //win=pos-WINDOW_SIZE;
    win=pos-0xfee;
    if (win<0) win=0;

    //for (int i=win; i<pos+MAX_MATCH_LENGTH && MAX_MATCH_LENGTH + i < MAX_MATCH_LENGTH + pos && i+MAX_MATCH_LENGTH < inputsize && pos < inputsize; i++)
    // size <= LONG_RUN_SIZE && current + size < start + insize
    //for (int i=win; i<pos && i+MAX_MATCH_LENGTH < pos+MAX_MATCH_LENGTH && pos+MAX_MATCH_LENGTH<i+inputsize; i++)
    for (int i=win; i<pos && i+MAX_MATCH_LENGTH < pos+MAX_MATCH_LENGTH; i++)
    {
        if ((buffer[i]==buffer[pos]))
        {
            match=1;
            while((buffer[i+match]==buffer[pos+match])&&(pos+match<inputsize))
            {
                if (match>=MAX_MATCH_LENGTH)
                {
                    break;
                }
                match++;

            }
            if(match>lz_len)
            {
                lz_len=match;
                lz_off=i;
            }
        }
        if(match>=MAX_MATCH_LENGTH)
        {
            lz_len=MAX_MATCH_LENGTH;
            break;
        }
    }


    variant.length=lz_len;
    variant.offset=lz_off;

    return variant;
}

uint32_t write_lit(uint32_t &read_pos, uint8_t* read_buf, uint32_t &write_pos, uint8_t* write_buf)
//uint32_t write_lit(uint32_t &read_pos, uint8_t* read_buf, uint32_t &write_pos, uint8_t* write_buf, uint32_t &win_pos, uint8_t* win_buf)
{
    int countsize;
    uint8_t lit;
    lit=read_buf[read_pos++];

    cout << hex << "\tLITERAL = ["<< setw(2) << (int)read_buf[read_pos] << "]" << endl;
    //win_buf[win_pos++]=lit;
    write_buf[write_pos++]=lit;

    countsize=1;

    return countsize;
}

void write_bit(uint8_t &codebyte, uint8_t &bitcount, uint8_t bit)
{
    codebyte=codebyte>>1;

    if(bit==1)
    {
        codebyte=codebyte|0x80;
    }

    if(bit==0)
    {
        codebyte=codebyte|0x00;
    }

    bitcount++;
}

//uint32_t write_lz(uint32_t read_pos, uint8_t* write_buf, uint32_t write_pos, lz_t lz)
uint16_t write_lz (uint32_t &read_pos, uint8_t *write_buf, uint16_t write_pos, lz_t lz)
{
    uint16_t outsize;
    uint16_t lz1,lz2;
    uint16_t lz_len;
    uint16_t lz_off;

    lz_len = lz.length - 3;
    lz_off = (lz.offset+0xfee)&0xfff;

    lz1=lz_off&0xff;
    lz2=((lz_off&0xf00)>>4)|lz_len;

    write_buf[write_pos++]=lz1;
    write_buf[write_pos++]=lz2;

    read_pos+=lz.length;

    cout << hex << "\tLZ = ["<< setw(2) << (int)lz1 << (int)lz2 << "]";
    cout << hex << "\tLENGTH = [" << setw(2) << (int)lz.length << "]";
    cout << hex << "\tOFFSET = [" << setw(2) << (int)lz.offset << "]" << endl;

    outsize = 2;
    return outsize;
}


uint32_t decompress(uint32_t read_pos, uint8_t* read_buf, uint32_t write_pos, uint8_t* write_buf, uint16_t size)
{
    uint8_t ctrl;
    uint8_t lz1,lz2;

    uint16_t lz_len;
    uint16_t lz_off;

    int32_t buf_pos=0xfee;

    uint8_t win_buf[WINDOW_SIZE];

    for (int i=0; i<WINDOW_SIZE; i++) win_buf[i]=0x20;

    uint16_t in_size=0;

    while(in_size<size)
    {
        in_size++;

        cout << hex << "\nInput position = [" << setw(4) << (int) read_pos << "]";
        ctrl=read_buf[read_pos++];
        cout << hex << "\tBytecode = [" << setw(4) << (int)ctrl << "]";

        for (uint16_t i=0; i<8; i++)

        {

            if (in_size==size) break;

            if ((ctrl&TEST_BIT)==TEST_BIT)
            {
                in_size++;
                cout << hex << "\nLITERAL = ["<< (int)read_buf[read_pos] << "]";
                cout << hex << "\twrite_pos = ["<< (int)write_pos << "]";
                win_buf[buf_pos++]=read_buf[read_pos];

                if(buf_pos>WINDOW_SIZE)
                {
                    buf_pos=buf_pos-WINDOW_SIZE;
                }

                write_buf[write_pos++]=read_buf[read_pos++];
            }

            else

            {
                cout << hex << "\nLZ";
                in_size++;
                lz1=read_buf[read_pos++];
                in_size++;
                lz2=read_buf[read_pos++];

                cout << hex << " LZ1 : " << (int)lz1 <<  " LZ2 : " << (int)lz2;

                lz_len=(lz2&0x0f)+0x03;
                cout << hex << " lz_len = "<< lz_len;

                lz_off=(lz1|(lz2<<4)&0xf00);
                cout << hex << " lz_off = "<< lz_off;
                cout << hex << "\twrite_pos = ["<< (int)write_pos << "]";

                for (int j=0; j<lz_len; j++)
                {
                    lz_off=check_pos(lz_off);
                    win_buf[buf_pos++]=win_buf[lz_off];
                    buf_pos=check_pos(buf_pos);
                    write_buf[write_pos++]=win_buf[lz_off];
                    lz_off++;
                    lz_off=check_pos(lz_off);
                }
            }
            ctrl=ctrl>>1;
        }
    }
    return write_pos;
}


uint32_t check_pos(uint32_t cur_pos)
{
    if(cur_pos>=WINDOW_SIZE)
    {
        cur_pos=cur_pos-WINDOW_SIZE;
    }
    return cur_pos;
}







