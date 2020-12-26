//File Name: hls_ssr_fft_adder_tree.h

#ifndef HLS_SSR_FFT_ADDER_TREE_H_
#define HLS_SSR_FFT_ADDER_TREE_H_

#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_traits.h"
#include "hls_ssr_fft_enums.h"

#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_traits.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_enums.h"

#endif
#include <complex>
/*
 ========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_- AdderTreeClass: defines a function called creatTreeLevel, which is used to        -_-
-_- create adder tree, every tree level is defined by number of tree nodes at that    -_-
-_- level, the recursion terminates when the tree level is created by two need.       -_-
-_- Essentially ever tree level represents a set of N/2 binary adders doing addition  -_-
-_- in parallel.                                                                      -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */

// Constant Defined For Debugging ///////////////////////////
//#define ADDER_TREE_PRINT_DEBUG_MESSAGES
/////////////////////////////////////////////////////////////


//=====================================================================================
//2di : No  2 dimensional arrays in this block for reshaping and streaming used
//=====================================================================================





namespace hls {
	namespace ssr_fft {

		template < int tp_numberOfTreeNodes>
		struct AdderTreeClass
		{
			template < int tp_treeLevel,scaling_mode_enum tp_scaling_mode,typename T_in, typename T_out  >
			void createTreeLevel (T_in data[tp_numberOfTreeNodes], T_out &accumValue);
		};
		template <int tp_numberOfTreeNodes>
		template < int tp_treeLevel,scaling_mode_enum tp_scaling_mode,typename T_in, typename T_out  >
		void  AdderTreeClass<tp_numberOfTreeNodes>::createTreeLevel(T_in data[tp_numberOfTreeNodes],T_out &accumValue)
		{
		#pragma HLS INLINE
			//T_in data_out[tp_numberOfTreeNodes/2];
			typename bfly_traits<tp_scaling_mode,T_in>::TTR_bflyAccumType data_out[tp_numberOfTreeNodes/2];

		#ifdef ADDER_TREE_PRINT_DEBUG_MESSAGES
			std::cout<<"The Butter Fly  Type Used ::"<<bfly_traits<tp_scaling_mode,T_in>::bfly_type<<std::endl;
		#endif


		#pragma HLS ARRAY_PARTITION variable=data_out complete dim=1

			LOOP_TREE_LEVEL:
			for (int n = 0; n < tp_numberOfTreeNodes/2; n++)
			{
		#pragma HLS UNROLL
				data_out[n] = data[2*n] + data[2*n + 1];
			}
			AdderTreeClass<tp_numberOfTreeNodes/2> AdderTreeClass_obj;

			AdderTreeClass_obj.template createTreeLevel<tp_treeLevel+1,tp_scaling_mode,typename bfly_traits<tp_scaling_mode,T_in>::TTR_bflyAccumType,T_out>(data_out, accumValue);

		}


		template <>
		template < int tp_treeLevel,scaling_mode_enum tp_scaling_mode,typename T_in, typename T_out  >
		void AdderTreeClass<2>::createTreeLevel(T_in data[2],T_out &accumValue)
		{
		#pragma HLS INLINE
		#pragma HLS ARRAY_PARTITION variable=data complete dim=1

			accumValue= data[1] + data[0];
		}

	} 		//namespace hls
} 			//namespace ssr_fft


#endif //HLS_SSR_FFT_ADDER_TREE_H_
