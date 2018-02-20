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
    const unsigned int POINTER_SIZE=0x2000;
    

    uint8_t in_buf[BUFFER_SIZE]={0};
    uint8_t out_buf[BUFFER_SIZE]={0};
        
    uint8_t p_buf[POINTER_SIZE]={0}; 
    
    uint8_t a_buf[1]={0};     
    uint8_t b_buf[1]={0};     
    uint8_t c_buf[1]={0};     
    

    uint32_t in_pos = 0;
    uint32_t out_pos = 0;
    uint32_t p_pos = 0;
    
    uint32_t a_off = 0x141BF6;
    uint32_t b_off = 0x141BF7;
    uint32_t c_off = 0x141BF8;
    
    uint32_t p_off = 0x141BF9;    
    
    uint32_t in_off = 0x143BF9;
    
     // char *off;
   // uint8_t header[2];
  //  uint16_t comp_size;
    //rle_t rle;

    FILE *in_file;
    FILE *out_file;
      
    const char* in_name = "Spawn (U) [!].smc";
    const char* out_name = "Spawn (U) [!]_decomp.bin";
  
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
    
    
    fseek(in_file,a_off,SEEK_SET);
    fread(a_buf,sizeof(uint8_t),1,in_file);
    
    fseek(in_file,b_off,SEEK_SET);
    fread(b_buf,sizeof(uint8_t),1,in_file);
    
    fseek(in_file,c_off,SEEK_SET);
    fread(c_buf,sizeof(uint8_t),1,in_file);
    
    fseek(in_file,p_off,SEEK_SET);
    fread(p_buf,sizeof(uint8_t),POINTER_SIZE,in_file);
    
    fseek(in_file,in_off,SEEK_SET);
    fread(in_buf,sizeof(uint8_t),BUFFER_SIZE,in_file);
    
    fclose(in_file);
    
  
            
        uint8_t c=0;
        uint8_t e=0;
    
    
    while (p_pos<0x2000)
    {


        cout << hex << "\nP.pos.=[" << setw(4) << (int) p_pos << "]";
        cout << hex << " In.pos.=[" << setw(4) << (int) in_pos << "]";
        cout << hex << " Out.pos.=[" << setw(4) << (int) out_pos << "]";

        c=p_buf[p_pos++];
        cout << hex << " C=[" << setw(4) << (int) c << "]" << endl;
        
        for (int i=0;i<4;i++)
        {
            cout << hex << " I=[" << setw(4) << (int) i << "]";            
            e=c&0xC0;
            e=e>>6;            
            cout << hex << " E=[" << setw(4) << (int) e << "]";
                        
            switch (e)
        {

        case (0x00):
        
            cout << hex << "\tCASE: 0x00" << " [" << setw(4) << (int) in_buf[in_pos] << "]" << endl;
            out_buf[out_pos++]=in_buf[in_pos++];
            break;
            
            
        case (0x01):

            cout << hex << "\tCASE: 0x01" << " [" << setw(4) << (int) a_buf[0] << "]" << endl;
            out_buf[out_pos++]=a_buf[0];
            break;                
            

        case (0x02):

            cout << hex << "\tCASE: 0x01" << " [" << setw(4) << (int) b_buf[0] << "]" << endl;
            out_buf[out_pos++]=b_buf[0];
            break;                
            
            
        case (0x03):
        
			cout << hex << "\tCASE: 0x01" << " [" << setw(4) << (int) c_buf[0] << "]" << endl;
            out_buf[out_pos++]=c_buf[0];
            break;                			
            
            break;           
        }
        
        cout << "SHIFT [C]";
        c=(c<<2)&0xff;    
            
            
        }
        
        

        
    }
cout << hex << "\nDecompressed data size: " << (int)out_pos;
    fwrite(out_buf,sizeof(uint8_t),out_pos,out_file);
    fclose(out_file);
    return 0;
};
