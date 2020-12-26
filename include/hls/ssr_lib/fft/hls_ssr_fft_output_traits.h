// File Name : hls_ssr_fft_output_traits.h

#ifndef HLS_SSR_FFT_OUTPUT_TRAITS_H_
#define HLS_SSR_FFT_OUTPUT_TRAITS_H_

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
#include "hls_ssr_fft_input_traits.h"

#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_enums.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_dsp48.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_input_traits.h"

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
		ssr_fft_output_traits struct is defined in this file. It defines trait
		for SSR FFT output. Given the input type, scaling mode and tp_L and tp_R
		it can be used to find :
		       1- inner type used for std::complex output
               2- complex type which will be used by output std::complex<ap_fixed<>>

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

	template <int tp_L,int  tp_R,scaling_mode_enum tp_scaling_mode, typename T_in >
	struct ssr_fft_output_traits
	{

	};


	//==============================================Double Type========================================
	template <int tp_L,int  tp_R,scaling_mode_enum tp_scaling_mode >
	struct ssr_fft_output_traits < tp_L,tp_R,tp_scaling_mode,std::complex<double>  >
	{
		typedef std::complex<double> TTR_fftOutType;
		typedef double TTR_innerFftOutType;

		static const int type = 0;
	};

	//==============================================ap_fixed Type:: generic========================================
	template <int tp_L,int  tp_R,scaling_mode_enum tp_scaling_mode, int I_WL, int I_IL >
	struct ssr_fft_output_traits < tp_L,tp_R,tp_scaling_mode,std::complex< ap_fixed<I_WL,I_IL> >  >
	{
		typedef std::complex< ap_fixed<I_WL,I_IL,AP_TRN,AP_WRAP,0> > TTR_fftOutType;
		typedef ap_fixed<I_WL,I_IL,AP_TRN,AP_WRAP,0> TTR_innerFftOutType;

		static const int type = 1;
	};
	template <int tp_L,int  tp_R,scaling_mode_enum tp_scaling_mode, int I_WL, int I_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits >
	struct ssr_fft_output_traits < tp_L,tp_R,tp_scaling_mode,std::complex< ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> >  >
	{
		typedef std::complex< ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> > TTR_fftOutType;
		typedef ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> TTR_innerFftOutType;

		static const int type = 2;
	};


	//==============================================ap_fixed Type && SSR_FFT_NO_SCALING==================================
	template <int tp_L,int  tp_R, int I_WL, int I_IL >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_NO_SCALING,std::complex< ap_fixed<I_WL,I_IL> >  >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int tp_log2R = ssr_fft_log2<tp_R>::val;

		typedef std::complex< ap_fixed<   I_WL + (NO_OF_FFT_STAGES * tp_log2R)   ,  I_IL+ (NO_OF_FFT_STAGES * tp_log2R),AP_TRN,AP_WRAP,0 > > TTR_fftOutType;
		typedef ap_fixed<   I_WL + (NO_OF_FFT_STAGES * tp_log2R)   ,  I_IL+ (NO_OF_FFT_STAGES * tp_log2R),AP_TRN,AP_WRAP,0 > TTR_innerFftOutType;
		static const int type = 3;
	};

	template <int tp_L,int  tp_R , int I_WL, int I_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_NO_SCALING,std::complex< ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> >   >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int tp_log2R = ssr_fft_log2<tp_R>::val;
		typedef std::complex< ap_fixed<I_WL + (NO_OF_FFT_STAGES * tp_log2R)   ,  I_IL+ (NO_OF_FFT_STAGES * tp_log2R),in_q_mode,in_o_mode,in_sat_bits> > TTR_fftOutType;
		typedef ap_fixed<I_WL + (NO_OF_FFT_STAGES * tp_log2R)   ,  I_IL+ (NO_OF_FFT_STAGES * tp_log2R),in_q_mode,in_o_mode,in_sat_bits>  TTR_innerFftOutType;

		static const int type = 4;
	};
	//==============================================ap_fixed Type && SSR_FFT_SCALE=======================================

	template <int tp_L,int  tp_R, int I_WL, int I_IL >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_SCALE,std::complex< ap_fixed<I_WL,I_IL> >  >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int tp_log2R = ssr_fft_log2<tp_R>::val;

		typedef std::complex< ap_fixed<I_WL,I_IL+(NO_OF_FFT_STAGES * tp_log2R),AP_TRN,AP_WRAP,0> > TTR_fftOutType;
		typedef  ap_fixed<I_WL,I_IL+(NO_OF_FFT_STAGES * tp_log2R),AP_TRN,AP_WRAP,0> TTR_innerFftOutType;

		static const int type = 5;
	};
	template <int tp_L,int  tp_R , int I_WL, int I_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_SCALE,std::complex< ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> >   >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int tp_log2R = ssr_fft_log2<tp_R>::val;

		typedef std::complex< ap_fixed<I_WL,  I_IL+(NO_OF_FFT_STAGES * tp_log2R) ,in_q_mode,in_o_mode,in_sat_bits> > TTR_fftOutType;
		typedef ap_fixed<I_WL,  I_IL+(NO_OF_FFT_STAGES * tp_log2R) ,in_q_mode,in_o_mode,in_sat_bits>  TTR_innerFftOutType;

		static const int type = 6;

	};
	//===============================================SSR_FFT_GROW_TO_MAX_WIDTH===========================================
	template <int tp_L,int  tp_R, int I_WL, int I_IL >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_GROW_TO_MAX_WIDTH,std::complex< ap_fixed<I_WL,I_IL> >  >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		//replaced//static const int MAX_POSSIBLE_BIT_GROWTH = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int MAX_POSSIBLE_BIT_GROWTH = ssr_fft_log2<tp_L>::val;

		static const int tp_log2R = ssr_fft_log2<tp_R>::val;
		static const int EXPECTED_OUTPUT_BIT_WIDTH = ( I_WL + (MAX_POSSIBLE_BIT_GROWTH) );
		static const int EXPECTED_OUTPUT_INTEGER_WIDTH = ( I_IL + (MAX_POSSIBLE_BIT_GROWTH) );
		static const int MAX_ALLOWED_BIT_WIDTH_MARGIN = DSP48_OP2_BIT_WIDTH - I_WL;


		// If the expected grown output has bit width larger then max allowed saturate it.
		static const int O_WL = ( EXPECTED_OUTPUT_BIT_WIDTH <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_OUTPUT_BIT_WIDTH:DSP48_OP2_BIT_WIDTH;
		// If the expected  grown integer part width is larger then max allowed saturate it with any margin if left.
		static const int O_IL= ( EXPECTED_OUTPUT_INTEGER_WIDTH <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_OUTPUT_INTEGER_WIDTH:DSP48_OP2_BIT_WIDTH;
		typedef std::complex< ap_fixed<O_WL,O_IL,AP_TRN,AP_WRAP,0> > TTR_fftOutType;
		typedef ap_fixed<O_WL,O_IL,AP_TRN,AP_WRAP,0> TTR_innerFftOutType;

		static const int type = 7;
	};


	template <int tp_L,int  tp_R , int I_WL, int I_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits >
	struct ssr_fft_output_traits < tp_L,tp_R,SSR_FFT_GROW_TO_MAX_WIDTH,std::complex< ap_fixed<I_WL,I_IL,in_q_mode,in_o_mode,in_sat_bits> >   >
	{
		static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		//static const int MAX_POSSIBLE_BIT_GROWTH = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
		static const int MAX_POSSIBLE_BIT_GROWTH = ssr_fft_log2<tp_L>::val;


		static const int tp_log2R = ssr_fft_log2<tp_R>::val;
		static const int EXPECTED_OUTPUT_BIT_WIDTH = ( I_WL + (MAX_POSSIBLE_BIT_GROWTH) );
		static const int EXPECTED_OUTPUT_INTEGER_WIDTH = ( I_IL + (MAX_POSSIBLE_BIT_GROWTH) );
		static const int MAX_ALLOWED_BIT_WIDTH_MARGIN = DSP48_OP2_BIT_WIDTH - I_WL;


		static const int O_WL = ( EXPECTED_OUTPUT_BIT_WIDTH <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_OUTPUT_BIT_WIDTH:DSP48_OP2_BIT_WIDTH;
		static const int O_IL= ( EXPECTED_OUTPUT_INTEGER_WIDTH <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_OUTPUT_INTEGER_WIDTH:DSP48_OP2_BIT_WIDTH;
		typedef std::complex< ap_fixed<O_WL,O_IL ,in_q_mode,in_o_mode,in_sat_bits> > TTR_fftOutType;
		typedef ap_fixed<O_WL,O_IL ,in_q_mode,in_o_mode,in_sat_bits> TTR_innerFftOutType;

		static const int type = 8;

	};
	template <typename fft_params, typename tt_fft_input_type >
	struct ssr_fft_output_type
	{
		typedef typename ssr_fft_input_traits<tt_fft_input_type>::TTR_casted_type casted_type;
		typedef typename ssr_fft_output_traits<fft_params::N,fft_params::R,fft_params::scaling_mode,casted_type>::TTR_fftOutType t_ssr_fft_out;

	};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //namespace hls
} //namespace ssr_fft



//======================================================================================================================
#endif //HLS_SSR_FFT_OUTPUT_TRAITS_H_
