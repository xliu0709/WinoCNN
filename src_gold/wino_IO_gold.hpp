#ifndef __WINO_IO_GOLD_HPP_
#define __WINO_IO_GOLD_HPP_

#include "../src/wino_macro.h"
#include "../src/wino_struct.h"
#include <cstdlib>
#include <ap_int.h>




template<int dummy>
void load_input_row_DDR_uniformed(
    ap_int<8>* input0,
    ap_int<8>* input1,
    ap_uint<16> input_buffer[INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
    int input_height,
    int input_width,
    int input_width_align8,
    int input_depth,
    int input_depth_align8,
    int inwidth_ceildiv_inbufferwidth,
    int row_idx,
    ConvDesc_t conv_desc
)
{
    if(row_idx >= input_height) return;
	int buffer_address_offset;


    if(conv_desc.row_address_bitnumber_flag)
    {
        buffer_address_offset=(row_idx/INBUFFER_HEIGHT)%2*(INPUT_BUFFER_DEPTH/2);
    }
    else
    {
        buffer_address_offset=(row_idx/INBUFFER_HEIGHT)%4*(INPUT_BUFFER_DEPTH/4);
    }
    
	for(int col_idx=0;col_idx<input_width_align8;col_idx++)
	{
		for(int depth_idx=0;depth_idx<input_depth_align8;depth_idx++)
		{
			int depth_tile_idx = depth_idx/8;
			int depth_idx_in_tile = depth_idx%8;
			int col_idx_by_bufferwidth = col_idx/INBUFFER_WIDTH;
			int bank_idx = col_idx%INBUFFER_WIDTH;

			
			int buffer_address = buffer_address_offset
                                +depth_idx/INDEPTH_MINITILE_SIZE*inwidth_ceildiv_inbufferwidth*INDEPTH_MINITILE_SIZE
                                +col_idx_by_bufferwidth*INDEPTH_MINITILE_SIZE
                                +depth_idx%INDEPTH_MINITILE_SIZE;

            int ddr_address = depth_idx*input_height* input_width + row_idx*input_width+col_idx;

            ap_int<8> data0,data1;
            
            if(col_idx<input_width && depth_idx < input_depth)
            {
                data0= input0[ddr_address];
                data1= input1[ddr_address];
            }
            else
            {
                data0=0xCD;
                data1=0xCD;
            }



			ap_uint<16> value = (data1,data0);
			input_buffer[bank_idx][buffer_address] = value;
		}
	}
}

template<int dummy>
void load_input_DDR_uniform(
        ap_int<8>* input0,
        ap_int<8>* input1,
		ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
        int input_height,
        int input_width,
        int input_width_align8,
        int input_depth,
        int input_depth_align8,
        int inwidth_ceildiv_inbufferwidth,
        int next_start_output_row,
        int pad_size,
        ConvDesc_t conv_desc,
        bool first_flag

)
{
    puts("into");
    static int loaded_input_row_number;
    if(first_flag)
        loaded_input_row_number = 0;
    
    int required_loaded_input_row_number = (next_start_output_row+conv_desc.out_rowstep) + pad_size;

    if ( required_loaded_input_row_number > input_height )
        required_loaded_input_row_number = input_height;
    while( required_loaded_input_row_number > loaded_input_row_number)
    {
            load_input_row_DDR_uniformed<0>(
                input0,
                input1,
                input_buffer[loaded_input_row_number%INBUFFER_HEIGHT],
                input_height,
                input_width,
                input_width_align8,
                input_depth,
                input_depth_align8,
                inwidth_ceildiv_inbufferwidth,
                loaded_input_row_number,
                conv_desc
            );
            loaded_input_row_number++;
    }
    puts("outa");
    fflush(stdout);
}


template<int dummy>
void input_tile_feed(
    ap_int<IN_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_WIDTH][BATCH_SIZE][WINO_DOMAIN_SIZE_SQUARE],
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
    int input_width,
    int input_height,
    int stride,
    int pad_size,
    int inwidth_ceildiv_inbufferwidth,
    int wino_output_tile_size,
    int start_output_row,
    int start_output_col,
    int start_depth_idx,
    ConvDesc_t conv_desc
    )
{
    for(int indepth_inner_idx=0;indepth_inner_idx<INDEPTH_MINITILE_SIZE;indepth_inner_idx++)
    {

        int input_depth_idx = start_depth_idx+indepth_inner_idx;

        for(int batch_idx=0;batch_idx<BATCH_SIZE;batch_idx++)
        for(int wino_tile_idx=0;wino_tile_idx < WINO_WIDTH;wino_tile_idx++)
        for(int wino_row=0; wino_row<WINO_DOMAIN_SIZE;wino_row++)
        for(int wino_col=0; wino_col<WINO_DOMAIN_SIZE;wino_col++)
        {
            int input_col=start_output_col*stride+wino_col-pad_size+wino_tile_idx*wino_output_tile_size;
            int input_row=start_output_row*stride+wino_row-pad_size;
            if(input_col<0 || input_col >= input_width || input_row < 0 || input_row >= input_width)
            {
                input_tile[indepth_inner_idx][wino_tile_idx][batch_idx][wino_row*WINO_DOMAIN_SIZE+wino_col]=0;
            }
            else
            {
                int bank_col = input_col%INBUFFER_WIDTH;
                int bank_row = input_row%INBUFFER_HEIGHT;
                int bank_base_address;
                if(conv_desc.row_address_bitnumber_flag)
                {
                    bank_base_address = (input_row/INBUFFER_HEIGHT%2)*(INPUT_BUFFER_DEPTH/2);
                }
                else
                {
                    bank_base_address = (input_row/INBUFFER_HEIGHT%4)*(INPUT_BUFFER_DEPTH/4);
                }
                
                int bank_address = input_depth_idx/INDEPTH_MINITILE_SIZE*inwidth_ceildiv_inbufferwidth*INDEPTH_MINITILE_SIZE
                                +input_col/INBUFFER_WIDTH*INDEPTH_MINITILE_SIZE
                                +input_depth_idx%INDEPTH_MINITILE_SIZE;
                ap_uint<16> batch_value = input_buffer[bank_row][bank_col][bank_address+bank_base_address];
                input_tile[indepth_inner_idx][wino_tile_idx][batch_idx][wino_row*WINO_DOMAIN_SIZE+wino_col].range(IN_WIDTH-1,0)=
                batch_value.range(batch_idx*8+IN_WIDTH-1,batch_idx*8);
            }
        }
    }
}


#endif

