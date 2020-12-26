//File Name :  hls_ssr_fft_complex_exp_table.h
#ifndef HLS_SSR_FFT_COMPLEX_EXP_TALBE_H_
#define HLS_SSR_FFT_COMPLEX_EXP_TALBE_H_

//#define HLS_SSR_FFT_COMPLEX_EXP_TALBE_PRINT_DEBUG_MESSAGES
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
-_-  is used for initialization of complex exp table, with given length,the template  -_-
-_-  parameter is named tp_R that captures the length of table because for ssr fft    -_-
-_-  the complex exponential table has a length equal to radix of fft being           -_-
-_-  calculated.                                                                      -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */

#ifdef HLS_SSR_FFT_COMPLEX_EXP_TALBE_PRINT_DEBUG_MESSAGES
#include <iostream>
#endif

#include <ap_fixed.h>

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	template <int tp_R,typename tt_dtype>
	struct complex_exp_table
	{

	};
	//spec-1
	template <int tp_R,typename tt_dtype>
	struct complex_exp_table<tp_R,std::complex < tt_dtype > >
	{
		static void initComplexExp(std::complex < tt_dtype > table[])
		{
		#pragma HLS INLINE off

			if(tp_R > 2)
			for (int i = 0; i < tp_R; i++)
			{
				double real = cos((2 * i*M_PI) / tp_R);
				double imag = -sin((2 * i*M_PI) / tp_R);
				table[i].real(real);
				table[i].imag(imag);
			}
			else
			for (int i = 0; i < 4; i++)
			{
				//double phase =(2 * i*M_PI) / tp_R ;
				double real = cos((2 * i*M_PI) / tp_R);
				double imag = -sin((2 * i*M_PI) / tp_R);
				table[i].real(real);
				table[i].imag(imag);

			}
		}

	};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //end name space hls
} //end name space ssr_fft

#endif //HLS_SSR_FFT_COMPLEX_EXP_TALBE_H_
