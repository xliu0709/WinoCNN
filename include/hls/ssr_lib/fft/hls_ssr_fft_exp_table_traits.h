// File Name : hls_ssr_fft_exp_table_traits.h

#ifndef HLS_SSR_FFT_EXP_TABLE_TRAITS_H_
#define HLS_SSR_FFT_EXP_TABLE_TRAITS_H_

#include <ap_fixed.h>
#include <complex>
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

			exp_table_traits: struct defined traits for exp table. Given the
			input type this struct can be used to define exp table type.

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

	template <typename T >
	struct exp_table_traits{

	};

	template<typename T>
	struct exp_table_traits<std::complex<T> >
	{
		typedef std::complex<T> TTR_expTableType;
	};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //namespace hls
} //namespace ssr_fft



#endif //HLS_SSR_FFT_EXP_TABLE_TRAITS_H_
