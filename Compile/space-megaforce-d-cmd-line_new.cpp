#include <iostream>
#include <fstream>
//#include <cstdlib> //malloc
#include <iomanip> //setw

using namespace std;

const uint32_t DATA_SIZE = 65536;
const uint32_t WINDOW_SIZE = 4096;
const uint16_t MAX_MATCH_LENGTH = 18;
const uint16_t MIN_MATCH_LENGTH = 3;

const uint8_t TEST_BIT = 0x01;

void reorder(char* buf, int size);

int main(int argc, char **argv)
{
const unsigned int BUFFER_SIZE=0x10000;

char _inputData[BUFFER_SIZE] = {0};
char _outputData[BUFFER_SIZE] = {0};
//char _windowData[WINDOW_SIZE] = {0};

//for (int i=0;i<WINDOW_SIZE;i++) _windowData[i]=0x00;

//cout << hex << int(_windowData[0]) << endl;
//cout << hex << int(_windowData[1]) << endl;

int _inputPosition = 0;
int _outputPosition = 0;
//int _windowPosition = 0xfee;

int offset = 0x08d8e8;

ifstream in_file ("Space Megaforce (U) [!].smc", ios::in | ios::binary);

if (!in_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
    else
    {

in_file.seekg (offset, ios::beg);

//char header[2];
//in_file.read (header, 2);

//cout << hex << int(header[1]) << endl;
//cout << hex << int(header[0]) << endl;

//unsigned short _compressionSize=header[1]&0xff;
//_compressionSize=_compressionSize<<8;
//_compressionSize=_compressionSize|(header[0]&0xff);

//const unsigned short _decompressionSize=0x600;
//unsigned short _decompressionSize=header[1]&0xff;
//_decompressionSize=_decompressionSize<<8;
//_decompressionSize=_decompressionSize|(header[0]&0xff);

//cout << hex << int(_compressionSize) << endl;
//cout << _compressionSize << endl;

//cout << hex << int(_decompressionSize) << endl;
//cout << _decompressionSize << endl;

//uint32_t offsetData = offset+2;
uint32_t offsetData = offset;

in_file.seekg (offsetData, ios::beg);

in_file.read (_inputData, BUFFER_SIZE);
in_file.close();

//while (_outputPosition<_decompressionSize)
while (_outputPosition<DATA_SIZE)
{

cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]" << endl;

uint8_t code;

code=_inputData[_inputPosition++];

cout << hex << " CODE = " << int (code);

//if (_outputPosition==_decompressionSize) break; 
if (code==0x00) break; 

      
            if (code<0x80)
            {
cout << hex << " RAW = ["<< int(_inputData[_inputPosition]) << "]" << endl;


uint8_t cnt=code&0x7f;


 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = _inputData[_inputPosition++];
                //_outputPosition++;
               
                     
				 }

//if (_outputPosition==_decompressionSize) break;


            }

            else

            {

uint8_t lz1, lz2;
uint16_t lz_len;
uint16_t lz_off;
uint16_t lz_dist;
                
                //lz1=_inputData[_inputPosition++];
                lz1=code;
                lz2=_inputData[_inputPosition++];
                
                //cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
//cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]";
                
                cout << hex << " LZ = ["<< setw(2) << (int)lz1 << (int)lz2 << "]";      
                
                //lz_len=((lz2>>4)&0x0f)+MIN_MATCH_LENGTH;
                //lz_len=((lz2)&0x0f)+MIN_MATCH_LENGTH;
                //LZ
                lz_len=((lz1)&0x7f)+MIN_MATCH_LENGTH;
                
                cout << hex << " LENGTH = "<< lz_len;
                
                //lz_dist=(((lz2&0x0f)<<8)|lz1)&0x0fff;
                               
                //lz_off=(((lz2&0x0f)<<8)|lz1)&0x0fff;
                //lz_off=(((lz2&0xf0)<<4)|lz1)&0x0fff;
                //lz_off=_outputPosition-lz_dist;
                //LZ
                lz_dist=(lz2&0xff)+1;
                lz_off=_outputPosition-lz_dist;
                
                cout << hex << " OFFSET = "<< lz_off << endl; 

                for (int j=0; j<lz_len; j++)
                
                {                  
                     //lz_off=check_pos(lz_off);
                                      
                    //_windowData[_windowPosition++]=_windowData[lz_off];
                    _outputData[_outputPosition++]=_outputData[lz_off++];
                    //_outputPosition++;
                    //lz_off++;
                    
                    //_outputData[_outputPosition++]=_windowData[lz_off];
                     
                    //_windowPosition=check_pos(_windowPosition);
                    
                    //lz_off++;
                    //lz_off=check_pos(lz_off);
                  
                     
				 }
                     
                    //if (_inputPosition==_compressionSize) break;
                    //if (_outputPosition==_decompressionSize) break;                
                              
            }
            //code=code>>1;
        }

    
reorder(_outputData,_outputPosition);

ofstream out_file ("Space Megaforce (U) [!]_decomp_new.bin", ios::out | ios::binary);
if (!out_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
else
{
out_file.write(_outputData,_outputPosition);
out_file.close();
}

}

return 0;
}

void reorder(char* buf, int size)

{
int rows = size / 16;

uint8_t backup[16] = {0};

for (int row=0;row<rows;row++)
{

for (int i=0;i<sizeof(backup);i++)
backup[i]=buf[row*16+i];

int r=row*16;

for (int j=0; j<16;j++)
{
	if(j<8)
	{
		buf[row*16+j*2]=backup[j];
	}
	
	else
	
	{
		buf[row*16+j-(15-j)]=backup[j];
	}
	
	
	
}

//buf[row*16+0]=backup[0];
//buf[row*16+2]=backup[1];
//buf[row*16+4]=backup[2];
//buf[row*16+6]=backup[3];
//buf[row*16+8]=backup[4];
//buf[row*16+10]=backup[5];
//buf[row*16+12]=backup[6];
//buf[row*16+14]=backup[7];

//buf[row*16+1]=backup[8];15-8=7
//buf[row*16+3]=backup[9];
//buf[row*16+5]=backup[10];
//buf[row*16+7]=backup[11];
//buf[row*16+9]=backup[12];
//buf[row*16+11]=backup[13];
//buf[row*16+13]=backup[14];
//buf[row*16+15]=backup[15];	
	
}

}

