#include <iostream>
#include <fstream>
//#include <cstdlib> //malloc
#include <iomanip> //setw

using namespace std;

const uint32_t DATA_SIZE = 65536;
const uint32_t WINDOW_SIZE = 4096;
const uint16_t MAX_MATCH_LENGTH = 18;
const uint16_t MIN_MATCH_LENGTH = 3;

const uint8_t TEST_BIT = 1;

int32_t check_pos(int32_t);

int main(int argc, char **argv)
{
const unsigned int BUFFER_SIZE=0x1000;
const int DEBUG_SIZE=0x100;

char _inputData[BUFFER_SIZE] = {0};
char _outputData[BUFFER_SIZE] = {0};
char _windowData[WINDOW_SIZE] = {0};

//for (int i=0;i<WINDOW_SIZE;i++) _windowData[i]=0x00;
//cout << hex << int(_windowData[0]) << endl;
//cout << hex << int(_windowData[1]) << endl;

int _inputPosition = 0;
int _outputPosition = 0;
//int _windowPosition = 0xfee;
int _windowPosition = 0;

int offset = 0x011747;

ifstream in_file ("Pitfall - The Mayan Adventure (U) [!].smc", ios::in | ios::binary);

if (!in_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
    else
    {

in_file.seekg (offset, ios::beg);

char header[2];
in_file.read (header, 2);

cout << hex << int(header[1]) << endl;
cout << hex << int(header[0]) << endl;

//unsigned short _compressionSize=header[1]&0xff;
//_compressionSize=_compressionSize<<8;
//_compressionSize=_compressionSize|(header[0]&0xff);

//const unsigned short _decompressionSize=0x600;
unsigned short _decompressionSize=header[1]&0xff;
_decompressionSize=_decompressionSize<<8;
_decompressionSize=_decompressionSize|(header[0]&0xff);

//cout << hex << int(_compressionSize) << endl;
//cout << _compressionSize << endl;

cout << hex << int(_decompressionSize) << endl;
cout << _decompressionSize << endl;

uint32_t offsetData = offset+2;
//uint32_t offsetData = offset;

in_file.seekg (offsetData, ios::beg);
in_file.read (_inputData, BUFFER_SIZE);
in_file.close();




    
uint8_t cnt=0;

//while (_inputPosition<_compressionSize)
//while (_inputPosition<DEBUG_SIZE)
while (_outputPosition<_decompressionSize)
//while (_outputPosition<DATA_SIZE)
//while (_outputPosition<DEBUG_SIZE)
{
uint8_t code=0;
uint8_t bits=8;
uint8_t w=0;

cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]";

cout << hex << "\n--==NEW CODEBYTE==-";

code=_inputData[_inputPosition++];

cout << hex << "\nCODEBYTE = [" << int (code)<< "] = [";
 for(int i=0;i<8;i++)
			cout << hex << int((code>>i)&TEST_BIT) << "]";                
			

//if ((code&0x80)!=0x80) cout << hex << " !!!WARNING!!!";
//cout << hex << "\n\tZERO COUNT= ["<< int(cnt) << "]";                
if (_outputPosition==_decompressionSize) break; 

         //for (int i=0; i<0x08;)         
         //while (c>0)
         for(uint8_t i=0;i<8;i++)
        {
			
			cout << hex << "\n\tBIT NUMBER = ["<< int(i) << "] = [";
			
			for(uint8_t y=0;y<8;y++)
			{			
			if(y==i)
			cout << hex << "X";			
			else cout << hex << (int)y+1;
			}
			cout << hex << "]";
		cout << hex << "\n\tBIT = ["<< int(code&TEST_BIT) << "]";
	        
					
				
				
				if ((code&TEST_BIT)==TEST_BIT)
            {
			
				cout << hex << "\n\t\tLITERAL = ["<< int(_inputData[_inputPosition]) << "]";
				cout << hex << "\n\t\t\t_windowPosition = ["<< int(_windowPosition) << "]";
            uint8_t lit;
            lit = _inputData[_inputPosition++];            
            _windowData[_windowPosition++] = lit;
            _outputData[_outputPosition++] = lit;
            _windowPosition=check_pos(_windowPosition);
            if (_outputPosition==_decompressionSize) break;
            //i++;
            //bits--;
            //i++;
            code=code>>1;
            //cnt=0;
            //cout << hex << "\n\tCODEBYTE AFTER SHIFT RIGHT ON = ["<< int(8-bits) << "] BITS = " << "[" << int(code) << "]";
            //if (bits==0) break;
				
				
				
				
				
				
			
            
            
            
            }
            
            else{
            
            
            			//cout << hex << "\n\t\t STEP 1 " <<  " COUNT ZEROES ";
				//do{
                //cnt++;
                //bits--;
                //i++;                
                //code=code>>1;
				//if (bits==0) break;
                //}
                //while ((code&TEST_BIT)!=TEST_BIT);
				            
				            
//cout << hex << "\n\t\t\t test cnt = ["<< int(cnt) << "]";                
//cout << hex << "\n\t\t\t test bits = ["<< int(bits) << "]";
//cout << hex << "\n\t\t\t test i = ["<< int(i) << "]";
				            

                
                //cout << hex << " STEP 2 " << endl;
				//cout << hex << "\n\t\t\t LZ POS = ["<< int(_inputData[_inputPosition]) << "]";
                //cout << hex << "\n\t\t\t test code = ["<< int(code) << "]";

uint8_t lz1, lz2, lz3;
uint16_t lz_len;
uint16_t lz_off;
uint16_t lz_dist; 
                

//if (bits==0) cout << hex << "\n\t\t\t BITS!!! = ["<< int(bits) << "]";
                //if (bits==0) break;
                //cout << hex << "LA-LA-LA";
                
                //cout << hex << "\n\t\t\t BIT NUMBER = ["<< int(i) << "]" <<  " SHIFT ";                
                //i++;
                //bits--;
                //code=code>>1;

				//cout << hex << "\n\t\t\t STEP 3 ";               
                                
                //if (cnt==2){
				//cout << hex << "\n\t\t\t cnt(2) = ["<< int(cnt) << "]";
                ////lz_len=cnt+2;
                //lz_len=6;
                //lz_dist=_inputData[_inputPosition++];
                //}
                
                //if (cnt==3){
				//cout << hex << "\n\t\t\t cnt(3) = ["<< int(cnt) << "]";
                ////lz_len=cnt+3;
                //lz_len=4;
                ////lz_len=_inputData[_inputPosition++]+2;
                //lz_dist=_inputData[_inputPosition++];
                
                
                //}                

                //if (cnt==4){
				//cout << hex << "\n\t\t\t cnt(4) = ["<< int(cnt) << "]";
                ////lz_len=cnt-1;
                //lz_len=2;
                //lz_dist=_inputData[_inputPosition++];
                //}           
                                                         
                //if (cnt==1)
                //{
                    
                //cout << hex << "\n\t\t\t cnt(1) = ["<< int(cnt) << "]";
                
                //lz_dist=_inputData[_inputPosition++];
                
                ////cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
                ////cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]" << endl;
                ////lz1=code;
                
                //lz2=_inputData[_inputPosition++];
                
                //if (lz2==0x00){\
				//cout << hex << "\n\t\t\t LZ2 = ["<< int(lz2) << "]";
                //lz3=_inputData[_inputPosition++];
                //lz_len=lz3+1;
                //}
                
                //else{
                //lz_len=lz2+3;   
                //}
                
                //}
                
                //cnt=0;
                
                
                
                lz1=_inputData[_inputPosition++];
                lz2=_inputData[_inputPosition++];
                cout << hex << "\n\t\t\t LZ_1 = ["<< (int)lz1 << "] LZ_2 = [" << (int)lz2 << "]";
                
                //lz_dist=_inputData[_inputPosition++];
                
                //lz_off = _outputPosition-lz_dist;
                //cout << hex << "\n\t\t\t LZ_OFF = "<< lz_off;
                
                //cout << hex << "\n\t\t\t STEP 4 "; 
                //cout << hex << "\nInput position = [" << setw(4) << (int) _inputPosition << "]";
//cout << hex << "\tOutput position = [" << setw(4) << (int) _outputPosition << "]";
                
                //cout << hex << " LZ = ["<< setw(2) << (int)lz1 << (int)lz2 << (int)lz2 << "]";      
                
                lz_len=((lz2>>4)&0x0f)+MIN_MATCH_LENGTH;
                //lz_len=((lz2)&0x0f)+MIN_MATCH_LENGTH;
                //LZ
                
                //lz_len=((lz2)&0xff)+1;
                
                //if lz2==0x00
                //{
                //lz3=_inputData[_inputPosition++];
                //}
                
                
                cout << hex << "\n\t\t\tLENGTH = "<< lz_len;
                
                //lz_dist=(((lz2&0xff)<<8)|lz1)&0xffff;
                               
                lz_off=(((lz2&0x0f)<<8)|lz1)&0x0fff;
                //lz_off=(((lz2&0xf0)<<4)|lz1)&0x0fff;
                //lz_off=_outputPosition-lz_dist;
                //LZ
                //lz_dist=(lz2&0xff)+1;
                
                //lz_off=_outputPosition-lz_dist;
                
                cout << hex << "\n\t\t\tOFFSET = "<< lz_off; 

                for (int j=0; j<lz_len; j++){                  
                                          
                     uint8_t chr = 55;
                    //chr=_outputData[lz_off++];
                    chr=_windowData[lz_off++];
                    cout << hex << int(chr);
                    lz_off=check_pos(lz_off);
                    _windowData[_windowPosition++]=chr;
                    _windowPosition=check_pos(_windowPosition);
                    _outputData[_outputPosition++]=chr;
                     
                    //_windowData[_windowPosition++]=_windowData[lz_off];                                      
                    //_outputData[_outputPosition++]=_outputData[lz_off++];
                    //_outputPosition++;
                    //lz_off++;
                    //_outputData[_outputPosition++]=_windowData[lz_off];
                    //_windowPosition=check_pos(_windowPosition);
                    //lz_off++;
                    //lz_off=check_pos(lz_off);
                  
                     
				 }
                     
                    //if (_inputPosition==_compressionSize) break;
                    if (_outputPosition==_decompressionSize) break;
                    
                    cout << hex << "\n\t\t\t STOP LZ ";
                    code=code>>1; 
                    
           			
				
				
				
			}
		
			
			
    
            
   
}

                    
                              
           
            
   
	
}
    
ofstream out_file ("Pitfall - The Mayan Adventure (U) [!]_decomp_v1.bin", ios::out | ios::binary);
if (!out_file.is_open()) // если файл не открыт
        cout << "Файл не может быть открыт!\n"; // сообщить об этом
else
{
out_file.write(_outputData,_outputPosition);
out_file.close();
}

}

return 0;}

int32_t check_pos(int32_t cur_pos)
{
    if(cur_pos>WINDOW_SIZE)
    {
        cur_pos=cur_pos-WINDOW_SIZE;
        //cur_pos=0;
    }
    return cur_pos;
};


