#include "wino_macro.h"
#include "wino_struct.h"

#include <ap_int.h>
#include <cstdlib>




#ifndef _WINO_IO_HPP_
#define _WINO_IO_HPP_

void load_bias_value(
	ap_int<32> *bias_mem,
	ap_int<16> biasbuffer0[8][BIAS_BUFFER_DEPTH],
	ap_int<16> biasbuffer1[8][BIAS_BUFFER_DEPTH],
	int outdepth
)
{
	int outdepth_by2=outdepth>>1;

	ap_uint<2> buffer_idx=0;
	ap_uint<10> buffer_addr=0;
	for(ap_uint<16> i=0;i<outdepth_by2;i++)
	{
		#pragma HLS pipeline
		ap_int<16> temp0,temp1;

		biasbuffer0[buffer_idx*2+1][buffer_addr]=temp1;
		biasbuffer1[buffer_idx*2+1][buffer_addr]=temp1;
		biasbuffer0[buffer_idx*2][buffer_addr]=temp0;
		biasbuffer1[buffer_idx*2][buffer_addr]=temp0;
		
		(temp1,temp0)=bias_mem[i];
		
		if(buffer_idx==3)
		{
			buffer_addr++;
		}
		buffer_idx++;
	}

}

// template<int port_idx>
// void load_input_row_from_ddr_new(
// 		#if INBUFFER_WIDTH<=4
// 		ap_uint<INFM_WORD_WIDTH>* DDR_port,
// 		#else
// 		ap_uint<128>* DDR_port,
// 		#endif
// 		ap_uint<16> input_buffer[INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
// 		ap_uint<16> input_buffer1[INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
// 		ap_uint<16> inheight,
// 		ap_uint<16> inwidth,
// 		ap_uint<16> inwidth_align8,
// 		ap_uint<16> indepth_align8,
// 		ap_uint<16> group_indepth_x_inwidth_align8_by8,
// 		ap_uint<16> group_indepth_offset_x_inwidth_align8_by8,
// 		ap_uint<16> inwidth_ceildiv_inbufferwidth,
// 		ap_uint<16> buffer_address_mid_increment_step,
// 		ap_uint<16> input_load_burst_length,
// 		ap_uint<1> row_address_bitnumber_flag,
// 		ap_uint<16> row_idx,
// 		ap_int<16> row_load_bound 
// )
// {
// 	#pragma HLS array_partition variable = input_buffer dim=1 complete

// 	if(row_idx >= inheight || row_idx >= row_load_bound) return;

// 	bool bufferflag= (row_idx%INBUFFER_HEIGHT/4 == 0);


// 	// #if DEBUG_FILE_PRINT
// 		printf("\tLoading row_idx %d\n", (int) row_idx);
// 	// #endif


// 	ap_uint<16> register_matrix[8][8];
// 	#pragma HLS array_partition variable=register_matrix dim=1 complete
// 	#pragma HLS array_partition variable=register_matrix dim=2 complete

// 	ap_uint<1> row_pingpong_bit1 = row_idx[INBUFFER_HEIGHT_BITWIDTH];
// 	ap_uint<2> row_pingpong_bit2 = row_idx.range(INBUFFER_HEIGHT_BITWIDTH+1, INBUFFER_HEIGHT_BITWIDTH);


// 	ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid=0;
// 	ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_offset=0;


// 	#if DEBUG_FILE_PRINT
// 		if(row_address_bitnumber_flag)
// 		{
// 			if( inwidth_ceildiv_inbufferwidth*indepth_align8 > (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 1)) )
// 			{
// 				printf("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n", (int) inwidth_ceildiv_inbufferwidth, (int) indepth_align8, (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 1)) );
// 				exit(-1);
// 			}
// 		}
// 		else
// 		{
// 			if( inwidth_ceildiv_inbufferwidth*indepth_align8 > (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 2)) )
// 			{
// 				printf("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n", (int) inwidth_ceildiv_inbufferwidth, (int) indepth_align8, (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 2)) );
// 				exit(-1);
// 			}
// 		}
		
// 	#endif


// 	ap_uint<INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mini_tile=0;

// 	ap_uint<8> read_inwidth_counter=1;
// 	ap_uint<8> write_inwidth_counter=1;

// 	ap_uint<32> ddr_address_offset = row_idx * group_indepth_x_inwidth_align8_by8 + group_indepth_offset_x_inwidth_align8_by8;

// 	ap_uint<128>* DDR_offset = DDR_port + ddr_address_offset;
	
// 	ap_uint<1> left_down_flag=1;
	
// 	ap_uint<INBUFFER_WIDTH_BITWIDTH-3> bank_split_idx=0;

// 	for(ap_uint<16> cycle=0;cycle<input_load_burst_length;cycle++)
// 	{
// 		#pragma HLS pipeline
// 		ap_uint<INFM_WORD_WIDTH> DDR_data;

// 		DDR_data = DDR_offset[cycle];
	
// 		ap_uint<16> DDR_data_divided[INFM_WORD_WIDTH/16];
// 		#pragma HLS array_partition variable = DDR_data_divided complete

// 		for(int i=0;i<INFM_WORD_WIDTH/16;i++)
// 		{
// 		#pragma HLS unroll
// 			DDR_data_divided[i]=DDR_data.range(i*16+15,i*16);
// 		}

// 		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH> buffer_addr; 
// 		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit1_offset;
// 		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 2 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit2_offset;
		
// 		buffer_address_mid_rowbit1_offset=buffer_address_mid_offset;
// 		buffer_address_mid_rowbit2_offset=buffer_address_mid_offset;

// 		if(row_address_bitnumber_flag)
// 		{
// 			buffer_addr=(row_pingpong_bit1,buffer_address_mid_rowbit1_offset,buffer_address_mini_tile);
// 		}
// 		else
// 		{
// 			buffer_addr=(row_pingpong_bit2,buffer_address_mid_rowbit2_offset,buffer_address_mini_tile);
// 		}


	
// 		#if INBUFFER_HEIGHT == 8
// 		if(bufferflag)
// 		{
// 		#endif
// 			input_buffer[bank_split_idx*8+0][buffer_addr]=bank_data[0];
// 			input_buffer[bank_split_idx*8+1][buffer_addr]=bank_data[1];
// 			input_buffer[bank_split_idx*8+2][buffer_addr]=bank_data[2];
// 			input_buffer[bank_split_idx*8+3][buffer_addr]=bank_data[3];
// 			input_buffer[bank_split_idx*8+4][buffer_addr]=bank_data[4];
// 			input_buffer[bank_split_idx*8+5][buffer_addr]=bank_data[5];
// 			input_buffer[bank_split_idx*8+6][buffer_addr]=bank_data[6];
// 			input_buffer[bank_split_idx*8+7][buffer_addr]=bank_data[7];

// 		#if INBUFFER_HEIGHT == 8
// 		}
// 		else
// 		{
// 			input_buffer1[bank_split_idx*8+0][buffer_addr]=bank_data[0];
// 			input_buffer1[bank_split_idx*8+1][buffer_addr]=bank_data[1];
// 			input_buffer1[bank_split_idx*8+2][buffer_addr]=bank_data[2];
// 			input_buffer1[bank_split_idx*8+3][buffer_addr]=bank_data[3];
// 			input_buffer1[bank_split_idx*8+4][buffer_addr]=bank_data[4];
// 			input_buffer1[bank_split_idx*8+5][buffer_addr]=bank_data[5];
// 			input_buffer1[bank_split_idx*8+6][buffer_addr]=bank_data[6];
// 			input_buffer1[bank_split_idx*8+7][buffer_addr]=bank_data[7];
// 		}
// 		#endif

// 		if(write_inwidth_counter == inwidth_align8)
// 		{
// 			buffer_address_mid += buffer_address_mid_increment_step;
// 		}
// 		else if( write_inwidth_counter.range(INBUFFER_WIDTH_BITWIDTH-1,0) == 0 )
// 		{
// 			buffer_address_mid++;
// 		}

// 		if( write_inwidth_counter.range(2,0) == 0)
// 		{
// 			buffer_address_mid_offset = buffer_address_mid;
// 		}
// 		else if(buffer_address_mini_tile == INDEPTH_MINITILE_SIZE-1)
// 		{
// 			buffer_address_mid_offset += inwidth_ceildiv_inbufferwidth;
// 		}

// 		if(write_inwidth_counter == inwidth_align8/INFM_WORD_BYTES)
// 		{
// 			buffer_address_mid_offset=buffer_address_mid;
// 		}
// 		else if( bank_split_idx==INBUFFER_WIDTH/INFM_WORD_BYTES-1)
// 		{
// 			buffer_address_mid_offset+=1;
// 		}
		
// 		if(write_inwidth_counter == inwidth_align8/INFM_WORD_BYTES)
// 		{
// 			write_inwidth_counter=1;
// 			buffer_address_mini_tile++;
// 			bank_split_idx=0;
// 		}
// 		else
// 		{
// 			write_inwidth_counter++;
// 			bank_split_idx++;
// 		}

// 	}
// }

template<int port_idx>
void load_input_row_from_ddr(
		ap_uint<128>* DDR_port,
		ap_uint<16> input_buffer[INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
		#if INBUFFER_HEIGHT ==8
		ap_uint<16> input_buffer1[INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
		#endif
		ap_uint<16> inheight,
		ap_uint<16> inwidth,
		ap_uint<16> inwidth_align8,
		ap_uint<16> indepth_align8,
		ap_uint<16> group_indepth_x_inwidth_align8_by8,
		ap_uint<16> group_indepth_offset_x_inwidth_align8_by8,
		ap_uint<16> inwidth_ceildiv_inbufferwidth,
		ap_uint<16> buffer_address_mid_increment_step,
		ap_uint<16> input_load_burst_length,
		ap_uint<1> row_address_bitnumber_flag,
		ap_uint<16> row_idx,
		ap_int<16> row_load_bound)
{
	#pragma HLS array_partition variable = input_buffer dim=1 complete

	if(row_idx >= inheight || row_idx >= row_load_bound) return;

	bool bufferflag= (row_idx%INBUFFER_HEIGHT/4 == 0);


	// #if DEBUG_FILE_PRINT
		printf("\tLoading row_idx %d\n", (int) row_idx);
	// #endif


	ap_uint<16> register_matrix[8][8];
	#pragma HLS array_partition variable=register_matrix dim=1 complete
	#pragma HLS array_partition variable=register_matrix dim=2 complete

	ap_uint<1> row_pingpong_bit1 = row_idx[INBUFFER_HEIGHT_BITWIDTH];
	ap_uint<2> row_pingpong_bit2 = row_idx.range(INBUFFER_HEIGHT_BITWIDTH+1, INBUFFER_HEIGHT_BITWIDTH);


	ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid=0;
	ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_offset=0;


	#if DEBUG_FILE_PRINT
		if(row_address_bitnumber_flag)
		{
			if( inwidth_ceildiv_inbufferwidth*indepth_align8 > (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 1)) )
			{
				printf("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n", (int) inwidth_ceildiv_inbufferwidth, (int) indepth_align8, (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 1)) );
				exit(-1);
			}
		}
		else
		{
			if( inwidth_ceildiv_inbufferwidth*indepth_align8 > (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 2)) )
			{
				printf("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n", (int) inwidth_ceildiv_inbufferwidth, (int) indepth_align8, (1<<(INPUT_BUFFER_DEPTH_BITWIDTH - 2)) );
				exit(-1);
			}
		}
		
	#endif



	ap_uint<INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mini_tile=0;

	ap_uint<8> read_inwidth_counter=1;
	ap_uint<8> write_inwidth_counter=1;

	ap_uint<32> ddr_address_offset = row_idx * group_indepth_x_inwidth_align8_by8 + group_indepth_offset_x_inwidth_align8_by8;

	ap_uint<128>* DDR_offset = DDR_port + ddr_address_offset;
	
	ap_uint<1> left_down_flag=1;
	
	ap_uint<INBUFFER_WIDTH_BITWIDTH-3> bank_split_idx=0;

	for(ap_uint<16> cycle=0;cycle<input_load_burst_length;cycle++)
	{
		#pragma HLS pipeline
		ap_uint<128> DDR_data;

		DDR_data = DDR_offset[cycle];
	
		ap_uint<16> DDR_data_divided[8];
		#pragma HLS array_partition variable = DDR_data_divided complete

		for(int i=0;i<8;i++)
		{
		#pragma HLS unroll
			DDR_data_divided[i]=DDR_data.range(i*16+15,i*16);
		}

		if(read_inwidth_counter == inwidth_align8)
		{
			read_inwidth_counter=1;
		}
		else
		{
			read_inwidth_counter++;
		}
		
		ap_uint<16> bank_data[8];
		#pragma HLS array_partition variable = bank_data complete

		if(left_down_flag)
		{
			for(int i=0;i<8;i++)
			{
			#pragma HLS unroll
				bank_data[i]=register_matrix[i][0];
			}
			for(int i=0;i<7;i++)
			{
			#pragma HLS unroll
				for(int j=0;j<8;j++)
				{
				#pragma HLS unroll
					register_matrix[j][i]=register_matrix[j][i+1];
				}
			}
			for(int i=0;i<8;i++)
			{ 
			#pragma HLS unroll
				register_matrix[i][7]=DDR_data_divided[i];
			}
		}
		else
		{
			for(int i=0;i<8;i++)
			{
			#pragma HLS unroll
				bank_data[i]=register_matrix[0][i];
			}

			for(int i=0;i<7;i++)
			{
			#pragma HLS unroll
				for(int j=0;j<8;j++)
				{
				#pragma HLS unroll
					register_matrix[i][j]=register_matrix[i+1][j];
				}
			}
			for(int i=0;i<8;i++)
			{ 
			#pragma HLS unroll
				register_matrix[7][i]=DDR_data_divided[i];
			}
		}
		if(cycle%8==7)
			left_down_flag=~left_down_flag;


		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH> buffer_addr; 
		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit1_offset;
		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 2 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit2_offset;
		
		buffer_address_mid_rowbit1_offset=buffer_address_mid_offset;
		buffer_address_mid_rowbit2_offset=buffer_address_mid_offset;

		if(row_address_bitnumber_flag)
		{
			buffer_addr=(row_pingpong_bit1,buffer_address_mid_rowbit1_offset,buffer_address_mini_tile);
		}
		else
		{
			buffer_addr=(row_pingpong_bit2,buffer_address_mid_rowbit2_offset,buffer_address_mini_tile);
		}


		if(cycle>=8)
		{
			#if INBUFFER_HEIGHT == 8
			if(bufferflag)
			{
			#endif
				input_buffer[bank_split_idx*8+0][buffer_addr]=bank_data[0];
				input_buffer[bank_split_idx*8+1][buffer_addr]=bank_data[1];
				input_buffer[bank_split_idx*8+2][buffer_addr]=bank_data[2];
				input_buffer[bank_split_idx*8+3][buffer_addr]=bank_data[3];
				input_buffer[bank_split_idx*8+4][buffer_addr]=bank_data[4];
				input_buffer[bank_split_idx*8+5][buffer_addr]=bank_data[5];
				input_buffer[bank_split_idx*8+6][buffer_addr]=bank_data[6];
				input_buffer[bank_split_idx*8+7][buffer_addr]=bank_data[7];

			#if INBUFFER_HEIGHT == 8
			}
			else
			{
				input_buffer1[bank_split_idx*8+0][buffer_addr]=bank_data[0];
				input_buffer1[bank_split_idx*8+1][buffer_addr]=bank_data[1];
				input_buffer1[bank_split_idx*8+2][buffer_addr]=bank_data[2];
				input_buffer1[bank_split_idx*8+3][buffer_addr]=bank_data[3];
				input_buffer1[bank_split_idx*8+4][buffer_addr]=bank_data[4];
				input_buffer1[bank_split_idx*8+5][buffer_addr]=bank_data[5];
				input_buffer1[bank_split_idx*8+6][buffer_addr]=bank_data[6];
				input_buffer1[bank_split_idx*8+7][buffer_addr]=bank_data[7];
			}
			#endif

			if(write_inwidth_counter == inwidth_align8)
			{
				buffer_address_mid += buffer_address_mid_increment_step;
			}
			else if( write_inwidth_counter.range(INBUFFER_WIDTH_BITWIDTH-1,0) == 0 )
			{
				buffer_address_mid++;
			}

			if( write_inwidth_counter.range(2,0) == 0)
			{
				buffer_address_mid_offset = buffer_address_mid;
			}
			else if(buffer_address_mini_tile == INDEPTH_MINITILE_SIZE-1)
			{
				buffer_address_mid_offset += inwidth_ceildiv_inbufferwidth;
			}

			buffer_address_mini_tile++;
			#if INBUFFER_WIDTH_BITWIDTH>3
			if(write_inwidth_counter == inwidth_align8)
			{
				bank_split_idx=0;
			}
			else if(write_inwidth_counter.range(2,0)==0)
			{
				bank_split_idx++;
			}
			#endif
				
			if(write_inwidth_counter == inwidth_align8)
			{
				write_inwidth_counter=1;
			}
			else
			{
				write_inwidth_counter++;
			}
		}

	}

	for(ap_uint<16> cycle=0;cycle<8;cycle++)
	{
		#pragma HLS pipeline

		ap_uint<16> bank_data[8];
		#pragma HLS array_partition variable = bank_data complete

		if(left_down_flag)
		{
			for(int i=0;i<8;i++)
			{
			#pragma HLS unroll
				bank_data[i]=register_matrix[i][0];
			}
			for(int i=0;i<7;i++)
			{
			#pragma HLS unroll
				for(int j=0;j<8;j++)
				{
				#pragma HLS unroll
					register_matrix[j][i]=register_matrix[j][i+1];
				}
			}
		}
		else
		{
			for(int i=0;i<8;i++)
			{
			#pragma HLS unroll
				bank_data[i]=register_matrix[0][i];
			}

			for(int i=0;i<7;i++)
			{
			#pragma HLS unroll
				for(int j=0;j<8;j++)
				{
				#pragma HLS unroll
					register_matrix[i][j]=register_matrix[i+1][j];
				}
			}
		}
			
		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH> buffer_addr; 
		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 1 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit1_offset;
		ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH - 2 - INDEPTH_MINITILE_SIZE_BITWIDTH> buffer_address_mid_rowbit2_offset;
		
		buffer_address_mid_rowbit1_offset=buffer_address_mid_offset;
		buffer_address_mid_rowbit2_offset=buffer_address_mid_offset;

		if(row_address_bitnumber_flag)
		{
			buffer_addr=(row_pingpong_bit1,buffer_address_mid_rowbit1_offset,buffer_address_mini_tile);
		}
		else
		{
			buffer_addr=(row_pingpong_bit2,buffer_address_mid_rowbit2_offset,buffer_address_mini_tile);
		}
			#if INBUFFER_HEIGHT == 8
			if(bufferflag)
			{
			#endif
				input_buffer[bank_split_idx*8+0][buffer_addr]=bank_data[0];
				input_buffer[bank_split_idx*8+1][buffer_addr]=bank_data[1];
				input_buffer[bank_split_idx*8+2][buffer_addr]=bank_data[2];
				input_buffer[bank_split_idx*8+3][buffer_addr]=bank_data[3];
				input_buffer[bank_split_idx*8+4][buffer_addr]=bank_data[4];
				input_buffer[bank_split_idx*8+5][buffer_addr]=bank_data[5];
				input_buffer[bank_split_idx*8+6][buffer_addr]=bank_data[6];
				input_buffer[bank_split_idx*8+7][buffer_addr]=bank_data[7];

			#if INBUFFER_HEIGHT == 8
			}
			else
			{
				input_buffer1[bank_split_idx*8+0][buffer_addr]=bank_data[0];
				input_buffer1[bank_split_idx*8+1][buffer_addr]=bank_data[1];
				input_buffer1[bank_split_idx*8+2][buffer_addr]=bank_data[2];
				input_buffer1[bank_split_idx*8+3][buffer_addr]=bank_data[3];
				input_buffer1[bank_split_idx*8+4][buffer_addr]=bank_data[4];
				input_buffer1[bank_split_idx*8+5][buffer_addr]=bank_data[5];
				input_buffer1[bank_split_idx*8+6][buffer_addr]=bank_data[6];
				input_buffer1[bank_split_idx*8+7][buffer_addr]=bank_data[7];
			}
			#endif
		

 		if(buffer_address_mini_tile == INDEPTH_MINITILE_SIZE-1)
		{
			buffer_address_mid_offset += inwidth_ceildiv_inbufferwidth;
		}
		buffer_address_mini_tile++;
	}
}







//template<int DUMMY>
void load_input_rowtile_from_ddr(
		INPUT_PORTS_DECLARE(DDR_port),
		// ap_uint<128>* DDR_port0,
        // ap_uint<128>* DDR_port1,
        // ap_uint<128>* DDR_port2,
        // ap_uint<128>* DDR_port3,
		ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
		ap_uint<16> inheight,
		ap_uint<16> inwidth,
		ap_uint<16> stride,
        ap_uint<16> pad_size,
		ap_uint<16> inwidth_align8,
		ap_uint<16> indepth_align8,
		ap_uint<16> group_indepth_x_inwidth_align8_by8,
		ap_uint<16> group_indepth_offset_x_inwidth_align8_by8,
		ap_uint<16> inwidth_ceildiv_inbufferwidth,
		ap_uint<16> buffer_address_mid_increment_step,
		ap_uint<16> input_load_burst_length,
		ap_uint<1>  row_address_bitnumber_flag,
		ap_uint<16> out_rowstep,
		ap_uint<16> new_start_input_row,
		ap_int<16> required_loaded_input_row_number,
        ap_uint<1> clear_flag,
		ap_uint<1> skip_flag
)
{
	if(skip_flag) return;
	printf("****load_input_rowtile_from_ddr****\n");
	#pragma HLS interface m_axi port = DDR_port0
	#pragma HLS interface m_axi port = DDR_port1
	#pragma HLS interface m_axi port = DDR_port2
	#pragma HLS interface m_axi port = DDR_port3



	#pragma HLS array_partition variable =input_buffer dim=1 complete
	#pragma HLS array_partition variable =input_buffer dim=2 complete
    static ap_uint<16> loaded_input_row_number;
    if(clear_flag)
        loaded_input_row_number = new_start_input_row;
    std::cout<<"clear flag "<<clear_flag<<std::endl;
    std::cout<<"loaded_input_row_number "<<loaded_input_row_number<<std::endl;
    std::cout<<"required_loaded_input_row_number "<<required_loaded_input_row_number<<std::endl;
    // ap_int<16> required_loaded_input_row_number = (next_start_output_row+out_rowstep)*stride + pad_size;

    while( required_loaded_input_row_number > loaded_input_row_number)
    {
		ap_uint<16> row_idx[4];
		#pragma HLS array_partition variable=row_idx complete
		ap_uint<2> row_idx_sel[4];
		#pragma HLS array_partition variable=row_idx_sel complete

		ap_uint<16> row_idx_selected[4];
		#pragma HLS array_partition variable=row_idx complete

		for(int i=0;i<4;i++)
		{
			#pragma HLS unroll 
			row_idx[i]=loaded_input_row_number+i;
			row_idx_sel[i]=i-loaded_input_row_number.range(1,0);
		}

		for(int i=0;i<4;i++)
		{
			#pragma HLS unroll 
			row_idx_selected[i]=row_idx[row_idx_sel[i]];
		}

		printf("\trow_idx[%d %d %d %d]\n",(int) row_idx_selected[0],(int) row_idx_selected[1],(int) row_idx_selected[2],(int) row_idx_selected[3] );

		#if INBUFFER_HEIGHT == 8
			// if( row_idx_selected[0]%INBUFFER_HEIGHT/4 == 0)
			// {

				load_input_row_from_ddr<0>(
					DDR_port0,
					input_buffer[0],
					input_buffer[4],
					inheight,
					inwidth,
					inwidth_align8,
					indepth_align8,
					group_indepth_x_inwidth_align8_by8,
					group_indepth_offset_x_inwidth_align8_by8,
					inwidth_ceildiv_inbufferwidth,
					buffer_address_mid_increment_step,
					input_load_burst_length,
					row_address_bitnumber_flag,
					row_idx_selected[0],
					required_loaded_input_row_number
				);
			// }
			// else
			// {
			// 	load_input_row_from_ddr<0>(
			// 		DDR_port0,
			// 		input_buffer[4],
			// 		inheight,
			// 		inwidth,
			// 		inwidth_align8,
			// 		indepth_align8,
			// 		group_indepth_x_inwidth_align8_by8,
			// 		group_indepth_offset_x_inwidth_align8_by8,
			// 		inwidth_ceildiv_inbufferwidth,
			// 		buffer_address_mid_increment_step,
			// 		input_load_burst_length,
			// 		row_address_bitnumber_flag,
			// 		row_idx_selected[0],
			// 		required_loaded_input_row_number
			// 	);
			// }

			// if( row_idx_selected[1]%INBUFFER_HEIGHT/4 == 0)
			// {

				load_input_row_from_ddr<1>(
					DDR_port1,
					input_buffer[1],
					input_buffer[5],
					inheight,
					inwidth,
					inwidth_align8,
					indepth_align8,
					group_indepth_x_inwidth_align8_by8,
					group_indepth_offset_x_inwidth_align8_by8,
					inwidth_ceildiv_inbufferwidth,
					buffer_address_mid_increment_step,
					input_load_burst_length,
					row_address_bitnumber_flag,
					row_idx_selected[1],
					required_loaded_input_row_number
				);
			// }
			// else
			// {
			// 	load_input_row_from_ddr<1>(
			// 		DDR_port1,
			// 		input_buffer[5],
			// 		inheight,
			// 		inwidth,
			// 		inwidth_align8,
			// 		indepth_align8,
			// 		group_indepth_x_inwidth_align8_by8,
			// 		group_indepth_offset_x_inwidth_align8_by8,
			// 		inwidth_ceildiv_inbufferwidth,
			// 		buffer_address_mid_increment_step,
			// 		input_load_burst_length,
			// 		row_address_bitnumber_flag,
			// 		row_idx_selected[1],
			// 		required_loaded_input_row_number
			// 	);
			// }

			// if( row_idx_selected[2]%INBUFFER_HEIGHT/4 == 0)
			// {

				load_input_row_from_ddr<2>(
					DDR_port2,
					input_buffer[2],
					input_buffer[6],
					inheight,
					inwidth,
					inwidth_align8,
					indepth_align8,
					group_indepth_x_inwidth_align8_by8,
					group_indepth_offset_x_inwidth_align8_by8,
					inwidth_ceildiv_inbufferwidth,
					buffer_address_mid_increment_step,
					input_load_burst_length,
					row_address_bitnumber_flag,
					row_idx_selected[2],
					required_loaded_input_row_number
				);
			// }
			// else
			// {
			// 	load_input_row_from_ddr<2>(
			// 		DDR_port2,
			// 		input_buffer[6],
			// 		inheight,
			// 		inwidth,
			// 		inwidth_align8,
			// 		indepth_align8,
			// 		group_indepth_x_inwidth_align8_by8,
			// 		group_indepth_offset_x_inwidth_align8_by8,
			// 		inwidth_ceildiv_inbufferwidth,
			// 		buffer_address_mid_increment_step,
			// 		input_load_burst_length,
			// 		row_address_bitnumber_flag,
			// 		row_idx_selected[2],
			// 		required_loaded_input_row_number
			// 	);
			// }


			// if( row_idx_selected[3]%INBUFFER_HEIGHT/4 == 0)
			// {

				load_input_row_from_ddr<3>(
					DDR_port3,
					input_buffer[3],
					input_buffer[7],
					inheight,
					inwidth,
					inwidth_align8,
					indepth_align8,
					group_indepth_x_inwidth_align8_by8,
					group_indepth_offset_x_inwidth_align8_by8,
					inwidth_ceildiv_inbufferwidth,
					buffer_address_mid_increment_step,
					input_load_burst_length,
					row_address_bitnumber_flag,
					row_idx_selected[3],
					required_loaded_input_row_number
				);
			// }
			// else
			// {
			// 	load_input_row_from_ddr<3>(
			// 		DDR_port3,
			// 		input_buffer[7],
			// 		inheight,
			// 		inwidth,
			// 		inwidth_align8,
			// 		indepth_align8,
			// 		group_indepth_x_inwidth_align8_by8,
			// 		group_indepth_offset_x_inwidth_align8_by8,
			// 		inwidth_ceildiv_inbufferwidth,
			// 		buffer_address_mid_increment_step,
			// 		input_load_burst_length,
			// 		row_address_bitnumber_flag,
			// 		row_idx_selected[3],
			// 		required_loaded_input_row_number
			// 	);

			// }
		#elif INBUFFER_HEIGHT==4
			load_input_row_from_ddr<0>(
			DDR_port0,
			input_buffer[0],
			inheight,
			inwidth,
			inwidth_align8,
			indepth_align8,
			group_indepth_x_inwidth_align8_by8,
			group_indepth_offset_x_inwidth_align8_by8,
			inwidth_ceildiv_inbufferwidth,
			buffer_address_mid_increment_step,
			input_load_burst_length,
			row_address_bitnumber_flag,
			row_idx_selected[0],
			required_loaded_input_row_number
			);
			load_input_row_from_ddr<1>(
			DDR_port1,
			input_buffer[1],
			inheight,
			inwidth,
			inwidth_align8,
			indepth_align8,
			group_indepth_x_inwidth_align8_by8,
			group_indepth_offset_x_inwidth_align8_by8,
			inwidth_ceildiv_inbufferwidth,
			buffer_address_mid_increment_step,
			input_load_burst_length,
			row_address_bitnumber_flag,
			row_idx_selected[1],
			required_loaded_input_row_number
			);

			load_input_row_from_ddr<2>(
			DDR_port2,
			input_buffer[2],
			inheight,
			inwidth,
			inwidth_align8,
			indepth_align8,
			group_indepth_x_inwidth_align8_by8,
			group_indepth_offset_x_inwidth_align8_by8,
			inwidth_ceildiv_inbufferwidth,
			buffer_address_mid_increment_step,
			input_load_burst_length,
			row_address_bitnumber_flag,
			row_idx_selected[2],
			required_loaded_input_row_number
			);

			load_input_row_from_ddr<3>(
			DDR_port3,
			input_buffer[3],
			inheight,
			inwidth,
			inwidth_align8,
			indepth_align8,
			group_indepth_x_inwidth_align8_by8,
			group_indepth_offset_x_inwidth_align8_by8,
			inwidth_ceildiv_inbufferwidth,
			buffer_address_mid_increment_step,
			input_load_burst_length,
			row_address_bitnumber_flag,
			row_idx_selected[3],
			required_loaded_input_row_number
			);
		#endif


        loaded_input_row_number+=4;

    }

	loaded_input_row_number=required_loaded_input_row_number;
	#if DEBUG_FILE_PRINT
	fflush(stdout);
	#endif
}




template<int port_idx>
void write_output_row(
	ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM>* out_DDR,
	ap_uint<OUT_WIDTH*2> out_buffer0[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> rowtile_baseaddr0,
	ap_uint<16> row_idx,
	// ap_int<16> bias_buffer[8][BIAS_BUFFER_DEPTH],
	ConvDesc_t conv_desc
)
{
	if(port_idx%2==1 && conv_desc.stride!=1) return;
	if(port_idx>= conv_desc.wino_output_tile_size) return;
	#pragma HLS array_partition variable=out_buffer0 complete dim=1
	#pragma HLS array_partition variable=out_buffer0 complete dim=2
	#pragma HLS array_partition variable=out_buffer0 complete dim=3

	if(row_idx >= conv_desc.outheight)
	{
		return;
	}

	ap_int<16> scale_oback=conv_desc.scale_oback_int;
	ap_uint<1> relu_flag=conv_desc.relu_flag;


	ap_uint<16> row_address=rowtile_baseaddr0;
	ap_uint<16> col_address=0;

	ap_uint<8> col=1;
	

	ap_uint<3> wino_cell_inneridx=0;
	ap_uint<6> wino_width_idx=0;

	ap_uint<1> o4=0;
	ap_uint<8> o8=0;


	int out_address=0;

	ap_uint<2> stride=conv_desc.stride;
	for(int out_address=0;out_address<conv_desc.output_burst_length;out_address++)
	// for(int o8=0;o8<conv_desc.outdepth_align8;o8+=8)
	// for(int col=0;col<conv_desc.outwidth_align8;col++)
	// for(int o4=0;o4<8;o4+=4)
	{	
		#pragma HLS pipeline
		ap_uint<OUT_WIDTH*BATCH_SIZE> outbuffer_data_lo[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE];
		ap_uint<OUT_WIDTH*BATCH_SIZE> outbuffer_data_hi[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE];

		#pragma HLS array_partition variable=outbuffer_data_lo complete dim=1 
		#pragma HLS array_partition variable=outbuffer_data_lo complete dim=2 

		#pragma HLS array_partition variable=outbuffer_data_hi complete dim=1 
		#pragma HLS array_partition variable=outbuffer_data_hi complete dim=2 


		int depth_address_lo=(o8+o4)*2*conv_desc.outbuffer_omini_increment_step;
		int depth_address_hi=((o8+o4)*2+1)*conv_desc.outbuffer_omini_increment_step;
		// int row_address=rowtile_baseaddr0;
		// int col_address=col*conv_desc.stride/conv_desc.wino_out_size_by_wino_width;
		// int wino_width_idx=col*conv_desc.stride% ( conv_desc.wino_out_size_by_wino_width) /conv_desc.wino_output_tile_size;
		// int wino_cell_inneridx=col*conv_desc.stride%conv_desc.wino_output_tile_size;

		int buffer_address_lo=depth_address_lo+row_address+col_address;
		int buffer_address_hi=depth_address_hi+row_address+col_address;

		// std::cout<<"col_address "<<col_address<<std::endl;
		// std::cout<<"wino_width_idx "<<wino_width_idx<<std::endl;
		// std::cout<<"wino_cell_inneridx "<<wino_cell_inneridx<<std::endl;
		// std::cout<<"conv_desc.wino_out_size_by_wino_width "<<conv_desc.wino_out_size_by_wino_width<<std::endl;
		// std::cout<<"buffer_address_lo "<<buffer_address_lo<<std::endl;
		// std::cout<<"buffer_address_hi "<<buffer_address_hi<<std::endl;

	
		for(int i=0;i<WINO_OUT_SIZE_CELL ;i++)
		for(int j=0;j<OUTDEPTH_MINITILE_SIZE ;j++)
		{
			#pragma HLS unroll
			outbuffer_data_lo[i][j]=out_buffer0[i][j][wino_width_idx][buffer_address_lo];
			outbuffer_data_hi[i][j]=out_buffer0[i][j][wino_width_idx][buffer_address_hi];
		}

		
		ap_int<OUT_WIDTH> outdata_vect[4][2];

		#pragma HLS array_partition variable=outdata_vect complete dim=1 
		#pragma HLS array_partition variable=outdata_vect complete dim=2 


		for(int i=0;i<2 ;i++)
		{
			#pragma HLS unroll
			(outdata_vect[i][1],outdata_vect[i][0])=outbuffer_data_lo[wino_cell_inneridx][i];
			(outdata_vect[2+i][1],outdata_vect[2+i][0])=outbuffer_data_hi[wino_cell_inneridx][i];
		}

		// for(int j=0;j<4 ;j++)
		// {
		// 	printf("%d ",(int) outdata_vect[j][0]);
		// }
		// getchar();

		// for(int j=0;j<4 ;j++)
		// {
		// 	printf("%d ",(int) outdata_vect[j][1]);
		// }
		// getchar();

		ap_int<OUT_WIDTH> outmem_data_scale[4][2];

		#pragma HLS array_partition variable=outmem_data_scale complete dim=1 
		#pragma HLS array_partition variable=outmem_data_scale complete dim=2 

		for(int i=0;i<OUT_PORT_BATCH_NUM;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				outmem_data_scale[i][b]=(( ((int)outdata_vect[i][b])*(int)scale_oback)>>OBACK_QUANT_BIT);//+bias_vect[i];		
			}
		}
		ap_int<OUT_WIDTH> outmem_data_sat[4][2];
		
		#pragma HLS array_partition variable =outmem_data_sat complete dim=2
		#pragma HLS array_partition variable =outmem_data_sat complete dim=1

		for(int i=0;i<4;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				ap_int<19-ODDR_WIDTH> judge_bits0=outmem_data_scale[i][b].range(17,ODDR_WIDTH-1);

				if(judge_bits0==0 || judge_bits0==-1)
				{
					outmem_data_sat[i][b]=outmem_data_scale[i][b];
				}
				else if(judge_bits0<0)
				{
					outmem_data_sat[i][b]=-1<<(ODDR_WIDTH-1);
				}
				else
				{
					outmem_data_sat[i][b]=(1<<(ODDR_WIDTH-1))-1;
				}				
			}
		}

		ap_int<ODDR_WIDTH>  outmem_data[4][BATCH_SIZE];
		

		#pragma HLS array_partition variable =outmem_data complete dim=2
		#pragma HLS array_partition variable =outmem_data complete dim=1

	

		for(int i=0;i<OUT_PORT_BATCH_NUM;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				if(relu_flag && outmem_data_sat[i][b]<0)
					outmem_data[i][b]=0;
				else
					outmem_data[i][b]=outmem_data_sat[i][b];
			}
		}

		ap_int<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> write_back_value;

		for(int i=0;i<OUT_PORT_BATCH_NUM;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{
				#pragma HLS unroll
				write_back_value.range( (i*BATCH_SIZE+b)*ODDR_WIDTH+ODDR_WIDTH-1, (i*BATCH_SIZE+b)*ODDR_WIDTH )=outmem_data[i][b];
			}
		}
		out_DDR[out_address]=write_back_value;
	



		if(o4==1)
		{
			if (wino_cell_inneridx+stride == conv_desc.wino_output_tile_size)
			{
				if(wino_width_idx+1==WINO_WIDTH )
				{
					col_address++;
					wino_width_idx=0;
				}
				else
				{
					wino_width_idx++;
				}
				wino_cell_inneridx=0;
			}
			else
			{
				wino_cell_inneridx+=stride;
			}




			if(col==conv_desc.outwidth_align8)
			{
				col=1;
				wino_width_idx=0;
				col_address=0;
				wino_cell_inneridx=0;
				o8+=2;
			}
			else
			{
				col++;
			}
		}		

		o4++;
	}




		




}



template<int port_idx>
void write_output_to_DDR_onerow(
	ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR,
	ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
	ap_uint<16> outheight,
	ap_uint<16> outwidth_align8,
	ap_uint<4> wino_output_tile_size,
	ap_uint<4> wino_col_pix_upper_bound,
	ap_uint<8> wino_tile_number_rowcol,
	ap_uint<24> output_burst_length,
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> rowtile_baseaddr,
	ap_uint<16> start_row_idx,
	ap_int<18> oback_scale,
	ap_int<16> bias_buffer[8][BIAS_BUFFER_DEPTH]
	#if DEBUG_CONV_DESC
	,ConvDesc_t conv_desc
	#endif
)
{
	printf("\t^^write_output_to_DDR_onerow %d^^\n",(int) start_row_idx);
	#pragma HLS array_partition variable=out_buffer complete dim=1
	#pragma HLS array_partition variable=out_buffer complete dim=2
	#pragma HLS array_partition variable=out_buffer complete dim=3
	#if WINO_OUT_SIZE == 4
	
	if(port_idx >= wino_output_tile_size || start_row_idx >= outheight)
	{
		return;
	}
	#else

	if(start_row_idx >= outheight)
	{
		return;
	}
	#endif
	ap_uint<WINO_WIDTH_BITWIDTH> wino_widthtile_idx=0;
	
	ap_uint<4> wino_col_pix_idx=0;



	ap_uint<2> depthtile_idx=0;

	ap_uint<16> col_idx_cnt=1;

	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> coltile_baseaddr=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> depthtile_baseaddr0=0;
	ap_uint<10> bias_buffer_addr=0;
	#if WEIGHT_FEED_NUMBER_PER_PORT == 1
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> depthtile_baseaddr1=wino_tile_number_rowcol;
	#endif



	#if WEIGHT_FEED_NUMBER_PER_PORT == 1
	ap_uint<8> depthtile_baseaddr_increment_step=wino_tile_number_rowcol*2;
	#else
	ap_uint<8> depthtile_baseaddr_increment_step=wino_tile_number_rowcol;
	#endif


	// printf("write_length %d\n", (int) conv_desc.output_burst_length);
	for(ap_uint<24> cycle=0;cycle < output_burst_length;cycle++)
	{		
		#pragma HLS pipeline
		ap_uint<OUT_WIDTH*BATCH_SIZE> outbuffer_data_depth0[OUTDEPTH_MINITILE_SIZE];
		#pragma HLS array_partition variable =outbuffer_data_depth0 complete

		#if WEIGHT_FEED_NUMBER_PER_PORT == 1
		ap_int<OUT_WIDTH*BATCH_SIZE> outbuffer_data_depth1[OUTDEPTH_MINITILE_SIZE];
		#pragma HLS array_partition variable =outbuffer_data_depth1 complete
		#endif

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> buffer_address0;
		buffer_address0=depthtile_baseaddr0+rowtile_baseaddr+coltile_baseaddr;

		#if WEIGHT_FEED_NUMBER_PER_PORT == 1
			ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> buffer_address1;
			buffer_address1=depthtile_baseaddr1+rowtile_baseaddr+coltile_baseaddr;
		#endif

		// printf("outbuffer add %d %d %d %d\n",(int) coltile_baseaddr,(int)wino_widthtile_idx,(int) wino_col_pix_idx, (int) wino_col_pix_upper_bound );

		for(int b=0;b<BATCH_SIZE;b++)
		{
			#pragma HLS unroll
			for(int od=0;od<OUTDEPTH_MINITILE_SIZE;od++)
			{
				#pragma HLS unroll
				outbuffer_data_depth0[od]=out_buffer[wino_col_pix_idx][od][wino_widthtile_idx][buffer_address0];
				
				#if WEIGHT_FEED_NUMBER_PER_PORT == 1
				outbuffer_data_depth1[od]=out_buffer[wino_col_pix_idx][od][wino_widthtile_idx][buffer_address1];
				#endif
			}
		}
		// printf("Addr:%7d", (int) buffer_address0);
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d %6d]",(int) outbuffer_data_depth0[i].range(17,0),(int) outbuffer_data_depth0[i].range(35,18) );
		// }
		// printf("\n");
		// getchar();
		ap_int<ODDR_WIDTH>  outmem_data[8][BATCH_SIZE];
		#pragma HLS array_partition variable =outmem_data complete dim=2
		#pragma HLS array_partition variable =outmem_data complete dim=1
		#if WEIGHT_FEED_NUMBER_PER_PORT ==1
		
			for(int i=0;i<4;i++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{	
					#pragma HLS unroll
					outmem_data[i][b]= (outbuffer_data_depth0[i].range(b*OUT_WIDTH+OUT_WIDTH-1,b*OUT_WIDTH))>>(OUT_WIDTH- ODDR_WIDTH);
					outmem_data[i+4][b]= (outbuffer_data_depth1[i].range(b*OUT_WIDTH+OUT_WIDTH-1,b*OUT_WIDTH))>>(OUT_WIDTH- ODDR_WIDTH);
				}
			}
		#else
			for(int i=0;i<8;i++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{	
					#pragma HLS unroll

					ap_int<OUT_WIDTH> temp= outbuffer_data_depth0[i].range(b*OUT_WIDTH+OUT_WIDTH-1,b*OUT_WIDTH);
					ap_int<18> temp2= (temp*oback_scale)>>OBACK_QUANT_BIT;
					ap_int<16> bias=bias_buffer[i][bias_buffer_addr];
					ap_int<18> temp3= temp2+bias;
					if(temp3 < 0 ) outmem_data[i][b]=0;
					else if(temp3 > (1<<(IN_WIDTH-1))-1 ) outmem_data[i][b]=(1<<(IN_WIDTH-1))-1;
					else outmem_data[i][b]=temp3;
				}
			}
		#endif


		ap_int<ODDR_WIDTH*8*BATCH_SIZE> write_back_value;

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{
				#pragma HLS unroll
				write_back_value.range( (i*BATCH_SIZE+b)*ODDR_WIDTH+ODDR_WIDTH-1, (i*BATCH_SIZE+b)*ODDR_WIDTH )=outmem_data[i][b];
			}
		}


		// printf("Addr:%7d ", (int) cycle);
		// for(int i=0;i<16;i++)
		// {

		// 	printf("%3d ",(int) write_back_value.range(i*8+7,i*8));
		// }
		// printf("\n");
		// getchar();

		out_DDR[cycle]=write_back_value;

		if(col_idx_cnt==outwidth_align8 )
		{
			depthtile_baseaddr0+=depthtile_baseaddr_increment_step;
			bias_buffer_addr++;
			#if WEIGHT_FEED_NUMBER_PER_PORT == 1
			depthtile_baseaddr1+=depthtile_baseaddr_increment_step;
			#endif
		}




		if(col_idx_cnt==outwidth_align8 )
		{
			coltile_baseaddr=0;
		}
		else if( wino_col_pix_idx == wino_col_pix_upper_bound  && wino_widthtile_idx == WINO_WIDTH-1  )
		{
			coltile_baseaddr++;
		}


		if( (wino_col_pix_idx == wino_col_pix_upper_bound  && wino_widthtile_idx == WINO_WIDTH-1) || col_idx_cnt==outwidth_align8)
		{
			wino_widthtile_idx=0;
		}
		else if( wino_col_pix_idx == wino_col_pix_upper_bound )
		{
			wino_widthtile_idx++;
		}


		if(wino_col_pix_idx == wino_col_pix_upper_bound || col_idx_cnt==outwidth_align8)
		{
			wino_col_pix_idx=0;
		}
		else
		{
			wino_col_pix_idx++;
		}

		if(col_idx_cnt==outwidth_align8)
		{
			col_idx_cnt=1;
		}
		else
		{
			col_idx_cnt++;
		}
	}
}




template<int port_idx>
void write_output_to_DDR_two_row_two_port(
	ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR_row0,
	ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR_row1,
	ap_uint<OUT_WIDTH*2> out_buffer0[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUT_WIDTH*2> out_buffer1[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> rowtile_baseaddr0,
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> rowtile_baseaddr1,
	ap_uint<16> start_row_idx,
	ap_int<16> bias_buffer[8][BIAS_BUFFER_DEPTH],
	ConvDesc_t conv_desc
)
{

	if(port_idx==1 && conv_desc.stride!=1) return;
	printf("\t^^write_output_to_DDR_onerow %d^^\n",(int) start_row_idx);
	#pragma HLS array_partition variable=out_buffer0 complete dim=1
	#pragma HLS array_partition variable=out_buffer0 complete dim=2
	#pragma HLS array_partition variable=out_buffer0 complete dim=3

	#pragma HLS array_partition variable=out_buffer1 complete dim=1
	#pragma HLS array_partition variable=out_buffer1 complete dim=2
	#pragma HLS array_partition variable=out_buffer1 complete dim=3

	if(start_row_idx >= conv_desc.outheight)
	{
		return;
	}
	ap_int<16> scale_oback=conv_desc.scale_oback_int;
	ap_uint<1> relu_flag=conv_desc.relu_flag;

	// std::cout<<"relu_flag "<<relu_flag<<std::endl;
	// getchar();	
	ap_uint<WINO_WIDTH_BITWIDTH> wino_widthtile_idx=0;
	
	ap_uint<4> wino_col_pix_idx=0;



	ap_uint<2> depthtile_idx=0;

	ap_uint<16> col_idx_cnt=conv_desc.stride;

	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> coltile_baseaddr=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> depthtile_baseaddr0=0;
	ap_uint<10> bias_buffer_addr=0;

	#if WEIGHT_FEED_NUMBER_PER_PORT == 1
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> depthtile_baseaddr1=wino_tile_number_rowcol;
	#endif



	#if WEIGHT_FEED_NUMBER_PER_PORT == 1
	ap_uint<8> depthtile_baseaddr_increment_step=wino_tile_number_rowcol*2;
	#else
	ap_uint<8> depthtile_baseaddr_increment_step=conv_desc.wino_tile_number_rowcol;
	#endif


	// printf("write_length %d\n", (int) conv_desc.output_burst_length);
	for(ap_uint<24> cycle=0;cycle < conv_desc.output_burst_length;cycle++)
	{		
		#pragma HLS pipeline
		ap_uint<OUT_WIDTH*BATCH_SIZE> outbuffer_data_row0[OUTDEPTH_MINITILE_SIZE];
		#pragma HLS array_partition variable =outbuffer_data_row0 complete

		ap_uint<OUT_WIDTH*BATCH_SIZE> outbuffer_data_row1[OUTDEPTH_MINITILE_SIZE];
		#pragma HLS array_partition variable =outbuffer_data_row1 complete

		#if WEIGHT_FEED_NUMBER_PER_PORT == 1
		ap_int<OUT_WIDTH*BATCH_SIZE> outbuffer_data_depth1[OUTDEPTH_MINITILE_SIZE];
		#pragma HLS array_partition variable =outbuffer_data_depth1 complete
		#endif

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> buffer_address0,buffer_address1;
		buffer_address0=depthtile_baseaddr0+rowtile_baseaddr0+coltile_baseaddr;
		buffer_address1=depthtile_baseaddr0+rowtile_baseaddr1+coltile_baseaddr;



		// std::cout<<"depthtile_baseaddr0 "<<depthtile_baseaddr0<<std::endl;
		// std::cout<<"coltile_baseaddr "<<coltile_baseaddr<<std::endl;
		// std::cout<<"buffer_address0 "<<buffer_address0<<std::endl;
		// std::cout<<"buffer_address1 "<<buffer_address1<<std::endl;
		// getchar();
		// printf("outbuffer add %d %d %d %d\n",(int) coltile_baseaddr,(int)wino_widthtile_idx,(int) wino_col_pix_idx, (int) conv_desc.wino_col_pix_upper_bound );


		for(int b=0;b<BATCH_SIZE;b++)
		{
			#pragma HLS unroll
			for(int od=0;od<OUTDEPTH_MINITILE_SIZE;od++)
			{
				#pragma HLS unroll
				outbuffer_data_row0[od]=out_buffer0[wino_col_pix_idx][od][wino_widthtile_idx][buffer_address0];

				if(conv_desc.wino3x3_flag)
				{
					outbuffer_data_row1[od]=out_buffer0[wino_col_pix_idx][od][wino_widthtile_idx][buffer_address1];
				}
				else
				{
					outbuffer_data_row1[od]=out_buffer1[wino_col_pix_idx][od][wino_widthtile_idx][buffer_address1];
				}
			
			}
		}

		// std::cout<<"wino_col_pix_idx "<<wino_col_pix_idx<<std::endl;
		// std::cout<<"wino_widthtile_idx "<<wino_widthtile_idx<<std::endl;
		// std::cout<<"buffer_address0 "<<buffer_address0<<std::endl;
		// std::cout<<"buffer_address1 "<<buffer_address1<<std::endl;
		// getchar();
		
		ap_int<OUT_WIDTH> obuffer_data_row0[8][BATCH_SIZE];
		ap_int<OUT_WIDTH> obuffer_data_row1[8][BATCH_SIZE];
	#pragma HLS array_partition variable =obuffer_data_row0 complete dim=2
	#pragma HLS array_partition variable =obuffer_data_row0 complete dim=1

	#pragma HLS array_partition variable =obuffer_data_row1 complete dim=2
	#pragma HLS array_partition variable =obuffer_data_row1 complete dim=1

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				obuffer_data_row0[i][b]=outbuffer_data_row0[i].range(b*OUT_WIDTH+OUT_WIDTH-1,b*OUT_WIDTH);
				obuffer_data_row1[i][b]=outbuffer_data_row1[i].range(b*OUT_WIDTH+OUT_WIDTH-1,b*OUT_WIDTH);
			}
		}

		ap_int<16> bias_vect[8];
		#pragma HLS array_partition variable =bias_vect complete dim=1

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			bias_vect[i]=bias_buffer[i][bias_buffer_addr];
		}


		// printf("Addr:%7d", (int) cycle);
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d %6d]",(int) obuffer_data_row0[i][0],(int) obuffer_data_row0[i][1] );
		// }
		// printf("\n");
		// printf("Addr:%7d", (int) cycle);
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d %6d]",(int) obuffer_data_row1[i][0],(int) obuffer_data_row1[i][1] );
		// }
		// printf("\n");
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d]",(int) bias_vect[i]);
		// }
		// printf("\n");
		// getchar();

		ap_int<18>  outmem_data_scale_row0[8][BATCH_SIZE];
		ap_int<18>  outmem_data_scale_row1[8][BATCH_SIZE];

		#pragma HLS array_partition variable =outmem_data_scale_row0 complete dim=2
		#pragma HLS array_partition variable =outmem_data_scale_row0 complete dim=1

		#pragma HLS array_partition variable =outmem_data_scale_row1 complete dim=2
		#pragma HLS array_partition variable =outmem_data_scale_row1 complete dim=1
		
		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				outmem_data_scale_row0[i][b]=(( ((int)obuffer_data_row0[i][b])*(int)scale_oback)>>OBACK_QUANT_BIT)+bias_vect[i];
				outmem_data_scale_row1[i][b]=(( ((int)obuffer_data_row1[i][b])*(int)scale_oback)>>OBACK_QUANT_BIT)+bias_vect[i];				
			}
		}

		// printf("Addr:%7d", (int) scale_oback);
		// printf("Addr:%7d", (int) cycle);
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d %6d]",(int) outmem_data_scale_row0[i][0],(int) outmem_data_scale_row0[i][1] );
		// }
		// printf("\n");
		// printf("Addr:%7d", (int) cycle);
		// for(int i=0;i<8;i++)
		// {
		// 	printf("[%6d %6d]",(int) outmem_data_scale_row1[i][0],(int) outmem_data_scale_row1[i][1] );
		// }
		// printf("\n");
		// getchar();
		ap_int<18>  outmem_data_sat_row0[8][BATCH_SIZE];
		ap_int<18>  outmem_data_sat_row1[8][BATCH_SIZE];

		#pragma HLS array_partition variable =outmem_data_sat_row0 complete dim=2
		#pragma HLS array_partition variable =outmem_data_sat_row0 complete dim=1

		#pragma HLS array_partition variable =outmem_data_sat_row1 complete dim=2
		#pragma HLS array_partition variable =outmem_data_sat_row1 complete dim=1

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				ap_int<19-ODDR_WIDTH> judge_bits0=outmem_data_scale_row0[i][b].range(17,ODDR_WIDTH-1);
				ap_int<19-ODDR_WIDTH> judge_bits1=outmem_data_scale_row1[i][b].range(17,ODDR_WIDTH-1);

				if(judge_bits0==0 || judge_bits0==-1)
				{
					outmem_data_sat_row0[i][b]=outmem_data_scale_row0[i][b];
				}
				else if(judge_bits0<0)
				{
					outmem_data_sat_row0[i][b]=-1<<(ODDR_WIDTH-1);
				}
				else
				{
					outmem_data_sat_row0[i][b]=(1<<(ODDR_WIDTH-1))-1;
				}


				if(judge_bits1==0 || judge_bits1==-1)
				{
					outmem_data_sat_row1[i][b]=outmem_data_scale_row1[i][b];
				}
				else if(judge_bits1<0)
				{
					outmem_data_sat_row1[i][b]=-1<<(ODDR_WIDTH-1);
				}
				else
				{
					outmem_data_sat_row1[i][b]=(1<<(ODDR_WIDTH-1))-1;
				}				
			}
		}

		ap_int<ODDR_WIDTH>  outmem_data_row0[8][BATCH_SIZE];
		ap_int<ODDR_WIDTH>  outmem_data_row1[8][BATCH_SIZE];

		#pragma HLS array_partition variable =outmem_data_row0 complete dim=2
		#pragma HLS array_partition variable =outmem_data_row0 complete dim=1

		#pragma HLS array_partition variable =outmem_data_row1 complete dim=2
		#pragma HLS array_partition variable =outmem_data_row1 complete dim=1

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{	
				#pragma HLS unroll
				if(relu_flag && outmem_data_sat_row0[i][b]<0)
					outmem_data_row0[i][b]=0;
				else
					outmem_data_row0[i][b]=outmem_data_sat_row0[i][b];

				if(relu_flag && outmem_data_sat_row1[i][b]<0)
					outmem_data_row1[i][b]=0;
				else
					outmem_data_row1[i][b]=outmem_data_sat_row1[i][b];		
			}
		}

	


		ap_int<ODDR_WIDTH*8*BATCH_SIZE> write_back_value0;
		ap_int<ODDR_WIDTH*8*BATCH_SIZE> write_back_value1;

		for(int i=0;i<8;i++)
		{
			#pragma HLS unroll
			for(int b=0;b<BATCH_SIZE;b++)
			{
				#pragma HLS unroll
				write_back_value0.range( (i*BATCH_SIZE+b)*ODDR_WIDTH+ODDR_WIDTH-1, (i*BATCH_SIZE+b)*ODDR_WIDTH )=outmem_data_row0[i][b];
				write_back_value1.range( (i*BATCH_SIZE+b)*ODDR_WIDTH+ODDR_WIDTH-1, (i*BATCH_SIZE+b)*ODDR_WIDTH )=outmem_data_row1[i][b];
			}
		}


		// printf("Addr:%7d ", (int) cycle);
		// for(int i=0;i<16;i++)
		// {

		// 	printf("%3d ",(int) write_back_value.range(i*8+7,i*8));
		// }
		// printf("\n");
		// getchar();

		out_DDR_row0[cycle]=write_back_value0;
		out_DDR_row1[cycle]=write_back_value1;

		if(col_idx_cnt==conv_desc.outwidth_align8 )
		{
			depthtile_baseaddr0+=depthtile_baseaddr_increment_step;
			bias_buffer_addr++;
			#if WEIGHT_FEED_NUMBER_PER_PORT == 1
			depthtile_baseaddr1+=depthtile_baseaddr_increment_step;
			#endif
		}




		if(col_idx_cnt==conv_desc.outwidth_align8 )
		{
			coltile_baseaddr=0;
		}
		else if( wino_col_pix_idx == conv_desc.wino_col_pix_upper_bound  && wino_widthtile_idx == WINO_WIDTH-1  )
		{
			coltile_baseaddr++;
		}


		if( (wino_col_pix_idx == conv_desc.wino_col_pix_upper_bound  && wino_widthtile_idx == WINO_WIDTH-1) || col_idx_cnt==conv_desc.outwidth_align8)
		{
			wino_widthtile_idx=0;
		}
		else if( wino_col_pix_idx == conv_desc.wino_col_pix_upper_bound )
		{
			wino_widthtile_idx++;
		}


		if(wino_col_pix_idx == conv_desc.wino_col_pix_upper_bound || col_idx_cnt==conv_desc.outwidth_align8)
		{
			wino_col_pix_idx=0;
		}
		else
		{
			wino_col_pix_idx+=conv_desc.stride;
		}

		if(col_idx_cnt==conv_desc.outwidth_align8)
		{
			col_idx_cnt=conv_desc.stride;
		}
		else
		{
			col_idx_cnt+=conv_desc.stride;
		}
	}
}










void write_output_to_DDR2(
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR0,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR1,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR2,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR3,
		ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
		ap_int<16> start_row_idx,
		ap_uint<1> first_flag,
		ap_int<16> bias_buffer0[8][BIAS_BUFFER_DEPTH],
		ap_int<16> bias_buffer1[8][BIAS_BUFFER_DEPTH],
		ConvDesc_t conv_desc
)
{


	#pragma HLS array_partition variable=out_buffer complete dim=1
	#pragma HLS array_partition variable=out_buffer complete dim=2
	#pragma HLS array_partition variable=out_buffer complete dim=3
	#pragma HLS array_partition variable=out_buffer complete dim=4

	static ap_uint<24> out_ddr_offset0;
	static ap_uint<24> out_ddr_offset1;
	static ap_uint<24> out_ddr_offset2;
	static ap_uint<24> out_ddr_offset3;
	
	
	printf("---write_output_to_DDR %d---\n",(int) start_row_idx);
	if(start_row_idx<0) return;

	ap_uint<16> outrow_idx=start_row_idx;
	if( first_flag)
	{
		out_ddr_offset0=0;
		out_ddr_offset1=conv_desc.output_burst_length;
		out_ddr_offset2= (conv_desc.stride==2)?conv_desc.output_burst_length:((conv_desc.output_burst_length)<<1);
		out_ddr_offset3=out_ddr_offset1+out_ddr_offset2;
	}

	ap_uint<16> rowtile_baseaddr_increment_step=conv_desc.wino_tile_number_in_outwidth;
	ap_uint<16> rowtile_baseaddr0=0;


	ap_uint<16> rowtile_baseaddr1;
	if(conv_desc.wino3x3_flag)
	{
		rowtile_baseaddr1=rowtile_baseaddr_increment_step;
	}
	else
	{
		rowtile_baseaddr1=0;
	}
	
	for(ap_uint<8> wino_rowtile_idx=0; wino_rowtile_idx<conv_desc.out_rowstep;wino_rowtile_idx+=4)
	{
		write_output_to_DDR_two_row_two_port<0>(
			out_DDR0+out_ddr_offset0,
			out_DDR2+out_ddr_offset2,
		 	out_buffer[0],
		 	out_buffer[2],
			rowtile_baseaddr0,
			rowtile_baseaddr1,
			outrow_idx,
			bias_buffer0,
			conv_desc);

		write_output_to_DDR_two_row_two_port<1>(
			out_DDR1+out_ddr_offset1,
			out_DDR3+out_ddr_offset3,
		 	out_buffer[1],
		 	out_buffer[3],
			rowtile_baseaddr0,
			rowtile_baseaddr1,
			outrow_idx+1,
			bias_buffer1,
			conv_desc);

		if(conv_desc.wino3x3_flag)
		{
			rowtile_baseaddr0+=rowtile_baseaddr_increment_step*2;
			rowtile_baseaddr1+=rowtile_baseaddr_increment_step*2;
		}
		else
		{
			rowtile_baseaddr0+=rowtile_baseaddr_increment_step;
			rowtile_baseaddr1+=rowtile_baseaddr_increment_step;
		}


		out_ddr_offset0+=conv_desc.out_ddr_increment_step;
		out_ddr_offset1+=conv_desc.out_ddr_increment_step;
		out_ddr_offset2+=conv_desc.out_ddr_increment_step;
		out_ddr_offset3+=conv_desc.out_ddr_increment_step;
		outrow_idx+=4;

	}
}



void write_output_to_DDR3(
		ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM>* out_DDR0,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM>* out_DDR1,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM>* out_DDR2,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM>* out_DDR3,
		ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
		ap_int<16> start_row_idx,
		ap_uint<1> first_flag,
		ConvDesc_t conv_desc
)
{


	#pragma HLS array_partition variable=out_buffer complete dim=1
	#pragma HLS array_partition variable=out_buffer complete dim=2
	#pragma HLS array_partition variable=out_buffer complete dim=3
	#pragma HLS array_partition variable=out_buffer complete dim=4

	static ap_uint<24> out_ddr_offset0;
	static ap_uint<24> out_ddr_offset1;
	static ap_uint<24> out_ddr_offset2;
	static ap_uint<24> out_ddr_offset3;
	
	
	printf("---write_output_to_DDR %d---\n",(int) start_row_idx);
	if(start_row_idx<0) return;

	ap_uint<16> outrow_idx=start_row_idx;
	if( first_flag)
	{
		out_ddr_offset0=0;
		out_ddr_offset1=conv_desc.output_burst_length;
		out_ddr_offset2= (conv_desc.stride==2)?conv_desc.output_burst_length:((conv_desc.output_burst_length)<<1);
		out_ddr_offset3=out_ddr_offset1+out_ddr_offset2;
	}

	ap_uint<16> rowtile_baseaddr_increment_step=conv_desc.wino_tile_number_in_outwidth;
	ap_uint<16> rowtile_baseaddr0=0;


	ap_uint<16> rowtile_baseaddr1;

	for(ap_uint<8> row_idx=0; row_idx<conv_desc.out_rowstep;row_idx+=conv_desc.wino_output_tile_size)
	{


		
		write_output_row<0>(
		out_DDR0+out_ddr_offset0,
		out_buffer[0],
		rowtile_baseaddr0,
		outrow_idx,
		conv_desc);

		write_output_row<1>(
		out_DDR1+out_ddr_offset1,
		out_buffer[1],
		rowtile_baseaddr0,
		outrow_idx+1,
		conv_desc);

		write_output_row<2>(
		out_DDR2+out_ddr_offset2,
		out_buffer[2],
		rowtile_baseaddr0,
		outrow_idx+2,
		conv_desc);


		write_output_row<3>(
		out_DDR3+out_ddr_offset3,
		out_buffer[3],
		rowtile_baseaddr0,
		outrow_idx+3,
		conv_desc);

		out_ddr_offset0+=conv_desc.out_ddr_increment_step;
		out_ddr_offset1+=conv_desc.out_ddr_increment_step;
		out_ddr_offset2+=conv_desc.out_ddr_increment_step;
		out_ddr_offset3+=conv_desc.out_ddr_increment_step;



		rowtile_baseaddr0+=rowtile_baseaddr_increment_step;
		outrow_idx+=conv_desc.wino_output_tile_size;

	}
}






void write_output_to_DDR(
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR0,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR1,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR2,
		ap_uint<ODDR_WIDTH*BATCH_SIZE*8>* out_DDR3,
		ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][OUTPUT_BUFFER_DEPTH],
		ap_int<16> start_row_idx,
		ap_uint<1> first_flag,
		ap_int<18> oback_scale,
		ap_int<16> bias_buffer[8][BIAS_BUFFER_DEPTH],
		ConvDesc_t conv_desc
)
{


	#pragma HLS array_partition variable=out_buffer complete dim=1
	#pragma HLS array_partition variable=out_buffer complete dim=2
	#pragma HLS array_partition variable=out_buffer complete dim=3
	#pragma HLS array_partition variable=out_buffer complete dim=4

	static ap_uint<24> out_ddr_offset0;
	static ap_uint<24> out_ddr_offset1;
	static ap_uint<24> out_ddr_offset2;
	static ap_uint<24> out_ddr_offset3;
	printf("---write_output_to_DDR %d---\n",(int) start_row_idx);
	if(start_row_idx<0) return;

	ap_uint<16> outrow_idx=start_row_idx;
	if( first_flag)
	{
		out_ddr_offset0=0;
		out_ddr_offset1=conv_desc.output_burst_length;
		out_ddr_offset2=(conv_desc.output_burst_length)<<1;
		out_ddr_offset3=out_ddr_offset1+out_ddr_offset2;
	}

	ap_uint<16> rowtile_baseaddr=0;

	for(ap_uint<4> wino_rowtile_idx=0; wino_rowtile_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_rowtile_idx++)
	{
		write_output_to_DDR_onerow<0>(
			out_DDR0+out_ddr_offset0,
			out_buffer[0],
			conv_desc.outheight,
			conv_desc.outwidth_align8,
			conv_desc.wino_output_tile_size,
			conv_desc.wino_col_pix_upper_bound,
			conv_desc.wino_tile_number_rowcol,
			conv_desc.output_burst_length,
			rowtile_baseaddr,
			outrow_idx+0,
			oback_scale,
			bias_buffer
			#if DEBUG_CONV_DESC
			,conv_desc
			#endif
			);
		// write_output_d0
		// write_output_d1

		write_output_to_DDR_onerow<1>(
			out_DDR1+out_ddr_offset1,
			out_buffer[1],
			conv_desc.outheight,
			conv_desc.outwidth_align8,
			conv_desc.wino_output_tile_size,
			conv_desc.wino_col_pix_upper_bound,
			conv_desc.wino_tile_number_rowcol,
			conv_desc.output_burst_length,
			rowtile_baseaddr,
			outrow_idx+1,
			oback_scale,
			bias_buffer
			#if DEBUG_CONV_DESC
			,conv_desc
			#endif
			);

		// #if WINO_OUT_SIZE == 4
		// write_output_to_DDR_onerow<2>(
		// 	out_DDR2+out_ddr_offset2,
		// 	out_buffer[2],
		// 	outheight,
		// 	outwidth_align8,
		// 	wino_output_tile_size,
		// 	wino_col_pix_upper_bound,
		// 	wino_tile_number_rowcol,
		// 	output_burst_length,
		// 	rowtile_baseaddr,
		// 	outrow_idx+2,
		// 	oback_scale,
		// 	bias_buffer
		// 	#if DEBUG_CONV_DESC
		// 	,conv_desc
		// 	#endif
		// 	);
		// write_output_to_DDR_onerow<3>(
		// 	out_DDR3+out_ddr_offset3,
		// 	out_buffer[3],
		// 	outheight,
		// 	outwidth_align8,
		// 	wino_output_tile_size,
		// 	wino_col_pix_upper_bound,
		// 	wino_tile_number_rowcol,
		// 	output_burst_length,
		// 	rowtile_baseaddr,
		// 	outrow_idx+3,
		// 	oback_scale,
		// 	bias_buffer
		// 	#if DEBUG_CONV_DESC
		// 	,conv_desc
		// 	#endif
		// 	);
		// #endif
		rowtile_baseaddr+=conv_desc.wino_tile_number_in_outwidth;
		out_ddr_offset0+=conv_desc.out_ddr_increment_step;
		out_ddr_offset1+=conv_desc.out_ddr_increment_step;
		out_ddr_offset2+=conv_desc.out_ddr_increment_step;
		out_ddr_offset3+=conv_desc.out_ddr_increment_step;
		outrow_idx+=conv_desc.wino_output_tile_size;
	}
}

#endif
