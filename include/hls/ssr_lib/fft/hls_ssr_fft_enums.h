//File Name : hls_ssr_fft_enums.h
#ifndef HLS_SSR_FFT_ENUMS_H_
#define HLS_SSR_FFT_ENUMS_H_


/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

		scaling_mode_enum is defined to enumerate all of the possible scaling
		modes. 3 different scaling modes are supported.
				1.  SSR_FFT_NO_SCALING        : in this mode no scaling is done
				2.  SSR_FFT_SCALE             : in this mode scaling done in all stages
				3.  SSR_FFT_GROW_TO_MAX_WIDTH : Grow to max width and saturate

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

#include <ap_fixed.h>
#include <complex>
namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*===================================================================================
 * Enumeration Defines Saturation Modes for SSR FFT Datapath, Specifically related to
 * Butterfly computations. Butterfly computation are essentially Sum of product
 * operation and terminate at an adder tree resulting in bit growth. Different options
 * are available to deal with this bit growth.
 * 1- Do not scale the output and allow progressive bit growth in all FFT stages
 * 2- Do not scale the output and allow progressive bit growth but only until the bit-
 * width reaches 27 bits which is the max number of bit on ultrascale DSP48 slices for
 * multiplier input, so that multiplication operation can fit on these DSPs.
 * 3-Scale the outputs after summation so that there is no effective bit-growth
 * ===================================================================================*/
	enum scaling_mode_enum
	{
		SSR_FFT_NO_SCALING,
		SSR_FFT_GROW_TO_MAX_WIDTH,
		SSR_FFT_SCALE
	};

	struct ssr_fft_default_params {
		// Scaling Mode Selection

		static const int N=1024;
		static const int R=4;
		static const scaling_mode_enum scaling_mode=SSR_FFT_NO_SCALING;
		// Twiddle and Complex Exponential Tables : Effectively sin/cos storage resolution
		static const int twiddle_table_word_length=18;
		static const int twiddle_table_intger_part_length=2;  // 2 bits are selected to store +1/-1 correctly

	};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //namespace hls
} //namespace ssr_fft



#endif //HLS_SSR_FFT_ENUMS_H_
