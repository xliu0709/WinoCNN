//hls_ssr_fft_fork_merge_utils.h
#ifndef HLS_SSR_FFT_FORK_MERGE_UTILS_H_
#define HLS_SSR_FFT_FORK_MERGE_UTILS_H_
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_utilities.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"

#endif
/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

			SSR FFT in last few stages break its kernel into small FFT kernel
			by factor called forking factor. Generally most of the SSR FFT
			block in dataflow pipeline process blocks of data sized [tp_R][tp_L/tp_R]
			but for certain special cases when tp_L is not integer power of tp_R
			in last stage the large FFT kernel block breaks in small kernels of
			whose size is smaller by factor called forking factor. So dataflow pipeline
			that is processing blocks of [tp_R][tp_L/tp_R] forks into multiple
			streams each of size [tp_R/tp_forkingFactor][tp_L/tp_R]. This forking
			happens is fft stage kernel and also in blocks that do data re oredering.
			These utility functions are used to create a smaller forked stream
			and also to merge a small stream into big one for final output.

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

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
	void copyToLocalBuff(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R/tp_forkingFactor][tp_L/tp_R])
	{
	#pragma HLS INLINE off

		const int fork_width = tp_R/tp_forkingFactor;
		const int offset     = (tp_forkNumber-1) * fork_width;

		//CHECK_COVEARAGE;


		for (int t = 0; t < tp_L/tp_R;t++)
		{
	#pragma HLS PIPELINE II=1 rewind
			for (int r = 0; r < tp_R/tp_forkingFactor; r++)
			{
	#pragma HLS UNROLL

				out[r][t]=in[offset + r][t];
			}
		}

	}

	template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
	void copyFromLocalBuffToOuput(tt_T in[tp_R/tp_forkingFactor][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
	{
	#pragma HLS INLINE off

		const int fork_width = tp_R/tp_forkingFactor;
		const int offset     = (tp_forkNumber-1) * fork_width;
		//CHECK_COVEARAGE;
		for (int t = 0; t < tp_L/tp_R;t++)
		{
	#pragma HLS PIPELINE II=1 rewind
			for (int r = 0; r < tp_R/tp_forkingFactor; r++)
			{
	#pragma HLS UNROLL

				//out[t][2*tp_forkNumber-1-r]=in[t][tp_R/tp_forkingFactor-1-r];
	//			out[t][offset+r]=in[t][r];
				out[(r<<(ssr_fft_log2<tp_forkingFactor>::val)) + tp_forkNumber-1][t]=in[r][t];


			}
		}

	}


	template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
	void copyFromLocalBuffToOuput_NI(tt_T in[tp_R/tp_forkingFactor][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
	{
	#pragma HLS INLINE off
		const int fork_width = tp_R/tp_forkingFactor;
		const int offset     = (tp_forkNumber-1) * fork_width;

		//CHECK_COVEARAGE;

		for (int t = 0; t < tp_L/tp_R;t++)
		{
	#pragma HLS PIPELINE II=1 rewind
			for (int r = 0; r < tp_R/tp_forkingFactor; r++)
			{
	#pragma HLS UNROLL

				out[fork_width*(tp_forkNumber-1) + r][t]=in[r][t];
	//			out[t][offset+r]=in[t][r];
				//out[t][tp_forkingFactor*r + tp_forkNumber-1]=in[t][r];


			}
		}

		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFT_FORK_MERGE_UTILS_H_




