

 //hls_ssr_fft_transposer.h

#ifndef HLS_SSR_FFT_TRANSPOSER_H_
#define HLS_SSR_FFT_TRANSPOSER_H_

#include <iostream>
#include <assert.h>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_utilities.h"
#include "hls_ssr_fft_data_commutor.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_data_commutor.h"

#endif

namespace hls {
	namespace ssr_fft {



		template<int tp_stages, int tp_outputForkingFactor>
		struct inputSwap{

			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);
		};

		// These  declarations cover base case for modeling input swap functions were Length of SSR FFT is always integer
		// power of Radix R or SSR
		////////////////////////////////////////////////////////////////////////
		template<int tp_stages>
		struct inputSwap<tp_stages,1>{

			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);
		};

		template<>
		struct inputSwap<1,1>{

				template <int tp_L, int tp_R,int tp_PF, typename tt_T>
				void swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
				{
				#pragma HLS INLINE

					static const int isLargeMemFlag = (  ( (tp_PF*tp_R) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS );
					dataCommutations<10000,1,tp_PF,isLargeMemFlag> commutor;
					commutor.template dataCommutor<tp_L,tp_R,tt_T>(in,out);

				}

		};
		/////////////////////////////////////////////////////////////////////////
		// Struct declaration for the case where forking factor is > 1 and stage is 1
		// which is actually the terminal case to stop recursion

		template< int tp_outputForkingFactor>
		struct inputSwap<1,tp_outputForkingFactor>{

			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);
		};
		////////////////////////////////////////////////////////////////////////
		// Base Case Implementation where L is integer power of radix ///start
		template<int tp_stages>
		template <int tp_L, int tp_R, int tp_PF, typename tt_T>
		void inputSwap<tp_stages,1>::swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE   // The swap is in-lined in a data-flow region to expose dataCommutor to become a process.
							 // Recursion will create a chain of processes....

		tt_T temp[tp_R][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=temp
		#pragma HLS STREAM variable=temp depth=2 dim=2
#pragma HLS RESOURCE variable=temp core=FIFO_LUTRAM

#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=temp complete dim=1
#endif

		static const int isLargeMemFlag = (  ((tp_PF*tp_R) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<10000+tp_stages,0,tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L,tp_R,tt_T>(in,temp);

			// Instantiate Next State....Recursion
			inputSwap<tp_stages-1,1> nextStage;
			nextStage.template swap<tp_L,tp_R,(tp_PF*tp_R),tt_T>(temp, out);  // Supply Next Stage PF

		}


		///picked ...
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Base Case Implementation where L is integer power of radix ///END


		////////////////////////////////////////////////////////////////////////
		// The generic case where s > 1 and forking factor is > 1 : it structure is same like case when s>1 and f=1
		template<int tp_stages,int tp_outputForkingFactor>
		template <int tp_L, int tp_R, int tp_PF, typename tt_T>
		void inputSwap<tp_stages,tp_outputForkingFactor>::swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE   // The swap is in-lined in a data-flow region to expose dataCommutor to become a process.
							 // Recursion will create a chain of processes....
		tt_T temp[tp_R][tp_L/tp_R];

		#pragma HLS STREAM variable=temp depth=2 dim=2
		#pragma HLS RESOURCE variable=temp core=FIFO_LUTRAM

		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp


		static const int isLargeMemFlag = (  ((tp_PF*tp_R) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<10000+tp_stages,0,tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L,tp_R,tt_T>(in,temp);

			// Instantiate Next State....Recursion
			inputSwap<tp_stages-1,tp_outputForkingFactor> nextStage;
			nextStage.template swap<tp_L,tp_R,(tp_PF*tp_R),tt_T>(temp, out);  // Supply Next Stage PF

		}


		////////////////////////////////////////////////////////////////////////
		template<int tp_outputForkingFactor>
		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void inputSwap<1,tp_outputForkingFactor>::swap(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			DataCommutorFork <10000+1,tp_outputForkingFactor,tp_outputForkingFactor > DataCommutorFork_obj;
			DataCommutorFork_obj.template copyForkCommuteAndMerge<tp_PF,tp_L,tp_R,tt_T>(in,out);
		}
	} //namespace hls
} //namespace ssr_fft

#endif  //HLS_SSR_FFT_TRANSPOSER_H_

