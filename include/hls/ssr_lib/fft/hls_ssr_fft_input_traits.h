//File Name : hls_ssr_fft_input_traits.h
#ifndef HLS_SSR_FFT_INPUT_TRAITS_H_
#define HLS_SSR_FFT_INPUT_TRAITS_H_
#include <complex>
#include <ap_fixed.h>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_dsp48.h"
#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_dsp48.h"
#endif


namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	template < typename T_in>
	struct ssr_fft_input_traits
	{

	};

	/*
	* ==========================================================================
	* ssr fft input traits for type : complex <double>
	* ==========================================================================
	*/
	template <>
	struct ssr_fft_input_traits< std::complex<double> >
	{
	   typedef std::complex< double > TTR_casted_type;
	};
	 /*
	* ==========================================================================
	* ssr fft input traits for type : complex <ap_fixed>
	* ==========================================================================
	*/
	template <int tp_WL, int tp_IL>
	struct ssr_fft_input_traits< std::complex< ap_fixed <tp_WL,tp_IL> > >
	{
	   typedef std::complex< ap_fixed <tp_WL,tp_IL,AP_TRN,AP_WRAP,0> >  TTR_casted_type;
	};


	template <int tp_WL, int tp_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits>
	struct ssr_fft_input_traits< std::complex< ap_fixed <tp_WL,tp_IL,in_q_mode,in_o_mode,in_sat_bits> > >
	{
	   typedef std::complex< ap_fixed <tp_WL,tp_IL,in_q_mode,in_o_mode,in_sat_bits> >  TTR_casted_type;
	};

	/*
	* ==========================================================================
	* ssr fft input traits for type : complex <ap_ufixed>
	* ==========================================================================
	*/
	template <int tp_WL, int tp_IL>
	struct ssr_fft_input_traits< std::complex< ap_ufixed <tp_WL,tp_IL> > >
	{
	   static const int EXPECTED_WL = tp_WL + 1;
	   static const int EXPECTED_IL = tp_IL + 1;
	   static const int WL = (EXPECTED_WL < DSP48_OP2_BIT_WIDTH) ? EXPECTED_WL:DSP48_OP2_BIT_WIDTH;
	   static const int IL = (EXPECTED_IL < DSP48_OP2_BIT_WIDTH) ? EXPECTED_IL:DSP48_OP2_BIT_WIDTH;

	   typedef std::complex< ap_fixed <WL,IL,AP_TRN,AP_WRAP,0> >  TTR_casted_type;
	};


	template <int tp_WL, int tp_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits>
	struct ssr_fft_input_traits< std::complex< ap_ufixed <tp_WL,tp_IL,in_q_mode,in_o_mode,in_sat_bits> > >
	{
		static const int EXPECTED_WL = tp_WL + 1;
		static const int EXPECTED_IL = tp_IL + 1;
		static const int WL = (EXPECTED_WL < DSP48_OP2_BIT_WIDTH) ? EXPECTED_WL:DSP48_OP2_BIT_WIDTH;
		static const int IL = (EXPECTED_IL < DSP48_OP2_BIT_WIDTH) ? EXPECTED_IL:DSP48_OP2_BIT_WIDTH;

	   typedef std::complex< ap_fixed <WL,IL,in_q_mode,in_o_mode,in_sat_bits> >  TTR_casted_type;
	};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} //end namespace ssr_fft
} //end namespace hls


#endif //HLS_SSR_FFT_INPUT_TRAITS_H_
