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
 *     (c) Copyright 2008-2017 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#ifndef _X_HLSTMP_MATH__
#define _X_HLSTMP_MATH__

#include "hls_half.h"
#include "hls_fpo.h"
#include <stdint.h>

namespace hls {

  // signbit
  int signbit(double);
  int signbit(float);
  int signbit(half);
  int signbitf(float);
  int half_signbit(half);

  // nan
  double nan(const char*);
  float  nanf(const char*);
  half   half_nan(const char *);

  // isfinite
  int isfinite(double);
  int isfinite(float);
  int isfinite(half);
  int isfinitef(float);
  int half_isfinite(half);

  // __isfinite
  int __isfinite(double);
  int __isfinite(float);
  int __isfinite(half);
  int __isfinitef(float);

  // isinf
  int isinf(double);
  int isinf(float);
  int isinf(half);
  int isinff(float);
  int half_isinf(half);

  // __isinf
  int __isinf(double);
  int __isinf(float);
  int __isinf(half);
  int __isinff(float);

  // isnan
  int isnan(double);
  int isnan(float);
  int isnan(half);
  int isnanf(float);
  int half_isnan(half);

  // __isnan
  int __isnan(double);
  int __isnan(float);
  int __isnan(half);
  int __isnanf(float);

  // isnormal
  int isnormal(double);
  int isnormal(float);
  int isnormal(half);
  int isnormalf(float);
  int half_isnormal(half);

  // __isnormal
  int __isnormal(double);
  int __isnormal(float);
  int __isnormal(half);
  int __isnormalf(float);

  // fpclassify
  int fpclassify(double);
  int fpclassify(float);
  int fpclassify(half);
  int fpclassifyf(float);
  int half_fpclassify(half);

  // __fpclassify
  int __fpclassify(double);
  int __fpclassify(float);
  int __fpclassify(half);
  int __fpclassifyf(float);

  // copysign
  double copysign(double, double);
  float  copysign(float, float);
  half   copysign(half, half);
  float  copysignf(float, float);
  half   half_copysign(half, half);
  int8_t copysign(int8_t, int8_t);
  uint8_t copysign(uint8_t, uint8_t);
  int16_t copysign(int16_t, int16_t);
  uint16_t copysign(uint16_t, uint16_t);
  int32_t copysign(int32_t, int32_t);
  uint32_t copysign(uint32_t, uint32_t);

  // fabs
  double fabs(double);
  float  fabs(float);
  half   fabs(half);
  float  fabsf(float);
  half   half_fabs(half);
  int8_t fabs(int8_t);
  uint8_t fabs(uint8_t);
  int16_t fabs(int16_t);
  uint16_t fabs(uint16_t);
  int32_t fabs(int32_t);
  uint32_t fabs(uint32_t);

  // abs
  double abs(double);
  float  abs(float);
  half   abs(half);
  float  absf(float);
  half   half_abs(half);
  int8_t abs(int8_t);
  uint8_t abs(uint8_t);
  int16_t abs(int16_t);
  uint16_t abs(uint16_t);
  int32_t abs(int32_t);
  uint32_t abs(uint32_t);

  // fma
  double fma(double, double, double);
  float  fma(float, float, float);
  half   fma(half, half, half);
  float  fmaf(float, float, float);
  half   half_fma(half, half, half);

  // mad
  double mad(double, double, double);
  float  mad(float, float, float);
  half   mad(half, half, half);
  float  madf(float, float, float);
  half   half_mad(half, half, half);

  // isequal
  int isequal(double, double);
  int isequal(float, float);
  int isequal(half, half);
  int isequalf(float, float);
  int half_isequal(half, half);

  // isgreater
  int isgreater(double, double);
  int isgreater(float, float);
  int isgreater(half, half);
  int isgreaterf(float, float);
  int half_isgreater(half, half);

  // isgreaterequal
  int isgreaterequal(double, double);
  int isgreaterequal(float, float);
  int isgreaterequal(half, half);
  int isgreaterequalf(float, float);
  int half_isgreaterequal(half, half);

  // isless
  int isless(double, double);
  int isless(float, float);
  int isless(half, half);
  int islessf(float, float);
  int half_isless(half, half);

  // islessequal
  int islessequal(double, double);
  int islessequal(float, float);
  int islessequal(half, half);
  int islessequalf(float, float);
  int half_islessequal(half, half);

  // islessgreater
  int islessgreater(double, double);
  int islessgreater(float, float);
  int islessgreater(half, half);
  int islessgreaterf(float, float);
  int half_islessgreater(half, half);

  // isnotequal
  int isnotequal(double, double);
  int isnotequal(float, float);
  int isnotequal(half, half);
  int isnotequalf(float, float);
  int half_isnotequal(half, half);

  // fmax
  double fmax(double, double);
  float  fmax(float, float);
  half   fmax(half, half);
  float  fmaxf(float, float);
  half   half_fmax(half, half);

  // fmin
  double fmin(double, double);
  float  fmin(float, float);
  half   fmin(half, half);
  float  fminf(float, float);
  half   half_fmin(half, half);

  // max
  template <class T>
  T max(T x, T y){
	return (x > y) ? x : y;
  }

  // min
  template <class T>
  T min(T x, T y){
	return (x < y) ? x : y;
  }

  // fdim
  double fdim(double, double);
  float  fdim(float, float);
  half   fdim(half, half);
  float  fdimf(float, float);
  half   half_fdim(half, half);

  // maxmag
  double maxmag(double, double);
  float  maxmag(float, float);
  half   maxmag(half, half);
  float  maxmagf(float, float);
  half   half_maxmag(half, half);

  // minmag
  double minmag(double, double);
  float  minmag(float, float);
  half   minmag(half, half);
  float  minmagf(float, float);
  half   half_minmag(half, half);

  // frexp
  double frexp(double, int*);
  float  frexp(float, int*);
  half   frexp(half, int*);
  float  frexpf(float, int*);
  half   half_frexp(half, int*);

  // ldexp
  double ldexp(double, int);
  float  ldexp(float, int);
  half   ldexp(half, int);
  float  ldexpf(float, int);
  half   half_ldexp(half, int);

  // ilogb
  int ilogb(double);
  int ilogb(float);
  int ilogb(half);
  int ilogbf(float);
  int half_ilogb(half);

  // scalbn
  double scalbn(double, int);
  float  scalbn(float, int);
  half   scalbn(half, int);
  float  scalbnf(float, int);
  half   half_scalbn(half, int);

  // scalbln
  double scalbln(double, long int);
  float  scalbln(float, long int);
  half   scalbln(half, long int);
  float  scalblnf(float, long int);
  half   half_scalbln(half, long int);

  // ceil
  double ceil(double);
  float  ceil(float);
  half   ceil(half);
  float  ceilf(float);
  half   half_ceil(half);
  
  // floor
  double floor(double);
  float  floor(float);
  half   floor(half);
  float  floorf(float);
  half   half_floor(half);

  // trunc
  double trunc(double);
  float  trunc(float);
  half   trunc(half);
  float  truncf(float);
  half   half_trunc(half);

  // round
  double round(double);
  float  round(float);
  half   round(half);
  float  roundf(float);
  half   half_round(half);

  // rint
  double rint(double);
  float  rint(float);
  half   rint(half);
  float  rintf(float);
  half   half_rint(half);

  // lround
  long int lround(double);
  long int lround(float);
  long int lround(half);
  long int lroundf(float);
  long int half_lround(half);

  // llround
  long long int llround(double);
  long long int llround(float);
  long long int llround(half);
  long long int llroundf(float);
  long long int half_llround(half);

  // lrint
  long int lrint(double);
  long int lrint(float);
  long int lrint(half);
  long int lrintf(float);
  long int half_lrint(half);

  // llrint
  long long int llrint(double);
  long long int llrint(float);
  long long int llrint(half);
  long long int llrintf(float);
  long long int half_llrint(half);

  // cast_IEEE754
  template<class T>
  T cast_IEEE754(double, bool);
  template<class T>
  T cast_IEEE754(float, bool);
  template<class T>
  T cast_IEEE754(half, bool);

  template<class T>
  T cast_IEEE754(double);
  template<class T>
  T cast_IEEE754(float);
  template<class T>
  T cast_IEEE754(half);


  // cast_IEEE754<char>
  template<>
  char cast_IEEE754<char>(double, bool);
  template<>
  char cast_IEEE754<char>(float, bool);
  template<>
  char cast_IEEE754<char>(half, bool);
  template<>
  char cast_IEEE754<char>(double);
  template<>
  char cast_IEEE754<char>(float);
  template<>
  char cast_IEEE754<char>(half);

  // cast_IEEE754<int8_t>
  template<>
  int8_t cast_IEEE754<int8_t>(double, bool);
  template<>
  int8_t cast_IEEE754<int8_t>(float, bool);
  template<>
  int8_t cast_IEEE754<int8_t>(half, bool);
  template<>
  int8_t cast_IEEE754<int8_t>(double);
  template<>
  int8_t cast_IEEE754<int8_t>(float);
  template<>
  int8_t cast_IEEE754<int8_t>(half);

  // cast_IEEE754<uint8_t>
  template<>
  uint8_t cast_IEEE754<uint8_t>(double, bool);
  template<>
  uint8_t cast_IEEE754<uint8_t>(float, bool);
  template<>
  uint8_t cast_IEEE754<uint8_t>(half, bool);
  template<>
  uint8_t cast_IEEE754<uint8_t>(double);
  template<>
  uint8_t cast_IEEE754<uint8_t>(float);
  template<>
  uint8_t cast_IEEE754<uint8_t>(half);

  // cast_IEEE754<int16_t>
  template<>
  int16_t cast_IEEE754<int16_t>(double, bool);
  template<>
  int16_t cast_IEEE754<int16_t>(float, bool);
  template<>
  int16_t cast_IEEE754<int16_t>(half, bool);
  template<>
  int16_t cast_IEEE754<int16_t>(double);
  template<>
  int16_t cast_IEEE754<int16_t>(float);
  template<>
  int16_t cast_IEEE754<int16_t>(half);

  // cast_IEEE754<uint16_t>
  template<>
  uint16_t cast_IEEE754<uint16_t>(double, bool);
  template<>
  uint16_t cast_IEEE754<uint16_t>(float, bool);
  template<>
  uint16_t cast_IEEE754<uint16_t>(half, bool);
  template<>
  uint16_t cast_IEEE754<uint16_t>(double);
  template<>
  uint16_t cast_IEEE754<uint16_t>(float);
  template<>
  uint16_t cast_IEEE754<uint16_t>(half);

  // cast_IEEE754<int32_t>
  template<>
  int32_t cast_IEEE754<int32_t>(double, bool);
  template<>
  int32_t cast_IEEE754<int32_t>(float, bool);
  template<>
  int32_t cast_IEEE754<int32_t>(half, bool);
  template<>
  int32_t cast_IEEE754<int32_t>(double);
  template<>
  int32_t cast_IEEE754<int32_t>(float);
  template<>
  int32_t cast_IEEE754<int32_t>(half);

  // cast_IEEE754<uint32_t>
  template<>
  uint32_t cast_IEEE754<uint32_t>(double, bool);
  template<>
  uint32_t cast_IEEE754<uint32_t>(float, bool);
  template<>
  uint32_t cast_IEEE754<uint32_t>(half, bool);
  template<>
  uint32_t cast_IEEE754<uint32_t>(double);
  template<>
  uint32_t cast_IEEE754<uint32_t>(float);
  template<>
  uint32_t cast_IEEE754<uint32_t>(half);

  // cast_IEEE754<int64_t>
  template<>
  int64_t cast_IEEE754<int64_t>(double, bool);
  template<>
  int64_t cast_IEEE754<int64_t>(float, bool);
  template<>
  int64_t cast_IEEE754<int64_t>(half, bool);
  template<>
  int64_t cast_IEEE754<int64_t>(double);
  template<>
  int64_t cast_IEEE754<int64_t>(float);
  template<>
  int64_t cast_IEEE754<int64_t>(half);

  // cast_IEEE754<uint64_t>
  template<>
  uint64_t cast_IEEE754<uint64_t>(double, bool);
  template<>
  uint64_t cast_IEEE754<uint64_t>(float, bool);
  template<>
  uint64_t cast_IEEE754<uint64_t>(half, bool);
  template<>
  uint64_t cast_IEEE754<uint64_t>(double);
  template<>
  uint64_t cast_IEEE754<uint64_t>(float);
  template<>
  uint64_t cast_IEEE754<uint64_t>(half);


  // nearbyint
  double nearbyint(double);
  float  nearbyint(float);
  half   nearbyint(half);
  float  nearbyintf(float);
  half   half_nearbyint(half);

  // modf
  double modf(double, double*);
  float  modf(float, float*);
  half   modf(half, half*);
  float  modff(float, float*);
  half   half_modf(half, half*);

  // fract
  double fract(double, double*);
  float  fract(float, float*);
  half   fract(half, half*);
  float  fractf(float, float*);
  half   half_fract(half, half*);

  // isordered
  int isordered(double, double);
  int isordered(float, float);
  int isordered(half, half);
  int isorderedf(float, float);
  int half_isordered(half, half);

  // isunordered
  int isunordered(double, double);
  int isunordered(float, float);
  int isunordered(half, half);
  int isunorderedf(float, float);
  int half_isunordered(half, half);

  // bitselect
  double   bitselect(double, double, double);
  float    bitselect(float, float, float);
  int      bitselect(int, int, int);
  uint32_t bitselect(uint32_t, uint32_t, uint32_t);
  half     bitselect(half, half, half);
  float    bitselectf(float, float, float);
  half     half_bitselect(half, half, half);

  // sin cos implemented with hotbm method
  namespace hotbm {
	double sin(double);
	float  sin(float);
	half   sin(half);
	float  sinf(float);
	half   half_sin(half);

	double cos(double);
	float  cos(float);
	half   cos(half);
	float  cosf(float);
	half   half_cos(half);

	void sincos(double, double*, double*);
	void sincos(float, float*, float*);
	void sincos(half, half*, half*);
	void sincosf(float, float*, float*);
	void half_sincos(half, half*, half*);
  }

  // sin cos implemented with cordic method
  namespace cordic {
	double sin(double);
	float  sin(float);
	half   sin(half);
	float  sinf(float);
	half   half_sin(half);

	double cos(double);
	float  cos(float);
	half   cos(half);
	float  cosf(float);
	half   half_cos(half);

	void sincos(double, double*, double*);
	void sincos(float, float*, float*);
	void sincos(half, half*, half*);
	void sincosf(float, float*, float*);
	void half_sincos(half, half*, half*);
  }
  
  // sin cos implemented with hotbm method
  double sin(double);
  float  sin(float);
  half   sin(half);
  float  sinf(float);
  half   half_sin(half);

  double cos(double);
  float  cos(float);
  half   cos(half);
  float  cosf(float);
  half   half_cos(half);

  // sincos
  void sincos(double, double*, double*);
  void sincos(float, float*, float*);
  void sincos(half, half*, half*);
  void sincosf(float, float*, float*);
  void half_sincos(half, half*, half*);

  // sinpi
  double sinpi(double);
  float  sinpi(float);
  half   sinpi(half);
  float  sinpif(float);
  half   half_sinpi(half);

  // cospi
  double cospi(double);
  float  cospi(float);
  half   cospi(half);
  float  cospif(float);
  half   half_cospi(half);

  // atan
  double atan(double);
  float  atan(float);
  half   atan(half);
  float  atanf(float);
  half   half_atan(half);

  // atan2
  double atan2(double, double);
  float  atan2(float, float);
  half   atan2(half, half);
  float  atan2f(float, float);
  half   half_atan2(half, half);

  // tan
  double tan(double);
  float  tan(float);
  half   tan(half);
  float  tanf(float);
  half   half_tan(half);

  // sinh
  double sinh(double);
  float  sinh(float);
  half   sinh(half);
  float  sinhf(float);
  half   half_sinh(half);

  // cosh
  double cosh(double);
  float  cosh(float);
  half   cosh(half);
  float  coshf(float);
  half   half_cosh(half);

  // exp
  double exp(double);
  float  exp(float);
  half   exp(half);
  float  expf(float);
  half   half_exp(half);
  int8_t exp(int8_t);
  uint8_t exp(uint8_t);
  int16_t exp(int16_t);
  uint16_t exp(uint16_t);
  int32_t exp(int32_t);
  uint32_t exp(uint32_t);

  // exp2
  double exp2(double);
  float  exp2(float);
  half   exp2(half);
  float  exp2f(float);
  half   half_exp2(half);

  // exp10
  double exp10(double);
  float  exp10(float);
  half   exp10(half);
  float  exp10f(float);
  half   half_exp10(half);

  // expm1
  double expm1(double);
  float  expm1(float);
  half   expm1(half);
  float  expm1f(float);
  half   half_expm1(half);

  // log
  double log(double);
  float  log(float);
  half   log(half);
  float  logf(float);
  half   half_log(half);
  int8_t log(int8_t);
  uint8_t log(uint8_t);
  int16_t log(int16_t);
  uint16_t log(uint16_t);
  int32_t log(int32_t);
  uint32_t log(uint32_t);

  // log2
  double log2(double);
  float  log2(float);
  half   log2(half);
  float  log2f(float);
  half   half_log2(half);

  // log10
  double log10(double);
  float  log10(float);
  half   log10(half);
  float  log10f(float);
  half   half_log10(half);
  int8_t log10(int8_t);
  uint8_t log10(uint8_t);
  int16_t log10(int16_t);
  uint16_t log10(uint16_t);
  int32_t log10(int32_t);
  uint32_t log10(uint32_t);

  // logb
  double logb(double);
  float  logb(float);
  half   logb(half);
  float  logbf(float);
  half   half_logb(half);

  // log1p
  double log1p(double);
  float  log1p(float);
  half   log1p(half);
  float  log1pf(float);
  half   half_log1p(half);

  // pow
  half   pow(half, half);
  float  pow(float, float);
  double pow(double, double);
  float  powf(float, float);
  half   half_pow(half, half);
  int8_t pow(int8_t, int8_t);
  uint8_t pow(uint8_t, uint8_t);
  int16_t pow(int16_t, int16_t);
  uint16_t pow(uint16_t, uint16_t);
  int32_t pow(int32_t, int32_t);
  uint32_t pow(uint32_t, uint32_t);

  // powr
  half   powr(half, half);
  float  powr(float, float);
  double powr(double, double);
  float  powrf(float, float);
  half   half_powr(half, half);

  // pown
  half   pown(half, int);
  float  pown(float, int);
  double pown(double, int);
  float  pownf(float, int);
  half   half_pown(half, int);

  // rootn
  half   rootn(half, int);
  float  rootn(float, int);
  double rootn(double, int);
  float  rootnf(float, int);
  half   half_rootn(half, int);

  // sqrt
  double sqrt(double);
  float  sqrt(float);
  half   sqrt(half);
  float  sqrtf(float);
  half   half_sqrt(half);
  int8_t sqrt(int8_t);
  uint8_t sqrt(uint8_t);
  int16_t sqrt(int16_t);
  uint16_t sqrt(uint16_t);
  int32_t sqrt(int32_t);
  uint32_t sqrt(uint32_t);

  // template<int W, int I>
  // ap_fixed<W,I> sqrt(ap_fixed<W,I> x){
  // 	return sqrt_fixed(x);
  // };
  // template<int W, int I>
  // ap_ufixed<W,I> sqrt(ap_ufixed<W,I> x){
  //   return sqrt_fixed(x);
  // }
  // template<int I>
  // ap_int<I> sqrt(ap_int<I> x){
  //   return sqrt_fixed(x);
  // }
  // template<int I>
  // ap_uint<I> sqrt(ap_uint<I> x){
  //   return sqrt_fixed(x);
  // }

  // recip
  double recip(double);
  float  recip(float);
  half   recip(half);
  float  recipf(float);
  half   half_recip(half);
  int8_t recip(int8_t);
  uint8_t recip(uint8_t);
  int16_t recip(int16_t);
  uint16_t recip(uint16_t);
  int32_t recip(int32_t);
  uint32_t recip(uint32_t);

  // rsqrt
  double rsqrt(double);
  float  rsqrt(float);
  half   rsqrt(half);
  float  rsqrtf(float);
  half   half_rsqrt(half);
  int8_t rsqrt(int8_t);
  uint8_t rsqrt(uint8_t);
  int16_t rsqrt(int16_t);
  uint16_t rsqrt(uint16_t);
  int32_t rsqrt(int32_t);
  uint32_t rsqrt(uint32_t);

  // cbrt
  double cbrt(double);
  float  cbrt(float);
  half   cbrt(half);
  float  cbrtf(float);
  half   half_cbrt(half);

  // hypot
  double hypot(double, double);
  float  hypot(float, float);
  half   hypot(half, half);
  float  hypotf(float, float);
  half   half_hypot(half, half);

  // erf
  double erf(double);
  float  erf(float);
  half   erf(half);
  float  erff(float);
  half   half_erf(half);

  // erfc
  double erfc(double);
  float  erfc(float);
  half   erfc(half);
  float  erfcf(float);
  half   half_erfc(half);

  // lgamma
  double lgamma(double);
  float  lgamma(float);
  half   lgamma(half);
  float  lgammaf(float);
  half   half_lgamma(half);

  // lgamma_r
  double lgamma_r(double, int*);
  float  lgamma_r(float, int*);
  half   lgamma_r(half, int*);
  float  lgamma_rf(float, int*);
  half   half_lgamma_r(half, int*);

  // cbrt
  double tgamma(double);
  float  tgamma(float);
  half   tgamma(half);
  float  tgammaf(float);
  half   half_tgamma(half);

  // fmod
  double fmod(double, double);
  float  fmod(float, float);
  half   fmod(half, half);
  float  fmodf(float, float);
  half   half_fmod(half, half);

  // remainder
  double remainder(double, double);
  float  remainder(float, float);
  half   remainder(half, half);
  float  remainderf(float, float);
  half   half_remainder(half, half);

  // remquo
  double remquo(double, double, int*);
  float  remquo(float, float, int*);
  half   remquo(half, half, int*);
  float  remquof(float, float, int*);
  half   half_remquo(half, half, int*);

  // divide
  double divide(double, double);
  float  divide(float, float);
  half   divide(half, half);
  float  dividef(float, float);
  half   half_divide(half, half);

  // nextafter
  double nextafter(double, double);
  float  nextafter(float, float);
  half   nextafter(half, half);
  float  nextafterf(float, float);
  half   half_nextafter(half, half);

  // tanh
  double tanh(double);
  float  tanh(float);
  half   tanh(half);
  float  tanhf(float);
  half   half_tanh(half);

  // atanh
  double atanh(double);
  float  atanh(float);
  half   atanh(half);
  float  atanhf(float);
  half   half_atanh(half);

  // asinh
  double asinh(double);
  float  asinh(float);
  half   asinh(half);
  float  asinhf(float);
  half   half_asinh(half);

  // acosh
  double acosh(double);
  float  acosh(float);
  half   acosh(half);
  float  acoshf(float);
  half   half_acosh(half);

  // asin
  double asin(double);
  float  asin(float);
  half   asin(half);
  float  asinf(float);
  half   half_asin(half);

  // acos
  double acos(double);
  float  acos(float);
  half   acos(half);
  float  acosf(float);
  half   half_acos(half);
}

#include "hls_hotbm_apfixed.h"
#include "hls_cordic_apfixed.h"
#include "hls_exp_apfixed.h"
#include "hls_log_apfixed.h"
#include "hls_sqrt_apfixed.h"
#include "hls_round_copysign_apfixed.h"
#include "hls_pow_apfixed.h"
namespace hls {

  // // sin(ap_fixed)
  // template<int W, int I>
  // ap_fixed<W, 2> sin(ap_fixed<W,I> x){
  // 	return hotbm_apfixed::sinf_or_cosf<W,I>(x, 0, 0);
  // }
  // template<int W, int I>
  // ap_fixed<W, 2> sinf(ap_fixed<W,I> x){
  // 	return hotbm_apfixed::sinf_or_cosf<W,I>(x, 0, 0);
  // }

  // // cos(ap_fixed)
  // template<int W, int I>
  // ap_fixed<W, 2> cos(ap_fixed<W,I> x){
  // 	return hotbm_apfixed::sinf_or_cosf<W,I>(x, 1, 0);
  // }
  // template<int W, int I>
  // ap_fixed<W, 2> cosf(ap_fixed<W,I> x){
  // 	return hotbm_apfixed::sinf_or_cosf<W,I>(x, 1, 0);
  // }

  // sinpi(ap_fixed), cospi(ap_fixed)
  template<int W, int I>
  ap_fixed<W, 2> sinpi(ap_fixed<W,I> x){
    return hotbm_apfixed::sinf_or_cosf<W,I>(x, 0, 1);
  }
  template<int W, int I>
  ap_fixed<W, 2> sinpif(ap_fixed<W,I> x){
    return hotbm_apfixed::sinf_or_cosf<W,I>(x, 0, 1);
  }
  template<int W, int I>
  ap_fixed<W, 2> cospi(ap_fixed<W,I> x){
    return hotbm_apfixed::sinf_or_cosf<W,I>(x, 1, 1);
  }
  template<int W, int I>
  ap_fixed<W, 2> cospif(ap_fixed<W,I> x){
    return hotbm_apfixed::sinf_or_cosf<W,I>(x, 1, 1);
  }

  // exp(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> exp(ap_fixed<W,I> x){
    return exp_reduce::exp(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> exp(ap_ufixed<W,I> x){
    return exp_reduce::exp(x);
  }
  template<int I>
  ap_int<I> exp(ap_int<I> x){
    return exp_reduce::exp(x);
  }
  template<int I>
  ap_uint<I> exp(ap_uint<I> x){
    return exp_reduce::exp(x);
  }

  // log(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> log(ap_fixed<W,I> x){
    return log_apfixed_reduce::log(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> log(ap_ufixed<W,I> x){
    return log_apfixed_reduce::log(x);
  }
  template<int I>
  ap_int<I> log(ap_int<I> x){
    return log_apfixed_reduce::log(x);
  }
  template<int I>
  ap_uint<I> log(ap_uint<I> x){
    return log_apfixed_reduce::log(x);
  }

  // log10(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> log10(ap_fixed<W,I> x){
    return log_apfixed_reduce::log10(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> log10(ap_ufixed<W,I> x){
    return log_apfixed_reduce::log10(x);
  }
  template<int I>
  ap_int<I> log10(ap_int<I> x){
    return log_apfixed_reduce::log10(x);
  }
  template<int I>
  ap_uint<I> log10(ap_uint<I> x){
	return log_apfixed_reduce::log10(x);
  }


  // sqrt(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> sqrt(ap_fixed<W,I> x){
    return sqrt_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> sqrt(ap_ufixed<W,I> x){
    return sqrt_fixed(x);
  }
  template<int I>
  ap_int<I> sqrt(ap_int<I> x){
    return sqrt_fixed(x);
  }
  template<int I>
  ap_uint<I> sqrt(ap_uint<I> x){
    return sqrt_fixed(x);
  }
  
  // rsqrt(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> rsqrt(ap_fixed<W,I> x){
    return rsqrt_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> rsqrt(ap_ufixed<W,I> x){
    return rsqrt_fixed(x);
  }
  template<int I>
  ap_int<I> rsqrt(ap_int<I> x){
    return rsqrt_fixed(x);
  }
  template<int I>
  ap_uint<I> rsqrt(ap_uint<I> x){
    return rsqrt_fixed(x);
  }
  
  // recip(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> recip(ap_fixed<W,I> x){
    return recip_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> recip(ap_ufixed<W,I> x){
    return recip_fixed(x);
  }
  template<int I>
  ap_int<I> recip(ap_int<I> x){
    return recip_fixed(x);
  }
  template<int I>
  ap_uint<I> recip(ap_uint<I> x){
    return recip_fixed(x);
  }

  // ceil(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> ceil(ap_fixed<W,I> x){
    return ceil_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> ceil(ap_ufixed<W,I> x){
    return ceil_fixed(x);
  }
  template<int I>
  ap_int<I> ceil(ap_int<I> x){
    return ceil_fixed(x);
  }
  template<int I>
  ap_uint<I> ceil(ap_uint<I> x){
    return ceil_fixed(x);
  }

  // floor(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> floor(ap_fixed<W,I> x){
    return floor_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> floor(ap_ufixed<W,I> x){
    return floor_fixed(x);
  }
  template<int I>
  ap_int<I> floor(ap_int<I> x){
    return floor_fixed(x);
  }
  template<int I>
  ap_uint<I> floor(ap_uint<I> x){
    return floor_fixed(x);
  }

  template<int W, int I>
  ap_fixed<W,I> trunc(ap_fixed<W,I> x){
    return trunc_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> trunc(ap_ufixed<W,I> x){
    return trunc_fixed(x);
  }
  template<int I>
  ap_int<I> trunc(ap_int<I> x){
    return trunc_fixed(x);
  }
  template<int I>
  ap_uint<I> trunc(ap_uint<I> x){
    return trunc_fixed(x);
  }

  // copysign(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> copysign(ap_fixed<W,I> x, ap_fixed<W,I> y){
    return copysign_fixed(x,y);
  }
  template<int W, int I>
  ap_ufixed<W,I> copysign(ap_ufixed<W,I> x, ap_ufixed<W,I> y){
    return copysign_fixed(x,y);
  }
  template<int I>
  ap_int<I> copysign(ap_int<I> x, ap_int<I> y){
    return copysign_fixed(x,y);
  }
  template<int I>
  ap_uint<I> copysign(ap_uint<I> x, ap_uint<I> y){
    return copysign_fixed(x,y);
  }

  // fabs(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> fabs(ap_fixed<W,I> x){
    return fabs_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> fabs(ap_ufixed<W,I> x){
    return fabs_fixed(x);
  }
  template<int I>
  ap_int<I> fabs(ap_int<I> x){
    return fabs_fixed(x);
  }
  template<int I>
  ap_uint<I> fabs(ap_uint<I> x){
    return fabs_fixed(x);
  }
  // abs(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> abs(ap_fixed<W,I> x){
    return fabs_fixed(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> abs(ap_ufixed<W,I> x){
    return fabs_fixed(x);
  }
  template<int I>
  ap_int<I> abs(ap_int<I> x){
    return fabs_fixed(x);
  }
  template<int I>
  ap_uint<I> abs(ap_uint<I> x){
    return fabs_fixed(x);
  }

  // sinh(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> sinh(ap_fixed<W,I> x){
	return cordic_apfixed::generic_sinh(x);
  }

  // sinh(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,I> sinh(ap_ufixed<W,I> x){
	return cordic_apfixed::generic_sinh(x);
  }

  // sinh(ap_int)
  template<int I>
  ap_int<I> sinh(ap_int<I> x){
	return cordic_apfixed::generic_sinh(x);
  }

  // sinh(ap_uint)
  template<int I>
  ap_uint<I> sinh(ap_uint<I> x){
	return cordic_apfixed::generic_sinh(x);
  }

  // cosh(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> cosh(ap_fixed<W,I> x){
	return cordic_apfixed::generic_cosh(x);
  }

  // cosh(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,I> cosh(ap_ufixed<W,I> x){
	return cordic_apfixed::generic_cosh(x);
  }

  // cosh(ap_int)
  template<int I>
  ap_int<I> cosh(ap_int<I> x){
	return cordic_apfixed::generic_cosh(x);
  }

  // cosh(ap_uint)
  template<int I>
  ap_uint<I> cosh(ap_uint<I> x){
	return cordic_apfixed::generic_cosh(x);
  }

  int8_t   sinh(int8_t);
  uint8_t  sinh(uint8_t);
  int16_t  sinh(int16_t);
  uint16_t sinh(uint16_t);
  int32_t  sinh(int32_t);
  uint32_t sinh(uint32_t);

  int8_t   cosh(int8_t);
  uint8_t  cosh(uint8_t);
  int16_t  cosh(int16_t);
  uint16_t cosh(uint16_t);
  int32_t  cosh(int32_t);
  uint32_t cosh(uint32_t);


  // asin(ap_fixed)
  template<int W, int I>
  ap_fixed<W-I+3,3> asin(ap_fixed<W,I> x){
	return cordic_apfixed::generic_asin(x);
  } 

  // acos(ap_fixed)
  template<int W, int I>
  ap_fixed<W-I+3,3> acos(ap_fixed<W,I> x){
	return cordic_apfixed::generic_acos(x);
  } 

  // atan(ap_fixed)
  template<int W, int I>
  ap_fixed<W,2> atan(ap_fixed<W,I> x){
	return cordic_apfixed::generic_atan(x);
  } 

  // atan(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,2> atan(ap_ufixed<W,I> x){
	return cordic_apfixed::generic_atan(x);
  } 

  // atan(ap_int)
  template<int I>
  ap_int<2> atan(ap_int<I> x){
  	return cordic_apfixed::generic_atan(x);
  } 

  // atan(ap_uint)
  template<int I>
  ap_uint<1> atan(ap_uint<I> x){
  	return cordic_apfixed::generic_atan(x);
  } 

  int8_t   asin(int8_t);
  uint8_t  asin(uint8_t);
  int16_t  asin(int16_t);
  uint16_t asin(uint16_t);
  int32_t  asin(int32_t);
  uint32_t asin(uint32_t);

  int8_t   acos(int8_t);
  uint8_t  acos(uint8_t);
  int16_t  acos(int16_t);
  uint16_t acos(uint16_t);
  int32_t  acos(int32_t);
  uint32_t acos(uint32_t);

  int8_t   atan(int8_t);
  uint8_t  atan(uint8_t);
  int16_t  atan(int16_t);
  uint16_t atan(uint16_t);
  int32_t  atan(int32_t);
  uint32_t atan(uint32_t);

  // atan2(ap_fixed)
  template<int W, int I>
  ap_fixed<W,3> atan2(ap_fixed<W,I> x, ap_fixed<W,I> y){
	return cordic_apfixed::generic_atan2(x,y);
  } 

  // atan2(ap_ufixed)
  template<int W, int I>
  ap_ufixed<W,3> atan2(ap_ufixed<W,I> x, ap_ufixed<W,I> y){
	return cordic_apfixed::generic_atan2(x,y);
  } 

  // atan2(ap_int)
  template<int I>
  ap_int<3> atan2(ap_int<I> x, ap_int<I> y){
  	return cordic_apfixed::generic_atan2(x,y);
  } 

  // atan2(ap_uint)
  template<int I>
  ap_uint<2> atan2(ap_uint<I> x, ap_uint<I> y){
  	return cordic_apfixed::generic_atan2(x,y);
  } 

  int8_t   atan2(int8_t, int8_t);
  uint8_t  atan2(uint8_t, uint8_t);
  int16_t  atan2(int16_t, int16_t);
  uint16_t atan2(uint16_t, uint16_t);
  int32_t  atan2(int32_t, int32_t);
  uint32_t atan2(uint32_t, uint32_t);

  template<int W, int I>
  void sincos(ap_fixed<W,I> in, 
  			  ap_fixed<W-I+2,2>* outsin, ap_fixed<W-I+2,2>* outcos){
  	cordic_apfixed::generic_sincos(in, *outsin, *outcos);
  }

  template<int W, int I>
  ap_fixed<W-I+2,2> sin(ap_fixed<W,I> x){
        ap_fixed<W-I+2,2> outsin;
        ap_fixed<W-I+2,2> outcos;
        cordic_apfixed::generic_sincos(x, outsin, outcos);
        return outsin;
  }

  template<int W, int I>
  ap_fixed<W-I+2,2> cos(ap_fixed<W,I> x){
        ap_fixed<W-I+2,2> outsin;
        ap_fixed<W-I+2,2> outcos;
        cordic_apfixed::generic_sincos(x, outsin, outcos);
        return outcos;
  }

  template<int W, int I>
  ap_fixed<W-I+2,2> sinf(ap_fixed<W,I> x){
        ap_fixed<W-I+2,2> outsin;
        ap_fixed<W-I+2,2> outcos;
        cordic_apfixed::generic_sincos(x, outsin, outcos);
        return outsin;
  }

  template<int W, int I>
  ap_fixed<W-I+2,2> cosf(ap_fixed<W,I> x){
	ap_fixed<W-I+2,2> outsin;
	ap_fixed<W-I+2,2> outcos;
        cordic_apfixed::generic_sincos(x, outsin, outcos);
        return outcos;
  }

  // sin cos(ap_int)
  template<int I>
  ap_int<I> sin(ap_int<I> in) {
	return ap_int<I>(0);
  };  
  template<int I>
  ap_int<I> cos(ap_int<I> in) {
	return ap_int<I>(0);
  };  

  // sin cos(ap_uint)
  template<int I>
  ap_uint<I> sin(ap_uint<I> in) {
	return ap_uint<I>(0);
  };  
  template<int I>
  ap_uint<I> cos(ap_uint<I> in) {
	return ap_uint<I>(0);
  };  

  void sincos(int8_t, int8_t*, int8_t*);
  void sincos(uint8_t, uint8_t*, uint8_t*);
  void sincos(int16_t, int16_t*, int16_t*);
  void sincos(uint16_t, uint16_t*, uint16_t*);
  void sincos(int32_t, int32_t*, int32_t*);
  void sincos(uint32_t, uint32_t*, uint32_t*);

  int8_t   sin(int8_t);
  uint8_t  sin(uint8_t);
  int16_t  sin(int16_t);
  uint16_t sin(uint16_t);
  int32_t  sin(int32_t);
  uint32_t sin(uint32_t);

  int8_t   cos(int8_t);
  uint8_t  cos(uint8_t);
  int16_t  cos(int16_t);
  uint16_t cos(uint16_t);
  int32_t  cos(int32_t);
  uint32_t cos(uint32_t);

  // tan
  template<int W, int I>
  ap_fixed<W,I> tan(ap_fixed<W,I> x){
	return cordic_apfixed::generic_tan(x);
  }
  template<int W, int I>
  ap_ufixed<W,I> tan(ap_ufixed<W,I> x){
	return cordic_apfixed::generic_tan(x);
  }
  template<int I>
  ap_int<I> tan(ap_int<I> x){
  	return cordic_apfixed::generic_tan(x);
  } 
  template<int I>
  ap_uint<I> tan(ap_uint<I> x){
  	return cordic_apfixed::generic_tan(x);
  } 

  int8_t   tan(int8_t);
  uint8_t  tan(uint8_t);
  int16_t  tan(int16_t);
  uint16_t tan(uint16_t);
  int32_t  tan(int32_t);
  uint32_t tan(uint32_t);
  //pow(ap_fixed)
  template<int W, int I>
  ap_fixed<W,I> pow(ap_fixed<W,I> x, ap_fixed<W,I> y) {
    return pow_apfixed_reduce::pow(x,y);
  }

  template<int W, int I>
  ap_ufixed<W,I> pow(ap_ufixed<W,I> x, ap_ufixed<W,I> y){
    return pow_apfixed_reduce::pow(x,y);
  }
  template<int I>
  ap_int<I> pow(ap_int<I> x, ap_int<I> y){
    return pow_apfixed_reduce::pow(x,y);
  }
  template<int I>
  ap_uint<I> pow(ap_uint<I> x,ap_uint<I> y){
    return pow_apfixed_reduce::pow(x,y);
  }
};
#endif
