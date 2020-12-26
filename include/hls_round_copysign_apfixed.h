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
 *     (c) Copyright 2008-2012 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/
#ifndef _X_HLS_ROUND_COPYSIGN_APFIXED_H_
#define _X_HLS_ROUND_COPYSIGN_APFIXED_H_
#include "ap_fixed.h"
#include "ap_int.h"

template <int W_, int I_>
ap_fixed<W_,I_> ceil_fixed(ap_fixed<W_,I_> x)
{
#pragma HLS pipeline
    if ( x(W_-I_-1,0) != 0 ) {
        x(W_-I_-1,0) = 0;
        x += 1;
    }
    return x;
}
template <int W_, int I_>
ap_ufixed<W_,I_> ceil_fixed(ap_ufixed<W_,I_> x)
{
#pragma HLS pipeline
    if ( x(W_-I_-1,0) != 0 ) {
        x(W_-I_-1,0) = 0;
        x += 1;
    }
    return x;
}
template <int I_>
ap_int<I_> ceil_fixed(ap_int<I_> x)
{
    return x;
}
template <int I_>
ap_uint<I_> ceil_fixed(ap_uint<I_> x)
{
    return x;
}

template <int W_, int I_>
ap_fixed<W_,I_> floor_fixed(ap_fixed<W_,I_> x)
{
    if ( x(W_-I_-1,0) != 0 ) x(W_-I_-1,0) = 0;
    return x;
}
template <int W_, int I_>
ap_ufixed<W_,I_> floor_fixed(ap_ufixed<W_,I_> x)
{
    if ( x(W_-I_-1,0) != 0 ) x(W_-I_-1,0) = 0;
    return x;
}
template <int I_>
ap_int<I_> floor_fixed(ap_int<I_> x)
{
    return x;
}
template <int I_>
ap_uint<I_> floor_fixed(ap_uint<I_> x)
{
    return x;
}

template <int W_, int I_>
ap_fixed<W_,I_> trunc_fixed(ap_fixed<W_,I_> x)
{
#pragma HLS pipeline
    if ( x(W_-I_-1,0) != 0 ) {
        x(W_-I_-1,0) = 0;
        if ( x[W_-1] )
            x += 1;
    }
    return x;
}
template <int W_, int I_>
ap_ufixed<W_,I_> trunc_fixed(ap_ufixed<W_,I_> x)
{
#pragma HLS pipeline
    if ( x(W_-I_-1,0) != 0 )
        x(W_-I_-1,0) = 0;
    return x;
}
template <int I_>
ap_int<I_> trunc_fixed(ap_int<I_> x)
{
    return x;
}
template <int I_>
ap_uint<I_> trunc_fixed(ap_uint<I_> x)
{
    return x;
}

template <int W_, int I_>
ap_fixed<W_,I_> copysign_fixed(ap_fixed<W_,I_> x, ap_fixed<W_,I_> y)
{
    x[W_-1] = y[W_-1];
    return x;
}
template <int W_, int I_>
ap_ufixed<W_,I_> copysign_fixed(ap_ufixed<W_,I_> x, ap_ufixed<W_,I_> y)
{
    return x;
}
template <int I_>
ap_int<I_> copysign_fixed(ap_int<I_> x, ap_int<I_> y)
{
    x[I_-1] = y[I_-1];
    return x;
}
template <int I_>
ap_uint<I_> copysign_fixed(ap_uint<I_> x, ap_uint<I_> y)
{
    return x;
}

template <int W_, int I_>
ap_fixed<W_,I_> fabs_fixed(ap_fixed<W_,I_> x)
{
    ap_fixed<W_,I_> xs       = -x;
                    xs[W_-1] = 0;
    return ( ( x[W_-1] ) ? xs : x );
}
template <int W_, int I_>
ap_ufixed<W_,I_> fabs_fixed(ap_ufixed<W_,I_> x)
{
    return x;
}
template <int I_>
ap_int<I_> fabs_fixed(ap_int<I_> x)
{
    ap_int<I_> xs       = -x;
               xs[I_-1] = 0;
    return ( ( x[I_-1] ) ? xs : x );
}
template <int I_>
ap_uint<I_> fabs_fixed(ap_uint<I_> x)
{
    return x;
}
#endif
