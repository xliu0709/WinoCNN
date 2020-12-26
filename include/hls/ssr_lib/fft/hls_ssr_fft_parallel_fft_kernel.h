// File Name : hls_ssr_fft_parallel_fft_kernel.h
#ifndef HLS_SSR_FFT_PARALLEL_FFT_KERNEL_H_
#define HLS_SSR_FFT_PARALLEL_FFT_KERNEL_H_

#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================

#include "hls_ssr_fft_adder_tree.h"
#include "hls_ssr_fft_enums.h"
#include "hls_ssr_fft_multiplication_traits.h"
#include "hls_ssr_fft_complex_multiplier.h"

#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_adder_tree.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_enums.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_multiplication_traits.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_complex_multiplier.h"

#endif
//#define DEBUG_FORK_MERGE
namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Parallel Kernel with Adder Tree used for accumulation instead of a unrolls
	template 	<
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
				>
	void __parallel_fft_kernel_L4_R4x1_atree( T_bflyIn			bflyIndata[4],
											  T_bflyOut		bflyOutdata[4]
									   	    )
	{
		#pragma HLS INLINE recursive
		const T_twiddle local_r4_kernel[4][4]= {
												  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
												  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
												  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
												  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
												};
		#pragma HLS ARRAY_PARTITION variable=local_r4_kernel complete dim=0

		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		stage2_accum D0t[4];
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		stage2_accum accum;
		stage2_accum accum2;

		/// Get Sequence of EVEN number samples from input
		T_bflyIn D0[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i];
		}

		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
			#pragma HLS UNROLL
			complex_multiply(D0[j],local_r4_kernel[i][j],product_vector[j]);
			}
		AdderTreeClass<4> AdderTreeClass_obj;
		AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D0t[i]);
		}

		// write final output
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL

			bflyOutdata[i] = D0t[i];
		}

	}

	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++Length 8 fft kernel using two radix 4 kernels+++++++++++++++++++++++++++++++++++++
	template 	<
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
				>
	void __parallel_fft_kernel_L8_R4x2_atree( T_bflyIn			bflyIndata[8],
											  T_bflyOut		bflyOutdata[8]
									        )
	{
		#pragma HLS INLINE recursive
		const T_twiddle local_r8_twiddle_table[4] = { 	  T_twiddle(1,0),
														  T_twiddle(0.707106781186548,-0.707106781186547),
														  T_twiddle(0,-1),
														  T_twiddle(-0.707106781186548,-0.707106781186547),
													};
		#pragma HLS ARRAY_PARTITION variable=local_r8_twiddle_table complete dim=1
		const T_twiddle local_r8_kernel[4][4] = {
												  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
												  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
												  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
												  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
												};
		#pragma HLS ARRAY_PARTITION variable=local_r8_kernel complete dim=0
		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		stage2_accum D0t[4];
		stage2_accum D2t[4];
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		stage2_accum accum;
		stage2_accum accum2;
		/// Get Sequence of EVEN number samples from input
		T_bflyIn D0[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i*2];
		}

		/// Get Sequence of ODD number samples from input
		T_bflyIn D2[4];
		#pragma HLS ARRAY_PARTITION variable=D2 complete dim=1

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*2+1];
		}
		////////////////////////////////////////////////////

		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(D0[j],local_r8_kernel[i][j],product_vector[j]);

			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D0t[i]);
		}

		//// Calculate radix4 butterfly of Sequence D2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(D2[j],local_r8_kernel[i][j],product_vector[j]);
			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D2t[i]);
		}

		/// Multiply by twiddle factors , sequence D2 only
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL
			typedef typename ssr_fft_multiplication_traits<T_twiddle,stage2_accum>::TTR_product_op t_product_op;
			complex_multiply(D2t[i],local_r8_twiddle_table[i],D2t[i]);
		}

		/// Merge two FFTs of length 4 To create an FFT of length 8 which is final output
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL

			bflyOutdata[i] = D0t[i] + D2t[i];
			bflyOutdata[i+4] = D0t[i] - D2t[i];
		}
	}




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++Length 16 fft kernel using 4 radix 4 kernels 2 stages+++++++++++++++++++++++++++
	template <
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
			 >

	void __parallel_fft_kernel_L16_R4x4xS2_atree
										  (
											T_bflyIn			bflyIndata[16],
											T_bflyOut		bflyOutdata[16]
										  )
	{
	//#pragma HLS PIPELINE II=1
		#pragma HLS INLINE recursive
		const T_twiddle local_r16_twiddle_table[10] = {
														 T_twiddle( 1.000000000000000 , 0.000000000000000 ),
														 T_twiddle( 0.923879532511287 , -0.382683432365090 ),
														 T_twiddle( 0.707106781186547 , -0.707106781186548 ),
														 T_twiddle( 0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( 0.000000000000000 , -1.000000000000000 ),
														 T_twiddle( -0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( -0.707106781186548 , -0.707106781186547 ),
														 T_twiddle( -0.923879532511287 , -0.382683432365089 ),
														 T_twiddle( -1.000000000000000 , 0.000000000000000 ),
														 T_twiddle( -0.923879532511287 , 0.382683432365090 )
													 };

		#pragma HLS ARRAY_PARTITION variable=local_r8_twiddle_table complete dim=1

		const T_twiddle local_r8_kernel[4][4] = {
												  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
												  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
												  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
												  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
												};
		#pragma HLS ARRAY_PARTITION variable=local_r8_kernel complete dim=0
		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage2_accum>::TTR_bflyAccumType stage3_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage3_accum>::TTR_bflyAccumType stage4_accum;
		stage2_accum D0t[4];
		stage2_accum D1t[4];
		stage2_accum D2t[4];
		stage2_accum D3t[4];
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D1t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D3t complete dim=1
		stage2_accum accum0;
		stage2_accum accum1;
		stage2_accum accum2;
		stage2_accum accum3;
		T_bflyIn D0[4];
		T_bflyIn D1[4];
		T_bflyIn D2[4];
		T_bflyIn D3[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D3 complete dim=1

		/// Get Sequence of even number samples from input : sequence : 4k+0

		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i*4];
		}
		////////////////////////////////////////////////////////////////////////


		/// Get Sequence of even number samples from input : sequence : 4k+1

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D1[i] = bflyIndata[i*4+1];
		}
		////////////////////////////////////////////////////////////////////////

		/// Get Sequence of even number samples from input : sequence : 4k+2

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*4+2];
		}
		////////////////////////////////////////////////////////////////////////


		/// Get Sequence of even number samples from input : sequence : 4k+2

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*4+2];
		}
		////////////////////////////////////////////////////////////////////////

		/// Get Sequence of even number samples from input : sequence : 4k+3

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D3[i] = bflyIndata[i*4+3];
		}
		////////////////////////////////////////////////////////////////////////
		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(D0[j],local_r8_kernel[i][j],product_vector[j]);
			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D0t[i]);
		}
		//// Calculate radix4 butterfly of Sequence D1
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(D1[j],local_r8_kernel[i][j],product_vector[j]);
			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D1t[i]);
		}
		//// Calculate radix4 butterfly of Sequence D2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1
			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(D2[j],local_r8_kernel[i][j],product_vector[j]);
			}

			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D2t[i]);
		}


		//// Calculate radix4 butterfly of Sequence D3
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			T_bflyIn product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				complex_multiply(D3[j],local_r8_kernel[i][j],product_vector[j]);

			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D3t[i]);
		}

		//////////////////////////////////////// Multiply outputs by Twiddle Factors ////////////////////////////START
		stage2_accum OUT0[4];
		stage2_accum OUT1[4];
		stage2_accum OUT2[4];
		stage2_accum OUT3[4];

		#pragma HLS ARRAY_PARTITION variable=OUT0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT3 complete dim=1
		typedef typename ssr_fft_multiplication_traits<T_twiddle,stage2_accum>::TTR_product_op t_product_op2;
		OUT0[0] = D0t[0];
		OUT0[1] = D1t[0];
		OUT0[2] = D2t[0];
		OUT0[3] = D3t[0];
		OUT1[0] = D0t[1];
		//OUT1[1] = (t_product_op2)D1t[1]*( (t_product_op2)local_r16_twiddle_table[1] );
		complex_multiply(D1t[1],local_r16_twiddle_table[1],OUT1[1]);

		//OUT1[2] = (t_product_op2)D2t[1]*( (t_product_op2)local_r16_twiddle_table[2] );
		complex_multiply(D2t[1],local_r16_twiddle_table[2],OUT1[2]);

		//OUT1[3] = (t_product_op2)D3t[1]*( (t_product_op2)local_r16_twiddle_table[3] );
		complex_multiply(D3t[1],local_r16_twiddle_table[3],OUT1[3]);

		OUT2[0] = (t_product_op2)D0t[2];

		//OUT2[1] = (t_product_op2)D1t[2]*( (t_product_op2)local_r16_twiddle_table[2] );
		complex_multiply(D1t[2],local_r16_twiddle_table[2],OUT2[1]);

		//OUT2[2] = (t_product_op2)D2t[2]*( (t_product_op2)local_r16_twiddle_table[4] );
		complex_multiply(D2t[2],local_r16_twiddle_table[4],OUT2[2]);

		//OUT2[3] = (t_product_op2)D3t[2]*( (t_product_op2)local_r16_twiddle_table[6] );
		complex_multiply(D3t[2],local_r16_twiddle_table[6],OUT2[3]);


		OUT3[0] = (t_product_op2)D0t[3];

		//OUT3[1] = (t_product_op2)D1t[3]*( (t_product_op2)local_r16_twiddle_table[3] );
		complex_multiply(D1t[3],local_r16_twiddle_table[3],OUT3[1]);

		//OUT3[2] = (t_product_op2)D2t[3]*( (t_product_op2)local_r16_twiddle_table[6] );
		complex_multiply(D2t[3],local_r16_twiddle_table[6],OUT3[2]);

		//OUT3[3] = (t_product_op2)D3t[3]*( (t_product_op2)local_r16_twiddle_table[9] );
		complex_multiply(D3t[3],local_r16_twiddle_table[9],OUT3[3]);

		//////////////////////////////////////// Multiply outputs by Twiddle Factors ////////////////////////////END
		typedef typename bfly_traits<tp_scaling_mode,stage2_accum>::TTR_bflyAccumType stage3_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage3_accum>::TTR_bflyAccumType stage4_accum;
		stage4_accum D00t[4];
		stage4_accum D11t[4];
		stage4_accum D22t[4];
		stage4_accum D33t[4];
		#pragma HLS ARRAY_PARTITION variable=D00t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D11t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D22t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D33t complete dim=1
		stage4_accum accum00;
		stage4_accum accum11;
		stage4_accum accum22;
		stage4_accum accum33;
	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START


		//// Calculate radix4 butterfly of Sequence OUT0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			stage2_accum product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				complex_multiply(OUT0[j],local_r8_kernel[i][j],product_vector[j]);

			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D00t[i]);
		}

		//// Calculate radix4 butterfly of Sequence OUT1
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			stage2_accum product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1
			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(OUT1[j],local_r8_kernel[i][j],product_vector[j]);
			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D11t[i]);
		}

		//// Calculate radix4 butterfly of Sequence OUT2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			stage2_accum product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				complex_multiply(OUT2[j],local_r8_kernel[i][j],product_vector[j]);

			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D22t[i]);
		}

		//// Calculate radix4 butterfly of Sequence OUT3
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			stage2_accum product_vector[4];
			#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1



			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL
				complex_multiply(OUT3[j],local_r8_kernel[i][j],product_vector[j]);

			}
			AdderTreeClass<4> AdderTreeClass_obj;
			AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,D33t[i]);
		}

		unsigned int count=0;

		for (int i = 0; i < 4; i++)
		{
		#pragma HLS UNROLL
			bflyOutdata[count++]=D00t[i];
			bflyOutdata[count++]=D11t[i];
			bflyOutdata[count++]=D22t[i];
			bflyOutdata[count++]=D33t[i];

		}


	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END
	////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END

	}




	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++Length 4 parallel fft kernel using 1 radix 4 kernels++++++++++++++++++++++++++++
	template 	<
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
				>
	void __parallel_fft_kernel_L4_R4x1( T_bflyIn			bflyIndata[4],
										T_bflyOut		bflyOutdata[4]
									   )
	{
		#pragma HLS INLINE recursive


		const T_twiddle local_r4_kernel[4][4] = {
												  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
												  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
												  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
												  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
													};
		#pragma HLS ARRAY_PARTITION variable=local_r4_kernel complete dim=0
		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		stage2_accum D0t[4];
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		stage2_accum accum;
		stage2_accum accum2;
		/// Get Sequence of EVEN number samples from input
		T_bflyIn D0[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i];
		}
		////////////////////////////////////////////////////

		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r4_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D0[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum = prd;
				}
				else
				{
					T_bflyIn prd   =(D0[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum =  accum + (stage2_accum)prd;
				}

			}

			D0t[i] = accum;
		}

		// write final output
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL

			bflyOutdata[i] = D0t[i];
		}
	}


    ///// Parallel Kernels Based on FFT Split Radix FFT Algorithms and unrolled accumulatio no adder tree////////////////////////////////
	/*
	 *
	 *
	 *
	 *
	 *
	 *
	 */

	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //++++++++++++++++++++++++++++++++++++++++++++++++++Length 8 fft kernel using two radix 4 kernels+++++++++++++++++++++++++++++++++++++
	template 	<
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
				>
	void __parallel_fft_kernel_L8_R4x2( T_bflyIn			bflyIndata[8],
										T_bflyOut		bflyOutdata[8]
									   )
	{
		#pragma HLS INLINE recursive


		const T_twiddle local_r8_twiddle_table[4] = { T_twiddle(1,0),
													  T_twiddle(0.707106781186548,-0.707106781186547),
													  T_twiddle(0,-1),
													  T_twiddle(-0.707106781186548,-0.707106781186547),
													};
		#pragma HLS ARRAY_PARTITION variable=local_r8_twiddle_table complete dim=1
												const T_twiddle local_r8_kernel[4][4] = {
																						  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
																						  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
																						  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
																						  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
																							};
		#pragma HLS ARRAY_PARTITION variable=local_r8_kernel complete dim=0
		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		stage2_accum D0t[4];
		stage2_accum D2t[4];
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		stage2_accum accum;
		stage2_accum accum2;
		/// Get Sequence of EVEN number samples from input
		T_bflyIn D0[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i*2];
		}
		////////////////////////////////////////////////////

		/// Get Sequence of ODD number samples from input
		T_bflyIn D2[4];
		#pragma HLS ARRAY_PARTITION variable=D2 complete dim=1

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*2+1];
		}
		////////////////////////////////////////////////////

		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			for (int  j= 0; j < 4; j++)
			{
			#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D0[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum = prd;
				}
				else
				{
					T_bflyIn prd   =(D0[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum =  accum + (stage2_accum)prd;
				}
			}

		D0t[i] = accum;
		}


		//// Calculate radix4 butterfly of Sequence D2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D2[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum2 = prd;
				}
				else
				{
					T_bflyIn prd   =(D2[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum2 =  accum2 + (stage2_accum)prd;
				}

			}

			D2t[i] = accum2;
		}

		/// Multiply by twiddle factors , sequence D2 only
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL
			stage2_accum twiddle_factor = local_r8_twiddle_table[i];

			D2t[i] = D2t[i] * twiddle_factor;
		}

		/// Merge two FFTs of length 4 To create an FFT of length 8 which is final output
		for (int  i= 0; i < 4; i++)
		{
		#pragma HLS UNROLL

			bflyOutdata[i] = D0t[i] + D2t[i];
			bflyOutdata[i+4] = D0t[i] - D2t[i];
		}


	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++Length 16 fft kernel using 4 radix 4 kernels 2 stages+++++++++++++++++++++++++++
	template <
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
			 >

	void __parallel_fft_kernel_L16_R4x4xS2
										  (
											T_bflyIn			bflyIndata[16],
											T_bflyOut		bflyOutdata[16]
										  )

	{
		#pragma HLS INLINE recursive


		const T_twiddle local_r16_twiddle_table[10] = {
														 T_twiddle( 1.000000000000000 , 0.000000000000000 ),
														 T_twiddle( 0.923879532511287 , -0.382683432365090 ),
														 T_twiddle( 0.707106781186547 , -0.707106781186548 ),
														 T_twiddle( 0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( 0.000000000000000 , -1.000000000000000 ),
														 T_twiddle( -0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( -0.707106781186548 , -0.707106781186547 ),
														 T_twiddle( -0.923879532511287 , -0.382683432365089 ),
														 T_twiddle( -1.000000000000000 , 0.000000000000000 ),
														 T_twiddle( -0.923879532511287 , 0.382683432365090 )
													 };

		#pragma HLS ARRAY_PARTITION variable=local_r8_twiddle_table complete dim=1

		const T_twiddle local_r8_kernel[4][4] = {
												  { T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0),T_twiddle(1,0) },
												  { T_twiddle(1,0),T_twiddle(0,-1),T_twiddle(-1,0),T_twiddle(0,1) },
												  { T_twiddle(1,0),T_twiddle(-1,0),T_twiddle(1,0),T_twiddle(-1,0) },
												  { T_twiddle(1,0),T_twiddle(0,1),T_twiddle(-1,0),T_twiddle(0,-1) }
												};
		#pragma HLS ARRAY_PARTITION variable=local_r8_kernel complete dim=0
		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage2_accum>::TTR_bflyAccumType stage3_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage3_accum>::TTR_bflyAccumType stage4_accum;
		stage2_accum D0t[4];
		stage2_accum D1t[4];
		stage2_accum D2t[4];
		stage2_accum D3t[4];
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D1t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D3t complete dim=1
		stage2_accum accum0;
		stage2_accum accum1;
		stage2_accum accum2;
		stage2_accum accum3;
		T_bflyIn D0[4];
		T_bflyIn D1[4];
		T_bflyIn D2[4];
		T_bflyIn D3[4];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D3 complete dim=1
		/// Get Sequence of even number samples from input : sequence : 4k+0
		for(int i=0; i<4; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i*4];
		}
		////////////////////////////////////////////////////////////////////////


		/// Get Sequence of even number samples from input : sequence : 4k+1

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D1[i] = bflyIndata[i*4+1];
		}
		////////////////////////////////////////////////////////////////////////

		/// Get Sequence of even number samples from input : sequence : 4k+2

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*4+2];
		}
		////////////////////////////////////////////////////////////////////////


		/// Get Sequence of even number samples from input : sequence : 4k+2

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D2[i] = bflyIndata[i*4+2];
		}
		////////////////////////////////////////////////////////////////////////

		/// Get Sequence of even number samples from input : sequence : 4k+3

		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			D3[i] = bflyIndata[i*4+3];
		}
		////////////////////////////////////////////////////////////////////////
		//// Calculate radix4 butterfly of Sequence D0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D0[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum0 = prd;
				}
				else
				{
					T_bflyIn prd   =(D0[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum0 =  accum0 + (stage2_accum)prd;
				}

			}

			D0t[i] = accum0;
		}
		//// Calculate radix4 butterfly of Sequence D1
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL
			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D1[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum1 = prd;
				}
				else
				{
					T_bflyIn prd   =(D1[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum1 =  accum1 + (stage2_accum)prd;
				}

			}

			D1t[i] = accum1;
		}


		//// Calculate radix4 butterfly of Sequence D2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			for (int  j= 0; j < 4; j++)
			{
			#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D2[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum2 = prd;
				}
				else
				{
					T_bflyIn prd   =(D2[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum2 =  accum2 + (stage2_accum)prd;
				}

			}

			D2t[i] = accum2;
		}


		//// Calculate radix4 butterfly of Sequence D3
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL

			for (int  j= 0; j < 4; j++)
			{
			#pragma HLS UNROLL

				T_bflyIn twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					T_bflyIn prd =D3[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum3 = prd;
				}
				else
				{
					T_bflyIn prd   =(D3[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum3 =  accum3 + (stage2_accum)prd;
				}

			}

			D3t[i] = accum3;
		}

		//////////////////////////////////////// Multiply outputs by Twiddle Factors ////////////////////////////START
		stage2_accum OUT0[4];
		stage2_accum OUT1[4];
		stage2_accum OUT2[4];
		stage2_accum OUT3[4];

		#pragma HLS ARRAY_PARTITION variable=OUT0 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT1 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT2 complete dim=1
		#pragma HLS ARRAY_PARTITION variable=OUT3 complete dim=1

		OUT0[0] = D0t[0];
		OUT0[1] = D1t[0];
		OUT0[2] = D2t[0];
		OUT0[3] = D3t[0];

		OUT1[0] = D0t[1];
		OUT1[1] = D1t[1]*( (stage2_accum)local_r16_twiddle_table[1] );
		OUT1[2] = D2t[1]*( (stage2_accum)local_r16_twiddle_table[2] );
		OUT1[3] = D3t[1]*( (stage2_accum)local_r16_twiddle_table[3] );

		OUT2[0] = D0t[2];
		OUT2[1] = D1t[2]*( (stage2_accum)local_r16_twiddle_table[2] );
		OUT2[2] = D2t[2]*( (stage2_accum)local_r16_twiddle_table[4] );
		OUT2[3] = D3t[2]*( (stage2_accum)local_r16_twiddle_table[6] );

		OUT3[0] = D0t[3];
		OUT3[1] = D1t[3]*( (stage2_accum)local_r16_twiddle_table[3] );
		OUT3[2] = D2t[3]*( (stage2_accum)local_r16_twiddle_table[6] );
		OUT3[3] = D3t[3]*( (stage2_accum)local_r16_twiddle_table[9] );


		//////////////////////////////////////// Multiply outputs by Twiddle Factors ////////////////////////////END


		typedef typename bfly_traits<tp_scaling_mode,stage2_accum>::TTR_bflyAccumType stage3_accum;

		typedef typename bfly_traits<tp_scaling_mode,stage3_accum>::TTR_bflyAccumType stage4_accum;

		stage4_accum D00t[4];
		stage4_accum D11t[4];
		stage4_accum D22t[4];
		stage4_accum D33t[4];

		#pragma HLS ARRAY_PARTITION variable=D00t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D11t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D22t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D33t complete dim=1

		stage4_accum accum00;
		stage4_accum accum11;
		stage4_accum accum22;
		stage4_accum accum33;

////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////START


		//// Calculate radix4 butterfly of Sequence OUT0
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				stage2_accum twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					stage2_accum prd =OUT0[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum00 = prd;
				}
				else
				{
					stage2_accum prd   =(OUT0[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum00 =  accum00 + (stage4_accum)prd;
				}

			}

			D00t[i] = accum00;
		}

		//// Calculate radix4 butterfly of Sequence OUT1
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				stage2_accum twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					stage2_accum prd =OUT1[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum11 = prd;
				}
				else
				{
					stage2_accum prd   =(OUT1[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum11 =  accum11 + (stage4_accum)prd;
				}

			}

			D11t[i] = accum11;
		}

		//// Calculate radix4 butterfly of Sequence OUT2
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				stage2_accum twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					stage2_accum prd =OUT2[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum22 = prd;
				}
				else
				{
					stage2_accum prd   =(OUT2[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum22 =  accum22 + (stage4_accum)prd;
				}

			}

			D22t[i] = accum22;
		}

		//// Calculate radix4 butterfly of Sequence OUT3
		for (int  i= 0;  i<4; i++)
		{
		#pragma HLS UNROLL


			for (int  j= 0; j < 4; j++)
			{
		#pragma HLS UNROLL

				stage2_accum twiddle_factor = local_r8_kernel[i][j];
				if(j==0)
				{
					stage2_accum prd =OUT3[j]*twiddle_factor;//local_r8_kernel[i][j];
					accum33 = prd;
				}
				else
				{
					stage2_accum prd   =(OUT3[j] * twiddle_factor);// local_r8_kernel[i][j]);
					 accum33 =  accum33 + (stage4_accum)prd;
				}

			}

			D33t[i] = accum33;
		}
		unsigned int count=0;

		for (int i = 0; i < 4; i++)
		{
		#pragma HLS UNROLL
			bflyOutdata[count++]=D00t[i];
			bflyOutdata[count++]=D11t[i];
			bflyOutdata[count++]=D22t[i];
			bflyOutdata[count++]=D33t[i];

		}



////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END
////////////////////////////////////////////////////////// Calculate Butterflies for 2nd Stage////////////////////////////////////////////////////////////END


	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++Length 32 fft kernel ++++++++++++++++++++++++++++++++++++++++++
    // Implemented using two 16-fft kernels and one raidx-2 merging stage , 16-fft kernels are implemented using two stage radix 4 algorithm


	template
		<
			scaling_mode_enum tp_scaling_mode,
			typename  T_bflyIn,
			typename T_bflyOut,
			typename T_twiddle
		>

	void __parallel_fft_kernel_L32_R16x2xS2
										  (
											T_bflyIn			bflyIndata[32],
											T_bflyOut		bflyOutdata[32]												  )

	{
		const T_twiddle local_r32_twiddle_table[16] =  {
														 T_twiddle( 1.000000000000000 , 0.000000000000000 ),
														 T_twiddle( 0.980785280403230 , -0.195090322016128 ),
														 T_twiddle( 0.923879532511287 , -0.382683432365090 ),
														 T_twiddle( 0.831469612302545 , -0.555570233019602 ),
														 T_twiddle( 0.707106781186547 , -0.707106781186547 ),
														 T_twiddle( 0.555570233019602 , -0.831469612302545 ),
														 T_twiddle( 0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( 0.195090322016128 , -0.980785280403230 ),
														 T_twiddle( 0.000000000000000 , -1.000000000000000 ),
														 T_twiddle( -0.195090322016128 , -0.980785280403230 ),
														 T_twiddle( -0.382683432365090 , -0.923879532511287 ),
														 T_twiddle( -0.555570233019602 , -0.831469612302545 ),
														 T_twiddle( -0.707106781186547 , -0.707106781186547 ),
														 T_twiddle( -0.831469612302545 , -0.555570233019602 ),
														 T_twiddle( -0.923879532511286 , -0.382683432365090 ),
														 T_twiddle( -0.980785280403230 , -0.195090322016128 ),
													};
		#pragma HLS ARRAY_PARTITION variable=local_r32_twiddle_table complete dim=1


		typedef typename bfly_traits<tp_scaling_mode,T_bflyIn>::TTR_bflyAccumType stage1_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage1_accum>::TTR_bflyAccumType stage2_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage2_accum>::TTR_bflyAccumType stage3_accum;
		typedef typename bfly_traits<tp_scaling_mode,stage3_accum>::TTR_bflyAccumType stage4_accum;

		stage4_accum D0t[16];
		stage4_accum D2t[16];
		#pragma HLS ARRAY_PARTITION variable=D2t complete dim=1
		#pragma HLS ARRAY_PARTITION variable=D0t complete dim=1

		stage2_accum accum;
		stage2_accum accum2;

		/// Get Sequence of EVEN number samples from input
		T_bflyIn D0[16];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<16; i++)
		{
		#pragma HLS UNROLL
			D0[i] = bflyIndata[i*2];
		}
		////////////////////////////////////////////////////
		T_bflyIn D2[16];
		#pragma HLS ARRAY_PARTITION variable=D0 complete dim=1

		for(int i=0; i<16; i++)
		{
		#pragma HLS UNROLL
			D2[i] = bflyIndata[i*2+1];
		}
		////////////////////////////////////////////////////
		__parallel_fft_kernel_L16_R4x4xS2<tp_scaling_mode,T_bflyIn, stage4_accum,T_twiddle >(D0,D0t);
		__parallel_fft_kernel_L16_R4x4xS2<tp_scaling_mode,T_bflyIn, stage4_accum,T_twiddle >(D2,D2t);

		//// Multiply by twiddle factors D2t only/////////////////////
		for (int i = 0; i < 16; i++)
		{
		#pragma HLS UNROLL

			//stage4_accum twiddle_factor = local_r32_twiddle_table[i];
			//D2t[i] = D2t[i]*twiddle_factor;//twiddle_factor;//local_r32_twiddle_table[i];
			complex_multiply(D2t[i],local_r32_twiddle_table[i],D2t[i]);

			//CHECK_COVEARAGE;
		}
		//// Merger two radix 16 FFT using raidx 16 radix 2 butterflies and twiddle tables
		for (int i = 0; i < 16; i++)
		{
		#pragma HLS UNROLL
			bflyOutdata[i]    =  D0t[i] + D2t[i];
			bflyOutdata[i+16] =  D0t[i] - D2t[i];
		}

	}

	template<int tp_R>
	struct Butterfly
	{

		template 	<
									int tp_L,
									scaling_mode_enum tp_scaling_mode,
									typename  T_bflyIn,
									typename T_bflyOut,
									typename T_twiddle
					>
		void calcButterFly( T_bflyIn			bflyIndata[tp_R],
							T_bflyOut		bflyOutdata[tp_R],
							T_twiddle 		p_twiddleTable[tp_R]
						  );
	};
	//++++++++++++++++++++++++++++Specialization for Radix=4 Parallel FFT++++++++++++++++++++++++++++++++++Start+++
	template <>
	struct Butterfly<4>
	{

		template 	<
									int tp_L,
									scaling_mode_enum tp_scaling_mode,
									typename  T_bflyIn,
									typename T_bflyOut,
									typename T_twiddle
					>
		void calcButterFly( T_bflyIn			bflyIndata[4],
							T_bflyOut		bflyOutdata[4],
							T_twiddle 		p_twiddleTable[4]
						  )
		{
		//#pragma HLS PIPELINE II=1

			#pragma HLS INLINE recursive

			__parallel_fft_kernel_L4_R4x1_atree<tp_scaling_mode, T_bflyIn,T_bflyOut,T_twiddle>(bflyIndata,bflyOutdata);
			//__parallel_fft_kernel_L4_R4x1<tp_scaling_mode, T_bflyIn,T_bflyOut,T_twiddle>(bflyIndata,bflyOutdata);
		}


	};

	//++++++++++++++++++++++++++++Specialization for Radix=8 Parallel FFT++++++++++++++++++++++++++++++++++End++++


	//++++++++++++++++++++++++++++Specialization for Radix=8 Parallel FFT++++++++++++++++++++++++++++++++++Start+++
	template <>
	struct Butterfly<8>
	{

		template 	<
									int tp_L,
									scaling_mode_enum tp_scaling_mode,
									typename  T_bflyIn,
									typename T_bflyOut,
									typename T_twiddle
					>
		void calcButterFly( T_bflyIn			bflyIndata[8],
							T_bflyOut		bflyOutdata[8],
							T_twiddle 		p_twiddleTable[8]
						  )
		{

			#pragma HLS INLINE recursive

			__parallel_fft_kernel_L8_R4x2_atree<tp_scaling_mode, T_bflyIn,T_bflyOut,T_twiddle>(bflyIndata,bflyOutdata);
			//__parallel_fft_kernel_L8_R4x2<tp_scaling_mode, T_bflyIn,T_bflyOut,T_twiddle>(bflyIndata,bflyOutdata);
		}


	};

	//++++++++++++++++++++++++++++Specialization for Radix=8 Parallel FFT++++++++++++++++++++++++++++++++++End++++


	//++++++++++++++++++++++++++++Specialization for Radix=16 Parallel FFT++++++++++++++++++++++++++++++++++Start+++
	template <>
	struct Butterfly<16>
	{

		template 	<
						int tp_L,
						scaling_mode_enum tp_scaling_mode,
						typename  T_bflyIn,
						typename T_bflyOut,
						typename T_twiddle
					>
		void calcButterFly(
							T_bflyIn			bflyIndata[16],
							T_bflyOut		bflyOutdata[16],
							T_twiddle 		p_twiddleTable[16]
					  	  )
		{
			#pragma HLS INLINE recursive
			 __parallel_fft_kernel_L16_R4x4xS2_atree<tp_scaling_mode,T_bflyIn, T_bflyOut,T_twiddle >(bflyIndata,bflyOutdata);
			 //__parallel_fft_kernel_L16_R4x4xS2<tp_scaling_mode,T_bflyIn, T_bflyOut,T_twiddle >(bflyIndata,bflyOutdata);
		}
	};

	//++++++++++++++++++++++++++++Specialization for Radix=32 Parallel FFT++++++++++++++++++++++++++++++++++End++++


	//++++++++++++++++++++++++++++Specialization for Radix=32 Parallel FFT++++++++++++++++++++++++++++++++++Start+++
	template <>
	struct Butterfly<3>
	{
		template 	<
						int tp_L,
						scaling_mode_enum tp_scaling_mode,
						typename  T_bflyIn,
						typename T_bflyOut,
						typename T_twiddle
					>
		void calcButterFly(
							T_bflyIn			bflyIndata[32],
							T_bflyOut		bflyOutdata[32],
							T_twiddle 		p_twiddleTable[32]
						  )
		{
			#pragma HLS INLINE recursive
			__parallel_fft_kernel_L32_R16x2xS2<tp_scaling_mode,T_bflyIn, T_bflyOut,T_twiddle >(bflyIndata,bflyOutdata);
		}
	};

	//++++++++++++++++++++++++++++Specialization for Radix=16 Parallel FFT++++++++++++++++++++++++++++++++++End++++

	template <int tp_R>
	template<
				int tp_L,
				scaling_mode_enum tp_scaling_mode,
				typename  T_bflyIn,
				typename T_bflyOut,
				typename T_twiddle
			>

	void Butterfly<tp_R>::calcButterFly(
										T_bflyIn		bflyIndata[tp_R],
										T_bflyOut		bflyOutdata[tp_R],
										T_twiddle 		p_twiddleTable[tp_R]
									)
	{
			#pragma HLS INLINE recursive

				L_CALC_R_BUTTERFLY_SAMPLE: // [```````` ONE BUTTERFLY`````]
				for (int r = 0;r < tp_R;r++)
				{
			#pragma HLS UNROLL    // Unroll butterfly outer Loop Required for proper implementation of SSR FFT
					T_bflyIn  bflyInType;
					T_bflyOut sop;
					sop = 0;
					T_bflyIn  product_vector[tp_R];
					#pragma HLS ARRAY_PARTITION variable=product_vector complete dim=1
					#pragma HLS DATA_PACK variable=product_vector

					L_CALC_ONE_BUTTERFLY_SAMPLE:
					for (int r1 = 0;r1 < tp_R;r1++) //[```````` ONE SAMPLE IN BUTTERFLY`````]
					{
			#pragma HLS UNROLL   // Unroll butterfly inner Loop Required for proper implementation of SSR FFT
						//replaced//int twiddleIndex = (r1*r) % tp_R;
						int twiddleIndex = (r1*r)  & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);
						complex_multiply(bflyIndata[r1],p_twiddleTable[twiddleIndex],product_vector[r1]);
					}
					AdderTreeClass<tp_R> AdderTreeClass_obj;
					AdderTreeClass_obj.template createTreeLevel<0,tp_scaling_mode>(product_vector,bflyOutdata[r]);
				}
    }//calcButterfly_function


	template 	<
					int tp_L, int tp_R,
					typename  T_bflyIn,
					typename T_bflyOut,
					typename T_twiddle
				>
	void calcButterFly_WithoutAdderTree( T_bflyIn			bflyIndata[tp_R],
						T_bflyOut		bflyOutdata[tp_R],
						T_twiddle 		p_twiddleTable[tp_R]
					  )
	{
			#pragma HLS INLINE recursive
				 // inlined butterfly in each stage

		L_CALC_R_BUTTERFLY_SAMPLE: // [```````` ONE BUTTERFLY`````]
		for (int r = 0;r < tp_R;r++)
		{
		#pragma HLS UNROLL    // Unroll butterfly outer Loop Required for proper implementation of SSR FFT
			T_bflyIn  bflyInType;
			T_bflyOut sop;
			sop = 0;

			L_CALC_ONE_BUTTERFLY_SAMPLE:
			for (int r1 = 0;r1 < tp_R;r1++) //[```````` ONE SAMPLE IN BUTTERFLY`````]
			{
		#pragma HLS UNROLL   // Unroll butterfly inner Loop Required for proper implementation of SSR FFT

				//repalced//int twiddleIndex = (r1*r) % tp_R;
				int twiddleIndex = (r1*r)  & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);
				T_bflyIn tw_factor;
				T_bflyIn in_sample;

				tw_factor= (p_twiddleTable[twiddleIndex]);
				in_sample = bflyIndata[r1];

				T_bflyOut  temp;
				temp = in_sample * tw_factor;

				sop = sop + temp;
			}
			bflyOutdata[r] = sop;
		}
	}//calcButterfly_function
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} //namespace hls
} //namespace ssr_fft

#endif
