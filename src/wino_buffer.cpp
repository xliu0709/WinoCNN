

#ifndef _WINO_BUFFER_CPP_
#define _WINO_BUFFER_CPP_
#include "wino_macro.h"
#include "wino_struct.h"
#include <ap_int.h>
#include <hls_stream.h>

#include "wino_transform.cpp"




void input_feed_underconstruction(
	ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
	hls::stream< ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_stream[WINO_WIDTH], 
	ap_uint<16> inwidth,
	// ap_uint<16> pad_size,
	ap_uint<16> weightbuffer_load_outdepth_number,
	ap_uint<16> wino_output_tile_size,
	ap_uint<32> input_buffer_feeding_loop_bound,
	ap_uint<16> loop_wino_tile_row_reset_cycle,
	ap_uint<16> loop_wino_tile_col_reset_cycle,
	ap_uint<10> buffer_address_mid_minitile_depth_step,
	ap_uint<16> wino_out_size_by_wino_width,
	ap_uint<1> row_address_bitnumber_flag,
	ap_int<16> start_row_idx_minus_pad_size,
	ap_int<16> start_col_idx_minus_pad_size
)
{
	#if DEBUG_FILE_PRINT
	printf("---input_feed_underconstruction---\n");fflush(stdout);
	#endif
	// row_selection preparation 
	#pragma HLS array_partition variable=input_buffer dim=1 complete
	#pragma HLS array_partition variable=input_buffer dim=2 complete

	ap_int<10> input_row_idx[WINO_DOMAIN_SIZE];
	#pragma HLS array_partition variable=input_row_idx complete
	
	ap_int<10> input_col_idx[WINO_WIDTH][WINO_DOMAIN_SIZE];
	#pragma HLS array_partition variable=input_col_idx dim=1 complete
	#pragma HLS array_partition variable=input_col_idx dim=2 complete

	ap_uint<16> wino_col_offset_constant[WINO_WIDTH];
	#pragma HLS array_partition variable=wino_col_offset_constant complete

	for(int i=0;i<WINO_WIDTH;i++)
	{
		#pragma HLS unroll
		wino_col_offset_constant[i]=wino_output_tile_size*i;
	}


	ap_uint<16>  first_col_idx=0;

	ap_uint<INDEPTH_MINITILE_SIZE_BITWIDTH> loop_indepth_minitile_idx=0;

	ap_uint<16> loop_wino_tile_row_cnt=1;
	ap_uint<16> loop_wino_tile_col_cnt=1;




	// loop_wino_tile_col_reset_cycle =conv_desc.wino_tile_number_in_outwidth*conv_desc.weightbuffer_outdepth_minitile_number*INDEPTH_MINITILE_SIZE;
	




	ap_uint<INBUFFER_MID_ADDR_BITWIDTH> buffer_address_mid_minitile_depth_offset=0;
	ap_uint<INBUFFER_MID_ADDR_BITWIDTH> buffer_address_mid_buffertile_depth_offset=0;




	ap_int<10> input_head_row_idx=start_row_idx_minus_pad_size;

	for(int i=0;i<WINO_DOMAIN_SIZE;i++)
	{
		input_row_idx[i]=start_row_idx_minus_pad_size+i;
	}

	ap_int<16> input_head_col_idx=start_col_idx_minus_pad_size;

	for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
	{
		#pragma HLS unroll
		for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		{
			#pragma HLS unroll
			input_col_idx[wino_array_col][i]=i+start_col_idx_minus_pad_size+wino_col_offset_constant[wino_array_col];
		}
	}



	for(ap_uint<16> outdepth_buffertile_idx=0;outdepth_buffertile_idx<weightbuffer_load_outdepth_number;outdepth_buffertile_idx++)
	{

		buffer_address_mid_minitile_depth_offset = 0;

		for(int counter=0;counter<input_buffer_feeding_loop_bound;counter++ )
		{
			#pragma HLS pipeline II =1
			// it is a flattened loop which does following
			// for(ap_uint<16> indepth_buffertile_baseidx=0;indepth_buffertile_baseidx<weightbuffer_load_indepth_number;indepth_buffertile_baseidx++)
			// for( int indepth_minitile_baseidx=0;indepth_minitile_baseidx<weightbuffer_indepth_minitile_number; indepth_minitile_baseidx ++)
			// for( int wino_row_cnt;wino_row_cnt <conv_desc.wino_tile_number_in_outwidth;wino_row_cnt++)
			// for(int wino_tile_col_idx =1; wino_tile_col_idx < wino_tile_number_in_outwidth+1 ; wino_tile_col_idx++)
			// for(ap_uint<3> indepth_minitile_idx=0; indepth_minitile_idx< INDEPTH_MINITILE_SIZE; indepth_minitile_idx++)


			ap_uint<1> row_legal_flag[WINO_DOMAIN_SIZE];
			#pragma HLS array_partition variable=row_legal_flag complete

			for(int i=0;i<WINO_DOMAIN_SIZE;i++)
			{
			#pragma HLS unroll
				row_legal_flag[i] = ( input_row_idx[i] >=0 && input_row_idx[i] < inwidth);
			}


			ap_uint<1> row_address_offset_bit1[INBUFFER_HEIGHT];
			#pragma HLS array_partition variable=row_address_offset_bit1 complete
			
			ap_uint<2> row_address_offset_bit2[INBUFFER_HEIGHT];
			#pragma HLS array_partition variable=row_address_offset_bit2 complete

			ap_uint<INBUFFER_HEIGHT_BITWIDTH> row_breakpoint=input_head_row_idx.range(INBUFFER_HEIGHT_BITWIDTH-1,0);
			ap_uint<2> input_head_row_address_offset=input_head_row_idx.range(INBUFFER_HEIGHT_BITWIDTH+1,INBUFFER_HEIGHT_BITWIDTH);



			for(int i=0;i<INBUFFER_HEIGHT;i++)
			{
				if(i>=row_breakpoint)
				{
					row_address_offset_bit1[i]=input_head_row_address_offset;
					row_address_offset_bit2[i]=input_head_row_address_offset;
				}
				else
				{
					row_address_offset_bit1[i]=input_head_row_address_offset+1;
					row_address_offset_bit2[i]=input_head_row_address_offset+1;
				}
				
			}



			ap_uint<1> col_legal_flag[WINO_WIDTH][WINO_DOMAIN_SIZE];
			#pragma HLS array_partition variable=col_legal_flag complete

			for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			{
				#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					col_legal_flag[wino_array_col][i]= ( input_col_idx[wino_array_col][i] >=0 && input_col_idx[wino_array_col][i] < inwidth);
				}
			}



			
			ap_uint<INBUFFER_MID_ADDR_BITWIDTH> col_pix_address_offset[INBUFFER_WIDTH];
			#pragma HLS array_partition variable=col_pix_address_offset complete
			ap_uint<INBUFFER_WIDTH_BITWIDTH> col_breakpoint=input_head_col_idx.range(INBUFFER_WIDTH_BITWIDTH-1,0);
		

			ap_uint<INBUFFER_MID_ADDR_BITWIDTH> input_head_col_address_offset;
			input_head_col_address_offset= input_head_col_idx.range(INBUFFER_WIDTH_BITWIDTH+INBUFFER_MID_ADDR_BITWIDTH-1,INBUFFER_WIDTH_BITWIDTH) 
			+ buffer_address_mid_minitile_depth_offset;




			for(int i=0;i<INBUFFER_WIDTH;i++)
			{
				if(i>=col_breakpoint)
					col_pix_address_offset[i] = input_head_col_address_offset;
				else
					col_pix_address_offset[i] = input_head_col_address_offset+1;
			}


			ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH> buffer_address[INBUFFER_HEIGHT][INBUFFER_WIDTH];
			#pragma HLS array_partition variable=buffer_address complete dim=2
			#pragma HLS array_partition variable=buffer_address complete dim=1

			for(int i=0;i<INBUFFER_HEIGHT; i++)
			{
				#pragma HLS unroll
				for(int j=0;j<INBUFFER_WIDTH;j++)
				{
					#pragma HLS unroll
					ap_uint<INPUT_BUFFER_DEPTH_BITWIDTH-2> common=((ap_uint<INBUFFER_MID_ADDR_BITWIDTH-1> )col_pix_address_offset[j],loop_indepth_minitile_idx);
					ap_uint<2> headbits=row_address_bitnumber_flag?
										(row_address_offset_bit1[i],col_pix_address_offset[j][INBUFFER_MID_ADDR_BITWIDTH-1]):
										row_address_offset_bit2[i];
					buffer_address[i][j]=(headbits,common);
				}
			}

			ap_uint<16> input_buffer_val[INBUFFER_HEIGHT][INBUFFER_WIDTH];
			#pragma HLS array_partition variable=input_buffer_val complete dim=1
			#pragma HLS array_partition variable=input_buffer_val complete dim=2

			for(int i=0;i<INBUFFER_HEIGHT; i++)
			{
				#pragma HLS unroll
				for(int j=0;j<INBUFFER_WIDTH;j++)
				{
					#pragma HLS unroll
					input_buffer_val[i][j]=input_buffer[i][j][buffer_address[i][j]];
				}
			}



			ap_uint<16> input_plane_tile_row[WINO_DOMAIN_SIZE][INBUFFER_WIDTH];
			#pragma HLS array_partition variable=input_plane_tile_row dim=1 complete
			#pragma HLS array_partition variable=input_plane_tile_row dim=2 complete


			for(int j=0;j<INBUFFER_WIDTH;j++)
			{
			#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
				#pragma HLS unroll
					if(row_legal_flag[i])
					{
						input_plane_tile_row[i][j]=input_buffer_val[ (ap_uint<INBUFFER_HEIGHT_BITWIDTH>) input_row_idx[i].range(INBUFFER_HEIGHT_BITWIDTH-1,0) ][j];
					}
					else
					{
						input_plane_tile_row[i][j]=0;
					}
				}
			}



			ap_uint<16*WINO_DOMAIN_SIZE_SQUARE> input_plane_tile[WINO_WIDTH];
			#pragma HLS array_partition variable=input_plane_tile complete dim=1


			for(int i=0;i<WINO_WIDTH;i++)
			{
			#pragma HLS unroll
				for(int j=0;j<WINO_DOMAIN_SIZE;j++)
				{
				#pragma HLS unroll
					for(int k=0;k<WINO_DOMAIN_SIZE;k++)
					{
					#pragma HLS unroll
						if(col_legal_flag[i][k])
							input_plane_tile[i].range((j*WINO_DOMAIN_SIZE+k)*16+15,(j*WINO_DOMAIN_SIZE+k)*16)=input_plane_tile_row[j][ (ap_uint<INBUFFER_WIDTH_BITWIDTH>) input_col_idx[i][k].range(INBUFFER_WIDTH_BITWIDTH-1,0) ];
						else
							input_plane_tile[i].range((j*WINO_DOMAIN_SIZE+k)*16+15,(j*WINO_DOMAIN_SIZE+k)*16)=0;
					}
				}
			}


			for(int i=0;i<WINO_WIDTH;i++)
			{
				#pragma HLS unroll
				input_tile_stream[i]<<input_plane_tile[i];
			}

			#if DEBUG_FILE_PRINT
				// if(counter<4 && outdepth_buffertile_idx==0)
				// {
					int indepth = buffer_address_mid_minitile_depth_offset/buffer_address_mid_minitile_depth_step*INDEPTH_MINITILE_SIZE
								+loop_indepth_minitile_idx;
					attach_streaming_content<WINO_WIDTH,WINO_DOMAIN_SIZE>(input_plane_tile, start_row_idx_minus_pad_size+pad_size, input_head_col_idx+pad_size, indepth, (char*) "instream.txt");
				// }

				getchar();
			#endif


			

			if(loop_wino_tile_row_cnt == loop_wino_tile_row_reset_cycle)
			{
				buffer_address_mid_minitile_depth_offset += buffer_address_mid_minitile_depth_step;
			}

			if(loop_wino_tile_col_cnt == loop_wino_tile_col_reset_cycle)
			{
				input_head_col_idx=start_col_idx_minus_pad_size;
				for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
				{
					#pragma HLS unroll
					for(int i=0;i<WINO_DOMAIN_SIZE;i++)
					{
						#pragma HLS unroll
						input_col_idx[wino_array_col][i]=i+start_col_idx_minus_pad_size+wino_col_offset_constant[wino_array_col];
					}
				}
			}
			else if(loop_indepth_minitile_idx==INDEPTH_MINITILE_SIZE-1)
			{
				input_head_col_idx+=wino_out_size_by_wino_width;
				for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
				{
					#pragma HLS unroll
					for(int i=0;i<WINO_DOMAIN_SIZE;i++)
					{
						#pragma HLS unroll
						input_col_idx[wino_array_col][i]+=wino_out_size_by_wino_width;
					}
				}
			}


			
			if(loop_wino_tile_row_cnt == loop_wino_tile_row_reset_cycle)
			{
				input_head_row_idx=start_row_idx_minus_pad_size;

				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					input_row_idx[i]=start_row_idx_minus_pad_size+i;
				}

			}
			else if(loop_wino_tile_col_cnt == loop_wino_tile_col_reset_cycle)
			{
				input_head_row_idx+=wino_output_tile_size;
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					input_row_idx[i]+=wino_output_tile_size;
				}
			}


			if(loop_wino_tile_row_cnt==loop_wino_tile_row_reset_cycle)
			{
				loop_wino_tile_row_cnt=1;
			}
			else
			{
				loop_wino_tile_row_cnt++;
			}

			if(loop_wino_tile_col_cnt == loop_wino_tile_col_reset_cycle)
			{
				loop_wino_tile_col_cnt=1;
			}
			else 
			{
				loop_wino_tile_col_cnt++;
			}
			loop_indepth_minitile_idx++;							
		}
	}	
}


void input_transform(
	hls::stream< ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &input_tile_stream,
	hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &input_tile_transformed_stream,
	int input_transform_feeding_loop_bound,
	bool wino3x3_flag,
	ap_uint<3> wino_array_col
)
{
	#if DEBUG_FILE_PRINT
	printf("---input_transform---\n");fflush(stdout);

	int write_idx=0;
	#endif
	
	for(int cycle=0;cycle<input_transform_feeding_loop_bound;cycle++)
	{
		#pragma HLS pipeline
		ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> input_tile_stream_data;
		input_tile_stream>>input_tile_stream_data;

		ap_int<IN_WIDTH> in[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=in complete dim=1
		#pragma HLS array_partition variable=in complete dim=2
		#pragma HLS array_partition variable=in complete dim=3



		for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		{
			#pragma HLS unroll
			for(int j=0;j<WINO_DOMAIN_SIZE;j++)
			{
				#pragma HLS unroll
				for(int k=0;k<BATCH_SIZE;k++)
				{
					#pragma HLS unroll
					in[i][j][k]=input_tile_stream_data.range( ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*8+IN_WIDTH-1, ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*8);
				}
			}
		}
		#if DEBUG_FILE_PRINT
			char infilename[100];
			sprintf(infilename,"intile_transform_%d.txt",(int) wino_array_col);
			attach_output_vector<IN_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(in,write_idx,infilename);
		#endif
		// if(cycle == 0)
		// {
		// 	for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		// 	{
		// 		for(int j=0;j<WINO_DOMAIN_SIZE;j++)
		// 		{
		// 			printf("[%8d]",(int) in[i][j][0] );
		// 		}
		// 		printf("\n");

		// 	}
		// 	printf("\n");
		// 	for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		// 	{
		// 		for(int j=0;j<WINO_DOMAIN_SIZE;j++)
		// 		{
		// 			printf("[%8d]",(int) in[i][j][1] );
		// 		}
		// 		printf("\n");

		// 	}
		// 	getchar();
		// // }


		ap_int<DB_WIDTH> DB[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=DB complete dim=1
		#pragma HLS array_partition variable=DB complete dim=2
		#pragma HLS array_partition variable=DB complete dim=3

		for(int k=0;k<BATCH_SIZE;k++)
		{
		#pragma HLS unroll
			for(int i=0;i<WINO_DOMAIN_SIZE;i++)
			{
			#pragma HLS unroll
				#if WINO_DOMAIN_SIZE == 6
					DB6x6_1(in,DB,i,k)
				#else
					DB4x4_1(in,DB,i,k)
				#endif	
			}
		}
		// std::cout<<"DB"<<std::endl;
		// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		// {
		// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
		// 	{
		// 		printf("%8d ",(int) DB[i][j][0] );
		// 	}
		// 	printf("\n");

		// }
		
		// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		// {
		// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
		// 	{
		// 		printf("%8d ",(int) DB[i][j][1] );
		// 	}
		// 	printf("\n");

		// }
		// getchar();

		ap_int<BTB_WIDTH> BtDB[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=BtDB complete dim=1
		#pragma HLS array_partition variable=BtDB complete dim=2
		#pragma HLS array_partition variable=BtDB complete dim=3


		if(wino3x3_flag)
		{
	
			for(int k=0;k<BATCH_SIZE;k++)
			{
			#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
				#pragma HLS unroll
					#if WINO_DOMAIN_SIZE ==6
						BTB6x6_1(DB,BtDB,i,k)
					#else
						BTB4x4_1(DB,BtDB,i,k)
					#endif	
				}
			}

		}
		else
		{
			for(int k=0;k<BATCH_SIZE;k++)
			{
				#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					for(int j=0;j<WINO_DOMAIN_SIZE;j++)
					{
						#pragma HLS unroll
						BtDB[i][j][k]=in[i][j][k];
					}
				}
			}
		}
	
		// if(cycle == 0)
		// {
			// std::cout<<"BtdB"<<std::endl;
			// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
			// {
			// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
			// 	{
			// 		printf("%8d ",(int) BtDB[i][j][0] );
			// 	}
			// 	printf("\n");
			// }
			// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
			// {
			// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
			// 	{
			// 		printf("%8d ",(int) BtDB[i][j][1] );
			// 	}
			// 	printf("\n");
			// }
			// getchar();
		// }
		ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> input_tile_transformed_data;

		for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		{
			#pragma HLS unroll
			for(int j=0;j<WINO_DOMAIN_SIZE;j++)
			{
				#pragma HLS unroll
				for(int k=0;k<BATCH_SIZE;k++)
				{
					#pragma HLS unroll
					input_tile_transformed_data.range(  ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH+BTB_WIDTH-1, ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH)=BtDB[i][j][k];
				}
			}
		}


		#if DEBUG_FILE_PRINT
			// char infilename[100];
			sprintf(infilename,"intile_transform_%d.txt",(int) wino_array_col);
			attach_output_vector<BTB_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(BtDB,write_idx,infilename);
			write_idx++;
		#endif
		input_tile_transformed_stream<<input_tile_transformed_data;
	}
}



//template<int dummy>
void load_weight_ddr_one_port(
	ap_uint<128>* weight_DDR,
	ap_uint<W_WIDTH*4> weight_buff[WEIGHT_FEED_NUMBER_PER_PORT][WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4][WEIGHT_BUFFER_DEPTH],
	ap_uint<16> weightDDR_buffer_burst_length,
	ap_uint<24> weightDDR_port_burst_length,
	ap_uint<32> ddr_address_offset,
	ap_uint<1> wino3x3_flag,
	ap_uint<1> pingpong,
	ap_uint<1> skip_flag
	#if DEBUG_CONV_DESC
	,ConvDesc_t conv_desc
	#endif
	)
{
	#pragma HLS interface m_axi port=weight_DDR
	#if DEBUG_FILE_PRINT
	printf("\t^^load_weight_ddr_one_port^^ \n\t SKIP FLAG:%d \n\t Pingpong %d \n", (int) skip_flag, (int) pingpong);fflush(stdout);
	#endif
	#pragma HLS array_partition variable = weight_buff dim=1 complete
	#pragma HLS array_partition variable = weight_buff dim=2 complete


	if(skip_flag)
		return;

	ap_uint<8> counter_boundary;

	#if WINO_DOMAIN_SIZE == 6
	// if(kernel_size==5)
	// 	counter_boundary=WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP-1;
	// else
	// 	counter_boundary=INDEPTH_MINITILE_SIZE/2-1;
		counter_boundary=INDEPTH_MINITILE_SIZE/2-1;
	#else
		counter_boundary=INDEPTH_MINITILE_SIZE/2-1;
	#endif

	ap_uint<128>* offseted_weight_DDR=weight_DDR+ddr_address_offset;

	ap_uint<WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP_BITWIDTH+1> counter_x2=0;

	ap_uint<24> port_load_cnt_x2=2;

	ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH> buffer_address_offset_x2=0;

	#if WEIGHT_FEED_NUMBER_PER_PORT_BITWIDTH == 0
	ap_uint<1> buffer_idx;
	#else
	ap_uint<WEIGHT_FEED_NUMBER_PER_PORT_BITWIDTH> buffer_idx=0;
	#endif
	

	#if WINO5x5_EN
	ap_uint<W_WIDTH*4> weight_reg[WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4];
	#endif
	
	#if WINO3x3_EN
	ap_uint<W_WIDTH*4> trans_weight_reg[WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4];
	#endif

	// std::cout<<"counter_boundary" <<counter_boundary <<std::endl;
	for(int address = 0; address<weightDDR_port_burst_length; address++)
	{		
		#pragma HLS pipeline
		ap_uint<128> temp128 = offseted_weight_DDR[address];

		#if WINO3x3_EN
			ap_int<G_WIDTH> g_tile[2][3][3];
			#pragma HLS array_partition  variable = g_tile complete dim=1
			#pragma HLS array_partition  variable = g_tile complete dim=2
			#pragma HLS array_partition  variable = g_tile complete dim=3
			for(int i=0;i<2;i++)
			{
				#pragma HLS unroll
				for(int j=0;j<3;j++)
				{
					#pragma HLS unroll
					for(int k=0;k<3;k++)
					{
						#pragma HLS unroll
						g_tile[i][j][k]=temp128.range( (i*9+j*3+k)*G_WIDTH+G_WIDTH-1, (i*9+j*3+k)*G_WIDTH);
					}
				}
			}

			// // if(address==0)
			// // {
				// for(int i=0;i<2;i++)
				// {
				// 	#pragma HLS unroll
				// 	for(int j=0;j<3;j++)
				// 	{
				// 		#pragma HLS unroll
				// 		for(int k=0;k<3;k++)
				// 		{
				// 			#pragma HLS unroll
				// 			printf("[%3d ]",(int) g_tile[i][j][k]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	printf("\n");
				// }
				// printf("\n");
				// getchar();
			// // }


			ap_int<GL_WIDTH> gG_tile[2][WINO_DOMAIN_SIZE][3];
			#pragma HLS array_partition  variable = gG_tile complete dim=1
			#pragma HLS array_partition  variable = gG_tile complete dim=2
			#pragma HLS array_partition  variable = gG_tile complete dim=3
			for(int b=0;b<2;b++)
			{
				#pragma HLS unroll
				for(int c=0;c<3;c++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					gGT3to6(g_tile,gG_tile,c,b);
					#else
					gGT3to4(g_tile,gG_tile,c,b);
					#endif
				}
			}

			ap_int<RG_WIDTH> GgG_tile[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
			#pragma HLS array_partition  variable = GgG_tile complete dim=1
			#pragma HLS array_partition  variable = GgG_tile complete dim=2
			#pragma HLS array_partition  variable = GgG_tile complete dim=3
			for(int b=0;b<2;b++)
			{
				#pragma HLS unroll
				for(int r=0;r<WINO_DOMAIN_SIZE;r++)
				{
					#pragma HLS unroll
					
					#if WINO_DOMAIN_SIZE==6
					GgG3to6(gG_tile,GgG_tile,r,b);
					#else
					GgG3to4(gG_tile,GgG_tile,r,b);
					#endif
				}
			}

			// for(int i=0;i<16;i++)
			// {
			// 	printf("%d ", (int) GgG_tile[0][i/4][i%4] );
			// }

			// printf("\n");
			// for(int i=0;i<16;i++)
			// {
			// 	printf("%d ", (int) GgG_tile[1][i/4][i%4] );
			// }		
			// printf("\n");
			// getchar();
			ap_int<W_WIDTH> GgG_tile_flat[2*WINO_DOMAIN_SIZE_SQUARE];
			#pragma HLS array_partition  variable = GgG_tile_flat complete dim=1
			if(wino3x3_flag==0)
			{
				for(int i=0;i<2*WINO_DOMAIN_SIZE_SQUARE;i++)
				{
					#pragma HLS unroll
					GgG_tile_flat[i]=g_tile[i/WINO_DOMAIN_SIZE_SQUARE][0][0]*(512>>(RG_WIDTH-W_WIDTH) ) ;//.range(RG_WIDTH-1,RG_WIDTH-W_WIDTH);
				}
			}
			else
			{
				for(int i=0;i<2*WINO_DOMAIN_SIZE_SQUARE;i++)
				{
					#pragma HLS unroll
					GgG_tile_flat[i]=GgG_tile[i/WINO_DOMAIN_SIZE_SQUARE][i%WINO_DOMAIN_SIZE_SQUARE/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE].range(RG_WIDTH-1,RG_WIDTH-W_WIDTH);
				}
			}





			if(counter_x2/2==0)
			{
				for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE/2;i++)
				{
					#pragma HLS unroll
					trans_weight_reg[i]=(GgG_tile_flat[i*4+3],GgG_tile_flat[i*4+2],GgG_tile_flat[i*4+1],GgG_tile_flat[i*4]);
				}
			}
			else
			{
				for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE/2;i++)
				{
					#pragma HLS unroll
					trans_weight_reg[i+WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4/2]=(GgG_tile_flat[i*4+3],GgG_tile_flat[i*4+2],GgG_tile_flat[i*4+1],GgG_tile_flat[i*4]);
				}
			}
			
		#endif

		// std::cout<<"counter_x2 "<<counter_x2<<std::endl;
		// std::cout<<"buffer_address_offset_x2 "<<buffer_address_offset_x2<<std::endl;

		ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH> buffer_address = (pingpong,buffer_address_offset_x2.range(WEIGHT_BUFFER_DEPTH_BITWIDTH-1,1));
		if(counter_x2/2==counter_boundary)
		{
			for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4;i++)
			{
				#if WEIGHT_FEED_NUMBER_PER_PORT_BITWIDTH == 0
					#if WINO3x3_EN && WINO5x5_EN
					if(kernel_size==3)
					{
						weight_buff[0][i][buffer_address]=trans_weight_reg[i];
					}
					else
					{
						weight_buff[0][i][buffer_address]=weight_reg[i];
					}
					

					#elif WINO3x3_EN 
					weight_buff[0][i][buffer_address]=trans_weight_reg[i];
					
					#elif WINO5x5_EN
					weight_buff[0][i][buffer_address]=weight_reg[i];
					#endif


				#else

					#if WINO3x3_EN && WINO5x5_EN
					if(kernel_size==3)
					{
						weight_buff[buffer_idx][i][buffer_address]=trans_weight_reg[i];
					}
					else
					{
						weight_buff[buffer_idx][i][buffer_address]=weight_reg[i];
					}
					

					#elif WINO3x3_EN 
					weight_buff[buffer_idx][i][buffer_address]=trans_weight_reg[i];
					// for(int k=0;k<4;k++)
					// {
					// 	printf("%d ", (int) weight_buff[buffer_idx][i][buffer_address].range(k*W_WIDTH+W_WIDTH-1,k*W_WIDTH) );
					// }
					// printf("\n");
					#elif WINO5x5_EN
					weight_buff[buffer_idx][i][buffer_address]=weight_reg[i];
					#endif

					// weight_buff[buffer_idx][i][buffer_address]=weight_reg[i];
				#endif
			}
			
			// fflush(stdout);
			// getchar();

		}

		if(port_load_cnt_x2/2==weightDDR_buffer_burst_length)
		{
			buffer_address_offset_x2=0;
		}
		else if(counter_x2/2== counter_boundary)
		{
			buffer_address_offset_x2+=2;
		}

		if(port_load_cnt_x2/2==weightDDR_buffer_burst_length)
		{
			buffer_idx++;
			port_load_cnt_x2=2;
		}
		else
		{
			port_load_cnt_x2+=2;
		}

		if(counter_x2/2==counter_boundary)
		{
			counter_x2=0;
		}
		else
		{
			counter_x2+=2;
		}
	}
}


void weight_streamer(
	ap_uint<W_WIDTH*4> weight_buff[WEIGHT_FEED_NUMBER_PER_PORT][WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4][WEIGHT_BUFFER_DEPTH],
	#if WINO_HEIGHT==8 || WINO_HEIGHT==2
	hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream[WEIGHT_FEED_NUMBER_PER_PORT],
	#else
	hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  &weight_stream,
	#endif
	ap_uint<16> loop_outdepth_minitile_baseidx_reset_cycle_minus1,
	ap_uint<16> loop_start_output_baserowcol_reset_cycle,
	ap_uint<32> loop_weight_feed_bound,
	ap_uint<16> weightbuffer_outdepth_minitile_number,
	ap_uint<1> pingpong
	#if DEBUG_CONV_DESC
	,ConvDesc_t conv_desc
	#endif
)
{
	#pragma HLS array_partition variable = weight_buff dim=1 complete
	#pragma HLS array_partition variable = weight_buff dim=2 complete
	// int weight_feed_total_size_by2 = weight_feed_total_size/2;

	ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH -1> outdepth_minitile_addr_offset=0;
	ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH -1> indepth_minitile_addr_offset=0;

	ap_uint<16> loop_outdepth_minitile_baseidx_cnt=1;
	ap_uint<16> loop_start_output_baserowcol_cnt=1;




	// loop_outdepth_minitile_baseidx_reset_cycle_minus1=conv_desc.weightbuffer_outdepth_minitile_number-1;
	// loop_start_output_baserowcol_reset_cycle=conv_desc.weightbuffer_outdepth_minitile_number * conv_desc.wino_tile_number_in_outwidth;

	// int loop_weight_feed_bound = conv_desc.weightbuffer_indepth_minitile_number * conv_desc.weightbuffer_outdepth_minitile_number * conv_desc.wino_tile_number_in_outwidth;

	for(ap_uint<32> cycle=0;cycle < loop_weight_feed_bound; cycle++)
	{
		#pragma HLS pipeline
		// for(int indepth_minitile_baseidx=0;indepth_minitile_baseidx<conv_desc.weightbuffer_load_indepth_step; indepth_minitile_baseidx += INDEPTH_MINITILE_SIZE)
		// for(int start_output_baserowcol =0; start_output_baserowcol < conv_desc.outwidth *conv_desc.wino_out_tile_in_rowstep; start_output_baserowcol+=conv_desc.wino_output_tile_size*WINO_WIDTH)
		// for(int outdepth_minitile_baseidx=0;outdepth_minitile_baseidx<conv_desc.weightbuffer_load_outdepth_step; outdepth_minitile_baseidx += OUTDEPTH_MINITILE_SIZE)
		ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH-1> weight_buffer_address_right=indepth_minitile_addr_offset+outdepth_minitile_addr_offset;
		ap_uint<WEIGHT_BUFFER_DEPTH_BITWIDTH> weight_buffer_address = (pingpong,weight_buffer_address_right);


		ap_uint<W_WIDTH*4> temp18[WEIGHT_FEED_NUMBER_PER_PORT][WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4];
		#pragma HLS array_partition variable = temp18 complete

		// std::cout<<"cycle"<<cycle<<std::endl;
		// std::cout<<"loop_start_output_baserowcol_cnt"<<loop_start_output_baserowcol_cnt<<std::endl;
		// std::cout<<"outdepth_minitile_addr_offset"<<outdepth_minitile_addr_offset<<std::endl;
		// std::cout<<"indepth_minitile_addr_offset"<<indepth_minitile_addr_offset<<std::endl;
		// std::cout<<"loop_start_output_baserowcol_cnt"<<loop_start_output_baserowcol_cnt<<std::endl;


		for(int buffer_idx =0; buffer_idx< WEIGHT_FEED_NUMBER_PER_PORT; buffer_idx++)
		{
		#pragma HLS unroll
			for(int j18=0;j18<WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4;j18++)
			{
			#pragma HLS unroll
				temp18[buffer_idx][j18]=weight_buff[buffer_idx][j18][weight_buffer_address];				
			}
		}

		// if(cycle==0)
		// {
			// for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4;i++)
			// {
			// 	printf("[%d %d %d %d]\n", 
			// 	(int) temp18[0][i].range(W_WIDTH*0+W_WIDTH-1,W_WIDTH*0),
			// 	(int) temp18[0][i].range(W_WIDTH*1+W_WIDTH-1,W_WIDTH*1),
			// 	(int) temp18[0][i].range(W_WIDTH*2+W_WIDTH-1,W_WIDTH*2),
			// 	(int) temp18[0][i].range(W_WIDTH*3+W_WIDTH-1,W_WIDTH*3));
			// }
			// getchar();
		// }
	

		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> temp16x36[WEIGHT_FEED_NUMBER_PER_PORT];
		#pragma HLS array_partition variable = temp16x36 complete

		for(int buffer_idx =0; buffer_idx< WEIGHT_FEED_NUMBER_PER_PORT; buffer_idx++)
		{
			#pragma HLS unroll
			for(int j18=0;j18<WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4;j18++)
			{
				#pragma HLS unroll
				temp16x36[buffer_idx].range(j18*W_WIDTH*4+W_WIDTH*4-1,j18*W_WIDTH*4)=temp18[buffer_idx][j18];
			}
		}
		for(int buffer_idx =0; buffer_idx< WEIGHT_FEED_NUMBER_PER_PORT; buffer_idx++)
		{
			#pragma HLS unroll
			#if WINO_HEIGHT==8 || WINO_HEIGHT==2
			weight_stream[buffer_idx]<<temp16x36[buffer_idx];
			#else
			weight_stream<<temp16x36[buffer_idx];
			#endif
		}


		if(loop_start_output_baserowcol_cnt==loop_start_output_baserowcol_reset_cycle){
			indepth_minitile_addr_offset+=weightbuffer_outdepth_minitile_number;
		}
		
		if(outdepth_minitile_addr_offset==loop_outdepth_minitile_baseidx_reset_cycle_minus1){
			outdepth_minitile_addr_offset=0;
		}
		else{
			outdepth_minitile_addr_offset++;
		}

		if(loop_start_output_baserowcol_cnt==loop_start_output_baserowcol_reset_cycle){
			loop_start_output_baserowcol_cnt=1;
		}
		else{
			loop_start_output_baserowcol_cnt++;
		}
	}
}


template<int dummy> // the dummy template is to make sure each module have indepedent weight buffer
void weight_feed_one_port(
	ap_uint<128>* weight_DDR0,
	#if WINO_HEIGHT==8 || WINO_HEIGHT==2
	hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream[WEIGHT_FEED_NUMBER_PER_PORT],
	#else
	hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  &weight_stream,
	#endif
	ap_uint<16> weightDDR_burst_number,
	ap_uint<16> weightDDR_buffer_burst_length,
	ap_uint<24> weightDDR_port_burst_length,
	ap_uint<16> loop_outdepth_minitile_baseidx_reset_cycle_minus1,
	ap_uint<16> loop_start_output_baserowcol_reset_cycle,
	ap_uint<32> loop_weight_feed_bound,
	ap_uint<16> weightbuffer_outdepth_minitile_number,
	ap_uint<32> reset_DDR_offset,
	ap_uint<1> wino3x3_flag,
	ap_uint<1> first_flag,
	ap_uint<1> last_flag
	#if DEBUG_CONV_DESC
	,ConvDesc_t conv_desc	
	#endif
)
{
	#if DEBUG_FILE_PRINT
	printf("---weight_feed_one_port %d---\n", dummy);fflush(stdout);
	#endif
	static ap_uint<W_WIDTH*4> weight_buff[WEIGHT_FEED_NUMBER_PER_PORT][WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE/4][WEIGHT_BUFFER_DEPTH];

	#pragma HLS array_partition variable=weight_buff complete dim=1
	#pragma HLS array_partition variable=weight_buff complete dim=2
	#pragma HLS resource variable=weight_buff core=RAM_S2P_BRAM 

	static ap_uint<24> DDR_offset;
	static ap_uint<16> DDR_load_cnt;
	static ap_uint<1> pingpong;

	if(first_flag )
	{
		DDR_offset=0;
		DDR_load_cnt=0;
		pingpong = 0;
	}
	// std::cout<<"first flag "<<first_flag<<std::endl; 
	// std::cout<<"pingpong "<<pingpong<<std::endl; 
	// std::cout<<"reset_DDR_offset "<<reset_DDR_offset<<std::endl;
	load_weight_ddr_one_port(
	weight_DDR0,
	weight_buff,
	weightDDR_buffer_burst_length,
	weightDDR_port_burst_length,
	0,
	wino3x3_flag,
	0,
	~first_flag
	#if DEBUG_CONV_DESC
	,conv_desc
	#endif
	);

#if 0
	char filename[100];
	sprintf(filename,"weightbuffer_content%d.txt",dummy);

	attach_weight_buffer_content_uniformed<W_WIDTH,WINO_DOMAIN_SIZE,INDEPTH_MINITILE_SIZE,WEIGHT_FEED_NUMBER_PER_PORT,WEIGHT_BUFFER_DEPTH>
	(weight_buff,0,filename);
	// getchar();
#endif



	for(ap_uint<16> cnt=0;cnt< weightDDR_burst_number ;cnt++)
	{
		#pragma HLS DEPENDENCE variable=weight_buff intra false

		if(DDR_load_cnt == weightDDR_burst_number-1)
		{
			DDR_load_cnt = 0;
			DDR_offset = reset_DDR_offset;
		}
		else
		{
			DDR_load_cnt+=1;
			DDR_offset+=weightDDR_port_burst_length;
		}


		load_weight_ddr_one_port(
		weight_DDR0,
		weight_buff,
		weightDDR_buffer_burst_length,
		weightDDR_port_burst_length,
		DDR_offset,
		wino3x3_flag,
		~pingpong,
		last_flag & (DDR_load_cnt==0) 
		#if DEBUG_CONV_DESC
		,conv_desc
		#endif
		);
#if 0
	attach_weight_buffer_content_uniformed<W_WIDTH,WINO_DOMAIN_SIZE,INDEPTH_MINITILE_SIZE,WEIGHT_FEED_NUMBER_PER_PORT,WEIGHT_BUFFER_DEPTH>
	(weight_buff,cnt+1,filename);
#endif
		weight_streamer(
			weight_buff,
			weight_stream,
			loop_outdepth_minitile_baseidx_reset_cycle_minus1,
			loop_start_output_baserowcol_reset_cycle,
			loop_weight_feed_bound,
			weightbuffer_outdepth_minitile_number,
			pingpong
			#if DEBUG_CONV_DESC
			,conv_desc
			#endif
		);
		pingpong = ~pingpong;
	}
}
#endif
