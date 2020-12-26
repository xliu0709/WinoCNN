/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2008-2013 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#ifndef X_HLS_TRAITS_H
#define X_HLS_TRAITS_H

#include "ap_fixed.h"
#include "hls_half.h"
#include "x_hls_utils.h"
#include "x_hls_defines.h"
#include <stdint.h>

namespace hls
{

template<typename T>
struct x_traits_default {
 	typedef T ADD_T;
 	typedef T SUB_T;
 	typedef T SAT_T;
 	typedef T MULT_T;
 	typedef T MADD_T;
};

template<typename T1, typename T2>
struct x_traits {
// 	typedef T1 ADD_T;
// 	typedef T1 SAT_T;
// 	typedef T1 MULT_T;
// 	typedef T1 MADD_T;
// 	typedef T1 CMADD_T;
// 	typedef T1 CONJ_SQ_T;
// 	typedef T1 CONJ_SQ_2_T;
// 	typedef int INT_T;
//     typedef T1 INV_T;
//     typedef float_struct<24,8> FLOAT_STRUCT_T;

};

template<>
struct x_traits<double, double> : public x_traits_default<double> { };

template<typename T2>
struct x_traits<double, T2> : public x_traits_default<double> { };

template<typename T1>
struct x_traits<T1, double> : public x_traits_default<double> { };

template<>
struct x_traits<float, float> : public x_traits_default<float> { };

template<typename T2>
struct x_traits<float, T2> : public x_traits_default<float> { };

template<typename T1>
struct x_traits<T1, float> : public x_traits_default<float> { };

template<>
struct x_traits<half, half> : public x_traits_default<half> { };

template<typename T2>
struct x_traits<half, T2> : public x_traits_default<float> { };

template<typename T1>
struct x_traits<T1, half> : public x_traits_default<float> { };

template<>
struct x_traits<double, float> : public x_traits_default<double> { };

template<>
struct x_traits<float, double> : public x_traits_default<double> { };

template<>
struct x_traits<double, half> : public x_traits_default<double> { };

template<>
struct x_traits<half, double> : public x_traits_default<double> { };

template<>
struct x_traits<int, int> : public x_traits_default<int> { };

template<>
struct x_traits<unsigned int, unsigned int> : public x_traits_default<unsigned int> { };

template<>
struct x_traits<int64_t, int64_t> : public x_traits_default<int64_t> { };

template<int A,int B> class MAX{
  public: 
    const static int value = (A > B) ? A : B;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_fixed<W2,I2,Q2,O2> > {
    typedef typename ap_fixed<W1,I1>::template RType<W2,I2,true> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
	typedef ap_fixed<W1,I1,AP_RND,AP_SAT> 	SAT_T;
	typedef ap_fixed<W1+W2+1,I1+I2+1> 		MADD_T;
	typedef ap_fixed<W1+W2+2,I1+I2+2> 		CMADD_T;
	typedef ap_fixed<(2*W1)+1,(2*I1)> 		CONJ_SQ_T;
	typedef ap_fixed<(2*W1)+2,(2*I1)+1> 	CONJ_SQ_2_T;
    typedef ap_fixed<2*(MAX<I1,W1-I1>::value+1),(MAX<I1,W1-I1>::value+1)> INV_T;
    typedef int INT_T;
    typedef float_struct<24,8> FLOAT_STRUCT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_ufixed<W2,I2,Q2,O2> > {
    typedef typename ap_fixed<W1,I1>::template RType<W2,I2,false> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_fixed<W2,I2,Q2,O2> > {
    typedef typename ap_ufixed<W1,I1>::template RType<W2,I2,true> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_ufixed<W2,I2,Q2,O2> > {
    typedef typename ap_ufixed<W1,I1>::template RType<W2,I2,false> RTYPE;
    typedef ap_ufixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_ufixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
	typedef ap_ufixed<W1,I1,AP_RND,AP_SAT> 	SAT_T;
	typedef ap_ufixed<W1+W2+1,I1+I2+1> 		MADD_T;
	typedef ap_ufixed<W1+W2+2,I1+I2+2> 		CMADD_T;
	typedef ap_ufixed<(2*W1)+1,(2*I1)> 		CONJ_SQ_T;
	typedef ap_ufixed<(2*W1)+2,(2*I1)+1> 	CONJ_SQ_2_T;
    typedef ap_ufixed<2*(MAX<I1,W1-I1>::value+1),(MAX<I1,W1-I1>::value+1)> INV_T;
    typedef int INT_T;
    typedef float_struct<24,8> FLOAT_STRUCT_T;
};

template<int W1, int W2>
struct x_traits<ap_int<W1>,ap_int<W2> > {
	typedef ap_int<MAX<W1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
	typedef ap_int<(W1 > W2)? W1:W2> MADD_T;
};

template<int W1, int W2>
struct x_traits<ap_int<W1>,ap_uint<W2> > {
	typedef ap_int<MAX<W1,W2+1>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2+1>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
};

template<int W1, int W2>
struct x_traits<ap_uint<W1>,ap_int<W2> > {
	typedef ap_int<MAX<W1+1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1+1,W2>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
};

template<int W1, int W2>
struct x_traits<ap_uint<W1>,ap_uint<W2> > {
	typedef ap_uint<MAX<W1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2>::value + 1> 			SUB_T;
	typedef ap_uint<W1+W2>            MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_int<W2> > : public x_traits<ap_fixed<W1,I1>, ap_fixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_uint<W2> > : public x_traits<ap_fixed<W1,I1>, ap_ufixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_int<W2> > : public x_traits<ap_ufixed<W1,I1>, ap_fixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_uint<W2> > : public x_traits<ap_ufixed<W1,I1>, ap_ufixed<W2,W2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_int<W1>, ap_fixed<W2,I2,Q2,O2> > : public x_traits<ap_fixed<W1,W1>, ap_fixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_int<W1>, ap_ufixed<W2,I2,Q2,O2> > : public x_traits<ap_fixed<W1,W1>, ap_ufixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_uint<W1>, ap_fixed<W2,I2,Q2,O2> > : public x_traits<ap_ufixed<W1,W1>, ap_fixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_uint<W1>, ap_ufixed<W2,I2,Q2,O2> > : public x_traits<ap_ufixed<W1,W1>, ap_ufixed<W2,I2> > {};

// fixed_type<X>::T is the smallest ap_fixed/ap_ufixed type that can losslessly contain X
// e.g. X x;  fixed_type<X>::T y(x); is sensible;

    template<typename T> struct fixed_type { };
    template<> struct fixed_type<char> { typedef ap_fixed<8,8> T; };
    template<> struct fixed_type<unsigned char> { typedef ap_ufixed<8,8> T; };
    template<> struct fixed_type<short> { typedef ap_fixed<16,16> T; };
    template<> struct fixed_type<unsigned short> { typedef ap_ufixed<16,16> T; };
    template<> struct fixed_type<int> { typedef ap_fixed<32,32> T; };
    template<> struct fixed_type<unsigned int> { typedef ap_ufixed<32,32> T; };
    template<> struct fixed_type<float> { typedef ap_ufixed<64,32> T; };
    template<int W> struct fixed_type<ap_int<W> > { typedef ap_fixed<W,W> T; };
    template<int W> struct fixed_type<ap_uint<W> > { typedef ap_ufixed<W,W> T; };
    template<int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O> struct fixed_type<ap_fixed<W,I, _AP_Q, _AP_O> > { typedef ap_fixed<W,I> T; };
    template<int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O> struct fixed_type<ap_ufixed<W,I, _AP_Q, _AP_O> > { typedef ap_ufixed<W,I> T; };

template<typename T1, int DIM>
struct x_traits_d_default {
    typedef T1 ACCUM_T;
    typedef T1 ACCUM2_T;
    typedef T1 ADD_TREE_T;
};

template<typename T1, int DIM>
struct x_traits_d {};

template<int DIM>
struct x_traits_d <float,DIM>:x_traits_d_default<float, DIM> {};

template<int DIM>
struct x_traits_d <double,DIM>:x_traits_d_default<double, DIM> {};

template<int DIM>
struct x_traits_d <half,DIM>:x_traits_d_default<half, DIM> {};

template<int W, int DIM>
struct x_traits_d <ap_int<W>,DIM> {
    typedef ap_int<W> ACCUM_T;
    typedef ap_int<2*W> ACCUM2_T;
    typedef ap_int<25+W> ADD_TREE_T;
};

template<int W, int DIM>
struct x_traits_d <ap_uint<W>,DIM> {
    typedef ap_uint<W> ACCUM_T;
    typedef ap_uint<2*W> ACCUM2_T;
    typedef ap_uint<25+W> ADD_TREE_T;
};

template<int W, int I, ap_q_mode Q, ap_o_mode O, int DIM>
struct x_traits_d <ap_fixed<W,I,Q,O>,DIM> {
    typedef ap_fixed<W+BitWidth<DIM>::Value,I+BitWidth<DIM>::Value,Q,O>         ACCUM_T;
    typedef ap_fixed<(2*W)+BitWidth<DIM>::Value,(2*I)+BitWidth<DIM>::Value,Q,O> ACCUM2_T;
    typedef ap_fixed<25+W+BitWidth<DIM>::Value,25+I+BitWidth<DIM>::Value,Q,O> ADD_TREE_T; // TODO placeholder
};

template<int W, int I, ap_q_mode Q, ap_o_mode O, int DIM>
struct x_traits_d <ap_ufixed<W,I,Q,O>,DIM> {
    typedef ap_ufixed<W+BitWidth<DIM>::Value,I+BitWidth<DIM>::Value,Q,O>         ACCUM_T;
    typedef ap_ufixed<(2*W)+BitWidth<DIM>::Value,(2*I)+BitWidth<DIM>::Value,Q,O> ACCUM2_T;
    typedef ap_ufixed<25+W+BitWidth<DIM>::Value,25+I+BitWidth<DIM>::Value,Q,O> ADD_TREE_T; // TODO placeholder
};

}



namespace hlstmp
{

template<typename T>
struct x_traits_default {
 	typedef T ADD_T;
 	typedef T SUB_T;
 	typedef T SAT_T;
 	typedef T MULT_T;
 	typedef T MADD_T;
};

template<typename T1, typename T2>
struct x_traits {
// 	typedef T1 ADD_T;
// 	typedef T1 SAT_T;
// 	typedef T1 MULT_T;
// 	typedef T1 MADD_T;
// 	typedef T1 CMADD_T;
// 	typedef T1 CONJ_SQ_T;
// 	typedef T1 CONJ_SQ_2_T;
// 	typedef int INT_T;
//     typedef T1 INV_T;
//     typedef float_struct<24,8> FLOAT_STRUCT_T;

};

template<>
struct x_traits<double, double> : public x_traits_default<double> { };

template<typename T2>
struct x_traits<double, T2> : public x_traits_default<double> { };

template<typename T1>
struct x_traits<T1, double> : public x_traits_default<double> { };

template<>
struct x_traits<float, float> : public x_traits_default<float> { };

template<typename T2>
struct x_traits<float, T2> : public x_traits_default<float> { };

template<typename T1>
struct x_traits<T1, float> : public x_traits_default<float> { };

template<>
struct x_traits<half, half> : public x_traits_default<half> { };

template<typename T2>
struct x_traits<half, T2> : public x_traits_default<float> { };

template<typename T1>
struct x_traits<T1, half> : public x_traits_default<float> { };

template<>
struct x_traits<double, float> : public x_traits_default<double> { };

template<>
struct x_traits<float, double> : public x_traits_default<double> { };

template<>
struct x_traits<double, half> : public x_traits_default<double> { };

template<>
struct x_traits<half, double> : public x_traits_default<double> { };

template<>
struct x_traits<int, int> : public x_traits_default<int> { };

template<>
struct x_traits<unsigned int, unsigned int> : public x_traits_default<unsigned int> { };

template<>
struct x_traits<int64_t, int64_t> : public x_traits_default<int64_t> { };

template<int A,int B> class MAX{
  public: 
    const static int value = (A > B) ? A : B;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_fixed<W2,I2,Q2,O2> > {
    typedef typename ap_fixed<W1,I1>::template RType<W2,I2,true> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
	typedef ap_fixed<W1,I1,AP_RND,AP_SAT> 	SAT_T;
	typedef ap_fixed<W1+W2+1,I1+I2+1> 		MADD_T;
	typedef ap_fixed<W1+W2+2,I1+I2+2> 		CMADD_T;
	typedef ap_fixed<(2*W1)+1,(2*I1)> 		CONJ_SQ_T;
	typedef ap_fixed<(2*W1)+2,(2*I1)+1> 	CONJ_SQ_2_T;
    typedef ap_fixed<2*(MAX<I1,W1-I1>::value+1),(MAX<I1,W1-I1>::value+1)> INV_T;
    typedef int INT_T;
    typedef float_struct<24,8> FLOAT_STRUCT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_ufixed<W2,I2,Q2,O2> > {
    typedef typename ap_fixed<W1,I1>::template RType<W2,I2,false> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_fixed<W2,I2,Q2,O2> > {
    typedef typename ap_ufixed<W1,I1>::template RType<W2,I2,true> RTYPE;
    typedef ap_fixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_fixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_ufixed<W2,I2,Q2,O2> > {
    typedef typename ap_ufixed<W1,I1>::template RType<W2,I2,false> RTYPE;
    typedef ap_ufixed<RTYPE::plus_w, RTYPE::plus_i> ADD_T;
    typedef ap_fixed<RTYPE::minus_w, RTYPE::minus_i> SUB_T;
    typedef ap_ufixed<RTYPE::mult_w, RTYPE::mult_i> MULT_T;
	typedef ap_ufixed<W1,I1,AP_RND,AP_SAT> 	SAT_T;
	typedef ap_ufixed<W1+W2+1,I1+I2+1> 		MADD_T;
	typedef ap_ufixed<W1+W2+2,I1+I2+2> 		CMADD_T;
	typedef ap_ufixed<(2*W1)+1,(2*I1)> 		CONJ_SQ_T;
	typedef ap_ufixed<(2*W1)+2,(2*I1)+1> 	CONJ_SQ_2_T;
    typedef ap_ufixed<2*(MAX<I1,W1-I1>::value+1),(MAX<I1,W1-I1>::value+1)> INV_T;
    typedef int INT_T;
    typedef float_struct<24,8> FLOAT_STRUCT_T;
};

template<int W1, int W2>
struct x_traits<ap_int<W1>,ap_int<W2> > {
	typedef ap_int<MAX<W1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
	typedef ap_int<(W1 > W2)? W1:W2> MADD_T;
};

template<int W1, int W2>
struct x_traits<ap_int<W1>,ap_uint<W2> > {
	typedef ap_int<MAX<W1,W2+1>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2+1>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
};

template<int W1, int W2>
struct x_traits<ap_uint<W1>,ap_int<W2> > {
	typedef ap_int<MAX<W1+1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1+1,W2>::value + 1> 			SUB_T;
	typedef ap_int<W1+W2>            MULT_T;
};

template<int W1, int W2>
struct x_traits<ap_uint<W1>,ap_uint<W2> > {
	typedef ap_uint<MAX<W1,W2>::value + 1> 			ADD_T;
	typedef ap_int<MAX<W1,W2>::value + 1> 			SUB_T;
	typedef ap_uint<W1+W2>            MULT_T;
};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_int<W2> > : public x_traits<ap_fixed<W1,I1>, ap_fixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_fixed<W1,I1,Q1,O1>,ap_uint<W2> > : public x_traits<ap_fixed<W1,I1>, ap_ufixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_int<W2> > : public x_traits<ap_ufixed<W1,I1>, ap_fixed<W2,W2> > {};

template<int W1, int I1, ap_q_mode Q1, ap_o_mode O1, int W2>
struct x_traits<ap_ufixed<W1,I1,Q1,O1>,ap_uint<W2> > : public x_traits<ap_ufixed<W1,I1>, ap_ufixed<W2,W2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_int<W1>, ap_fixed<W2,I2,Q2,O2> > : public x_traits<ap_fixed<W1,W1>, ap_fixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_int<W1>, ap_ufixed<W2,I2,Q2,O2> > : public x_traits<ap_fixed<W1,W1>, ap_ufixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_uint<W1>, ap_fixed<W2,I2,Q2,O2> > : public x_traits<ap_ufixed<W1,W1>, ap_fixed<W2,I2> > {};

template<int W1, int W2, int I2, ap_q_mode Q2, ap_o_mode O2>
struct x_traits<ap_uint<W1>, ap_ufixed<W2,I2,Q2,O2> > : public x_traits<ap_ufixed<W1,W1>, ap_ufixed<W2,I2> > {};

// fixed_type<X>::T is the smallest ap_fixed/ap_ufixed type that can losslessly contain X
// e.g. X x;  fixed_type<X>::T y(x); is sensible;

    template<typename T> struct fixed_type { };
    template<> struct fixed_type<char> { typedef ap_fixed<8,8> T; };
    template<> struct fixed_type<unsigned char> { typedef ap_ufixed<8,8> T; };
    template<> struct fixed_type<short> { typedef ap_fixed<16,16> T; };
    template<> struct fixed_type<unsigned short> { typedef ap_ufixed<16,16> T; };
    template<> struct fixed_type<int> { typedef ap_fixed<32,32> T; };
    template<> struct fixed_type<unsigned int> { typedef ap_ufixed<32,32> T; };
    template<> struct fixed_type<float> { typedef ap_ufixed<64,32> T; };
    template<int W> struct fixed_type<ap_int<W> > { typedef ap_fixed<W,W> T; };
    template<int W> struct fixed_type<ap_uint<W> > { typedef ap_ufixed<W,W> T; };
    template<int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O> struct fixed_type<ap_fixed<W,I, _AP_Q, _AP_O> > { typedef ap_fixed<W,I> T; };
    template<int W, int I, ap_q_mode _AP_Q, ap_o_mode _AP_O> struct fixed_type<ap_ufixed<W,I, _AP_Q, _AP_O> > { typedef ap_ufixed<W,I> T; };

template<typename T1, int DIM>
struct x_traits_d_default {
    typedef T1 ACCUM_T;
    typedef T1 ACCUM2_T;
    typedef T1 ADD_TREE_T;
};

template<typename T1, int DIM>
struct x_traits_d {};

template<int DIM>
struct x_traits_d <float,DIM>:x_traits_d_default<float, DIM> {};

template<int DIM>
struct x_traits_d <double,DIM>:x_traits_d_default<double, DIM> {};

template<int DIM>
struct x_traits_d <half,DIM>:x_traits_d_default<half, DIM> {};

template<int W, int DIM>
struct x_traits_d <ap_int<W>,DIM> {
    typedef ap_int<W> ACCUM_T;
    typedef ap_int<2*W> ACCUM2_T;
    typedef ap_int<25+W> ADD_TREE_T;
};

template<int W, int DIM>
struct x_traits_d <ap_uint<W>,DIM> {
    typedef ap_uint<W> ACCUM_T;
    typedef ap_uint<2*W> ACCUM2_T;
    typedef ap_uint<25+W> ADD_TREE_T;
};

template<int W, int I, ap_q_mode Q, ap_o_mode O, int DIM>
struct x_traits_d <ap_fixed<W,I,Q,O>,DIM> {
    typedef ap_fixed<W+BitWidth<DIM>::Value,I+BitWidth<DIM>::Value,Q,O>         ACCUM_T;
    typedef ap_fixed<(2*W)+BitWidth<DIM>::Value,(2*I)+BitWidth<DIM>::Value,Q,O> ACCUM2_T;
    typedef ap_fixed<25+W+BitWidth<DIM>::Value,25+I+BitWidth<DIM>::Value,Q,O> ADD_TREE_T; // TODO placeholder
};

template<int W, int I, ap_q_mode Q, ap_o_mode O, int DIM>
struct x_traits_d <ap_ufixed<W,I,Q,O>,DIM> {
    typedef ap_ufixed<W+BitWidth<DIM>::Value,I+BitWidth<DIM>::Value,Q,O>         ACCUM_T;
    typedef ap_ufixed<(2*W)+BitWidth<DIM>::Value,(2*I)+BitWidth<DIM>::Value,Q,O> ACCUM2_T;
    typedef ap_ufixed<25+W+BitWidth<DIM>::Value,25+I+BitWidth<DIM>::Value,Q,O> ADD_TREE_T; // TODO placeholder
};

}


#endif



