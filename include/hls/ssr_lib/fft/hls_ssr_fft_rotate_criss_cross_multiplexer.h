//File Name:hls_ssr_fft_rotate_criss_cross_multiplexer.h

#ifndef HLS_SSR_FFT_ROTATE_CRISS_CROSS_MULTIPLEXER_H_
#define HLS_SSR_FFT_ROTATE_CRISS_CROSS_MULTIPLEXER_H_

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

namespace hls {
	namespace ssr_fft {


		template <int stage>
		struct rotateCrissCrossMultiplexerClass
		{
			template <int tp_R, int tp_L, typename tt_T,typename tt_T2>
			void rotateCrissCrossMultiplexer(int timeIndexAddressOffset,int ssrDimensionAddressOffset,tt_T in[tp_R][tp_L/tp_R], tt_T2 out[tp_R] );
		};



		template <int stage>
		template <int tp_R, int tp_L, typename tt_T,typename tt_T2>
		void rotateCrissCrossMultiplexerClass<stage>::rotateCrissCrossMultiplexer(int timeIndexAddressOffset,int ssrDimensionAddressOffset,tt_T in[tp_R][tp_L/tp_R], tt_T2 out[tp_R] )
		{
		#pragma HLS INLINE
			const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
			const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;

				if(ssrDimensionAddressOffset==(stage-1))
				{
					SSR_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
		#pragma HLS UNROLL

						ap_uint<  ssr_fft_log2<tp_R>::val > rotated_r = r;
						rotated_r.lrotate(outputShuffleAmount);
						//replaced//int pingPongSuperSampleIndex = ((stage-1) + rotated_r) % tp_R;
						int pingPongSuperSampleIndex = ((stage-1) + rotated_r) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);

						//CHECK_COVEARAGE;
						//replaced//int pingPongTimeIndex=(r+timeIndexAddressOffset)%(tp_L/tp_R);
						int pingPongTimeIndex=(r+timeIndexAddressOffset)& (ssr_fft_log2BitwiseAndModMask<tp_L/tp_R>::val);


					out[r]= in[pingPongSuperSampleIndex][pingPongTimeIndex];
					}
				}
				else
				{
					rotateCrissCrossMultiplexerClass<stage-1> obj;
					obj.template rotateCrissCrossMultiplexer<tp_R,tp_L,tt_T,tt_T2>(timeIndexAddressOffset,ssrDimensionAddressOffset,in,out);
				}
		}




		template <>
		template <int tp_R, int tp_L, typename tt_T,typename tt_T2>
		void rotateCrissCrossMultiplexerClass<1>::rotateCrissCrossMultiplexer(int timeIndexAddressOffset,int ssrDimensionAddressOffset,tt_T in[tp_R][tp_L/tp_R], tt_T2 out[tp_R] )
		{
		#pragma HLS INLINE
			const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
			const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;

			SSR_LOOP:
						for (unsigned int r = 0; r < tp_R; r++)
						{
			#pragma HLS UNROLL

							ap_uint<  ssr_fft_log2<tp_R>::val > rotated_r = r;
							rotated_r.lrotate(outputShuffleAmount);
							//replaced//int pingPongSuperSampleIndex = ((1-1) + rotated_r) % tp_R;
							int pingPongSuperSampleIndex = ((1-1) + rotated_r) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);

							//replaced//int pingPongTimeIndex=(r+timeIndexAddressOffset)%(tp_L/tp_R);
							int pingPongTimeIndex=(r+timeIndexAddressOffset)& (ssr_fft_log2BitwiseAndModMask<(tp_L/tp_R)>::val);
							//CHECK_COVEARAGE;

						out[r]= in[pingPongSuperSampleIndex][pingPongTimeIndex];
						}
		}

	} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFT_ROTATE_CRISS_CROSS_MULTIPLEXER_H_
