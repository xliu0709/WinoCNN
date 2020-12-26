#ifndef HLS_SSR_FFT_DATA_REORDER_H
#define HLS_SSR_FFT_DATA_REORDER_H


#include <math.h>
#include <iostream>
#include <complex>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_utilities.h"
#include "hls_ssr_fft_read_barrel_shifter.h"
#include "hls_ssr_fft_output_reorder.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_read_barrel_shifter.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_output_reorder.h"

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

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		template <int tp_L, int tp_R,typename tt_dtype>
		void cacheDataDR(std::complex<tt_dtype> inDatac[tp_R][tp_L/tp_R], std::complex<tt_dtype> digitReverseBuff[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off
		/*
		#pragma HLS ARRAY_PARTITION variable=inDatac complete dim=1
		#pragma HLS ARRAY_PARTITION variable=digitReverseBuff complete dim=1
		*/

		const unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);

		cacheDataDR_LOverRLooP:
				for (int r = 0; r <(tp_L / tp_R); r++)
				{
			#pragma HLS PIPELINE II=1 rewind

					std::complex<tt_dtype> temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

					cacheDataDR_SSRLoop1:
					for (int c= 0; c < tp_R; c++)
					{
						//int cdash = (c +  r / ( tp_L / (tp_R*tp_R) )   )%tp_R;
						//replaced//int cdash = (c +  (r*tp_R*tp_R) / ( tp_L  )   )%tp_R;
						int cdash = (c +  ((r) >> ( ssr_fft_log2<tp_L/(tp_R*tp_R)>::val ))   ) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);
						//CHECK_COVEARAGE;

						temp[cdash]=inDatac[c][r];   // Read in Order :: Should be a stream
					}
					cacheDataDR_SSRLoop2:
					for (int c=0; c < tp_R; c++)
					{
						digitReverseBuff[c][r]=temp[c];
					}

				}
		}

		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void cacheDataDR(std::complex<tt_dtype> inDatac[tp_R][tp_L/tp_R], std::complex<tt_dtype2> digitReverseBuff[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off


		const unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);

		cacheDataDR_LOverRLooP:
				for (int r = 0; r <(tp_L / tp_R); r++)
				{
			#pragma HLS PIPELINE II=1 rewind

					std::complex<tt_dtype> temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

					cacheDataDR_SSRLoop1:
					for (int c= 0; c < tp_R; c++)
					{
						//replaced//int cdash = (c +  (r*tp_R*tp_R) / ( tp_L  )   )%tp_R;
						int cdash = (c +  ( r>>(ssr_fft_log2<tp_L/(tp_R*tp_R)>::val) ) ) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);
						//CHECK_COVEARAGE;

						temp[cdash]=inDatac[c][r];   // Read in Order :: Should be a stream
					}
					cacheDataDR_SSRLoop2:
					for (int c=0; c < tp_R; c++)
					{
						digitReverseBuff[c][r]=temp[c];
					}

				}
		}

		template <int tp_L, int tp_R,typename tt_dtype>
		void WriteBackCacheDataDR(std::complex<tt_dtype> digitReverseBuff[tp_R][tp_L/tp_R], std::complex<tt_dtype> outDatac[tp_R][tp_L/tp_R])
		{

			//CHECK_COVEARAGE;
		#pragma HLS INLINE off

			const unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);

			WriteBackCacheDataDR_LOverRLoop:
				for (int r = 0; r <(tp_L / tp_R); r++)
				{
					#pragma HLS PIPELINE II=1 //rewind // This loop has rewind issue : VERIFIED

					std::complex<tt_dtype> temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp


					unsigned int lin_index = (r<<log2_radix)| 0;  // equivalent to : r*tp_R + c;
					unsigned int bitReversedIndex = digitReversal_fracIsLSB<tp_L, tp_R>(lin_index);
					unsigned int out_r = bitReversedIndex >> log2_radix ;   // equivalent to :  bitReversedIndex / tp_R;
					unsigned int out_c = bitReversedIndex & ( (1<<log2_radix) -1 ); // equivalent to:bitReversedIndex % tp_R;
					//int offset = (out_c  +  (out_r  /  ( tp_L / (tp_R*tp_R) )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //



					//int offset = (out_c  +  ( (out_r *tp_R*tp_R) /  ( tp_L  )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //

					//replaced//					int offset = (out_c  +  ( (out_r *tp_R*tp_R) /  ( tp_L  )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //
					int offset = (out_c  +  (out_r >> (ssr_fft_log2<tp_L/(tp_R*tp_R)>::val)) ) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val) ;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //

					readBarrelShifter<tp_R> barrelShifterObj;
					barrelShifterObj.template memReadBarrelShifter< tp_R,tp_L,std::complex<tt_dtype> >(r,offset,digitReverseBuff, temp );
					for (int c= 0; c < tp_R; c++)
					{
						outDatac[c][r]=temp[c];     //outDatac is written in order should be a stream
					}

				}

		}


		template <int tp_L, int tp_R,typename tt_dtype,typename tt_dtype2>
		void WriteBackCacheDataDR(std::complex<tt_dtype> digitReverseBuff[tp_R][tp_L/tp_R], std::complex<tt_dtype2> outDatac[tp_R][tp_L/tp_R])
		{
			//CHECK_COVEARAGE;

		#pragma HLS INLINE off


			const unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);

			WriteBackCacheDataDR_LOverRLoop:
				for (int r = 0; r <(tp_L / tp_R); r++)
				{
					#pragma HLS PIPELINE II=1 //rewind // This loop has rewind issue

					std::complex<tt_dtype> temp[tp_R];
					#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

					unsigned int lin_index = (r<<log2_radix)| 0;  // equivalent to : r*tp_R + c;
					unsigned int bitReversedIndex = digitReversal_fracIsLSB<tp_L, tp_R>(lin_index);
					unsigned int out_r = bitReversedIndex >> log2_radix ;   // equivalent to :  bitReversedIndex / tp_R;
					unsigned int out_c = bitReversedIndex & ( (1<<log2_radix) -1 ); // equivalent to:bitReversedIndex % tp_R;
					//int offset = (out_c  +  (out_r  /  ( tp_L / (tp_R*tp_R) )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //
					//int offset = (out_c  +  ( (out_r*tp_R*tp_R)/( tp_L )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //

					//replaced//					int offset = (out_c  +  ( (out_r *tp_R*tp_R) /  ( tp_L  )    ) ) %tp_R;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //
				    int offset = (out_c  +  (out_r >> (ssr_fft_log2<tp_L/(tp_R*tp_R)>::val)) ) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val) ;//int out_cDash = (out_c  +  (out_r/tp_R) ) %tp_R; // ((r>>log2_radix) + c)%tp_R;     //
					//readBarrelShifter<tp_R> barrelShifterObj;
					//barrelShifterObj.template memReadBarrelShifter< tp_R,tp_L,std::complex<tt_dtype> >(r,offset,digitReverseBuff, temp );
					for (int c= 0; c < tp_R; c++)
					{
					#pragma HLS UNROLL
						unsigned int lin_index1 = (r<<log2_radix) | ((tp_R+ c - offset)%tp_R);  // equivalent to : r*tp_R + c;
						unsigned int bitReversedIndex1 = digitReversal<tp_L, tp_R>(lin_index1);
						unsigned int out_r = bitReversedIndex1 >> log2_radix ;   // equivalent to :  bitReversedIndex / tp_R;
						//replaced//out[c]= in[(c+(stage-1))%tp_R][out_r];
						temp[(tp_R+ c - offset)%tp_R]= digitReverseBuff[c][out_r];
					}
		//			CHECK_COVEARAGE;
					for (int c= 0; c < tp_R; c++)
					{
						outDatac[c][r]=temp[c];     //outDatac is written in order should be a stream
					}

				}

		}

		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void digitReversedDataReOrder(std::complex<tt_dtype> inDatac[tp_R][tp_L/tp_R], std::complex<tt_dtype2> outDatac[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE

			const unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);

		std::complex<tt_dtype> digitReverseBuff[tp_R][tp_L/tp_R];
		#pragma HLS ARRAY_PARTITION variable=digitReverseBuff complete dim=1
		#pragma HLS DATA_PACK variable=digitReverseBuff
			cacheDataDR<tp_L,tp_R,tt_dtype,tt_dtype>(inDatac,digitReverseBuff);
			WriteBackCacheDataDR<tp_L,tp_R,tt_dtype,tt_dtype2>(digitReverseBuff,outDatac);
		}


		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void simModel_DigitalReversal(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		unsigned int ind1=0;
		unsigned int revind=0;


						for (int i = 0;i <tp_R;i++)
						{
							for (int j = 0;j <tp_L / tp_R  ;j++)
							{
								 ind1 = j*(tp_R)+i;
								 revind = digitReversal_fracIsLSB<tp_L, tp_R>(ind1);
								 outDatac[revind%tp_R][revind/tp_R]=inDatac[i][j];
							}
						}
		}

#if 0 // These functions were all used to build initial simulation model for SSR FFT
//===========================================================================================================================
//===========================================================================================================================
//===========================================================================================================================
//===========================================================================================================================

		template < int tp_L, int tp_R,int STAGE,typename tt_inType, typename tt_outType>
		void reOrderData_static(tt_inType inDatac[tp_R][tp_L/tp_R], tt_outType outDatac[tp_R][tp_L/tp_R])
		{
		////#pragma HLS LOOP_MERGE

		//#pragma HLS INLINE off

			const int no_of_fft_in_stage = ssr_fft_pow<tp_R,STAGE>::val; //(int)pow((double)(tp_R), double(STAGE));
			const int fft_len_in_stage = tp_L / no_of_fft_in_stage;
			const int no_bflys_per_fft = fft_len_in_stage / tp_R;
			const int no_samples_per_bfly = tp_R;
			//static_assert(STAGE - 1 <= ((_log2<LEN>::val) - _log2<SSR>::val), "Stage no. should be less or equal of log Radix of FFT length minus 1");


			//assert(STAGE <_log2<LEN>::val/ _log2<SSR>::val);
		FFTs_LOOP:
			for (int f = 0;f < no_of_fft_in_stage;f++)
			{
			BFLYs_LOOP:
				for (int bf = 0;bf < no_bflys_per_fft;bf++)

		//#pragma HLS PIPELINE
			{
				RADIX_LOOP:
					for (int r = 0;r < no_samples_per_bfly;r++)

		//#pragma HLS UNROLL
			{
						// butterfly number r sample;
						int in_index = (fft_len_in_stage*f + bf*tp_R + r);
						int idx1 = in_index / (tp_R);
						int idx2 = in_index % tp_R;
						int out_index = (bf + r*(fft_len_in_stage / tp_R) + (fft_len_in_stage*f));
						int odx1 = out_index / ( tp_R);
						int odx2 = out_index % tp_R;

		#ifdef FUNCTION_INTERNALS
						std::cout << "in_index:" << in_index << std::endl;
						std::cout << "Out_index:" << out_index << std::endl;
		#endif // FUNCTION_INTERNALS

						outDatac[idx2][idx1] = inDatac[odx2][odx1];
						/*out[idx1][idx2].real = in[odx1][odx2].real;
						out[idx1][idx2].imag = in[odx1][odx2].imag;*/
					}
				}

			}
		}


		template <typename tt_dtype, int tp_L, int tp_R>
		void reOrderData_dynamic(std::complex<tt_dtype> inDatac[tp_R][tp_L/tp_R], std::complex<tt_dtype> outDatac[tp_R][tp_L/tp_R], int STAGE)
		{
			int no_of_fft_in_stage = (int)pow((double)(tp_R), double(STAGE));
			int fft_len_in_stage = tp_L / no_of_fft_in_stage;
			int no_bflys_per_fft = fft_len_in_stage / tp_R;
			int no_samples_per_bfly = tp_R;
			//static_assert(STAGE - 1 <= ((_log2<LEN>::val) - _log2<SSR>::val), "Stage no. should be less or equal of log Radix of FFT length minus 1");

			//assert(STAGE <_log2<LEN>::val/ _log2<SSR>::val);
		FFTs_LOOP:
			for (int f = 0;f < no_of_fft_in_stage;f++)
			{
			BFLYs_LOOP:
				for (int bf = 0;bf < no_bflys_per_fft;bf++)
				{
				RADIX_LOOP:
					for (int r = 0;r < no_samples_per_bfly;r++)
					{
						// butterfly number r sample;
						int in_index = (fft_len_in_stage*f + bf*tp_R + r);
						int idx1 = in_index / (tp_R);
						int idx2 = in_index % tp_R;
						int out_index = (bf + r*(fft_len_in_stage / tp_R) + (fft_len_in_stage*f));
						int odx1 = out_index / ( tp_R);
						int odx2 = out_index % tp_R;

		#ifdef FUNCTION_INTERNALS
						std::cout << "in_index:" << in_index << std::endl;
						std::cout << "Out_index:" << out_index << std::endl;
		#endif // FUNCTION_INTERNALS


						outDatac[idx2][idx1] = inDatac[odx2][odx1];
					}
				}

			}
		}



		template < int tp_L, int tp_R, int STAGE,typename tt_dtype>
		void ReverseReOrderData_static(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype outDatac[tp_R][tp_L/tp_R])
		{
		////#pragma HLS LOOP_MERGE
		//#pragma HLS INLINE off

			const int no_of_fft_in_stage =ssr_fft_pow<tp_R,STAGE>::val; // (int)pow((double)(tp_R), double(STAGE));
			const int fft_len_in_stage = (tp_L/tp_R) / no_of_fft_in_stage;
			const int no_bflys_per_fft = fft_len_in_stage / tp_R;
			const int no_samples_per_bfly = tp_R;
			//static_assert(STAGE - 1 <= ((_log2<LEN>::val) - _log2<SSR>::val), "Stage no. should be less or equal of log Radix of FFT length minus 1");

			//assert(STAGE <_log2<LEN>::val/ _log2<SSR>::val);
		FFTs_LOOP:
			for (int f = 0;f < no_of_fft_in_stage;f++)
			{
			BFLYs_LOOP:
				for (int bf = 0;bf < no_bflys_per_fft;bf++)
				{
		//#pragma HLS PIPELINE II=1
				RADIX_LOOP:
					for (int r = 0;r < no_samples_per_bfly;r++)
					{
		//#pragma HLS UNROLL
						// butterfly number r sample;
						int idx1 = (fft_len_in_stage*f + bf*tp_R + r) / (tp_R);
						int idx2 = (fft_len_in_stage*f + bf*tp_R + r) % tp_R;
						int temp = (bf + r*(fft_len_in_stage / tp_R) + (fft_len_in_stage*f));
						int odx1 = temp / (tp_R);
						int odx2 = temp % (tp_R);
						//std::cout << "temp:" << temp << std::endl;;
						//std::cout << idx1 << ":" << idx2 << std::endl;;
						//out[idx1][idx2] = in[odx1][odx2];
						outDatac[odx2][odx1] = inDatac[idx2][idx1];
					}
				}
			}
		}

		template <typename tt_dtype, int tp_L, int tp_R>
		void ReverseReOrderData_dynamic(std::complex<tt_dtype> inDatac[tp_R][tp_L/tp_R], std::complex<tt_dtype> outDatac[tp_R][tp_L/tp_R], int STAGE)
		{
			int no_of_fft_in_stage = (int)pow((double)(tp_R), double(STAGE));
			int fft_len_in_stage = tp_L / no_of_fft_in_stage;
			int no_bflys_per_fft = fft_len_in_stage / tp_R;
			int no_samples_per_bfly = tp_R;
			//static_assert(STAGE - 1 <= ((_log2<LEN>::val) - _log2<SSR>::val), "Stage no. should be less or equal of log Radix of FFT length minus 1");

			//assert(STAGE <_log2<LEN>::val/ _log2<SSR>::val);
		FFTs_LOOP:
			for (int f = 0;f < no_of_fft_in_stage;f++)
			{
			BFLYs_LOOP:
				for (int bf = 0;bf < no_bflys_per_fft;bf++)
				{
				RADIX_LOOP:
					for (int r = 0;r < no_samples_per_bfly;r++)
					{
						// butterfly number r sample;
						int idx1 = (fft_len_in_stage*f + bf*tp_R + r) / (tp_R);
						int idx2 = (fft_len_in_stage*f + bf*tp_R + r) % tp_R;
						int temp = (bf + r*(fft_len_in_stage / tp_R) + (fft_len_in_stage*f));
						int odx1 = temp / (tp_R);
						int odx2 = temp % (tp_R);
						//std::cout << "temp:" << temp << std::endl;;
						//std::cout << idx1 << ":" << idx2 << std::endl;;
						//out[idx1][idx2] = in[odx1][odx2];
						outDatac[odx2][odx1] = inDatac[idx2][idx1];
					}
				}
			}

		}
		template <typename tt_dtype, int tp_L, int tp_R>
		int verifyDataReversal(std::complex<tt_dtype> data[tp_R][tp_L/tp_R])
		{
			int flag = 0;
			for (int i = 0; i < tp_L/tp_R; i++)
			{
				for (int j = 0; j < tp_R; j++)
				{
					int index = i*tp_R + j;
					if (data[j][i].real() != index   || data[j][i].imag() != index)
						flag++;
				}
			}
			return flag;
		}
//===========================================================================================================================
//===========================================================================================================================
//===========================================================================================================================
//===========================================================================================================================
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //namespace hls
} //namespace ssr_fft


#endif // !HLS_SSR_FFT_DATA_REORDER_H
