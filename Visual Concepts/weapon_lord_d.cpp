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

uint32_t check_pos(uint32_t cur_pos);


int main(int argc, char **argv)
{
const unsigned int BUFFER_SIZE=0x10000;

char _inputData[BUFFER_SIZE] = {0};
char _outputData[BUFFER_SIZE] = {0};
char _windowData[WINDOW_SIZE] = {0x20};

for (int i=0;i<WINDOW_SIZE;i++) _windowData[i]=0x20;

cout << hex << int(_windowData[0]) << endl;
cout << hex << int(_windowData[1]) << endl;

int _inputPosition = 0;
int _outputPosition = 0;
int _windowPosition = 0xfee;

int offset = 0x2F5030;

ifstream in_file ("Weapon Lord (U) [!].smc", ios::in | ios::binary);

if (!in_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
    else
    {

in_file.seekg (offset, ios::beg);

//char header[2];
//in_file.read (header, 2);



//short header;
//in_file.read (header, sizeof(char));

//cout << hex << int(header[1]) << endl;
//cout << hex << int(header[0]) << endl;

//const unsigned short _decompressionSize=(header[1]<<8)|header[0];

unsigned short _decompressionSize=0x0b90;
//unsigned short _decompressionSize=header[1]&0xff;
//_decompressionSize=_decompressionSize<<8;
//_decompressionSize=_decompressionSize|(header[0]&0xff);

//char h1[1];
//in_file.read (h1, sizeof(h1));

//cout << hex << "h1[0]=" << int(h1[0]) << endl;

//char head1 = h1[0];


//in_file.seekg (offset+1, ios::beg);
//char h2[1];
//in_file.read (h2, sizeof(h2));

//cout << hex << "h2[0]=" << int(h2[0]) << endl;

//unsigned short _decompressionSize=h2[0]&0xff;
//_decompressionSize=_decompressionSize<<8;
//_decompressionSize=_decompressionSize|head1;





cout << hex << int(_decompressionSize) << endl;
cout << _decompressionSize << endl;

uint32_t offsetData = offset+0;

in_file.seekg (offsetData, ios::beg);

in_file.read (_inputData, BUFFER_SIZE);
in_file.close();




while (_outputPosition<_decompressionSize)
{	

cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]" << endl;

uint8_t code;
uint8_t count;

code=_inputData[_inputPosition++];

cout << hex << " code = " << int (code);

if (_outputPosition==_decompressionSize) break; 
if (code==0x80) break; 

       //  for (int i=0; i<0x08; i++)
      //  {

            //if ((code&TEST_BIT)==TEST_BIT)
            if (code<0x80)
            {
				
				count = (code & 0x7f)+1;
//cout << hex << " LITERAL = ["<< int(_inputData[_inputPosition]) << "]" << endl;

 for (int i=0; i<count; i++)
                
                {                  
                     //lz_off=check_pos(lz_off);
                     
                    //_windowData[_windowPosition++]=_windowData[lz_off];
                    //_outputData[_outputPosition++]=_windowData[lz_off];
                    _outputData[_outputPosition++]=_inputData[_inputPosition++];
                     
                    //_windowPosition=check_pos(_windowPosition);
                    
                    //lz_off++;
                    //lz_off=check_pos(lz_off);

                     
                     
                     
                     
                     
                     
				 }
                     
                     if (_outputPosition==_decompressionSize) break;    


//uint8_t lit;

//lit = _inputData[_inputPosition++];
                
//_outputData[_outputPosition++] = lit;
//_windowData[_windowPosition++] = lit;


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
                
                cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]";
                
                cout << hex << " LZ = ["<< setw(2) << (int)lz1 << (int)lz2 << "]";      
                
                //lz_len=((lz2>>4)&0x0f)+MIN_MATCH_LENGTH;
                //lz_len=((lz2)&0x0f)+MIN_MATCH_LENGTH;
                lz_len=((lz1&0x7f)>>3)+1;
                
                cout << hex << " LENGTH = "<< lz_len;
                
                //lz_dist=(((lz2&0x0f)<<8)|lz1)&0x0fff;
                //lz_off=(((lz2&0x0f)<<8)|lz1)&0x0fff;
                //lz_off=(((lz2&0xf0)<<4)|lz1)&0x0fff;
                
                lz_dist=(((lz1&0x07)<<8)|lz2)&0x0fff;
                
                lz_off=_outputPosition-lz_dist;
                
                cout << hex << " OFFSET = "<< lz_off << endl; 

                for (int j=0; j<lz_len; j++)
                
                {                  
                //     lz_off=check_pos(lz_off);
                     
                  //  _windowData[_windowPosition++]=_windowData[lz_off];
                    //_outputData[_outputPosition++]=_windowData[lz_off];
                    _outputData[_outputPosition++]=_outputData[lz_off++];
                     
                    //_windowPosition=check_pos(_windowPosition);
                    
                    //lz_off++;
                    //lz_off=check_pos(lz_off);

                     
                     
                     
                     
                     
                     
				 }
                     
                     if (_outputPosition==_decompressionSize) break;                
                              
            }
            //code=code>>1;
       // }

    }






ofstream out_file ("Weapon Lord (U) [!]_decomp.bin", ios::out | ios::binary);
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

uint32_t check_pos(uint32_t cur_pos)
{
    if(cur_pos==WINDOW_SIZE)
    {
        //cur_pos=cur_pos-WINDOW_SIZE;
        cur_pos=0;
    }
    return cur_pos;
}

