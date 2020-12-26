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


   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef __AESL_AP_COMMON_H__
#define __AESL_AP_COMMON_H__

#ifndef __cplusplus
#error "C++ is required to include this header file"
#else

#if (defined AESL_SYN) || (defined AP_AUTOCC)

// Forward declaration for synthesis
template <int _AP_W, bool _AP_S, bool _AP_C = (_AP_W <= 64)>
struct ap_int_base;

template <int _AP_W>
struct ap_uint;

template <int _AP_W>
struct ap_int;

template <int _AP_W, bool _AP_S>
struct ap_range_ref;

template <int _AP_W, bool _AP_S>
struct ap_bit_ref;

template <int _AP_W1, typename _AP_T1, int _AP_W2, typename _AP_T2>
struct ap_concat_ref;

enum ap_q_mode {
  SC_RND,         // rounding to plus infinity
  SC_RND_ZERO,    // rounding to zero
  SC_RND_MIN_INF, // rounding to minus infinity
  SC_RND_INF,     // rounding to infinity
  SC_RND_CONV,    // convergent rounding
  SC_TRN,         // truncation
  SC_TRN_ZERO     // truncation to zero

};

enum ap_o_mode {
  SC_SAT,      // saturation
  SC_SAT_ZERO, // saturation to zero
  SC_SAT_SYM,  // symmetrical saturation
  SC_WRAP,     // wrap-around (*)
  SC_WRAP_SM   // sign magnitude wrap-around (*)
};

template<int _AP_W, int _AP_I, bool _AP_S=true, 
         ap_q_mode _AP_Q=SC_TRN, ap_o_mode _AP_O=SC_WRAP, int _AP_N=0>
struct ap_fixed_base;

template<int _AP_W, int _AP_I, ap_q_mode _AP_Q = SC_TRN, 
         ap_o_mode _AP_O = SC_WRAP, int _AP_N = 0>
struct ap_fixed;

template<int _AP_W, int _AP_I, ap_q_mode _AP_Q = SC_TRN, 
         ap_o_mode _AP_O = SC_WRAP, int _AP_N = 0>
struct ap_ufixed;

template <int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
          int _AP_N>
struct af_range_ref;

template <int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
          int _AP_N>
struct af_bit_ref;

#else

/// Forward declaration for simulation
template <int _AP_W>
class ap_int;

template <int _AP_W>
class ap_uint;

template <int _AP_W, bool _AP_S>
struct ap_range_ref;

template <int _AP_W, bool _AP_S>
struct ap_bit_ref;

template <int _AP_W1, typename _AP_T1, int _AP_W2, typename _AP_T2>
struct ap_concat_ref;

enum ap_q_mode {
  AP_RND,         // rounding to plus infinity
  AP_RND_ZERO,    // rounding to zero
  AP_RND_MIN_INF, // rounding to minus infinity
  AP_RND_INF,     // rounding to infinity
  AP_RND_CONV,    // convergent rounding
  AP_TRN,         // truncation
  AP_TRN_ZERO     // truncation to zero

};

enum ap_o_mode {
  AP_SAT,      // saturation
  AP_SAT_ZERO, // saturation to zero
  AP_SAT_SYM,  // symmetrical saturation
  AP_WRAP,     // wrap-around (*)
  AP_WRAP_SM   // sign magnitude wrap-around (*)
};

template <int _AP_W, int _AP_I, bool _AP_S = true, ap_q_mode _AP_Q = AP_TRN,
          ap_o_mode _AP_O = AP_WRAP, int _AP_N = 0>
struct ap_fixed_base;

template <int _AP_W, int _AP_I, ap_q_mode _AP_Q = AP_TRN,
          ap_o_mode _AP_O = AP_WRAP, int _AP_N = 0>
class ap_fixed;

template <int _AP_W, int _AP_I, ap_q_mode _AP_Q = AP_TRN,
          ap_o_mode _AP_O = AP_WRAP, int _AP_N = 0>
class ap_ufixed;

template <int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
          int _AP_N>
struct af_range_ref;

template <int _AP_W, int _AP_I, bool _AP_S, ap_q_mode _AP_Q, ap_o_mode _AP_O,
          int _AP_N>
struct af_bit_ref;

#endif // (defined AESL_SYN) || (defined AP_AUTOCC)

#endif // __cplusplus

#endif // __AESL_AP_COMMON_H__
