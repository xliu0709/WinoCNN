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
 *     (c) Copyright 2008-2018 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#ifndef _X_HLS_CORDIC_APFIXED_H_
#define _X_HLS_CORDIC_APFIXED_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include <iostream>

namespace cordic_apfixed {

  extern ap_ufixed<128,2> circ_table_arctan_128[128];
  extern int hyperb_table_tau_128[128];
  extern ap_ufixed<128,4> hyperb_table_arctanh_128[128];

  /*
   * Range reduces input to between 0 and pi/2 by
   * solving for k and r in x = k*(pi/2) + r
   */
  template<int W, int I, int Wo>
  void circ_range_redux( ap_ufixed<W,I> x,
						 ap_uint<2> &k,
						 ap_ufixed<Wo,1> &r) {
	
	ap_ufixed<(Wo+I),0> inv_pi2("0x0.A2F9836E4E43FC715BBF"); // 2/pi
	ap_ufixed<Wo+1,1> pi2("1.5707963267948966192313216916397514420985846996876"); // pi/2
	ap_ufixed<Wo+I,I> prod = x * inv_pi2;
	ap_uint<I> kint = prod;

    k = kint;
	r = x - kint * pi2;
  };

  /*
   * Main CORDIC sub-function
   * MODE 0: 'Rotation' : rotate by angle z.  x' <- K*(x*cos(z) - y*sin(z)), y' <- K*(x*sin(z) + y*cos(z))
   * MODE 1: 'Vectoring': rotate y to zero.   x' <- sqrt(x^2 + y^2)  , z' <- atan(y/x)
   */
  template<int W, int I, int MODE>
  void cordic_circ_apfixed( ap_fixed<W,I> &x, 
							ap_fixed<W,I> &y, 
							ap_fixed<W,I> &z) {

	const int Nmax = W;

	ap_fixed<W,I> tx, ty, tz;
	ap_fixed<W,I> x_s, y_s, z_s;
	ap_uint<1> d;

	for (int n=0; n<Nmax; n++){
#pragma HLS pipeline	  

	  if(MODE == 1) { // Vectoring mode
		d = ~y[W-1];
	  } 
	  else {          // Rotation mode 
		d = z[W-1];
	  }

	  x_s = x >> n;
	  y_s = y >> n;
	  z_s = circ_table_arctan_128[n];
	 
	  if(d){          // y[n-1] is positive
		tx = x + y_s;
		ty = y - x_s;
		tz = z + z_s;
	  }
	  else{ 
		tx = x - y_s;
		ty = y + x_s;
		tz = z - z_s;
	  }

	  x = tx; 
	  y = ty; 
	  z = tz;

#ifdef _HLSCLIB_DEBUG_
#ifndef __SYNTHESIS__
	  std::cout << "  x_s: " << x_s
	  			<< "  y_s: " << y_s
	  			<< "  z_s: " << z_s
	  			<< "  x : " << x
	  			<< "  y : " << y
	  			<< "  z : " << z << std::endl;
#endif
#endif
    }
  };


  /*
   * Main double-CORDIC sub-function
   * MODE 0: asin
   * MODE 1: acos
   */
  template<int W, int I, int MODE, int INTYPE>
  void doublecordic_apfixed( ap_fixed<W,I> &x, 
                             ap_fixed<W,I> &y, 
                             ap_fixed<W,I> &z, 
                             ap_fixed<W,I> &t) {

    const int Nmax = W - I + 1;

    for (int n=0; n<Nmax; n++){

      if(INTYPE==64){
#pragma HLS pipeline II=5
      }
      else if(INTYPE==32){
#pragma HLS pipeline II=3
      }
      else if(INTYPE==16){
#pragma HLS pipeline II=1
      }
      else{
#pragma HLS pipeline II=4
      }

      ap_uint<1> d;
      if(MODE == 0) { // asin
        if(y<=t){
          d = x[W-1];
        }
        else{
          d = ~x[W-1];
        }
      } 
      else {          // acos
        if(x>=t){
          d = y[W-1];
        }
        else{
          d = ~y[W-1];
        }
      }

      ap_fixed<W,I> x_s = x >> (n-1);
      ap_fixed<W,I> y_s = y >> (n-1);
      ap_fixed<W,I> x_s2 = x_s >> (n+1);
      ap_fixed<W,I> y_s2 = y_s >> (n+1);

	  x = x - x_s2;
	  y = y - y_s2;
      if(d){ // d==1, negtive
        x = x + y_s;
        y = y - x_s;
      }
      else{  // d==0, positive
        x = x - y_s;
        y = y + x_s;
      }

      // t
	  ap_fixed<W,I> t_s;
	  int n2 = (n << 1); // 2*n
      if(n2<W){
        t_s = (t >> n2);
		t = t + t_s;
      }

      // z
      ap_fixed<W,I> ca = circ_table_arctan_128[n];
      ap_fixed<W,I> z_s = (ca << 1);
	  ap_fixed<W,I> tz;
      if(d){ // d==1, negtive
        tz = z - z_s;
      }
      else{  // d==0, positive
        tz = z + z_s;
      }
      z = tz;

#ifdef _HLSCLIB_DEBUG_
#ifndef __SYNTHESIS__
      std::cout << "  x : " << x
                << "  y : " << y
                << "  z : " << z 
                << "  t : " << t 
                << std::endl;
#endif
#endif
    }
  };


  // sin cos
  template<int W, int I>
  void generic_sincos(ap_fixed<W,I> in, 
					  ap_fixed<W-I+2,2> & outsin, ap_fixed<W-I+2,2> & outcos){ 

	ap_uint<1> sign0 = (in > 0) ? 1: 0;

	// abs(in)
	ap_ufixed<W,I> inabs;
	if(sign0)
	  inabs = in;
	else
	  inabs = -in;

	// inabs = k * (pi/2) + r
	const int Fr = (W - I) + I + 1;
	ap_uint<2> k;
	ap_ufixed<Fr,1> r;
	circ_range_redux(inabs, k, r);

	ap_fixed<W+2,3> x, y, z;

	const ap_fixed<W+2,3> K1("0x0.9B74EDA8435E47F277F7");
	x = K1;
	y = 0;
	
	if(k==1 or k==3){
	  ap_ufixed<Fr,1> pi2("0x1.921FB54442D1846");
	  // ap_ufixed<Fr,1> pi2("1.5707963267948966192313216916397514420985846996876");
	  z = pi2 - r;
	}
	else{
	  z = r;
	}
	
#ifdef _HLSCLIB_DEBUG_
#ifndef __SYNTHESIS__
	std::cout << "xyz:  " << x << "  " << y << "  " << z << std::endl;
#endif
#endif
	
	cordic_circ_apfixed<W+2,3,0>(x,y,z);
	
#ifdef _HLSCLIB_DEBUG_
#ifndef __SYNTHESIS__
	std::cout << "xyz:  " << x << "  " << y << "  " << z << std::endl;
#endif	
#endif
	
	if(k==0){
	  outsin = y;
	  outcos = x;
	}
	else if(k==1){
	  outsin = y;
	  outcos = -x;
	}
	else if(k==2){
	  outsin = -y;
	  outcos = -x;
	}
	else{
	  outsin = -y;
	  outcos = x;
	}
	
	if(sign0==0)
	  outsin = -outsin;	
  }; 
  
  // tan
  template<int W, int I>
  ap_fixed<W,I> generic_tan(ap_fixed<W,I> x){
	
	const int Ix = I;
	const int Fx = (((W-I)>(I-1)) ? (W-I): (I-1)) + I;
	const int Wx = I + Fx;

	ap_fixed<Wx,Ix> xs = x;

	ap_fixed<Wx-Ix+2,2> outsin;
	ap_fixed<Wx-Ix+2,2> outcos;	
	generic_sincos(xs, outsin, outcos);

	ap_fixed<W,I> out = outsin / outcos;

	// overflow tan
	if(I==1){
	  if(out[W-1]==1){
		out[W-1] = 0;
		for(int i=0;i<W-1;i++){
#pragma HLS UNROLL
		  out[i] = 1;
		}	  
	  }
	}

	return out;
  };

  // tan(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,I> generic_tan(ap_ufixed<W,I> in) {
	ap_fixed<W+1,I+1> fin = in;
	ap_fixed<W+1,I+1> fout = generic_tan(fin);
	ap_ufixed<W,I> out = fout;
	return out;
  };  

  // tan(ap_int)
  template<int I>
  ap_int<I> generic_tan(ap_int<I> in) {
	ap_fixed<I,I> fin = in;
	ap_fixed<I,I> fout = generic_tan(fin);
	ap_int<I> out = fout;
	return out;
  };  

  // tan(ap_uint)
  template<int I>
  ap_uint<I> generic_tan(ap_uint<I> in) {
	ap_fixed<I+1,I+1> fin = in;
	ap_fixed<I+1,I+1> fout = generic_tan(fin);
	ap_uint<I> out = fout;
	return out;
  };  

  // atan
  template<int W, int I>
  ap_fixed<W,2> generic_atan(ap_fixed<W,I> in){ 
	
	ap_uint<1> sign0 = (in > 0) ? 1: 0;

	// abs(in)
	ap_fixed<W+1,I+1> inabs;
	if(sign0)
	  inabs = in;
	else
	  inabs = -in;

	// sign1
	ap_uint<1> sign1 = (inabs > 1) ? 1: 0;

	ap_fixed<W+2,3> x, y, z;
	if(sign1){
	  x = 1;
	  // y = ap_fixed<W+2,3>("0x1.0") / inabs;
	  y = 1.0 / inabs.to_double();
	  z = 0;
	}
	else{
	  x = 1;
	  y = inabs;
	  z = 0;
	}
	cordic_circ_apfixed<W+2,3,1>(x,y,z);

	ap_fixed<W,2> out;
	if(sign1){
	  const ap_fixed<W,2> pi2("0x1.921FB54442D1846"); // pi/2
	  out = pi2 - z;
	}
	else{
	  out = z;
	}

	// return
	if(sign0){
	  return out;
	}
	else{
	  return -out;
	} 
  }; 

  // atan(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,2> generic_atan(ap_ufixed<W,I> in){ 
	ap_fixed<W+1,I+1> fin = in;
	ap_fixed<W+1,2> fout = generic_atan(fin);
	ap_ufixed<W,2> out = fout;
	return out;
  };

  // atan(ap_int)
  template<int I>
  ap_int<2> generic_atan(ap_int<I> in){ 
	if(in<-1)
	  return ap_int<2>(-1);
	else if(in>1)
	  return ap_int<2>(1);
	else
	  return ap_int<2>(0);
  };

  // atan(ap_uint)
  template<int I>
  ap_uint<1> generic_atan(ap_uint<I> in){ 
	if(in>1)
	  return ap_uint<1>(1);
	else
	  return ap_uint<1>(0);
  };

  // atan2
  template<int W, int I>
  ap_fixed<W,3> generic_atan2(ap_fixed<W,I> in1, 
							  ap_fixed<W,I> in2){ 

	const ap_fixed<W+1,3> pi("0x3.243F6A8885A308D3");  // pi
	const ap_fixed<W+2,3> pi2("0x1.921FB54442D1846");  // pi/2
	const ap_fixed<W+1,3> pi4("0x0.C90FDAA22168C23");  // pi/4
	const ap_fixed<W+1,3> pi3_("-0x2.5B2F8FE6643A469");// -pi*(3/4)

	ap_uint<2> signin1 = (in1 > 0) ? 2: (in1==0) ? 1: 0;
	ap_uint<2> signin2 = (in2 > 0) ? 2: (in2==0) ? 1: 0;

	ap_fixed<W,3> out; // output 
	
	// 1. corner cases

	if(signin1==1 and signin2==2){
	  out = 0;
	  return out;
	}
	else if(signin1==1 and signin2==0){
	  out = pi;
	  return out;
	}
	else if(signin1==2 and signin2==1){
	  out = pi2;
	  return out;
	}
	else if(signin1==0 and signin2==1){
	  out = -pi2;
	  return out;
	}
	else if(in1==in2){
	  if(signin1==2){
		out = pi4;
		return out;
	  }
	  else if(signin1==1){
		out = 0;
		return out;
	  }
	  else{
		out = pi3_;
		return out;
	  }
	}

	// 2. non-corner cases

	ap_fixed<W+1,I+1> in1abs; // abs(in1)
	if(signin1==0)
	  in1abs = -in1;
	else
	  in1abs = in1;

	ap_fixed<W+1,I+1> in2abs; // abs(in2)
	if(signin2==0)
	  in2abs = -in2;
	else
	  in2abs = in2;

	ap_fixed<W+1,2> in1abs_sft; // shift
	ap_fixed<W+1,2> in2abs_sft;
	for(int i=0;i<W+1;i++){
#pragma HLS UNROLL
	  in1abs_sft[i] = in1abs[i];
	}
	for(int i=0;i<W+1;i++){
#pragma HLS UNROLL
	  in2abs_sft[i] = in2abs[i];
	}
	
	ap_fixed<W+7,3> x, y, z;
	if(in1abs > in2abs){
	  x = in1abs_sft;
	  y = in2abs_sft;
	  z = 0;
	}
	else{
	  x = in2abs_sft;
	  y = in1abs_sft;
	  z = 0;
	}
 	cordic_circ_apfixed<W+7,3,1>(x,y,z);

	if(in1abs > in2abs){
	  z = pi2 - z;
	}

	if(signin2==0 and signin1==2){
	  out = pi - z;
	}
	else if(signin2==0 and signin1==0){
	  out = z - pi;
	}
	else if(signin2==2 and signin1==0){
	  out = -z;
	}
	else{
	  out = z;
	}

	return out;
  }; 

  // atan2(ap_fixed)
  template<int W, int I>
  ap_ufixed<W,3> generic_atan2(ap_ufixed<W,I> in1, 
							   ap_ufixed<W,I> in2){ 
	ap_fixed<W+1,I+1> fin1 = in1;
	ap_fixed<W+1,I+1> fin2 = in2;
	ap_fixed<W+1,3> fout = generic_atan2(fin1, fin2);
	ap_ufixed<W,3> out = fout;
	return out;
  }

  // atan2(ap_int)
  template<int I>
  ap_int<3> generic_atan2(ap_int<I> in1, 
  						  ap_int<I> in2){ 
  	ap_fixed<I+1,I+1> fin1 = in1;
  	ap_fixed<I+1,I+1> fin2 = in2;
  	ap_fixed<I+1,3> fout = generic_atan2(fin1, fin2);
  	ap_int<3> out = fout;
  	return out;
  }

  // atan2(ap_uint)
  template<int I>
  ap_uint<2> generic_atan2(ap_uint<I> in1, 
  						   ap_uint<I> in2){ 
  	ap_fixed<I+1,I+1> fin1 = in1;
  	ap_fixed<I+1,I+1> fin2 = in2;
  	ap_fixed<I+1,3> fout = generic_atan2(fin1, fin2);
  	ap_uint<2> out = fout;
  	return out;
  }

  /*
   * Range reduces input to between 0 and ln2 by
   * solving for k and r in x = k*ln2 + r
   */
  template<int W, int I, int Wo>
  void hyperb_range_redux( ap_ufixed<W,I> x,
						   ap_uint<I+1> &k,
						   ap_ufixed<Wo,0> &r) {
	
	ap_ufixed<(Wo+I)+1,1> inv_ln2("0x1.71547652b82fe1777d0ffda0d23a7d11d6aef551cp0"); // 1.0/log(2)
	ap_ufixed<Wo,0> ln2("0xb.17217f7d1cf79abc9e3b39803f2f6af40f343267p-4");           // log(2)
	ap_ufixed<Wo+(I+1),I+1> prod = x * inv_ln2;
	ap_ufixed<Wo,0> prod_dec = prod;

    k = prod;
	r = prod_dec * ln2;
  };

  /*
   * Main hyperbolic CORDIC sub-function
   * MODE 0: 'Rotation' : rotate by angle z.  x' = K*(x*cosh(z) + y*sinh(z)), y' = x*sinh(z) + y*cosh(z)
   * MODE 1: 'Vectoring': rotate y to zero.   x' = K*sqrt(x^2 - y^2)  , z' = z+atanh(y/x)
   */
  template<int W, int I, int MODE>
  void cordic_hyperb_apfixed( ap_fixed<W,I> &x, 
							  ap_fixed<W,I> &y, 
							  ap_fixed<W,I> &z) {

	const int Nmax = W + 1;

	ap_fixed<W,I> tx, ty, tz;
	ap_fixed<W,I> x_s, y_s, z_s;
	ap_uint<1> d;

	for (int n=0; n<Nmax; n++){
	  
	  int k = hyperb_table_tau_128[n];

	  if(MODE == 1) { // Vectoring mode
		d = ~y[W-1];
	  } 
	  else {          // Rotation mode 
		d = z[W-1];
	  }

	  x_s = x >> k;
	  y_s = y >> k;
	  z_s = hyperb_table_arctanh_128[k-1];
	 
	  if(d){          // z[n-1] is negative
		tx = x - y_s;
		ty = y - x_s;
		tz = z + z_s;
	  }
	  else{ 
		tx = x + y_s;
		ty = y + x_s;
		tz = z - z_s;
	  }

	  x = tx; 
	  y = ty; 
	  z = tz;

	  // std::cout << "  x_s: " << x_s
	  // 			<< "  y_s: " << y_s
	  // 			<< "  z_s: " << z_s
	  // 			<< "  x : " << x
	  // 			<< "  y : " << y
	  // 			<< "  z : " << z << std::endl;

    }
  }

  // Cordic method with range reduce for sinh and cosh
  template <int W, int I>
  void sinh_cosh_range_redux_cordic( ap_fixed<W,I> in,
									 ap_fixed<W,I> &s_out,
									 ap_fixed<W,I> &c_out){

	ap_uint<1> sign0 = in[W-1];

	ap_ufixed<W,I> inabs;
	if(sign0 == 0)
	  inabs = in;
	else
	  inabs = -in;

	const int Fr = (W - I) + I;

	if(in==0){
	  s_out = 0;
	  c_out = 1;
	  return;
	}

	// inabs = k * ln(2) + r
	ap_uint<I+1> k;
	ap_ufixed<Fr,0> r;
	hyperb_range_redux(inabs, k, r);
	
	ap_fixed<Fr+2,2> x("1.20749706776307212887772");
	ap_fixed<Fr+2,2> y(0);
	ap_fixed<Fr+2,2> z(r);
	
	//	std::cout << x << "  " << y << "  " << z << std::endl;

	// compute x = sinh(r) and y = cosh(r)
	cordic_hyperb_apfixed<Fr+2,2,0>(x,y,z);

	//	std::cout << x << "  " << y << "  " << z << std::endl;

	ap_fixed<Fr+(I+2),I+2> er, e_r;
	er  = x + y; // e^r
	e_r = x - y; // e^{-r}

	//	std::cout << er << "  " << e_r << std::endl;

	er <<= (k-1);  // (e^k * 2^k) / 2
	e_r >>= (k+1); // (e^{-k} * 2^{-k})/2
	  
	//	std::cout << er << "  " << e_r << std::endl;

	ap_fixed<W+1,I+1> s_out_tmp = er - e_r;
	ap_fixed<W+1,I+1> c_out_tmp = er + e_r;

	//	std::cout << "out_tmp: "<< "  " << c_out_tmp << std::endl;

	// overflow sinh
	if(s_out_tmp[W-1]==1){
	  s_out_tmp[W-1] = 0;
	  for(int i=0;i<W-1;i++){
#pragma HLS UNROLL
		s_out_tmp[i] = 1;
	  }	  
	}

	// return sinh
	if(sign0 == 0){
	  s_out = s_out_tmp; 
	}
	else{
	  s_out = -s_out_tmp; 
	} 

	//	std::cout << c_out << "  " << c_out_tmp << std::endl;

	// overflow cosh and return
	if(c_out_tmp[W-1]==1){
	  c_out[W-1] = 0;
	  for(int i=0;i<W-1;i++){
#pragma HLS UNROLL
		c_out[i] = 1;
	  }	  
	}
	else{
	  c_out = c_out_tmp;
	}
  };

  // sinh(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> generic_sinh(ap_fixed<W,I> in) {
	ap_fixed<W,I> s_out, c_out;
	sinh_cosh_range_redux_cordic(in,s_out,c_out);
	return s_out;
  }
  
  // sinh(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,I> generic_sinh(ap_ufixed<W,I> in) {
	ap_fixed<W+1,I+1> fin = in;
	ap_fixed<W+1,I+1> fout = generic_sinh(fin);
	ap_ufixed<W,I> out = fout;
	return out;
  };  

  // sinh(ap_int)
  template<int I>
  ap_int<I> generic_sinh(ap_int<I> in) {
	ap_fixed<I,I> fin = in;
	ap_fixed<I,I> fout = generic_sinh(fin);
	ap_int<I> out = fout;
	return out;
  };  

  // sinh(ap_uint)
  template<int I>
  ap_uint<I> generic_sinh(ap_uint<I> in) {
	ap_fixed<I+1,I+1> fin = in;
	ap_fixed<I+1,I+1> fout = generic_sinh(fin);
	ap_uint<I> out = fout;
	return out;
  };  

  // cosh(ap_fixed)
  template<int W, int I>
  ap_fixed<W, I> generic_cosh(ap_fixed<W,I> in) {
	ap_fixed<W,I> s_out, c_out;
	sinh_cosh_range_redux_cordic(in,s_out,c_out);
	return c_out;
  }

  // cosh(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,I> generic_cosh(ap_ufixed<W,I> in) {
	ap_fixed<W+1,I+1> fin = in;
	ap_fixed<W+1,I+1> fout = generic_cosh(fin);
	ap_ufixed<W,I> out = fout;
	return out;
  };  

  // cosh(ap_int)
  template<int I>
  ap_int<I> generic_cosh(ap_int<I> in) {
	ap_fixed<I,I> fin = in;
	ap_fixed<I,I> fout = generic_cosh(fin);
	ap_int<I> out = fout;
	return out;
  };  

  // cosh(ap_uint)
  template<int I>
  ap_uint<I> generic_cosh(ap_uint<I> in) {
	ap_fixed<I+1,I+1> fin = in;
	ap_fixed<I+1,I+1> fout = generic_cosh(fin);
	ap_uint<I> out = fout;
	return out;
  };  


  // asin
  template<int W, int I>
  ap_fixed<W-I+3,3> generic_asin(ap_fixed<W,I> in){

    const int I0 = 3; 
    const int W0 = W - I + I0 + 6; 

    const ap_fixed<W,I> cst1("0x1");
    const ap_fixed<W,I> cst0("0x0");
    const ap_fixed<W,I> cst_1("-0x1");

    // input value should be between -1 and 1
#ifndef __SYNTHESIS__
    if(I>=2){
      if((in>cst1) or (in<cst_1)){
        printf("Error: input value %f (for asin) is not between -1 and 1.\n", in.to_double());
        assert(0);
      }
    }
#endif
    
    ap_fixed<W-I+3,3> out; // output

    // special case
    if(in==cst1){
      out = ap_fixed<W-I+3,3>("0x1.921FB54442D1846");
      return out;
    }
    else if(in==cst_1){
      out = ap_fixed<W-I+3,3>("-0x1.921FB54442D1846");
      return out;
    }
    else if(in==cst0){
      out = ap_fixed<W-I+3,3>("0x0");
      return out;
    }

    // sign
    ap_uint<1> sign0 = (in >= 0) ? 1: 0;

    // abs(in)
    ap_fixed<W,I> inabs;
    if(sign0)
      inabs = in;
    else
      inabs = -in;

    ap_fixed<W0,I0> x("0x1");
    ap_fixed<W0,I0> y("0x0");
    ap_fixed<W0,I0> z("0x0");
    ap_fixed<W0,I0> t = inabs;
	
    // double cordic algorithm
    doublecordic_apfixed<W0,I0,0,0>(x,y,z,t);

    // return 
    if(sign0)
      out = z;
    else
      out = -z;

    return out;
  }


  // acos
  template<int W, int I>
  ap_fixed<W-I+3,3> generic_acos(ap_fixed<W,I> in){

    const int I0 = 3; 
    const int W0 = W - I + I0;

    // input value should be between -1 and 1
#ifndef __SYNTHESIS__
    if(I>=2){
      const ap_fixed<W,I> cst1("0x1");
      const ap_fixed<W,I> cst_1("-0x1");
    
      if((in>cst1) or (in<cst_1)){
        printf("Error: input value %f (for acos) is not between -1 and 1.\n", in.to_double());
        assert(0);
      }
    }
#endif

    // acos = pi/2 - asin
    const ap_fixed<W0,I0> pi2("0x1.921FB54442D1846"); // pi/2
    ap_fixed<W0,I0> out = generic_asin(in);

    return pi2 - out;

	//     ap_fixed<W0,I0> x("0x1");
	//     ap_fixed<W0,I0> y("0x0");
	//     ap_fixed<W0,I0> z("0x0");
	//     ap_fixed<W0,I0> t = in;
	
	//     // double cordic algorithm
	//     doublecordic_apfixed<W0,I0,1,0>(x,y,z,t);

	//     // return 
	//     ap_fixed<W0,I0> out = z;
	//     return out;
  }
  
} // namespace

#endif
