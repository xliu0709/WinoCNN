//File Name : hls_ssr_fft_complex_exp_table_last_stage.h
#ifndef HLS_SSR_FFT_COMPLEX_EXP_TALBE_LAST_STAGE_H_
#define HLS_SSR_FFT_COMPLEX_EXP_TALBE_LAST_STAGE_H_
#include <ap_fixed.h>

//#define HLS_SSR_FFT_COMPLEX_EXP_TALBE_LAST_STAGE_PRINT_DEBUG_MESSAGES
//#define HLS_SSR_FFT_DEBUG
/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-  initComplexExpTable Specialization is defined here, in general this function     -_-
-_-  is used for initialization of complex exp table, with given length, this spec    -_-
-_-  is written to deal with the case when actual table length and the length used   -_-
-_-  in calculating the phase factor is different. HLS have some synthesis issues     -_-
-_-  for mapping -sin/cose tables to ROM memory for length smaller then 4, so this     -_-
-_-  spec is created to deal with this case. All tables are kept of length 4 or       -_-
-_-  larger to deal with this issue while calling the function original length is     -_-
-_-  passed but internally the minimum table length is used.                           -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */
#ifdef HLS_SSR_FFT_COMPLEX_EXP_TALBE_LAST_STAGE_PRINT_DEBUG_MESSAGES
#include <iostream>
#endif

namespace hls
{
namespace ssr_fft
{
	template <int tp_R,int tp_realLength,typename tt_dtype>
	struct complex_exp_table_last_stage
	{

	};
	//spec-1
	template <int tp_R,int tp_realLength,typename tt_dtype>
	struct complex_exp_table_last_stage<tp_R,tp_realLength,std::complex < tt_dtype > >
	{
		static void initComplexExp(std::complex < tt_dtype > table[])
		{
		#pragma HLS INLINE off

			for (int i = 0; i < tp_R; i++)
			{
				double real = cos((2 * i*M_PI) / tp_realLength);
				double imag = -sin((2 * i*M_PI) / tp_realLength);
				table[i].real(real);
				table[i].imag(imag);
			}
		}
	};
} //end name space hls
} //end name space ssr_fft



#endif //HLS_SSR_FFT_COMPLEX_EXP_TALBE_LAST_STAGE_H_

