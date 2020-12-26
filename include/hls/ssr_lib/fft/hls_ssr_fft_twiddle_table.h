// File Name : hls_ssr_fft_twiddle_table.h
#ifndef HLS_SSR_FFT_TWIDDLE_TALBE_H_
#define HLS_SSR_FFT_TWIDDLE_TALBE_H_

//#define HLS_SSR_FFT_TWIDDLE_TALBE_PRINT_DEBUG_MESSAGES
//#define HLS_SSR_FFT_DEBUG

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-  initTwiddleFactors is defined here. It initializes complex exponential tables    -_-
-_-  that is used for calculating butterflies. three template parameters are passed
	 	 	 tp_R : The radix of the SSR FFT to be calculated
	 	 	 tp_L : the length of the SSR FFT to be calculated
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
 */


#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
#include "../spu.h"
#endif
#include <ap_fixed.h>

namespace hls {
	namespace ssr_fft {



		template <int tp_L, int tp_R,typename tt_dtype>
		struct  twiddle_table { };


		template <int tp_L, int tp_R,typename tt_dtype>
		struct twiddle_table<tp_L,tp_R,std::complex < tt_dtype > > {
		static void initTwiddleFactors(std::complex < tt_dtype > table[ twiddle_traits_len< tp_L,tp_R> ::TWIDDLE_TALBE_LENGTH ])
		{
		#pragma HLS INLINE off


			for (int i = 0; i<(twiddle_traits_len< tp_L,tp_R> ::TWIDDLE_TALBE_LENGTH); i++)
			{

				double real =  cos((2 * i*M_PI) / tp_L);
				double imag=-sin((2 * i*M_PI) / tp_L);
				table[i].real(real);
				table[i].imag(imag);
			}
		}
		};


		template <int tp_L, int tp_R,unsigned int index_bw,typename tt_dtype>
		tt_dtype   readTwiddleTable(ap_uint<index_bw> index,tt_dtype table[])
		{
#pragma HLS INLINE
			//ap_uint < ssr_fft_log2<tp_L/tp_R>::val > index_modulus =  index (ssr_fft_log2<tp_L/tp_R>::val-1,0);
			//std::cout << "Index: Index modulus = " <<index<<":"<< index_modulus<<"\n";
			typedef typename twiddle_traits<tt_dtype>::TTR_twiddleInnerType tableType;
			tableType SinVal = table[index].real();
			tableType cosVal = table[index].imag();
			//std::cout<<SinVal<<":SinValue\n";
			//std::cout<<cosVal<<":cosVal\n";
			tt_dtype sample;
			sample.real(SinVal);
			sample.imag(cosVal);
			return sample; //table[index];
		}


		template <int tp_L, int tp_R,unsigned int phase_bit_width,typename tt_dtype>
		tt_dtype   readQuaterTwiddleTable(ap_uint<phase_bit_width> index,tt_dtype table[])
		{
#pragma HLS INLINE
			const ap_int<2> MAX_OUT = -1;
			ap_uint<phase_bit_width> index_cos =  index + (3*tp_L / 4);
			ap_uint<phase_bit_width> index_sin =  index;
			typedef typename twiddle_traits<tt_dtype>::TTR_twiddleInnerType tableType;
			tableType realCosVal;
			tableType imagSinVal;
			tt_dtype complexOut;

			ap_uint<1> index_invert_control_imag = index_sin(phase_bit_width-2,phase_bit_width-2);
		    ap_uint<1> output_negate_control_imag = index_sin(phase_bit_width-1,phase_bit_width-1);
		    ap_uint<1> output_saturation_control_imag = (  index_sin==(tp_L/4) ||  (index_sin== (3*tp_L/4) ) );
		    ap_uint<phase_bit_width-2> lut_index_imag = index_sin(phase_bit_width-3,0);
			if( index_invert_control_imag==1)
				lut_index_imag = ( (~lut_index_imag) + 1 );
			//lut_index=bitSelect(lut_index,phase_bit_width-1-2,0) ::: was modulus in matlab not required here
			tableType lut_out_imag = table[lut_index_imag].imag();
			//std::cout<<"lut_index_imag:"<<lut_index_imag<<"\n";

			tableType temp_out_sin;
			if(output_saturation_control_imag)
			    temp_out_sin = MAX_OUT;
			else
			    temp_out_sin = lut_out_imag;

			if(output_negate_control_imag)
				imagSinVal= -temp_out_sin;
			  else
				  imagSinVal=temp_out_sin;

			ap_uint<1> index_invert_control_real = index_cos(phase_bit_width-2,phase_bit_width-2);
		    ap_uint<1> output_negate_control_real = index_cos(phase_bit_width-1,phase_bit_width-1);
		    ap_uint<1> output_saturation_control_real = (  index_cos==(tp_L/4) ||  (index_cos== (3*tp_L/4) ) );
		    ap_uint<phase_bit_width-2> lut_index_real = index_cos(phase_bit_width-3,0);
			if( index_invert_control_real==1)
				lut_index_real = ( (~lut_index_real) + 1 );
			//lut_index=bitSelect(lut_index,phase_bit_width-1-2,0) ::: was modulus in matlab not required here
			//std::cout<<"lut_index_real:"<<lut_index_real<<"\n";
			tableType lut_out_real = table[lut_index_real].imag();
			tableType temp_out_cos;
			if(output_saturation_control_real)
			    temp_out_cos = MAX_OUT;
			else
			    temp_out_cos = lut_out_real;
			if(output_negate_control_real)
				realCosVal= -temp_out_cos;
			  else
				realCosVal=temp_out_cos;

			complexOut.imag(imagSinVal);
			complexOut.real(realCosVal);
			return complexOut;
			//return table[index];
		}


	} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFT_TWIDDLE_TALBE_H_

