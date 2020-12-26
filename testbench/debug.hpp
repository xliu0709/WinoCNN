
#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

#include <ap_int.h>
#include <hls_stream.h>
#include <cstring>

#ifndef CEIL_DIV
#define CEIL_DIV(x,y)  ( ( (x) + (y) - 1) / (y) )
#endif

#ifndef ALIGN
#define ALIGN(x,y)  ( ( (x) + (y) - 1) / (y) * (y) )
#endif




template<int dummy>
void print_feature_map_ddr(char* input, char* filename, int length)
{
    printf("Writing file lenght as %d\n", length);
    assert(length%16==0);

    FILE* fptr = fopen(filename,"a");
  
    for(int i=0;i<length;i+=16)
    {
        fprintf(fptr, "ADDRESS[%5d]--",i/16);
        for(int j=0;j<16;j+=2)
        {
            fprintf(fptr,"[%4x %4x]", (unsigned char) input[i+j], (unsigned char) input[i+j+1]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

template<int dummy>
void print_weight(ap_uint<128>* weight, char* filename, int length)
{
    printf("Writing file lenght as %d\n", length);
   

    FILE* fptr = fopen(filename,"a");
  
    for(int i=0;i<length;i++)
    {
        fprintf(fptr, "ADDRESS[%5d]--",i);
        ap_uint<128> temp=weight[i];
        for(int j=0;j<18;j++)
        {
            fprintf(fptr,"[%3d]", (unsigned char) temp.range(j*7+6,j*7) );
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}


template<class T>
void print_feature_map_ddr_type(T* input, char* filename, int length)
{
    assert(length%16==0);

    FILE* fptr = fopen(filename,"a");
  
    for(int i=0;i<length;i+=16)
    {
        fprintf(fptr, "ADDRESS[%5d]--",i/16);
        for(int j=0;j<16;j+=2)
        {
            fprintf(fptr,"[%8d %8d]", (unsigned int) input[i+j], (unsigned int) input[i+j+1]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}


template<class T>
void print_feature_map(
    T *feature_map,
    char* filename,
    int height,
    int width,
    int depth)
{
    printf("print dimension: %d %d %d\n", height, width, depth);
    FILE* fptr = fopen(filename,"w");
    for(int i=0;i<height;i++)
    for(int j=0;j<width;j++)
    {
        fprintf(fptr, "ADDRESS[%3d %3d]: ",i,j);
        for(int k=0;k<depth;k++)
        {
            int address = k*height*width +i*width+j;
            fprintf(fptr,"[%8d]", (int) (feature_map[address]) );
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

template<int BUFFER_HEIGHT, int BUFFER_WIDTH, int BUFFER_DEPTH>
void clear_buffer_content(ap_uint<16> input_buffer[BUFFER_HEIGHT][BUFFER_WIDTH][BUFFER_DEPTH])
{
    for(int i=0;i<BUFFER_HEIGHT;i++)
    {
        for(int j=0;j<BUFFER_WIDTH;j++)
        {
            memset(input_buffer[i][j], 0xaa, BUFFER_DEPTH*2 );
        }
    }
}


template<int out_width, int batch_size, int wino_height, int wino_width, int wino_out_size, int outbuffer_depth>
void clear_output_buffer_content_uniformed(
    ap_uint<out_width*batch_size> output_buffer[wino_height][wino_width][wino_out_size*wino_out_size][outbuffer_depth]
)
{
    for(int wh=0;wh<wino_height;wh++)
    {
        for(int wr=0;wr<wino_out_size;wr++)
        {
            for(int ww=0;ww<wino_width;ww++)
            {
                for(int wc=0;wc<wino_out_size;wc++)
                {
                    memset(output_buffer[wh][ww][wr*wino_out_size+wc],0xAA,sizeof(ap_uint<out_width*batch_size>)*outbuffer_depth);
                }
            }
        }
    }
}


template<int out_width, int batch_size, int wino_height, int wino_width, int wino_out_size, int outbuffer_depth>
void clear_output_buffer_content_uniformed_hw(
    ap_uint<out_width*batch_size> output_buffer[wino_out_size][wino_out_size][wino_height][wino_width][outbuffer_depth]
)
{
    for(int wh=0;wh<wino_height;wh++)
    {
        for(int wr=0;wr<wino_out_size;wr++)
        {
            for(int ww=0;ww<wino_width;ww++)
            {
                for(int wc=0;wc<wino_out_size;wc++)
                {
                    memset(output_buffer[wr][wc][wh][ww],0xAA,sizeof(ap_uint<out_width*batch_size>)*outbuffer_depth);
                }
            }
        }
    }
}






template<int BUFFER_HEIGHT, int BUFFER_WIDTH, int BUFFER_DEPTH>
void attach_input_buffer_content_uniformed(
    ap_uint<16> input_buffer[BUFFER_HEIGHT][BUFFER_WIDTH][BUFFER_DEPTH],
    int idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[%d]\n",idx);
    for( int address =0; address < BUFFER_DEPTH;  address++ )
    {
        for(int i=0;i<BUFFER_HEIGHT;i++)
        {
            fprintf(fptr, "ADDR %4d:", address);

            for(int j=0;j<BUFFER_WIDTH;j++)
            {
                fprintf(fptr,"[%3d %3d]", (unsigned char) input_buffer[i][j][address].range(7,0),  (unsigned char) input_buffer[i][j][address].range(15,8));
            }
            fprintf(fptr,"\n");
        }
    }
    fclose(fptr);
}


template<int w_width, int wino_domain_size,int indepth_minitile_size, int feeding_port_number, int buffer_depth>
void attach_weight_buffer_content_uniformed(
    ap_uint<w_width*4> 
    weight_buffer[feeding_port_number][wino_domain_size*wino_domain_size*indepth_minitile_size/4][buffer_depth],
    int idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[%d]\n",idx);
    for( int address =0; address < buffer_depth;  address++ )
    {
        fprintf(fptr, "ADDR %4d:\n", address);
        for(int k=0;k<feeding_port_number;k++)
        {
            for(int i=0;i<indepth_minitile_size;i++)
            {
                for(int j=0;j<wino_domain_size*wino_domain_size;j++)
                {
                    int idx=i*wino_domain_size*wino_domain_size+j;
                    int bit=idx%4;
                    int bank=idx/4;
                    fprintf(fptr,"[%2x]", (unsigned char)weight_buffer[k][bank][address].range( 
                                            bit*w_width+w_width-1,
                                           bit*w_width));
                }
                fprintf(fptr,"\n");
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

template<int out_width, int batch_size, int wino_height, int wino_width, int wino_out_size, int outbuffer_depth>
void attach_output_buffer_content_uniformed(
    ap_uint<out_width*batch_size> output_buffer[wino_height][wino_width][wino_out_size*wino_out_size][outbuffer_depth],
    int idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[%d]\n",idx);
    for( int address =0; address < outbuffer_depth;  address++ )
    {
        fprintf(fptr, "ADDR %4d:\n", address);
        for(int wh=0;wh<wino_height;wh++)
        {
            for(int wr=0;wr<wino_out_size;wr++)
            {
                for(int ww=0;ww<wino_width;ww++)
                {
                    for(int wc=0;wc<wino_out_size;wc++)
                    {
                        fprintf(fptr,"[%8x %8x]", (int) output_buffer[wh][ww][wr*wino_out_size +wc][address].range(out_width-1,0), 
                                                (int) output_buffer[wh][ww][wr*wino_out_size +wc][address].range(out_width*2-1,out_width) );
                    }
                    fprintf(fptr," ");
                }
                fprintf(fptr,"\n");
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

template<int out_width, int batch_size, int wino_height, int wino_width, int wino_out_size, int outbuffer_depth>
void attach_output_buffer_content_uniformed_hw(
    ap_uint<out_width*batch_size> output_buffer[wino_out_size][wino_out_size][wino_height][wino_width][outbuffer_depth],
    int idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[%d]\n",idx);
    for( int address =0; address < outbuffer_depth;  address++ )
    {
        fprintf(fptr, "ADDR %4d:\n", address);
        for(int wh=0;wh<wino_height;wh++)
        {
            for(int wr=0;wr<wino_out_size;wr++)
            {
                for(int ww=0;ww<wino_width;ww++)
                {
                    for(int wc=0;wc<wino_out_size;wc++)
                    {
                        fprintf(fptr,"[%8d %8d]", (int) output_buffer[wr][wc][wh][ww][address].range(out_width-1,0), 
                                                (int) output_buffer[wr][wc][wh][ww][address].range(out_width*2-1,out_width) );
                    }
                    fprintf(fptr," ");
                }
                fprintf(fptr,"\n");
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}


template<int in_width,int wino_width, int batch_size, int wino_domain_size >
void attach_streaming_content_gold(
    ap_int<in_width> input_buffer[wino_width][batch_size][wino_domain_size*wino_domain_size],
    int start_row_idx,
    int start_col_idx,
    int depth_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[r:%d c:%d d%d]\n",start_row_idx,start_col_idx,depth_idx);
    
    for(int i=0;i<wino_domain_size;i++)
    {
        for(int wino_array_col=0;wino_array_col<wino_width;wino_array_col++)
        {
            for(int j=0;j<wino_domain_size;j++)
            {
                fprintf(fptr,"[%3d %3d]", (unsigned char) input_buffer[wino_array_col][0][i*wino_domain_size+j],  (unsigned char) input_buffer[wino_array_col][1][i*wino_domain_size+j]);
            }
            fprintf(fptr,"       ");
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}


template<int wino_width, int wino_domain_size>
void attach_streaming_content(
    ap_uint<16*wino_domain_size*wino_domain_size> input_buffer[wino_width],
    int start_row_idx,
    int start_col_idx,
    int depth_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");

    fprintf(fptr, "--Log idx--[r:%d c:%d d%d]\n",start_row_idx,start_col_idx,depth_idx);
    
    for(int i=0;i<wino_domain_size;i++)
    {
        for(int wino_array_col=0;wino_array_col<wino_width;wino_array_col++)
        {
            for(int j=0;j<wino_domain_size;j++)
            {
                fprintf(fptr,"[%3d %3d]", (unsigned char) input_buffer[wino_array_col].range( (i*wino_domain_size+j)*16+7,(i*wino_domain_size+j)*16),  (unsigned char) input_buffer[wino_array_col].range((i*wino_domain_size+j)*16+15,(i*wino_domain_size+j)*16+8));
            }
            fprintf(fptr,"       ");
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}








template<int in_width,int indepth_tile_size, int wino_domain_size, int batch_size>
void attach_input_vector(
    ap_int<in_width> input[indepth_tile_size][wino_domain_size][wino_domain_size][batch_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int id=0;id<indepth_tile_size;id++)
    {
        for(int bk_row=0;bk_row<wino_domain_size;bk_row++)
        {
            for(int bk_col=0;bk_col<wino_domain_size;bk_col++)
            {
                fprintf(fptr,"[%8d %8d]", (int) input[id][bk_row][bk_col][0],(int) input[id][bk_row][bk_col][1]);
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fprintf(fptr,"\n");
    fclose(fptr);
}

template<int in_width,int indepth_tile_size, int wino_domain_size, int batch_size>
void attach_input_vector_gold(
    ap_int<in_width> input[indepth_tile_size][batch_size][wino_domain_size*wino_domain_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int id=0;id<indepth_tile_size;id++)
    {
        for(int bk_row=0;bk_row<wino_domain_size;bk_row++)
        {
            for(int bk_col=0;bk_col<wino_domain_size;bk_col++)
            {
                fprintf(fptr,"[%08x %08x]", (unsigned int) input[id][0][bk_row*wino_domain_size+bk_col],(unsigned int) input[id][1][bk_row*wino_domain_size+bk_col]);
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fprintf(fptr,"\n");
    fclose(fptr);
}

template<int w_width,int indepth_tile_size, int wino_domain_size>
void attach_weight_vector(
    ap_int<w_width> weight[indepth_tile_size][wino_domain_size][wino_domain_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int id=0;id<indepth_tile_size;id++)
    {
        for(int bk_row=0;bk_row<wino_domain_size;bk_row++)
        {
            for(int bk_col=0;bk_col<wino_domain_size;bk_col++)
            {
                fprintf(fptr,"[%4d]", (int) weight[id][bk_row][bk_col]);
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fprintf(fptr,"\n");
    fclose(fptr);
}

template<int w_width,int indepth_tile_size, int wino_domain_size>
void attach_weight_vector_gold(
    ap_int<w_width> weight[indepth_tile_size][wino_domain_size*wino_domain_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int id=0;id<indepth_tile_size;id++)
    {
        for(int bk_row=0;bk_row<wino_domain_size;bk_row++)
        {
            for(int bk_col=0;bk_col<wino_domain_size;bk_col++)
            {
                fprintf(fptr,"[%2x]", (unsigned char) weight[id][bk_row*wino_domain_size+bk_col]);
            }
            fprintf(fptr,"\n");
        }
        fprintf(fptr,"\n");
    }
    fprintf(fptr,"\n");
    fclose(fptr);
}




template<int out_width, int wino_out_size, int batch_size>
void attach_output_vector(
    ap_int<out_width> output[wino_out_size][wino_out_size][batch_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int bk_row=0;bk_row<wino_out_size;bk_row++)
    {
        for(int bk_col=0;bk_col<wino_out_size;bk_col++)
        {
            fprintf(fptr,"[%8d %8d]", (unsigned int) output[bk_row][bk_col][0],(unsigned int) output[bk_row][bk_col][1]);
        }
        fprintf(fptr,"\n");
    }

    fprintf(fptr,"\n");
    fclose(fptr);
}






template<int out_width, int wino_out_size, int batch_size>
void attach_output_vector_gold(
    ap_int<out_width> output[batch_size][wino_out_size*wino_out_size],
    int write_idx,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--Write idx %3d --  \n",write_idx);

    for(int bk_row=0;bk_row<wino_out_size;bk_row++)
    {
        for(int bk_col=0;bk_col<wino_out_size;bk_col++)
        {
            fprintf(fptr,"[%8x %8x]", (unsigned int) output[0][bk_row*wino_out_size+bk_col],(unsigned int) output[1][bk_row*wino_out_size+bk_col]);
        }
        fprintf(fptr,"\n");
    }

    fprintf(fptr,"\n");
    fclose(fptr);
}



template<int indepth_minitile_size,int wino_domain_size, int win_domain_size_square>
void attach_weight_stream_content(
    hls::stream< ap_uint<8*indepth_minitile_size*win_domain_size_square> > & stream_data,
    char* filename 
)
{
    hls::stream< ap_uint<8*indepth_minitile_size*win_domain_size_square> > stream_data_save;
    FILE* fptr = fopen(filename,"a");
    int stream_index=0;
    ap_uint<8*indepth_minitile_size*win_domain_size_square> data;
    while(stream_data.read_nb(data) )
    {
        fprintf(fptr, "--stream idx %3d --  \n",stream_index);

        for(int j=0;j<indepth_minitile_size;j++)
        {
            for(int i=0;i<36;i++)
            {
                fprintf(fptr,"[%3d]", (unsigned char) data.range(i*8+7+8*win_domain_size_square*j,i*8+8*win_domain_size_square*j));
            }
            fprintf(fptr,"\n");
        }
        stream_data_save<<data;
        stream_index++;
    }
    while(stream_data_save.read_nb(data) )
    {
        stream_data<<data;
    }
    fclose(fptr);
}

template<int w_width, int indepth_minitile_size,int wino_domain_size_square>
void attach_weight_stream_content_gold(
    ap_int<w_width> data[indepth_minitile_size][wino_domain_size_square],
    int stream_index,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--stream idx %3d --  \n",stream_index);

    for(int j=0;j<indepth_minitile_size;j++)
    {
        for(int i=0;i<wino_domain_size_square;i++)
        {
            fprintf(fptr,"[%3d]", (unsigned char) data[j][i]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

template<int dummy>
void wino_cell_stream_input(
    ap_uint<32*36>  stream_data,
    int &index,
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr, "--stream idx %3d --  \n",index);
    for(int row_idx=0;row_idx<6;row_idx++)
    {
        for(int col_idx=0;col_idx<6;col_idx++)
        {
            int flatten_idx = row_idx*6+col_idx;
            fprintf(fptr,"[%4x %4x]",(unsigned short) stream_data.range(flatten_idx*32+15, flatten_idx*32 ), 
            (unsigned short) stream_data.range(flatten_idx*32+31, flatten_idx*32+16 ));
        }
        fprintf(fptr,"\n");
    }
    index++;
    fclose(fptr);
}





template<int dummy>
void attach_output_buffer_content(
    ap_uint<36>  output_buffer[16][16][1024],
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr,"--------------------NEWPRINT -----------------------\n");
    for(int i=0;i<16;i++)
    {
    
        fprintf(fptr,"--------------------NEWBANK  %d-----------------------\n", i);
        for(int addr=0;addr<1024;addr++)
        {
            fprintf(fptr,"DETPH %d, TILE %d ADDR %d:\n", i%8,i/8,addr);
            for(int r=0;r<4;r++)
            {
                for(int c=0;c<4;c++ )
                {
                    fprintf(fptr,"[%4x %4x]", (unsigned short) output_buffer[i][r*4+c][addr].range(18,0), (unsigned short) output_buffer[i][r*4+c][addr].range(35,18) );
                    // fprintf(fptr,"[%d %d %d %d]",  (unsigned int) output_buffer[i][r*4+c][addr].range(17,0)/56/56 , (unsigned int) output_buffer[i][r*4+c][addr].range(17,0)/56%56, (unsigned int) output_buffer[i][r*4+c][addr].range(17,0)%56  );
                }
                fprintf(fptr,"\n");
            }
        }
    }
    fprintf(fptr,"\n");
    fclose(fptr);
}

template<int dummy>
void diff_two_ddr(
    char* result,
    char* gold,
    int height,
    int width,
    int depth
)
{
    
    int depth_ceildiv8 = CEIL_DIV(depth,8);
    int width_align8 = ALIGN(width,8);



    int ddr_address = 0;
    for(int  depth_tile8_idx =0; depth_tile8_idx < depth_ceildiv8; depth_tile8_idx++)
    {
        for(int row=0; row<height;row++)
        for(int col=0; col<width; col++)
        {
            for(int idx_tile8=0;idx_tile8<8;idx_tile8++)
            {
 
                int depth_idx = depth_tile8_idx *8+ idx_tile8;
                if(depth_idx <depth)
                {
   
                    int feature_map_address= depth_idx*height*width +row*width +col;
                    unsigned char data1_batch0=  result[ddr_address];
                    unsigned char data1_batch1=  result[ddr_address+1];
                    unsigned char data2_batch0=  gold[ddr_address];
                    unsigned char data2_batch1=  gold[ddr_address+1];
                    ddr_address+=2;

                    if( data1_batch0 != data2_batch0 || data1_batch1 != data2_batch1 )
                    {
                        printf(" (%d %d %d) [%2x %2x] [%2x %2x]\n", depth_idx, row,col, data1_batch0 , data2_batch0, data1_batch1, data2_batch1 );
                    }

                }
            }
        }
    }
}

template<int BUFFER_DEPTH>
void attach_misc_inbuffer_content(
    ap_uint<16>  inbuffer[2][8][BUFFER_DEPTH],
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr,"--------------------NEWPRINT -----------------------\n");
    for(int addr=0;addr<BUFFER_DEPTH;addr++)
    {
        fprintf(fptr,"ADDR %6d:", addr);
        for(int i=0;i<16;i++)
        {
            fprintf(fptr,"[%4d]", (unsigned char) inbuffer[i/8][i%8][addr].range(7,0) );
        }
        fprintf(fptr,"\n");
        fprintf(fptr,"ADDR %6d:", addr);
        for(int i=0;i<16;i++)
        {
            fprintf(fptr,"[%4d]", (unsigned char) inbuffer[i/8][i%8][addr].range(15,8) );
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}


template<int BUFFER_DEPTH>
void attach_misc_outbuffer_content(
    ap_uint<18>  outbuffer[2][8][BUFFER_DEPTH],
    char* filename 
)
{
    FILE* fptr = fopen(filename,"a");
    fprintf(fptr,"--------------------NEWPRINT -----------------------\n");
    for(int addr=0;addr<BUFFER_DEPTH;addr++)
    {
        fprintf(fptr,"ADDR %6d:", addr);
        for(int i=0;i<16;i++)
        {
            fprintf(fptr,"[%4d]", (unsigned char) outbuffer[i/8][i%8][addr].range(7,0) );
        }
        fprintf(fptr,"\n");
        fprintf(fptr,"ADDR %6d:", addr);
        for(int i=0;i<16;i++)
        {
            fprintf(fptr,"[%4d]", (unsigned char) outbuffer[i/8][i%8][addr].range(15,8) );
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
}

#endif