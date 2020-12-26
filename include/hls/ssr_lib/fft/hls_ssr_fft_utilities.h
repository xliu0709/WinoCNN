/*/////////////////////////////////////////////////////////////////////
 * hls_ssr_fft_utilities.h
 *
 *      Author: Shahzad Ahmad Butt
 *
 */////////////////////////////////////////////////////////////////////

#ifndef HLS_SSR_FFT_UTILITIES_H_
#define HLS_SSR_FFT_UTILITIES_H_


/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
			Different utility functions are defined in this file. Including
			functions for calculating log2, pow at compile time. Two other
			utility functions used by SSR FFT called digitReversal_m and
			digitReversal_fracIsLsb are defined in this file. Both of these
			functions are used for digital reversal, digit reversal is the
			process where given integer is broken into log2(R) bit parts
			and these parts as whole are assumed as digit and reversed as
			whole digits ( like bits are reversed in ordinary bit reversal).
			Two different functions are defined for selecting the direction
			of reversal. In the case of digitReversal_fracIsLsb if the total
			number of bits used to represent the number is not multiple of
			digit size then fractional bits are assumed on the lsb side and
			used as one whole digit while reversal is performed.


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




#include <ap_int.h>
namespace hls {
	namespace ssr_fft {



	   /* template <int L>
	    struct powerOf2CheckonL
		{
	    	 void check()
	    	 {

	    	 }
		};

	    template <>
	     void powerOf2CheckonL<0>::check()
		{
#error    "----------------The Selected FFT Length is not Power of 2-----------------"
		}


	    template <int L>
	    struct powerOf2CheckonRadix
		{
	    	 void check()
	    	 {

	    	 }
		};

	    template <>
	     void powerOf2CheckonRadix<0>::check()
		{
#error    "----------------The Selected FFT Radix or SSR is not Power of 2-----------------"
		}

*/
		void inline check_covearage()
		{
			std::cout<<"\n\n\n\n\n\n\n\nCovered;;;;;;;;\n"<<__FILE__<< __LINE__<<"<<\n\n\n\n\n\n\n\n";

					//exit(1);
		}
		template <int num> //
		struct ssr_fft_log2 {
			static const int val = 1 + ssr_fft_log2 <num / 2>::val;

		};
		template <>
		struct ssr_fft_log2<1> {
			static const int val = 0;
		};

		template <unsigned int num, unsigned int p>
		struct ssr_fft_pow {
			static const unsigned long int val = num * ssr_fft_pow <num, p - 1>::val;

		};

		template <unsigned int num>
		struct ssr_fft_pow<num, 1> {
			static const unsigned long int val = num;
		};
		template <unsigned int num>
		struct ssr_fft_pow<num, 0> {
			static const unsigned long int val = 1;
		};

		template <int num>
		struct ssr_fft_log2BitwiseAndModMask {
			static const int val= ( 1<<(ssr_fft_log2<num>::val) )  -  1;
		};
		template < unsigned int tp_L, unsigned int tp_R>
		unsigned int digitReversal(unsigned int number)
		{
			//CHECK_COVEARAGE;
		#pragma HLS INLINE
			unsigned int log_radix_tp_L = (ssr_fft_log2<tp_L>::val)/(ssr_fft_log2<tp_R>::val);
			unsigned int log2_radix = (ssr_fft_log2<tp_R>::val);
			unsigned int mask = (1 << log2_radix) - 1;
			unsigned int result = 0;
			unsigned temp = number;
			for (int i =0; i< log_radix_tp_L; i++)
			{
		#pragma HLS UNROLL
				unsigned int log2_r_bits_lsbs = temp & mask;
				temp = temp >> log2_radix;
				result = (result << log2_radix) | log2_r_bits_lsbs;
			}

			return result;
		}

		template < unsigned int tp_L, unsigned int tp_R>
		unsigned int digitReversal_m(unsigned int p_number)
		{
		#pragma HLS INLINE
			//CHECK_COVEARAGE;

			ap_uint< ssr_fft_log2<tp_L>::val > number = p_number;
			ap_uint< ssr_fft_log2<tp_L>::val > reversedNumber =0;
			ap_uint < ssr_fft_log2<tp_R>::val > digitContainer;
			const unsigned int log2_of_R = ssr_fft_log2<tp_R>::val;
			const unsigned int log2_of_L = ssr_fft_log2<tp_L>::val;
			const unsigned int numDigits = log2_of_L / log2_of_R;
			const unsigned int fracDigitBits = log2_of_L % log2_of_R;
			for (int i = 0; i < numDigits; i++)
			{
		#pragma HLS UNROLL
				//reversedNumber << log2_of_R;

				reversedNumber(log2_of_L-(i*log2_of_R)-1,log2_of_L-((i+1)*log2_of_R))= number((log2_of_R*(i+1))-1,(log2_of_R*i));
			}
			if(  (log2_of_L % log2_of_R) != 0 )
			reversedNumber(fracDigitBits-1,0) =number(log2_of_L-1,log2_of_L-fracDigitBits);


			unsigned int result = reversedNumber.to_uint();


			return result;

		}



		template < unsigned int tp_L, unsigned int tp_R>
		unsigned int digitReversal_fracIsLSB(unsigned int p_number)
		{
		#pragma HLS INLINE
			//CHECK_COVEARAGE;

			ap_uint< ssr_fft_log2<tp_L>::val > number = p_number;
			ap_uint< ssr_fft_log2<tp_L>::val > reversedNumber =0;
			ap_uint < ssr_fft_log2<tp_R>::val > digitContainer;
			const unsigned int log2_of_R = ssr_fft_log2<tp_R>::val;
			const unsigned int log2_of_L = ssr_fft_log2<tp_L>::val;
			const unsigned int numDigits = log2_of_L / log2_of_R;
			const unsigned int fracDigitBits = log2_of_L % log2_of_R;
			if(  (log2_of_L % log2_of_R) != 0 )
			reversedNumber(log2_of_L-1, log2_of_L-fracDigitBits) = number(fracDigitBits-1,0);
			for (int i = 0; i < numDigits; i++)
			{
		#pragma HLS UNROLL
				//reversedNumber << log2_of_R;

				reversedNumber(   (log2_of_L-fracDigitBits-1) - (log2_of_R*i) , (log2_of_L-fracDigitBits) - (log2_of_R*(i+1)) )=number((log2_of_R*(i+1))+fracDigitBits-1,(log2_of_R*i+fracDigitBits));
			}
			//if(  (log2_of_L % log2_of_R) != 0 )
			//reversedNumber(fracDigitBits-1,0) =number(log2_of_L-1,log2_of_L-fracDigitBits);


			unsigned int result = reversedNumber.to_uint();


			return result;

		}


	} //namespace hls
} //namespace ssr_fft

#endif /* HLS_SSR_FFT_UTILITIES_H_ */



