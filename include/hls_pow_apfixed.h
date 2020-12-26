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
 *     (c) Copyright 2012-2017 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/
#ifndef _X_HLS_POW_APFIXED_H_
#define _X_HLS_POW_APFIXED_H_
#include "ap_fixed.h"
#include "hls_log_apfixed.h"
#include "hls_exp_apfixed.h"
#include "hls_round_copysign_apfixed.h"
namespace pow_apfixed_reduce{
/*
 * Normally, pow(x,y) = exp(ylog(|x|))*sig(x)^y. 
 * To make sure the result has 1ulp relative error, the relative error of log(|x|) is 2^(-W-I-2) and the relative error of ylog(|x|) is 2^(-W-2).
 * Exception handling. (Note: if 1 > Max, then return Max)
 * ============================================
 * ||     x     ||     y     ||    pow(x,y)
 * ||     0     ||     0     ||       1
 * ||     0     ||     <0    ||      Max
 * ||     0     ||     >0    ||       0
 * ||     1     ||   Random  ||       1
 * ||   Random  ||     0     ||       1
 * ||     <0    ||Not Integer||       0
 * ============================================
 */
template<int W_,int I_>
ap_fixed<W_,I_> pow(ap_fixed<W_,I_> x, ap_fixed<W_,I_> y) {
    const static int F_ = W_ - I_;
    //only support ap_fixed with I < 34 and F < 33
    if (I_>34) return 0;
    else if (F_>33) return 0;
    ap_fixed<W_,I_> r = 0;
    
    bool r_is_neg = 0;
    bool y_is_frac = 0;
    //y is fraction or not
    if (F_>0&&y(F_-1,0)>0) 
        y_is_frac =1;
    if (x==0) {
        //x==0, y==0, return 0
        if (y==0&&I_>1) {
            return 1;
        //x==0, y==0, return 0
        } else if (y>0) {
            return 0;
        } else {
        //x==0, y<0, return the Max
            #pragma unroll
            for (int j = 0; j < W_-1; j++) r[j] = 1;
               r[W_-1] = 0;
            return r;
        }
    } else if (x<0) {
        //x<0 and y is fractoin, return 0(NaN)
        if (y_is_frac) return 0;
        //x<0 and y is integer, if y is odd, r is positive otherwise r is negative.
        else {
            if (y==0||(I_>1&&y[F_]==0))
                r_is_neg = 0;  
            else 
                r_is_neg = 1; 
        }
    }
    //|x|
    ap_fixed<W_+1,I_+1> x_e_1 = x;
    //avoid overflow after abs
    ap_fixed<W_+1,I_+1> x_p = 0;
    x_p = fabs_fixed(x_e_1);
    //ln(|x|)
    //extend width of fraction
    const static int E_l = I_+2;
    const static int F_l = W_+E_l;
    //The output of log shouble be less then 2^5
    const static int I_l = I_+1>6?I_+1:6;
    const static int W_l = F_l + I_l;
    //std::cout<<"[POW]x_p="<<x_p.to_string()<<std::endl;
    ap_fixed<W_l,I_l> x_l = x_p;
    //-------------------------------------------------------
    //std::cout<<"[POW]x_l="<<x_l.to_string()<<std::endl;
    ap_fixed<W_l,I_l> ln_x = log_apfixed_reduce::log(x_l);
    //std::cout<<"[POW]ln_x="<<ln_x.to_string()<<std::endl;
    //range of log is less 2^5
    ap_fixed<F_l+6,6> ln_x_s = ln_x;
    //---------------------------------------------------------
    //y*ln(|x|)--Maxprecision=2^(-W-2) 
    const static int FI_m = W_+2; 
    //The input of exp should be less then 2^5. If it > 2^5, i will definitely overflow. 
    const static int I_m = I_>6 ? I_ : 6;//add 1 due to min value is -2^I;
    const static int WI_m = FI_m + I_m; 
    const static int WO_m = F_ + I_m;
    ap_fixed<F_l+W_+6,6+I_> mul_y_ln = ln_x_s * y;
    //std::cout<<"[POW]mul_y_ln="<<mul_y_ln.to_string()<<std::endl;
    //Rounding 
    //ap_fixed<1+, 1> delta = 0;
    //delta[delta.wl()-1] = mul_e[mul_e.wl()-1];
    //delta[delta.wl()-delta.iwl()-1] = 1;
    //mul_e = mul_e + ( delta >> wf_m );
    //std::cout<<"mul_e="<<mul_e.to_string()<<std::endl;
    
    //check overflow before truncate
    ap_fixed<WO_m,I_m> exp_r = 0;
    ap_fixed<W_,I_> r_1 = 0;
    bool m_overf = 0;
    bool e_overf = 0;
    bool overf   = 0;
    //check overflow or underflow after multiply
    #pragma unroll
    for (int j = F_l+F_+I_m-1; j < F_l+W_+5; j++)
        if (mul_y_ln[F_l+W_+5]!=mul_y_ln[j])
            m_overf = 1;

    if (!m_overf) {
        //truncate multiply result
        ap_fixed<WI_m,I_m> mul_y_ln_s = mul_y_ln;
        //std::cout<<"[POW]mul_y_ln_s="<<mul_y_ln_s.to_string()<<std::endl;
        exp_r = exp_reduce::exp_core<WO_m,I_m,WI_m>(mul_y_ln_s);// 
        //std::cout<<"[POW]exp_r="<<exp_r.to_string()<<std::endl;
        //check overflow after exp
        if (I_<I_m) {
            #pragma unroll
            for (int j = WO_m-1; j >= W_-1; j--) {
                if (exp_r[j])
                    e_overf=1;
            }
        }
        r_1 = exp_r;
    }
    //overflow when exp reulst overflow or multiply result is positive and oveflow
    if (e_overf||(m_overf&&!mul_y_ln[F_l+W_+5])) {
        overf = 1;
    } 
    //sig(x)^y = -1
    if (r_is_neg) {
        //overflow, return MIN
        if (overf) {
            r=0;
            r[W_-1]=1;
        //return -exp(y*log(|x|))
        } else {
            if (r_1!=0) {
                r = -r_1;
                r[W_-1] = 1;
            }
        }
    //sig(x)^y =1
    } else {
        // return MAX
        if(overf) {
            r[W_-1] = 0;
            #pragma unroll
            for (int j = W_-2; j >= 0; j--)
               r[j] = 1;
        //return exp(y*log(|x|)
        } else {
            r = r_1;
        }
    }
    //std::cout<<"[OWW]r="<<r.to_double()<<std::endl;
    return r;
}

template<int W_, int I_>
ap_ufixed<W_,I_> pow(ap_ufixed<W_,I_> x, ap_ufixed<W_,I_> y) {
    ap_fixed<W_+1,I_+1> xf = x;
    ap_fixed<W_+1,I_+1> yf = y;
    return pow(xf,yf);
}

template<int I_>
ap_int<I_> pow(ap_int<I_> x, ap_int<I_> y) {
    ap_fixed<I_,I_> xf = x;
    ap_fixed<I_,I_> yf = y;
    return pow(xf,yf);
}

template<int I_>
ap_uint<I_> pow(ap_uint<I_> x,ap_uint<I_> y) {
    ap_fixed<I_+1,I_+1> xf = x;
    ap_fixed<I_+1,I_+1> yf = y;
    return pow(xf,yf);
}
}
#endif
