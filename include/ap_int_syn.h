// FIXME: Filling in syn/ap_int.h
// -*- c++ -*-
/*
#-  (c) Copyright 2011-2018 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************

 *
 */
#ifndef __AESL_AP_SYN_H__
#define __AESL_AP_SYN_H__

#include "hls_half.h"

// FIXME: Filling in ap_int_syn.h 
/* -*- c++ -*-*/
/*
#-  (c) Copyright 2011-2018 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************

 *
 */

#ifndef __AESL_AP_INT_SYN_H__
#define __AESL_AP_INT_SYN_H__



#ifdef __STRICT_ANSI__
#define typeof __typeof__
#endif



#if !defined(AP_AUTOCC)
#  include <iostream>
#  include <math.h>
#  include <string>
#endif

#include <limits.h>
#include "hls_half.h"

#ifdef HECTOR
#include "Hector.h"
#endif

/* for safety*/
#if (defined(_AP_N)|| defined(_AP_C))
#error One or more of the following is defined: _AP_N, _AP_C. Definition conflicts with their usage as template parameters. 
#endif

/* for safety*/
#if (defined(_AP_W) || defined(_AP_I) || defined(_AP_S) || defined(_AP_Q) || defined(_AP_O) || defined(_AP_W2) || defined(_AP_I2) || defined(_AP_S2) || defined(_AP_Q2) || defined(_AP_O2) || defined(_AP_N) || defined(_AP_N2))
#error One or more of the following is defined: _AP_W, _AP_I, _AP_S, _AP_Q, _AP_O,  _AP_N, _AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2. Definition conflicts with their usage as template parameters. 
#endif

/*for safety*/
#if (defined(_AP_W3) || defined(_AP_S3) || defined(_AP_W4) || defined(_AP_S4))
#error One or more of the following is defined: _AP_W3, _AP_S3, _AP_W4,_AP_S4. Definition conflicts with their usage as template parameters. 
#endif

#if (defined(_AP_W1) || defined(_AP_S1) || defined(_AP_T) || defined(_AP_T1) || defined(_AP_T2) || defined(_AP_T3) || defined(_AP_T4))
#error One or more of the following is defined: _AP_W1, _AP_S1, _AP_T,  _AP_T1, _AP_T2, _AP_T3, _AP_T4. Definition conflicts with their usage as template parameters. 
#endif

#define AP_DEBUG(x) 

#define INLINE inline __attribute__((always_inline))
#define AP_WEAK //__attribute__((weak))

#if defined(_MSC_VER)
#  if _MSC_VER < 1400 && !defined(for)
#     define for if(0);else for
#  endif
typedef unsigned __int64 ap_ulong;
typedef signed   __int64 ap_slong;
#else
typedef unsigned long long ap_ulong;
typedef signed   long long ap_slong;
#endif
    
#define AP_MAX(a,b) ((a) > (b) ? (a) : (b))
#define AP_MIN(a,b) ((a) < (b) ? (a) : (b))

#ifndef AP_ASSERT
#ifndef AESL_SYN
#   include <assert.h>
#   define AP_ASSERT(cond, msg) assert(cond && msg)
#else
#   define AP_ASSERT(cond, msg) 
#endif
#endif



/*support SC mode*/
#define AP_TRN SC_TRN
#define AP_RND SC_RND
#define AP_TRN_ZERO SC_TRN_ZERO
#define AP_RND_ZERO SC_RND_ZERO
#define AP_RND_INF SC_RND_INF
#define AP_RND_MIN_INF SC_RND_MIN_INF
#define AP_RND_CONV SC_RND_CONV
#define AP_WRAP SC_WRAP
#define AP_SAT SC_SAT
#define AP_SAT_ZERO SC_SAT_ZERO
#define AP_SAT_SYM SC_SAT_SYM
#define AP_WRAP_SM SC_WRAP_SM 
#define AP_BIN SC_BIN
#define AP_OCT SC_OCT
#define AP_DEC SC_DEC
#define AP_HEX SC_HEX

extern "C" void _ssdm_string2bits(...);
//#ifdef C99STRING
#ifndef NON_C99STRING
#define _AP_C99 true
#else
#define _AP_C99 false
#endif

#if ((__clang_major__ == 3) && (__clang_minor__ == 1))

/* Basic AP data types.
   ----------------------------------------------------------------
*/
template<int _AP_N, bool _AP_S> struct ssdm_int;

#ifndef HECTOR

#ifndef AP_INT_MAX_W
#define AP_INT_MAX_W 1024
#endif
#define BIT_WIDTH_UPPER_LIMIT (1 << 15)
#if AP_INT_MAX_W > BIT_WIDTH_UPPER_LIMIT
#error "Bitwidth exceeds 32768 (1 << 15), the maximum allowed value"
#endif


#define MAX_MODE(BITS) ((BITS + 1023) / 1024)
#define AP_INT_BASE(_AP_N, mode)					\
  template<> struct ssdm_int<_AP_N + 1024 * mode,true> {		\
    int V  __attribute__ ((bitwidth(_AP_N + 1024 * mode)));		\
    INLINE ssdm_int<_AP_N + 1024 * mode ,true>() { };			\
  };									\
  template<> struct ssdm_int<_AP_N + 1024 * mode, false> {		\
    unsigned int V  __attribute__ ((bitwidth(_AP_N + 1024 * mode)));	\
    INLINE ssdm_int<_AP_N + 1024 * mode , false>() { };			\
  }; 

#if MAX_MODE(AP_INT_MAX_W) >= 1
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 0) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 2
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 1) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 3
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 2) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 4
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 3) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 5
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 4) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 6
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 5) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 7
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 6) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 8
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 7) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 9
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 8) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 10
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 9) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 11
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 10) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 12
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 11) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 13
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 12) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 14
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 13) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 15
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 14) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 16
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 15) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 17
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 16) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 18
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 17) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 19
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 18) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 20
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 19) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 21
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 20) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 22
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 21) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 23
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 22) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 24
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 23) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 25
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 24) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 26
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 25) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 27
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 26) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 28
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 27) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 29
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 28) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 30
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 29) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 31
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 30) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if MAX_MODE(AP_INT_MAX_W) >= 32
#define HANDLE_BAT(TYPE, _AP_N) AP_INT_BASE(_AP_N, 31) 
#define APINT_DEFINE_INT64
#include "etc/autopilot_dt.def"
#undef APINT_DEFINE_INT64
#undef HANDLE_BAT
#endif

#if 0
#if MAX_MODE(AP_INT_MAX_W) == 1 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (1024)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 2 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (2048)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 3 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (3072)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 4 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (4096)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 5 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (5120)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 6 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (6144)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 7 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (7168)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 8 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (8192)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 9 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (9196)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 10 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (10240)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 11 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (11264)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 12 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (12288)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 13 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (13312)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 14 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (14336)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 15 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (15360)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 16 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (16384)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 17 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (17408)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 18 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (18432)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 19 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (19456)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 20 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (20480)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 21 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (21504)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 22 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (22528)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 23 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (23552)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 24 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (24576)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 25 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (25600)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 26 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (26624)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 27 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (27648)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 28 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (28672)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 29 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (29696)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 30 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (30720)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 31 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (31744)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif

#if MAX_MODE(AP_INT_MAX_W) == 32 
#warning "Currently C++ ap_int/ap_fixed support max bitwidth (32768)"
#warning "You can use macro AP_INT_MAX_W to set a larger maximum value"
#endif
#endif
#undef MAX_MODE
#undef AP_INT_BASE

#endif //HECTOR
#else  // else other clang
template<int _AP_N, bool _AP_S> struct ssdm_int;

template<int _AP_N> struct ssdm_int<_AP_N,true> {    
  int V  __attribute__ ((bitwidth(_AP_N)));   
  INLINE ssdm_int<_AP_N,true>() { };     
  // FIXME: This is only for internal use. Clang checker.
  INLINE ssdm_int(int __attribute__((bitwidth(_AP_N))) val) : V(val) {}
};                  
template<int _AP_N> struct ssdm_int<_AP_N, false> {    
  unsigned V  __attribute__ ((bitwidth(_AP_N)));  
  INLINE ssdm_int<_AP_N,false>() { };     
  // FIXME: This is only for internal use. Clang checker.
  INLINE ssdm_int(int __attribute__((bitwidth(_AP_N))) val) : V(val) {}
};
#endif // end clang 3.1

enum BaseMode { AP_BIN=2, AP_OCT=8, AP_DEC=10, AP_HEX=16 };

#ifndef HECTOR

#define _ssdm_op_concat(Ret, X, Y) ({					\
      typeof(Ret) __Result__ = 0;					\
      typeof(X) __X2__ = X;						\
      typeof(Y) __Y2__ = Y;						\
      __builtin_bit_concat((void*)(&__Result__), (void*)(&__X2__), (void*)(&__Y2__)); \
      __Result__;							\
    })

#define _ssdm_op_get_bit(Val, Bit) ({					\
      typeof(Val) __Result__ = 0;					\
      typeof(Val) __Val2__ = Val;					\
      __builtin_bit_part_select((void*)(&__Result__), (void*)(&__Val2__), Bit, Bit); \
      (bool)(__Result__ & 1);						\
    })

#define _ssdm_op_set_bit(Val, Bit, Repl) ({				\
      typeof(Val) __Result__ = 0;					\
      typeof(Val) __Val2__ = Val;					\
      typeof(Repl) __Repl2__ = !!Repl;					\
      __builtin_bit_part_set((void*)(&__Result__), (void*)(&__Val2__), (void*)(&__Repl2__), Bit, Bit); \
      __Result__;							\
    })

#define _ssdm_op_get_range(Val, Lo, Hi) ({				\
      typeof(Val) __Result__ = 0;					\
      typeof(Val) __Val2__ = Val;					\
      __builtin_bit_part_select((void*)(&__Result__), (void*)(&__Val2__), Lo, Hi); \
      __Result__;							\
    })

#define _ssdm_op_set_range(Val, Lo, Hi, Repl) ({			\
      typeof(Val) __Result__ = 0;					\
      typeof(Val) __Val2__ = Val;					\
      typeof(Repl) __Repl2__ = Repl;					\
      __builtin_bit_part_set((void*)(&__Result__),			\
			     (void*)(&__Val2__), (void*)(&__Repl2__), Lo, Hi); \
      __Result__;							\
    })

#include "etc/autopilot_ssdm_bits.h"
#endif //HECTOR

// Forward declaration.
#include "ap_common.h"

/* Concatination reference.
   ----------------------------------------------------------------
*/
template<int _AP_W1, typename _AP_T1, int _AP_W2, typename _AP_T2>
struct ap_concat_ref {
  enum { _AP_WR = _AP_W1+_AP_W2, };
    
  _AP_T1& mbv1;
  _AP_T2& mbv2;
    
  INLINE ap_concat_ref(const ap_concat_ref<_AP_W1, _AP_T1,
		       _AP_W2, _AP_T2>& ref):
    mbv1(ref.mbv1), mbv2(ref.mbv2) {}
    
  INLINE ap_concat_ref( _AP_T1& bv1, _AP_T2& bv2) : mbv1(bv1), mbv2(bv2) { } 
    
    
  template <int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref& operator = (const ap_int_base<_AP_W3, _AP_S3>& val) {
    ap_int_base<_AP_W1+_AP_W2, false> vval(val);
    int W_ref1 = mbv1.length();
    int W_ref2 = mbv2.length();
    ap_int_base<_AP_W1,false> Part1;
    Part1.V = _ssdm_op_get_range(vval.V, W_ref2, W_ref1+W_ref2-1);
    mbv1.set(Part1);
    ap_int_base<_AP_W2,false> Part2;
    Part2.V = _ssdm_op_get_range(vval.V, 0, W_ref2-1);
    mbv2.set(Part2);
    return *this;
  }
    
  INLINE ap_concat_ref& operator = (unsigned long long val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator = (tmpVal);
  }

  /*template<typename _AP_T3>
    INLINE ap_concat_ref& operator = ( const _AP_T3& val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator=<_AP_W1+_AP_W2,false>(tmpVal);
    }*/
  template<int _AP_W3, typename _AP_T3, int _AP_W4, typename _AP_T4>
  INLINE ap_concat_ref& operator = (const ap_concat_ref<_AP_W3,_AP_T3,_AP_W4,_AP_T4>& val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator = (tmpVal);
  }

  INLINE ap_concat_ref& operator = (const ap_concat_ref<_AP_W1,_AP_T1,_AP_W2,_AP_T2>& val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator = (tmpVal);
  }
  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref& operator = (const ap_bit_ref<_AP_W3, _AP_S3>& val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator = (tmpVal);
  }
  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref& operator = (const ap_range_ref<_AP_W3, _AP_S3>& val) {
    ap_int_base<_AP_W1+_AP_W2, false> tmpVal(val);
    return operator = (tmpVal);
  }

  template<int _AP_W3, int _AP_I3, bool _AP_S3, 
	   ap_q_mode _AP_Q3, ap_o_mode _AP_O3, int _AP_N3> 
  INLINE ap_concat_ref& operator= (const af_range_ref<_AP_W3, _AP_I3, _AP_S3,
				   _AP_Q3, _AP_O3, _AP_N3>& val) {
    return operator = ((const ap_int_base<_AP_W3, false>)(val));
  } 

  template<int _AP_W3, int _AP_I3, bool _AP_S3, 
	   ap_q_mode _AP_Q3, ap_o_mode _AP_O3, int _AP_N3> 
  INLINE ap_concat_ref& operator= (const ap_fixed_base<_AP_W3, _AP_I3, _AP_S3,
				   _AP_Q3, _AP_O3, _AP_N3>& val) {
    return operator = (val.to_ap_int_base());
  } 

  template<int _AP_W3, int _AP_I3, bool _AP_S3, 
	   ap_q_mode _AP_Q3, ap_o_mode _AP_O3, int _AP_N3> 
  INLINE ap_concat_ref& operator= (const af_bit_ref<_AP_W3, _AP_I3, _AP_S3,
				   _AP_Q3, _AP_O3, _AP_N3>& val) {
    return operator=((unsigned long long)(bool)(val));
  }
  INLINE operator ap_int_base<_AP_WR, false> () const {
    return get();
  }

  INLINE operator unsigned long long () const {
    return get().to_uint64();
  }



  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, ap_range_ref<_AP_W3, _AP_S3> >
  operator, (const ap_range_ref<_AP_W3, _AP_S3>& a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3, ap_range_ref<_AP_W3, _AP_S3> >(*this, 
					     const_cast<ap_range_ref<_AP_W3, _AP_S3>& >(a2));
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, ap_int_base<_AP_W3, _AP_S3> >
  operator, (ap_int_base<_AP_W3, _AP_S3>& a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3, ap_int_base<_AP_W3, _AP_S3> >(*this, a2);
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, ap_int_base<_AP_W3, _AP_S3> >
  operator, (volatile ap_int_base<_AP_W3, _AP_S3>& a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3, ap_int_base<_AP_W3, _AP_S3> >(*this, 
					    const_cast<ap_int_base<_AP_W3, _AP_S3>& >(a2));
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, ap_int_base<_AP_W3, _AP_S3> >
  operator, (const ap_int_base<_AP_W3, _AP_S3>& a2) {
    ap_int_base<_AP_W3,_AP_S3> op(a2);
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3, ap_int_base<_AP_W3, _AP_S3> >(*this, 
					    const_cast<ap_int_base<_AP_W3, _AP_S3>& >(op));
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, ap_int_base<_AP_W3, _AP_S3> >
  operator, (const volatile ap_int_base<_AP_W3, _AP_S3>& a2) {
    ap_int_base<_AP_W3,_AP_S3> op(a2);
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3, ap_int_base<_AP_W3, _AP_S3> >(*this, 
					    const_cast<ap_int_base<_AP_W3, _AP_S3>& >(op));
  }
  template<int _AP_W3, bool _AP_S3>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, 1, ap_bit_ref<_AP_W3, _AP_S3> >
  operator, (const ap_bit_ref<_AP_W3, _AP_S3>& a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      1, ap_bit_ref<_AP_W3, _AP_S3> >(*this, 
				      const_cast<ap_bit_ref<_AP_W3, _AP_S3>& >(a2));
  }

  template<int _AP_W3, typename _AP_T3, int _AP_W4, typename _AP_T4>
  INLINE ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3+_AP_W4, 
		       ap_concat_ref<_AP_W3,_AP_T3,_AP_W4,_AP_T4> >
  operator, (const ap_concat_ref<_AP_W3,_AP_T3,_AP_W4,_AP_T4>& a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 
      _AP_W3+_AP_W4, ap_concat_ref<_AP_W3,_AP_T3,_AP_W4,_AP_T4> >(
								  *this, const_cast<ap_concat_ref<_AP_W3,_AP_T3,
								  _AP_W4,_AP_T4>& >(a2));
  }
    
  template <int _AP_W3, int _AP_I3, bool _AP_S3, ap_q_mode _AP_Q3, ap_o_mode _AP_O3, int _AP_N3>
  INLINE
  ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, af_range_ref<_AP_W3, _AP_I3, _AP_S3, _AP_Q3, _AP_O3, _AP_N3> >
  operator, (const af_range_ref<_AP_W3, _AP_I3, _AP_S3, _AP_Q3, 
	     _AP_O3, _AP_N3> &a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, _AP_W3, af_range_ref<_AP_W3,
      _AP_I3, _AP_S3, _AP_Q3, _AP_O3, _AP_N3> >(*this, 
						const_cast<af_range_ref< _AP_W3, _AP_I3, _AP_S3, _AP_Q3,
						_AP_O3, _AP_N3>& >(a2));
  }
    
  template <int _AP_W3, int _AP_I3, bool _AP_S3, ap_q_mode _AP_Q3, ap_o_mode _AP_O3, int _AP_N3>
  INLINE
  ap_concat_ref<_AP_WR, ap_concat_ref, 1, af_bit_ref<_AP_W3, _AP_I3, _AP_S3, _AP_Q3, _AP_O3, _AP_N3> >
  operator, (const af_bit_ref<_AP_W3, _AP_I3, _AP_S3, _AP_Q3, 
	     _AP_O3, _AP_N3> &a2) {
    return ap_concat_ref<_AP_WR, ap_concat_ref, 1, af_bit_ref<_AP_W3,
      _AP_I3, _AP_S3, _AP_Q3, _AP_O3, _AP_N3> >(*this, 
						const_cast<af_bit_ref<_AP_W3, _AP_I3, _AP_S3, _AP_Q3,
						_AP_O3, _AP_N3>& >(a2));
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_int_base<AP_MAX(_AP_WR,_AP_W3), _AP_S3>
  operator & (const ap_int_base<_AP_W3,_AP_S3>& a2) {
    return get() & a2;
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_int_base<AP_MAX(_AP_WR,_AP_W3), _AP_S3>
  operator | (const ap_int_base<_AP_W3,_AP_S3>& a2) {
    return get() | a2;
  }

  template<int _AP_W3, bool _AP_S3>
  INLINE ap_int_base<AP_MAX(_AP_WR,_AP_W3), _AP_S3>
  operator ^ (const ap_int_base<_AP_W3,_AP_S3>& a2) {
    return get() ^ a2;
  }

#if 0
  template<int Hi, int Lo>
  INLINE ap_int_base<Hi-Lo+1, false> slice() {
    ap_int_base<_AP_WR, false> bv = get();
    return bv.slice<Hi,Lo>();
  }
#endif
    
  INLINE ap_int_base<_AP_WR,false> get() const {
    ap_int_base<_AP_WR,false> tmpVal(0);
    int W_ref1 = mbv1.length();
    int W_ref2 = mbv2.length();
    tmpVal.V = _ssdm_op_set_range(tmpVal.V, 0, W_ref2-1, 
				  (ap_int_base<_AP_W2,false>(mbv2)).V);
    tmpVal.V = _ssdm_op_set_range(tmpVal.V, W_ref2, W_ref1+W_ref2-1,
				  (ap_int_base<_AP_W1,false>(mbv1)).V);
    return tmpVal;
  }

  template <int _AP_W3>
  INLINE void set(const ap_int_base<_AP_W3, false>& val) { 
    ap_int_base<_AP_W1+_AP_W2, false> vval(val);
    int W_ref1 = mbv1.length();
    int W_ref2 = mbv2.length();
    ap_int_base<_AP_W1,false> tmpVal1;
    tmpVal1.V = _ssdm_op_get_range(vval.V, W_ref2, W_ref1+W_ref2-1); 
    mbv1.set(tmpVal1);
    ap_int_base<_AP_W2, false> tmpVal2;
    tmpVal2.V=_ssdm_op_get_range(vval.V, 0, W_ref2-1);
    mbv2.set(tmpVal2);
  }

  INLINE int length() const {
    return mbv1.length() + mbv2.length();
  }
};

#if 0
template <int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>
INLINE
ap_concat_ref<_AP_W1,ap_int_base<_AP_W1,_AP_S1>, _AP_W2,ap_int_base<_AP_W2,_AP_S2> >
operator , (ap_int_base<_AP_W1,_AP_S1>& a1, ap_int_base<_AP_W2,_AP_S2>& a2) {
  /*printf("concat 2...\n");*/
  return ap_concat_ref<_AP_W1,ap_int_base<_AP_W1,_AP_S1>,_AP_W2,ap_int_base<_AP_W2,_AP_S2> >(a1, a2);
}
#endif

/* Range (slice) reference.
   ----------------------------------------------------------------
*/
template<int _AP_W, bool _AP_S>
struct ap_range_ref {
  ap_int_base<_AP_W,_AP_S>  &d_bv;
  int l_index;
  int h_index;
    
public:
  INLINE ap_range_ref(const ap_range_ref<_AP_W, _AP_S>& ref):
    d_bv(ref.d_bv), l_index(ref.l_index), h_index(ref.h_index) {}

  INLINE ap_range_ref(ap_int_base<_AP_W,_AP_S>* bv, int h, int l) :
    d_bv(*bv), l_index(l), h_index(h) {
    /*AP_ASSERT(h >= l, "Range must be (High, Low)");*/
  }

  INLINE operator ap_int_base<_AP_W, false> () const {
    ap_int_base<_AP_W,false> ret;
    ret.V = _ssdm_op_get_range(d_bv.V, l_index, h_index);
    return ret;
  }

  INLINE operator unsigned long long () const {
    return to_uint64();
  }

  INLINE ap_range_ref& operator = (unsigned long long val) {
    ap_int_base<_AP_W, false> loc(val);
    d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, loc.V);
    return *this;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref& operator = (const ap_int_base<_AP_W2,_AP_S2>& val) {
    ap_int_base<_AP_W, false> loc(val);
    d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, loc.V);
    return *this;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref& operator= (const ap_range_ref<_AP_W2,_AP_S2>& val) {
    return operator=((const ap_int_base<_AP_W2, false>)val);
  }

  INLINE ap_range_ref& operator= (const ap_range_ref<_AP_W, _AP_S>& val) {
    return operator=((const ap_int_base<_AP_W, false>)val);
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_range_ref& operator= (const af_range_ref<_AP_W2, _AP_I2, _AP_S2,
				  _AP_Q2, _AP_O2, _AP_N2>& val) {
    return operator=((const ap_int_base<_AP_W2, false>)(val));
  } 

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_range_ref& operator= (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2,
				  _AP_Q2, _AP_O2, _AP_N2>& val) {
    return operator=(val.to_ap_int_base());
  }
 
  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_range_ref& operator= (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2,
				  _AP_Q2, _AP_O2, _AP_N2>& val) {
    return operator=((unsigned long long)(bool)(val));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref& operator= (const ap_bit_ref<_AP_W2, _AP_S2>& val) {
    return operator=((unsigned long long)(bool)(val));
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_range_ref& operator= (const ap_concat_ref<_AP_W2, _AP_T3, _AP_W3, _AP_T3>& val) {
    return operator=((const ap_int_base<_AP_W2 + _AP_W3, false>)(val));
  }
 
  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W2,ap_range_ref<_AP_W2,_AP_S2> > 
  operator, (const ap_range_ref<_AP_W2,_AP_S2> &a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, 
      ap_range_ref<_AP_W2,_AP_S2> >(*this, 
				    const_cast<ap_range_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > 
  operator, (ap_int_base<_AP_W2,_AP_S2> &a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, a2);
  }

  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W,ap_int_base<_AP_W,_AP_S> > 
  operator, (ap_int_base<_AP_W,_AP_S> &a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W, ap_int_base<_AP_W,_AP_S> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > 
  operator, (volatile ap_int_base<_AP_W2,_AP_S2>& a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, 
									      const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > 
  operator, (const ap_int_base<_AP_W2,_AP_S2>& a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, 
									      const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > 
  operator, (const volatile ap_int_base<_AP_W2,_AP_S2>& a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, 
      ap_int_base<_AP_W2,_AP_S2> >(*this, 
				   const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W,ap_range_ref,1,ap_bit_ref<_AP_W2,_AP_S2> > 
  operator, (const ap_bit_ref<_AP_W2,_AP_S2> &a2) {
    return 
      ap_concat_ref<_AP_W, ap_range_ref, 1, ap_bit_ref<_AP_W2,_AP_S2> >(*this, 
									const_cast<ap_bit_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE
  ap_concat_ref<_AP_W, ap_range_ref, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >
  operator, (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> &a2) {
    return ap_concat_ref<_AP_W, ap_range_ref, _AP_W2+_AP_W3, 
      ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(*this, 
						      const_cast<ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& >(a2));
  }

  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> a2) {
    return ap_concat_ref<_AP_W, ap_range_ref, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, 
						const_cast<af_range_ref<_AP_W2,_AP_I2, _AP_S2, 
						_AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_range_ref, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<_AP_W, ap_range_ref, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, 
						const_cast<af_bit_ref<_AP_W2,_AP_I2, _AP_S2, _AP_Q2, 
						_AP_O2, _AP_N2>& >(a2));
  }
    
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator == (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    ap_int_base<_AP_W, false> lop(*this); 
    ap_int_base<_AP_W2, false> hop(op2);
    return lop == hop;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator != (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    return !(operator == (op2));
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator < (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    ap_int_base<_AP_W, false> lop (*this);
 
    ap_int_base<_AP_W2, false> hop (op2);
    return lop < hop;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator <= (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    ap_int_base<_AP_W, false> lop (*this);
 
    ap_int_base<_AP_W2, false> hop (op2);
    return lop <= hop;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator > (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    return !(operator <= (op2));
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator >= (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    return !(operator < (op2));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator |= (const ap_range_ref<_AP_W2,_AP_S2> &op2) {
    (this->d_bv).V |= (op2.d_bv).V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator |= (const ap_int<_AP_W2> &op2) {
    (this->d_bv).V |= op2.V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator |= (const ap_uint<_AP_W2> &op2) {
    (this->d_bv.V) |= op2.V;
    return *this;
  };

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator &= (const ap_range_ref<_AP_W2,_AP_S2> &op2) {
    (this->d_bv).V &= (op2.d_bv).V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator &= (const ap_int<_AP_W2> &op2) {
    (this->d_bv).V &= op2.V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator &= (const ap_uint<_AP_W2> &op2) {
    (this->d_bv.V) &= op2.V;
    return *this;
  };

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator ^= (const ap_range_ref<_AP_W2,_AP_S2> &op2) {
    (this->d_bv).V ^= (op2.d_bv).V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator ^= (const ap_int<_AP_W2> &op2) {
    (this->d_bv).V ^= op2.V;
    return *this;
  };

  template<int _AP_W2>
  INLINE ap_range_ref<_AP_W,_AP_S> & operator ^= (const ap_uint<_AP_W2> &op2) {
    (this->d_bv.V) ^= op2.V;
    return *this;
  };

  template <int _AP_W3>
  INLINE void set(const ap_int_base<_AP_W3, false>& val) {
    d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, val.V);
  }

  INLINE int length() const { 
    return h_index >= l_index ? h_index - l_index + 1 : l_index - h_index + 1;
  }

  INLINE int to_int() const {
    return (int)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE unsigned to_uint() const {
    return (unsigned)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE long to_long() const {
    return (long)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE unsigned long to_ulong() const {
    return (unsigned long)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE ap_slong to_int64() const {
    return (ap_slong)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE ap_ulong to_uint64() const {
    return (ap_ulong)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
  }
  INLINE bool and_reduce() const {
    bool ret = true;
    bool reverse = l_index > h_index;
    unsigned low = reverse ? h_index : l_index;
    unsigned high = reverse ? l_index : h_index;
    for (unsigned i = low; i != high; ++i) {
#pragma HLS unroll
      ret &= (bool)(_ssdm_op_get_range(d_bv.V, i, i)); 
    }
    return ret;
  }
  INLINE bool or_reduce() const {
    bool ret = false;
    bool reverse = l_index > h_index;
    unsigned low = reverse ? h_index : l_index;
    unsigned high = reverse ? l_index : h_index;
    for (unsigned i = low; i != high; ++i) {
#pragma HLS unroll
      ret |= (bool)(_ssdm_op_get_range(d_bv.V, i, i)); 
    }
    return ret;
  }
  INLINE bool xor_reduce() const {
    bool ret = false;
    bool reverse = l_index > h_index;
    unsigned low = reverse ? h_index : l_index;
    unsigned high = reverse ? l_index : h_index;
    for (unsigned i = low; i != high; ++i) {
#pragma HLS unroll
      ret ^= (bool)(_ssdm_op_get_range(d_bv.V, i, i)); 
    }
    return ret;
  }
};


/* Bit reference.
   ----------------------------------------------------------------
*/
template<int _AP_W, bool _AP_S>
struct ap_bit_ref { 
  ap_int_base<_AP_W, _AP_S>& d_bv;
  int d_index;

public:
  INLINE ap_bit_ref(const ap_bit_ref<_AP_W, _AP_S>& ref):
    d_bv(ref.d_bv), d_index(ref.d_index) {}

  INLINE ap_bit_ref(ap_int_base<_AP_W,_AP_S>* bv, int index=0) :
    d_bv(*bv), d_index(index) { }
  INLINE operator bool () const { return _ssdm_op_get_bit(d_bv.V, d_index); }
  INLINE bool to_bool() const { return _ssdm_op_get_bit(d_bv.V, d_index); }
    
  INLINE ap_bit_ref& operator = ( unsigned long long val ) {
    d_bv.V = _ssdm_op_set_bit(d_bv.V, d_index, val);
    return *this;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref& operator = ( const ap_int_base<_AP_W2,_AP_S2> &val )  {
    return operator =((unsigned long long)(val.V != 0));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref& operator = ( const ap_range_ref<_AP_W2,_AP_S2> &val )  {
    return operator =(val.operator ap_int_base<_AP_W2, false>());
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref& operator = (const ap_bit_ref<_AP_W2,_AP_S2>& val) {
    return operator =((unsigned long long) (bool) val);
  }

  INLINE ap_bit_ref& operator = (const ap_bit_ref<_AP_W,_AP_S>& val) {
    return operator =((unsigned long long) (bool) val);
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_bit_ref& operator= (const af_range_ref<_AP_W2, _AP_I2, _AP_S2,
				_AP_Q2, _AP_O2, _AP_N2>& val) {
    return operator=((const ap_int_base<_AP_W2, false>)(val));
  } 

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_bit_ref& operator= (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2,
				_AP_Q2, _AP_O2, _AP_N2>& val) {
    return operator=((unsigned long long)(bool)(val));
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_bit_ref& operator= (const ap_concat_ref<_AP_W2, _AP_T3, _AP_W3, _AP_T3>& val) {
    return operator=((const ap_int_base<_AP_W2 + _AP_W3, false>)(val));
  }
 
 
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> > 
  operator, (ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, a2);
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> > 
  operator, (volatile ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> 
      >(*this, const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> > 
  operator, (const ap_int_base<_AP_W2, _AP_S2>& a2) {
    ap_int_base<_AP_W2,_AP_S2> op(a2);
    return ap_concat_ref<1,ap_bit_ref,_AP_W2,ap_int_base<_AP_W2,
      _AP_S2> >(*this, const_cast<ap_int_base<_AP_W2, _AP_S2>& >(op));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_int_base<_AP_W2,_AP_S2> > 
  operator, (const volatile ap_int_base<_AP_W2, _AP_S2>& a2) {
    ap_int_base<_AP_W2,_AP_S2> op(a2);
    return ap_concat_ref<1,ap_bit_ref,_AP_W2, ap_int_base<_AP_W2,_AP_S2>
      >(*this, const_cast< ap_int_base<_AP_W2, _AP_S2>& >(op));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_range_ref<_AP_W2,_AP_S2> >
  operator, (const ap_range_ref<_AP_W2, _AP_S2> &a2) {
    return 
      ap_concat_ref<1, ap_bit_ref, _AP_W2, ap_range_ref<_AP_W2,
      _AP_S2> >(*this, const_cast<ap_range_ref<_AP_W2, _AP_S2> &>(a2));
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<1, ap_bit_ref, 1, ap_bit_ref<_AP_W2,_AP_S2> >
  operator, (const ap_bit_ref<_AP_W2, _AP_S2> &a2) {
    return ap_concat_ref<1, ap_bit_ref, 1, ap_bit_ref<_AP_W2,_AP_S2> >(*this, 
								       const_cast<ap_bit_ref<_AP_W2,_AP_S2>& >(a2));
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_concat_ref<1, ap_bit_ref, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3> >
  operator, (const ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3> &a2) {
    return ap_concat_ref<1,ap_bit_ref,_AP_W2+_AP_W3,ap_concat_ref<_AP_W2,
      _AP_T2,_AP_W3,_AP_T3> >(*this, 
			      const_cast<ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3> &>(a2));
  }

  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<1, ap_bit_ref, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<1, ap_bit_ref, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, 
						const_cast<af_range_ref<_AP_W2,  _AP_I2, _AP_S2, _AP_Q2, 
						_AP_O2, _AP_N2>& >(a2));
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<1, ap_bit_ref, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<1, ap_bit_ref, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, 
						const_cast<af_bit_ref<_AP_W2, _AP_I2, _AP_S2, 
						_AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
 
  template <int _AP_W2, bool _AP_S2>
  INLINE bool operator == (const ap_bit_ref<_AP_W2, _AP_S2>& op) {
    return get() == op.get();
  }
    
  template <int _AP_W2, bool _AP_S2>
  INLINE bool operator != (const ap_bit_ref<_AP_W2, _AP_S2>& op) {
    return get() != op.get();
  }

  INLINE bool get() const {
    return _ssdm_op_get_bit(d_bv.V, d_index);
  }

  INLINE bool get() {
    return _ssdm_op_get_bit(d_bv.V, d_index);
  }

  template <int _AP_W3>
  INLINE void set(const ap_int_base<_AP_W3, false>& val) {
    operator = (val);
  }

  INLINE bool operator ~() const  {
    bool bit = _ssdm_op_get_bit(d_bv.V, d_index);
    return bit ? false : true;
  }

  INLINE int length() const { return 1; }
};

template <int _AP_N, bool _AP_S> struct retval;

template <int _AP_N> 
struct retval<_AP_N, true>  {
  typedef ap_slong Type;
};
template <int _AP_N> 
struct retval<_AP_N, false>  {
  typedef ap_ulong Type;
};

template<> struct retval<1, true> {
  typedef signed char Type;
};
template<> struct retval<1, false> {
  typedef unsigned char Type;
};
template<> struct retval<2, true> {
  typedef short Type;
};
template<> struct retval<2, false> {
  typedef unsigned short Type;
};
template<> struct retval<3, true> {
  typedef int Type;
};
template<> struct retval<3, false> {
  typedef unsigned int Type;
};
template<> struct retval<4, true> {
  typedef int Type;
};
template<> struct retval<4, false> {
  typedef unsigned int Type;
};

/* ----------------------------------------------------------------
   ap_int_base: AutoPilot integer/Arbitrary precision integer.
   ----------------------------------------------------------------
*/

template<int _AP_W, bool _AP_S>
struct ap_int_base <_AP_W, _AP_S, true>: public ssdm_int<_AP_W,_AP_S> { 

public:
  typedef ssdm_int<_AP_W, _AP_S> Base;

  typedef typename retval< (_AP_W + 7)/8, _AP_S>::Type RetType;

  static const int width = _AP_W;



  template<int _AP_W2, bool _AP_S2>
  struct RType {
    enum {
      mult_w = _AP_W+_AP_W2, 
      mult_s = _AP_S||_AP_S2,
      plus_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2))+1,
      plus_s = _AP_S||_AP_S2,
      minus_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2))+1,
      minus_s = true,
      div_w = _AP_W+_AP_S2,
      div_s = _AP_S||_AP_S2,
      mod_w = AP_MIN(_AP_W,_AP_W2+(!_AP_S2&&_AP_S)), 
      mod_s = _AP_S,
      logic_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2)),
      logic_s = _AP_S||_AP_S2
    };

    typedef ap_int_base<mult_w, mult_s> mult;
    typedef ap_int_base<plus_w, plus_s> plus;
    typedef ap_int_base<minus_w, minus_s> minus;
    typedef ap_int_base<logic_w, logic_s> logic;
    typedef ap_int_base<div_w, div_s> div;
    typedef ap_int_base<mod_w, mod_s> mod;
    typedef ap_int_base<_AP_W, _AP_S> arg1;
    typedef bool reduce;
  };
    
  /* Constructors.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base() {
    /*
      #ifdef __SC_COMPATIBLE__
      Base::V = 0;
      #endif
    */
  }
    
  //INLINE ap_int_base(const ap_int_base& op) { Base::V = op.V; }
  //INLINE ap_int_base(const volatile ap_int_base& op) { Base::V = op.V; }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_int_base<_AP_W2,_AP_S2> &op) { Base::V = op.V; }
    
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const volatile ap_int_base<_AP_W2,_AP_S2> &op) { Base::V = op.V; }

  /* For C++ basic data types.*/
#if 0
  template<typename _AP_T>
  INLINE ap_int_base(_AP_T v) { Base::V = v; }
#else
  INLINE explicit ap_int_base(bool op) { Base::V = op; }
  INLINE explicit ap_int_base(signed char op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned char op) { Base::V = op; }
  INLINE explicit ap_int_base(short op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned short op) { Base::V = op; }
  INLINE explicit ap_int_base(int op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned int op) { Base::V = op; }
  INLINE explicit ap_int_base(long op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned long op) { Base::V = op; }
  INLINE explicit ap_int_base(ap_slong op) { Base::V = op; }
  INLINE explicit ap_int_base(ap_ulong op) { Base::V = op; }
  INLINE explicit ap_int_base(half op) { Base::V = op; }
  INLINE explicit ap_int_base(float op) { Base::V = op; }
  INLINE explicit ap_int_base(double op) { Base::V = op; }
#endif 
  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base(const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op) {
    Base::V = op.to_ap_int_base().V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_range_ref<_AP_W2,_AP_S2>& ref) {
    Base::V = ref.operator ap_int_base<_AP_W2, false>().V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_bit_ref<_AP_W2,_AP_S2>& ref) {
    Base::V = ref.operator bool();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base(const ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>& ref) {
    const ap_int_base<ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>::_AP_WR,false> tmp = ref.get();
    Base::V = tmp.V;
  }

  /* This constructor is not usable yet, because the second parameter of 
     __builtin_bit_from_string(...)
     is required to be a constant C string.
  */
  INLINE ap_int_base(const char* str) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), 10, 
		      _AP_W, _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
  }

  INLINE ap_int_base(const char* str, signed char radix) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), radix, 
		      _AP_W, _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
  }
    
  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_int_base(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, 
		     _AP_Q2, _AP_O2, _AP_N2> &val) {
    Base::V = (val.operator ap_int_base<_AP_W2, false> ()).V;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_int_base(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, 
		     _AP_Q2, _AP_O2, _AP_N2> &val) {
    Base::V = val.operator bool ();
  }
  
  // FIXME: This is only for internal use. Clang checker.
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_int_base(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val) {}
  #endif
  
  INLINE ap_int_base read() volatile {
    AP_DEBUG(printf("call read %d\n", Base::V);); 
    ap_int_base ret;
    ret.V = Base::V;
    return ret;
  }
  INLINE void write(const ap_int_base<_AP_W, _AP_S>& op2) volatile {
    AP_DEBUG(printf("call write %d\n", op2.V););
    Base::V = op2.V;
  }

  /* Another form of "write".*/
#if 1
  template<int _AP_W2, bool _AP_S2>
  INLINE void operator = (const volatile ap_int_base<_AP_W2,_AP_S2>& op2) volatile {
    Base::V = op2.V;
  }
	
  INLINE void operator = (const volatile ap_int_base<_AP_W, _AP_S>& op2) volatile {
    Base::V = op2.V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE void operator = (const ap_int_base<_AP_W2,_AP_S2>& op2) volatile {
    Base::V = op2.V;
  }
	
  INLINE void  operator = (const ap_int_base<_AP_W, _AP_S>& op2) volatile {
    Base::V = op2.V;
  }
#endif
#if 0
  template<typename _AP_T2>
  INLINE void operator = (const _AP_T2& op2) volatile {
    Base::V = ap_int_base(op2).V;
  }
#endif
#if 1
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const volatile ap_int_base<_AP_W2,_AP_S2>& op2) {
    Base::V = op2.V;
    return *this;
  }
	
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_int_base<_AP_W2,_AP_S2>& op2)  {
    Base::V = op2.V;
    return *this;
  }

  INLINE ap_int_base& operator = (const volatile ap_int_base<_AP_W,_AP_S>& op2) {
    Base::V = op2.V;
    return *this;
  }
	
  INLINE ap_int_base& operator = (const ap_int_base<_AP_W,_AP_S>& op2)  {
    Base::V = op2.V;
    return *this;
  }
  INLINE ap_int_base& operator = (const char* str) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), 
		      10, _AP_W,  _AP_S, AP_TRN, AP_WRAP, 0,_AP_C99);
    Base::V = Result;
    return *this;
  }

  INLINE ap_int_base& set(const char* str, signed char radix) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), radix,
		      _AP_W,  _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
    return *this;
  }
        
#endif
#define ASSIGN_FROM_C_TYPE(_AP_T2)		\
  INLINE ap_int_base& operator = (_AP_T2 op) {	\
    Base::V = op;				\
    return *this;				\
  }
  ASSIGN_FROM_C_TYPE(signed char)
  ASSIGN_FROM_C_TYPE(unsigned char)
  ASSIGN_FROM_C_TYPE(short)
  ASSIGN_FROM_C_TYPE(unsigned short)
  ASSIGN_FROM_C_TYPE(int)
  ASSIGN_FROM_C_TYPE(unsigned int)
  ASSIGN_FROM_C_TYPE(ap_slong)
  ASSIGN_FROM_C_TYPE(ap_ulong)
#undef ASSIGN_FROM_C_TYPE
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_bit_ref<_AP_W2, _AP_S2>& op2) {
    Base::V = (bool) op2;
    return *this;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    Base::V = (ap_int_base<_AP_W2, false>(op2)).V;
    return *this;
  }
 
  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base& operator = (const ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>& op2) {
    Base::V = op2.get().V;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = op.to_ap_int_base().V;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = (bool) op;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = ((const ap_int_base<_AP_W2, false>)(op)).V;
    return *this;
  }

  INLINE operator RetType() const { return (RetType)(Base::V); }


  /* Explicit conversions to C interger types.
     ----------------------------------------------------------------
  */
  INLINE bool to_bool() const {return (bool)(Base::V);}
  INLINE unsigned char to_uchar() const {return (unsigned char)(Base::V);}
  INLINE signed char to_char() const {return (signed char)(Base::V);}
  INLINE unsigned short to_ushort() const {return (unsigned short)(Base::V);}
  INLINE short to_short() const {return (short)(Base::V);}
  INLINE int to_int() const { return (int)(Base::V); }
  INLINE unsigned to_uint() const { return (unsigned)(Base::V); }
  INLINE long to_long() const { return (long)(Base::V); }
  INLINE unsigned long to_ulong() const { return (unsigned long)(Base::V); }
  INLINE ap_slong to_int64() const { return (ap_slong)(Base::V); }
  INLINE ap_ulong to_uint64() const { return (ap_ulong)(Base::V); }
  INLINE double to_double() const { return (double)(Base::V); }
#if 0
  INLINE operator char() const { return (char)(Base::V); }
  INLINE operator signed char() const { return (signed char)(Base::V); }
  INLINE operator unsigned char() const { return (unsigned char)(Base::V); }
  INLINE operator short() const { return (short)(Base::V); }
  INLINE operator unsigned short() const { return (unsigned short)(Base::V); }
  INLINE operator int() const { return (int)(Base::V); }
  INLINE operator unsigned int () const { return (unsigned)(Base::V); }
  INLINE operator long () const { return (long)(Base::V); }
  INLINE operator unsigned long () const { return (unsigned long)(Base::V); }
  INLINE operator long long () const { return (ap_slong)(Base::V); }
  INLINE operator unsigned long long () const { return (ap_ulong)(Base::V); }
#endif
  INLINE int length() const { return _AP_W; }
  INLINE int length() const volatile { return _AP_W; }

  /*INLINE operator ap_ulong () { return (ap_ulong)(Base::V); }*/

  /*Reverse the contents of ap_int_base instance. I.e. LSB becomes MSB and vise versa*/
  INLINE ap_int_base& reverse () {
    Base::V = _ssdm_op_get_range(Base::V, _AP_W - 1, 0);    
    return *this;
  }

  /*Return true if the value of ap_int_base instance is zero*/
  INLINE bool iszero () const {
    return Base::V == 0 ; 
  }     
    
  /*Return true if the value of ap_int_base instance is zero*/
  INLINE bool is_zero () const {
    return Base::V == 0 ; 
  }
     
  /* x < 0 */ 
  INLINE bool sign () const {
    if (_AP_S && _ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, _AP_W - 1))
      return true;
    else
      return false;
  }
  
  /* x[i] = 0 */
  INLINE void clear(int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, 0); 
  }

  /* x[i] = !x[i]*/
  INLINE void invert(int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    bool val =  _ssdm_op_get_bit(Base::V, i);
    if (val) Base::V = _ssdm_op_set_bit(Base::V, i, 0);
    else     Base::V = _ssdm_op_set_bit(Base::V, i, 1);
  } 

      
  INLINE bool test (int i) const { 
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    return _ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, i);
  }
   
  //Set the ith bit into 1
  INLINE void set (int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, 1); 
  }

  //Set the ith bit into v
  INLINE void set (int i, bool v) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, v);
  }
    
  //This is used for sc_lv and sc_bv, which is implemented by sc_uint 
  //Rotate an ap_int_base object n places to the left
  INLINE void lrotate(int n) {
    AP_ASSERT(n >= 0 && n < _AP_W, "shift value out of range");
    typeof(Base::V) l_p = Base::V << n;
    typeof(Base::V) r_p = Base::V >> (_AP_W - n);
    Base::V = l_p | r_p;
  }

  //This is used for sc_lv and sc_bv, which is implemented by sc_uint 
  //Rotate an ap_int_base object n places to the right 
  INLINE void rrotate(int n) {
    AP_ASSERT(n >= 0 && n < _AP_W, "shift value out of range");
    typeof(Base::V) l_p = Base::V << (_AP_W - n);
    typeof(Base::V) r_p = Base::V >> n;
    Base::V = l_p | r_p;
  }
 
  //Set the ith bit into v 
  INLINE void set_bit (int i, bool v) {
    Base::V = _ssdm_op_set_bit(Base::V, i, v);
  }

  //Get the value of ith bit
  INLINE bool get_bit (int i) const {
    return (bool)_ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, i);
  }

  //complements every bit 
  INLINE void b_not() {
    Base::V = ~Base::V; 
  }

  // Count the number of zeros from the most significant bit 
  // to the first one bit. Note this is only for ap_fixed_base whose 
  // _AP_W <= 64, otherwise will incur assertion.
  INLINE int countLeadingZeros() {
    if (_AP_W <= 32) {
      ap_int_base<32, false> t(-1ULL);
      t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
      return __builtin_ctz(t.V);
    } else if (_AP_W <= 64) {
      ap_int_base<64, false> t(-1ULL);
      t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
      return __builtin_ctzll(t.V);
    } else {
      enum { __N = (_AP_W+63)/64 };
      int NZeros = 0;
      int i = 0;
      bool hitNonZero = false;
      for (i=0; i<__N-1; ++i) {
	ap_int_base<64, false> t;
	t.range(0, 63) = this->range(_AP_W - i*64 - 64, _AP_W - i*64 - 1);
	NZeros += hitNonZero?0:__builtin_clzll(t.V);
	hitNonZero |= (t.to_uint64() != 0);
      }
      if (!hitNonZero) {
	ap_int_base<64, false> t(-1ULL);
	t.range(63-(_AP_W-1)%64, 63) = this->range(0, (_AP_W-1)%64);
	NZeros += __builtin_clzll(t.V);
      }
      return NZeros;
    }
  }

#define OP_ASSIGN_AP(Sym)						\
  template<int _AP_W2, bool _AP_S2>					\
  INLINE ap_int_base& operator Sym ( const ap_int_base<_AP_W2,_AP_S2> &op2) { \
    AP_DEBUG(printf("call assign op %s\n", #Sym););			\
    Base::V Sym op2.V;							\
    return *this;							\
  }

  /* Arithmetic assign.
     ----------------------------------------------------------------
  */
  OP_ASSIGN_AP(*=)
  OP_ASSIGN_AP(+=)
  OP_ASSIGN_AP(-=)
  OP_ASSIGN_AP(/=)
  OP_ASSIGN_AP(%=)
        
  /* Bitwise assign: and, or, xor.
     ----------------------------------------------------------------
  */
  OP_ASSIGN_AP(&=)
  OP_ASSIGN_AP(|=)
  OP_ASSIGN_AP(^=)

    
  /* Prefix increment, decrement.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base& operator ++() {
    operator+=((ap_int_base<1,false>) 1);
    return *this;
  }
  INLINE ap_int_base& operator --() {
    operator-=((ap_int_base<1,false>) 1);
    return *this;
  }
    
  /* Postfix increment, decrement
     ----------------------------------------------------------------
  */
  INLINE const ap_int_base operator ++(int) {
    ap_int_base t = *this;
    operator+=((ap_int_base<1,false>) 1);
    return t;
  }
  INLINE const ap_int_base operator --(int) {
    ap_int_base t = *this;
    operator-=((ap_int_base<1,false>) 1);
    return t;
  }
    
  /* Unary arithmetic.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base operator +() const {
    return *this;
  }
  /* Not (!)
     ----------------------------------------------------------------
  */
  INLINE bool operator ! () const {
    return Base::V == 0; 
  }

  INLINE ap_int_base<AP_MIN(64, _AP_W + 1), true>
  operator -() const {
    return ((ap_int_base<1,false>) 0) - *this;
  }
    
  /* Shift (result constrained by left operand).
     ----------------------------------------------------------------
  */

  template<int _AP_W2>
  INLINE ap_int_base operator << ( const ap_int_base<_AP_W2,true> &op2 ) const {
    bool isNeg = op2[_AP_W2 - 1]; 
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator >> (sh);
    } else 
      return operator << (sh);
  }

  template<int _AP_W2>
  INLINE ap_int_base operator << ( const ap_int_base<_AP_W2,false> &op2 ) const {
    ap_int_base r ;
    r.V = Base::V << op2.to_uint();
    return r;
  }


  template<int _AP_W2>
  INLINE ap_int_base operator >> ( const ap_int_base<_AP_W2,true> &op2 ) const {
    bool isNeg = op2[_AP_W2 - 1];
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator << (sh);
    } 
    return operator >> (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base operator >> ( const ap_int_base<_AP_W2,false> &op2 ) const {
    ap_int_base r;
    r.V = Base::V >> op2.to_uint();
    return r;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base operator << ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) const {
    return *this << (op2.operator ap_int_base<_AP_W2, false>());
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base operator >> ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) const {
    return *this >> (op2.operator ap_int_base<_AP_W2, false>());
  }

    
  /* Shift assign
     ----------------------------------------------------------------
  */
  template<int _AP_W2>
  INLINE ap_int_base& operator <<= ( const ap_int_base<_AP_W2,true> &op2 ) {
    bool isNeg = op2[_AP_W2 - 1]; 
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator >>= (sh);
    } else 
      return operator <<= (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator <<= ( const ap_int_base<_AP_W2,false> &op2 ) {
    Base::V <<= op2.to_uint();
    return *this;
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator >>= ( const ap_int_base<_AP_W2,true> &op2 ) {
    bool isNeg = op2[_AP_W2 - 1];
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator <<= (sh);
    } 
    return operator >>= (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator >>= ( const ap_int_base<_AP_W2,false> &op2 ) {
    Base::V >>= op2.to_uint();
    return *this;
  }
    
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator <<= ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) {
    return *this <<= (op2.operator ap_int_base<_AP_W2, false>());
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator >>= ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) {
    return *this >>= (op2.operator ap_int_base<_AP_W2, false>());
  }

  /* Comparisons.
     ----------------------------------------------------------------
  */
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator == ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V == op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator != ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return !(Base::V == op2.V);
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator < ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V < op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator >= ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V >= op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator > ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V > op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator <= ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V <= op2.V;
  }
    

  /* Bit and Part Select
     ----------------------------------------------------------------
  */ 
  INLINE ap_range_ref<_AP_W,_AP_S>
  range (int Hi, int Lo) {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(this, Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  operator () (int Hi, int Lo) {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(this, Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  range (int Hi, int Lo) const {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(const_cast<ap_int_base*>(this), Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  operator () (int Hi, int Lo) const {
    return this->range(Hi, Lo);
  }

#if 0
  template<int Hi, int Lo>
  INLINE ap_int_base<Hi-Lo+1, false> slice() const {
    AP_ASSERT(Hi >= Lo && Hi < _AP_W && Lo < _AP_W, "Out of bounds in slice()");
    ap_int_base<Hi-Lo+1, false> tmp ;
    tmp.V = _ssdm_op_get_range(Base::V, Lo, Hi);
    return tmp;
  }
    
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] ( unsigned int uindex) {
    AP_ASSERT(uindex < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, uindex );
    return bvh;
  } 
#endif
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] (int index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index );
    return bvh;
  } 
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] (const ap_int_base<_AP_W2,_AP_S2> &index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index.to_int() );
    return bvh;
  }

  INLINE bool operator [] (int index) const {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), index);
    return br.to_bool();
  }    
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator [] (const ap_int_base<_AP_W2,_AP_S2>& index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), 
			       index.to_int());
    return br.to_bool();
  } 

  INLINE ap_bit_ref<_AP_W,_AP_S> bit (int index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index );
    return bvh;
  } 
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref<_AP_W,_AP_S> bit (const ap_int_base<_AP_W2,_AP_S2> &index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index.to_int() );
    return bvh;
  }

  INLINE bool bit (int index) const {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), index);
    return br.to_bool();
  }    
  template<int _AP_W2, bool _AP_S2>
  INLINE bool bit (const ap_int_base<_AP_W2,_AP_S2>& index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br = bit(index);
    return br.to_bool();
  }    
  
#if 0
  template<typename _AP_T>
  INLINE bool operator [] (_AP_T index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br =operator [] (index);
    return br.to_bool();
  }    
#endif
  template <int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<_AP_W,ap_int_base,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > concat(const ap_int_base<_AP_W2,_AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
										  const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<_AP_W,ap_int_base,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > concat(ap_int_base<_AP_W2,_AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> > 
  operator, (const ap_range_ref<_AP_W2, _AP_S2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, 
      ap_range_ref<_AP_W2, _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
				     const_cast< ap_range_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> > 
  operator, (ap_range_ref<_AP_W2, _AP_S2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, 
      ap_range_ref<_AP_W2, _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (const ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(*this,
		const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (ap_int_base<_AP_W2, _AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
		a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (const ap_int_base<_AP_W2, _AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
		const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2, _AP_S2> > 
  operator, (const ap_bit_ref<_AP_W2, _AP_S2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2,
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this), const_cast<ap_bit_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2, _AP_S2> > 
  operator, (ap_bit_ref<_AP_W2, _AP_S2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2,
      _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >
  operator, (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, 
      ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						      const_cast<ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& >(a2));
  }

  template <int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >
  operator, (ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, 
      ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(*this, a2);
  }

  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						const_cast<af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, a2);
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						const_cast<af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
 
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, a2);
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator & (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this & a2.get();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator | (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this | a2.get();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator ^ (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this ^ a2.get();
  }

  template <int _AP_W3>
  INLINE void set(const ap_int_base<_AP_W3, false>& val) {
    Base::V = val.V;
  }

  /* Reduce operations.
     ----------------------------------------------------------------
  */
  INLINE bool and_reduce() {
    return _ssdm_op_reduce(and, Base::V);
  }
  INLINE bool nand_reduce() {
    return _ssdm_op_reduce(nand, Base::V);
  }
  INLINE bool or_reduce() {
    return _ssdm_op_reduce(or, Base::V);
  }
  INLINE bool nor_reduce() {
    return !(_ssdm_op_reduce(or, Base::V));
  }
  INLINE bool xor_reduce() {
    return _ssdm_op_reduce(xor, Base::V);        
  }
  INLINE bool xnor_reduce() {
    return !(_ssdm_op_reduce(xor, Base::V)); 
  }

  INLINE bool and_reduce() const {
    return _ssdm_op_reduce(and, Base::V);
  }
  INLINE bool nand_reduce() const {
    return _ssdm_op_reduce(nand, Base::V);
  }
  INLINE bool or_reduce() const {
    return _ssdm_op_reduce(or, Base::V);
  }
  INLINE bool nor_reduce() const {
    return !(_ssdm_op_reduce(or, Base::V));
  }
  INLINE bool xor_reduce() const {
    return _ssdm_op_reduce(xor, Base::V);        
  }
  INLINE bool xnor_reduce() const {
    return !(_ssdm_op_reduce(xor, Base::V)); 
  }

  /* Output as a string.
     ----------------------------------------------------------------
  */
  void to_string(char* str, int len, BaseMode mode, bool sign = false) const {
    for (int i = 0; i <= len; ++i) str[i] = '\0';
    if (mode == AP_BIN) {
      int size = AP_MIN(_AP_W, len);
      for (int bit = size; bit > 0; --bit) {
	if (_ssdm_op_get_bit(Base::V, bit-1)) str[size-bit] = '1';
	else str[size-bit] = '0';
      }
    } /*else if (mode == AP_HEX) {
	typeof(Base::V) tmpV = Base::V;
	int idx = 0;
	int size = AP_MIN((_AP_W+3)/4, len);
	while (idx < size) {
	char hexb = tmpV & 0xF;
	if (hexb > 9) hexb = hexb - 10 + 'a';
	else hexb += '0';
	str[size-1-idx] = hexb;
	tmpV >> 4;
	idx ++;
	}
	} */ else if (mode == AP_OCT || mode == AP_DEC) {
      AP_ASSERT(0, "unimplemented base mode");
    } else {
      AP_ASSERT(0, "unsupported base mode");
    }
  }

  INLINE char* to_string(BaseMode mode, bool sign=false) const {
    return 0;
  }
   
  INLINE char* to_string(signed char mode, bool sign=false) const {
    return to_string(BaseMode(mode), sign);
  }
};
template<int _AP_W, bool _AP_S>
struct ap_int_base<_AP_W, _AP_S, false> : public ssdm_int<_AP_W,_AP_S> { 

public:
  typedef ssdm_int<_AP_W, _AP_S> Base;
  typedef typename retval<8, _AP_S>::Type RetType;
  static const int width = _AP_W;


  template<int _AP_W2, bool _AP_S2>
  struct RType {
    enum {
      mult_w = _AP_W+_AP_W2, 
      mult_s = _AP_S||_AP_S2,
      plus_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2))+1,
      plus_s = _AP_S||_AP_S2,
      minus_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2))+1,
      minus_s = true,
      div_w = _AP_W+_AP_S2,
      div_s = _AP_S||_AP_S2,
      mod_w = AP_MIN(_AP_W,_AP_W2+(!_AP_S2&&_AP_S)), 
      mod_s = _AP_S,
      logic_w = AP_MAX(_AP_W+(_AP_S2&&!_AP_S),_AP_W2+(_AP_S&&!_AP_S2)),
      logic_s = _AP_S||_AP_S2
    };

    typedef ap_int_base<mult_w, mult_s> mult;
    typedef ap_int_base<plus_w, plus_s> plus;
    typedef ap_int_base<minus_w, minus_s> minus;
    typedef ap_int_base<logic_w, logic_s> logic;
    typedef ap_int_base<div_w, div_s> div;
    typedef ap_int_base<mod_w, mod_s> mod;
    typedef ap_int_base<_AP_W, _AP_S> arg1;
    typedef bool reduce;
  };
    
  /* Constructors.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base() {
    /*
      #ifdef __SC_COMPATIBLE__
      Base::V = 0;
      #endif
    */
  }
    
  //INLINE ap_int_base(const ap_int_base& op) { Base::V = op.V; }
  //INLINE ap_int_base(const volatile ap_int_base& op) { Base::V = op.V; }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_int_base<_AP_W2,_AP_S2> &op) { Base::V = op.V; }
    
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const volatile ap_int_base<_AP_W2,_AP_S2> &op) { Base::V = op.V; }

  /* For C++ basic data types.*/
#if 0
  template<typename _AP_T>
  INLINE ap_int_base(_AP_T v) { Base::V = v; }
#else
  INLINE explicit ap_int_base(bool op) { Base::V = op; }
  INLINE explicit ap_int_base(signed char op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned char op) { Base::V = op; }
  INLINE explicit ap_int_base(short op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned short op) { Base::V = op; }
  INLINE explicit ap_int_base(int op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned int op) { Base::V = op; }
  INLINE explicit ap_int_base(long op) { Base::V = op; }
  INLINE explicit ap_int_base(unsigned long op) { Base::V = op; }
  INLINE explicit ap_int_base(ap_slong op) { Base::V = op; }
  INLINE explicit ap_int_base(ap_ulong op) { Base::V = op; }
  INLINE explicit ap_int_base(half op) { Base::V = op; }
  INLINE explicit ap_int_base(float op) { Base::V = op; }
  INLINE explicit ap_int_base(double op) { Base::V = op; }
#endif 
  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base(const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op) {
    Base::V = op.to_ap_int_base().V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_range_ref<_AP_W2,_AP_S2>& ref) {
    Base::V = ref.operator ap_int_base<_AP_W2, false>().V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base(const ap_bit_ref<_AP_W2,_AP_S2>& ref) {
    Base::V = ref.operator bool();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base(const ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>& ref) {
    const ap_int_base<ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>::_AP_WR,false> tmp = ref.get();
    Base::V = tmp.V;
  }

  /* This constructor is not usable yet, because the second parameter of 
     __builtin_bit_from_string(...)
     is required to be a constant C string.
  */
  INLINE ap_int_base(const char* str) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), 10, 
		      _AP_W, _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
  }

  INLINE ap_int_base(const char* str, signed char radix) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), radix,
		      _AP_W,  _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
  }
    
  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_int_base(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, 
		     _AP_Q2, _AP_O2, _AP_N2> &val) {
    Base::V = (val.operator ap_int_base<_AP_W2, false> ()).V;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, 
	   ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
  INLINE ap_int_base(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, 
		     _AP_Q2, _AP_O2, _AP_N2> &val) {
    Base::V = val.operator bool ();
  }
  
  // FIXME: This is only for internal use. Clang checker.
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_int_base(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val) {}   
  #endif
  
  INLINE ap_int_base read() volatile {
    AP_DEBUG(printf("call read %d\n", Base::V);); 
    ap_int_base ret;
    ret.V = Base::V;
    return ret;
  }
  INLINE void write(const ap_int_base<_AP_W, _AP_S>& op2) volatile {
    AP_DEBUG(printf("call write %d\n", op2.V););
    Base::V = op2.V;
  }

  /* Another form of "write".*/
#if 1
  template<int _AP_W2, bool _AP_S2>
  INLINE void operator = (const volatile ap_int_base<_AP_W2,_AP_S2>& op2) volatile {
    Base::V = op2.V;
  }
	
  INLINE void operator = (const volatile ap_int_base<_AP_W, _AP_S>& op2) volatile {
    Base::V = op2.V;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE void operator = (const ap_int_base<_AP_W2,_AP_S2>& op2) volatile {
    Base::V = op2.V;
  }
	
  INLINE void  operator = (const ap_int_base<_AP_W, _AP_S>& op2) volatile {
    Base::V = op2.V;
  }
#endif
#if 0
  template<typename _AP_T2>
  INLINE void operator = (const _AP_T2& op2) volatile {
    Base::V = ap_int_base(op2).V;
  }
#endif
#if 1
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const volatile ap_int_base<_AP_W2,_AP_S2>& op2) {
    Base::V = op2.V;
    return *this;
  }
	
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_int_base<_AP_W2,_AP_S2>& op2)  {
    Base::V = op2.V;
    return *this;
  }

  INLINE ap_int_base& operator = (const volatile ap_int_base<_AP_W,_AP_S>& op2) {
    Base::V = op2.V;
    return *this;
  }
	
  INLINE ap_int_base& operator = (const ap_int_base<_AP_W,_AP_S>& op2)  {
    Base::V = op2.V;
    return *this;
  }
  INLINE ap_int_base& operator = (const char* str) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), 10,
		      _AP_W,  _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
    return *this;
  }

  INLINE ap_int_base& set(const char* str, signed char radix) {
    typeof(Base::V) Result;
    _ssdm_string2bits((void*)(&Result), (const char*)(str), radix,
		      _AP_W, _AP_S, AP_TRN, AP_WRAP, 0, _AP_C99);
    Base::V = Result;
    return *this;
  }
        
#endif
#define ASSIGN_FROM_C_TYPE(_AP_T2)		\
  INLINE ap_int_base& operator = (_AP_T2 op) {	\
    Base::V = op;				\
    return *this;				\
  }
  ASSIGN_FROM_C_TYPE(char)
  ASSIGN_FROM_C_TYPE(unsigned char)
  ASSIGN_FROM_C_TYPE(short)
  ASSIGN_FROM_C_TYPE(unsigned short)
  ASSIGN_FROM_C_TYPE(int)
  ASSIGN_FROM_C_TYPE(unsigned int)
  ASSIGN_FROM_C_TYPE(ap_slong)
  ASSIGN_FROM_C_TYPE(ap_ulong)
#undef ASSIGN_FROM_C_TYPE

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_bit_ref<_AP_W2, _AP_S2>& op2) {
    Base::V = (bool) op2;
    return *this;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator = (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
    Base::V = (ap_int_base<_AP_W2, false>(op2)).V;
    return *this;
  }
 
  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base& operator = (const ap_concat_ref<_AP_W2,_AP_T2,_AP_W3,_AP_T3>& op2) {
    Base::V = op2.get().V;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = op.to_ap_int_base().V;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = (bool) op;
    return *this;
  }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int_base& operator = (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    Base::V = ((const ap_int_base<_AP_W2, false>)(op)).V;
    return *this;
  }

  INLINE operator RetType() const { return (RetType)(Base::V); }


  /* Explicit conversions to C interger types.
     ----------------------------------------------------------------
  */
  INLINE bool to_bool() const {return (bool)(Base::V);}
  INLINE bool to_uchar() const {return (unsigned char)(Base::V);}
  INLINE bool to_char() const {return (char)(Base::V);}
  INLINE bool to_ushort() const {return (unsigned short)(Base::V);}
  INLINE bool to_short() const {return (short)(Base::V);}
  INLINE int to_int() const { return (int)(Base::V); }
  INLINE unsigned to_uint() const { return (unsigned)(Base::V); }
  INLINE long to_long() const { return (long)(Base::V); }
  INLINE unsigned long to_ulong() const { return (unsigned long)(Base::V); }
  INLINE ap_slong to_int64() const { return (ap_slong)(Base::V); }
  INLINE ap_ulong to_uint64() const { return (ap_ulong)(Base::V); }
  INLINE double to_double() const { return (double)(Base::V); }
#if 0
  INLINE operator char() const { return (char)(Base::V); }
  INLINE operator unsigned char() const { return (unsigned char)(Base::V); }
  INLINE operator short() const { return (short)(Base::V); }
  INLINE operator unsigned short() const { return (unsigned short)(Base::V); }
  INLINE operator int() const { return (int)(Base::V); }
  INLINE operator unsigned int () const { return (unsigned)(Base::V); }
  INLINE operator long () const { return (long)(Base::V); }
  INLINE operator unsigned long () const { return (unsigned long)(Base::V); }
  INLINE operator long long () const { return (ap_slong)(Base::V); }
  INLINE operator unsigned long long () const { return (ap_ulong)(Base::V); }
#endif
  INLINE int length() const { return _AP_W; }
  INLINE int length() const volatile { return _AP_W; }

  /*INLINE operator ap_ulong () { return (ap_ulong)(Base::V); }*/

  /*Reverse the contents of ap_int_base instance. I.e. LSB becomes MSB and vise versa*/
  INLINE ap_int_base& reverse () {
    Base::V = _ssdm_op_get_range(Base::V, _AP_W - 1, 0);    
    return *this;
  }

  /*Return true if the value of ap_int_base instance is zero*/
  INLINE bool iszero () const {
    return Base::V == 0 ; 
  }     
    
  /*Return true if the value of ap_int_base instance is zero*/
  INLINE bool is_zero () const {
    return Base::V == 0 ; 
  }
     
  /* x < 0 */ 
  INLINE bool sign () const {
    if (_AP_S && _ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, _AP_W - 1))
      return true;
    else
      return false;
  }
  
  /* x[i] = 0 */
  INLINE void clear(int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, 0); 
  }

  /* x[i] = !x[i]*/
  INLINE void invert(int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    bool val =  _ssdm_op_get_bit(Base::V, i);
    if (val) Base::V = _ssdm_op_set_bit(Base::V, i, 0);
    else     Base::V = _ssdm_op_set_bit(Base::V, i, 1);
  } 

      
  INLINE bool test (int i) const { 
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    return _ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, i);
  }
   
  //Set the ith bit into 1
  INLINE void set (int i) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, 1); 
  }

  //Set the ith bit into v
  INLINE void set (int i, bool v) {
    AP_ASSERT( i >= 0 && i < _AP_W, "position out of range");
    Base::V = _ssdm_op_set_bit(Base::V, i, v);
  }
    
  //This is used for sc_lv and sc_bv, which is implemented by sc_uint 
  //Rotate an ap_int_base object n places to the left
  INLINE void lrotate(int n) {
    AP_ASSERT(n >= 0 && n < _AP_W, "shift value out of range");
    typeof(Base::V) l_p = Base::V << n;
    typeof(Base::V) r_p = Base::V >> (_AP_W - n);
    Base::V = l_p | r_p;
  }

  //This is used for sc_lv and sc_bv, which is implemented by sc_uint 
  //Rotate an ap_int_base object n places to the right 
  INLINE void rrotate(int n) {
    AP_ASSERT(n >= 0 && n < _AP_W, "shift value out of range");
    typeof(Base::V) l_p = Base::V << (_AP_W - n);
    typeof(Base::V) r_p = Base::V >> n;
    Base::V = l_p | r_p;
  }
 
  //Set the ith bit into v 
  INLINE void set_bit (int i, bool v) {
    Base::V = _ssdm_op_set_bit(Base::V, i, v);
  }

  //Get the value of ith bit
  INLINE bool get_bit (int i) const {
    return (bool)_ssdm_op_get_bit(const_cast<ap_int_base*>(this)->V, i);
  }

  //complements every bit 
  INLINE void b_not() {
    Base::V = ~Base::V; 
  }

  // Count the number of zeros from the most significant bit 
  // to the first one bit. Note this is only for ap_fixed_base whose 
  // _AP_W <= 64, otherwise will incur assertion.
  INLINE int countLeadingZeros() {
    if (_AP_W <= 32) {
      ap_int_base<32, false> t(-1ULL);
      t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
      return __builtin_ctz(t.V);
    } else if (_AP_W <= 64) {
      ap_int_base<64, false> t(-1ULL);
      t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
      return __builtin_ctzll(t.V);
    } else {
      enum { __N = (_AP_W+63)/64 };
      int NZeros = 0;
      unsigned i = 0;
      bool hitNonZero = false;
      for (i=0; i<__N-1; ++i) {
	ap_int_base<64, false> t;
	t.range(0, 63) = this->range(_AP_W - i*64 - 64, _AP_W - i*64 - 1);
	NZeros += hitNonZero?0:__builtin_clzll(t.V);
	hitNonZero |= (t.to_uint64() != 0);
      }
      if (!hitNonZero) {
	ap_int_base<64, false> t(-1ULL);
	t.range(63-(_AP_W-1)%64, 63) = this->range(0, (_AP_W-1)%64);
	NZeros += __builtin_clzll(t.V);
      }
      return NZeros;
    }
  }

#define OP_ASSIGN_AP(Sym)						\
  template<int _AP_W2, bool _AP_S2>					\
  INLINE ap_int_base& operator Sym ( const ap_int_base<_AP_W2,_AP_S2> &op2) { \
    AP_DEBUG(printf("call assign op %s\n", #Sym););			\
    Base::V Sym op2.V;							\
    return *this;							\
  }

  /* Arithmetic assign.
     ----------------------------------------------------------------
  */
  OP_ASSIGN_AP(*=)
  OP_ASSIGN_AP(+=)
  OP_ASSIGN_AP(-=)
  OP_ASSIGN_AP(/=)
  OP_ASSIGN_AP(%=)
        
  /* Bitwise assign: and, or, xor.
     ----------------------------------------------------------------
  */
  OP_ASSIGN_AP(&=)
  OP_ASSIGN_AP(|=)
  OP_ASSIGN_AP(^=)

    
  /* Prefix increment, decrement.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base& operator ++() {
    operator+=((ap_int_base<1,false>) 1);
    return *this;
  }
  INLINE ap_int_base& operator --() {
    operator-=((ap_int_base<1,false>) 1);
    return *this;
  }
    
  /* Postfix increment, decrement
     ----------------------------------------------------------------
  */
  INLINE const ap_int_base operator ++(int) {
    ap_int_base t = *this;
    operator+=((ap_int_base<1,false>) 1);
    return t;
  }
  INLINE const ap_int_base operator --(int) {
    ap_int_base t = *this;
    operator-=((ap_int_base<1,false>) 1);
    return t;
  }
    
  /* Unary arithmetic.
     ----------------------------------------------------------------
  */
  INLINE ap_int_base operator +() const{
    return *this;
  }

  INLINE typename RType<1,false>::minus operator -() const {
    return ((ap_int_base<1,false>) 0) - *this;
  }
    
  /* Not (!)
     ----------------------------------------------------------------
  */
  INLINE bool operator ! () const {
    return Base::V == 0; 
  }

  /* Bitwise (arithmetic) unary: complement
     ----------------------------------------------------------------
  */
  INLINE ap_int_base<_AP_W+!_AP_S, true> operator ~() const {
    ap_int_base<_AP_W+!_AP_S, true> r;
    r.V = ~Base::V;
    return r;
  }
    
  /* Shift (result constrained by left operand).
     ----------------------------------------------------------------
  */
  template<int _AP_W2>
  INLINE ap_int_base operator << ( const ap_int_base<_AP_W2,true> &op2 ) const {
    bool isNeg = op2[_AP_W2 - 1]; 
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator >> (sh);
    } else 
      return operator << (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base operator << ( const ap_int_base<_AP_W2,false> &op2 ) const {
    ap_int_base r ;
    r.V = Base::V << op2.to_uint();
    return r;
  }
  template<int _AP_W2>
  INLINE ap_int_base operator >> ( const ap_int_base<_AP_W2,true> &op2 ) const {
    bool isNeg = op2[_AP_W2 - 1];
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator << (sh);
    } 
    return operator >> (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base operator >> ( const ap_int_base<_AP_W2,false> &op2 ) const {
    ap_int_base r;
    r.V = Base::V >> op2.to_uint();
    return r;
  }

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base operator << ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) const {
    return *this << (op2.operator ap_int_base<_AP_W2, false>());
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base operator >> ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) const {
    return *this >> (op2.operator ap_int_base<_AP_W2, false>());
  }

  /* Shift assign
     ----------------------------------------------------------------
  */
  template<int _AP_W2>
  INLINE ap_int_base& operator <<= ( const ap_int_base<_AP_W2,true> &op2 ) {
    bool isNeg = op2[_AP_W2 - 1]; 
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      return operator >>= (sh);
    } else 
      return operator <<= (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator <<= ( const ap_int_base<_AP_W2,false> &op2 ) {
    Base::V <<= op2.to_uint();
    return *this;
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator >>= ( const ap_int_base<_AP_W2,true> &op2 ) {
    bool isNeg = op2[_AP_W2 - 1];
    ap_int_base<_AP_W2, false> sh = op2;
    if (isNeg) {
      sh = -op2;
      operator <<= (sh);
    } 
    return operator >>= (sh);
  }
  template<int _AP_W2>
  INLINE ap_int_base& operator >>= ( const ap_int_base<_AP_W2,false> &op2 ) {
    Base::V >>= op2.to_uint();
    return *this;
  }
    
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator <<= ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) {
    return *this <<= (op2.operator ap_int_base<_AP_W2, false>());
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int_base& operator >>= ( const ap_range_ref<_AP_W2,_AP_S2>& op2 ) {
    return *this >>= (op2.operator ap_int_base<_AP_W2, false>());
  }

  /* Comparisons.
     ----------------------------------------------------------------
  */
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator == ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V == op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator != ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return !(Base::V == op2.V);
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator < ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V < op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator >= ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V >= op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator > ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V > op2.V;
  }
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator <= ( const ap_int_base<_AP_W2,_AP_S2> &op2) const {
    return Base::V <= op2.V;
  }
    

  /* Bit and Part Select
     ----------------------------------------------------------------
  */ 
  INLINE ap_range_ref<_AP_W,_AP_S>
  range (int Hi, int Lo) {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(this, Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  operator () (int Hi, int Lo) {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(this, Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  range (int Hi, int Lo) const {
    AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
    return ap_range_ref<_AP_W,_AP_S>(const_cast<ap_int_base*>(this), Hi, Lo);
  }

  INLINE ap_range_ref<_AP_W,_AP_S>
  operator () (int Hi, int Lo) const {
    return this->range(Hi, Lo);
  }

    
#if 0
  template<int Hi, int Lo>
  INLINE ap_int_base<Hi-Lo+1, false> slice() const {
    AP_ASSERT(Hi >= Lo && Hi < _AP_W && Lo < _AP_W, "Out of bounds in slice()");
    ap_int_base<Hi-Lo+1, false> tmp ;
    tmp.V = _ssdm_op_get_range(Base::V, Lo, Hi);
    return tmp;
  }
    
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] ( unsigned int uindex) {
    AP_ASSERT(uindex < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, uindex );
    return bvh;
  } 
#endif
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] (int index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index );
    return bvh;
  } 
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref<_AP_W,_AP_S> operator [] (const ap_int_base<_AP_W2,_AP_S2> &index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index.to_int() );
    return bvh;
  }

  INLINE bool operator [] (int index) const {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), index);
    return br.to_bool();
  }    
  template<int _AP_W2, bool _AP_S2>
  INLINE bool operator [] (const ap_int_base<_AP_W2,_AP_S2>& index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), 
			       index.to_int());
    return br.to_bool();
  } 

  INLINE ap_bit_ref<_AP_W,_AP_S> bit (int index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index );
    return bvh;
  } 
  template<int _AP_W2, bool _AP_S2>
  INLINE ap_bit_ref<_AP_W,_AP_S> bit (const ap_int_base<_AP_W2,_AP_S2> &index) {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> bvh( this, index.to_int() );
    return bvh;
  }

  INLINE bool bit (int index) const {
    AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br(const_cast<ap_int_base*>(this), index);
    return br.to_bool();
  }    
  template<int _AP_W2, bool _AP_S2>
  INLINE bool bit (const ap_int_base<_AP_W2,_AP_S2>& index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br = bit(index);
    return br.to_bool();
  }    
  
#if 0
  template<typename _AP_T>
  INLINE bool operator [] (_AP_T index) const {
    AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
    ap_bit_ref<_AP_W,_AP_S> br =operator [] (index);
    return br.to_bool();
  }    
#endif
  template <int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<_AP_W,ap_int_base,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > concat(const ap_int_base<_AP_W2,_AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
										  const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE ap_concat_ref<_AP_W,ap_int_base,_AP_W2,ap_int_base<_AP_W2,_AP_S2> > concat(ap_int_base<_AP_W2,_AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2,_AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> > 
  operator, (const ap_range_ref<_AP_W2, _AP_S2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, 
      ap_range_ref<_AP_W2, _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
				     const_cast< ap_range_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> > 
  operator, (ap_range_ref<_AP_W2, _AP_S2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, 
      ap_range_ref<_AP_W2, _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (const ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(*this,
		const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (ap_int_base<_AP_W2, _AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
		a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (const ap_int_base<_AP_W2, _AP_S2>& a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, 
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
		const_cast<ap_int_base<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> > 
  operator, (ap_int_base<_AP_W2, _AP_S2>& a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, ap_int_base<_AP_W2, _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2, _AP_S2> > 
  operator, (const ap_bit_ref<_AP_W2, _AP_S2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2,
      _AP_S2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this), const_cast<ap_bit_ref<_AP_W2, _AP_S2>& >(a2));
  }

  template <int _AP_W2, bool _AP_S2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2, _AP_S2> > 
  operator, (ap_bit_ref<_AP_W2, _AP_S2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, 1, ap_bit_ref<_AP_W2,
      _AP_S2> >(*this, a2);
  }

  template <int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >
  operator, (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, 
      ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						      const_cast<ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& >(a2));
  }

  template <int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >
  operator, (ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2+_AP_W3, 
      ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(*this, a2);
  }

  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						const_cast<af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, _AP_W2, af_range_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, a2);
  }
    
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) const {
    return ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(const_cast<ap_int_base<_AP_W, _AP_S>& >(*this),
						const_cast<af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& >(a2));
  }
 
  template <int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
  INLINE
  ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
  operator, (af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
	     _AP_O2, _AP_N2> &a2) {
    return ap_concat_ref<_AP_W, ap_int_base, 1, af_bit_ref<_AP_W2,
      _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, a2);
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator & (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this & a2.get();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator | (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this | a2.get();
  }

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int_base<AP_MAX(_AP_W2+_AP_W3,_AP_W), _AP_S>
  operator ^ (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& a2) {
    return *this ^ a2.get();
  }

  template <int _AP_W3>
  INLINE void set(const ap_int_base<_AP_W3, false>& val) {
    Base::V = val.V;
  }

  /* Reduce operations.
     ----------------------------------------------------------------
  */
  INLINE bool and_reduce() {
    return _ssdm_op_reduce(and, Base::V);
  }
  INLINE bool nand_reduce() {
    return _ssdm_op_reduce(nand, Base::V);
  }
  INLINE bool or_reduce() {
    return _ssdm_op_reduce(or, Base::V);
  }
  INLINE bool nor_reduce() {
    return !(_ssdm_op_reduce(or, Base::V));
  }
  INLINE bool xor_reduce() {
    return _ssdm_op_reduce(xor, Base::V);        
  }
  INLINE bool xnor_reduce() {
    return !(_ssdm_op_reduce(xor, Base::V)); 
  }

  INLINE bool and_reduce() const {
    return _ssdm_op_reduce(and, Base::V);
  }
  INLINE bool nand_reduce() const {
    return _ssdm_op_reduce(nand, Base::V);
  }
  INLINE bool or_reduce() const {
    return _ssdm_op_reduce(or, Base::V);
  }
  INLINE bool nor_reduce() const {
    return !(_ssdm_op_reduce(or, Base::V));
  }
  INLINE bool xor_reduce() const {
    return _ssdm_op_reduce(xor, Base::V);        
  }
  INLINE bool xnor_reduce() const {
    return !(_ssdm_op_reduce(xor, Base::V)); 
  }

  /* Output as a string.
     ----------------------------------------------------------------
  */
  void to_string(char* str, int len, BaseMode mode, bool sign = false) const {
    for (int i = 0; i <= len; ++i) str[i] = '\0';
    if (mode == AP_BIN) {
      int size = AP_MIN(_AP_W, len);
      for (int bit = size; bit > 0; --bit) {
	if (_ssdm_op_get_bit(Base::V, bit-1)) str[size-bit] = '1';
	else str[size-bit] = '0';
      }
    } /*else if (mode == AP_HEX) {
	typeof(Base::V) tmpV = Base::V;
	int idx = 0;
	int size = AP_MIN((_AP_W+3)/4, len);
	while (idx < size) {
	char hexb = tmpV & 0xF;
	if (hexb > 9) hexb = hexb - 10 + 'a';
	else hexb += '0';
	str[size-1-idx] = hexb;
	tmpV >> 4;
	idx ++;
	}
	} */ else if (mode == AP_OCT || mode == AP_DEC) {
      AP_ASSERT(0, "unimplemented base mode");
    } else {
      AP_ASSERT(0, "unsupported base mode");
    }
  }

  INLINE char* to_string(BaseMode mode, bool sign=false) const {
    return 0;
  }
   
  INLINE char* to_string(signed char mode, bool sign=false) const {
    return to_string(BaseMode(mode), sign);
  }
};



       

/* Output streaming.
   ----------------------------------------------------------------
*/

#if !defined(AP_AUTOCC)
template<int _AP_W, bool _AP_S>
INLINE std::ostream& operator << (std::ostream &os, const ap_int_base<_AP_W,_AP_S> &x) {
  //os << x.to_string(AP_DEC);
  return os;
}

/* Input streaming.
   ......................................................
*/
template<int _AP_W, bool _AP_S>
INLINE std::istream& operator >> (std::istream& in, ap_int_base<_AP_W,_AP_S> &op) {
#if 0
  std::string str;
  in >> str;
  op = ap_int_base<_AP_W, _AP_S>(str.c_str());
#endif
  return in;
}

template<int _AP_W, bool _AP_S>
INLINE std::ostream& operator << (std::ostream &os, const ap_range_ref<_AP_W,_AP_S> &x) {
  //os << x.to_string(AP_DEC);
  return os;
}

/* Input streaming.
   ......................................................
*/
template<int _AP_W, bool _AP_S>
INLINE std::istream& operator >> (std::istream& in, ap_range_ref<_AP_W,_AP_S> &op) {
#if 0
  std::string str;
  in >> str;
  op = ap_int_base<_AP_W, _AP_S>(str.c_str());
#endif
  return in;
}

#endif

/*Binary Arithmetic. 
  ----------------------------------------------------------------
*/
#define OP_BIN_AP(Sym, Rty)						\
  template<int _AP_W, bool _AP_S, int _AP_W2, bool _AP_S2>		\
  INLINE								\
  typename ap_int_base<_AP_W, _AP_S>::template RType<_AP_W2,_AP_S2>::Rty \
  operator Sym (const ap_int_base<_AP_W,_AP_S> &op,			\
		const ap_int_base<_AP_W2,_AP_S2> &op2) {		\
    AP_DEBUG(printf("call operator %s %s\n", #Sym, #Rty););		\
    typename ap_int_base<_AP_W, _AP_S>::template  RType<_AP_W2,_AP_S2>::Rty lhs(op); \
    typename ap_int_base<_AP_W, _AP_S>::template  RType<_AP_W2,_AP_S2>::Rty rhs(op2); \
    typename ap_int_base<_AP_W, _AP_S>::template  RType<_AP_W2,_AP_S2>::Rty r ; \
    r.V = lhs.V Sym rhs.V;						\
    return r;								\
  }

#define OP_BIN_AP2(Sym, Rty)						\
  template<int _AP_W, bool _AP_S, int _AP_W2, bool _AP_S2>		\
  INLINE								\
  typename ap_int_base<_AP_W, _AP_S>::template RType<_AP_W2,_AP_S2>::Rty \
  operator Sym ( const ap_int_base<_AP_W,_AP_S> &op,			\
		 const ap_int_base<_AP_W2,_AP_S2> &op2) {		\
    AP_DEBUG(printf("call operator %s %s\n", #Sym, #Rty););		\
    typename ap_int_base<_AP_W, _AP_S>::template RType<_AP_W2,_AP_S2>::Rty r ; \
    r.V = op.V Sym op2.V;						\
    return r;								\
  }

OP_BIN_AP(*, mult)
OP_BIN_AP(+, plus)
OP_BIN_AP(-, minus)
OP_BIN_AP2(/, div)
OP_BIN_AP2(%, mod)

/* Bitwise and, or, xor.
   ----------------------------------------------------------------
*/
OP_BIN_AP(&, logic)
OP_BIN_AP(|, logic)
OP_BIN_AP(^, logic)


//FIXME

//char a[100];
//char* ptr = a;
//ap_int<2> n = 3;
//char* ptr2 = ptr + n*2;
//avoid ambiguous errors
#define OP_BIN_MIX_PTR(BIN_OP)						\
  template<typename PTR_TYPE, int _AP_W, bool _AP_S>			\
  INLINE PTR_TYPE*							\
  operator BIN_OP (PTR_TYPE* i_op, const ap_int_base<_AP_W,_AP_S> &op) { \
    typename ap_int_base<_AP_W,_AP_S>::RetType op2 = op;		\
    return i_op BIN_OP op2;						\
  }									\
  template<typename PTR_TYPE, int _AP_W, bool _AP_S>			\
  INLINE PTR_TYPE*							\
  operator BIN_OP (const ap_int_base<_AP_W,_AP_S> &op, PTR_TYPE* i_op) { \
    typename ap_int_base<_AP_W,_AP_S>::RetType op2 = op;		\
    return op2 BIN_OP i_op;						\
  } 

OP_BIN_MIX_PTR(+)
OP_BIN_MIX_PTR(-)

//float OP ap_int
//when ap_int<wa>'s width > 64, then trunc ap_int<w> to ap_int<64>
#define OP_BIN_MIX_FLOAT(BIN_OP, C_TYPE)				\
  template<int _AP_W, bool _AP_S>					\
  INLINE C_TYPE								\
  operator BIN_OP (C_TYPE i_op, const ap_int_base<_AP_W,_AP_S> &op) {	\
    typename ap_int_base<_AP_W,_AP_S>::RetType op2 = op;		\
    return i_op BIN_OP op2;						\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE C_TYPE								\
  operator BIN_OP (const ap_int_base<_AP_W,_AP_S> &op, C_TYPE i_op) {	\
    typename ap_int_base<_AP_W,_AP_S>::RetType op2 = op;		\
    return i_op BIN_OP op2;						\
  } 

#define OPS_MIX_FLOAT(C_TYPE)			\
  OP_BIN_MIX_FLOAT(*, C_TYPE)			\
  OP_BIN_MIX_FLOAT(/, C_TYPE)			\
  OP_BIN_MIX_FLOAT(+, C_TYPE)			\
  OP_BIN_MIX_FLOAT(-, C_TYPE)

OPS_MIX_FLOAT(half)
OPS_MIX_FLOAT(float)
OPS_MIX_FLOAT(double)

/* Operators mixing Integers with AP_Int
   ----------------------------------------------------------------
*/
// partially specialize template argument _AP_C in order that:
// for _AP_W > 64, we will explicitly convert operand with native data type
// into corresponding ap_private
// for _AP_W <= 64, we will implicitly convert operand with ap_private into
// (unsigned) long long
#define OP_BIN_MIX_INT(BIN_OP, C_TYPE, _AP_W2, _AP_S2, RTYPE)		\
  template<int _AP_W, bool _AP_S>					\
  INLINE typename ap_int_base<_AP_W,_AP_S>::template RType<_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP (C_TYPE i_op,  const ap_int_base<_AP_W,_AP_S> &op) {  \
    return ap_int_base<_AP_W2,_AP_S2>(i_op) BIN_OP (op);		\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE typename ap_int_base<_AP_W,_AP_S>::template RType<_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_int_base<_AP_W,_AP_S> &op, C_TYPE i_op) {  \
    return op BIN_OP ap_int_base<_AP_W2,_AP_S2>(i_op);			\
  }

#define OPS_BIN_MIX_INT(C_TYPE, _AP_WI, SI)	\
  OP_BIN_MIX_INT(*, C_TYPE, _AP_WI, SI, mult)	\
  OP_BIN_MIX_INT(+, C_TYPE, _AP_WI, SI, plus)	\
  OP_BIN_MIX_INT(-, C_TYPE, _AP_WI, SI, minus)	\
  OP_BIN_MIX_INT(/, C_TYPE, _AP_WI, SI, div)	\
  OP_BIN_MIX_INT(%, C_TYPE, _AP_WI, SI, mod)	\
  OP_BIN_MIX_INT(&, C_TYPE, _AP_WI, SI, logic)	\
  OP_BIN_MIX_INT(|, C_TYPE, _AP_WI, SI, logic)	\
  OP_BIN_MIX_INT(^, C_TYPE, _AP_WI, SI, logic)

OPS_BIN_MIX_INT(bool, 1, false)
OPS_BIN_MIX_INT(char, 8, true)
OPS_BIN_MIX_INT(signed char, 8, true)
OPS_BIN_MIX_INT(unsigned char, 8, false)
OPS_BIN_MIX_INT(short, 16, true)
OPS_BIN_MIX_INT(unsigned short, 16, false)
OPS_BIN_MIX_INT(int, 32, true)
OPS_BIN_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
OPS_BIN_MIX_INT(long, 64, true)
OPS_BIN_MIX_INT(unsigned long, 64, false)
#else
OPS_BIN_MIX_INT(long, 32, true)
OPS_BIN_MIX_INT(unsigned long, 32, false)
#endif
OPS_BIN_MIX_INT(ap_slong, 64, true)
OPS_BIN_MIX_INT(ap_ulong, 64, false)

#define OP_REL_MIX_INT(REL_OP, C_TYPE, _AP_W2, _AP_S2)			\
  template<int _AP_W, bool _AP_S>					\
  INLINE bool operator REL_OP ( C_TYPE i_op, const ap_int_base<_AP_W,_AP_S, false> &op) { \
    return ap_int_base<_AP_W2,_AP_S2>(i_op).operator REL_OP (op);	\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE bool operator REL_OP ( const ap_int_base<_AP_W,_AP_S> &op, C_TYPE op2) { \
    return op.operator REL_OP (ap_int_base<_AP_W2,_AP_S2>(op2));	\
  }

#define OPS_REL_MIX_INT(C_TYPE, _AP_WI, SI)	\
  OP_REL_MIX_INT(==, C_TYPE, _AP_WI, SI)	\
  OP_REL_MIX_INT(!=, C_TYPE, _AP_WI, SI)	\
  OP_REL_MIX_INT(>, C_TYPE, _AP_WI, SI)		\
  OP_REL_MIX_INT(>=, C_TYPE, _AP_WI, SI)	\
  OP_REL_MIX_INT(<, C_TYPE, _AP_WI, SI)		\
  OP_REL_MIX_INT(<=, C_TYPE, _AP_WI, SI) 

OPS_REL_MIX_INT(bool, 1, false)
OPS_REL_MIX_INT(char, 8, true)
OPS_REL_MIX_INT(signed char, 8, true)
OPS_REL_MIX_INT(unsigned char, 8, false)
OPS_REL_MIX_INT(short, 16, true)
OPS_REL_MIX_INT(unsigned short, 16, false)
OPS_REL_MIX_INT(int, 32, true)
OPS_REL_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
OPS_REL_MIX_INT(long, 64, true)
OPS_REL_MIX_INT(unsigned long, 64, false)
#else
OPS_REL_MIX_INT(long, 32, true)
OPS_REL_MIX_INT(unsigned long, 32, false)
#endif
OPS_REL_MIX_INT(ap_slong, 64, true)
OPS_REL_MIX_INT(ap_ulong, 64, false)

#define OP_ASSIGN_MIX_INT(ASSIGN_OP, C_TYPE, _AP_W2, _AP_S2)		\
  template<int _AP_W, bool _AP_S>					\
  INLINE ap_int_base<_AP_W,_AP_S> &operator ASSIGN_OP ( ap_int_base<_AP_W,_AP_S> &op, C_TYPE op2) { \
    return op.operator ASSIGN_OP (ap_int_base<_AP_W2,_AP_S2>(op2));	\
  }

#define OPS_ASSIGN_MIX_INT(C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(+=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(-=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(*=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(/=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(%=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(>>=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(<<=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(&=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(|=, C_TYPE, _AP_WI, SI)	\
  OP_ASSIGN_MIX_INT(^=, C_TYPE, _AP_WI, SI)

OPS_ASSIGN_MIX_INT(bool, 1, false)
OPS_ASSIGN_MIX_INT(char, 8, true)
OPS_ASSIGN_MIX_INT(signed char, 8, true)
OPS_ASSIGN_MIX_INT(unsigned char, 8, false)
OPS_ASSIGN_MIX_INT(short, 16, true)
OPS_ASSIGN_MIX_INT(unsigned short, 16, false)
OPS_ASSIGN_MIX_INT(int, 32, true)
OPS_ASSIGN_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
OPS_ASSIGN_MIX_INT(long, 64, true)
OPS_ASSIGN_MIX_INT(unsigned long, 64, false)
#else
OPS_ASSIGN_MIX_INT(long, 32, true)
OPS_ASSIGN_MIX_INT(unsigned long, 32, false)
#endif
OPS_ASSIGN_MIX_INT(ap_slong, 64, true)
OPS_ASSIGN_MIX_INT(ap_ulong, 64, false)

#define OP_SHIFT_CTYPE(TYPE, _AP_S2)				\
  template <int _AP_W, bool _AP_S>				\
  INLINE ap_int_base<_AP_W, _AP_S>				\
  operator << (const ap_int_base<_AP_W, _AP_S>& op, TYPE op2) {	\
    ap_int_base<_AP_W, _AP_S> r;				\
    if (_AP_S2)							\
      r.V = op2 >= 0 ? (op.V << op2) : (op.V >> (-op2));        \
    else							\
      r.V = op.V << op2;					\
    return r;							\
  }								\
  template <int _AP_W, bool _AP_S>				\
  INLINE ap_int_base<_AP_W, _AP_S>				\
  operator >> (const ap_int_base<_AP_W, _AP_S>& op, TYPE op2) {	\
    ap_int_base<_AP_W, _AP_S> r;				\
    if (_AP_S2)							\
      r.V = op2 >= 0 ? (op.V >> op2) : (op.V << (-op2));        \
    else							\
      r.V = op.V >> op2;					\
    return r;							\
  }

OP_SHIFT_CTYPE(bool, false)
OP_SHIFT_CTYPE(char, true)
OP_SHIFT_CTYPE(signed char, true)
OP_SHIFT_CTYPE(unsigned char, false)
OP_SHIFT_CTYPE(short, true)
OP_SHIFT_CTYPE(unsigned short, false)
OP_SHIFT_CTYPE(int, true)
OP_SHIFT_CTYPE(unsigned int, false)
# if defined __x86_64__
OP_SHIFT_CTYPE(long, true)
OP_SHIFT_CTYPE(unsigned long, false)
#else
OP_SHIFT_CTYPE(long, true)
OP_SHIFT_CTYPE(unsigned long, false)
#endif
OP_SHIFT_CTYPE(ap_slong, true)
OP_SHIFT_CTYPE(ap_ulong, false)

#define OP_BIN_MIX_RANGE(BIN_OP, RTYPE)					\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE typename ap_int_base<_AP_W1,_AP_S1>::template RType<_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_range_ref<_AP_W1,_AP_S1>& op1, const ap_int_base<_AP_W2,_AP_S2>& op2) { \
    return ap_int_base<_AP_W1, false>(op1) BIN_OP (op2);		\
  }									\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE typename ap_int_base<_AP_W1,_AP_S1>::template RType<_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_int_base<_AP_W1,_AP_S1>& op1, const ap_range_ref<_AP_W2,_AP_S2>& op2) { \
    return op1 BIN_OP (ap_int_base<_AP_W2, false>(op2));		\
  }

#define OP_REL_MIX_RANGE(REL_OP)					\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE bool operator REL_OP ( const ap_range_ref<_AP_W1,_AP_S1>& op1, const ap_int_base<_AP_W2,_AP_S2>& op2) { \
    return ap_int_base<_AP_W1,false>(op1).operator REL_OP (op2);	\
  }									\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE bool operator REL_OP ( const ap_int_base<_AP_W1,_AP_S1>& op1, const ap_range_ref<_AP_W2,_AP_S2>& op2) { \
    return op1.operator REL_OP (op2.operator ap_int_base<_AP_W2, false>()); \
  }

#define OP_ASSIGN_MIX_RANGE(ASSIGN_OP)					\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE ap_int_base<_AP_W1,_AP_S1>& operator ASSIGN_OP ( ap_int_base<_AP_W1,_AP_S1>& op1, ap_range_ref<_AP_W2,_AP_S2>& op2) { \
    return op1.operator ASSIGN_OP (ap_int_base<_AP_W2, false>(op2));	\
  }									\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE ap_range_ref<_AP_W1,_AP_S1>& operator ASSIGN_OP ( ap_range_ref<_AP_W1,_AP_S1>& op1, ap_int_base<_AP_W2,_AP_S2>& op2) { \
    ap_int_base<_AP_W1, false> tmp(op1);				\
    tmp.operator ASSIGN_OP (op2);					\
    op1 = tmp;								\
    return op1;								\
  }


OP_ASSIGN_MIX_RANGE(+=)
OP_ASSIGN_MIX_RANGE(-=)
OP_ASSIGN_MIX_RANGE(*=)
OP_ASSIGN_MIX_RANGE(/=)
OP_ASSIGN_MIX_RANGE(%=)
OP_ASSIGN_MIX_RANGE(>>=)
OP_ASSIGN_MIX_RANGE(<<=)
OP_ASSIGN_MIX_RANGE(&=)
OP_ASSIGN_MIX_RANGE(|=)
OP_ASSIGN_MIX_RANGE(^=)

OP_REL_MIX_RANGE(==)
OP_REL_MIX_RANGE(!=)
OP_REL_MIX_RANGE(>)
OP_REL_MIX_RANGE(>=)
OP_REL_MIX_RANGE(<)
OP_REL_MIX_RANGE(<=)

OP_BIN_MIX_RANGE(+, plus)
OP_BIN_MIX_RANGE(-, minus)
OP_BIN_MIX_RANGE(*, mult)
OP_BIN_MIX_RANGE(/, div)
OP_BIN_MIX_RANGE(%, mod)
OP_BIN_MIX_RANGE(>>, arg1)
OP_BIN_MIX_RANGE(<<, arg1)
OP_BIN_MIX_RANGE(&, logic)
OP_BIN_MIX_RANGE(|, logic)
OP_BIN_MIX_RANGE(^, logic)

#define OP_BIN_MIX_BIT(BIN_OP, RTYPE)					\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE typename ap_int_base<_AP_W1,_AP_S1>::template RType<1,false>::RTYPE \
  operator BIN_OP ( const ap_int_base<_AP_W1,_AP_S1>& op1, const ap_bit_ref<_AP_W2,_AP_S2>& op2) { \
    return op1 BIN_OP (ap_int_base<1, false>(op2));			\
  }

#define OP_REL_MIX_BIT(REL_OP)						\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE bool operator REL_OP ( const ap_int_base<_AP_W1,_AP_S1>& op1, const ap_bit_ref<_AP_W2,_AP_S2>& op2) { \
    return op1.operator REL_OP (ap_int_base<1, false>(op2));		\
  }

#define OP_ASSIGN_MIX_BIT(ASSIGN_OP)					\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE ap_int_base<_AP_W1,_AP_S1>& operator ASSIGN_OP ( ap_int_base<_AP_W1,_AP_S1>& op1, ap_bit_ref<_AP_W2,_AP_S2>& op2) { \
    return op1.operator ASSIGN_OP (ap_int_base<1, false>(op2));		\
  }									\
  template<int _AP_W1, bool _AP_S1, int _AP_W2, bool _AP_S2>		\
  INLINE ap_bit_ref<_AP_W1,_AP_S1>& operator ASSIGN_OP ( ap_bit_ref<_AP_W1,_AP_S1>& op1, ap_int_base<_AP_W2,_AP_S2>& op2) { \
    ap_int_base<1, false> tmp(op1);					\
    tmp.operator ASSIGN_OP (op2);					\
    op1 = tmp;								\
    return op1;								\
  }


OP_ASSIGN_MIX_BIT(+=)
OP_ASSIGN_MIX_BIT(-=)
OP_ASSIGN_MIX_BIT(*=)
OP_ASSIGN_MIX_BIT(/=)
OP_ASSIGN_MIX_BIT(%=)
OP_ASSIGN_MIX_BIT(>>=)
OP_ASSIGN_MIX_BIT(<<=)
OP_ASSIGN_MIX_BIT(&=)
OP_ASSIGN_MIX_BIT(|=)
OP_ASSIGN_MIX_BIT(^=)

OP_REL_MIX_BIT(==)
OP_REL_MIX_BIT(!=)
OP_REL_MIX_BIT(>)
OP_REL_MIX_BIT(>=)
OP_REL_MIX_BIT(<)
OP_REL_MIX_BIT(<=)

OP_BIN_MIX_BIT(+, plus)
OP_BIN_MIX_BIT(-, minus)
OP_BIN_MIX_BIT(*, mult)
OP_BIN_MIX_BIT(/, div)
OP_BIN_MIX_BIT(%, mod)
OP_BIN_MIX_BIT(>>, arg1)
OP_BIN_MIX_BIT(<<, arg1)
OP_BIN_MIX_BIT(&, logic)
OP_BIN_MIX_BIT(|, logic)
OP_BIN_MIX_BIT(^, logic)

#define REF_REL_OP_MIX_INT(REL_OP, C_TYPE, _AP_W2, _AP_S2)		\
  template<int _AP_W, bool _AP_S>					\
  INLINE bool operator REL_OP ( const ap_range_ref<_AP_W,_AP_S> &op, C_TYPE op2) { \
    return (ap_int_base<_AP_W, false>(op)).operator REL_OP (ap_int_base<_AP_W2,_AP_S2>(op2)); \
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE bool operator REL_OP ( const ap_bit_ref<_AP_W,_AP_S> &op, C_TYPE op2) { \
    return (bool(op)) REL_OP op2;					\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE bool operator REL_OP ( C_TYPE op2, const ap_bit_ref<_AP_W,_AP_S> &op) { \
    return op2 REL_OP (bool(op));					\
  }									\
  template<int _AP_W, typename _AP_T, int _AP_W1, typename _AP_T1>	\
  INLINE bool operator REL_OP ( const ap_concat_ref<_AP_W,_AP_T, _AP_W1, _AP_T1> &op, C_TYPE op2) { \
    return (ap_int_base<_AP_W + _AP_W1, false>(op)).operator REL_OP (ap_int_base<_AP_W2,_AP_S2>(op2)); \
  }  

// Make the line shorter than 5000 chars
#define REF_REL_MIX_INT_1(C_TYPE, _AP_WI, _AP_SI)	\
  REF_REL_OP_MIX_INT(>, C_TYPE, _AP_WI, _AP_SI)		\
  REF_REL_OP_MIX_INT(<, C_TYPE, _AP_WI, _AP_SI)		\
  REF_REL_OP_MIX_INT(>=, C_TYPE, _AP_WI, _AP_SI)	\
  REF_REL_OP_MIX_INT(<=, C_TYPE, _AP_WI, _AP_SI)

REF_REL_MIX_INT_1(bool, 1, false)
REF_REL_MIX_INT_1(char, 8, CHAR_MIN != 0)
REF_REL_MIX_INT_1(signed char, 8, true)
REF_REL_MIX_INT_1(unsigned char, 8, false)
REF_REL_MIX_INT_1(short, 16, true)
REF_REL_MIX_INT_1(unsigned short, 16, false)
REF_REL_MIX_INT_1(int, 32, true)
REF_REL_MIX_INT_1(unsigned int, 32, false)
# if defined __x86_64__
REF_REL_MIX_INT_1(long, 64, true)
REF_REL_MIX_INT_1(unsigned long, 64, false)
#else
REF_REL_MIX_INT_1(long, 32, true)
REF_REL_MIX_INT_1(unsigned long, 32, false)
#endif
REF_REL_MIX_INT_1(ap_slong, 64, true)
REF_REL_MIX_INT_1(ap_ulong, 64, false)

// Make the line shorter than 5000 chars
#define REF_REL_MIX_INT_2(C_TYPE, _AP_WI, _AP_SI)	\
  REF_REL_OP_MIX_INT(==, C_TYPE, _AP_WI, _AP_SI)	\
  REF_REL_OP_MIX_INT(!=, C_TYPE, _AP_WI, _AP_SI) 

REF_REL_MIX_INT_2(bool, 1, false)
REF_REL_MIX_INT_2(char, 8, CHAR_MIN != 0)
REF_REL_MIX_INT_2(signed char, 8, true)
REF_REL_MIX_INT_2(unsigned char, 8, false)
REF_REL_MIX_INT_2(short, 16, true)
REF_REL_MIX_INT_2(unsigned short, 16, false)
REF_REL_MIX_INT_2(int, 32, true)
REF_REL_MIX_INT_2(unsigned int, 32, false)
# if defined __x86_64__
REF_REL_MIX_INT_2(long, 64, true)
REF_REL_MIX_INT_2(unsigned long, 64, false)
#else
REF_REL_MIX_INT_2(long, 32, true)
REF_REL_MIX_INT_2(unsigned long, 32, false)
#endif
REF_REL_MIX_INT_2(ap_slong, 64, true)
REF_REL_MIX_INT_2(ap_ulong, 64, false)

#define REF_BIN_OP_MIX_INT(BIN_OP, RTYPE, C_TYPE, _AP_W2, _AP_S2)	\
  template<int _AP_W, bool _AP_S>					\
  INLINE typename ap_int_base<_AP_W,false>::template RType<_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_range_ref<_AP_W,_AP_S> &op, C_TYPE op2) {  \
    return (ap_int_base<_AP_W, false>(op)) BIN_OP (ap_int_base<_AP_W2,_AP_S2>(op2)); \
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE typename ap_int_base<_AP_W2,_AP_S2>::template RType<_AP_W,false>::RTYPE \
  operator BIN_OP ( C_TYPE op2, const ap_range_ref<_AP_W,_AP_S> &op) {  \
    return ap_int_base<_AP_W2,_AP_S2>(op2) BIN_OP (ap_int_base<_AP_W, false>(op)); \
  }

#define REF_BIN_MIX_INT(C_TYPE, _AP_WI, _AP_SI)		\
  REF_BIN_OP_MIX_INT(+, plus, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(-, minus, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(*, mult, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(/, div, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(%, mod, C_TYPE, _AP_WI, _AP_SI)

REF_BIN_MIX_INT(bool, 1, false)
REF_BIN_MIX_INT(char, 8, CHAR_MIN != 0)
REF_BIN_MIX_INT(signed char, 8, true)
REF_BIN_MIX_INT(unsigned char, 8, false)
REF_BIN_MIX_INT(short, 16, true)
REF_BIN_MIX_INT(unsigned short, 16, false)
REF_BIN_MIX_INT(int, 32, true)
REF_BIN_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
REF_BIN_MIX_INT(long, 64, true)
REF_BIN_MIX_INT(unsigned long, 64, false)
#else
REF_BIN_MIX_INT(long, 32, true)
REF_BIN_MIX_INT(unsigned long, 32, false)
#endif
REF_BIN_MIX_INT(ap_slong, 64, true)
REF_BIN_MIX_INT(ap_ulong, 64, false)
 
#define REF_BIN_MIX_INT_BITS(C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(>>, arg1, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(<<, arg1, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(&, logic, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(|, logic, C_TYPE, _AP_WI, _AP_SI)	\
  REF_BIN_OP_MIX_INT(^, logic, C_TYPE, _AP_WI, _AP_SI)

REF_BIN_MIX_INT_BITS(bool, 1, false)
REF_BIN_MIX_INT_BITS(char, 8, CHAR_MIN != 0)
REF_BIN_MIX_INT_BITS(signed char, 8, true)
REF_BIN_MIX_INT_BITS(unsigned char, 8, false)
REF_BIN_MIX_INT_BITS(short, 16, true)
REF_BIN_MIX_INT_BITS(unsigned short, 16, false)
REF_BIN_MIX_INT_BITS(int, 32, true)
REF_BIN_MIX_INT_BITS(unsigned int, 32, false)
# if defined __x86_64__
REF_BIN_MIX_INT_BITS(long, 64, true)
REF_BIN_MIX_INT_BITS(unsigned long, 64, false)
#else
REF_BIN_MIX_INT_BITS(long, 32, true)
REF_BIN_MIX_INT_BITS(unsigned long, 32, false)
#endif
REF_BIN_MIX_INT_BITS(ap_slong, 64, true)
REF_BIN_MIX_INT_BITS(ap_ulong, 64, false)

#define REF_BIN_OP(BIN_OP, RTYPE)					\
  template<int _AP_W, bool _AP_S, int _AP_W2, bool _AP_S2>		\
  INLINE typename ap_int_base<_AP_W, false>::template RType<_AP_W2, false>::RTYPE \
  operator BIN_OP (const ap_range_ref<_AP_W,_AP_S> &lhs, const ap_range_ref<_AP_W2,_AP_S2> &rhs) { \
    return ap_int_base<_AP_W, false>(lhs) BIN_OP (ap_int_base<_AP_W2, false>(rhs)); \
  }

REF_BIN_OP(+, plus)
REF_BIN_OP(-, minus)
REF_BIN_OP(*, mult)
REF_BIN_OP(/, div)
REF_BIN_OP(%, mod)
REF_BIN_OP(>>, arg1)
REF_BIN_OP(<<, arg1)
REF_BIN_OP(&, logic)
REF_BIN_OP(|, logic)
REF_BIN_OP(^, logic)

//************************************************************************
//  Implement
//      ap_private<M+N> = ap_concat_ref<M> OP ap_concat_ref<N>
//  for operators  +, -, *, /, %, >>, <<, &, |, ^
//  Without these operators the operands are converted to int64 and
//  larger results lose informations (higher order bits).
//
//                       operand OP
//                      /          |
//              left-concat         right-concat
//                /     |            /         |
//         <LW1,LT1>  <LW2,LT2>   <RW1,RT1>    <RW2,RT2>
//
//      _AP_LW1, _AP_LT1 (width and type of left-concat's left side)
//      _AP_LW2, _AP_LT2 (width and type of left-concat's right side)
//  Similarly for RHS of operand OP: _AP_RW1, AP_RW2, _AP_RT1, _AP_RT2
//
//  In Verilog 2001 result of concatenation is always unsigned even
//  when both sides are signed.
//************************************************************************

#undef SYN_CONCAT_REF_BIN_OP

#define SYN_CONCAT_REF_BIN_OP(BIN_OP, RTYPE)				\
  template<int _AP_LW1,typename _AP_LT1,int _AP_LW2,typename _AP_LT2,	\
	   int _AP_RW1,typename _AP_RT1,int _AP_RW2,typename _AP_RT2>	\
  INLINE typename							\
  ap_int_base<_AP_LW1+_AP_LW2,false>::template RType<_AP_RW1+_AP_RW2,false>::RTYPE \
  operator BIN_OP (const ap_concat_ref<_AP_LW1,_AP_LT1,_AP_LW2,_AP_LT2>& lhs, \
		   const ap_concat_ref<_AP_RW1,_AP_RT1,_AP_RW2,_AP_RT2>& rhs) \
  {									\
    return lhs.get() BIN_OP rhs.get();					\
  }

SYN_CONCAT_REF_BIN_OP(+, plus)
SYN_CONCAT_REF_BIN_OP(-, minus)
SYN_CONCAT_REF_BIN_OP(*, mult)
SYN_CONCAT_REF_BIN_OP(/, div)
SYN_CONCAT_REF_BIN_OP(%, mod)
SYN_CONCAT_REF_BIN_OP(>>, arg1)
SYN_CONCAT_REF_BIN_OP(<<, arg1)
SYN_CONCAT_REF_BIN_OP(&, logic)
SYN_CONCAT_REF_BIN_OP(|, logic)
SYN_CONCAT_REF_BIN_OP(^, logic)

#undef SYN_CONCAT_REF_BIN_OP
//************************************************************************

#if 1
#define CONCAT_OP_MIX_INT(C_TYPE, _AP_WI, _AP_SI)			\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base< _AP_W +  _AP_WI, false >					\
  operator, (const ap_int_base<_AP_W, _AP_S> &op1, C_TYPE op2) {	\
    ap_int_base<_AP_WI + _AP_W, false> val(op2);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op1);			\
    ret <<= _AP_WI;							\
    if (_AP_SI) {							\
      val <<= _AP_W; val >>= _AP_W;					\
    }									\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base< _AP_W +  _AP_WI, false >					\
  operator, (C_TYPE op1, const ap_int_base<_AP_W, _AP_S>& op2)  {	\
    ap_int_base<_AP_WI + _AP_W, false> val(op1);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op2);			\
    if (_AP_S) {							\
      ret <<= _AP_WI; ret >>= _AP_WI;					\
    }									\
    ret |= val << _AP_W;						\
    return ret;								\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base< _AP_W +  _AP_WI, false >					\
  operator, (const ap_range_ref<_AP_W, _AP_S> &op1, C_TYPE op2) {	\
    ap_int_base<_AP_WI + _AP_W, false> val(op2);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op1);			\
    ret <<= _AP_WI;							\
    if (_AP_SI) {							\
      val <<= _AP_W; val >>= _AP_W;					\
    }									\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base< _AP_W +  _AP_WI, false >					\
  operator, (C_TYPE op1, const ap_range_ref<_AP_W, _AP_S> &op2)  {	\
    ap_int_base<_AP_WI + _AP_W, false> val(op1);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op2);			\
    int len = op2.length();						\
    val <<= len;							\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base<_AP_WI + 1, false >					\
  operator, (const ap_bit_ref<_AP_W, _AP_S> &op1, C_TYPE op2) {		\
    ap_int_base<_AP_WI + 1, false> val(op2);				\
    val[_AP_WI] = op1;							\
    return val;								\
  }									\
  template<int _AP_W, bool _AP_S>					\
  INLINE								\
  ap_int_base<_AP_WI + 1, false >					\
  operator, (C_TYPE op1, const ap_bit_ref<_AP_W, _AP_S> &op2)  {	\
    ap_int_base<_AP_WI + 1, false> val(op1);				\
    val <<= 1;								\
    val[0] = op2;							\
    return val;								\
  }									\
  template<int _AP_W, typename _AP_T, int _AP_W2, typename _AP_T2>	\
  INLINE								\
  ap_int_base<_AP_W + _AP_W2 + _AP_WI, false >				\
  operator, (const ap_concat_ref<_AP_W, _AP_T, _AP_W2, _AP_T2> &op1, C_TYPE op2) { \
    ap_int_base<_AP_WI + _AP_W + _AP_W2, _AP_SI> val(op2);		\
    ap_int_base<_AP_WI + _AP_W + _AP_W2, _AP_SI> ret(op1);		\
    if (_AP_SI) {							\
      val <<= _AP_W + _AP_W2; val >>= _AP_W + _AP_W2;			\
    }									\
    ret <<= _AP_WI;							\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, typename _AP_T, int _AP_W2, typename _AP_T2>	\
  INLINE								\
  ap_int_base<_AP_W + _AP_W2 + _AP_WI, false >				\
  operator, (C_TYPE op1, const ap_concat_ref<_AP_W, _AP_T, _AP_W2, _AP_T2> &op2) { \
    ap_int_base<_AP_WI + _AP_W + _AP_W2, _AP_SI> val(op1);		\
    ap_int_base<_AP_WI + _AP_W + _AP_W2, _AP_SI> ret(op2);		\
    int len = op2.length();						\
    val <<= len;							\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, \
	   int _AP_N >							\
  INLINE								\
  ap_int_base< _AP_W + _AP_WI, false >					\
  operator, (const af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op1, C_TYPE op2) { \
    ap_int_base<_AP_WI + _AP_W, false> val(op2);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op1);			\
    if (_AP_SI) {							\
      val <<= _AP_W; val >>= _AP_W;					\
    }									\
    ret <<= _AP_WI;							\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, \
	   int _AP_N >							\
  INLINE								\
  ap_int_base< _AP_W + _AP_WI, false >					\
  operator, (C_TYPE op1, const af_range_ref<_AP_W, _AP_I, _AP_S,	\
             _AP_Q, _AP_O, _AP_N> &op2) {				\
    ap_int_base<_AP_WI + _AP_W, false> val(op1);			\
    ap_int_base<_AP_WI + _AP_W, false> ret(op2);			\
    int len = op2.length();						\
    val <<= len;							\
    ret |= val;								\
    return ret;								\
  }									\
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, \
	   int _AP_N >							\
  INLINE								\
  ap_int_base< 1 + _AP_WI, false>					\
  operator, (const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O,	\
             _AP_N> &op1, C_TYPE op2) {					\
    ap_int_base<_AP_WI + 1, _AP_SI> val(op2);				\
    val[_AP_WI] = op1;							\
    return val;								\
  }									\
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, \
	   int _AP_N >							\
  INLINE								\
  ap_int_base< 1 + _AP_WI, false>					\
  operator, (C_TYPE op1, const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q,	\
	     _AP_O, _AP_N> &op2) {					\
    ap_int_base<_AP_WI + 1, _AP_SI> val(op1);				\
    val <<= 1;								\
    val[0] = op2;							\
    return val;								\
  } 

CONCAT_OP_MIX_INT(bool, 1, false)
CONCAT_OP_MIX_INT(char, 8, CHAR_MIN != 0)
CONCAT_OP_MIX_INT(signed char, 8, true)
CONCAT_OP_MIX_INT(unsigned char, 8, false)
CONCAT_OP_MIX_INT(short, 16, true)
CONCAT_OP_MIX_INT(unsigned short, 16, false)
CONCAT_OP_MIX_INT(int, 32, true)
CONCAT_OP_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
CONCAT_OP_MIX_INT(long, 64, true)
CONCAT_OP_MIX_INT(unsigned long, 64, false)
#else
CONCAT_OP_MIX_INT(long, 32, true)
CONCAT_OP_MIX_INT(unsigned long, 32, false)
#endif
CONCAT_OP_MIX_INT(ap_slong, 64, true)
CONCAT_OP_MIX_INT(ap_ulong, 64, false)
#endif

#if 1
#define CONCAT_SHIFT_MIX_INT(C_TYPE, op)				\
  template<int _AP_W, typename _AP_T, int _AP_W1, typename _AP_T1>	\
  INLINE ap_uint<_AP_W+_AP_W1> operator op (const ap_concat_ref<_AP_W, _AP_T, _AP_W1, _AP_T1> lhs, C_TYPE rhs) { \
    return ((ap_uint<_AP_W+_AP_W1>)lhs.get()) op ((int)rhs);		\
  }

CONCAT_SHIFT_MIX_INT(long, <<)
CONCAT_SHIFT_MIX_INT(unsigned long, <<)
CONCAT_SHIFT_MIX_INT(unsigned int, <<)
CONCAT_SHIFT_MIX_INT(ap_ulong, <<)
CONCAT_SHIFT_MIX_INT(ap_slong, <<)
CONCAT_SHIFT_MIX_INT(long, >>)
CONCAT_SHIFT_MIX_INT(unsigned long, >>)
CONCAT_SHIFT_MIX_INT(unsigned int, >>)
CONCAT_SHIFT_MIX_INT(ap_ulong, >>)
CONCAT_SHIFT_MIX_INT(ap_slong, >>)
#endif
#endif /* #ifndef __AESL_AP_INT_SYN_H__*/
// FIXME: Finished filling ap_int_syn.h

// FIXME: Filling in ap_fixed_syn.h
// -*- c++ -*-

/*
#-  (c) Copyright 2011-2018 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************

 *
 */

#ifndef __AESL_AP_FIXED_SYN_H__
#define __AESL_AP_FIXED_SYN_H__



/// ap_fixed_base
// -----------------------------------------------------------------------------
//#include <math.h>

#define HALF_MAN 10
#define HALF_EXP 5
#define FLOAT_MAN 23
#define FLOAT_EXP 8
#define DOUBLE_MAN 52
#define DOUBLE_EXP 11

#define BIAS(e) ((1<<(e-1))-1)
#define HALF_BIAS BIAS(HALF_EXP)
#define FLOAT_BIAS BIAS(FLOAT_EXP)
#define DOUBLE_BIAS BIAS(DOUBLE_EXP)
#define APFX_IEEE_DOUBLE_E_MAX DOUBLE_BIAS
#define APFX_IEEE_DOUBLE_E_MIN (-DOUBLE_BIAS + 1)

template<int _AP_W, int _AP_I, bool _AP_S, 
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
struct af_bit_ref {
    ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& d_bv;
    int d_index;

public:
    INLINE af_bit_ref(const af_bit_ref<_AP_W,_AP_I,_AP_S,
              _AP_Q,_AP_O,_AP_N>&ref): 
              d_bv(ref.d_bv), d_index(ref.d_index) {}

    INLINE af_bit_ref(ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>* bv, int index = 0) : 
        d_bv(*bv), d_index(index) {}
    INLINE operator bool () const { return _ssdm_op_get_bit(d_bv.V, d_index); }

    INLINE af_bit_ref& operator = (unsigned long long val) {
        d_bv.V = _ssdm_op_set_bit(d_bv.V, d_index, val);
        return *this;
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE af_bit_ref& operator = (const ap_int_base<_AP_W2,_AP_S2>& val) {
        return operator =(val.to_uint64());
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE af_bit_ref& operator = (const af_bit_ref<_AP_W2,_AP_I2,
                                  _AP_S2,_AP_Q2,_AP_O2,_AP_N2>& val) {
        return operator =((unsigned long long) (bool) val);
    }
    
    INLINE af_bit_ref& operator = (const af_bit_ref<_AP_W,_AP_I,
                                  _AP_S,_AP_Q,_AP_O,_AP_N>& val) {
        return operator =((unsigned long long) (bool) val);
    }
    template<int _AP_W2, bool _AP_S2>
    INLINE af_bit_ref& operator = ( const ap_bit_ref<_AP_W2, _AP_S2> &val) {
        return operator =((unsigned long long) (bool) val);
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE af_bit_ref& operator = ( const ap_range_ref<_AP_W2,_AP_S2>& val) {
        return operator =((const ap_int_base<_AP_W2, false>) val);
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
    INLINE af_bit_ref& operator= (const af_range_ref<_AP_W2, _AP_I2, _AP_S2,
                                    _AP_Q2, _AP_O2, _AP_N2>& val) {
        return operator=((const ap_int_base<_AP_W2, false>)(val));
    } 

    template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
    INLINE af_bit_ref& operator= (const ap_concat_ref<_AP_W2, _AP_T3, _AP_W3, _AP_T3>& val) {
        return operator=((const ap_int_base<_AP_W2 + _AP_W3, false>)(val));
    }
    
    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<1, af_bit_ref, _AP_W2, ap_int_base<_AP_W2, _AP_S2> >
    operator, (ap_int_base<_AP_W2, _AP_S2>& op) {
        return ap_concat_ref<1, af_bit_ref, _AP_W2, 
               ap_int_base<_AP_W2, _AP_S2> >(*this, op);
    }

    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<1, af_bit_ref, 1, ap_bit_ref<_AP_W2, _AP_S2> >
    operator, (const ap_bit_ref<_AP_W2, _AP_S2> &op) {
        return ap_concat_ref<1, af_bit_ref, 1, 
                 ap_bit_ref<_AP_W2, _AP_S2> >(*this, 
               const_cast<ap_bit_ref<_AP_W2, _AP_S2>& >(op));
    }

    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<1, af_bit_ref, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> >
    operator, (const ap_range_ref<_AP_W2, _AP_S2> &op) {
        return ap_concat_ref<1, af_bit_ref, _AP_W2, 
                  ap_range_ref<_AP_W2, _AP_S2> >(*this, 
                  const_cast< ap_range_ref<_AP_W2, _AP_S2>& >(op));
    }

    template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
    INLINE ap_concat_ref<1, af_bit_ref, _AP_W2 + _AP_W3, 
                        ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> > 
    operator, (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> op) {
        return ap_concat_ref<1, af_bit_ref, _AP_W2 + _AP_W3, 
                 ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(*this, 
                 const_cast<ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& > (op));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_concat_ref<1, af_bit_ref, _AP_W2, 
             af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
    operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
               _AP_O2, _AP_N2> &op) {
       return ap_concat_ref<1, af_bit_ref,  _AP_W2,
             af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this,
             const_cast<af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
             _AP_O2, _AP_N2>& >(op));
    } 
              
    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_concat_ref<1, af_bit_ref, 1, 
             af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
    operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
               _AP_O2, _AP_N2> &op) {
       return ap_concat_ref<1, af_bit_ref, 1,
             af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this, 
             const_cast<af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
             _AP_O2, _AP_N2>& >(op));
    } 
              
    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
    INLINE bool operator == (const af_bit_ref<_AP_W2, _AP_I2, 
                             _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
        return get() == op.get();
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
    INLINE bool operator != (const af_bit_ref<_AP_W2, _AP_I2, 
                             _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
        return get() != op.get();
    }

    INLINE bool get() const {
        return _ssdm_op_get_bit(d_bv.V, d_index);
    }

    INLINE bool operator ~ () const {
        bool bit = _ssdm_op_get_bit(d_bv.V, d_index);
        return bit ? false : true;
    }

    INLINE int length() const {
        return 1;
    }

};
/* Range (slice) reference.
 ----------------------------------------------------------------
*/
template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
struct af_range_ref {
    ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& d_bv;
    int l_index;
    int h_index;
    
public:
    INLINE af_range_ref(const af_range_ref<_AP_W,_AP_I,_AP_S,
           _AP_Q,_AP_O, _AP_N>&ref):
           d_bv(ref.d_bv), l_index(ref.l_index), h_index(ref.h_index) {}

    INLINE af_range_ref(ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>* bv
                        , int h, int l) :
        d_bv(*bv), l_index(l), h_index(h) {
    }

    INLINE operator ap_int_base<_AP_W,false> () const {
        ap_int_base<_AP_W, false> ret;
        ret.V = _ssdm_op_get_range(d_bv.V, l_index, h_index);
        return ret;
    }

    INLINE operator unsigned long long () const {
        ap_int_base<_AP_W, false> ret;
        ret.V = _ssdm_op_get_range(d_bv.V, l_index, h_index);
        return ret.to_uint64();
    }
#define ASSIGN_INT_TO_AF_RANGE(DATA_TYPE) \
INLINE af_range_ref& operator = (const DATA_TYPE val) { \
        ap_int_base<_AP_W, false> loc(val); \
        d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, loc.V); \
        return *this;  \
    }

ASSIGN_INT_TO_AF_RANGE(char)
ASSIGN_INT_TO_AF_RANGE(signed char)
ASSIGN_INT_TO_AF_RANGE(short)
ASSIGN_INT_TO_AF_RANGE(unsigned short)
ASSIGN_INT_TO_AF_RANGE(int)
ASSIGN_INT_TO_AF_RANGE(unsigned int)
ASSIGN_INT_TO_AF_RANGE(long)
ASSIGN_INT_TO_AF_RANGE(unsigned long)
ASSIGN_INT_TO_AF_RANGE(long long)
ASSIGN_INT_TO_AF_RANGE(unsigned long long)

#undef ASSIGN_INT_TO_AF_RANGE
    

    template<int _AP_W2, bool _AP_S2>
    INLINE af_range_ref& operator = (const ap_int_base<_AP_W2,_AP_S2>& val) {
        ap_int_base<_AP_W, false> loc(val);
        d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, loc.V);
        return *this;
    }

    INLINE af_range_ref& operator = (const char* val) {
        ap_int_base<_AP_W, false> loc(val);
        d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, loc.V);
        return *this;
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE af_range_ref& operator= (const af_range_ref<_AP_W2, 
                          _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& val) {
        ap_int_base<_AP_W2, false> tmp(val);
        return operator=(tmp);
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE af_range_ref& operator= (const ap_fixed_base<_AP_W2, 
                          _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& val) {
        return operator=(val.to_ap_int_base());
    }

    INLINE af_range_ref& operator= (const af_range_ref<_AP_W, 
                          _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& val) {
        ap_int_base<_AP_W, false> tmp(val);
        return operator=(tmp);
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE af_range_ref& operator= (const ap_range_ref<_AP_W2, _AP_S2>& val) {
        return operator=((ap_int_base<_AP_W2, false>)val);
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> 
    INLINE af_range_ref& operator= (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2,
                                    _AP_Q2, _AP_O2, _AP_N2>& val) {
        return operator=((unsigned long long)(bool)(val));
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE af_range_ref& operator= (const ap_bit_ref<_AP_W2, _AP_S2>& val) {
        return operator=((unsigned long long)(bool)(val));
    }

    template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
    INLINE af_range_ref& operator= (const ap_concat_ref<_AP_W2, _AP_T3, _AP_W3, _AP_T3>& val) {
        return operator=((const ap_int_base<_AP_W2 + _AP_W3, false>)(val));
    }
 
    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator == (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        ap_int_base<_AP_W, false> lop (*this); 
        ap_int_base<_AP_W2, false> rop (op2); 
        return lop == rop;
    }
    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator != (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        return !(operator == (op2));
    }
    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator < (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        ap_int_base<_AP_W, false> lop(*this); 
        ap_int_base<_AP_W2, false> rop(op2); 
        return lop < rop;
    }
    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator <= (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        ap_int_base<_AP_W, false> lop(*this); 
        ap_int_base<_AP_W2, false> rop(op2); 
        return lop <= rop;
    }
    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator > (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        return !(operator <= (op2));
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE bool operator >= (const ap_range_ref<_AP_W2, _AP_S2>& op2) {
        return !(operator < (op2));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator == (const af_range_ref<_AP_W2, _AP_I2,
                              _AP_S2,_AP_Q2, _AP_O2, _AP_N2>& op2) {
        ap_int_base<_AP_W, false> lop (*this);
        ap_int_base<_AP_W2, false> rop (op2);
        return lop == rop;
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator != (const af_range_ref<_AP_W2, _AP_I2,
                             _AP_S2,_AP_Q2, _AP_O2, _AP_N2>& op2) {
       return !(operator == (op2));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator < (const af_range_ref<_AP_W2, _AP_I2,
                            _AP_S2,_AP_Q2, _AP_O2, _AP_N2>& op2) {
        ap_int_base<_AP_W, false> lop (*this);
        ap_int_base<_AP_W2, false> rop (op2);
        return lop < rop;
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator <= (const af_range_ref<_AP_W2, _AP_I2,
                             _AP_S2,_AP_Q2, _AP_O2, _AP_N2>& op2) {
        ap_int_base<_AP_W, false> lop( *this);
        ap_int_base<_AP_W2, false> rop (op2); 
        return lop <= rop;
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator > (const af_range_ref<_AP_W2, _AP_I2,
                               _AP_S2,_AP_Q2, _AP_O2, _AP_N2>& op2) {
        return !(operator <= (op2));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE bool operator >= (const af_range_ref<_AP_W2, _AP_I2, 
                             _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op2) {
        return !(operator < (op2));
    }

    template <int _AP_W3>
    INLINE void set(const ap_int_base<_AP_W3, false>& val) {
        d_bv.V = _ssdm_op_set_range(d_bv.V, l_index, h_index, val.V);
    }

    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<_AP_W, af_range_ref, _AP_W2, ap_int_base<_AP_W2, _AP_S2> >
    operator, (ap_int_base<_AP_W2, _AP_S2>& op) {
        return ap_concat_ref<_AP_W, af_range_ref, _AP_W2, 
               ap_int_base<_AP_W2, _AP_S2> >(*this, op);
    }

    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<_AP_W, af_range_ref, 1, ap_bit_ref<_AP_W2, _AP_S2> >
    operator, (const ap_bit_ref<_AP_W2, _AP_S2> &op) {
        return ap_concat_ref<_AP_W, af_range_ref, 1, 
               ap_bit_ref<_AP_W2, _AP_S2> >(*this, 
               const_cast<ap_bit_ref<_AP_W2, _AP_S2>& >(op));
    }

    template<int _AP_W2, int _AP_S2>
    INLINE ap_concat_ref<_AP_W, af_range_ref, _AP_W2, ap_range_ref<_AP_W2, _AP_S2> >
    operator, (const ap_range_ref<_AP_W2, _AP_S2> &op) {
        return ap_concat_ref<_AP_W, af_range_ref, _AP_W2, 
               ap_range_ref<_AP_W2, _AP_S2> >(*this, 
               const_cast<ap_range_ref<_AP_W2, _AP_S2>& >(op));
    }

    template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
    INLINE ap_concat_ref<_AP_W, af_range_ref, _AP_W2 + _AP_W3, 
                        ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> > 
    operator, (const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& op) {
        return ap_concat_ref<_AP_W, af_range_ref, _AP_W2 + _AP_W3, 
                 ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3> >(*this, 
                 const_cast<ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& >(op));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_concat_ref<_AP_W, af_range_ref, _AP_W2, 
             af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
    operator, (const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
               _AP_O2, _AP_N2> &op) {
       return ap_concat_ref<_AP_W, af_range_ref,  _AP_W2,
             af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this,
             const_cast<af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
                       _AP_O2, _AP_N2>& >(op));
    } 
              
    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_concat_ref<_AP_W, af_range_ref, 1, 
             af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >
    operator, (const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
               _AP_O2, _AP_N2> &op) {
       return ap_concat_ref<_AP_W, af_range_ref, 1,
             af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> >(*this,
             const_cast<af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, 
             _AP_O2, _AP_N2>& >(op));
    } 
 

    INLINE int length() const { 
      return h_index >= l_index ? h_index - l_index + 1 : l_index - h_index + 1;
    }

    INLINE int to_int() const {
        return (int)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
    INLINE unsigned to_uint() const {
        return (unsigned)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
    INLINE long to_long() const {
        return (long)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
    INLINE unsigned long to_ulong() const {
        return (unsigned long)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
    INLINE ap_slong to_int64() const {
        return (ap_slong)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
    INLINE ap_ulong to_uint64() const {
        return (ap_ulong)(_ssdm_op_get_range(d_bv.V, l_index, h_index));
    }
};

// -----------------------------------------------------------------------------
/// ap_fixed_base: AutoPilot fixed point.
// -----------------------------------------------------------------------------
// XXX: default template parameter in first declaration.
template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
         int _AP_N>
struct ap_fixed_base : ssdm_int<_AP_W, _AP_S> {
    
public:
    typedef ssdm_int<_AP_W, _AP_S> Base;

    static const int width = _AP_W;
    static const int iwidth = _AP_I;
    static const ap_q_mode qmode = _AP_Q;
    static const ap_o_mode omode = _AP_O;

    AP_WEAK void overflow_adjust(bool underflow, bool overflow,bool lD, bool sign) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif

        if (!underflow && !overflow) return;
        if (_AP_O==SC_WRAP) {
            if (_AP_N == 0)
                return;
            if (_AP_S) {
                //signed SC_WRAP 
                //n_bits == 1
                Base::V = _ssdm_op_set_bit(Base::V, _AP_W - 1, sign);
                if (_AP_N > 1) {
                //n_bits > 1
                    ap_int_base<_AP_W, false> mask(-1);
                    if (sign) mask.V = 0;
                    Base::V = _ssdm_op_set_range(Base::V, 
                              _AP_W - _AP_N, _AP_W - 2, mask.V);  
                }
            } else {
                //unsigned SC_WRAP 
                ap_int_base<_AP_W, false> mask(-1);
                Base::V = _ssdm_op_set_range(Base::V, _AP_W - _AP_N,
                          _AP_W - 1, mask.V);
            }
        } else if (_AP_O==SC_SAT_ZERO) {
              Base::V = 0;
        }
        else if (_AP_O == SC_WRAP_SM && _AP_S) {
            bool Ro = _ssdm_op_get_bit(Base::V, _AP_W - 1);
            if (_AP_N == 0) {
                if (lD != Ro) {
                    Base::V = ~Base::V;
                    Base::V = _ssdm_op_set_bit(Base::V, _AP_W - 1, lD);
                }
            } else {
                if (_AP_N == 1 && sign != Ro) {
                   Base::V = ~Base::V;
                } else if (_AP_N > 1) {
                    bool lNo = _ssdm_op_get_bit(Base::V, _AP_W - _AP_N);
                    if (lNo == sign) 
                        Base::V = ~Base::V;
                    ap_int_base<_AP_W, false> mask(-1);
                    if (sign) mask.V = 0;
                    Base::V = _ssdm_op_set_range(Base::V, _AP_W - _AP_N, 
                              _AP_W - 2, mask.V); 
                } 
                Base::V = _ssdm_op_set_bit(Base::V, _AP_W - 1, sign);
                
            }
        } else {
            if (_AP_S) {
                if (overflow) { 
                    Base::V = 1;
                    Base::V <<= _AP_W - 1;
                    Base::V = ~Base::V;
                } else if (underflow) {
                    Base::V = 1;
                    Base::V <<= _AP_W - 1;
                    if (_AP_O==SC_SAT_SYM)
                        Base::V |= 1;
                }
            }
            else {
                if (overflow) 
                    Base::V = ~(ap_int_base<_AP_W,false>(0).V);
                else if (underflow)
                    Base::V = 0;
            }
        }
    }

    AP_WEAK bool quantization_adjust(bool qb, bool r, bool s) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        bool carry=(bool)_ssdm_op_get_bit(Base::V,_AP_W-1);
        if (_AP_Q==SC_TRN)
            return false;
        if (_AP_Q==SC_RND_ZERO)
            qb &= s || r;
        else if (_AP_Q==SC_RND_MIN_INF)
            qb &= r;
        else if (_AP_Q==SC_RND_INF)
            qb &= !s || r;
        else if (_AP_Q==SC_RND_CONV)
            qb &= _ssdm_op_get_bit(Base::V,0) || r;
        else if (_AP_Q==SC_TRN_ZERO)
            qb = s && ( qb || r );
        Base::V += qb;
        //return qb;
        return carry&&(!(bool)_ssdm_op_get_bit(Base::V,_AP_W-1));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2>
    struct RType {
        enum {
            _AP_F=_AP_W-_AP_I, 
            F2=_AP_W2-_AP_I2,
            mult_w = _AP_W+_AP_W2,
            mult_i = _AP_I+_AP_I2,
            mult_s = _AP_S||_AP_S2,
            plus_w = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2))+1+AP_MAX(_AP_F,F2),
            plus_i = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2))+1,
            plus_s = _AP_S||_AP_S2,
            minus_w = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2))+1+AP_MAX(_AP_F,F2),
            minus_i = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2))+1,
            minus_s = true,
#ifndef __SC_COMPATIBLE__
            div_w = _AP_W + AP_MAX(_AP_W2 - _AP_I2, 0) + _AP_S2,
#else
            div_w = _AP_W + AP_MAX(_AP_W2 - _AP_I2, 0) + _AP_S2 + AP_MAX(_AP_I2, 0),
#endif
            div_i = _AP_I + _AP_W2 -_AP_I2 + _AP_S2,
            div_s = _AP_S||_AP_S2,
            logic_w = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2))+AP_MAX(_AP_F,F2),
            logic_i = AP_MAX(_AP_I+(_AP_S2&&!_AP_S),_AP_I2+(_AP_S&&!_AP_S2)),
            logic_s = _AP_S||_AP_S2
        };

        typedef ap_fixed_base<mult_w, mult_i, mult_s> mult;
        typedef ap_fixed_base<plus_w, plus_i, plus_s> plus;
        typedef ap_fixed_base<minus_w, minus_i, minus_s> minus;
        typedef ap_fixed_base<logic_w, logic_i, logic_s> logic;
        typedef ap_fixed_base<div_w, div_i, div_s> div;
        typedef ap_fixed_base<_AP_W, _AP_I, _AP_S> arg1;
    };

    /// Constructors.
    // -------------------------------------------------------------------------
    INLINE ap_fixed_base() {
        AP_DEBUG(printf("C1: af1=<%d,%d,%d,%d,%d>\n", _AP_W, _AP_I, _AP_S, _AP_Q, _AP_O););
/*
    #ifdef __SC_COMPATIBLE__
        Base::V = 0;
    #endif
*/
    }
    template<int _AP_W2, int _AP_I2, bool _AP_S2,
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    AP_WEAK ap_fixed_base (const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2> &op) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        enum { N2=_AP_W2, _AP_F=_AP_W-_AP_I, F2=_AP_W2-_AP_I2, 
               QUAN_INC = F2>_AP_F && !(_AP_Q==SC_TRN || (_AP_Q==SC_TRN_ZERO && !_AP_S2)) };
        bool carry = false;
        AP_DEBUG(printf("C2: af1=<%d,%d,%d,%d,%d,%d>; af2=<%d,%d,%d,%d,%d,%d>\n",
                        _AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N,>
                        _AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2);)
        // handle quantization
        unsigned sh_amt = (F2 > _AP_F) ? F2 - _AP_F : _AP_F - F2;
        bool signbit = _ssdm_op_get_bit(
                        (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>(&op)->V), _AP_W2-1);
        bool isneg = signbit && _AP_S2; 
        if (F2 == _AP_F)
            Base::V = op.V;
        else if (F2 > _AP_F) {
            if (sh_amt < _AP_W2)
                Base::V = op.V >> sh_amt;
            else { 
                static int AllOnesInt = -1;
                if (isneg) Base::V = AllOnesInt; 
                else Base::V = 0;
            }
            if (_AP_Q!=SC_TRN && !(_AP_Q==SC_TRN_ZERO && !_AP_S2)) {
                bool qbit = _ssdm_op_get_bit(
                        (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>(&op)->V),F2-_AP_F-1);
                bool qb = (F2-_AP_F > _AP_W2) ? _AP_S2 && signbit : qbit;
                          
                bool r = (F2 > _AP_F+1) ? 
                         _ssdm_op_get_range(
                        (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>(&op)->V),
                        0,F2-_AP_F-2<_AP_W2?F2-_AP_F-2:_AP_W2-1)!=0 : false;
                carry = quantization_adjust(qb, r, _AP_S2 && signbit);
            }
        }
        else {  // no quantization
            Base::V = op.V;
            if (sh_amt < _AP_W)
                Base::V = Base::V << sh_amt;
            else 
                Base::V = 0;
        }
       
        // handle overflow/underflow
        if ((_AP_O != SC_WRAP || _AP_N != 0) && ((!_AP_S && _AP_S2) || 
             _AP_I-_AP_S < _AP_I2-_AP_S2+(QUAN_INC || 
              (_AP_S2 && _AP_O==SC_SAT_SYM)))) { // saturation
            bool deleted_zeros = _AP_S2?true:!carry,
                 deleted_ones = true;
            bool neg_src = isneg;
            bool lD = false;
            
            bool newsignbit = _ssdm_op_get_bit(Base::V,_AP_W-1);
            int pos1 = F2 - _AP_F + _AP_W;
            int pos2 = F2 - _AP_F + _AP_W + 1;
            if (pos1 < _AP_W2 && pos1 >= 0)
                lD = _ssdm_op_get_bit(
                        (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>(&op)->V),
                         pos1);

            if(pos1 < _AP_W2)
            {
                bool Range1_all_ones = true;
                bool Range1_all_zeros = true; 
                bool Range2_all_ones = true;
                ap_int_base<_AP_W2,false> Range1(0);
                ap_int_base<_AP_W2,false> Range2(0);
                ap_int_base<_AP_W2,false> all_ones(-1);
                
                if (pos2 < _AP_W2 && pos2 >= 0) {
                    Range2.V = _ssdm_op_get_range(
                                   (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>
                                   (&op)->V),pos2,_AP_W2-1);
                    Range2_all_ones = Range2 == (all_ones >> pos2);
                } else if (pos2 < 0) 
                    Range2_all_ones = false;
                 
                if (pos1 >= 0 && pos2 < _AP_W2) {
                    Range1.V = _ssdm_op_get_range(
                                   (const_cast<ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>*>
                                   (&op)->V),pos1,_AP_W2-1);
                     
                    Range1_all_ones = Range1 == (all_ones >> pos1);
                    Range1_all_zeros = !Range1.V ;
                } else if (pos2 == _AP_W2) {
                    Range1_all_ones = lD;
                    Range1_all_zeros = !lD;
                } else if (pos1 < 0) {
                    Range1_all_zeros = !op.V;
                    Range1_all_ones = false;
                }
                    
               
                deleted_zeros = deleted_zeros && (carry ? Range1_all_ones: Range1_all_zeros);
                deleted_ones = carry ? Range2_all_ones && 
                               (pos1 < 0  || !lD): Range1_all_ones;
                neg_src = isneg && !(carry&&Range1_all_ones);
            } else
                neg_src = isneg && newsignbit;
            bool neg_trg = _AP_S && newsignbit; 
            bool overflow = (neg_trg || !deleted_zeros) && !isneg;
            bool underflow = (!neg_trg || !deleted_ones) && neg_src;
            if ((_AP_O == SC_SAT_SYM) && _AP_S2 && _AP_S) 
                underflow |= neg_src && (_AP_W > 1 ? 
                  _ssdm_op_get_range(Base::V, 0, _AP_W - 2) == 0 : true);
            
            overflow_adjust(underflow, overflow, lD, neg_src);
        }
    }
    
    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base(const volatile ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> &op) {
        *this = const_cast<ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>&>(op);
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE ap_fixed_base (const ap_int_base<_AP_W2,_AP_S2>& op) {
        AP_DEBUG(printf("C3: af1=<%d,%d,%d,%d,%d>\n", _AP_W, _AP_I, _AP_S, _AP_Q, _AP_O););
        ap_fixed_base<_AP_W2,_AP_W2,_AP_S2> f_op;
        f_op.V = op.V;
        *this = f_op;
    }

    INLINE ap_fixed_base( bool b ) { *this = (ap_fixed_base<1, 1, false>) b; }
    INLINE ap_fixed_base( char b ) { *this = (ap_fixed_base<8, 8, true>) b; }
    INLINE ap_fixed_base( signed char b ) { *this = (ap_fixed_base<8, 8, true>) b; }
    INLINE ap_fixed_base( unsigned char b ) { *this = (ap_fixed_base<8, 8, false>) b; }
    INLINE ap_fixed_base( signed short b ) { *this = (ap_fixed_base<16, 16, true>) b; }
    INLINE ap_fixed_base( unsigned short b ) { *this = (ap_fixed_base<16, 16, false>) b; }
    INLINE ap_fixed_base( signed int b ) { *this = (ap_fixed_base<32, 32, true>) b; }
    INLINE ap_fixed_base( unsigned int b ) { *this = (ap_fixed_base<32, 32, false>) b; }
# if defined __x86_64__
    INLINE ap_fixed_base( signed long b ) { *this = (ap_fixed_base<64, 64, true>) b; }
    INLINE ap_fixed_base( unsigned long b ) { *this = (ap_fixed_base<64, 64, false>) b; }
#else
    INLINE ap_fixed_base( signed long b ) { *this = (ap_fixed_base<32, 32, true>) b; }
    INLINE ap_fixed_base( unsigned long b ) { *this = (ap_fixed_base<32, 32, false>) b; }
#endif
    INLINE ap_fixed_base( ap_slong b ) { *this = (ap_fixed_base<64, 64, true>) b; }
    INLINE ap_fixed_base( ap_ulong b ) { *this = (ap_fixed_base<64, 64, false>) b; }
   INLINE ap_fixed_base(const char* str) {
        typeof(Base::V) Result;
        _ssdm_string2bits((void*)(&Result), (const char*)(str), 
         10, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N, _AP_C99);
        Base::V = Result;
    }

    INLINE ap_fixed_base(const char* str, signed char radix) {
        typeof(Base::V) Result;
        _ssdm_string2bits((void*)(&Result), (const char*)(str), 
            radix, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N, _AP_C99);
        Base::V = Result;
    }
    
    template<int _AP_W2, bool _AP_S2>
    INLINE ap_fixed_base(const ap_bit_ref<_AP_W2, _AP_S2>& op) {
        *this = ((bool)op);
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE ap_fixed_base(const ap_range_ref<_AP_W2, _AP_S2>& op) {
        *this = (ap_int_base<_AP_W2, false>(op));
    }

    template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
    INLINE ap_fixed_base(const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& op) {
        *this = (ap_int_base<_AP_W2 + _AP_W3, false>(op));
 
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
         *this = (bool(op));
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
        *this = (ap_int_base<_AP_W2, false>(op));
    } 
    
    // FIXME: This is only for internal use. Clang checker.
    #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
    #else
    INLINE ap_fixed_base(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val) {}   
    #endif
    
    // helper function.
    INLINE unsigned long long doubleToRawBits(double pf) const {
        union {
            unsigned long long __L;
            double __D;
        } LD;
        LD.__D = pf;
        return LD.__L;
    }
    INLINE unsigned int floatToRawBits(float pf) const {
        union {
            unsigned int __L;
            float __D;
        } LD;
        LD.__D = pf;
        return LD.__L;
    }
    INLINE unsigned short halfToRawBits(half pf) const {
        union {
            unsigned short __L;
            half __D;
        } LD;
        LD.__D = pf;
        return LD.__L;
    }

    INLINE double rawBitsToDouble(unsigned long long pi) const {
        union {
            unsigned long long __L;
            double __D;
        } LD;
        LD.__L = pi;
        return LD.__D;
    }

    INLINE float rawBitsToFloat (unsigned int pi) const {
        union {
            unsigned int  __L;
            float __D;
        } LD;
        LD.__L = pi;
        return LD.__D;
    }

    INLINE half rawBitsToHalf (unsigned short pi) const {
        union {
            unsigned short  __L;
            half __D;
        } LD;
        LD.__L = pi;
        return LD.__D;
    }

    AP_WEAK ap_fixed_base(double d) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        ap_int_base<64,false> ireg;
        ireg.V = doubleToRawBits(d);
        bool isneg = _ssdm_op_get_bit(ireg.V, 63);

        ap_int_base<DOUBLE_EXP + 1, true> exp;
        ap_int_base<DOUBLE_EXP, false> exp_tmp;
        exp_tmp.V = _ssdm_op_get_range(ireg.V, DOUBLE_MAN, 
               DOUBLE_MAN + DOUBLE_EXP -1);
        exp = exp_tmp - DOUBLE_BIAS;
        ap_int_base<DOUBLE_MAN + 2, true> man;
        man.V = _ssdm_op_get_range(ireg.V, 0, DOUBLE_MAN - 1);
        //do not support NaN
        AP_ASSERT(exp != APFX_IEEE_DOUBLE_E_MAX + 1 || man.V == 0, 
                  "assign NaN to fixed point value");
        man.V = _ssdm_op_set_bit(man.V, DOUBLE_MAN, 1);
        if(isneg) man = -man;
        if ( (ireg.V & 0x7fffffffffffffffLL)==0 ) {
            Base::V = 0;
        } else {
            int _AP_W2=DOUBLE_MAN+2, _AP_I2=exp.V+2, _AP_F=_AP_W-_AP_I, F2=_AP_W2-_AP_I2;
            bool _AP_S2 = true,
                 QUAN_INC = F2>_AP_F && !(_AP_Q==SC_TRN || (_AP_Q==SC_TRN_ZERO && !_AP_S2));
            bool carry = false;
            // handle quantization
            unsigned sh_amt = (F2 > _AP_F) ? F2 - _AP_F : _AP_F - F2;
            if (F2 == _AP_F)
                Base::V = man.V;
            else if (F2 > _AP_F) {
                if (sh_amt < DOUBLE_MAN + 2)
                   Base::V = man.V >> sh_amt;
                else { 
                   static int AllOnesInt = -1;
                   if (isneg) Base::V = AllOnesInt; 
                   else Base::V = 0;
                }
                if ((_AP_Q != SC_TRN) && !((_AP_Q == SC_TRN_ZERO) && !_AP_S2)) {
                    
                    bool qb = (F2-_AP_F > _AP_W2) ? 
                        isneg : (bool) _ssdm_op_get_bit(man.V, F2 - _AP_F - 1);
                    bool r = (F2 > _AP_F + 1) ? _ssdm_op_get_range(man.V, 
                             0, (F2 - _AP_F - 2 < _AP_W2) ? (F2 - _AP_F - 2): (_AP_W2 - 1)) !=
                             0 : false;
                    carry = quantization_adjust(qb, r, isneg);
                }
            }
            else { // no quantization 
                Base::V = man.V;
                if (sh_amt < _AP_W)
                    Base::V = Base::V << sh_amt;
                else 
                    Base::V = 0;
            }
            // handle overflow/underflow
            if ((_AP_O != SC_WRAP || _AP_N != 0) && ((!_AP_S && _AP_S2) 
                || _AP_I - _AP_S < _AP_I2 - _AP_S2 + (QUAN_INC || 
                (_AP_S2 && (_AP_O == SC_SAT_SYM)))) ) { // saturation
                bool deleted_zeros = _AP_S2?true:!carry,
                     deleted_ones = true;
                bool neg_src = isneg;
                bool lD = false;
                int pos1 =F2 - _AP_F + _AP_W;
                int pos2 =F2 - _AP_F + _AP_W + 1;
                bool newsignbit = _ssdm_op_get_bit(Base::V, _AP_W - 1);
                if (pos1 < _AP_W2 && pos1 >= 0)
                  //lD = _ssdm_op_get_bit(man.V, pos1);
                  lD = (man.V >> pos1) & 1;
                if (pos1 < _AP_W2 ) {
                    bool Range1_all_ones = true;
                    bool Range1_all_zeros = true;
                    bool Range2_all_ones = true;
                    ap_int_base<DOUBLE_MAN+2,false> Range2;
                    ap_int_base<DOUBLE_MAN+2,false> all_ones(-1);

                    if (pos2 >= 0 && pos2 < _AP_W2) {
                        //Range2.V = _ssdm_op_get_range(man.V, 
                        //                        pos2, _AP_W2 - 1);
                        Range2.V = man.V;
                        Range2.V >>= pos2;
                        Range2_all_ones = Range2 == (all_ones >> pos2);
                    } else if (pos2 < 0)  
                        Range2_all_ones = false;
                    if (pos1 >= 0 && pos2 < _AP_W2) {
                        Range1_all_ones = Range2_all_ones && lD;
                        Range1_all_zeros = !Range2.V && !lD;
                    } else if (pos2 == _AP_W2) {
                        Range1_all_ones = lD;
                        Range1_all_zeros = !lD;
                    } else if (pos1 < 0) {
                        Range1_all_zeros = !man.V;
                        Range1_all_ones = false;
                    }
                        
                    deleted_zeros = deleted_zeros && (carry ? Range1_all_ones: Range1_all_zeros);
                    deleted_ones = carry ? Range2_all_ones &&
                                   ( pos1 < 0 || !lD): Range1_all_ones;
                    neg_src=isneg && !(carry&&Range1_all_ones);
                } else
                    neg_src = isneg && newsignbit;
                bool neg_trg = _AP_S && newsignbit;
                bool overflow = (neg_trg || !deleted_zeros) && !isneg;
                bool underflow =(!neg_trg || !deleted_ones) && neg_src;
                if ((_AP_O == SC_SAT_SYM) && _AP_S2 && _AP_S) 
                   underflow |= neg_src && (_AP_W > 1 ? 
                     _ssdm_op_get_range(Base::V, 0, _AP_W - 2) == 0 : true);
                overflow_adjust(underflow, overflow, lD, neg_src);
            }
        }
    }

    INLINE ap_fixed_base(float d) {
        *this = ap_fixed_base(double(d));
    }

    INLINE ap_fixed_base(half d) {
        *this = ap_fixed_base(double(d));
    }

    INLINE ap_fixed_base& operator=(const ap_fixed_base<_AP_W, _AP_I, _AP_S,
                                        _AP_Q, _AP_O, _AP_N>& op)
    {
        Base::V = op.V;
        return *this;
    }

    INLINE ap_fixed_base& operator=(const volatile ap_fixed_base<_AP_W, _AP_I, _AP_S,
                                        _AP_Q, _AP_O, _AP_N>& op)
    {
        Base::V = op.V;
        return *this;
    }
                                                                                           
    INLINE void operator=(const ap_fixed_base<_AP_W, _AP_I, _AP_S,
                                        _AP_Q, _AP_O, _AP_N>& op) volatile
    {
        Base::V = op.V;
    }

    INLINE void operator=(const volatile ap_fixed_base<_AP_W, _AP_I, _AP_S,
                                        _AP_Q, _AP_O, _AP_N>& op) volatile
    {
        Base::V = op.V;
    }
                                                                                           
    // Set this ap_fixed_base with a bits string. That means the ssdm_int::V
    // inside this ap_fixed_base is assigned by bv.
    // Note the input parameter should be a fixed-point formatted bit string.
    INLINE ap_fixed_base& setBits(unsigned long long bv) {
      Base::V = bv;
      return *this;
    }
    // Return a ap_fixed_base object whose ssdm_int::V is assigned by bv.
    // Note the input parameter should be a fixed-point formatted bit string.
    static INLINE ap_fixed_base bitsToFixed(unsigned long long bv) {
      ap_fixed_base Tmp;
      Tmp.V = bv;
      return Tmp;
    }

    // Explicit conversion functions to ap_int_base that captures 
    // all integer bits (bits are truncated)
    INLINE ap_int_base<AP_MAX(_AP_I,1),_AP_S> 
    to_ap_int_base(bool Cnative = true) const {
        //return ap_int_base<AP_MAX(_AP_I,1),_AP_S>(_AP_I > 1 ? 
        // _ssdm_op_get_range(const_cast<ap_fixed_base*>(this)->Base::V,_AP_W-_AP_I,_AP_W-1) : 0);
        ap_int_base<AP_MAX(_AP_I,1),_AP_S> ret(0);
        if(_AP_I > 0 && _AP_I <= _AP_W)
            ret.V = _ssdm_op_get_range(const_cast<
                    ap_fixed_base*>(this)->Base::V,
                    _AP_W - _AP_I, _AP_W - 1);
        else if (_AP_I > _AP_W)
        {
            ret.V = _ssdm_op_get_range(const_cast<
                   ap_fixed_base*>(this)->Base::V, 0, _AP_W - 1);
            unsigned int shift = _AP_I - _AP_W;
            ret.V <<= shift;
        }
        if (Cnative) {
            //Follow C native data type, conversion from double to int
            if (_AP_S && _ssdm_op_get_bit(const_cast<
                ap_fixed_base*>(this)->Base::V, _AP_W - 1)
                && (_AP_I < _AP_W) && (_ssdm_op_get_range(
                const_cast<ap_fixed_base*>(this)->Base::V, 
                0, _AP_I >= 0 ? _AP_W - _AP_I - 1: _AP_W - 1) != 0))
                ret.V += 1;
        } else { 
        //Follow OSCI library, conversion from sc_fixed to sc_int
        }
        return ret; 
    };

    template<int _AP_W2, bool _AP_S2>
    INLINE operator ap_int_base<_AP_W2,_AP_S2> () const {
        return (ap_int_base<_AP_W2,_AP_S2>)to_ap_int_base();
    }

    // Explicit conversion function to C built-in integral type.
    INLINE int to_int() const { 
        return to_ap_int_base().to_int();
    }
    INLINE unsigned to_uint() const {
        return to_ap_int_base().to_uint();
    }
    INLINE ap_slong to_int64() const {
        return to_ap_int_base().to_int64();
    }
    INLINE ap_ulong to_uint64() const {
        return to_ap_int_base().to_uint64();
    }
    AP_WEAK double to_double() const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        if (_AP_W - _AP_I > 0 && _AP_W <= 64) {
            if (!Base::V) 
                return 0;
            double dp = Base::V;
            ap_int_base<64,true> res;
            res.V = doubleToRawBits(dp);
            ap_int_base<DOUBLE_EXP, true> exp;
            exp.V = _ssdm_op_get_range(res.V,DOUBLE_MAN, 62);
            exp -= _AP_W - _AP_I;
            res.V = _ssdm_op_set_range(res.V, DOUBLE_MAN, 62, exp.V);
            dp = rawBitsToDouble(res.to_int64());
            return dp;
        } else if (_AP_I - _AP_W >= 0 && _AP_I <= 64) {
            ap_int_base<AP_MAX(1, _AP_I), _AP_S> temp;
            temp.V = Base::V;
            temp <<= _AP_I - _AP_W;
            double dp = temp.V;
            return dp;
        } else {
            if (!Base::V) 
                return 0;
            ap_int_base<64,true> res;
            res.V = 0;
            bool isneg = _AP_S ? _ssdm_op_get_bit(
                    const_cast<ap_fixed_base*>(this)->V, _AP_W - 1) : false;
            ap_int_base<_AP_W+_AP_S,_AP_S> tmp;
            tmp.V = Base::V;
            if (isneg) tmp.V = -Base::V;

            res.V = _ssdm_op_set_bit(res.V,63,isneg);
            int j = _AP_W+_AP_S-1-tmp.countLeadingZeros();

            int exp = _AP_I-(_AP_W-j);
            res.V = _ssdm_op_set_range(res.V,DOUBLE_MAN, 62, exp + DOUBLE_BIAS);
            if (j == 0)
                res.V = _ssdm_op_set_range(res.V, 0, DOUBLE_MAN - 1,0);
            else {
                ap_int_base<DOUBLE_MAN,false> man;
                man.V = _ssdm_op_get_range(tmp.V, j > DOUBLE_MAN ? 
                        j - DOUBLE_MAN : 0, j - 1);
                man.V <<= DOUBLE_MAN > j ? DOUBLE_MAN-j : 0;
                res.V = _ssdm_op_set_range (res.V, 0, DOUBLE_MAN - 1, man.V);
            }
            double dp = rawBitsToDouble(res.to_int64());
            return dp;
        }

    }

    AP_WEAK float to_float() const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        if (!Base::V) return 0;
        /*
           When ap_fixed is wide, float cannot hold all its precision bits.
           To make rounding consistent, the following code first constructs
           an unsigned integer of 32 bits, and then utilize its conversion to float.
        */
        bool is_neg =
            _AP_S && _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, _AP_W - 1);
        ap_int_base<_AP_W, false> tmp;
        tmp.V = (is_neg ? -Base::V : Base::V);
        int num_zeros = tmp.countLeadingZeros();
        int msb_idx = _AP_W - 1 - num_zeros;
        int exp = _AP_I - (_AP_W - msb_idx);
        msb_idx = (msb_idx < 0) ? 0 : msb_idx;
        ap_int_base<32, false> tmp32;
        if (msb_idx < 32) {
          tmp32.V = tmp.V;
          tmp32.V <<= (31 - msb_idx);
        } else {
          // truncate when ap_fixed has too much precision bits.
          tmp32.V = _ssdm_op_get_range(tmp.V, msb_idx - 31, msb_idx);
        }
        float f = tmp32.V; // set mantissa, rounding may happen here
        tmp32.V = floatToRawBits(f);
        int has_carry;
        has_carry = _ssdm_op_get_range(tmp32.V, FLOAT_MAN, 30) !=
            (31 + FLOAT_BIAS); // check carry in rounding
        tmp32.V = _ssdm_op_set_range(tmp32.V, FLOAT_MAN, 30,
                                     exp + has_carry + FLOAT_BIAS); // set exponent
        tmp32.V = _ssdm_op_set_bit(tmp32.V, 31, is_neg); // handle sign
        f = rawBitsToFloat(tmp32.to_uint());
        return f;
    }

  /// convert function to half.
  /** only round-half-to-even mode supported, does not obey FE env. */
  INLINE half to_half() const {
    enum { BITS = HALF_MAN + HALF_EXP + 1 };
    if (!Base::V) return 0.0f;
    ap_int_base<_AP_W, false> tmp;
    tmp.V = Base::V;
    bool s = _AP_S && tmp.get_bit(_AP_W - 1); ///< sign.
    if (s)
      tmp.V = -Base::V; // may truncate one bit extra from neg in sim.
    int l = tmp.countLeadingZeros();  ///< number of leading zeros.
    int e = _AP_I - l - 1 + HALF_BIAS; ///< exponent
    int lsb_index = _AP_W - l - 1 - HALF_MAN;
    // more than 0.5?
    bool a = (lsb_index >=2) ?
        (tmp.range(lsb_index - 2, 0) != 0) : 0;
    // round to even
    a |= (lsb_index >=0) ? tmp.get_bit(lsb_index) : 0;
    // short is at least 16-bit
    unsigned short m;
    // may actually left shift, ensure buffer is wide enough.
    if (_AP_W > BITS) {
      m = (lsb_index >= 1) ? (unsigned short)(tmp.V >> (lsb_index - 1))
                           : (unsigned short)(tmp.V << (1 - lsb_index));
    } else {
      m = (unsigned short)tmp.V;
      m = (lsb_index >= 1) ? (m >> (lsb_index - 1))
                           : (m << (1 - lsb_index));
    }
    m += a;
    m >>= 1;
    // carry to MSB, increase exponent
    if (_ssdm_op_get_bit(m, HALF_MAN + 1)) {
      e += 1;
    }
    // set sign and exponent
    m = _ssdm_op_set_bit(m, BITS - 1, s);
    m = _ssdm_op_set_range(m, HALF_MAN, HALF_MAN + HALF_EXP - 1, e);
    // cast to fp
    return rawBitsToHalf(m);
  }

    INLINE  operator double () const {
        return to_double();
    }
#ifndef __SC_COMPATIBLE__
    INLINE  operator float () const {
        return to_float();
    }
    INLINE  operator half () const {
        return to_half();
    }

    INLINE operator bool () const {
      return (bool) Base::V != 0;
    }

    INLINE  operator char () const {
        return (char) to_int();
    }

    INLINE  operator signed char () const {
        return (signed char) to_int();
    }

    INLINE operator unsigned char () const {
        return (unsigned char) to_uint();
    }

    INLINE operator short () const {
        return (short) to_int();
    }

    INLINE operator unsigned short () const {
        return (unsigned short) to_uint();
    }


    INLINE  operator  int () const {
        return to_int();
    }

    INLINE  operator unsigned int () const {
        return to_uint();
    }
#if 1
# if defined __x86_64__
    INLINE operator long () const {
        return (long)to_int64();
    }

    INLINE operator unsigned long () const {
        return (unsigned long) to_uint64();
    }
# else 
    INLINE operator long () const {
        return (long)to_int();
    }

    INLINE operator unsigned long () const {
        return (unsigned long) to_uint();
    }
# endif
#endif
    INLINE operator unsigned long long  () const {
        return to_uint64();
    }
   
    INLINE operator long long () const {
        return to_int64();
    }
#endif
    INLINE int length() const { return _AP_W; };
    // Count the number of zeros from the most significant bit 
    // to the first one bit. Note this is only for ap_fixed_base whose 
    // _AP_W <= 64, otherwise will incur assertion.
    INLINE int countLeadingZeros() {
        if (_AP_W <= 32) {
            ap_int_base<32, false> t(-1ULL);
            t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
            return __builtin_ctz(t.V);
        } else if (_AP_W <= 64) {
            ap_int_base<64, false> t(-1ULL);
            t.range(_AP_W-1, 0) = this->range(0, _AP_W-1);
            return __builtin_ctzll(t.V);
        } else {
            enum { __N = (_AP_W+63)/64 };
            int NZeros = 0;
            unsigned i = 0;
            bool hitNonZero = false;
            for (i=0; i<__N-1; ++i) {
                ap_int_base<64, false> t;
                t.range(0, 63) = this->range(_AP_W - i*64 - 64, _AP_W - i*64 - 1);
                NZeros += hitNonZero?0:__builtin_clzll(t.V);
                hitNonZero |= (t != 0);
            }
            if (!hitNonZero) {
                ap_int_base<64, false> t(-1ULL);
                t.range(63-(_AP_W-1)%64, 63) = this->range(0, (_AP_W-1)%64);
                NZeros += __builtin_clzll(t.V);
            }
            return NZeros;
        }
    }

    // Arithmetic : Binary
    // -------------------------------------------------------------------------
    template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2>
    INLINE typename RType<_AP_W2,_AP_I2,_AP_S2>::mult
    operator *(const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op2) const {
        typename RType<_AP_W2,_AP_I2,_AP_S2>::mult r; 
        ap_int_base<_AP_W+_AP_W2,_AP_S> OP1;
        OP1.V = Base::V; 
        ap_int_base<_AP_W+_AP_W2,_AP_S2> OP2;
        OP2.V = op2.V ;
        r.V = OP1.V * OP2.V;
        return r;
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2>
    INLINE typename RType<_AP_W2,_AP_I2,_AP_S2>::div
    operator /(const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op2) const {
        typename RType<_AP_W2,_AP_I2,_AP_S2>::div r;
#ifndef __SC_COMPATIBLE__
        ap_fixed_base<_AP_W + AP_MAX(_AP_W2 - _AP_I2, 0), 
                      _AP_I, _AP_S> t(*this);
#else 
        ap_fixed_base<_AP_W + AP_MAX(_AP_W2 - _AP_I2, 0) + AP_MAX(_AP_I2, 0), 
                      _AP_I, _AP_S> t(*this);
#endif 
        r.V = t.V / op2.V;
        //r = double(to_double() / op2.to_double());
        return r;
    }

#define OP_BIN_AF(Sym, Rty) \
    template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2> \
    INLINE typename RType<_AP_W2,_AP_I2,_AP_S2>::Rty \
    operator Sym (const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op2) const { \
        AP_DEBUG(printf("call fixed operator %s %s\n", #Sym, #Rty);); \
        enum { _AP_F = _AP_W-_AP_I, F2 = _AP_W2-_AP_I2 }; \
        AP_DEBUG(printf("done?\n", _AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2);); \
        typename RType<_AP_W2,_AP_I2,_AP_S2>::Rty r, lhs(*this), rhs(op2); \
        AP_DEBUG(printf("op2<%d,%d,%d,%d,%d>\n", _AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2);); \
        r.V = lhs.V Sym rhs.V; \
        return r; \
    }

    OP_BIN_AF(+, plus)
    OP_BIN_AF(-, minus)
    OP_BIN_AF(&, logic)
    OP_BIN_AF(|, logic)
    OP_BIN_AF(^, logic)
        

    // Arithmetic : assign
    // -------------------------------------------------------------------------
#define OP_ASSIGN_AF(Sym1, Sym2) \
    template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2> \
    INLINE ap_fixed_base& \
    operator Sym1 (const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2,_AP_N2>& op2) { \
        AP_DEBUG(printf("call assign op %s\n", #Sym1);); \
        *this = operator Sym2 (op2); \
        return *this; \
    }

    OP_ASSIGN_AF(+=, +)
    OP_ASSIGN_AF(-=, -)
    OP_ASSIGN_AF(*=, *)
    OP_ASSIGN_AF(/=, /)
    OP_ASSIGN_AF(&=, &)
    OP_ASSIGN_AF(|=, |)
    OP_ASSIGN_AF(^=, ^)


    // Prefix increment, decrement.
    // -------------------------------------------------------------------------
    INLINE ap_fixed_base& operator ++() {
        operator+=(ap_fixed_base<_AP_W-_AP_I+1,1,false>(1));
        return *this;
    }
    INLINE ap_fixed_base& operator --() {
        operator-=(ap_fixed_base<_AP_W-_AP_I+1,1,false>(1));
        return *this;
    }

    // Postfix increment, decrement
    // -------------------------------------------------------------------------
    INLINE const ap_fixed_base 
    operator ++(int) {
        ap_fixed_base t(*this);
        operator++();
        return t;
    }
    INLINE const ap_fixed_base 
    operator --(int) {
        ap_fixed_base t(*this);
        operator--();
        return t;
    }
    
    // Unary arithmetic.
    // -------------------------------------------------------------------------
    INLINE ap_fixed_base operator +() {
        return *this;
    }

    INLINE ap_fixed_base<_AP_W + 1, _AP_I + 1, true> operator -() const {
        ap_fixed_base<_AP_W + 1, _AP_I + 1, true> ret(*this);
        ret.V = - ret.V;
        return ret;
    }

    INLINE ap_fixed_base<_AP_W,_AP_I,true,_AP_Q,_AP_O,_AP_N> getNeg() {
        ap_fixed_base<_AP_W,_AP_I,true,_AP_Q,_AP_O,_AP_N> Tmp(*this);
        Tmp.V = -Tmp.V;
        return Tmp;
    }

    // Not (!)
    // -------------------------------------------------------------------------
    INLINE bool operator !() const {
        return Base::V == 0;
    }

    // Bitwise complement
    // -------------------------------------------------------------------------
    INLINE ap_fixed_base<_AP_W, _AP_I, _AP_S> operator ~() const {
        ap_fixed_base<_AP_W, _AP_I, _AP_S> ret;
        ret.V=~Base::V;
        return ret;
    }

    // Shift
    // -------------------------------------------------------------------------
     template<int _AP_SHIFT>
     INLINE ap_fixed_base<_AP_W, _AP_I + _AP_SHIFT, _AP_S> lshift () const {
         ap_fixed_base<_AP_W, _AP_I + _AP_SHIFT, _AP_S> r;
         r.V = Base::V;
         return r;
     }
     
     template<int _AP_SHIFT>
     INLINE ap_fixed_base<_AP_W, _AP_I - _AP_SHIFT, _AP_S> rshift () const {
         ap_fixed_base<_AP_W, _AP_I - _AP_SHIFT, _AP_S> r;
         r.V = Base::V;
         return r;
     }

     AP_WEAK ap_fixed_base 
     operator << (int sh) const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        ap_fixed_base r;
        bool isNeg = sh & 0x80000000;
        sh = isNeg ? -sh : sh;
        if (isNeg) r.V = Base::V >> sh;
        else r.V = Base::V << sh;
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return r;
        if (isNeg && _AP_Q != SC_TRN) {
           bool qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           bool rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0;
           r.quantization_adjust(qb, rb, _AP_S &&
                                  _ssdm_op_get_bit(Base::V, _AP_W-1));
        } else if (!isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
              bool neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
              bool allones, allzeros;
              ap_int_base<_AP_W, false> ones(-1);
              if (sh <= _AP_W) {
                   ap_int_base<_AP_W,false> range1;
                   range1.V = _ssdm_op_get_range(const_cast<ap_fixed_base*>
                              (this)->Base::V, _AP_W - sh, _AP_W - 1);

                   allones = range1 == (ones >>(_AP_W - sh));
                   allzeros = range1 == 0;
              } else {
                   allones = false;
                   allzeros = Base::V == 0;
              }
              bool overflow = !allzeros && !neg_src;
              bool underflow =!allones && neg_src;
              if ((_AP_O == SC_SAT_SYM) && _AP_S)
                      underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                   r.V, 0, _AP_W - 2) == 0: true);
              bool lD = false;
              if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);
              r.overflow_adjust(underflow, overflow, lD, neg_src);
        }
#endif
        return r;

    }

    template<int _AP_W2>
    INLINE ap_fixed_base 
    operator << (const ap_int_base<_AP_W2,true>& op2) const {
        int sh = op2.to_int();
        return operator << (sh);
    }
    
    AP_WEAK ap_fixed_base 
    operator << (unsigned int sh) const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        ap_fixed_base r;
        r.V = Base::V << sh;
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return r;
        if (_AP_O != SC_WRAP || _AP_N != 0) {
             bool neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
             bool allones, allzeros;
             ap_int_base<_AP_W, false> ones(-1);
             if (sh <= _AP_W) {
                  ap_int_base<_AP_W,false> range1;
                  range1.V = _ssdm_op_get_range(
                    const_cast<ap_fixed_base*>(this)->Base::V, 
                    _AP_W - sh, _AP_W - 1);
                  allones = range1 == (ones >> (_AP_W - sh));
                  allzeros = range1 == 0;
             } else {
                  allones = false;
                  allzeros = Base::V == 0;
             }
             bool overflow = !allzeros && !neg_src;
             bool underflow = !allones && neg_src;
             if ((_AP_O == SC_SAT_SYM) && _AP_S)
                     underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                  r.V, 0, _AP_W - 2) == 0: true);
             bool lD = false;
             if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);
             r.overflow_adjust(underflow, overflow, lD, neg_src);
        }
#endif
        return r;
    }

    template<int _AP_W2>
    INLINE ap_fixed_base 
    operator << (const ap_int_base<_AP_W2,false>& op2) const {
        unsigned int sh = op2.to_uint();
        return operator << (sh);
    }

    AP_WEAK ap_fixed_base 
    operator >> (int sh) const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        ap_fixed_base r;
        bool isNeg = sh & 0x80000000;
        sh = isNeg ? -sh : sh;
        if (isNeg) r.V = Base::V << sh;
        else r.V = Base::V >> sh;
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return r;
        if (!isNeg && _AP_Q != SC_TRN) {
           bool qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           bool rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(const_cast<
                     ap_fixed_base*>(this)->Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0; 
           r.quantization_adjust(qb, rb, _AP_S && _ssdm_op_get_bit(Base::V, _AP_W-1));
        } else if (isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
            bool neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
            bool allones, allzeros;
            ap_int_base<_AP_W, false> ones(-1);
            if (sh <= _AP_W) {
                 ap_int_base<_AP_W,false> range1;
                 range1.V  = _ssdm_op_get_range(
                         const_cast<ap_fixed_base*>(this)->Base::V, 
                         _AP_W - sh, _AP_W - 1);
                 allones = range1 == (ones >>(_AP_W - sh));
                 allzeros = range1 == 0;
            } else {
                 allones = false;
                 allzeros = Base::V == 0;
            }
            bool overflow = !allzeros && !neg_src;
            bool underflow =!allones && neg_src;
            if ((_AP_O == SC_SAT_SYM) && _AP_S)
                    underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                 r.V, 0, _AP_W - 2) == 0: true);
            bool lD = false;
            if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);
            r.overflow_adjust(underflow, overflow, lD, neg_src);
        }
#endif
        return r;
    }

    template<int _AP_W2>
    INLINE ap_fixed_base 
    operator >> (const ap_int_base<_AP_W2,true>& op2) const {
        int sh = op2.to_int();
        return operator >> (sh);
    }

    AP_WEAK ap_fixed_base 
    operator >> (unsigned sh) const {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        ap_fixed_base r;
        r.V = Base::V >> sh;
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return r;
        if ( _AP_Q != SC_TRN) {
           bool qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           bool rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(const_cast<
                     ap_fixed_base*>(this)->Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0; 
           r.quantization_adjust(qb, rb, _AP_S && _ssdm_op_get_bit(Base::V, _AP_W-1));
        }
#endif
        return r;
    }
    
    template<int _AP_W2>
    INLINE ap_fixed_base 
    operator >> (const ap_int_base<_AP_W2,false>& op2) const {
        unsigned int sh =  op2.to_uint();
        return operator >> (sh);
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base
    operator >> (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2,
                 _AP_N2>& op2) {
        return operator >> (op2.to_ap_int_base());
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base
    operator << (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2,
                 _AP_N2>& op2) {
        return operator << (op2.to_ap_int_base());
    }



    // Shift assign
    // -------------------------------------------------------------------------
    AP_WEAK ap_fixed_base& 
    operator <<= (int sh) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        if (sh == 0) return *this;
        bool isNeg = sh & 0x80000000;
        sh = isNeg ? -sh : sh;
#ifdef __SC_COMPATIBLE__
        bool qb, rb, sb;
        bool neg_src, allones, allzeros, lD;
        if (isNeg && _AP_Q != SC_TRN) {
           qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0; 
           sb = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
        } else if (!isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
              neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
              ap_int_base<_AP_W, false> ones(-1);
              if (sh <= _AP_W) {
                   ap_int_base<_AP_W,false> range1;
                   range1.V  = _ssdm_op_get_range(Base::V, 
                                            _AP_W - sh, _AP_W - 1);
                   allones = range1 == (ones >> (_AP_W -sh));
                   allzeros = range1 == 0;
              } else {
                   allones = false;
                   allzeros = Base::V == 0;
              }
              lD = false;
              if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);
        }

#endif
        if (isNeg) Base::V >>= sh;
        else Base::V <<= sh;
#ifdef __SC_COMPATIBLE__
        if (isNeg && _AP_Q != SC_TRN) 
           quantization_adjust(qb, rb, sb);
        else if ( !isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
            bool overflow = !allzeros && !neg_src;
            bool underflow = !allones && neg_src;
            if ((_AP_O == SC_SAT_SYM) && _AP_S)
                    underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                Base::V, 0, _AP_W - 2) == 0: true);
            overflow_adjust(underflow, overflow, lD, neg_src);
         }
#endif  
        return *this;
    }

    template<int _AP_W2>
    INLINE ap_fixed_base& 
    operator <<= (const ap_int_base<_AP_W2,true>& op2) {
        int sh = op2.to_int();
        return operator <<= (sh);
    }

    AP_WEAK ap_fixed_base& 
    operator <<= (unsigned int sh) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return *this;
        bool neg_src, allones,allzeros, lD;
        if ( _AP_O != SC_WRAP || _AP_N != 0) {
              neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
              ap_int_base<_AP_W, false> ones(-1);
              if (sh <= _AP_W) {
                   ap_int_base<_AP_W,false> range1;
                   range1.V  = _ssdm_op_get_range(Base::V, _AP_W - sh, _AP_W - 1);
                   allones = range1 == (ones >> (_AP_W - sh));
                   allzeros = range1 == 0;
              } else {
                   allones = false;
                   allzeros = Base::V == 0;
              }
              lD = false;
              if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);
            
        }
#endif
        Base::V <<= sh;
#ifdef __SC_COMPATIBLE__
        if ( _AP_O != SC_WRAP || _AP_N != 0) {
              bool overflow = !allzeros && !neg_src;
              bool underflow = !allones && neg_src;
              if ((_AP_O == SC_SAT_SYM) && _AP_S)
                      underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                  Base::V, 0, _AP_W - 2) == 0: true);
              overflow_adjust(underflow, overflow, lD, neg_src);
            
        }
#endif
        return *this;
    }

    template<int _AP_W2>
    INLINE ap_fixed_base&
    operator <<= (const ap_int_base<_AP_W2,false>& op2) {
        unsigned int sh = op2.to_uint();
        return operator <<= (sh);
    }

    AP_WEAK ap_fixed_base& 
    operator >>= (int sh) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
        if (sh == 0) return *this;
        bool isNeg = sh & 0x80000000;
        sh = isNeg ? -sh : sh;
#ifdef __SC_COMPATIBLE__
        bool qb, rb, sb;
        bool neg_src, allones,allzeros, lD;
        if (!isNeg && _AP_Q != SC_TRN) {
           qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0; 
           sb = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
        } else if (isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
              neg_src = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
              ap_int_base<_AP_W, false> ones(-1);
              if (sh <= _AP_W) {
                   ap_int_base<_AP_W,false> range1;
                   range1.V = _ssdm_op_get_range(Base::V, _AP_W - sh, _AP_W - 1);
                   allones = range1 == (ones >> (_AP_W - sh));
                   allzeros = range1 == 0;
              } else {
                   allones = false;
                   allzeros = Base::V == 0;
              }
              lD = false;
              if (sh < _AP_W) lD =  _ssdm_op_get_bit(Base::V, _AP_W - sh -1);

        }
#endif
        if (isNeg) Base::V <<= sh;
        else Base::V >>= sh;
#ifdef __SC_COMPATIBLE__
        if (!isNeg && _AP_Q != SC_TRN) 
            quantization_adjust(qb, rb, sb);
        else if (isNeg && (_AP_O != SC_WRAP || _AP_N != 0)) {
            bool overflow = !allzeros && !neg_src;
            bool underflow = !allones && neg_src;
            if ((_AP_O == SC_SAT_SYM) && _AP_S)
                    underflow |= neg_src && (_AP_W > 1 ? _ssdm_op_get_range(
                                Base::V, 0, _AP_W - 2) == 0: true);
            overflow_adjust(underflow, overflow, lD, isNeg);
        }  
#endif  
        return *this;
    }

    template<int _AP_W2>
    INLINE ap_fixed_base& 
    operator >>= (const ap_int_base<_AP_W2,true>& op2) {
        int sh = op2.to_int();
        return operator >>= (sh);
    }

    AP_WEAK ap_fixed_base&
    operator >>= (unsigned int sh) {
#ifdef AESL_SYN
#pragma AUTOPILOT inline self
#endif
#ifdef __SC_COMPATIBLE__
        if (sh == 0) return *this;
        bool qb, rb, sb;
        if ( _AP_Q != SC_TRN) {
           qb = false;
           if (sh <= _AP_W)
                qb = _ssdm_op_get_bit(Base::V, sh - 1);
           rb  = false;
           if (sh > 1 && sh <= _AP_W) 
                rb = _ssdm_op_get_range(Base::V, 0, sh - 2) != 0;
           else if (sh > _AP_W)
                rb = Base::V != 0; 
           sb = _AP_S && _ssdm_op_get_bit(Base::V, _AP_W - 1);
        }
#endif
        Base::V >>= sh;
#ifdef __SC_COMPATIBLE__
        if ( _AP_Q != SC_TRN) 
           quantization_adjust(qb, rb, sb);
#endif 
        return *this; 
    }
    
    template<int _AP_W2>
    INLINE ap_fixed_base&
    operator >>= (const ap_int_base<_AP_W2,false>& op2) {
        unsigned int sh = op2.to_uint();
        return operator >>= (sh);
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base&
    operator >>= (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2,
                 _AP_N2>& op2) {
        return operator >>= (op2.to_ap_int_base());
    }

    template<int _AP_W2, int _AP_I2, bool _AP_S2, 
             ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int _AP_N2>
    INLINE ap_fixed_base&
    operator <<= (const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2,
                 _AP_N2>& op2) {
        return operator <<= (op2.to_ap_int_base());
    }

    // Comparisons.
    // -------------------------------------------------------------------------
#define OP_CMP_AF(Sym) \
    template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2> \
    INLINE bool operator Sym (const ap_fixed_base<_AP_W2,_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>& op2) const { \
        const int  _AP_F = _AP_W-_AP_I, F2 = _AP_W2-_AP_I2 ; \
        if (_AP_F == F2) \
            return Base::V Sym op2.V;\
        else if (_AP_F > F2) \
            return Base::V Sym ap_fixed_base<AP_MAX(_AP_W2+_AP_F-F2, 1),_AP_I2,_AP_S2,_AP_Q2,_AP_O2, _AP_N2>(op2).V; \
        \
        else \
            return ap_fixed_base<AP_MAX(_AP_W+F2-_AP_F+1, 1),_AP_I+1,_AP_S,_AP_Q,_AP_O, _AP_N>(*this).V Sym op2.V; \
        return false; \
    }

    OP_CMP_AF(==)
    OP_CMP_AF(!=)
    OP_CMP_AF(>)
    OP_CMP_AF(>=)
    OP_CMP_AF(<)
    OP_CMP_AF(<=)

#define DOUBLE_CMP_AF(Sym) \
    INLINE bool operator Sym (double d) const { \
        return to_double() Sym d; \
    }

    DOUBLE_CMP_AF(==)
    DOUBLE_CMP_AF(!=)
    DOUBLE_CMP_AF(>)
    DOUBLE_CMP_AF(>=)
    DOUBLE_CMP_AF(<)
    DOUBLE_CMP_AF(<=)

    // Bit and Slice Select
    INLINE af_bit_ref<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> operator[] (unsigned  index) {
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index);
    }

    template<int _AP_W2, bool _AP_S2>
    INLINE af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> operator [] (const ap_int_base<_AP_W2,_AP_S2>& index) {
        AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index.to_int());
    }
  
    INLINE bool operator [] (unsigned  index) const {
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, index);
    }

    INLINE af_bit_ref<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> bit(unsigned  index) {
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index);
    }


    template<int _AP_W2, bool _AP_S2>
    INLINE af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> bit (const ap_int_base<_AP_W2,_AP_S2>& index) {
        AP_ASSERT(index >= 0, "Attempting to read bit with negative index");
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index.to_int());
    }

    INLINE bool bit (unsigned index) const {
        AP_ASSERT(index < _AP_W, "Attempting to read bit beyond MSB");
        return _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, index);
    }

    template<int _AP_W2>
    INLINE af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> get_bit (const ap_int_base<_AP_W2, true>& index) {
        AP_ASSERT(index >= _AP_I - _AP_W, "Attempting to read bit with negative index");
        AP_ASSERT(index < _AP_I, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index.to_int() + _AP_W - _AP_I);
    }

    INLINE bool get_bit (int index) const {
        AP_ASSERT(index < _AP_I, "Attempting to read bit beyond MSB");
        AP_ASSERT(index >= _AP_I - _AP_W, "Attempting to read bit beyond MSB");
        return _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, index + _AP_W - _AP_I);
    }

    INLINE af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N> get_bit (int index) {
        AP_ASSERT(index >= _AP_I - _AP_W, "Attempting to read bit with negative index");
        AP_ASSERT(index < _AP_I, "Attempting to read bit beyond MSB");
        return af_bit_ref<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>(this, index + _AP_W - _AP_I);
    }

    template<int _AP_W2>
    INLINE bool get_bit (const ap_int_base<_AP_W2, true>& index) const {
        AP_ASSERT(index < _AP_I, "Attempting to read bit beyond MSB");
        AP_ASSERT(index >= _AP_I - _AP_W, "Attempting to read bit beyond MSB");
        return _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, index.to_int() + _AP_W - _AP_I);
    }


    INLINE af_range_ref<_AP_W,_AP_I,_AP_S, _AP_Q, _AP_O, _AP_N>
    range(int Hi, int Lo) {
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(this, Hi, Lo);
    }

    INLINE af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>
    operator () (int Hi, int Lo) {
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(this, Hi, Lo);
    }

    INLINE af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>
    range(int Hi, int Lo) const {
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(const_cast<
               ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>*>(this), 
               Hi, Lo);
    }

    template<int _AP_W2, bool _AP_S2, int _AP_W3, bool _AP_S3>
    INLINE af_range_ref<_AP_W,_AP_I,_AP_S, _AP_Q, _AP_O, _AP_N>
    range(const ap_int_base<_AP_W2, _AP_S2> &HiIdx, 
          const ap_int_base<_AP_W3, _AP_S3> &LoIdx) {
        int Hi = HiIdx.to_int();
        int Lo = LoIdx.to_int();
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(this, Hi, Lo);
    }

    template<int _AP_W2, bool _AP_S2, int _AP_W3, bool _AP_S3>
    INLINE af_range_ref<_AP_W,_AP_I,_AP_S, _AP_Q, _AP_O, _AP_N>
    operator () (const ap_int_base<_AP_W2, _AP_S2> &HiIdx, 
                 const ap_int_base<_AP_W3, _AP_S3> &LoIdx) {
        int Hi = HiIdx.to_int();
        int Lo = LoIdx.to_int();
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(this, Hi, Lo);
    }

    template<int _AP_W2, bool _AP_S2, int _AP_W3, bool _AP_S3>
    INLINE af_range_ref<_AP_W,_AP_I,_AP_S, _AP_Q, _AP_O, _AP_N>
    range(const ap_int_base<_AP_W2, _AP_S2> &HiIdx, 
          const ap_int_base<_AP_W3, _AP_S3> &LoIdx) const {
        int Hi = HiIdx.to_int();
        int Lo = LoIdx.to_int();
        AP_ASSERT((Hi < _AP_W) && (Lo < _AP_W), "Out of bounds in range()");
        return af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>(const_cast<
               ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>*>(this), 
               Hi, Lo);
    }

    template<int _AP_W2, bool _AP_S2, int _AP_W3, bool _AP_S3>
    INLINE af_range_ref<_AP_W,_AP_I,_AP_S, _AP_Q, _AP_O, _AP_N>
    operator () (const ap_int_base<_AP_W2, _AP_S2> &HiIdx, 
                 const ap_int_base<_AP_W3, _AP_S3> &LoIdx) const {
        int Hi = HiIdx.to_int();
        int Lo = LoIdx.to_int();
        return this->range(Hi, Lo);
    }

    INLINE af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>
    operator () (int Hi, int Lo) const {
        return this->range(Hi, Lo);
    }

    INLINE af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>
    range() {
        return this->range(_AP_W - 1, 0);
    }

    INLINE af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>
    range() const {
        return this->range(_AP_W - 1, 0);
    }
 
    INLINE bool is_zero () const {
        return Base::V == 0;
    }

    INLINE bool is_neg () const {
        if (_AP_S && _ssdm_op_get_bit(const_cast<ap_fixed_base*>(this)->V, _AP_W - 1))
           return true;
        return false;
    }

    INLINE int wl () const {
        return _AP_W;
    }

    INLINE int iwl () const {
        return _AP_I;
    } 

    INLINE ap_q_mode q_mode () const {
        return _AP_Q;
    }

    INLINE ap_o_mode o_mode () const {
        return _AP_O;
    }

    INLINE int n_bits () const {
        return _AP_N;
    }
 
    INLINE char* to_string(BaseMode mode) {
          return 0;
    }
   
    INLINE char* to_string(signed char mode) {
          return to_string(BaseMode(mode));
    }
};

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE void b_not(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op) {
    ret.V = ~ op.V;
}

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE void b_and(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op1,
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op2) {
    ret.V =  op1.V & op2.V;
}

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE void b_or(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op1,
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op2) {
    ret.V =  op1.V | op2.V;
}

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE void b_xor(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op1,
            const ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& op2) {
    ret.V =  op1.V ^ op2.V;
}

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N, int _AP_W2, int _AP_I2,
         bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2>
INLINE void neg(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op) {
    ap_fixed_base<_AP_W2+!_AP_S2, _AP_I2+!_AP_S2, true, _AP_Q2, _AP_O2, _AP_N2> Tmp;
    Tmp.V = - op.V;
    ret = Tmp;
}

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N, int _AP_W2, int _AP_I2, 
         bool _AP_S2, ap_q_mode _AP_Q2, ap_o_mode _AP_O2, int  _AP_N2>
INLINE void lshift(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op,
            int i) {
    ap_fixed_base<_AP_W2 - _AP_I2 + AP_MAX(_AP_I, _AP_I2), AP_MAX(_AP_I, _AP_I2), _AP_S2, _AP_Q2, _AP_O2, _AP_N2> Tmp;
    Tmp.V = op.V;
    Tmp.V <<= i;
    ret = Tmp; 
} 

template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
         int _AP_N, int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
         ap_o_mode _AP_O2, int  _AP_N2>
INLINE void rshift(ap_fixed_base<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N>& ret, 
            const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2>& op,
            int i) {
    ap_fixed_base<_AP_I2 + AP_MAX(_AP_W - _AP_I, _AP_W2 - _AP_I2), _AP_I2, _AP_S2, _AP_Q2, _AP_O2, _AP_N2> Tmp;
    const int val = _AP_W - _AP_I - (_AP_W2 - _AP_I2);
    Tmp.V = op.V;
    if (val > 0) Tmp.V <<= val; 
    Tmp.V >>= i;
    ret = Tmp; 
}
 
#define AF_CTOR_SPEC_BASE(_AP_W, _AP_S, C_TYPE) \
  template<> INLINE ap_fixed_base<_AP_W,_AP_W,_AP_S,SC_TRN,SC_WRAP>::ap_fixed_base(C_TYPE i_op) { \
    Base::V = i_op; \
  }

#define AF_CTOR_SPEC(_AP_W, C_TYPE) \
  AF_CTOR_SPEC_BASE(_AP_W, true,  C_TYPE) \
  AF_CTOR_SPEC_BASE(_AP_W, false, C_TYPE)

AF_CTOR_SPEC(1, bool)
AF_CTOR_SPEC(8, char)
AF_CTOR_SPEC(8, signed char)
AF_CTOR_SPEC(8, unsigned char)
AF_CTOR_SPEC(16, signed short)
AF_CTOR_SPEC(16, unsigned short)
AF_CTOR_SPEC(32, signed int)
AF_CTOR_SPEC(32, unsigned int)
# if defined __x86_64__
AF_CTOR_SPEC(64, long)
AF_CTOR_SPEC(64, unsigned long)
# else
AF_CTOR_SPEC(32, long)
AF_CTOR_SPEC(32, unsigned long)
# endif 
AF_CTOR_SPEC(64, ap_slong)
AF_CTOR_SPEC(64, ap_ulong)

#ifndef AP_AUTOCC
/// Output streamimg.
// -----------------------------------------------------------------------------
template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE std::ostream& 
operator << (std::ostream& os, const ap_fixed_base<_AP_W,_AP_I,
             _AP_S,_AP_Q,_AP_O, _AP_N>& x) {
    // os << x.to_double();
    return os;
}

/// Input streamimg.
// -----------------------------------------------------------------------------
template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, 
         ap_o_mode _AP_O, int _AP_N>
INLINE std::istream& 
operator >> (std::istream& in, ap_fixed_base<_AP_W,_AP_I,
             _AP_S,_AP_Q,_AP_O, _AP_N>& x) {
#if 0
    double d;
    in >> d;
    x = ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>(d);
#endif
    return in;
}

#endif

/// Operators mixing Integers with ap_fixed_base
// -----------------------------------------------------------------------------
#define AF_BIN_OP_WITH_INT_SF(BIN_OP, C_TYPE, _AP_W2, _AP_S2, RTYPE) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE typename ap_fixed_base<_AP_W,_AP_I,_AP_S>::template RType<_AP_W2,_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op, C_TYPE i_op) { \
    return op.operator BIN_OP (ap_int_base<_AP_W2,_AP_S2>(i_op)); \
  }

#define AF_BIN_OP_WITH_INT(BIN_OP, C_TYPE, _AP_W2, _AP_S2, RTYPE) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE typename ap_fixed_base<_AP_W,_AP_I,_AP_S>::template RType<_AP_W2,_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op, C_TYPE i_op) { \
    return op.operator BIN_OP (ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  } \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE typename ap_fixed_base<_AP_W,_AP_I,_AP_S>::template RType<_AP_W2,_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( C_TYPE i_op, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op) { \
    return ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op).operator BIN_OP (op); \
  }

#define AF_REL_OP_WITH_INT(REL_OP, C_TYPE, _AP_W2, _AP_S2) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE bool operator REL_OP ( const ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op, C_TYPE i_op) { \
    return op.operator REL_OP (ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  } \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE bool operator REL_OP ( C_TYPE i_op, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q, _AP_O, _AP_N>& op) { \
    return ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op).operator REL_OP (op); \
  }

#define AF_ASSIGN_OP_WITH_INT(ASSIGN_OP, C_TYPE, _AP_W2, _AP_S2) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q, _AP_O, _AP_N>& operator ASSIGN_OP ( ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q, _AP_O, _AP_N>& op, C_TYPE i_op) { \
    return op.operator ASSIGN_OP (ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  }

#define AF_ASSIGN_OP_WITH_INT_SF(ASSIGN_OP, C_TYPE, _AP_W2, _AP_S2) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int  _AP_N> \
  INLINE ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& operator ASSIGN_OP ( ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op, C_TYPE i_op) { \
    return op.operator ASSIGN_OP (ap_int_base<_AP_W2,_AP_S2>(i_op)); \
  }

#define AF_OPS_WITH_INT(C_TYPE, _AP_W2, SI) \
  AF_BIN_OP_WITH_INT(+, C_TYPE, _AP_W2, SI, plus) \
  AF_BIN_OP_WITH_INT(-, C_TYPE, _AP_W2, SI, minus) \
  AF_BIN_OP_WITH_INT(*, C_TYPE, _AP_W2, SI, mult) \
  AF_BIN_OP_WITH_INT(/, C_TYPE, _AP_W2, SI, div) \
  AF_BIN_OP_WITH_INT_SF(>>, C_TYPE, _AP_W2, SI, arg1) \
  AF_BIN_OP_WITH_INT_SF(<<, C_TYPE, _AP_W2, SI, arg1) \
  AF_BIN_OP_WITH_INT(&, C_TYPE, _AP_W2, SI, logic) \
  AF_BIN_OP_WITH_INT(|, C_TYPE, _AP_W2, SI, logic) \
  AF_BIN_OP_WITH_INT(^, C_TYPE, _AP_W2, SI, logic) \
  \
  AF_REL_OP_WITH_INT(==, C_TYPE, _AP_W2, SI) \
  AF_REL_OP_WITH_INT(!=, C_TYPE, _AP_W2, SI) \
  AF_REL_OP_WITH_INT(>, C_TYPE, _AP_W2, SI) \
  AF_REL_OP_WITH_INT(>=, C_TYPE, _AP_W2, SI) \
  AF_REL_OP_WITH_INT(<, C_TYPE, _AP_W2, SI) \
  AF_REL_OP_WITH_INT(<=, C_TYPE, _AP_W2, SI) \
  \
  AF_ASSIGN_OP_WITH_INT(+=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(-=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(*=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(/=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT_SF(>>=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT_SF(<<=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(&=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(|=, C_TYPE, _AP_W2, SI) \
  AF_ASSIGN_OP_WITH_INT(^=, C_TYPE, _AP_W2, SI)

AF_OPS_WITH_INT(bool, 1, false)
AF_OPS_WITH_INT(char, 8, true)
AF_OPS_WITH_INT(signed char, 8, true)
AF_OPS_WITH_INT(unsigned char, 8, false)
AF_OPS_WITH_INT(signed short, 16, true)
AF_OPS_WITH_INT(unsigned short, 16, false)
AF_OPS_WITH_INT(int, 32, true)
AF_OPS_WITH_INT(unsigned int, 32, false)
# if defined __x86_64__
AF_OPS_WITH_INT(long, 64, true)
AF_OPS_WITH_INT(unsigned long, 64, false)
# else
AF_OPS_WITH_INT(long, 32, true)
AF_OPS_WITH_INT(unsigned long, 32, false)
# endif 
AF_OPS_WITH_INT(ap_slong, 64, true)
AF_OPS_WITH_INT(ap_ulong, 64, false)

#define AF_BIN_OP_WITH_AP_INT(BIN_OP, RTYPE) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE typename ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>::template RType<_AP_W,_AP_I,_AP_S>::RTYPE \
  operator BIN_OP ( const ap_int_base<_AP_W2,_AP_S2>& i_op, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op) { \
    return ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op).operator BIN_OP (op); \
  } \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE typename ap_fixed_base<_AP_W, _AP_I, _AP_S>::template RType<_AP_W2,_AP_W2,_AP_S2>::RTYPE \
  operator BIN_OP ( const ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op, const ap_int_base<_AP_W2,_AP_S2>& i_op) { \
    return op.operator BIN_OP (ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  }

#define AF_REL_OP_WITH_AP_INT(REL_OP) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP ( const ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op, const ap_int_base<_AP_W2,_AP_S2>& i_op) { \
    return op.operator REL_OP ( ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  } \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP ( const ap_int_base<_AP_W2,_AP_S2>& i_op, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op) { \
    return ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op).operator REL_OP (op); \
  }

#define AF_ASSIGN_OP_WITH_AP_INT(ASSIGN_OP) \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& operator ASSIGN_OP ( ap_fixed_base<_AP_W, _AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op, const ap_int_base<_AP_W2,_AP_S2>& i_op) { \
    return op.operator ASSIGN_OP (ap_fixed_base<_AP_W2,_AP_W2,_AP_S2>(i_op)); \
  } \
  template<int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE ap_int_base<_AP_W2,_AP_S2>& operator ASSIGN_OP ( ap_int_base<_AP_W2,_AP_S2>& i_op, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op) { \
    return i_op.operator ASSIGN_OP (op.to_ap_int_base()); \
  }

AF_BIN_OP_WITH_AP_INT(+, plus)
AF_BIN_OP_WITH_AP_INT(-, minus)
AF_BIN_OP_WITH_AP_INT(*, mult)
AF_BIN_OP_WITH_AP_INT(/, div)
AF_BIN_OP_WITH_AP_INT(&, logic)
AF_BIN_OP_WITH_AP_INT(|, logic)
AF_BIN_OP_WITH_AP_INT(^, logic)

AF_REL_OP_WITH_AP_INT(==)
AF_REL_OP_WITH_AP_INT(!=)
AF_REL_OP_WITH_AP_INT(>)
AF_REL_OP_WITH_AP_INT(>=)
AF_REL_OP_WITH_AP_INT(<)
AF_REL_OP_WITH_AP_INT(<=)

AF_ASSIGN_OP_WITH_AP_INT(+=)
AF_ASSIGN_OP_WITH_AP_INT(-=)
AF_ASSIGN_OP_WITH_AP_INT(*=)
AF_ASSIGN_OP_WITH_AP_INT(/=)
AF_ASSIGN_OP_WITH_AP_INT(&=)
AF_ASSIGN_OP_WITH_AP_INT(|=)
AF_ASSIGN_OP_WITH_AP_INT(^=)

// Relational Operators with double
template<int _AP_W, int _AP_I, bool _AP_S, 
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator == ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op2) {
  return op2.operator == (op1);
}
template<int _AP_W, int _AP_I, bool _AP_S,
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator != ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op2) {
  return op2.operator != (op1);
}
template<int _AP_W, int _AP_I, bool _AP_S, 
        ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator > ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op2) {
  return op2.operator < (op1);
}
template<int _AP_W, int _AP_I, bool _AP_S, 
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator >= ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O, _AP_N>& op2) {
  return op2.operator <= (op1);
}
template<int _AP_W, int _AP_I, bool _AP_S, 
        ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator < ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op2) {
  return op2.operator > (op1);
}
template<int _AP_W, int _AP_I, bool _AP_S, 
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
INLINE bool operator <= ( double op1, const ap_fixed_base<_AP_W,_AP_I,_AP_S,_AP_Q,_AP_O,_AP_N>& op2) {
  return op2.operator >= (op1);
}

#define AF_REF_REL_OP_MIX_INT(REL_OP, C_TYPE, _AP_W2, _AP_S2)  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N> \
  INLINE bool operator REL_OP ( const af_range_ref<_AP_W,_AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op, C_TYPE op2) {  \
    return (ap_int_base<_AP_W, false>(op)).operator REL_OP (ap_int_base<_AP_W2,_AP_S2>(op2));  \
  }  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N> \
  INLINE bool operator REL_OP ( C_TYPE op2, const af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op) {  \
    return ap_int_base<_AP_W2,_AP_S2>(op2).operator REL_OP (ap_int_base<_AP_W, false>(op));  \
  } \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N> \
  INLINE bool operator REL_OP ( const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op, C_TYPE op2) {  \
    return (bool(op)) REL_OP op2;  \
  }  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N> \
  INLINE bool operator REL_OP ( C_TYPE op2, const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op) {  \
    return op2 REL_OP (bool(op));  \
  } 
 
#define AF_REF_REL_MIX_INT(C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(>, C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(<, C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(>=, C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(<=, C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(==, C_TYPE, _AP_WI, _AP_SI) \
AF_REF_REL_OP_MIX_INT(!=, C_TYPE, _AP_WI, _AP_SI) 

AF_REF_REL_MIX_INT(bool, 1, false)
AF_REF_REL_MIX_INT(char, 8, true)
AF_REF_REL_MIX_INT(signed char, 8, true)
AF_REF_REL_MIX_INT(unsigned char, 8, false)
AF_REF_REL_MIX_INT(short, 16, true)
AF_REF_REL_MIX_INT(unsigned short, 16, false)
AF_REF_REL_MIX_INT(int, 32, true)
AF_REF_REL_MIX_INT(unsigned int, 32, false)
# if defined __x86_64__
AF_REF_REL_MIX_INT(long, 32, true)
AF_REF_REL_MIX_INT(unsigned long, 32, false)
# else 
AF_REF_REL_MIX_INT(long, 32, true)
AF_REF_REL_MIX_INT(unsigned long, 32, false)
# endif
AF_REF_REL_MIX_INT(ap_slong, 64, true)
AF_REF_REL_MIX_INT(ap_ulong, 64, false)

#define AF_REF_REL_OP_AP_INT(REL_OP)  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP ( const af_range_ref<_AP_W,_AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op, const ap_int_base<_AP_W2, _AP_S> &op2) {  \
    return (ap_int_base<_AP_W, false>(op)).operator REL_OP (op2);  \
  }  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP (const ap_int_base<_AP_W2, _AP_S2> &op2, const af_range_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op) {  \
    return op2.operator REL_OP (ap_int_base<_AP_W, false>(op));  \
  } \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP ( const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op, const ap_int_base<_AP_W2, _AP_S2> &op2) {  \
    return (ap_int_base<1, false>(op)).operator REL_OP (op2);  \
  }  \
template<int _AP_W, int _AP_I, bool _AP_S, \
         ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N, int _AP_W2, bool _AP_S2> \
  INLINE bool operator REL_OP ( const ap_int_base<_AP_W2, _AP_S2> &op2, const af_bit_ref<_AP_W, _AP_I, _AP_S, _AP_Q, _AP_O, _AP_N> &op) {  \
    return op2.operator REL_OP (ap_int_base<1,false>(op));  \
  } 
 
AF_REF_REL_OP_AP_INT(>) 
AF_REF_REL_OP_AP_INT(<) 
AF_REF_REL_OP_AP_INT(>=) 
AF_REF_REL_OP_AP_INT(<=) 
AF_REF_REL_OP_AP_INT(==) 
AF_REF_REL_OP_AP_INT(!=) 




#endif//#ifndef __AESL_AP_FIXED_SYN_H__
// FIXME: Finished filling ap_fixed_syn.h

//AP_INT
//--------------------------------------------------------
template<int _AP_W>
struct ap_int: ap_int_base<_AP_W, true> {
  typedef ap_int_base<_AP_W, true> Base;
  //Constructor
  INLINE ap_int(): Base() {}
  template<int _AP_W2>
  INLINE ap_int(const ap_int<_AP_W2> &op) {Base::V = op.V;}

  template<int _AP_W2>
  INLINE ap_int(const volatile ap_int<_AP_W2> &op) {Base::V = op.V;}

  template<int _AP_W2>
  INLINE ap_int(const ap_uint<_AP_W2> &op) { Base::V = op.V;}

  template<int _AP_W2>
  INLINE ap_int(const volatile ap_uint<_AP_W2> &op) { Base::V = op.V;}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int(const ap_range_ref<_AP_W2, _AP_S2>& ref):Base(ref) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int(const ap_bit_ref<_AP_W2, _AP_S2>& ref):Base(ref) {}
  
  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_int(const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& ref):Base(ref) {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, true, _AP_Q2, _AP_O2, _AP_N2>)op) {}
 
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, false, _AP_Q2, _AP_O2, _AP_N2>)op) {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const volatile ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, true, _AP_Q2, _AP_O2, _AP_N2>)op) {}
 
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const volatile ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, false, _AP_Q2, _AP_O2, _AP_N2>)op) {}


  template<int _AP_W2, bool _AP_S2>
  INLINE ap_int(const ap_int_base<_AP_W2, _AP_S2>& op){ Base::V = op.V; }

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, 
		_AP_N2>& op):Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, 
		_AP_N2>& op):Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_int(const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2,
		_AP_Q2, _AP_O2, _AP_N2>& op):Base(op) {}

#define CTOR(TYPE)				\
  INLINE ap_int(TYPE val) {Base::V = val; }
  CTOR(bool)
  CTOR(signed char)
  CTOR(unsigned char)
  CTOR(short)
  CTOR(unsigned short)
  CTOR(int)
  CTOR(unsigned int)
  CTOR(long)
  CTOR(unsigned long)
  CTOR(unsigned long long)
  CTOR(long long)
  CTOR(half)
  CTOR(float)
  CTOR(double)
#undef CTOR
  INLINE ap_int(const char* str):Base(str) {}
  INLINE ap_int(const char* str, signed char radix):Base(str, radix) {}

  // FIXME: This is only for internal use. Clang checker
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_int(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val,dummy) {}
  #endif

  //Assignment
  //Assignment
  //Another form of "write"
  INLINE void operator = (const ap_int<_AP_W>& op2) volatile {
    Base::V = op2.V;
  }
 
  INLINE void operator = (const volatile ap_int<_AP_W>& op2) volatile {
    Base::V = op2.V;
  }

  INLINE ap_int& operator = (const volatile ap_int<_AP_W>& op2) {
    Base::V = op2.V;
    return *this;
  }

  INLINE ap_int& operator = (const ap_int<_AP_W>& op2) {
    Base::V = op2.V;
    return *this;
  }

};

//AP_UINT
//---------------------------------------------------------------
template<int _AP_W>
struct ap_uint: ap_int_base<_AP_W, false> {
  typedef ap_int_base<_AP_W, false> Base;
  //Constructor
  INLINE ap_uint(): Base() {}
  template<int _AP_W2>
  INLINE ap_uint(const ap_uint<_AP_W2> &op) { Base::V = op.V; }

  template<int _AP_W2>
  INLINE ap_uint(const ap_int<_AP_W2> &op)  { Base::V = op.V;}

  template<int _AP_W2>
  INLINE ap_uint(const volatile ap_uint<_AP_W2> &op) { Base::V = op.V; }

  template<int _AP_W2>
  INLINE ap_uint(const volatile ap_int<_AP_W2> &op)  { Base::V = op.V;}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_uint(const ap_range_ref<_AP_W2, _AP_S2>& ref):Base(ref) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_uint(const ap_bit_ref<_AP_W2, _AP_S2>& ref):Base(ref) {}
  
  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_uint(const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& ref):Base(ref) {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, true, _AP_Q2, _AP_O2, _AP_N2>)op) {}
 
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, false, _AP_Q2, _AP_O2, _AP_N2>)op) {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const volatile ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, true, _AP_Q2, _AP_O2, _AP_N2>)op) {}
 
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const volatile ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, _AP_N2>& op)
    :Base((ap_fixed_base<_AP_W2, _AP_I2, false, _AP_Q2, _AP_O2, _AP_N2>)op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_uint(const ap_int_base<_AP_W2, _AP_S2>& op){ Base::V = op.V;}

  template<int _AP_W2, int _AP_I2,  bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, 
		 _AP_N2>& op):Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, _AP_Q2, _AP_O2, 
		 _AP_N2>& op):Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_uint(const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2,
		 _AP_Q2, _AP_O2, _AP_N2>& op):Base(op) {}

#define CTOR(TYPE)				\
  INLINE ap_uint(TYPE val) { Base::V = val; }
  CTOR(bool)
  CTOR(signed char)
  CTOR(unsigned char)
  CTOR(short)
  CTOR(unsigned short)
  CTOR(int)
  CTOR(unsigned int)
  CTOR(long)
  CTOR(unsigned long)
  CTOR(unsigned long long)
  CTOR(long long)
  CTOR(half)
  CTOR(float)
  CTOR(double)
#undef CTOR    
  INLINE ap_uint(const char* str):Base(str) {}
  INLINE ap_uint(const char* str, signed char radix):Base(str, radix) {}

  // FIXME: This is only for internal use. Clang checker.
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_uint(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val,dummy) {}
  #endif

  //Assignment
  //Another form of "write"
  INLINE void operator = (const ap_uint<_AP_W>& op2) volatile {
    Base::V = op2.V;
  }
 
  INLINE void operator = (const volatile ap_uint<_AP_W>& op2) volatile {
    Base::V = op2.V;
  }

  INLINE ap_uint& operator = (const volatile ap_uint<_AP_W>& op2) {
    Base::V = op2.V;
    return *this;
  }

  INLINE ap_uint& operator = (const ap_uint<_AP_W>& op2) {
    Base::V = op2.V;
    return *this;
  }

};
#define ap_bigint ap_int
#define ap_biguint ap_uint
//AP_FIXED
//---------------------------------------------------------------------  
// XXX: default template parameter in first declaration.
template <int _AP_W, int _AP_I, ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
struct ap_fixed : ap_fixed_base<_AP_W, _AP_I, true, _AP_Q, _AP_O, _AP_N> {

  typedef ap_fixed_base<_AP_W, _AP_I, true, _AP_Q, _AP_O, _AP_N> Base;
  //Constructor
  INLINE ap_fixed():Base() {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const ap_fixed<_AP_W2, _AP_I2, _AP_Q2, 
		  _AP_O2, _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
					     true, _AP_Q2, _AP_O2, _AP_N2>(op)) {}
    
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, 
		  _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				     false, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

  template<int _AP_W2>
  INLINE ap_fixed(const ap_int<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, true>(op)) {}

  template<int _AP_W2>
  INLINE ap_fixed(const ap_uint<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, false>(op)) {}
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const volatile ap_fixed<_AP_W2, _AP_I2, _AP_Q2, 
		  _AP_O2, _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
					     true, _AP_Q2, _AP_O2, _AP_N2>(op)) {}
    
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const volatile ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, 
		  _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				     false, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

  template<int _AP_W2>
  INLINE ap_fixed(const volatile ap_int<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, true>(op)) {}

  template<int _AP_W2>
  INLINE ap_fixed(const volatile ap_uint<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, false>(op)) {}
  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2, 
		  _AP_Q2, _AP_O2, _AP_N2>& op):Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_fixed(const ap_bit_ref<_AP_W2, _AP_S2>& op):
    Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_fixed(const ap_range_ref<_AP_W2, _AP_S2>& op):
    Base(op) {}

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_fixed(const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& op):
    Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, 
		  _AP_Q2, _AP_O2, _AP_N2>& op): Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_fixed(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, 
		  _AP_Q2, _AP_O2, _AP_N2>& op): Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_fixed(const ap_int_base<_AP_W2, _AP_S2>& op): Base(op) {}

#define CTOR(TYPE)				\
  INLINE ap_fixed(TYPE v):Base(v) {}
  CTOR(bool)
  CTOR(signed char)
  CTOR(unsigned char)
  CTOR(short)
  CTOR(unsigned short)
  CTOR(int)
  CTOR(unsigned int)
  CTOR(long)
  CTOR(unsigned long)
  CTOR(unsigned long long)
  CTOR(long long)
  CTOR(half)
  CTOR(float)
  CTOR(double)
#undef CTOR    
  INLINE ap_fixed(const char* str):Base(str) {}
  INLINE ap_fixed(const char* str, signed char radix):Base(str, radix) {}
 
  // FIXME: This is only for internal use. Clang checker.
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_fixed(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val,dummy) {}
  #endif
     
  //Assignment
  INLINE ap_fixed& operator = (const ap_fixed<_AP_W, _AP_I, _AP_Q, _AP_O, 
			       _AP_N>& op) {
    Base::V = op.V;
    return *this;
  }

  INLINE ap_fixed& operator = (const volatile ap_fixed<_AP_W, _AP_I, _AP_Q, _AP_O, _AP_N>& op) {
    Base::V = op.V;
    return *this;
  }

  INLINE void operator = (const ap_fixed<_AP_W, _AP_I, _AP_Q, _AP_O, 
			  _AP_N>& op) volatile {
    Base::V = op.V;
  }

  INLINE void operator = (const volatile ap_fixed<_AP_W, _AP_I, _AP_Q, _AP_O, _AP_N>& op) volatile {
    Base::V = op.V;
  }



};
//AP_UFIXED
//-------------------------------------------------------------------
// XXX: default template parameter in first declaration.
template <int _AP_W, int _AP_I, ap_q_mode _AP_Q, ap_o_mode _AP_O, int _AP_N>
struct ap_ufixed : ap_fixed_base<_AP_W, _AP_I, false, _AP_Q, _AP_O, _AP_N> {

  typedef ap_fixed_base<_AP_W, _AP_I, false, _AP_Q, _AP_O, _AP_N> Base;
  //Constructor
  INLINE ap_ufixed():Base() {}

  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2,
		   _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				      true, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

    
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, 
		   _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				      false, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

  template<int _AP_W2>
  INLINE ap_ufixed(const ap_int<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, true>(op)) {}

  template<int _AP_W2>
  INLINE ap_ufixed(const ap_uint<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, false>(op)) {}
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const volatile ap_fixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2,
		   _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				      true, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

    
  template<int _AP_W2, int _AP_I2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const volatile ap_ufixed<_AP_W2, _AP_I2, _AP_Q2, _AP_O2, 
		   _AP_N2>& op): Base(ap_fixed_base<_AP_W2, _AP_I2, 
				      false, _AP_Q2, _AP_O2, _AP_N2>(op)) {}

  template<int _AP_W2>
  INLINE ap_ufixed(const volatile ap_int<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, true>(op)) {}

  template<int _AP_W2>
  INLINE ap_ufixed(const volatile ap_uint<_AP_W2>& op):
    Base(ap_int_base<_AP_W2, false>(op)) {}
  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const ap_fixed_base<_AP_W2, _AP_I2, _AP_S2,
		   _AP_Q2, _AP_O2, _AP_N2>& op):Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_ufixed(const ap_bit_ref<_AP_W2, _AP_S2>& op):
    Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_ufixed(const ap_range_ref<_AP_W2, _AP_S2>& op):
    Base(op) {}

  template<int _AP_W2, typename _AP_T2, int _AP_W3, typename _AP_T3>
  INLINE ap_ufixed(const ap_concat_ref<_AP_W2, _AP_T2, _AP_W3, _AP_T3>& op):
    Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const af_bit_ref<_AP_W2, _AP_I2, _AP_S2, 
		   _AP_Q2, _AP_O2, _AP_N2>& op): Base(op) {}

  template<int _AP_W2, int _AP_I2, bool _AP_S2, ap_q_mode _AP_Q2, 
	   ap_o_mode _AP_O2, int _AP_N2>
  INLINE ap_ufixed(const af_range_ref<_AP_W2, _AP_I2, _AP_S2, 
		   _AP_Q2, _AP_O2, _AP_N2>& op): Base(op) {}

  template<int _AP_W2, bool _AP_S2>
  INLINE ap_ufixed(const ap_int_base<_AP_W2, _AP_S2>& op): Base(op) {}

#define CTOR(TYPE)				\
  INLINE ap_ufixed(TYPE v):Base(v) {}
  CTOR(bool)
  CTOR(signed char)
  CTOR(unsigned char)
  CTOR(short)
  CTOR(unsigned short)
  CTOR(int)
  CTOR(unsigned int)
  CTOR(long)
  CTOR(unsigned long)
  CTOR(unsigned long long)
  CTOR(long long)
  CTOR(half)
  CTOR(float)
  CTOR(double)
#undef CTOR    
  INLINE ap_ufixed(const char* str):Base(str) {}
  INLINE ap_ufixed(const char* str, signed char radix):Base(str, radix) {} 

  // FIXME: This is only for internal use. Clang checker.
  #if ((__clang_major__ == 3) && (__clang_minor__ == 1))
  #else
  INLINE ap_ufixed(int __attribute__((bitwidth(_AP_W))) val, bool dummy):Base(val,dummy) {}    
  #endif
     
  //Assignment
  INLINE ap_ufixed& operator = (const ap_ufixed<_AP_W, _AP_I, 
				_AP_Q, _AP_O, _AP_N>& op) {
    Base::V = op.V;
    return *this;
  }

  INLINE ap_ufixed& operator = (const volatile ap_ufixed<_AP_W, _AP_I,
				_AP_Q, _AP_O, _AP_N>& op) {
    Base::V = op.V;
    return *this;
  }

  INLINE void operator = (const ap_ufixed<_AP_W, _AP_I, 
			  _AP_Q, _AP_O, _AP_N>& op) volatile {
    Base::V = op.V;
  }

  INLINE void operator = (const volatile ap_ufixed<_AP_W, _AP_I,
			  _AP_Q, _AP_O, _AP_N>& op) volatile {
    Base::V = op.V;
  }

};

#endif
// FIXME: Finish filling in syn/ap_int.h
