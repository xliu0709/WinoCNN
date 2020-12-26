//File Name : hls_ssr_fft_multiplication_traits.h
#ifndef _HLS_SSR_FFT_MULTIPLICATION_TRAITS_H_
#define _HLS_SSR_FFT_MULTIPLICATION_TRAITS_H_

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		template < typename op1_twiddle_type, typename op2_sample_type>
		struct ssr_fft_multiplication_traits
		{

		};

	   /*
		* ==========================================================================
		* ssr fft input traits for type : complex <double>
		* ==========================================================================
		*/
		template <>
		struct ssr_fft_multiplication_traits< std::complex<double> , std::complex<double> >
		{
		   typedef std::complex< double > TTR_product_op;
		   typedef double TTR_op1_real;
		   typedef double TTR_op1_imag;
		   typedef double TTR_op2_real;
		   typedef double TTR_op2_imag;
		   typedef double TTR_accum;
		   typedef double TTR_product;
		};

		/* ==========================================================================
		* ssr fft input traits for type : complex <ap_fixed>
		* ==========================================================================
		*/
		template <int tp_op1_WL, int tp_op1_IL,int tp_op2_WL, int tp_op2_IL>
		struct ssr_fft_multiplication_traits< std::complex< ap_fixed <tp_op1_WL,tp_op1_IL> > , std::complex< ap_fixed <tp_op2_WL,tp_op2_IL> > >
		{
			static const int product_IL = (tp_op1_IL > tp_op2_IL) ? tp_op1_IL : tp_op2_IL;

			static const int op1_fraction_length = tp_op1_WL - tp_op1_IL;
			static const int op2_fraction_length = tp_op2_WL - tp_op2_IL;
			static const int product_fraction_lenth 		= (op1_fraction_length > op2_fraction_length) ? op1_fraction_length : op2_fraction_length;
			static const int product_WL = product_IL + product_fraction_lenth;
		   typedef std::complex< ap_fixed <product_WL,product_IL,AP_TRN,AP_WRAP,0> >  TTR_product_op;

		   /// Get inner types of the given comples types ::::
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL> TTR_op1_real;
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL> TTR_op1_imag;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL> TTR_op2_real;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL> TTR_op2_imag;

		   typedef std::complex< double > TTR_accum;
		   typedef std::complex< double > TTR_product;

		};

		/* ==========================================================================
		* ssr fft input traits for type : complex <ap_fixed>
		* ==========================================================================
		*/
		template <	int tp_op1_WL, int tp_op1_IL,ap_q_mode op1_q_mode, ap_o_mode op1_o_mode, int op1_sat_bits,  // type-1
		         	int tp_op2_WL, int tp_op2_IL                     											// type2
				 >
		struct ssr_fft_multiplication_traits<
											std::complex< ap_fixed <tp_op1_WL,tp_op1_IL,op1_q_mode,op1_o_mode, op1_sat_bits> > ,
											std::complex< ap_fixed <tp_op2_WL,tp_op2_IL> >
											>
		{
			static const int product_IL = (tp_op1_IL > tp_op2_IL) ? tp_op1_IL : tp_op2_IL;

			static const int op1_fraction_length = tp_op1_WL - tp_op1_IL;
			static const int op2_fraction_length = tp_op2_WL - tp_op2_IL;
			static const int product_fraction_lenth 		= (op1_fraction_length > op2_fraction_length) ? op1_fraction_length : op2_fraction_length;
			static const int product_WL = product_IL + product_fraction_lenth;
		   typedef std::complex< ap_fixed <product_WL,product_IL, op1_q_mode,  op1_o_mode,  op1_sat_bits> >  TTR_product_op;
		   /// Get inner types of the given comples types ::::
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL,op1_q_mode,op1_o_mode, op1_sat_bits> TTR_op1_real;
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL,op1_q_mode,op1_o_mode, op1_sat_bits> TTR_op1_imag;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL> TTR_op2_real;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL> TTR_op2_imag;
		};

		/* ==========================================================================
		* ssr fft input traits for type : complex <ap_fixed>
		* ==========================================================================
		*/
		template <	int tp_op1_WL, int tp_op1_IL,  															   // type-1
		         	int tp_op2_WL, int tp_op2_IL,ap_q_mode op2_q_mode, ap_o_mode op2_o_mode, int op2_sat_bits// type2
				 >
		struct ssr_fft_multiplication_traits<
												std::complex< ap_fixed <tp_op1_WL,tp_op1_IL> >,
												std::complex< ap_fixed <tp_op2_WL,tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> >
											>
		{
			static const int product_IL = (tp_op1_IL > tp_op2_IL) ? tp_op1_IL : tp_op2_IL;

			static const int op1_fraction_length = tp_op1_WL - tp_op1_IL;
			static const int op2_fraction_length = tp_op2_WL - tp_op2_IL;
			static const int product_fraction_lenth 		= (op1_fraction_length > op2_fraction_length) ? op1_fraction_length : op2_fraction_length;
			static const int product_WL = product_IL + product_fraction_lenth;
		   typedef std::complex< ap_fixed <product_WL,product_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> >  TTR_product_op;
		   /// Get inner types of the given comples types ::::
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL> TTR_op1_real;
		   typedef ap_fixed <tp_op1_WL,tp_op1_IL> TTR_op1_imag;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> TTR_op2_real;
		   typedef ap_fixed <tp_op2_WL,tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> TTR_op2_imag;
		};


		/* ==========================================================================
		* ssr fft input traits for type : complex <ap_fixed>
		* ==========================================================================
		*/
		template <	int tp_op1_WL, int tp_op1_IL,ap_q_mode op1_q_mode, ap_o_mode op1_o_mode, int op1_sat_bits,  // type-1
		         	int tp_op2_WL, int tp_op2_IL,ap_q_mode op2_q_mode, ap_o_mode op2_o_mode, int op2_sat_bits // type2
				 >
		struct ssr_fft_multiplication_traits<
												std::complex< ap_fixed <tp_op1_WL, tp_op1_IL, op1_q_mode,  op1_o_mode,  op1_sat_bits> > ,
												std::complex< ap_fixed <tp_op2_WL, tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> >
											>
		{
			static const int product_IL = (tp_op1_IL > tp_op2_IL) ? tp_op1_IL : tp_op2_IL;

			static const int op1_fraction_length = tp_op1_WL - tp_op1_IL;
			static const int op2_fraction_length = tp_op2_WL - tp_op2_IL;
			static const int product_fraction_lenth 		= (op1_fraction_length > op2_fraction_length) ? op1_fraction_length : op2_fraction_length;
			static const int product_WL = product_IL + product_fraction_lenth;
		   typedef std::complex< ap_fixed <product_WL,product_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> >  TTR_product_op;
		   /// Get inner types of the given comples types ::::
		   typedef ap_fixed <tp_op1_WL, tp_op1_IL, op1_q_mode,  op1_o_mode,  op1_sat_bits> TTR_op1_real;
		   typedef ap_fixed <tp_op1_WL, tp_op1_IL, op1_q_mode,  op1_o_mode,  op1_sat_bits> TTR_op1_imag;
		   typedef ap_fixed <tp_op2_WL, tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> TTR_op2_real;
		   typedef ap_fixed <tp_op2_WL, tp_op2_IL, op2_q_mode,  op2_o_mode,  op2_sat_bits> TTR_op2_imag;
		};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} // End name space ssr_fft
}; // End name space hls


#endif //_HLS_SSR_FFT_MULTIPLICATION_TRAITS_H_
