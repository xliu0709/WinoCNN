//hls_ssr_fft_data_commute_barrel_shifter.h

#ifndef HLS_SSR_FFT_DATA_COMMUTE_BARREL_SHIFTER_H_
#define HLS_SSR_FFT_DATA_COMMUTE_BARREL_SHIFTER_H_

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

	The datacommoutor is a basic block used to implement data re ordering
     at the input SSR FFT and als in between the SSR FFT Stages. The data commutor
     has two stages one read in R streams multiplexing them before storage to ping
     pong buffers in a circular rotate(word level rotation not bit like) fashion.
     The 2nd stage reads in R memories and streams it out to R different streams.
     The memory to stream mapping changes in every cycle. The Whole transformation
     is 4 phase:
     	 	 	 1- The input streams are rotated
     	 	 	 2- The input stream written to PIPO after rotation
     	 	 	 3- The ping pong memory is read
     	 	 	 4- The read data is shuffled and written to output
                    commuteBarrelShifter::memReadCommuteBarrelShifter
                    defined in this file
                    is used in phase 4.
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

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



		template <int stage>
		struct commuteBarrelShifter
		{
			template <int tp_R, int tp_L,int tp_PF, typename tt_T>
			void memReadCommuteBarrelShifter(int offset,int p,tt_T inin[tp_R][tp_PF*tp_R], tt_T out[tp_R] );
		};
		template <int stage>
		template <int tp_R, int tp_L,int tp_PF, typename tt_T>
		void commuteBarrelShifter<stage>::memReadCommuteBarrelShifter(int offset,int p,tt_T in[tp_R][tp_PF*tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
				if(offset==(stage-1))
				{

					for (int c = 0; c < tp_R; c++)
					{
		#pragma HLS UNROLL
						//replaced//unsigned int out_r = c*tp_PF + p ;   // equivalent to :  bitReversedIndex / tp_R;
						unsigned int out_r = (c<<ssr_fft_log2<tp_PF>::val) + p ;   // equivalent to :  bitReversedIndex / tp_R;

						//replaced // out[c]= in[(c+(stage-1))%tp_R][out_r];
						out[c]= in[(c+(stage-1))& ssr_fft_log2BitwiseAndModMask<tp_R>::val][out_r];

					}

				}
				else
				{
					commuteBarrelShifter<stage-1> obj;
					obj.template memReadCommuteBarrelShifter<tp_R,tp_L,tp_PF,tt_T>(offset,p,in,out);
				}
		}

		template <>
		template <int tp_R, int tp_L,int tp_PF, typename tt_T>
		void commuteBarrelShifter<1>::memReadCommuteBarrelShifter(int offset,int p,tt_T in[tp_R][tp_PF*tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
			const int c_shift = (ssr_fft_log2<tp_L>::val)-(ssr_fft_log2<tp_R>::val);
			const int log2_radix = (ssr_fft_log2<tp_R>::val);


					for (int c = 0; c < tp_R; c++)
					{
		#pragma HLS UNROLL

						//replaced//unsigned int out_r = c*tp_PF + p ;   // equivalent to :  bitReversedIndex / tp_R;
						unsigned int out_r = (c<<ssr_fft_log2<tp_PF>::val) + p ;   // equivalent to :  bitReversedIndex / tp_R;
						//replaced//out[c]= in[(c+(1-1))%tp_R][out_r];
						out[c]= in[(c+(1-1))& ssr_fft_log2BitwiseAndModMask<tp_R>::val][out_r];
					}
		}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// PF=1 Specialization //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		template <int stage>
		struct commuteBarrelShifter_pf1
		{
			template <int XF,int tp_R, int tp_L,int tp_PF, typename tt_T>
			void memReadCommuteBarrelShifter_pf1(int offset,int p,tt_T in[tp_R][XF*tp_PF*tp_R], tt_T out[tp_R] );
		};
		template <int stage>
		template <int XF,int tp_R, int tp_L,int tp_PF, typename tt_T>
		void commuteBarrelShifter_pf1<stage>::memReadCommuteBarrelShifter_pf1(int offset,int p,tt_T in[tp_R][XF*tp_PF*tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
				if((offset%tp_R)==(stage-1))
				{

					for (int c = 0; c < tp_R; c++)
					{
		#pragma HLS UNROLL
						//replaced//unsigned int out_r = c*tp_PF + p ;   // equivalent to :  bitReversedIndex / tp_R;
						unsigned int out_r = (c<<ssr_fft_log2<tp_PF>::val) + p  + ((offset/tp_R)*tp_R);   // equivalent to :  bitReversedIndex / tp_R;

						//replaced // out[c]= in[(c+(stage-1))%tp_R][out_r];
						out[c]= in[(c+(stage-1))& ssr_fft_log2BitwiseAndModMask<tp_R>::val][out_r];

					}

				}
				else
				{
					commuteBarrelShifter_pf1<stage-1> obj;
					obj.template memReadCommuteBarrelShifter_pf1<XF,tp_R,tp_L,tp_PF,tt_T>(offset,p,in,out);
				}
		}

		template <>
		template <int XF,int tp_R, int tp_L,int tp_PF, typename tt_T>
		void commuteBarrelShifter_pf1<1>::memReadCommuteBarrelShifter_pf1(int offset,int p,tt_T in[tp_R][XF*tp_PF*tp_R], tt_T out[tp_R] )
		{
		#pragma HLS INLINE
			const int c_shift = (ssr_fft_log2<tp_L>::val)-(ssr_fft_log2<tp_R>::val);
			const int log2_radix = (ssr_fft_log2<tp_R>::val);


					for (int c = 0; c < tp_R; c++)
					{
		#pragma HLS UNROLL

						//replaced//unsigned int out_r = c*tp_PF + p ;   // equivalent to :  bitReversedIndex / tp_R;
						unsigned int out_r = (c<<ssr_fft_log2<tp_PF>::val) + p + ((offset/tp_R)*tp_R) ;   // equivalent to :  bitReversedIndex / tp_R;
						//replaced//out[c]= in[(c+(1-1))%tp_R][out_r];
						out[c]= in[(c+(1-1))& ssr_fft_log2BitwiseAndModMask<tp_R>::val][out_r];
					}
		}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //end name space hls
} //end name space ssr_fft
#endif //HLS_SSR_FFT_DATA_COMMUTE_BARREL_SHIFTER_H_
