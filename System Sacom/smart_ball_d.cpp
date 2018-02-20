#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

struct pos_t
{
    uint16_t in, out;
};

using namespace std;

int main(int argc, char **argv)
{
    const unsigned int BUFFER_SIZE=0x10000;

    uint8_t in_buf[BUFFER_SIZE]={0};
    uint8_t out_buf[BUFFER_SIZE]={0};
    uint8_t c_buf[2]={0};

    uint32_t in_pos = 0;
    uint32_t out_pos = 0;
    
    uint32_t in_off = 0x072CC7; 
    uint32_t c_off = 0x072CC5;
    
     // char *off;
   // uint8_t header[2];
  //  uint16_t comp_size;
    //rle_t rle;

    FILE *in_file;
    FILE *out_file;
      
    const char* in_name = "Smart Ball (U) [!].smc";
    const char* out_name = "Smart Ball (U) [!]_decomp.bin";
  
    //const char* in_name = argv[1];
    //cout << "agv[1]=" << argv[1] << endl;
    //const char* out_name = argv[2];
    //cout << "agv[2]=" << argv[2] << endl;

    in_file = fopen(in_name, "rb");
    out_file = fopen(out_name, "wb");
    
     //in_file = fopen(argv[1], "rb");
    //    in_file = fopen(in_name, "rb");
    //    if (in_file==NULL)
    //    {
    //        fputs ("Input file error",stderr);
    //        exit (1);
   //     }

        //out_file = fopen(argv[2], "wb");
    //    out_file = fopen(out_name, "wb");
  //      if (out_file==NULL)
   //     {
  //          fputs ("Output file error",stderr);
  //          exit (1);
  //      }
        
  //      off = argv[3];
//if (off==NULL){fputs ("Offset error",stderr); exit (1);}

//sscanf (off, "%x", &offset);
    
    //fseek  (in_file,offset,SEEK_SET);
  //  fread(header,sizeof(uint8_t),sizeof(header),in_file);

    //comp_size=(header[1]<<8)|header[0];

    //cout << hex << "Size of data to be decompressed: " << (int)comp_size;
    //cout << hex << "\nOffset: " << (int)offset;
    
    
    fseek(in_file,c_off,SEEK_SET);
    fread(c_buf,sizeof(uint8_t),2,in_file);
    
    uint8_t x,y;
    
    x=c_buf[0];
    y=c_buf[1];
    
        cout << hex << " X=[" << setw(4) << (int) x << "]";
        cout << hex << " Y=[" << setw(4) << (int) y << "]";   
  
    fseek(in_file,in_off,SEEK_SET);
    fread(in_buf,sizeof(uint8_t),BUFFER_SIZE,in_file);
    
    fclose(in_file);
    
    uint16_t ref=0;
    uint16_t r=0;
    //uint16_t d=0;
    uint8_t cnt=0;
    uint8_t r1=0;
    uint8_t r2=0;
    
            
        uint16_t c=0;
        uint8_t c1=0;
        uint8_t c2=0;
        uint8_t e=0;
        uint32_t off=0;
      

    while (c_off)
    {

        cout << hex << " \nIn.pos.=[" << setw(4) << (int) in_pos << "]";
        cout << hex << " Out.pos.=[" << setw(4) << (int) out_pos << "]";

        c1=in_buf[in_pos];
        c2=in_buf[in_pos+1];
        
        if((c1==x)&&(c2==y))
        
        {
			in_pos++;
			in_pos++;
			cnt=in_buf[in_pos++];
			cout << hex << " CNT=[" << setw(4) << (int) cnt << "]";
			
			if (cnt==0) break;
			
			for(uint8_t i=0;i<cnt;i++)
			
			
			{
			//cout << hex << " [" << setw(4) << (int) in_buf[in_pos] << "]";
            out_buf[out_pos++]=in_buf[in_pos];				
            out_buf[out_pos++]=in_buf[in_pos+1];				
			}
			
			in_pos=in_pos+2;
			
		}
		
		
		
		else
		
		{
			out_buf[out_pos++]=in_buf[in_pos++];
		}
        
               
            
        }     

    
    
cout << hex << "\nDecompressed data size: " << (int)out_pos;
    fwrite(out_buf,sizeof(uint8_t),out_pos,out_file);
    fclose(out_file);
    return 0;
};
