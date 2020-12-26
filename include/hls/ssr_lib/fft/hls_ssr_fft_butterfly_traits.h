// File Name : hls_ssr_fft_butterfly_traits.h

#ifndef HLS_SSR_FFT_BUTTERFLY_TRAITS_H_
#define HLS_SSR_FFT_BUTTERFLY_TRAITS_H_

#include <ap_fixed.h>
#include <complex>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_utilities.h"
#include "hls_ssr_fft_enums.h"
#include "hls_ssr_fft_dsp48.h"

#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_enums.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_dsp48.h"
#endif

//#include "DEBUG_CONSTANTS.h"
////////////////////////////////////////////////////////////////////////

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-   Given the input type for accumulator the butterfly traits defines the output    -_-
-_-   type to be used for storing the output of accumulator based on the input type   -_-
-_-   and the scaling mode selected for accumulation. Three different scaling modes   -_-
-_-   are defined:                                                                    -_-
-_-               SSR_FFT_NO_SCALING : No scaling is done to prevent any overflows    -_-
-_- 								   bit growth is allowed every addition		      -_-
-_-               SSR_FFT_SCALE      : Scale the output by one bit ever binary        -_-
-_-                                    addition                                       -_-
-_-        SSR_FFT_GROW_TO_MAX_WIDTH : Initially the bit growth allowed until max     -_-
-_-                                    width reached and then scaling used after that -_-
-_-   Note : bfly_type is defined for debug purposes only                             -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */

// Traits for Butterfly input and output



#define HLS_SSR_FFT_ENABLE_ADDER_TREE_SCALING



namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#if 1
	template <typename T1>
	struct bfly_traits_default {
		//typedef T1 TTR_bflyProductType;
		typedef T1 TTR_bflyAccumType;
	};

	template <scaling_mode_enum tp_scaling_mode,typename T1>
	struct bfly_traits:public bfly_traits_default<T1>{
		static const int bfly_type=0;

	};

	template <scaling_mode_enum tp_scaling_mode>
	struct bfly_traits<tp_scaling_mode,std::complex<int> >:public bfly_traits_default<std::complex<int> >{
		static const int bfly_type=10;
	};
	template <scaling_mode_enum tp_scaling_mode>
	struct bfly_traits<tp_scaling_mode,std::complex<double> >:public bfly_traits_default<std::complex<double> >{
		static const int bfly_type=20;
	};


	template <scaling_mode_enum tp_scaling_mode, int W1,int I1>
	struct bfly_traits<tp_scaling_mode, std::complex<ap_fixed<W1,I1> > > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
		typedef std::complex < ap_fixed<W1+1,I1+1,AP_TRN,AP_WRAP,0> > TTR_bflyAccumType;
		static const int bfly_type=1;

	};

	template <int W1,int I1>
	struct bfly_traits< SSR_FFT_NO_SCALING,std::complex <  ap_fixed<W1,I1> > > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
		typedef std::complex <  ap_fixed<W1+1,I1+1,AP_TRN,AP_WRAP,0> > TTR_bflyAccumType;
		static const int bfly_type=2;

	};

	template <int W1,int I1>
	struct bfly_traits< SSR_FFT_SCALE,std::complex <  ap_fixed<W1,I1> > > {
	#ifdef HLS_SSR_FFT_ENABLE_ADDER_TREE_SCALING
		static const int BIT_GROWTH_WL = 0;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = ((W1 + BIT_GROWTH_WL) <= DSP48_OP2_BIT_WIDTH ) ? (W1 + BIT_GROWTH_WL):DSP48_OP2_BIT_WIDTH;
		static const int OUTPUT_IL = ((I1 + BIT_GROWTH_IL) <= OUTPUT_WL ) ? (I1 + BIT_GROWTH_IL):OUTPUT_WL;
	#else
		static const int BIT_GROWTH_WL = 1;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = (W1 + BIT_GROWTH_WL);
		static const int OUTPUT_IL = (I1 + BIT_GROWTH_IL);
	#endif
		typedef std::complex <  ap_fixed<OUTPUT_WL,OUTPUT_IL,AP_TRN,AP_WRAP,0> > TTR_bflyAccumType;
		static const int bfly_type=3;

	};

	template <int W1,int I1>
	struct bfly_traits< SSR_FFT_GROW_TO_MAX_WIDTH,std::complex <  ap_fixed<W1,I1>  > > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
	#ifdef HLS_SSR_FFT_ENABLE_ADDER_TREE_SCALING
		static const int BIT_GROWTH_WL = 1;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = ((W1 + BIT_GROWTH_WL) <= DSP48_OP2_BIT_WIDTH ) ? (W1 + BIT_GROWTH_WL):DSP48_OP2_BIT_WIDTH;
		static const int OUTPUT_IL = ((I1 + BIT_GROWTH_IL) <= DSP48_OP2_BIT_WIDTH ) ? (I1 + BIT_GROWTH_IL):DSP48_OP2_BIT_WIDTH;
	#else
		static const int BIT_GROWTH_WL = 1;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = (W1 + BIT_GROWTH_WL);
		static const int OUTPUT_IL = (I1 + BIT_GROWTH_IL);
	#endif
		typedef std::complex <  ap_fixed<OUTPUT_WL,OUTPUT_IL,AP_TRN,AP_WRAP,0> > TTR_bflyAccumType;
		static const int bfly_type=4;

	};



	/*template <int W1,int I1>
	struct bfly_traits<SSR_FFT_NO_SCALING, ap_fixed<W1,I1> > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
		typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyAccumType;
	}*/;

	template <scaling_mode_enum tp_scaling_mode,int W1,int I1, ap_q_mode Q, ap_o_mode O, int sat_bits>
	struct bfly_traits< tp_scaling_mode,std::complex <  ap_fixed<W1,I1,Q,O,sat_bits> > > {
		typedef std::complex <   ap_fixed<W1+1,I1+1,Q,O,sat_bits> > TTR_bflyAccumType;
		static const int bfly_type=5;

	};

	template < int W1,int I1, ap_q_mode Q, ap_o_mode O, int sat_bits>
	struct bfly_traits< SSR_FFT_NO_SCALING,std::complex <  ap_fixed<W1,I1,Q,O,sat_bits> > > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
		typedef std::complex <  ap_fixed<W1+1,I1+1,AP_TRN,AP_WRAP,0> > TTR_bflyAccumType;
		static const int bfly_type=6;


	};

	template < int W1,int I1, ap_q_mode Q, ap_o_mode O, int sat_bits>
	struct bfly_traits< SSR_FFT_SCALE,std::complex <   ap_fixed<W1,I1,Q,O,sat_bits> >  > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
	#ifdef HLS_SSR_FFT_ENABLE_ADDER_TREE_SCALING
		static const int BIT_GROWTH_WL = 0;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = ((W1 + BIT_GROWTH_WL) <= DSP48_OP2_BIT_WIDTH ) ? (W1 + BIT_GROWTH_WL):DSP48_OP2_BIT_WIDTH;
		static const int OUTPUT_IL = ((I1 + BIT_GROWTH_IL) <= OUTPUT_WL ) ? (I1 + BIT_GROWTH_IL):OUTPUT_WL;
	#else
		static const int BIT_GROWTH_WL = 1;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = (W1 + BIT_GROWTH_WL);
		static const int OUTPUT_IL = (I1 + BIT_GROWTH_IL);
	#endif
		typedef std::complex <   ap_fixed<OUTPUT_WL,OUTPUT_IL,Q,O,sat_bits> > TTR_bflyAccumType;
		static const int bfly_type=7;


	};

	template < int W1,int I1, ap_q_mode Q, ap_o_mode O, int sat_bits>
	struct bfly_traits< SSR_FFT_GROW_TO_MAX_WIDTH, std::complex <  ap_fixed<W1,I1,Q,O,sat_bits> > > {
		//typedef ap_fixed<W1,I1,AP_TRN,AP_WRAP,0> TTR_bflyProductType;
	#ifdef HLS_SSR_FFT_ENABLE_ADDER_TREE_SCALING
		static const int BIT_GROWTH_WL = 1;
		static const int BIT_GROWTH_IL = 1;
		static const int OUTPUT_WL = ((W1 + BIT_GROWTH_WL) <= DSP48_OP2_BIT_WIDTH ) ? (W1 + BIT_GROWTH_WL):DSP48_OP2_BIT_WIDTH;
		static const int OUTPUT_IL = ((I1 + BIT_GROWTH_IL) <= DSP48_OP2_BIT_WIDTH ) ? (W1 + BIT_GROWTH_IL):DSP48_OP2_BIT_WIDTH;
	#else
		static const int OUTPUT_WL = (W1 + 1);
		static const int OUTPUT_IL = (I1 + 1);
	#endif
		typedef std::complex <   ap_fixed<OUTPUT_WL,OUTPUT_IL,Q,O,sat_bits> > TTR_bflyAccumType;
		static const int bfly_type=8;

	};

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	#endif
} //end name space hls
} //end name space ssr_fft

#endif //HLS_SSR_FFT_BUTTERFLY_TRAITS_H_
