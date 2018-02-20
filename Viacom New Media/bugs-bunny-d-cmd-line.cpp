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

//void reorder(char* buf, int size);
void reorder(char* buf, int size, uint32_t m1, uint32_t m2);

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

int offset = 0x0f0200;

uint32_t mark1 = 0;
uint32_t mark2 = 0;

ifstream in_file ("Bugs Bunny - Rabbit Rampage (U) [!].smc", ios::in | ios::binary);

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

const unsigned short _decompressionSize=0x5040;
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

while (_outputPosition<_decompressionSize)
//while (_outputPosition<DATA_SIZE)
{

cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]" << endl;

uint8_t code;
uint8_t c;




code=_inputData[_inputPosition++];

cout << hex << " CODE = " << int (code);

//if (_outputPosition==_decompressionSize) break; 
//if (code==0x00) break; 

      
            if (code<=0xc0)
            {
				
								if (code==0xc0)
	
	{
		if (mark1==0)
		{
		mark1=_outputPosition;
	}
		else
		{
		mark2=_outputPosition;
		break;
	}
		
		
		
		
	}		

				
				
			c=code&0xe0;
			
			switch(c)
			{
				
			case (0x00):
			{
			cout << hex << " RAW" << endl;
			uint8_t cnt=code&0x1f;
			cnt++;
			cout << hex << " count=" << int (cnt) << endl;
 for (uint8_t i=0; i<cnt; i++)
                {                  
                _outputData[_outputPosition++] = _inputData[_inputPosition++];
                 }
break;
}
			
			case (0x20):

{			
			cout << hex << " RLE ZEROES" << endl;
			uint8_t cnt=code&0x1f;
			cnt++;

 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = 0x00;
                           
                     
				 }
				 
				 break;
			 }
			
			case 0x40:
			
			{
			
			cout << hex << " RLE 1-BYTE" << endl;
			
			uint8_t cnt=code&0x1f;
			cnt++;

 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = _inputData[_inputPosition];
                           
                     
				 }	
				_inputPosition++;
				
			
			break;
		}
			
			case 0x60:
			{
				
						cout << hex << " RLE 2-BYTE" << endl;
			uint8_t cnt=code&0x1f;
			cnt++;

 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = _inputData[_inputPosition];
                _outputData[_outputPosition++] = _inputData[_inputPosition+1];
                           
                     
				 }	
				_inputPosition++;
				_inputPosition++;
				break;	
				
				
			}
			
			
			case 0x80:
			{
				
	cout << hex << " RLE INCREMENT" << endl;
			uint8_t cnt=code&0x1f;
			cnt++;
			uint8_t chr = _inputData[_inputPosition];

 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = int(chr+i);
                    
				 }	
				_inputPosition++;
				break;	
				
				
			}
			
			case 0xA0:
			{
				
						cout << hex << " RLE DECREMENT" << endl;
			uint8_t cnt=code&0x1f;
			cnt++;
			uint8_t chr = _inputData[_inputPosition];

 for (uint8_t i=0; i<cnt; i++)
                
                {                  
                                                       
                _outputData[_outputPosition++] = int(chr-i);
                    
				 }	
				_inputPosition++;
				break;	
				
				
			}
			
			
			
				
			}
			
		}
		
		else
		
		{
			
			if (code>0xc0)
			
			{
				
			c=code&0x3f;			
			
			cout << hex << " RAW ONE BYTE" << endl;
                
            _outputData[_outputPosition++] = c;                
                 

}


				
				
            
                              
            }
			
			
		}		


cout << hex << "\n REORDER" << endl;
cout << hex << "mark1=[" << int(mark1) << endl;
cout << hex << "mark2=[" << int(mark2) << endl;
    
//reorder(_outputData,_outputPosition);
reorder(_outputData, mark2, mark1, mark2);

cout << hex << "\n SAVING" << endl;

ofstream out_file ("Bugs Bunny - Rabbit Rampage (U) [!]_decomp.bin", ios::out | ios::binary);
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

void reorder(char* buf, int size, uint32_t m1, uint32_t m2)

{

uint8_t backup[0x6000] = {0};
for (int i=0;i<size;i++)
{
backup[i]=buf[i];	
}

uint32_t o1=0;
uint32_t o2=1;

for (int i=0;i<m1;i++)
{
	
buf[i*2]=backup[i];

}
	
for (int i=0;i<(m2-m1);i++)
{
	
buf[i*2+1]=backup[m1+i];

}	
	


}

