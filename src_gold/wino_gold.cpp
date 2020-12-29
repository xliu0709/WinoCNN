#include <cstdlib>
#include <iostream>
#include "../src/wino_macro.h"
#include <ap_int.h>
#include "wino_cell_gold.hpp"
#include "wino_IO_gold.hpp"
#include "wino_gold.h"
#include "../testbench/debug.hpp"
#include "../src/wino_struct.h"


#define SHIFT_BIT 0

template<class Tin, class Tout>
void get_wino_input_tile(
    Tin* input,
    Tout* tile,
    int depth,
    int width,
    int height,
    int tilesize,
    int depth_idx,
    int start_output_row,
    int start_output_col,
    int pad_size
)
{
    for(int wino_row=0; wino_row<tilesize;wino_row++)
        for(int wino_col=0; wino_col<tilesize;wino_col++)
        {
            int col=start_output_col+wino_col-pad_size;
            int row=start_output_row+wino_row-pad_size;
            if(col<0 || col >= width || row < 0 || row >= height)
            {
                tile[wino_row*tilesize+wino_col]=0;
            }
            else
            {
                int address = depth_idx*height*width + row * width + col;
                tile[wino_row*tilesize+wino_col]=input[address];
            }
        }
}



template<class Tin, class Tout>
void get_wino_output_tile(
    Tin* output,
    Tout* tile,
    int depth,
    int width,
    int height,
    int tilesize,
    int depth_idx,
    int start_output_row,
    int start_output_col,
    int wino_out_size
)
{
    for(int wino_row=0; wino_row<tilesize;wino_row++)
        for(int wino_col=0; wino_col<tilesize;wino_col++)
        {
            int col=start_output_col+wino_col;
            int row=start_output_row+wino_row;
            if(col<0 || col >= width || row < 0 || row >= height)
            {
                tile[wino_row*wino_out_size+wino_col]=0;
            }
            else
            {
                int address = depth_idx*height*width + row * width + col;
                tile[wino_row*wino_out_size+wino_col]=output[address];
            }
        }
}


template<class Tin, class Tout>
void set_wino_output_tile(
    Tin* output,
    Tout* tile,
    int depth,
    int width,
    int height,
    int tilesize,
    int depth_idx,
    int start_output_row,
    int start_output_col, 
    int wino_out_size
)
{
    for(int wino_row=0; wino_row<tilesize;wino_row++)
        for(int wino_col=0; wino_col<tilesize;wino_col++)
        {
            int col=start_output_col+wino_col;
            int row=start_output_row+wino_row;
            if(col<0 || col >= width || row < 0 || row >= height)
            {
                continue;
            }
            else
            {
                int address = depth_idx*height*width + row * width + col;
                output[address]=tile[wino_row*wino_out_size+wino_col];
            }
        }
}




template<class Tin, class Tout>
void get_weight_tile(
    Tin* weight,
    Tout* wino_weight_tile,
    int indepth,
    int outdepth,
    int tilesize,
    int input_depth_idx,
    int output_depth_idx

)
{
        int address_offset=output_depth_idx*indepth*tilesize*tilesize +
                            input_depth_idx*tilesize*tilesize;

        for(int wino_idx=0; wino_idx<tilesize*tilesize;wino_idx++)
        {
            wino_weight_tile[wino_idx]= weight[address_offset+wino_idx];
        }
}


template<class Tin, class Tout>
void get_merge_weight_tile(
    Tin* weight,
    Tout* wino_weight_tile,
    int indepth,
    int outdepth,
    int tilesize,
    int input_depth_idx,
    int output_depth_idx,
    int tile_row_offset,
    int tile_col_offset

)
{
        int address_offset=output_depth_idx*indepth*tilesize*tilesize +
                            input_depth_idx*tilesize*tilesize;

        for(int r=0;r<3;r++)
        for(int c=0;c<3;c++)
        {
            int tile_row_idx=tile_row_offset+r;
            int tile_col_idx=tile_col_offset+c;
            if(tile_row_idx<tilesize && tile_col_idx < tilesize)
            {
                wino_weight_tile[r*3+c]=weight[address_offset+tile_row_idx*tilesize+tile_col_idx];
            }
            else
            {
                wino_weight_tile[r*3+c]=0;
            }
            
        }

}


template<int quant_bit>
void apply_quant_float(float* arr, int size)
{

    float quant_scale = 1.0f /(1<<quant_bit);
    for(int i=0;i<size;i++)
    {
        int rst=arr[i];
        rst=rst>>quant_bit;
        arr[i]=rst;
    }
}

template<int quant_bit>
void apply_quant_int(int* arr, int size)
{
    for(int i=0;i<size;i++)
    {
        arr[i]=arr[i]>>quant_bit;
    }
}

template<class T>
void print_tile( T* tile, int size, std::string name)
{
    std::cout<<name<<std::endl;
    for(int i=0;i<size;i++)
    {
        std::cout<<tile[i]<<" ";
    }
    std::cout<<std::endl;
    getchar();
    fflush(stdout);
}


void wino_model_float_offset(
    float* input,
    int input_depth,
    int input_height,
    int input_width,
    float* output,
    int output_depth,
    int output_height,
    int output_width,
    float* weight,
    float* bias,
    int kernel_size,
    int pad_size,
    int stride,
    bool relu_flag,
    int Scale_oback_int
)
{
    int wino_output_tile_size;
    if(kernel_size==5)
    {
       wino_output_tile_size=WINO_DOMAIN_SIZE+1-kernel_size;
    }
    else if(kernel_size==1)
    {
        wino_output_tile_size=WINO_DOMAIN_SIZE+1-1;
        std::cout<<"kernel_size equal 1"<<std::endl;
    }
    else
    {
        wino_output_tile_size=WINO_DOMAIN_SIZE+1-3;
    }
    

    int OH_ALIGNED = CEIL_DIV(output_height*stride,wino_output_tile_size);
    int OW_ALIGNED = CEIL_DIV(output_width*stride,wino_output_tile_size);

    std::vector<float*> output_temp_accum;

    for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
    {
        float* output_buffer=new float[output_depth*OH_ALIGNED*OW_ALIGNED*WINO_DOMAIN_SIZE_SQUARE];
        output_temp_accum.push_back(output_buffer);
    }

    float* output_buffer=new float[output_depth*output_height*stride*output_width*stride];

    for(int start_output_row =0; start_output_row < output_height*stride; start_output_row+=wino_output_tile_size )
    {
        int merge_kernel_size= (kernel_size==5) ?3:kernel_size;

        for(int merge_kernel_row_offset=0;merge_kernel_row_offset<merge_kernel_size;merge_kernel_row_offset+=3)
        for(int merge_kernel_col_offset=0;merge_kernel_col_offset<merge_kernel_size;merge_kernel_col_offset+=3)
        {
            for(int output_depth_idx=0;output_depth_idx<output_depth;output_depth_idx++)
            {
                for(int input_depth_idx=0;input_depth_idx<input_depth;input_depth_idx++)
                {
                    for(int start_output_col =0; start_output_col < output_width*stride; start_output_col+=wino_output_tile_size)
                    {
        
                        float input_tile[WINO_DOMAIN_SIZE_SQUARE];

                        get_wino_input_tile<float,float>(input,input_tile,input_depth,input_width,input_height,WINO_DOMAIN_SIZE,
                                                        input_depth_idx,start_output_row+merge_kernel_row_offset,start_output_col+merge_kernel_col_offset,pad_size);
                        
                        float dBT[WINO_DOMAIN_SIZE_SQUARE];
                        
                        // print_tile<float>(input_tile,WINO_DOMAIN_SIZE_SQUARE,"input_tile");
                        
                        float wino_input_tile[WINO_DOMAIN_SIZE_SQUARE];

                        #if WINO_DOMAIN_SIZE==4
                        if(kernel_size!=1)
                        {
                            input_right_mul_16<float>(input_tile,dBT);

                            apply_quant_float<DB_QUANT_BIT>(dBT,WINO_DOMAIN_SIZE_SQUARE);
                            // print_tile<float>(dBT,WINO_DOMAIN_SIZE_SQUARE,"dBT");

                            input_left_mul_16<float>(dBT,wino_input_tile);

                            apply_quant_float<BTB_QUANT_BIT>(wino_input_tile,WINO_DOMAIN_SIZE_SQUARE);
                        }
                        else
                        {
                            for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                            {
                                wino_input_tile[i]=input_tile[i];
                            }
                        }
                        #else
                        input_right_mul_36<float>(input_tile,dBT);
                        apply_quant_float<DB_QUANT_BIT>(dBT,WINO_DOMAIN_SIZE_SQUARE);

                        input_left_mul_36<float>(dBT,wino_input_tile);
                        apply_quant_float<BTB_QUANT_BIT>(wino_input_tile,WINO_DOMAIN_SIZE_SQUARE);
                        #endif
                        
                        // print_tile<float>(wino_input_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_input_tile");

                        float wino_weight_tile[WINO_DOMAIN_SIZE_SQUARE];
                        if(kernel_size!=5)
                        {
                            float g[WINO_DOMAIN_SIZE_SQUARE];
                            get_merge_weight_tile<float,float>(weight,g,input_depth,output_depth,kernel_size,input_depth_idx,output_depth_idx,merge_kernel_row_offset,merge_kernel_col_offset);
                                              
                            
                            float gGT[3*WINO_DOMAIN_SIZE];
                            #if WINO_DOMAIN_SIZE==4
                                if(kernel_size!=1)
                                {
                                    weight_right_mul_3to4<float>(g,gGT);
                                    weight_left_mul_3to4<float>(gGT,wino_weight_tile);    
                                    apply_quant_float<RG_WIDTH-W_WIDTH>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE);
                                    // print_tile<float>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_weight_tile");
                                }
                                else
                                {
                                    for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                                    {
                                        wino_weight_tile[i]=g[0]*4;
                                    }
                                }
                                
                            #else
                                weight_right_mul_3to6<float>(g,gGT);
                            
                                weight_left_mul_3to6<float>(gGT,wino_weight_tile);

                                // print_tile<float>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_weight_tile");

                                apply_quant_float<RG_WIDTH-W_WIDTH>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE);
                       
                                // for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++) wino_weight_tile[ii]=wino_weight_tile[ii]/576;
                                
                            #endif
                        }
                        else
                            get_weight_tile<float,float>(weight,wino_weight_tile,input_depth,output_depth,WINO_DOMAIN_SIZE,input_depth_idx,output_depth_idx);   
                        

                        int off = (output_depth_idx*OH_ALIGNED*OW_ALIGNED
                                                +start_output_row/wino_output_tile_size*OW_ALIGNED
                                                +start_output_col/wino_output_tile_size)*WINO_DOMAIN_SIZE_SQUARE;
                        
                
                        for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++)
                        { 

                            output_temp_accum[input_depth_idx% INDEPTH_MINITILE_SIZE][off+ii]=wino_weight_tile[ii]*wino_input_tile[ii];
                        }

                
                        // print_tile<float>(output_temp_accum[input_depth_idx% INDEPTH_MINITILE_SIZE]+off,WINO_DOMAIN_SIZE_SQUARE,"wino_mul");
                

                        if( input_depth_idx% INDEPTH_MINITILE_SIZE != INDEPTH_MINITILE_SIZE-1 && input_depth_idx != input_depth-1) continue;            

                        float wino_output_tile[WINO_DOMAIN_SIZE_SQUARE];

                        for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++) wino_output_tile[ii]=0;

                        for(int dd=0;dd<= input_depth_idx% INDEPTH_MINITILE_SIZE;dd++)
                        {
                                                                 
                            // print_tile<float>(output_temp_accum[dd]+off,WINO_DOMAIN_SIZE_SQUARE,"wino_mul");
                            for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++)
                            {
                                wino_output_tile[ii]+=output_temp_accum[dd][off+ii];
                            }
                        }
                        // print_tile<float>(wino_output_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_out_tile");

                
                        apply_quant_float<UV_QUANT_BIT>(wino_output_tile,WINO_DOMAIN_SIZE_SQUARE);



                        float output_tile[WINO_OUT_SIZE_SQUARE*4];

                        float vA[WINO_OUT_SIZE*WINO_DOMAIN_SIZE];   
                        if(kernel_size!=5 )
                        {
                            
                            #if WINO_DOMAIN_SIZE==4
                            if(kernel_size!=1)
                            {
                                output_right_mul_4to2<float>(wino_output_tile,vA);
                                apply_quant_float<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                                output_left_mul_4to2<float>(vA,output_tile);
                                apply_quant_float<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                            }
                            else
                            {
                                for(int i=0;i<WINO_OUT_SIZE_SQUARE*4;i++)
                                {
                                    output_tile[i]=wino_output_tile[i];
                                }
                            }
                            
                            #else
                                output_right_mul_6to4<float>(wino_output_tile,vA);
                                apply_quant_float<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                                output_left_mul_6to4<float>(vA,output_tile);
                                apply_quant_float<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                            #endif
                        }
                        else
                        {
                            output_right_mul_6to2<float>(wino_output_tile,vA);
                            apply_quant_float<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                            output_left_mul_6to2<float>(vA,output_tile);    
                            apply_quant_float<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE); 
                        }


                        apply_quant_float<OUT_BUFFER_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                  
                        // print_tile<float>(output_tile,WINO_OUT_SIZE_SQUARE,"output_tile");

                        float output_residual[WINO_OUT_SIZE_SQUARE*4];
                        if(input_depth_idx/INDEPTH_MINITILE_SIZE!=0 || merge_kernel_col_offset!=0 || merge_kernel_row_offset!=0)
                            get_wino_output_tile<float,float>(output_buffer,output_residual,output_depth,output_width*stride,output_height*stride,wino_output_tile_size,
                                                            output_depth_idx,start_output_row, start_output_col,wino_output_tile_size);
                        else
                            for(int ii=0;ii<wino_output_tile_size*wino_output_tile_size;ii++) output_residual[ii]=0;


                        float output_back[WINO_OUT_SIZE_SQUARE*4];
                        for(int ii=0;ii<wino_output_tile_size*wino_output_tile_size;ii++) 
                        {
                            output_back[ii]=output_residual[ii]+output_tile[ii];
                            // if(output_back[ii]> (1<<(OUT_WIDTH-1))-1 || output_back[ii]<((-1<<(OUT_WIDTH-1))))
                            // {
                            //     std::cout<<"output_back[ii] "<< output_back[ii]<<std::endl;
                            // }
                        }
                        // std::cout<<"start_output_col "<<start_output_col<<std::endl;
                        // print_tile<float>(output_back,WINO_OUT_SIZE_SQUARE,"output_back");
                        set_wino_output_tile<float,float>(output_buffer,output_back,output_depth,output_width*stride,output_height*stride,wino_output_tile_size,
                                    output_depth_idx,start_output_row, start_output_col, wino_output_tile_size);
                    }

                }

            }
        }
    }   
                    

    int addr=0;
    float max=0;
    float min=0;

    for(int od =0;od<output_depth;od++)
    {
        float bias_value=bias[od];
        // float bias_value=0;
        for(int oh=0;oh<output_height;oh++)
        for(int ow=0;ow<output_width;ow++)
        {
            int stride_addr=od*output_width*output_height*stride*stride+oh*stride*output_width*stride+ow*stride;
            // std::cout<<"["<<output[addr]<<" ";
            // std::cout<<" "<<bias_value<<"]";
            int output_int=output_buffer[stride_addr]*Scale_oback_int;
            output[addr]= (output_int>>OBACK_QUANT_BIT)+bias_value;

            // output[addr]=output_buffer[stride_addr]*Scale_oback_int;
            // if(max< output[addr]) 
            if(output[addr]<0 && relu_flag) output[addr]=0;
            addr++;
        }
    }
    std::cout<<"\n";


    for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
    {
        delete[] output_temp_accum[i];
    }
    delete [] output_buffer;
}







void wino_model_int(
    int8_t* input,
    int input_depth,
    int input_height,
    int input_width,
    int8_t* output,
    int output_depth,
    int output_height,
    int output_width,
    int8_t* weight,
    short* bias,
    int kernel_size,
    int pad_size,
    int stride,
    bool relu_flag,
    int Scale_oback_int
)
{

    std::cout<<"input_depth "<<input_depth<< std::endl;
    std::cout<<"input_height "<<input_height<< std::endl;
    std::cout<<"input_width "<<input_width<< std::endl;
    std::cout<<"output_depth "<<output_depth<< std::endl;
    std::cout<<"output_height "<<output_height<< std::endl;
    std::cout<<"output_width "<<output_width<< std::endl;
    std::cout<<"kernel_size "<<kernel_size<< std::endl;
    std::cout<<"pad_size "<<pad_size<< std::endl;
    std::cout<<"stride "<<stride<< std::endl;
    std::cout<<"relu_flag "<<relu_flag<< std::endl;
    std::cout<<"Scale_oback_int "<<Scale_oback_int<< std::endl;

    int wino_output_tile_size;
    if(kernel_size==5)
    {
       wino_output_tile_size=WINO_DOMAIN_SIZE+1-kernel_size;
    }
    else if(kernel_size==1)
    {
        wino_output_tile_size=WINO_DOMAIN_SIZE+1-1;
        std::cout<<"kernel_size equal 1"<<std::endl;
    }
    else
    {
        wino_output_tile_size=WINO_DOMAIN_SIZE+1-3;
    }


    int OH_ALIGNED = CEIL_DIV(output_height*stride,wino_output_tile_size);
    int OW_ALIGNED = CEIL_DIV(output_width*stride,wino_output_tile_size);



    std::vector<int*> output_temp_accum;

    for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
    {
        int* output_buf=new int[output_depth*OH_ALIGNED*OW_ALIGNED*WINO_DOMAIN_SIZE_SQUARE];
        output_temp_accum.push_back(output_buf);
    }

    int* output_buffer=new int[output_depth*output_height*stride*output_width*stride];
   std::cout<<"1"<<std::endl;
    fflush(stdout);
    for(int start_output_row =0; start_output_row < output_height*stride; start_output_row+=wino_output_tile_size )
    {
        int merge_kernel_size= (kernel_size==5) ?3:kernel_size;

        for(int merge_kernel_row_offset=0;merge_kernel_row_offset<merge_kernel_size;merge_kernel_row_offset+=3)
        for(int merge_kernel_col_offset=0;merge_kernel_col_offset<merge_kernel_size;merge_kernel_col_offset+=3)
        {
            for(int output_depth_idx=0;output_depth_idx<output_depth;output_depth_idx++)
            {
                for(int input_depth_idx=0;input_depth_idx<input_depth;input_depth_idx++)
                {
                    for(int start_output_col =0; start_output_col < output_width*stride; start_output_col+=wino_output_tile_size)
                    {
                        int input_tile[WINO_DOMAIN_SIZE_SQUARE];

                        get_wino_input_tile<int8_t,int>(input,input_tile,input_depth,input_width,input_height,WINO_DOMAIN_SIZE,
                                                        input_depth_idx,start_output_row+merge_kernel_row_offset,start_output_col+merge_kernel_col_offset,pad_size);

                        int dBT[WINO_DOMAIN_SIZE_SQUARE];

                        // print_tile<int>(input_tile,WINO_DOMAIN_SIZE_SQUARE,"input_tile");

                        
                        int wino_input_tile[WINO_DOMAIN_SIZE_SQUARE];
                        #if WINO_DOMAIN_SIZE==4
                        if(kernel_size!=1)
                        {
                            input_right_mul_16<int>(input_tile,dBT);

                            apply_quant_int<DB_QUANT_BIT>(dBT,WINO_DOMAIN_SIZE_SQUARE);
                            // print_tile<float>(dBT,WINO_DOMAIN_SIZE_SQUARE,"dBT");

                            input_left_mul_16<int>(dBT,wino_input_tile);

                            apply_quant_int<BTB_QUANT_BIT>(wino_input_tile,WINO_DOMAIN_SIZE_SQUARE);
                        }
                        else
                        {
                            for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                            {
                                wino_input_tile[i]=input_tile[i];
                            }
                        }
                        #else
                        input_right_mul_36<int>(input_tile,dBT);
                        apply_quant_int<DB_QUANT_BIT>(dBT,WINO_DOMAIN_SIZE_SQUARE);

                        input_left_mul_36<int>(dBT,wino_input_tile);
                        apply_quant_int<BTB_QUANT_BIT>(wino_input_tile,WINO_DOMAIN_SIZE_SQUARE);
                        #endif
                        
                        int wino_weight_tile[WINO_DOMAIN_SIZE_SQUARE];
                        if(kernel_size!=5)
                        {
                            int g[WINO_DOMAIN_SIZE_SQUARE];
                            get_merge_weight_tile<int8_t,int>(weight,g,input_depth,output_depth,kernel_size,input_depth_idx,output_depth_idx,merge_kernel_row_offset,merge_kernel_col_offset);
                            
                            // print_tile<int>(g,9,"weight_tile");
                            int gGT[3*WINO_DOMAIN_SIZE];
                            #if WINO_DOMAIN_SIZE==4
                                if(kernel_size!=1)
                                {
                                    weight_right_mul_3to4<int>(g,gGT);

                                    weight_left_mul_3to4<int>(gGT,wino_weight_tile);    
                                    apply_quant_int<RG_WIDTH-W_WIDTH>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE);
                                    for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++) wino_weight_tile[ii]=(ap_int<W_WIDTH>)wino_weight_tile[ii];
                                    // print_tile<int>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_weight_tile");
                                }
                                else
                                {
                                    for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                                    {
                                        wino_weight_tile[i]=g[0]*4;
                                    }
                                }
                            #else

                                weight_right_mul_3to6<int>(g,gGT);
                            
                                weight_left_mul_3to6<int>(gGT,wino_weight_tile);
                                apply_quant_int<RG_WIDTH-W_WIDTH>(wino_weight_tile,WINO_DOMAIN_SIZE_SQUARE);

                                for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++) wino_weight_tile[ii]=wino_weight_tile[ii]/576;
                            #endif
                        }
                        else
                            get_weight_tile<int8_t,int>(weight,wino_weight_tile,input_depth,output_depth,WINO_DOMAIN_SIZE,input_depth_idx,output_depth_idx);   
                        

                        int off = (output_depth_idx*OH_ALIGNED*OW_ALIGNED
                                                +start_output_row/wino_output_tile_size*OW_ALIGNED
                                                +start_output_col/wino_output_tile_size)*WINO_DOMAIN_SIZE_SQUARE;
                        

                        for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++)
                        { 
                            output_temp_accum[input_depth_idx% INDEPTH_MINITILE_SIZE][off+ii]=wino_weight_tile[ii]*wino_input_tile[ii];
                        }

                
                        // print_tile<int>(output_temp_accum[input_depth_idx% INDEPTH_MINITILE_SIZE]+off,WINO_DOMAIN_SIZE_SQUARE,"wino_mul");
                
                        if( input_depth_idx% INDEPTH_MINITILE_SIZE != INDEPTH_MINITILE_SIZE-1 && input_depth_idx != input_depth-1) continue;            

                        int wino_output_tile[WINO_DOMAIN_SIZE_SQUARE*4];
                        
                        for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++) wino_output_tile[ii]=0;

                        for(int dd=0;dd<= input_depth_idx% INDEPTH_MINITILE_SIZE;dd++)
                        {
                                        
                            // print_tile<int>(output_temp_accum[dd]+off,WINO_DOMAIN_SIZE_SQUARE,"wino_mul");

                            for(int ii=0;ii<WINO_DOMAIN_SIZE_SQUARE;ii++)
                            {
                                wino_output_tile[ii]+=output_temp_accum[dd][off+ii];
                            }
                        }
                        
                        // print_tile<int>(wino_output_tile,WINO_DOMAIN_SIZE_SQUARE,"wino_out_tile");

                
                        apply_quant_int<UV_QUANT_BIT>(wino_output_tile,WINO_DOMAIN_SIZE_SQUARE);




                        int output_tile[WINO_OUT_SIZE_SQUARE*4];

                        int vA[WINO_OUT_SIZE*WINO_DOMAIN_SIZE];   
                        if(kernel_size!=5)
                        {
                            
                            #if WINO_DOMAIN_SIZE==4
                            if(kernel_size!=1)
                            {
                                output_right_mul_4to2<int>(wino_output_tile,vA);
                                apply_quant_int<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                                // print_tile<int>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE,"vA");

                                output_left_mul_4to2<int>(vA,output_tile);
                                apply_quant_int<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                            }
                            else
                            {
                                for(int i=0;i<WINO_OUT_SIZE_SQUARE*4;i++)
                                {
                                    output_tile[i]=wino_output_tile[i];
                                }
                            }
                            #else
                                output_right_mul_6to4<int>(wino_output_tile,vA);
                                apply_quant_int<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                                output_left_mul_6to4<int>(vA,output_tile);
                                apply_quant_int<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                            #endif
                        }
                        else
                        {
                                output_right_mul_6to2<int>(wino_output_tile,vA);
                                apply_quant_int<UVA_QUANT_BIT>(vA,WINO_OUT_SIZE*WINO_DOMAIN_SIZE);
                                output_left_mul_6to2<int>(vA,output_tile);    
                                apply_quant_int<ATA_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE); 
                        }

                        apply_quant_int<OUT_BUFFER_QUANT_BIT>(output_tile,WINO_OUT_SIZE_SQUARE);
                        // print_tile<int>(output_tile,WINO_OUT_SIZE_SQUARE,"output_tile");

                        int output_residual[WINO_OUT_SIZE_SQUARE*4];
                        if(input_depth_idx/INDEPTH_MINITILE_SIZE!=0 || merge_kernel_col_offset!=0 || merge_kernel_row_offset!=0)
                            get_wino_output_tile<int,int>(output_buffer,output_residual,output_depth,output_width*stride,output_height*stride,wino_output_tile_size,
                                                            output_depth_idx,start_output_row, start_output_col,wino_output_tile_size);
                        else
                            for(int ii=0;ii<wino_output_tile_size*wino_output_tile_size;ii++) output_residual[ii]=0;


                        int output_back[WINO_OUT_SIZE_SQUARE*4];
                        for(int ii=0;ii<wino_output_tile_size*wino_output_tile_size;ii++) output_back[ii]=output_residual[ii]+output_tile[ii];

                        for(int ii=0;ii<wino_output_tile_size*wino_output_tile_size;ii++){
                            if( output_back[ii] >= OUT_SAT_MAX) {
                                std::cout<<"Soft enncountering MAX"<<std::endl;
                                output_back[ii]=OUT_SAT_MAX;

                            }
                            
                            else if( output_back[ii] <= OUT_SAT_MIN) 
                            {
                                std::cout<<"Soft enncountering MIN"<<std::endl;
                                output_back[ii]=OUT_SAT_MIN;
                            }
                        } 
                        
                        // print_tile<int>(output_back,WINO_OUT_SIZE_SQUARE,"output_back_tile");

                        set_wino_output_tile<int,int>(output_buffer,output_back,output_depth,output_width*stride,output_height*stride,wino_output_tile_size,
                                    output_depth_idx,start_output_row, start_output_col,wino_output_tile_size);

                    }

                }
            }
        }
    }   
                    

    int addr=0;
    int max=0;
    int min=0;
    for(int od =0;od<output_depth;od++)
    {
        int bias_value=bias[od];
        for(int oh=0;oh<output_height;oh++)
        for(int ow=0;ow<output_width;ow++)
        {
            
            // std::cout<<" "<<bias_value<<"]";
            int stride_addr=od*output_width*output_height*stride*stride+oh*stride*output_width*stride+ow*stride;
       
            int result= ((output_buffer[stride_addr]*Scale_oback_int)>>OBACK_QUANT_BIT)+bias_value;
            if( result >  (1<<(IN_WIDTH-1))-1) 
            {
                std::cout<<"Final Encounter Max"<<std::endl;
                result=(1<<(IN_WIDTH-1))-1;
            }
            else if(result <-(1<<(IN_WIDTH-1)))
            {
                std::cout<<"Final Encounter Min"<<std::endl;
                result=-(1<<(IN_WIDTH-1));
            }
            
            output[addr]=result;
            if(output[addr]<0 && relu_flag) output[addr]=0;            
            addr++;
        }
        // std::cout<<std::endl;
    }
    std::cout<<"\n";


    for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
    {
        delete[] output_temp_accum[i];
    }
    delete[] output_buffer;
}







