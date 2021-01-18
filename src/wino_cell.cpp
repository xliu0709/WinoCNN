#ifndef _WINO_CELL_HPP_
#define _WINO_CELL_HPP_
#include "wino_macro.h"
#include <dsp_builtins.h>
#include <hls_stream.h>
#include <ap_int.h>
#include "wino_buffer.cpp"
#include "wino_transform.cpp"








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
		// 			printf("[%8d %8d]",(int) in[i][j][0],(int) in[i][j][1] );
		// 		}
		// 		printf("\n");

		// 	}
		// 	getchar();
		// }


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
				#if WINO_DOMAIN_SIZE ==6
					DB6x6_1(in,DB,i,k)
				#else
					DB4x4_1(in,DB,i,k)
				#endif	
			}
		}
		// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
		// {
		// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
		// 	{
		// 		printf("%8d ",(int) DB[i][j][0] );
		// 	}
		// 	printf("\n");

		// }
		// getchar();

		ap_int<BTB_WIDTH> BtDB[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=BtDB complete dim=1
		#pragma HLS array_partition variable=BtDB complete dim=2
		#pragma HLS array_partition variable=BtDB complete dim=3

		#if WINO_OUT_SIZE == 2 && WINO_OUT_SIZE_CELL==4
		if(wino3x3_flag)
		{
		#endif
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
		#if WINO_OUT_SIZE == 2 && WINO_OUT_SIZE_CELL==4
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
		#endif
		// if(cycle == 0)
		// {
			// for(int i=0;i<WINO_DOMAIN_SIZE;i++)
			// {
			// 	for(int j=0;j<WINO_DOMAIN_SIZE;j++)
			// 	{
			// 		printf("%8d ",(int) BtDB[i][j][0] );
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

void load_input_tile(
	ap_int<BTB_WIDTH> input_tile[WINO_WIDTH][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg[WINO_WIDTH][INDEPTH_MINITILE_SIZE]
)
{
	#pragma HLS pipeline
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	for(int w=0;w<WINO_WIDTH;w++){
	#pragma HLS unroll
		for(int iid=0;iid<INDEPTH_MINITILE_SIZE;iid++){
			#pragma HLS unroll
			for(int i=0;i<WINO_DOMAIN_SIZE;i++){
				#pragma HLS unroll
				for(int j=0;j<WINO_DOMAIN_SIZE;j++){
					#pragma HLS unroll
					for(int k=0;k<BATCH_SIZE;k++){
					#pragma HLS unroll
						//if(stream_pingpong_flag)
							input_tile[w][iid][i][j][k]=
							stream_temp_reg[w][iid].range(  ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH+BTB_WIDTH-1, ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH);
					}
				}
			}
		}
	}
}

void load_input_tile_2x2(
	ap_int<BTB_WIDTH> input_tile[WINO_W2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg[WINO_W2][INDEPTH_MINITILE_SIZE]
)
{
	#pragma HLS pipeline
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	for(int w=0;w<WINO_W2;w++){
	#pragma HLS unroll
		for(int iid=0;iid<INDEPTH_MINITILE_SIZE;iid++){
			#pragma HLS unroll
			for(int i=0;i<WINO_DOMAIN_SIZE;i++){
				#pragma HLS unroll
				for(int j=0;j<WINO_DOMAIN_SIZE;j++){
					#pragma HLS unroll
					for(int k=0;k<BATCH_SIZE;k++){
					#pragma HLS unroll
						//if(stream_pingpong_flag)
							input_tile[w][iid][i][j][k]=
							stream_temp_reg[w][iid].range(  ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH+BTB_WIDTH-1, ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH);
					}
				}
			}
		}
	}
}

void load_input_tile_cell(
	ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg[INDEPTH_MINITILE_SIZE]
)
{
	#pragma HLS pipeline

	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1


	for(int iid=0;iid<INDEPTH_MINITILE_SIZE;iid++){
		#pragma HLS unroll
		for(int i=0;i<WINO_DOMAIN_SIZE;i++){
			#pragma HLS unroll
			for(int j=0;j<WINO_DOMAIN_SIZE;j++){
				#pragma HLS unroll
				for(int k=0;k<BATCH_SIZE;k++){
				#pragma HLS unroll
					//if(stream_pingpong_flag)
						input_tile[iid][i][j][k]=
						stream_temp_reg[iid].range(  ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH+BTB_WIDTH-1, ((i*WINO_DOMAIN_SIZE+j)*BATCH_SIZE+k)*BTB_WIDTH);
				}
			}
		}
	}

}

void load_weight_tile_2x2(
		ap_int<W_WIDTH> weight_tile[WINO_H2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_H2]
)
{
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1
		#pragma HLS array_partition variable=weight_value_temp complete 

		for(int wh=0;wh<WINO_H2;wh++)
		{
			#pragma HLS unroll
			for(int id=0;id<INDEPTH_MINITILE_SIZE;id++)
			{
				#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					for(int j=0;j<WINO_DOMAIN_SIZE;j++)
					{
						#pragma HLS unroll
						weight_tile[wh][id][i][j]=weight_value_temp[wh].range(
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH+W_WIDTH-1,
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH);
					}
				}
			}
		}
}

void load_weight_tile(
		ap_int<W_WIDTH> weight_tile[WINO_HEIGHT][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_HEIGHT]
)
{
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1
		#pragma HLS array_partition variable=weight_value_temp complete 

		for(int wh=0;wh<WINO_HEIGHT;wh++)
		{
			#pragma HLS unroll
			for(int id=0;id<INDEPTH_MINITILE_SIZE;id++)
			{
				#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					for(int j=0;j<WINO_DOMAIN_SIZE;j++)
					{
						#pragma HLS unroll
						weight_tile[wh][id][i][j]=weight_value_temp[wh].range(
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH+W_WIDTH-1,
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH);
					}
				}
			}
		}
}


void load_weight_tile_cell(
		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp
)
{
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1



			for(int id=0;id<INDEPTH_MINITILE_SIZE;id++)
			{
				#pragma HLS unroll
				for(int i=0;i<WINO_DOMAIN_SIZE;i++)
				{
					#pragma HLS unroll
					for(int j=0;j<WINO_DOMAIN_SIZE;j++)
					{
						#pragma HLS unroll
						weight_tile[id][i][j]=weight_value_temp.range(
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH+W_WIDTH-1,
							(id*WINO_DOMAIN_SIZE_SQUARE+i*WINO_DOMAIN_SIZE+j)*W_WIDTH);
					}
				}
			}

}


template<int dummy>
void element_wise_mult_6x6(
		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_int<1> ap_clk_div2
)
{
	#pragma HLS pipeline

	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = weight_tile complete dim=3
	#pragma HLS array_partition variable = weight_tile complete dim=2
	#pragma HLS array_partition variable = weight_tile complete dim=1

	for(int wr=0;wr<WINO_DOMAIN_SIZE;wr++)
	{
		#pragma HLS unroll
		for(int wc=0; wc<WINO_DOMAIN_SIZE;wc++)
		{
			#pragma HLS unroll
			for(int id2=0,id=0;id2<INDEPTH_MINITILE_SIZE/2;id2++,id+=2)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
					#pragma HLS unroll
					ap_int<BTB_WIDTH> input_val0=input_tile[id][wr][wc][b];
					ap_int<BTB_WIDTH> input_val1=input_tile[id+1][wr][wc][b];
					UV_MUL_TILE[id2][wr][wc][b]=__builtin_mac16x2(
							input_tile[id][wr][wc][b],
							input_tile[id+1][wr][wc][b],
							weight_tile[id][wr][wc],
							weight_tile[id+1][wr][wc],
							0,1,ap_clk_div2);
				}
			}
		}
	}
}


template<int dummy>
void element_wise_mult_4x4_cell(
		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_int<1> ap_clk_div2
)
{
	#pragma HLS pipeline

	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4




	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = weight_tile complete dim=3
	#pragma HLS array_partition variable = weight_tile complete dim=2
	#pragma HLS array_partition variable = weight_tile complete dim=1

	for(int wr=0;wr<WINO_DOMAIN_SIZE;wr++)
	{
	#pragma HLS unroll
		for(int wc=0; wc<WINO_DOMAIN_SIZE;wc++)
		{
		#pragma HLS unroll
			for(int id2=0,id=0;id2<INDEPTH_MINITILE_SIZE/2;id2++,id+=2)
			{
			#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
				#pragma HLS unroll

					UV_MUL_TILE[id2][wr][wc][b]=weight_tile[id][wr][wc]*input_tile[id][wr][wc][b]+weight_tile[id+1][wr][wc]*input_tile[id+1][wr][wc][b];
					// UV_MUL_TILE[1][id2][wr][wc][b]=__builtin_mac16x2( weight_tile[1][id][wr][wc],weight_tile[1][id+1][wr][wc],input_tile[id][wr][wc][b],input_tile[id+1][wr][wc][b],0,1,ap_clk_div2);
				}
			}
		}
	}
}


template<int dummy>
void element_wise_mult_4x4(
		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE],
		ap_int<W_WIDTH> weight_tile[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE],
		ap_int<1> ap_clk_div2
)
{
	#pragma HLS inline
	#pragma HLS pipeline

	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4
	#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5



	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1

	#pragma HLS array_partition variable = weight_tile complete dim=3
	#pragma HLS array_partition variable = weight_tile complete dim=2
	#pragma HLS array_partition variable = weight_tile complete dim=1
	#pragma HLS array_partition variable = weight_tile complete dim=4

	for(int wr=0;wr<WINO_DOMAIN_SIZE;wr++)
	{
	#pragma HLS unroll
		for(int wc=0; wc<WINO_DOMAIN_SIZE;wc++)
		{
		#pragma HLS unroll
			for(int id2=0,id=0;id2<INDEPTH_MINITILE_SIZE/2;id2++,id+=2)
			{
			#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
				#pragma HLS unroll

					UV_MUL_TILE[1][id2][wr][wc][b]=weight_tile[1][id][wr][wc]*input_tile[id][wr][wc][b]+weight_tile[1][id+1][wr][wc]*input_tile[id+1][wr][wc][b];
					UV_MUL_TILE[0][id2][wr][wc][b]=weight_tile[0][id][wr][wc]*input_tile[id][wr][wc][b]+weight_tile[0][id+1][wr][wc]*input_tile[id+1][wr][wc][b];
					// UV_MUL_TILE[1][id2][wr][wc][b]=__builtin_mac16x2( weight_tile[1][id][wr][wc],weight_tile[1][id+1][wr][wc],input_tile[id][wr][wc][b],input_tile[id+1][wr][wc][b],0,1,ap_clk_div2);
					// UV_MUL_TILE[0][id2][wr][wc][b]=__builtin_mac16x2( weight_tile[0][id][wr][wc],weight_tile[0][id+1][wr][wc],input_tile[id][wr][wc][b],input_tile[id+1][wr][wc][b],0,1,ap_clk_div2);

					// ap_int<48> result=__builtin_mac6x2_mac8x1(
					// 					weight_tile[0][id][wr][wc],weight_tile[0][id+1][wr][wc],
					// 					weight_tile[1][id][wr][wc],weight_tile[1][id+1][wr][wc],	
					// 					input_tile[id][wr][wc][b],	input_tile[id+1][wr][wc][b],
					// 					0,1,ap_clk_div2);
					// UV_MUL_TILE[1][id2][wr][wc][b]=result.range(UV_MUL_WIDTH-1,0);
					// #if __HLS_SYN__
					// UV_MUL_TILE[0][id2][wr][wc][b]=result.range(UV_MUL_WIDTH+19,20)+result[19];
					// #else
					// UV_MUL_TILE[0][id2][wr][wc][b]=result.range(UV_MUL_WIDTH+20,21)+result[20];
					// #endif
				}
			}
		}
	}
}

template<class T>
void load_reg(T &reg, T val)
{
	reg=val;
}


template<class T, int dim1, int dim2, int dim3, int dim4> 
void load_reg_tile4(	T reg[dim1][dim2][dim3][dim4],
				T val[dim1][dim2][dim3][dim4])
{
	#pragma HLS pipeline
	#pragma HLS array_partition variable = reg complete dim=4
	#pragma HLS array_partition variable = reg complete dim=3
	#pragma HLS array_partition variable = reg complete dim=2
	#pragma HLS array_partition variable = reg complete dim=1

	#pragma HLS array_partition variable = val complete dim=4
	#pragma HLS array_partition variable = val complete dim=3
	#pragma HLS array_partition variable = val complete dim=2
	#pragma HLS array_partition variable = val complete dim=1

	for(int d4=0;d4<dim4;d4++){
	#pragma HLS unroll	
	for(int d3=0;d3<dim3;d3++){
	#pragma HLS unroll	
	for(int d2=0;d2<dim2;d2++){
	#pragma HLS unroll	
	for(int d1=0;d1<dim1;d1++){
	#pragma HLS unroll	
		reg[d1][d2][d3][d4]=val[d1][d2][d3][d4];
	}}}}

}


template<class T, int dim1, int dim2, int dim3> 
void load_reg_tile3(	T reg[dim1][dim2][dim3],
				T val[dim1][dim2][dim3])
{
	#pragma HLS pipeline
	#pragma HLS array_partition variable = reg complete dim=3
	#pragma HLS array_partition variable = reg complete dim=2
	#pragma HLS array_partition variable = reg complete dim=1

	#pragma HLS array_partition variable = val complete dim=3
	#pragma HLS array_partition variable = val complete dim=2
	#pragma HLS array_partition variable = val complete dim=1

	
	for(int d3=0;d3<dim3;d3++){
	#pragma HLS unroll	
	for(int d2=0;d2<dim2;d2++){
	#pragma HLS unroll	
	for(int d1=0;d1<dim1;d1++){
	#pragma HLS unroll	
		reg[d1][d2][d3]=val[d1][d2][d3];
	}}}

}

template<int dummy> 
void DSP_LOADER(ap_int<32> &rst, ap_int<16> a0, ap_int<16> a1, ap_int<16> b0, ap_int<16> b1, ap_int<32> accum, ap_int<1> clear, ap_int<1> ap_clk_div2)
{
	rst=__builtin_mac16x2(a0,a1,b0,b1,accum,clear,ap_clk_div2);
}









void wino_stream_cell(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &top_stream_in,
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &bottom_stream_out,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &left_stream_in,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &right_stream_out,
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer0[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer1[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer2[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer3[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif



	#pragma HLS resource variable=out_buffer0 core=RAM_T2P_BRAM 
	#pragma HLS resource variable=out_buffer1 core=RAM_T2P_BRAM 
	#pragma HLS resource variable=out_buffer2 core=RAM_T2P_BRAM 
	#pragma HLS resource variable=out_buffer3 core=RAM_T2P_BRAM 
	
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable=out_buffer0 dim=1 complete
	#pragma HLS array_partition variable=out_buffer1 dim=1 complete
	#pragma HLS array_partition variable=out_buffer2 dim=1 complete
	#pragma HLS array_partition variable=out_buffer3 dim=1 complete


	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1



	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		#pragma HLS unroll
		for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
		{
			#pragma HLS unroll
			stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
		}
		
		top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		bottom_stream_out<<stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	LOOP_MAIN:for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{
		#pragma HLS PIPELINE 

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_cell(input_tile,stream_temp_reg0);
		else
			load_input_tile_cell(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif


		if(stream_pingpong_flag && load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg1[imini_idx]=stream_temp_reg1[imini_idx+1];
			}
	
			
			top_stream_in>>stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out<< stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
		}
		else if(load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
			}
		
			top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out<< stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		}



		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp; 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			
			left_stream_in>>weight_value_temp;
			right_stream_out<<weight_value_temp;
			#if 0
				printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
				for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
				{
					printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
				}
				printf("\n");
			#endif

		}

		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_cell(weight_tile,weight_value_temp);



		ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable = input_tile_reg complete dim=4
		#pragma HLS array_partition variable = input_tile_reg complete dim=3
		#pragma HLS array_partition variable = input_tile_reg complete dim=2
		#pragma HLS array_partition variable = input_tile_reg complete dim=1


		ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];

		#pragma HLS array_partition variable = weight_tile_reg complete dim=3
		#pragma HLS array_partition variable = weight_tile_reg complete dim=2
		#pragma HLS array_partition variable = weight_tile_reg complete dim=1


		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

		load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile);


		#if WINO_DOMAIN_SIZE==6
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
		#else
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile);
		#endif


			
		#if WINO_DOMAIN_SIZE==6
		element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#else
		// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
		// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
		element_wise_mult_4x4_cell<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#endif
		


			

		ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV complete dim=1
		#pragma HLS array_partition variable=UV complete dim=2
		#pragma HLS array_partition variable=UV complete dim=3

		for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
		{
			#pragma HLS unroll
			for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_int<UV_MUL_WIDTH> temp=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
					#else
					ap_int<UV_MUL_WIDTH> temp0=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp0+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
					#endif
				}
			}
		}


		#if DEBUG_FILE_PRINT
			char uvfilename[100];
			#if WINO_DOMAIN_SIZE==6
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
			}
			#else
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
			}
			#endif
		#endif


		ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=UVA complete dim=1
		#pragma HLS array_partition variable=UVA complete dim=2
		#pragma HLS array_partition variable=UVA complete dim=3

		
		for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
		{
			#pragma HLS unroll
			for(int bidx=0;bidx<BATCH_SIZE;bidx++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE==6
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#else
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#endif
			}
		}


		ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=ATA complete dim=1
		#pragma HLS array_partition variable=ATA complete dim=2
		#pragma HLS array_partition variable=ATA complete dim=3

		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		if(wino3x3_flag)
		{
		#endif
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll

				for(int bidx=0;bidx<BATCH_SIZE;bidx++)
				{
					#pragma HLS unroll
					
	
					#if WINO_DOMAIN_SIZE==6
					ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#else

						ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#endif
				}
			}
		
		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		}
		else
		{
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll
				for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						ATA[ridx][cidx][bidx]=UV[ridx][cidx][bidx]*4;
					}

				}
			}
		}
		#endif


			


		ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value complete dim=1
		#pragma HLS array_partition variable=out_value complete dim=2
		#pragma HLS array_partition variable=out_value complete dim=3

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
		load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);


		for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
		{
			#pragma HLS unroll

			ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
			#pragma HLS array_partition variable=data0 complete

			if(idepth_minitile_idx==0 && clear_flag)
			{
				data0[0]=0;
				data0[1]=0;
				data0[2]=0;
				data0[3]=0;
			}
			else
			{
				data0[0]=out_buffer0[c][outbuffer_addr_reg];
				data0[1]=out_buffer1[c][outbuffer_addr_reg];
				data0[2]=out_buffer2[c][outbuffer_addr_reg];
				data0[3]=out_buffer3[c][outbuffer_addr_reg];
			}
			(out_value[0][c][1],out_value[0][c][0])=data0[0];
			(out_value[1][c][1],out_value[1][c][0])=data0[1];
			(out_value[2][c][1],out_value[2][c][0])=data0[2];
			(out_value[3][c][1],out_value[3][c][0])=data0[3];

		}
	


		ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value_back complete dim=1
		#pragma HLS array_partition variable=out_value_back complete dim=2
		#pragma HLS array_partition variable=out_value_back complete dim=3

		for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
		{
			#pragma HLS unroll
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{

					#pragma HLS unroll
					ap_int<ATA_WIDTH+1> sum_sat0;
					
					sum_sat0=ATA[r][c][b]+out_value[r][c][b];

					#if ATA_WIDTH+1 > OUT_WIDTH
						ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
						if(judgebit0 == 0 ||  judgebit0 == -1)
							out_value_back[r][c][b]=sum_sat0;
						else if (sum_sat0[ATA_WIDTH]==1 )
						{
							// std::cout<<"Encountering MIN"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MIN;
						}
						else
						{
							// std::cout<<"Encountering MAX"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MAX;
						}
					#else
						out_value_back[0][r][c][b]=sum_sat0;
						out_value_back[1][r][c][b]=sum_sat1;
					#endif 


				}
			}
		}

		if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
		{
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE == 6
				out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
				#else
				out_buffer0[c][outbuffer_addr_reg]=(out_value_back[0][c][1],out_value_back[0][c][0]);
				out_buffer1[c][outbuffer_addr_reg]=(out_value_back[1][c][1],out_value_back[1][c][0]);
				out_buffer2[c][outbuffer_addr_reg]=(out_value_back[2][c][1],out_value_back[2][c][0]);
				out_buffer3[c][outbuffer_addr_reg]=(out_value_back[3][c][1],out_value_back[3][c][0]);
				#endif
			}
		}
	


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}

}




void wino_stream_cell_right(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &top_stream_in,
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &bottom_stream_out,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &left_stream_in,
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer0[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer1[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer2[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer3[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif




	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete


	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1



	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		#pragma HLS unroll
		for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
		{
			#pragma HLS unroll
			stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
		}
		
		top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		bottom_stream_out<<stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	LOOP_MAIN:for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{
		#pragma HLS PIPELINE 

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_cell(input_tile,stream_temp_reg0);
		else
			load_input_tile_cell(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif


		if(stream_pingpong_flag && load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg1[imini_idx]=stream_temp_reg1[imini_idx+1];
			}
	
			
			top_stream_in>>stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out<< stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
		}
		else if(load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
			}
		
			top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out<< stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		}



		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp; 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			
			left_stream_in>>weight_value_temp;
			#if 0
				printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
				for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
				{
					printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
				}
				printf("\n");
			#endif

		}

		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_cell(weight_tile,weight_value_temp);



		ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable = input_tile_reg complete dim=4
		#pragma HLS array_partition variable = input_tile_reg complete dim=3
		#pragma HLS array_partition variable = input_tile_reg complete dim=2
		#pragma HLS array_partition variable = input_tile_reg complete dim=1


		ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];

		#pragma HLS array_partition variable = weight_tile_reg complete dim=3
		#pragma HLS array_partition variable = weight_tile_reg complete dim=2
		#pragma HLS array_partition variable = weight_tile_reg complete dim=1


		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

		load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile);


		#if WINO_DOMAIN_SIZE==6
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
		#else
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile);
		#endif


			
		#if WINO_DOMAIN_SIZE==6
		element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#else
		// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
		// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
		element_wise_mult_4x4_cell<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#endif
		


			

		ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV complete dim=1
		#pragma HLS array_partition variable=UV complete dim=2
		#pragma HLS array_partition variable=UV complete dim=3

		for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
		{
			#pragma HLS unroll
			for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_int<UV_MUL_WIDTH> temp=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
					#else
					ap_int<UV_MUL_WIDTH> temp0=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp0+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
					#endif
				}
			}
		}


		#if DEBUG_FILE_PRINT
			char uvfilename[100];
			#if WINO_DOMAIN_SIZE==6
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
			}
			#else
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
			}
			#endif
		#endif


		ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=UVA complete dim=1
		#pragma HLS array_partition variable=UVA complete dim=2
		#pragma HLS array_partition variable=UVA complete dim=3

		
		for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
		{
			#pragma HLS unroll
			for(int bidx=0;bidx<BATCH_SIZE;bidx++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE==6
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#else
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#endif
			}
		}


		ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=ATA complete dim=1
		#pragma HLS array_partition variable=ATA complete dim=2
		#pragma HLS array_partition variable=ATA complete dim=3

		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		if(wino3x3_flag)
		{
		#endif
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll

				for(int bidx=0;bidx<BATCH_SIZE;bidx++)
				{
					#pragma HLS unroll
					
	
					#if WINO_DOMAIN_SIZE==6
					ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#else

						ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#endif
				}
			}
		
		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		}
		else
		{
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll
				for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						ATA[ridx][cidx][bidx]=UV[ridx][cidx][bidx]*4;
					}

				}
			}
		}
		#endif


			


		ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value complete dim=1
		#pragma HLS array_partition variable=out_value complete dim=2
		#pragma HLS array_partition variable=out_value complete dim=3

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
		load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);


		for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
		{
			#pragma HLS unroll

			ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
			#pragma HLS array_partition variable=data0 complete

			if(idepth_minitile_idx==0 && clear_flag)
			{
				data0[0]=0;
				data0[1]=0;
				data0[2]=0;
				data0[3]=0;
			}
			else
			{
				data0[0]=out_buffer0[c][outbuffer_addr_reg];
				data0[1]=out_buffer1[c][outbuffer_addr_reg];
				data0[2]=out_buffer2[c][outbuffer_addr_reg];
				data0[3]=out_buffer3[c][outbuffer_addr_reg];
			}
			(out_value[0][c][1],out_value[0][c][0])=data0[0];
			(out_value[1][c][1],out_value[1][c][0])=data0[1];
			(out_value[2][c][1],out_value[2][c][0])=data0[2];
			(out_value[3][c][1],out_value[3][c][0])=data0[3];

		}
	


		ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value_back complete dim=1
		#pragma HLS array_partition variable=out_value_back complete dim=2
		#pragma HLS array_partition variable=out_value_back complete dim=3

		for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
		{
			#pragma HLS unroll
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{

					#pragma HLS unroll
					ap_int<ATA_WIDTH+1> sum_sat0;
					
					sum_sat0=ATA[r][c][b]+out_value[r][c][b];

					#if ATA_WIDTH+1 > OUT_WIDTH
						ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
						if(judgebit0 == 0 ||  judgebit0 == -1)
							out_value_back[r][c][b]=sum_sat0;
						else if (sum_sat0[ATA_WIDTH]==1 )
						{
							// std::cout<<"Encountering MIN"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MIN;
						}
						else
						{
							// std::cout<<"Encountering MAX"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MAX;
						}
					#else
						out_value_back[0][r][c][b]=sum_sat0;
						out_value_back[1][r][c][b]=sum_sat1;
					#endif 


				}
			}
		}

		if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
		{

			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE == 6
				out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
				#else
				out_buffer0[c][outbuffer_addr_reg]=(out_value_back[0][c][1],out_value_back[0][c][0]);
				out_buffer1[c][outbuffer_addr_reg]=(out_value_back[1][c][1],out_value_back[1][c][0]);
				out_buffer2[c][outbuffer_addr_reg]=(out_value_back[2][c][1],out_value_back[2][c][0]);
				out_buffer3[c][outbuffer_addr_reg]=(out_value_back[3][c][1],out_value_back[3][c][0]);
				#endif
			}

		}
	


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}

}



void wino_stream_cell_bottom(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &top_stream_in,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &left_stream_in,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &right_stream_out,
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer0[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer1[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer2[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer3[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{

	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif




	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete


	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1



	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		#pragma HLS unroll
		for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
		{
			#pragma HLS unroll
			stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
		}
		
		top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	LOOP_MAIN:for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{
		#pragma HLS PIPELINE 

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_cell(input_tile,stream_temp_reg0);
		else
			load_input_tile_cell(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif


		if(stream_pingpong_flag && load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg1[imini_idx]=stream_temp_reg1[imini_idx+1];
			}
	
			
			top_stream_in>>stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
		}
		else if(load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
			}
		
			top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		}



		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp; 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			
			left_stream_in>>weight_value_temp;
			right_stream_out<<weight_value_temp;
			#if 0
				printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
				for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
				{
					printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
				}
				printf("\n");
			#endif

		}

		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_cell(weight_tile,weight_value_temp);



		ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable = input_tile_reg complete dim=4
		#pragma HLS array_partition variable = input_tile_reg complete dim=3
		#pragma HLS array_partition variable = input_tile_reg complete dim=2
		#pragma HLS array_partition variable = input_tile_reg complete dim=1


		ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];

		#pragma HLS array_partition variable = weight_tile_reg complete dim=3
		#pragma HLS array_partition variable = weight_tile_reg complete dim=2
		#pragma HLS array_partition variable = weight_tile_reg complete dim=1


		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

		load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile);


		#if WINO_DOMAIN_SIZE==6
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
		#else
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile);
		#endif


			
		#if WINO_DOMAIN_SIZE==6
		element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#else
		// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
		// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
		element_wise_mult_4x4_cell<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#endif
		


			

		ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV complete dim=1
		#pragma HLS array_partition variable=UV complete dim=2
		#pragma HLS array_partition variable=UV complete dim=3

		for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
		{
			#pragma HLS unroll
			for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_int<UV_MUL_WIDTH> temp=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
					#else
					ap_int<UV_MUL_WIDTH> temp0=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp0+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
					#endif
				}
			}
		}


		#if DEBUG_FILE_PRINT
			char uvfilename[100];
			#if WINO_DOMAIN_SIZE==6
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
			}
			#else
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
			}
			#endif
		#endif


		ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=UVA complete dim=1
		#pragma HLS array_partition variable=UVA complete dim=2
		#pragma HLS array_partition variable=UVA complete dim=3

		
		for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
		{
			#pragma HLS unroll
			for(int bidx=0;bidx<BATCH_SIZE;bidx++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE==6
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#else
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#endif
			}
		}


		ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=ATA complete dim=1
		#pragma HLS array_partition variable=ATA complete dim=2
		#pragma HLS array_partition variable=ATA complete dim=3

		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		if(wino3x3_flag)
		{
		#endif
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll

				for(int bidx=0;bidx<BATCH_SIZE;bidx++)
				{
					#pragma HLS unroll
					
	
					#if WINO_DOMAIN_SIZE==6
					ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#else

						ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#endif
				}
			}
		
		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		}
		else
		{
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll
				for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						ATA[ridx][cidx][bidx]=UV[ridx][cidx][bidx]*4;
					}

				}
			}
		}
		#endif


			


		ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value complete dim=1
		#pragma HLS array_partition variable=out_value complete dim=2
		#pragma HLS array_partition variable=out_value complete dim=3

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
		load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);


		for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
		{
			#pragma HLS unroll

			ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
			#pragma HLS array_partition variable=data0 complete

			if(idepth_minitile_idx==0 && clear_flag)
			{
				data0[0]=0;
				data0[1]=0;
				data0[2]=0;
				data0[3]=0;
			}
			else
			{
				data0[0]=out_buffer0[c][outbuffer_addr_reg];
				data0[1]=out_buffer1[c][outbuffer_addr_reg];
				data0[2]=out_buffer2[c][outbuffer_addr_reg];
				data0[3]=out_buffer3[c][outbuffer_addr_reg];
			}
			(out_value[0][c][1],out_value[0][c][0])=data0[0];
			(out_value[1][c][1],out_value[1][c][0])=data0[1];
			(out_value[2][c][1],out_value[2][c][0])=data0[2];
			(out_value[3][c][1],out_value[3][c][0])=data0[3];

		}
	


		ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value_back complete dim=1
		#pragma HLS array_partition variable=out_value_back complete dim=2
		#pragma HLS array_partition variable=out_value_back complete dim=3

		for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
		{
			#pragma HLS unroll
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{

					#pragma HLS unroll
					ap_int<ATA_WIDTH+1> sum_sat0;
					
					sum_sat0=ATA[r][c][b]+out_value[r][c][b];

					#if ATA_WIDTH+1 > OUT_WIDTH
						ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
						if(judgebit0 == 0 ||  judgebit0 == -1)
							out_value_back[r][c][b]=sum_sat0;
						else if (sum_sat0[ATA_WIDTH]==1 )
						{
							// std::cout<<"Encountering MIN"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MIN;
						}
						else
						{
							// std::cout<<"Encountering MAX"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MAX;
						}
					#else
						out_value_back[0][r][c][b]=sum_sat0;
						out_value_back[1][r][c][b]=sum_sat1;
					#endif 


				}
			}
		}

		if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
		{
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE == 6
				out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
				#else
				out_buffer0[c][outbuffer_addr_reg]=(out_value_back[0][c][1],out_value_back[0][c][0]);
				out_buffer1[c][outbuffer_addr_reg]=(out_value_back[1][c][1],out_value_back[1][c][0]);
				out_buffer2[c][outbuffer_addr_reg]=(out_value_back[2][c][1],out_value_back[2][c][0]);
				out_buffer3[c][outbuffer_addr_reg]=(out_value_back[3][c][1],out_value_back[3][c][0]);
				#endif
			}
		}
	


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}

}




void wino_stream_cell_corner(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &top_stream_in,
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > &left_stream_in,
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer0[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer1[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer2[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer3[WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif




	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete


	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1



	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		#pragma HLS unroll
		for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
		{
			#pragma HLS unroll
			stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
		}
		
		top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	LOOP_MAIN:for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{
		#pragma HLS PIPELINE 

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_cell(input_tile,stream_temp_reg0);
		else
			load_input_tile_cell(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif


		if(stream_pingpong_flag && load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg1[imini_idx]=stream_temp_reg1[imini_idx+1];
			}
	
			
			top_stream_in>>stream_temp_reg1[INDEPTH_MINITILE_SIZE-1];
		}
		else if(load_input_flag_reg)
		{

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[imini_idx]=stream_temp_reg0[imini_idx+1];
			}
		
			top_stream_in>>stream_temp_reg0[INDEPTH_MINITILE_SIZE-1];
		}



		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp; 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			
			left_stream_in>>weight_value_temp;
			#if 0
				printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
				for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
				{
					printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
				}
				printf("\n");
			#endif

		}

		ap_int<W_WIDTH> weight_tile[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_cell(weight_tile,weight_value_temp);



		ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable = input_tile_reg complete dim=4
		#pragma HLS array_partition variable = input_tile_reg complete dim=3
		#pragma HLS array_partition variable = input_tile_reg complete dim=2
		#pragma HLS array_partition variable = input_tile_reg complete dim=1


		ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];

		#pragma HLS array_partition variable = weight_tile_reg complete dim=3
		#pragma HLS array_partition variable = weight_tile_reg complete dim=2
		#pragma HLS array_partition variable = weight_tile_reg complete dim=1


		ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
		#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

		load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile);


		#if WINO_DOMAIN_SIZE==6
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
		#else
		load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile);
		#endif


			
		#if WINO_DOMAIN_SIZE==6
		element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#else
		// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
		// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
		element_wise_mult_4x4_cell<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
		#endif
		


			

		ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
		#pragma HLS array_partition variable=UV complete dim=1
		#pragma HLS array_partition variable=UV complete dim=2
		#pragma HLS array_partition variable=UV complete dim=3

		for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
		{
			#pragma HLS unroll
			for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_int<UV_MUL_WIDTH> temp=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
					#else
					ap_int<UV_MUL_WIDTH> temp0=0;
					for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
					{
						#pragma HLS unroll
						temp0+=UV_MUL_TILE[id2][wino_row][wino_col][b];
					}
					UV[wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
					#endif
				}
			}
		}


		#if DEBUG_FILE_PRINT
			char uvfilename[100];
			#if WINO_DOMAIN_SIZE==6
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
			}
			#else
			
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
			{
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
				sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
				attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
			}
			#endif
		#endif


		ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=UVA complete dim=1
		#pragma HLS array_partition variable=UVA complete dim=2
		#pragma HLS array_partition variable=UVA complete dim=3

		
		for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
		{
			#pragma HLS unroll
			for(int bidx=0;bidx<BATCH_SIZE;bidx++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE==6
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#else
				UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
				#endif
			}
		}


		ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=ATA complete dim=1
		#pragma HLS array_partition variable=ATA complete dim=2
		#pragma HLS array_partition variable=ATA complete dim=3

		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		if(wino3x3_flag)
		{
		#endif
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll

				for(int bidx=0;bidx<BATCH_SIZE;bidx++)
				{
					#pragma HLS unroll
					
	
					#if WINO_DOMAIN_SIZE==6
					ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#else

						ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
					#endif
				}
			}
		
		#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
		}
		else
		{
			for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
			{
				#pragma HLS unroll
				for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						ATA[ridx][cidx][bidx]=UV[ridx][cidx][bidx]*4;
					}

				}
			}
		}
		#endif


			


		ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value complete dim=1
		#pragma HLS array_partition variable=out_value complete dim=2
		#pragma HLS array_partition variable=out_value complete dim=3

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
		load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);


		for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
		{
			#pragma HLS unroll

			ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
			#pragma HLS array_partition variable=data0 complete

			if(idepth_minitile_idx==0 && clear_flag)
			{
				data0[0]=0;
				data0[1]=0;
				data0[2]=0;
				data0[3]=0;
			}
			else
			{
				data0[0]=out_buffer0[c][outbuffer_addr_reg];
				data0[1]=out_buffer1[c][outbuffer_addr_reg];
				data0[2]=out_buffer2[c][outbuffer_addr_reg];
				data0[3]=out_buffer3[c][outbuffer_addr_reg];
			}
			(out_value[0][c][1],out_value[0][c][0])=data0[0];
			(out_value[1][c][1],out_value[1][c][0])=data0[1];
			(out_value[2][c][1],out_value[2][c][0])=data0[2];
			(out_value[3][c][1],out_value[3][c][0])=data0[3];

		}
	


		ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
		#pragma HLS array_partition variable=out_value_back complete dim=1
		#pragma HLS array_partition variable=out_value_back complete dim=2
		#pragma HLS array_partition variable=out_value_back complete dim=3

		for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
		{
			#pragma HLS unroll
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				for(int b=0;b<BATCH_SIZE;b++)
				{

					#pragma HLS unroll
					ap_int<ATA_WIDTH+1> sum_sat0;
					
					sum_sat0=ATA[r][c][b]+out_value[r][c][b];

					#if ATA_WIDTH+1 > OUT_WIDTH
						ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
						if(judgebit0 == 0 ||  judgebit0 == -1)
							out_value_back[r][c][b]=sum_sat0;
						else if (sum_sat0[ATA_WIDTH]==1 )
						{
							// std::cout<<"Encountering MIN"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MIN;
						}
						else
						{
							// std::cout<<"Encountering MAX"<<std::endl;
							out_value_back[r][c][b]=OUT_SAT_MAX;
						}
					#else
						out_value_back[0][r][c][b]=sum_sat0;
						out_value_back[1][r][c][b]=sum_sat1;
					#endif 


				}
			}
		}

		if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
		{
			for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
			{
				#pragma HLS unroll
				#if WINO_DOMAIN_SIZE == 6
				out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
				#else
				out_buffer0[c][outbuffer_addr_reg]=(out_value_back[0][c][1],out_value_back[0][c][0]);
				out_buffer1[c][outbuffer_addr_reg]=(out_value_back[1][c][1],out_value_back[1][c][0]);
				out_buffer2[c][outbuffer_addr_reg]=(out_value_back[2][c][1],out_value_back[2][c][0]);
				out_buffer3[c][outbuffer_addr_reg]=(out_value_back[3][c][1],out_value_back[3][c][0]);
				#endif
			}
		}
	


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}

}





void winoPEB_CORN(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_W2],
		// hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > bottom_stream_out[WINO_W2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WINO_H2],
		// hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > right_stream_out[WEIGHT_PORT_NUM][WINO_H2],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_1[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_2[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_3[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag



	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer0 dim=2 complete
	#pragma HLS array_partition variable = out_buffer0 dim=3 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=2 complete
	#pragma HLS array_partition variable = out_buffer1 dim=3 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=2 complete
	#pragma HLS array_partition variable = out_buffer2 dim=3 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=2 complete
	#pragma HLS array_partition variable = out_buffer3 dim=3 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_W2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			// bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_2x2(input_tile,stream_temp_reg0);
		else
			load_input_tile_2x2(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				// bottom_stream_out[wino_array_col]<<stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				// bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_H2];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			for(int j=0;j<WINO_H2;j++)
			{
				#pragma HLS unroll
				left_stream_in[j]>>weight_value_temp[j];
				// right_stream_out[i][j]<<weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j];
				#if 0
					printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
					for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
					{
						printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
					}
					printf("\n");
				#endif
			}
	
		}

		ap_int<W_WIDTH> weight_tile[WINO_H2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_2x2(weight_tile,weight_value_temp);

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_w2=0;wino_w2<WINO_W2;wino_w2++)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_w2] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[0]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("[%4d %4d]", (int) input_tile_reg[nn][i][j][0],(int) input_tile_reg[nn][i][j][1]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
					// }
					// std::cout<<"weight tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }
					// printf("\n");
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// //TODO: print input_tile_reg
				// if( wino_array_idx%WINO_HEIGHT==2)
				// {
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	//TODO: print weight_tile
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}

				// 	getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_uint<OUT_WIDTH*BATCH_SIZE> data;
					if(idepth_minitile_idx==0)
						data=0;
					else
						data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
					(out_value[r][c][1],out_value[r][c][0])=data;
					#else
					ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
					#pragma HLS array_partition variable=data0 factor=4
					ap_uint<OUT_WIDTH*BATCH_SIZE> data1[4];
					#pragma HLS array_partition variable=data1 factor=4
					if(idepth_minitile_idx==0 && clear_flag)
					{
						data0[0]=0;
						data0[1]=0;
						data0[2]=0;
						data0[3]=0;
						data1[0]=0;
						data1[1]=0;
						data1[2]=0;
						data1[3]=0;
					}
					else
					{
						data0[0]=out_buffer_0[0][wino_w2][c][outbuffer_addr_reg];
						data0[1]=out_buffer_1[0][wino_w2][c][outbuffer_addr_reg];
						data0[2]=out_buffer_2[0][wino_w2][c][outbuffer_addr_reg];
						data0[3]=out_buffer_3[0][wino_w2][c][outbuffer_addr_reg];
						data1[0]=out_buffer_0[1][wino_w2][c][outbuffer_addr_reg];
						data1[1]=out_buffer_1[1][wino_w2][c][outbuffer_addr_reg];
						data1[2]=out_buffer_2[1][wino_w2][c][outbuffer_addr_reg];
						data1[3]=out_buffer_3[1][wino_w2][c][outbuffer_addr_reg];
					
					}
					(out_value[0][0][c][1],out_value[0][0][c][0])=data0[0];
					(out_value[0][1][c][1],out_value[0][1][c][0])=data0[1];
					(out_value[0][2][c][1],out_value[0][2][c][0])=data0[2];
					(out_value[0][3][c][1],out_value[0][3][c][0])=data0[3];
					(out_value[1][0][c][1],out_value[1][0][c][0])=data1[0];
					(out_value[1][1][c][1],out_value[1][1][c][0])=data1[1];
					(out_value[1][2][c][1],out_value[1][2][c][0])=data1[2];
					(out_value[1][3][c][1],out_value[1][3][c][0])=data1[3];
					#endif
				}
			

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}
				// if(wino_array_idx/WINO_HEIGHT==6 && cycle==0)
				// {
					// std::cout<<"Out addr "<< outbuffer_addr_reg <<std::endl;
					// std::cout<<"idepth_minitile_idx "<< idepth_minitile_idx <<std::endl;
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }


					
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[1][r][c][0],(int) out_value_back[1][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer_0[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][0][c][1],out_value_back[0][0][c][0]);
							out_buffer_1[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][1][c][1],out_value_back[0][1][c][0]);
							out_buffer_2[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][2][c][1],out_value_back[0][2][c][0]);
							out_buffer_3[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][3][c][1],out_value_back[0][3][c][0]);
							out_buffer_0[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][0][c][1],out_value_back[1][0][c][0]);
							out_buffer_1[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][1][c][1],out_value_back[1][1][c][0]);
							out_buffer_2[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][2][c][1],out_value_back[1][2][c][0]);
							out_buffer_3[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][3][c][1],out_value_back[1][3][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}



void winoPEB_EDG(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_W2],
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > bottom_stream_out[WINO_W2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WINO_H2],
		// hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > right_stream_out[WEIGHT_PORT_NUM][WINO_H2],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_1[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_2[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_3[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag

	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer0 dim=2 complete
	#pragma HLS array_partition variable = out_buffer0 dim=3 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=2 complete
	#pragma HLS array_partition variable = out_buffer1 dim=3 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=2 complete
	#pragma HLS array_partition variable = out_buffer2 dim=3 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=2 complete
	#pragma HLS array_partition variable = out_buffer3 dim=3 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_W2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_2x2(input_tile,stream_temp_reg0);
		else
			load_input_tile_2x2(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				bottom_stream_out[wino_array_col]<<stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_H2];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			for(int j=0;j<WINO_H2;j++)
			{
				#pragma HLS unroll
				left_stream_in[j]>>weight_value_temp[j];
				// right_stream_out[i][j]<<weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j];
				#if 0
					printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
					for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
					{
						printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
					}
					printf("\n");
				#endif
			}
	
		}

		ap_int<W_WIDTH> weight_tile[WINO_H2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_2x2(weight_tile,weight_value_temp);

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_w2=0;wino_w2<WINO_W2;wino_w2++)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_w2] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[0]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("%4d ", (int) input_tile_reg[nn][i][j][0]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
	
					// }
					// std::cout<<"weight tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// //TODO: print input_tile_reg
				// if( wino_array_idx%WINO_HEIGHT==2)
				// {
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	//TODO: print weight_tile
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}

				// 	getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_uint<OUT_WIDTH*BATCH_SIZE> data;
					if(idepth_minitile_idx==0)
						data=0;
					else
						data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
					(out_value[r][c][1],out_value[r][c][0])=data;
					#else
					ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
					#pragma HLS array_partition variable=data0 factor=4
					ap_uint<OUT_WIDTH*BATCH_SIZE> data1[4];
					#pragma HLS array_partition variable=data1 factor=4
					if(idepth_minitile_idx==0 && clear_flag)
					{
						data0[0]=0;
						data0[1]=0;
						data0[2]=0;
						data0[3]=0;
						data1[0]=0;
						data1[1]=0;
						data1[2]=0;
						data1[3]=0;
					}
					else
					{
						data0[0]=out_buffer_0[0][wino_w2][c][outbuffer_addr_reg];
						data0[1]=out_buffer_1[0][wino_w2][c][outbuffer_addr_reg];
						data0[2]=out_buffer_2[0][wino_w2][c][outbuffer_addr_reg];
						data0[3]=out_buffer_3[0][wino_w2][c][outbuffer_addr_reg];
						data1[0]=out_buffer_0[1][wino_w2][c][outbuffer_addr_reg];
						data1[1]=out_buffer_1[1][wino_w2][c][outbuffer_addr_reg];
						data1[2]=out_buffer_2[1][wino_w2][c][outbuffer_addr_reg];
						data1[3]=out_buffer_3[1][wino_w2][c][outbuffer_addr_reg];
					
					}
					(out_value[0][0][c][1],out_value[0][0][c][0])=data0[0];
					(out_value[0][1][c][1],out_value[0][1][c][0])=data0[1];
					(out_value[0][2][c][1],out_value[0][2][c][0])=data0[2];
					(out_value[0][3][c][1],out_value[0][3][c][0])=data0[3];
					(out_value[1][0][c][1],out_value[1][0][c][0])=data1[0];
					(out_value[1][1][c][1],out_value[1][1][c][0])=data1[1];
					(out_value[1][2][c][1],out_value[1][2][c][0])=data1[2];
					(out_value[1][3][c][1],out_value[1][3][c][0])=data1[3];
					#endif
				}
			

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}
				// if(wino_array_idx/WINO_HEIGHT==6 && cycle==0)
				// {
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer_0[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][0][c][1],out_value_back[0][0][c][0]);
							out_buffer_1[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][1][c][1],out_value_back[0][1][c][0]);
							out_buffer_2[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][2][c][1],out_value_back[0][2][c][0]);
							out_buffer_3[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][3][c][1],out_value_back[0][3][c][0]);
							out_buffer_0[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][0][c][1],out_value_back[1][0][c][0]);
							out_buffer_1[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][1][c][1],out_value_back[1][1][c][0]);
							out_buffer_2[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][2][c][1],out_value_back[1][2][c][0]);
							out_buffer_3[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][3][c][1],out_value_back[1][3][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}



void winoPEB_BOT(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_W2],
		// hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > bottom_stream_out[WINO_W2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WINO_H2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > right_stream_out[WINO_H2],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_1[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_2[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_3[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer0 dim=2 complete
	#pragma HLS array_partition variable = out_buffer0 dim=3 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=2 complete
	#pragma HLS array_partition variable = out_buffer1 dim=3 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=2 complete
	#pragma HLS array_partition variable = out_buffer2 dim=3 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=2 complete
	#pragma HLS array_partition variable = out_buffer3 dim=3 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_W2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			// bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_2x2(input_tile,stream_temp_reg0);
		else
			load_input_tile_2x2(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				// bottom_stream_out[wino_array_col]<<stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				// bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_H2];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			for(int j=0;j<WINO_H2;j++)
			{
				#pragma HLS unroll
				left_stream_in[j]>>weight_value_temp[j];
				right_stream_out[j]<<weight_value_temp[j];
				#if 0
					printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
					for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
					{
						printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
					}
					printf("\n");
				#endif
			}
	
		}

		ap_int<W_WIDTH> weight_tile[WINO_H2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_2x2(weight_tile,weight_value_temp);

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_w2=0;wino_w2<WINO_W2;wino_w2++)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_w2] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[0]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("%4d ", (int) input_tile_reg[nn][i][j][0]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
	
					// }
					// std::cout<<"weight tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// //TODO: print input_tile_reg
				// if( wino_array_idx%WINO_HEIGHT==2)
				// {
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	//TODO: print weight_tile
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}

				// 	getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_uint<OUT_WIDTH*BATCH_SIZE> data;
					if(idepth_minitile_idx==0)
						data=0;
					else
						data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
					(out_value[r][c][1],out_value[r][c][0])=data;
					#else
					ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
					#pragma HLS array_partition variable=data0 factor=4
					ap_uint<OUT_WIDTH*BATCH_SIZE> data1[4];
					#pragma HLS array_partition variable=data1 factor=4
					if(idepth_minitile_idx==0 && clear_flag)
					{
						data0[0]=0;
						data0[1]=0;
						data0[2]=0;
						data0[3]=0;
						data1[0]=0;
						data1[1]=0;
						data1[2]=0;
						data1[3]=0;
					}
					else
					{
						data0[0]=out_buffer_0[0][wino_w2][c][outbuffer_addr_reg];
						data0[1]=out_buffer_1[0][wino_w2][c][outbuffer_addr_reg];
						data0[2]=out_buffer_2[0][wino_w2][c][outbuffer_addr_reg];
						data0[3]=out_buffer_3[0][wino_w2][c][outbuffer_addr_reg];
						data1[0]=out_buffer_0[1][wino_w2][c][outbuffer_addr_reg];
						data1[1]=out_buffer_1[1][wino_w2][c][outbuffer_addr_reg];
						data1[2]=out_buffer_2[1][wino_w2][c][outbuffer_addr_reg];
						data1[3]=out_buffer_3[1][wino_w2][c][outbuffer_addr_reg];
					
					}
					(out_value[0][0][c][1],out_value[0][0][c][0])=data0[0];
					(out_value[0][1][c][1],out_value[0][1][c][0])=data0[1];
					(out_value[0][2][c][1],out_value[0][2][c][0])=data0[2];
					(out_value[0][3][c][1],out_value[0][3][c][0])=data0[3];
					(out_value[1][0][c][1],out_value[1][0][c][0])=data1[0];
					(out_value[1][1][c][1],out_value[1][1][c][0])=data1[1];
					(out_value[1][2][c][1],out_value[1][2][c][0])=data1[2];
					(out_value[1][3][c][1],out_value[1][3][c][0])=data1[3];
					#endif
				}
			

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}
				// if(wino_array_idx/WINO_HEIGHT==6 && cycle==0)
				// {
					// std::cout<<"Out addr "<< outbuffer_addr_reg <<std::endl;
					// std::cout<<"idepth_minitile_idx "<< idepth_minitile_idx <<std::endl;

					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer_0[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][0][c][1],out_value_back[0][0][c][0]);
							out_buffer_1[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][1][c][1],out_value_back[0][1][c][0]);
							out_buffer_2[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][2][c][1],out_value_back[0][2][c][0]);
							out_buffer_3[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][3][c][1],out_value_back[0][3][c][0]);
							out_buffer_0[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][0][c][1],out_value_back[1][0][c][0]);
							out_buffer_1[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][1][c][1],out_value_back[1][1][c][0]);
							out_buffer_2[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][2][c][1],out_value_back[1][2][c][0]);
							out_buffer_3[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][3][c][1],out_value_back[1][3][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}



void winoPEB_CENT(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_W2],
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > bottom_stream_out[WINO_W2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WINO_H2],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > right_stream_out[WINO_H2],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_1[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_2[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer_3[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer0 dim=1 complete
	#pragma HLS array_partition variable = out_buffer0 dim=2 complete
	#pragma HLS array_partition variable = out_buffer0 dim=3 complete
	#pragma HLS array_partition variable = out_buffer1 dim=1 complete
	#pragma HLS array_partition variable = out_buffer1 dim=2 complete
	#pragma HLS array_partition variable = out_buffer1 dim=3 complete
	#pragma HLS array_partition variable = out_buffer2 dim=1 complete
	#pragma HLS array_partition variable = out_buffer2 dim=2 complete
	#pragma HLS array_partition variable = out_buffer2 dim=3 complete
	#pragma HLS array_partition variable = out_buffer3 dim=1 complete
	#pragma HLS array_partition variable = out_buffer3 dim=2 complete
	#pragma HLS array_partition variable = out_buffer3 dim=3 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_W2][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_W2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile_2x2(input_tile,stream_temp_reg0);
		else
			load_input_tile_2x2(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_W2;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				bottom_stream_out[wino_array_col]<<stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
				bottom_stream_out[wino_array_col]<<stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_H2];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{

			for(int j=0;j<WINO_H2;j++)
			{
				#pragma HLS unroll
				left_stream_in[j]>>weight_value_temp[j];
				right_stream_out[j]<<weight_value_temp[j];
				#if 0
					printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
					for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
					{
						printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
					}
					printf("\n");
				#endif
			}
		}

		ap_int<W_WIDTH> weight_tile[WINO_H2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile_2x2(weight_tile,weight_value_temp);

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_w2=0;wino_w2<WINO_W2;wino_w2++)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_w2] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[0]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("%4d ", (int) input_tile_reg[nn][i][j][0]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
	
					// }
					// std::cout<<"weight tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// //TODO: print input_tile_reg
				// if( wino_array_idx%WINO_HEIGHT==2)
				// {
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	//TODO: print weight_tile
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}

				// 	getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
				{
					#pragma HLS unroll
					#if WINO_DOMAIN_SIZE==6
					ap_uint<OUT_WIDTH*BATCH_SIZE> data;
					if(idepth_minitile_idx==0)
						data=0;
					else
						data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
					(out_value[r][c][1],out_value[r][c][0])=data;
					#else
					ap_uint<OUT_WIDTH*BATCH_SIZE> data0[4];
					#pragma HLS array_partition variable=data0 factor=4
					ap_uint<OUT_WIDTH*BATCH_SIZE> data1[4];
					#pragma HLS array_partition variable=data1 factor=4
					if(idepth_minitile_idx==0 && clear_flag)
					{
						data0[0]=0;
						data0[1]=0;
						data0[2]=0;
						data0[3]=0;
						data1[0]=0;
						data1[1]=0;
						data1[2]=0;
						data1[3]=0;
					}
					else
					{
						data0[0]=out_buffer_0[0][wino_w2][c][outbuffer_addr_reg];
						data0[1]=out_buffer_1[0][wino_w2][c][outbuffer_addr_reg];
						data0[2]=out_buffer_2[0][wino_w2][c][outbuffer_addr_reg];
						data0[3]=out_buffer_3[0][wino_w2][c][outbuffer_addr_reg];
						data1[0]=out_buffer_0[1][wino_w2][c][outbuffer_addr_reg];
						data1[1]=out_buffer_1[1][wino_w2][c][outbuffer_addr_reg];
						data1[2]=out_buffer_2[1][wino_w2][c][outbuffer_addr_reg];
						data1[3]=out_buffer_3[1][wino_w2][c][outbuffer_addr_reg];
					
					}
					(out_value[0][0][c][1],out_value[0][0][c][0])=data0[0];
					(out_value[0][1][c][1],out_value[0][1][c][0])=data0[1];
					(out_value[0][2][c][1],out_value[0][2][c][0])=data0[2];
					(out_value[0][3][c][1],out_value[0][3][c][0])=data0[3];
					(out_value[1][0][c][1],out_value[1][0][c][0])=data1[0];
					(out_value[1][1][c][1],out_value[1][1][c][0])=data1[1];
					(out_value[1][2][c][1],out_value[1][2][c][0])=data1[2];
					(out_value[1][3][c][1],out_value[1][3][c][0])=data1[3];
					#endif
				}
			

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}
				// if(wino_array_idx/WINO_HEIGHT==6 && cycle==0)
				// {
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer_0[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][0][c][1],out_value_back[0][0][c][0]);
							out_buffer_1[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][1][c][1],out_value_back[0][1][c][0]);
							out_buffer_2[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][2][c][1],out_value_back[0][2][c][0]);
							out_buffer_3[0][wino_w2][c][outbuffer_addr_reg]=(out_value_back[0][3][c][1],out_value_back[0][3][c][0]);
							out_buffer_0[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][0][c][1],out_value_back[1][0][c][0]);
							out_buffer_1[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][1][c][1],out_value_back[1][1][c][0]);
							out_buffer_2[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][2][c][1],out_value_back[1][2][c][0]);
							out_buffer_3[1][wino_w2][c][outbuffer_addr_reg]=(out_value_back[1][3][c][1],out_value_back[1][3][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}



void wino_stream_block2(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_WIDTH],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WEIGHT_PORT_NUM][WEIGHT_FEED_NUMBER_PER_PORT],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/WINO_H2][WINO_WIDTH/WINO_W2][WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer dim=1 complete
	#pragma HLS array_partition variable = out_buffer dim=2 complete
	#pragma HLS array_partition variable = out_buffer dim=3 complete
	#pragma HLS array_partition variable = out_buffer dim=4 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_WIDTH][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_WIDTH][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_WIDTH][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}

	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;


	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer0 inter false
		#pragma HLS dependence variable=out_buffer1 inter false
		#pragma HLS dependence variable=out_buffer2 inter false
		#pragma HLS dependence variable=out_buffer3 inter false
		#pragma HLS dependence variable=out_buffer0 intra false
		#pragma HLS dependence variable=out_buffer1 intra false
		#pragma HLS dependence variable=out_buffer2 intra false
		#pragma HLS dependence variable=out_buffer3 intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile(input_tile,stream_temp_reg0);
		else
			load_input_tile(input_tile,stream_temp_reg1);

		




		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_HEIGHT];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{
			for(int i=0;i<WEIGHT_PORT_NUM;i++)
			{
				#pragma HLS unroll
				for(int j=0;j<WEIGHT_FEED_NUMBER_PER_PORT;j++)
				{
					#pragma HLS unroll
					left_stream_in[i][j]>>weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j];
					#if 0
						printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
						for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
						{
							printf("[%d]", (int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
						}
						printf("\n");
						getchar();
					#endif
				}
			}
		}

		ap_int<W_WIDTH> weight_tile[WINO_HEIGHT][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile(weight_tile,weight_value_temp);


		// if(cycle==0)
		// {
		// 	std::cout<<"weight tile in depth" <<std::endl;
		// 	for(int mm=0;mm<WINO_HEIGHT;mm++)
		// 	for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
		// 	{
		// 		for(int i=0;i<4;i++)
		// 		{
		// 			for(int j=0;j<4;j++)
		// 			{
		// 				printf("%4d ", (int) weight_tile[mm][0][i][j]);
		// 			}
		// 			printf("\n");
		// 		}
		// 		printf("\n");
		// 	}
			
		// // 	// input_tile[WINO_WIDTH][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
			
		// 	getchar();
		// 	std::cout<<"input tile in depth" <<std::endl;
		// 	for(int mm=0;mm<WINO_WIDTH;mm++)
		// 	{
		// 		for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
		// 		{
		// 			for(int i=0;i<4;i++)
		// 			{
		// 				for(int j=0;j<4;j++)
		// 				{
		// 					printf("%4d ", (int) input_tile[mm][nn][i][j][0]);
		// 				}
		// 				printf("\n");
		// 			}
		// 			printf("\n");
		// 		}
		// 	}
		// 	getchar();
		// }

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx+=2)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				#pragma HLS unroll factor=2
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_array_idx/WINO_HEIGHT] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[wino_array_idx%WINO_HEIGHT]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[wino_array_idx%WINO_HEIGHT+1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile with wino_array_idx" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("%4d ", (int) input_tile_reg[nn][i][j][0]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
					// }
					// getchar();
					// std::cout<<"weight tile with wino_array_idx" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// TODO: print input_tile_reg
				// if( wino_array_idx<WINO_HEIGHT)
				// {
					// std::cout<<"UV tile" <<std::endl;
					// for(int nn=0;nn<2;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
					// 	}
					// 	printf("\n");
					// }
					// // TODO: print weight_tile
					// for(int nn=0;nn<2;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}


				// if( wino_array_idx<WINO_HEIGHT)
				// {

				// 	for(int i=0;i<16;i++)
				// 	{
				// 		printf("%4d ", (int) UVA[0][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 	}
				// 	printf("\n");
				// 	// TODO: print weight_tile
				// 	for(int i=0;i<16;i++)
				// 	{
				// 		printf("%4d ",(int) UVA[1][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 	}
				// 	printf("\n");

				// 	getchar();
				// }


				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


				// if(wino_array_idx<WINO_HEIGHT and cycle==0)
				// {
				// 	printf("ATA\n");

				// 	for(int i=0;i<16;i++)
				// 	{
				// 		printf("%4d ", (int) ATA[0][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 	}
				// 	printf("\n");
				// 	// TODO: print weight_tile
				// 	for(int i=0;i<16;i++)
				// 	{
				// 		printf("%4d ",(int) ATA[1][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 	}
				// 	printf("\n");

				// 	getchar();
				// }


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						ap_uint<OUT_WIDTH*BATCH_SIZE> data;
						if(idepth_minitile_idx==0)
							data=0;
						else
							data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
						(out_value[r][c][1],out_value[r][c][0])=data;
						#else
						ap_uint<OUT_WIDTH*BATCH_SIZE> data0;
						ap_uint<OUT_WIDTH*BATCH_SIZE> data1;
						if(idepth_minitile_idx==0 && clear_flag)
						{
							data0=0;
							data1=0;
						}
						else
						{
							data0=out_buffer[r][wino_array_idx%WINO_HEIGHT/2][wino_array_idx/WINO_HEIGHT/2][0][wino_array_idx/WINO_HEIGHT%2][c][outbuffer_addr_reg];
							data1=out_buffer[r][wino_array_idx%WINO_HEIGHT/2][wino_array_idx/WINO_HEIGHT/2][1][wino_array_idx/WINO_HEIGHT%2][c][outbuffer_addr_reg];
						
						}
						(out_value[0][r][c][1],out_value[0][r][c][0])=data0;
						(out_value[1][r][c][1],out_value[1][r][c][0])=data1;
						#endif
					}
				}



				// if(wino_array_idx<WINO_HEIGHT and cycle==0)
				// {
					// printf("out_value\n");
					// for(int i=0;i<16;i++)
					// {
					// 	printf("%4d ", (int) out_value[0][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
					// }
					// printf("\n");
					// // TODO: print weight_tile
					// for(int i=0;i<16;i++)
					// {
					// 	printf("%4d ",(int) out_value[1][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
					// }
					// printf("\n");

					// getchar();
				// }

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

							// printf("[%8d %8d]\n",(int) sum_sat0,(int) sum_sat1 );


								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}

				// if(wino_array_idx<WINO_HEIGHT and cycle==0)
				// {
					// printf("out_value_back\n");
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// printf("\n");
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[1][r][c][0],(int) out_value_back[1][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer[r][wino_array_idx%WINO_HEIGHT/2][wino_array_idx/WINO_HEIGHT/2][0][wino_array_idx/WINO_HEIGHT%2][c][outbuffer_addr_reg]=(out_value_back[0][r][c][1],out_value_back[0][r][c][0]);
							out_buffer[r][wino_array_idx%WINO_HEIGHT/2][wino_array_idx/WINO_HEIGHT/2][1][wino_array_idx/WINO_HEIGHT%2][c][outbuffer_addr_reg]=(out_value_back[1][r][c][1],out_value_back[1][r][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}






void wino_stream_block(
		hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > top_stream_in[WINO_WIDTH],
		hls::stream< ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> > left_stream_in[WEIGHT_PORT_NUM][WEIGHT_FEED_NUMBER_PER_PORT],
		ap_uint<OUT_WIDTH*BATCH_SIZE> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE][WINO_WIDTH][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
		ap_uint<16> weightbuffer_outdepth_minitile_number,
		ap_uint<24> total_input_stream_tile,
		ap_uint<16> loop_omini_base_reset_cycle,
		ap_uint<10> loop_wino_tile_rowcol_self_reset_cycle_min1,
		ap_uint<32> loop_iload_reset_cycle,
		ap_uint<32> loop_wino_cell_bound,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_increment_step,
		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_increment_step,
		ap_uint<1> wino3x3_flag,
		bool clear_flag
		#if DEBUG_CONV_DESC
		,ConvDesc_t conv_desc
		#endif
		,ap_uint<1> ap_clk_div2
		)
{
	#if DEBUG_FILE_PRINT
	printf("---wino_stream_block---\n");fflush(stdout);
	#endif
	#pragma HLS interface ap_stable port=ap_clk_div2
	#pragma HLS interface ap_stable port=weightbuffer_outdepth_minitile_number
	#pragma HLS interface ap_stable port=total_input_stream_tile
	#pragma HLS interface ap_stable port=loop_omini_base_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_tile_rowcol_self_reset_cycle_min1
	#pragma HLS interface ap_stable port=loop_iload_reset_cycle
	#pragma HLS interface ap_stable port=loop_wino_cell_bound
	#pragma HLS interface ap_stable port=outbuffer_oload_increment_step
	#pragma HLS interface ap_stable port=outbuffer_omini_increment_step
	#pragma HLS interface ap_stable port=wino3x3_flag


	#pragma HLS array_partition variable = out_buffer dim=1 complete
	#pragma HLS array_partition variable = out_buffer dim=2 complete
	#pragma HLS array_partition variable = out_buffer dim=3 complete
	#pragma HLS array_partition variable = out_buffer dim=4 complete
	
	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg0[WINO_WIDTH][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg0 complete dim=2 

	ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> stream_temp_reg1[WINO_WIDTH][INDEPTH_MINITILE_SIZE];
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=1
	#pragma HLS array_partition variable = stream_temp_reg1 complete dim=2 


	#if DEBUG_FILE_PRINT
	for(int i=0;i<WINO_WIDTH;i++)
	{
		memset(stream_temp_reg0[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
		memset(stream_temp_reg1[i],0xAB,INDEPTH_MINITILE_SIZE*sizeof(ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE>));
	}
	#endif

	ap_int<BTB_WIDTH> input_tile[WINO_WIDTH][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=5
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1


	ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
	#pragma HLS array_partition variable = input_tile complete dim=4
	#pragma HLS array_partition variable = input_tile complete dim=3
	#pragma HLS array_partition variable = input_tile complete dim=2
	#pragma HLS array_partition variable = input_tile complete dim=1



	#if 0
	memset(stream_temp_reg0,0xAA,2*2*36*2);
	memset(stream_temp_reg1,0xAA,2*2*36*2);
	#endif



	for(int i=0;i<INDEPTH_MINITILE_SIZE;i++)
	{
		for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
		{
			#pragma HLS unroll

			for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
			{
				#pragma HLS unroll
				stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
			}
			top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
		}
	}






	ap_uint<1> load_input_flag=1;
	ap_uint<1> stream_pingpong_flag=1;
	ap_uint<24> loaded_input_stream_tile_number=1;
	ap_uint<16> loop_omini_base_cnt=1;
	ap_uint<10> loop_wino_tile_rowcol_cnt=0;
	ap_uint<32>	loop_iload_cnt=1;


	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_oload_offset=0;
	ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_omini_offset=0;


	ap_uint<10> idepth_minitile_idx=0;




	#if DEBUG_FILE_PRINT
	int write_idx=0;
	#endif
	for(int cycle=0;cycle < loop_wino_cell_bound; cycle++)
	{

		// for(int oload_idx=0;oload_idx<conv_desc.weightbuffer_load_outdepth_number;oload_idx++)
		// for(int iload_idx=0;iload_idx<conv_desc.weightbuffer_load_indepth_number;iload_idx++)
		// for(int imini_base_idx=0;imini_base_idx<conv_desc.weightbuffer_indepth_minitile_number;imini_base_idx++)
		// for(int wino_tile_row_idx=0;wino_tile_row_idx<conv_desc.wino_tile_number_in_out_rowstep;wino_tile_row_idx++)
		// for(int wino_tile_col_idx=0;wino_tile_col_idx<conv_desc.wino_tile_number_in_outwidth;wino_tile_col_idx++)
		// for(int omini_base_idx=0;omini_base_idx<loop_omini_base_reset_cycle ;omini_base_idx++)

		#pragma HLS pipeline
		#pragma HLS dependence variable=out_buffer inter false
		#pragma HLS dependence variable=out_buffer intra false

		ap_uint<1> load_input_flag_reg = (load_input_flag  && loaded_input_stream_tile_number !=  total_input_stream_tile);

		if(stream_pingpong_flag)
			load_input_tile(input_tile,stream_temp_reg0);
		else
			load_input_tile(input_tile,stream_temp_reg1);

		

		ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr = outbuffer_oload_offset + loop_wino_tile_rowcol_cnt +  outbuffer_omini_offset;

		// #if DEBUG_FILE_PRINT
		// int rowtile_idx=loop_wino_tile_rowcol_cnt/conv_desc.wino_tile_number_in_outwidth;
		// int coltile_idx=loop_wino_tile_rowcol_cnt%conv_desc.wino_tile_number_in_outwidth;
		// int outdepth_minitile_idx= (outbuffer_oload_offset+outbuffer_omini_offset)/(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth);
		// if((outbuffer_oload_offset+outbuffer_omini_offset)%(conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth))
		// {
		// 	printf("outdepth_minitile_idx not valid\n");
		// 	exit(-3);
		// }
		// #endif

		for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
		{
			#pragma HLS unroll
			if(stream_pingpong_flag && load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg1[wino_array_col][imini_idx]=stream_temp_reg1[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg1[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
			else if(load_input_flag_reg)
			{

				for(int imini_idx=0;imini_idx<INDEPTH_MINITILE_SIZE-1;imini_idx++)
				{
					#pragma HLS unroll
					stream_temp_reg0[wino_array_col][imini_idx]=stream_temp_reg0[wino_array_col][imini_idx+1];
				}
				top_stream_in[wino_array_col]>>stream_temp_reg0[wino_array_col][INDEPTH_MINITILE_SIZE-1];
			}
		}


		ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> weight_value_temp[WINO_HEIGHT];
		#pragma HLS array_partition variable=weight_value_temp complete 

		if(loop_omini_base_cnt <= weightbuffer_outdepth_minitile_number)
		{
			for(int i=0;i<WEIGHT_PORT_NUM;i++)
			{
				#pragma HLS unroll
				for(int j=0;j<WEIGHT_FEED_NUMBER_PER_PORT;j++)
				{
					#pragma HLS unroll
					left_stream_in[i][j]>>weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j];
					#if 0
						printf("wino_row_idx: %2d --", i*WEIGHT_FEED_NUMBER_PER_PORT+j);
						for(int k=0;k<WINO_DOMAIN_SIZE_SQUARE;k++)
						{
							printf("[%08x]", (unsigned int) weight_value_temp[i*WEIGHT_FEED_NUMBER_PER_PORT+j].range(k*32+31,k*32) );
						}
						printf("\n");
					#endif
				}
			}
		}

		ap_int<W_WIDTH> weight_tile[WINO_HEIGHT][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
		#pragma HLS array_partition variable = weight_tile complete dim=4
		#pragma HLS array_partition variable = weight_tile complete dim=3
		#pragma HLS array_partition variable = weight_tile complete dim=2
		#pragma HLS array_partition variable = weight_tile complete dim=1

		load_weight_tile(weight_tile,weight_value_temp);

		#if WINO_DOMAIN_SIZE==6
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx++)
		#else
		for(int wino_array_idx=0;wino_array_idx<WINO_HEIGHT*WINO_WIDTH;wino_array_idx+=2)
		#endif
		{
			#pragma HLS unroll 
			// for(int wino_array_col=0;wino_array_col<WINO_WIDTH;wino_array_col++)
			// {
				#pragma HLS unroll factor=2
				ap_int<BTB_WIDTH> input_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable = input_tile_reg complete dim=4
				#pragma HLS array_partition variable = input_tile_reg complete dim=3
				#pragma HLS array_partition variable = input_tile_reg complete dim=2
				#pragma HLS array_partition variable = input_tile_reg complete dim=1


				#if WINO_DOMAIN_SIZE==6
				ap_int<W_WIDTH> weight_tile_reg[INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#else
				ap_int<W_WIDTH> weight_tile_reg[2][INDEPTH_MINITILE_SIZE][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE];
				#pragma HLS array_partition variable = weight_tile_reg complete dim=4
				#endif
				#pragma HLS array_partition variable = weight_tile_reg complete dim=3
				#pragma HLS array_partition variable = weight_tile_reg complete dim=2
				#pragma HLS array_partition variable = weight_tile_reg complete dim=1

				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_MUL_WIDTH> UV_MUL_TILE[2][INDEPTH_MINITILE_SIZE/2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=5
				#endif
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=1
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=2
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=3
				#pragma HLS array_partition variable=UV_MUL_TILE complete dim=4

				load_reg_tile4<ap_int<BTB_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg, input_tile[wino_array_idx/WINO_HEIGHT] );


				#if WINO_DOMAIN_SIZE==6
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg, weight_tile[wino_array_idx%WINO_HEIGHT]);
				#else
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0], weight_tile[wino_array_idx%WINO_HEIGHT]);
				load_reg_tile3<ap_int<W_WIDTH>,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1], weight_tile[wino_array_idx%WINO_HEIGHT+1]);
				#endif

				#if DEBUG_FILE_PRINT
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						char infilename[100];
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						#if WINO_DOMAIN_SIZE==4
						sprintf(infilename,"invector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_input_vector<BTB_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE,BATCH_SIZE>(input_tile_reg,write_idx,infilename);
						
						#endif

						#if WINO_DOMAIN_SIZE==6
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg,write_idx,wfilename);
			
						#else
						char wfilename[100];
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[0],write_idx,wfilename);
						sprintf(wfilename,"wvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_weight_vector<W_WIDTH,INDEPTH_MINITILE_SIZE,WINO_DOMAIN_SIZE>(weight_tile_reg[1],write_idx,wfilename);
						#endif
					}

				#endif



				// TODO: print input_tile_reg
				// if(cycle==0 && wino_array_idx==0)
				// {
					// std::cout<<"input tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<4;i++)
					// 	{
					// 		for(int j=0;j<4;j++)
					// 		{
					// 			printf("%4d ", (int) input_tile_reg[nn][i][j][0]);
					// 		}
					// 		printf("\n");
					// 	}
					// 	printf("\n");
	
					// }
					// std::cout<<"weight tile in depth" <<std::endl;
					// for(int nn=0;nn<INDEPTH_MINITILE_SIZE;nn++)
					// {
					// 	for(int i=0;i<16;i++)
					// 	{
					// 		printf("%4d ",(int) weight_tile_reg[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE]);
					// 	}
					// 	printf("\n");
					// }

					// getchar();
				// }

				

				#if WINO_DOMAIN_SIZE==6
				element_wise_mult_6x6<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#else
				// element_wise_mult_6x6<0>(UV_MUL_TILE[0],input_tile_reg,weight_tile_reg[0], ap_clk_div2 );
				// element_wise_mult_6x6<0>(UV_MUL_TILE[1],input_tile_reg,weight_tile_reg[1], ap_clk_div2 );
				element_wise_mult_4x4<0>(UV_MUL_TILE,input_tile_reg,weight_tile_reg, ap_clk_div2 );
				#endif
				

				// //TODO: print input_tile_reg
				// if( wino_array_idx%WINO_HEIGHT==2)
				// {
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ", (int) UV_MUL_TILE[0][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}
				// 	//TODO: print weight_tile
				// 	for(int nn=0;nn<2;nn++)
				// 	{
				// 		for(int i=0;i<16;i++)
				// 		{
				// 			printf("%4d ",(int) UV_MUL_TILE[1][nn][i/WINO_DOMAIN_SIZE][i%WINO_DOMAIN_SIZE][0]);
				// 		}
				// 		printf("\n");
				// 	}

				// 	getchar();
				// }


			
				#if WINO_DOMAIN_SIZE==6
				ap_int<UV_WIDTH> UV[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#else
				ap_int<UV_WIDTH> UV[2][WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE];
				#pragma HLS array_partition variable=UV complete dim=4
				#endif
				#pragma HLS array_partition variable=UV complete dim=1
				#pragma HLS array_partition variable=UV complete dim=2
				#pragma HLS array_partition variable=UV complete dim=3

				for(int wino_row=0;wino_row<WINO_DOMAIN_SIZE;wino_row++)
				{
					#pragma HLS unroll
					for(int wino_col=0;wino_col<WINO_DOMAIN_SIZE;wino_col++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<UV_MUL_WIDTH> temp=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp+=UV_MUL_TILE[id2][wino_row][wino_col][b];
							}
							UV[wino_row][wino_col][b]=temp>>UV_QUANT_BIT;
							#else
							ap_int<UV_MUL_WIDTH> temp0=0;
							ap_int<UV_MUL_WIDTH> temp1=0;
							for(int id2=0;id2<INDEPTH_MINITILE_SIZE/2;id2++)
							{
								#pragma HLS unroll
								temp0+=UV_MUL_TILE[0][id2][wino_row][wino_col][b];
								temp1+=UV_MUL_TILE[1][id2][wino_row][wino_col][b];
							}
							UV[0][wino_row][wino_col][b]=temp0>>UV_QUANT_BIT;
							UV[1][wino_row][wino_col][b]=temp1>>UV_QUANT_BIT;
							#endif
						}
					}
				}


				#if DEBUG_FILE_PRINT
					char uvfilename[100];
					#if WINO_DOMAIN_SIZE==6
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV,write_idx,uvfilename);
					}
					#else
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[0],write_idx,uvfilename);
						sprintf(uvfilename,"uvvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<UV_WIDTH,WINO_DOMAIN_SIZE,BATCH_SIZE>(UV[1],write_idx,uvfilename);
					}
					#endif
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<UVA_WIDTH> UVA[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<UVA_WIDTH> UVA[2][WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=UVA complete dim=4
				#endif

				#pragma HLS array_partition variable=UVA complete dim=1
				#pragma HLS array_partition variable=UVA complete dim=2
				#pragma HLS array_partition variable=UVA complete dim=3

		
				for(int ridx=0;ridx<WINO_DOMAIN_SIZE;ridx++)
				{
					#pragma HLS unroll
					for(int bidx=0;bidx<BATCH_SIZE;bidx++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						UVA_row(UVA,UV,ridx,bidx,wino3x3_flag);
						#else
						UVA_row(UVA[0],UV[0],ridx,bidx,wino3x3_flag);
						UVA_row(UVA[1],UV[1],ridx,bidx,wino3x3_flag);
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<ATA_WIDTH> ATA[WINO_OUT_SIZE][WINO_OUT_SIZE][BATCH_SIZE];
				#else
				ap_int<ATA_WIDTH> ATA[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=ATA complete dim=4
				#endif
				#pragma HLS array_partition variable=ATA complete dim=1
				#pragma HLS array_partition variable=ATA complete dim=2
				#pragma HLS array_partition variable=ATA complete dim=3

				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				if(wino3x3_flag)
				{
				#endif
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
		
						for(int bidx=0;bidx<BATCH_SIZE;bidx++)
						{
							#pragma HLS 
							
			
							#if WINO_DOMAIN_SIZE==6
							ATA_col(ATA,UVA,cidx,bidx,wino3x3_flag);
							#else

								ATA_col(ATA[0],UVA[0],cidx,bidx,wino3x3_flag);
								ATA_col(ATA[1],UVA[1],cidx,bidx,wino3x3_flag);
							#endif
						}
					}
				
				#if WINO_OUT_SIZE_CELL == 4 && WINO_OUT_SIZE==2
				}
				else
				{
					for(int cidx=0;cidx<WINO_OUT_SIZE_CELL;cidx++)
					{
						#pragma HLS unroll
						for(int ridx=0;ridx<WINO_OUT_SIZE_CELL;ridx++)
						{
							#pragma HLS unroll
							for(int bidx=0;bidx<BATCH_SIZE;bidx++)
							{
								#pragma HLS
								ATA[0][ridx][cidx][bidx]=UV[0][ridx][cidx][bidx]*4;
								ATA[1][ridx][cidx][bidx]=UV[1][ridx][cidx][bidx]*4;
							}

						}
					}
				}
				#endif


			
				#if DEBUG_FILE_PRINT
					char filename[100];
					
					if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
					{
						#if WINO_DOMAIN_SIZE==6
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA,write_idx,filename);
						#else
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[0],write_idx,filename);
						sprintf(filename,"outvector_%d_%d.txt",wino_array_idx%WINO_HEIGHT+1,wino_array_idx/WINO_HEIGHT);
						attach_output_vector<ATA_WIDTH,WINO_OUT_SIZE_CELL,BATCH_SIZE>(ATA[1],write_idx,filename);
						#endif
					}
				#endif

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value complete dim=1
				#pragma HLS array_partition variable=out_value complete dim=2
				#pragma HLS array_partition variable=out_value complete dim=3

				ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> outbuffer_addr_reg;
				load_reg< ap_uint<OUTPUT_BUFFER_DEPTH_BITWIDTH> >(outbuffer_addr_reg,outbuffer_addr);

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						#if WINO_DOMAIN_SIZE==6
						ap_uint<OUT_WIDTH*BATCH_SIZE> data;
						if(idepth_minitile_idx==0)
							data=0;
						else
							data=out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg];
						(out_value[r][c][1],out_value[r][c][0])=data;
						#else
						ap_uint<OUT_WIDTH*BATCH_SIZE> data0;
						ap_uint<OUT_WIDTH*BATCH_SIZE> data1;
						if(idepth_minitile_idx==0 && clear_flag)
						{
							data0=0;
							data1=0;
						}
						else
						{
							data0=out_buffer[r][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][c][outbuffer_addr_reg];
							data1=out_buffer[r][wino_array_idx%WINO_HEIGHT+1][wino_array_idx/WINO_HEIGHT][c][outbuffer_addr_reg];
						
						}
						(out_value[0][r][c][1],out_value[0][r][c][0])=data0;
						(out_value[1][r][c][1],out_value[1][r][c][0])=data1;
						#endif
					}
				}

				#if WINO_DOMAIN_SIZE==6
				ap_int<OUT_WIDTH> out_value_back[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#else
				ap_int<OUT_WIDTH> out_value_back[2][WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE];
				#pragma HLS array_partition variable=out_value_back complete dim=4
				#endif
				#pragma HLS array_partition variable=out_value_back complete dim=1
				#pragma HLS array_partition variable=out_value_back complete dim=2
				#pragma HLS array_partition variable=out_value_back complete dim=3

				for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
				{
					#pragma HLS unroll
					for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					{
						#pragma HLS unroll
						for(int b=0;b<BATCH_SIZE;b++)
						{
							#if WINO_DOMAIN_SIZE==6
							ap_int<ATA_WIDTH+1> sum_sat;
							
							sum_sat=ATA[r][c][b]+out_value[r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit=sum_sat.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit ==0 ||  judgebit == -1)
										out_value_back[r][c][b]=sum_sat;
									else if (sum_sat[ATA_WIDTH]==1 )
										out_value_back[r][c][b]=OUT_SAT_MIN;
									else
										out_value_back[r][c][b]=OUT_SAT_MAX;
								#else
									out_value_back[r][c][b]=sum_sat;
								#endif 

							#else
							ap_int<ATA_WIDTH+1> sum_sat0;
							ap_int<ATA_WIDTH+1> sum_sat1;
							
							sum_sat0=ATA[0][r][c][b]+out_value[0][r][c][b];
							sum_sat1=ATA[1][r][c][b]+out_value[1][r][c][b];

								#if ATA_WIDTH+1 > OUT_WIDTH
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit0=sum_sat0.range(ATA_WIDTH,OUT_WIDTH-1);
									ap_int<ATA_WIDTH+2-OUT_WIDTH> judgebit1=sum_sat1.range(ATA_WIDTH,OUT_WIDTH-1);
									if(judgebit0 == 0 ||  judgebit0 == -1)
										out_value_back[0][r][c][b]=sum_sat0;
									else if (sum_sat0[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[0][r][c][b]=OUT_SAT_MAX;
									}

									if(judgebit1 ==0 ||  judgebit1 == -1)
										out_value_back[1][r][c][b]=sum_sat1;
									else if (sum_sat1[ATA_WIDTH]==1 )
									{
										// std::cout<<"Encountering MIN"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MIN;
									}
									else
									{
										// std::cout<<"Encountering MAX"<<std::endl;
										out_value_back[1][r][c][b]=OUT_SAT_MAX;
									}
								#else
									out_value_back[0][r][c][b]=sum_sat0;
									out_value_back[1][r][c][b]=sum_sat1;
							#endif 

							#endif

							// #if DEBUG_FILE_PRINT
							// int outdepth_idx = outdepth_minitile_idx*OUTDEPTH_MINITILE_SIZE+wino_array_row;
							// int col_idx = (coltile_idx*WINO_WIDTH+wino_array_col)*conv_desc.wino_output_tile_size+c;
							// int row_idx = rowtile_idx*conv_desc.wino_output_tile_size+r;
							// out_value_back[r][c][0]=row_idx*conv_desc.outwidth+col_idx;
							// out_value_back[r][c][1]=outdepth_idx;
							// #endif
						}
					}
				}
				// if(wino_array_idx/WINO_HEIGHT==6 && cycle==0)
				// {
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					
					// for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					// {
					// 	#pragma HLS unroll
					// 	for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
					// 	{
					// 		printf("[%8d %8d]",(int) out_value_back[0][r][c][0],(int) out_value_back[0][r][c][1] );
						
					// 	}
					// 	printf("\n");
					// }
					// getchar();
				// }

				if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				{
					for(int r=0;r<WINO_OUT_SIZE_CELL;r++)
					{
						#pragma HLS unroll
						for(int c=0;c<WINO_OUT_SIZE_CELL;c++)
						{
							#if WINO_DOMAIN_SIZE == 6
							out_buffer[r][c][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][outbuffer_addr_reg]=(out_value_back[r][c][1],out_value_back[r][c][0]);
							#else
							out_buffer[r][wino_array_idx%WINO_HEIGHT][wino_array_idx/WINO_HEIGHT][c][outbuffer_addr_reg]=(out_value_back[0][r][c][1],out_value_back[0][r][c][0]);
							out_buffer[r][wino_array_idx%WINO_HEIGHT+1][wino_array_idx/WINO_HEIGHT][c][outbuffer_addr_reg]=(out_value_back[1][r][c][1],out_value_back[1][r][c][0]);
							#endif
						}
					}
				}
			// }
		}
		#if DEBUG_FILE_PRINT
			if(loop_omini_base_cnt<=weightbuffer_outdepth_minitile_number)
				write_idx++;
		#endif

		
		// element_wise_mult(UV_MUL,input_tile,weight_tile,ap_clk_div2);


		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			idepth_minitile_idx++;
		}
		else if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			idepth_minitile_idx=0;
		}
		
		
	
		if(loop_omini_base_cnt==loop_omini_base_reset_cycle && loop_wino_tile_rowcol_cnt==loop_wino_tile_rowcol_self_reset_cycle_min1)
		{
			loop_wino_tile_rowcol_cnt=0;
		}
		else if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_wino_tile_rowcol_cnt++;
		}



		if(loop_iload_cnt==loop_iload_reset_cycle)
		{
			loop_iload_cnt=1;
			outbuffer_oload_offset+=outbuffer_oload_increment_step;
		}
		else
		{
			loop_iload_cnt++;
		}
		

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle ) 
		{
			load_input_flag = 1;
		}
		else if(loop_omini_base_cnt==INDEPTH_MINITILE_SIZE)
		{
			load_input_flag = 0;
		}

		if(loop_omini_base_cnt==loop_omini_base_reset_cycle)
		{
			loop_omini_base_cnt=1;
			loaded_input_stream_tile_number++;
			stream_pingpong_flag=~stream_pingpong_flag;
			outbuffer_omini_offset=0;
		}
		else
		{
			loop_omini_base_cnt++;
			outbuffer_omini_offset+=outbuffer_omini_increment_step;
		}	
	}
}












#endif
