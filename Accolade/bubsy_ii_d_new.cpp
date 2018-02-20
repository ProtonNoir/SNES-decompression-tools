#include <iostream>
#include <fstream>
//#include <cstdlib> //malloc
#include <iomanip> //setw

using namespace std;

const uint32_t DATA_SIZE = 65536;
const uint32_t WINDOW_SIZE = 4096;


void decompress (uint32_t pos, uint8_t* in, uint8_t* out);


int main(int argc, char **argv)
{
//const unsigned int BUFFER_SIZE=0x4c40;
const int BUFFER_SIZE=0x4c40;

char _inputData[BUFFER_SIZE] = {0};
char _outputData[BUFFER_SIZE] = {0};

int _inputPosition = 0;
int _outputPosition = 0;


int offset = 0x1c808a;

ifstream in_file ("Bubsy II (U) [!].smc", ios::in | ios::binary);

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


//cout << hex << int(_decompressionSize) << endl;
//cout << _decompressionSize << endl;

//uint32_t offsetData = offset+4;
uint32_t offsetData = offset;

in_file.seekg (offsetData, ios::beg);

in_file.read (_inputData, BUFFER_SIZE);
in_file.close();


while (_outputPosition<BUFFER_SIZE)
{	

cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]";

uint8_t code;
uint8_t cnt;
uint8_t chr;

code=_inputData[_inputPosition++];

cout << hex << " code = [" << int (code) << "]";


if (code==0x80) break;

            if (code<0x80)
            {
				
				if (code<0x40)
            {
cout << hex << "_REPEAT_";

cnt=(code&0x7f)+1;
chr=_inputData[_inputPosition++];

cout << hex << " cnt = [" << int (cnt) << "]";
cout << hex << " chr = [" << int (chr) << "]";

for (uint8_t i=0; i<cnt; i++)
                
                {                  
                    _outputData[_outputPosition++]=chr;
                   if (_outputPosition==BUFFER_SIZE) break; 
				 }

}

else 
{
	
	cout << hex << "_RAW_";

cnt=(code&0x7f)-0x40+1;

cout << hex << " cnt = [" << int (cnt) << "]";

for (uint8_t i=0; i<cnt; i++)
                
                {                  
                    _outputData[_outputPosition++]=_inputData[_inputPosition++];
                                       if (_outputPosition==BUFFER_SIZE) break; 
				 }
				 
}
				
				


}           



 if (code>0x80)
            {
				
				if (code<0xc0)
				
				{
cout << hex << "_LZ_SHORT_" << endl;

cnt=(code&0x7f)+1;

cout << hex << " cnt = [" << int (cnt) << "]";	

uint8_t dist=_inputData[_inputPosition++];
cout << hex << " dist = [" << int (dist) << "]";
uint16_t lz_off=_outputPosition-dist;

for (uint8_t i=0; i<cnt; i++)
                
                {                  
                    _outputData[_outputPosition++]=_outputData[lz_off++];
                    if (_outputPosition==BUFFER_SIZE) break;                
				 }
				 
			 }
			 
			 				if (code>0xc0)
				
				{
cout << hex << "_LZ_LONG_" << endl;

cnt=(code&0x7f)-0x40+1;

cout << hex << " cnt = [" << int (cnt) << "]";	

uint8_t d1=_inputData[_inputPosition++];
uint8_t d2=_inputData[_inputPosition++];

cout << hex << " d1 = [" << int (d1) << "]";
cout << hex << " d2 = [" << int (d2) << "]";

uint16_t dist;

//d=(d<<8)&0xff00;
//d2=d1;
//d2=d2<<8;
//d2=d2&0xff00;
dist=d2;
dist=(d2<<8)&0xff00;
dist=dist+d1;

cout << hex << " dist = [" << int (dist) << "]";
uint16_t lz_off=_outputPosition-dist;

for (uint8_t i=0; i<cnt; i++)
                
                {                  
                    _outputData[_outputPosition++]=_outputData[lz_off++];
                    if (_outputPosition==BUFFER_SIZE) break;                
				 }
				 
			 }
			 
			 
			 
			 
				 
}



}


ofstream out_file ("Bubsy II (U) [!]_decomp_new.bin", ios::out | ios::binary);
if (!out_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
else
{
out_file.write(_outputData,BUFFER_SIZE);
out_file.close();
}

}

return 0;
}

void decompress (uint32_t pos, uint8_t* in, uint8_t* out)
{
    

}

