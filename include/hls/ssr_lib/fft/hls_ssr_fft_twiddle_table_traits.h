// File Name : hls_ssr_fft_twiddle_table_traits.h

#ifndef HLS_SSR_FFT_TWIDDLE_TABLE_TRAITS_H_
#define HLS_SSR_FFT_TWIDDLE_TABLE_TRAITS_H_

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-

			This file defines traits for twiddle table and  exp table
			Twiddle table can take different forms depending on the
			stage where it is used also it has variations for FFTs
			when length is integer power of radix or not.

			   twiddle_traits_len <int tp_L, int tp_R> : defines
			   twiddle table length for the case when tp_L is
			   integer power of tp_R

			   exp_traits_len< int tp_outputForkingFactor,int tp_L, int tp_R>
			   : defines exp table length where the tp_outputForkingFactor
			   parameters defines if the tp_L is integer power of tp_R and
			   if not what is the ratio of tp_L and next integer power of tp_R

			   exp_traits_len::EXP_TALBE_LENGTH_LAST_STAGE : defined the exp
			   table length for last stage, for the case when tp_L not integer
			   power of tp_R last stage has different length then other stages

			  input_based_twiddle_traits<>:: Gives the type of twiddle and exp
			  table when the input sample type is passed in template input


-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */

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

namespace hls {
	namespace ssr_fft {

		template <int tp_L, int tp_R>
		struct twiddle_traits_len
		{
			//static const int TWIDDLE_TALBE_LENGTH = (tp_R-1)*(tp_L/tp_R-1)+1;
			static const int TWIDDLE_TALBE_LENGTH = tp_L/4;
		};

		template < int tp_outputForkingFactor,int tp_L, int tp_R>
		struct exp_traits_len
		{
			static const int EXP_TALBE_LENGTH = tp_R;
			static const int EXP_TALBE_LENGTH_LAST_STAGE = tp_R/tp_outputForkingFactor;
		};

		template <int tp_L, int tp_R>
		struct exp_traits_len<0,tp_L,tp_R>
		{
			static const int EXP_TALBE_LENGTH = tp_R;

		};

		template <int tp_L>
		struct exp_traits_len<0,tp_L,2>
		{
			static const int EXP_TALBE_LENGTH = 4;       // Keep The minimum exp table length as 4 because for smaller length the HLS will not infer ROM

		};

		template <typename T >
		struct twiddle_traits
		{
		};

		template<typename T>
		struct twiddle_traits<std::complex<T> >
		{
			typedef std::complex<T> TTR_twiddleType;
			typedef T TTR_twiddleInnerType;
		};

		template <typename param_struct_t,typename T >
		struct input_based_twiddle_traits
		{
		};

		template<typename param_struct_t,typename T>
		struct input_based_twiddle_traits<param_struct_t,std::complex<T> >
		{
			typedef std::complex<T> TTR_twiddleType;
			typedef std::complex<T> TTR_expTabType;
		};
	} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFT_TWIDDLE_TABLE_TRAITS_H_
