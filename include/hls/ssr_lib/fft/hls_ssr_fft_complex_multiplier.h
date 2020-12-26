// File Name  : hls_ssr_fft_complex_multiplier.h

#ifndef _HLS_SSR_FFT_COMPLEX_MULTIPLIER_H_
#define _HLS_SSR_FFT_COMPLEX_MULTIPLIER_H_
#include <complex>
namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	template < typename T1, typename T2 , typename T3 >
	void complex_multiply(std::complex<T1> complex_op1 ,std::complex<T2> complex_op2 , std::complex< T3 > &product )
	{
	#pragma HLS INLINE //recursive
		product.real( complex_op1.real()*complex_op2.real() - complex_op1.imag()*complex_op2.imag()) ;
		product.imag( complex_op1.real()*complex_op2.imag() + complex_op1.imag()*complex_op2.real()) ;

	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
} //end name space hls
} ////end name space ssr_fft
#endif //_HLS_SSR_FFT_COMPLEX_MULTIPLIER_H_
