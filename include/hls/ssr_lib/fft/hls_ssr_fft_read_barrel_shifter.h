//hls_ssr_fft_read_barrel_shifter.h

#ifndef HLS_SSR_FFT_READ_BARREL_SHIFTER_H_
#define HLS_SSR_FFT_READ_BARREL_SHIFTER_H_


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

	The digitReversedDataReOrder function is used to perform data re ordering at
	the output of final stage in SSR FFT. The data produced by final stage is
	shuffled in SSR dimension ( R or SSR streams coming from SSR FFT last stage)
	and also it needs re ordering in time dimension. The re or ordering is done
	using two PIPO buffers, in 4 phases.

     	 	 	 1- The input streams are rotated in SSR dimension ( R Streams)
     	 	 	 2- The input stream written to PIPO after rotation
     	 	 	 3- The ping pong memory is read
     	 	 	 4- The read data is shuffled and written to output
                    This file defines functions for phases 1,2,3
                    cacheData function : deals with phase 1 and 2
                    WriteCacheData  function deals with phase  3,4
                    and internally calls
                    readBarrelShifter::memReadBarrelShifter
     Note : This function only deals with the cases when tp_L is integer power of
     tp_R , for the cases when tp_L is not integer power of tp_R :
                    	OutputDataReOrder<>::digitReversal2Phase<> is used which
                    	deals with it through different specializations.

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
namespace hls {
	namespace ssr_fft {

		template <int stage>
		struct readBarrelShifter
		{
			template <int tp_R, int tp_L, typename tt_T>
			void memReadBarrelShifter(int lindex,int offset,tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R] );
		};



		template <int stage>
		template <int tp_R, int tp_L, typename tt_T>
		void readBarrelShifter<stage>::memReadBarrelShifter(int lindex,int offset,tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
			const int c_shift = (ssr_fft_log2<tp_L>::val)-(ssr_fft_log2<tp_R>::val);
			const int log2_radix = (ssr_fft_log2<tp_R>::val);

				if(offset==(stage-1))
				{
					for (int c = 0; c < tp_R; c++)
					{
		#pragma HLS UNROLL
					unsigned int lin_index = (lindex<<log2_radix) | c;  // equivalent to : r*tp_R + c;
					unsigned int bitReversedIndex = digitReversal<tp_L, tp_R>(lin_index);
					unsigned int out_r = bitReversedIndex >> log2_radix ;   // equivalent to :  bitReversedIndex / tp_R;
					//replaced//out[c]= in[(c+(stage-1))%tp_R][out_r];
					out[c]= in[(c+(stage-1))& (ssr_fft_log2BitwiseAndModMask<tp_R>::val)][out_r];
					}
				}
				else
				{
					readBarrelShifter<stage-1> obj;
					obj.template memReadBarrelShifter<tp_R,tp_L,tt_T>(lindex,offset,in,out);
				}
		}




		template <>
		template <int tp_R, int tp_L, typename tt_T>
		void readBarrelShifter<1>::memReadBarrelShifter(int lindex,int offset,tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
			const int c_shift = (ssr_fft_log2<tp_L>::val)-(ssr_fft_log2<tp_R>::val);
			const int log2_radix = (ssr_fft_log2<tp_R>::val);

					for (int c = 0; c < tp_R; c++)
					{
						#pragma HLS UNROLL
						unsigned int lin_index = (lindex<<log2_radix) | c;  // equivalent to : r*tp_R + c;
						unsigned int bitReversedIndex = digitReversal<tp_L, tp_R>(lin_index);
						unsigned int out_r = bitReversedIndex >> log2_radix ;   // equivalent to :  bitReversedIndex / tp_R;
						out[c]= in[(c+(1-1))%tp_R][out_r];
					}
		}
	} //namespace hls
} //namespace ssr_fft



#endif //HLS_SSR_FFT_READ_BARREL_SHIFTER_H_
