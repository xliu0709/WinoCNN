#include <ap_int.h>
#include <dsp_builtins.h>
#include "wino_macro.h"
#include "wino_struct.h"
#include "misc.h"
#include "../testbench/debug.hpp"
#define OUTPUT_BIT 0





template<int dummy>
void compute(
    ap_int<8> weight_value[8][8],
    ap_int<8> invalue[8][2],
    ap_int<18> product[8][4][2]
    ,ap_uint<1> ap_clk_div2
    )
{



    #pragma HLS pipeline
    #pragma HLS array_partition  variable=weight_value complete dim=1
    #pragma HLS array_partition  variable=weight_value complete dim=2

    #pragma HLS array_partition  variable=invalue complete dim=1
    #pragma HLS array_partition  variable=invalue complete dim=2

    #pragma HLS array_partition  variable=product complete dim=1
    #pragma HLS array_partition  variable=product complete dim=2
    #pragma HLS array_partition  variable=product complete dim=3

    for(int i=0;i<8;i++)
    {
        #pragma HLS unroll
        for(int j=0;j<4;j++)
        {
            #pragma HLS unroll
            for(int b=0;b<2;b++)
            {
                #pragma HLS unroll
                ap_uint<41> temp;
                product[i][j][0]=__builtin_mac16x2(invalue[j*2][0],weight_value[i][j*2],invalue[j*2+1][0],weight_value[i][j*2+1],0,0,ap_clk_div2);
                product[i][j][1]=__builtin_mac16x2(invalue[j*2][1],weight_value[i][j*2],invalue[j*2+1][1],weight_value[i][j*2+1],0,0,ap_clk_div2);

                // (invalue[j*2][0],invalue[j*2+1][0],invalue[j*2][1],invalue[j*2+1][1],weight_value[i][j*2],weight_value[i][j*2+1],0,0,ap_clk_div2);
                // product[i][j][0]=temp.range(38,21);
                // =temp.range(18,0);
            }
        }
    }
}
#pragma SDS data zero_copy(weight0[0:WEIGHT_PORT_DEPTH])
#pragma SDS data zero_copy(weight1[0:WEIGHT_PORT_DEPTH])
#pragma SDS data zero_copy(weight2[0:WEIGHT_PORT_DEPTH])
#pragma SDS data zero_copy(weight3[0:WEIGHT_PORT_DEPTH])
#pragma SDS data zero_copy(in[0:WEIGHT_PORT_DEPTH])
#pragma SDS data zero_copy(output[0:WEIGHT_PORT_DEPTH])


#pragma SDS data sys_port(weight0:ps_e_S_AXI_HP0_FPD)
#pragma SDS data sys_port(weight1:ps_e_S_AXI_HP1_FPD)
#pragma SDS data sys_port(weight2:ps_e_S_AXI_HP2_FPD)
#pragma SDS data sys_port(weight3:ps_e_S_AXI_HP3_FPD)
#pragma SDS data sys_port(in:ps_e_S_AXI_HP0_FPD)
#pragma SDS data sys_port(output:ps_e_S_AXI_HP1_FPD)
















typedef struct{

    bool pingpong;

    // iteration_counter
    ap_int<16> iter_idx;

    ap_int<16> read_update_cnt_lvl0;
    ap_int<16> process_update_cnt_lvl0;
    ap_int<16> write_update_cnt_lvl0;

    ap_int<16> update_cnt_lvl1;
    ap_int<16> update_cnt_lvl2;

    // read bound
    ap_int<16> read_iter_bound;

    ap_int<32> read_ddr_offset_port0;
    ap_int<32> read_ddr_offset_port1;

    ap_uint<16> read_ddr_length_port0;
    ap_uint<16> read_ddr_length_port1;

    ap_uint<16> read_buffer_offset;

    //maxpool specific value
    ap_uint<32> read_depth_initial_offset;

    //maxpool compute variable
    ap_uint<16> read_featuremap_start_buffer_addr;
    ap_uint<16> process_featuremap_start_buffer_addr;
    ap_uint<16> write_depth_tile_cnt;// increment misc_desc.depth_tile_size each tile


    

    

}MiscIterInfo_t;




//* initialization of iteration info
void init_iter_info(
    MiscDesc_t misc_desc,
    MiscIterInfo_t &iter_info
)
{
    iter_info.iter_idx=0;
    iter_info.read_update_cnt_lvl0=0;
    iter_info.process_update_cnt_lvl0=-1;
    iter_info.write_update_cnt_lvl0=-2;
    
    iter_info.read_iter_bound = misc_desc.first_required_rows;

    iter_info.read_depth_initial_offset=0;
    iter_info.read_ddr_offset_port0=0;
    iter_info.read_ddr_offset_port1=misc_desc.depth_addr_offset_step;
    iter_info.read_ddr_length_port0=misc_desc.depth_addr_offset_step;
    iter_info.read_ddr_length_port1=misc_desc.depth_addr_offset_step;
    iter_info.read_buffer_offset=0;

    iter_info.read_featuremap_start_buffer_addr=0;
    iter_info.process_featuremap_start_buffer_addr=0;


    iter_info.write_depth_tile_cnt=0;
}


void update_iter_info_maxpool(
    MiscDesc_t misc_desc,
    MiscIterInfo_t &iter_info
)
{
    std::cout<<"update_iter_info_maxpool"<<std::endl;
    //read update
    ap_int<16> update_bound_lvl0=misc_desc.update_bound_lvl0;
    //updating read_iter_bound
    if(iter_info.read_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.read_featuremap_start_buffer_addr+=misc_desc.buffer_featuremap_offset_step;
        iter_info.read_buffer_offset=iter_info.read_featuremap_start_buffer_addr;
    }
    else if(iter_info.read_update_cnt_lvl0==0)
    {
   
        iter_info.read_buffer_offset+=misc_desc.first_required_rows*misc_desc.depth_addr_offset_step;
    }
    else
    {
        iter_info.read_buffer_offset+=misc_desc.stride*misc_desc.depth_addr_offset_step;
    }
    
    if(iter_info.read_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.read_iter_bound=misc_desc.first_required_rows;
    }
    else
    {
        iter_info.read_iter_bound=misc_desc.stride;
    }
    //* updating initial offset
    if(iter_info.read_update_cnt_lvl0==update_bound_lvl0) //normal iteration
    {
        iter_info.read_depth_initial_offset+=misc_desc.depth_addr_offset_step*2;

        iter_info.read_ddr_offset_port0=iter_info.read_depth_initial_offset;
        iter_info.read_ddr_offset_port1=iter_info.read_depth_initial_offset+misc_desc.depth_addr_offset_step;
    }   
    else if(iter_info.read_update_cnt_lvl0==0)
    {
        iter_info.read_ddr_offset_port0+=misc_desc.in_row_first_required_addr_offset_step;
        iter_info.read_ddr_offset_port1+=misc_desc.in_row_first_required_addr_offset_step; 
    }
    else
    {
        iter_info.read_ddr_offset_port0+=misc_desc.in_row_stride_addr_offset_step;
        iter_info.read_ddr_offset_port1+=misc_desc.in_row_stride_addr_offset_step; 
    }



    if(iter_info.process_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.process_featuremap_start_buffer_addr+=misc_desc.buffer_featuremap_offset_step;
    }
    


    if(iter_info.read_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.read_update_cnt_lvl0=0;
    }
    else
    {
        iter_info.read_update_cnt_lvl0++;
    }



    //* update process labels


    if(iter_info.write_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.write_update_cnt_lvl0=0;
        iter_info.write_depth_tile_cnt++;
    }
    else
    {
        iter_info.write_update_cnt_lvl0++;
    }
    

    if(iter_info.process_update_cnt_lvl0==update_bound_lvl0)
    {
        iter_info.process_update_cnt_lvl0=0;
    }
    else
    {
        iter_info.process_update_cnt_lvl0++;
    }
}


void update_iter_info(
    MiscDesc_t misc_desc,
    MiscIterInfo_t &iter_info
)
{
   if(misc_desc.layer_type==MAXPOOL)
   {
       update_iter_info_maxpool(misc_desc,iter_info);
   }   
   if(iter_info.pingpong) iter_info.pingpong=false;
   else iter_info.pingpong=true;  
}


//* declaration of read
template<int dummy>
void misc_common_read(
    ap_uint<128>* inmem,
    ap_uint<16> inbuffer[8][MISC_INBUFF_DEPTH],
    int memoffset,
    int bufferoffest,
    int length
)
{

    ap_uint<128>* inmem_offset= inmem+memoffset;
    for(int i=0;i<length;i++)
    {
        #pragma HLS pipeline
        ap_uint<128> temp=inmem_offset[i];
        for(int bank_idx=0;bank_idx<8;bank_idx++)
        {
            #pragma HLS unroll
            ap_uint<MISC_INBUFF_DEPTH_BITS> bufferaddress=bufferoffest+i;
            inbuffer[bank_idx][bufferaddress]=temp.range(bank_idx*16+15,bank_idx*16);
        }
    }
}


typedef struct{
    int iter_idx;
    ap_uint<32> ddr_offset_port0;
    ap_uint<32> ddr_offset_port1;
    ap_uint<16> ddr_length_port0;
    ap_uint<16> ddr_length_port1;
    ap_uint<16> buffer_offset;
} MiscReadIterInfo_t;


void init_read_iter_info(
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info,
    MiscReadIterInfo_t &read_iter_info
)
{
    read_iter_info.iter_idx=0;
    if(misc_desc.layer_type==MAXPOOL)
    {
        read_iter_info.ddr_offset_port0=iter_info.read_ddr_offset_port0;
        read_iter_info.ddr_offset_port1=iter_info.read_ddr_offset_port1;
        read_iter_info.ddr_length_port0=iter_info.read_ddr_length_port0;
        read_iter_info.ddr_length_port1=iter_info.read_ddr_length_port1;
        read_iter_info.buffer_offset=iter_info.read_buffer_offset;
    }    
}

void update_read_iter_info(
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info,
    MiscReadIterInfo_t &read_iter_info
)
{
    if(misc_desc.layer_type==MAXPOOL)
    {
        read_iter_info.ddr_offset_port0+=misc_desc.in_row_addr_offset_step; // let it load in next row
        read_iter_info.ddr_offset_port1+=misc_desc.in_row_addr_offset_step; // let it load in next row
        read_iter_info.buffer_offset+=iter_info.read_ddr_length_port1;// next row is stored
    }
}

void misc_layer_read(
    ap_uint<128>* in0,
    ap_uint<128>* in1,
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info
)
{
    std::cout<<"misc_layer_read"<<std::endl;
    // std::cout<<"iter_info.ddr_offset_port0: "<< iter_info.read_ddr_offset_port0<<std::endl;
    // std::cout<<"iter_info.ddr_offset_port1: "<< iter_info.read_ddr_offset_port1<<std::endl;
    // std::cout<<"iter_info.ddr_length_port0: "<< iter_info.read_ddr_length_port0<<std::endl;
    // std::cout<<"iter_info.ddr_length_port1: "<< iter_info.read_ddr_length_port1<<std::endl;

    if(iter_info.iter_idx+1==misc_desc.iter_bound) return;
    MiscReadIterInfo_t read_iter_info;
  
    init_read_iter_info(misc_desc,iter_info,read_iter_info);
    
    // std::cout<<"iter_info.read_iter_bound "<< iter_info.read_iter_bound<<std::endl;
        // std::cout<<"read_iter_info.ddr_length_port1: "<< read_iter_info.ddr_length_port1<<std::endl;
    for(;read_iter_info.iter_idx<iter_info.read_iter_bound; read_iter_info.iter_idx++)
    {
        
        // std::cout<<"read_iter_info.buffer_offset: "<< read_iter_info.buffer_offset<<std::endl;
        // std::cout<<"read_iter_info.ddr_offset_port0: "<< read_iter_info.ddr_offset_port0<<std::endl;
        // std::cout<<"read_iter_info.ddr_offset_port1: "<< read_iter_info.ddr_offset_port1<<std::endl;
        // std::cout<<"read_iter_info.ddr_length_port0: "<< read_iter_info.ddr_length_port0<<std::endl;
        // std::cout<<"read_iter_info.ddr_length_port1: "<< read_iter_info.ddr_length_port1<<std::endl;
        // getchar();

        misc_common_read<0>(in0,inbuffer[0],read_iter_info.ddr_offset_port0,read_iter_info.buffer_offset,read_iter_info.ddr_length_port0);
        misc_common_read<1>(in0,inbuffer[1],read_iter_info.ddr_offset_port1,read_iter_info.buffer_offset,read_iter_info.ddr_length_port1);
        update_read_iter_info(misc_desc,iter_info,read_iter_info);
    }
}


//* delcaration of write function

template<int dummy>
void misc_common_write(
    ap_uint<128>* outmem,
    ap_uint<18> outbuffer[8][MISC_OUTBUFFER_DEPTH],
    int memoffset,
    int bufferoffest,
    int length
)
{
    if(length==0) return;

    ap_uint<128>* outmem_offset= outmem+memoffset;
    for(int i=0;i<length;i++)
    {
        #pragma HLS pipeline
        ap_uint<128> temp;
        for(int bank_idx=0;bank_idx<8;bank_idx++)
        {
            #pragma HLS unroll
            ap_uint<MISC_INBUFF_DEPTH_BITS> bufferaddress=bufferoffest+i;
            temp.range(bank_idx*16+15,bank_idx*16)=outbuffer[bank_idx][bufferaddress];
        }
        outmem_offset[i]=temp;
    }
}



//* declaration of process function

void maxpool_process(
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );



void linear_process(
    ap_uint<128>* weight0,
    ap_uint<128>* weight1,
    ap_uint<128>* weight2,
    ap_uint<128>* weight3,
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );

void eltwise_process(
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );

void misc_layer_process(
    ap_uint<128>* weight0,
    ap_uint<128>* weight1,
    ap_uint<128>* weight2,
    ap_uint<128>* weight3,
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info
)
{
    switch(misc_desc.layer_type)
    {
        case MAXPOOL: 
            maxpool_process(inbuffer,outbuffer,misc_desc,iter_info); break;
        // case LINEAR: 
        //     linear_process(weight0,weight1,weight2,weight3, inbuffer,outbuffer, misc_desc,iter_info);break;
        // case ELTWISE: 
        //     eltwise_process(inbuffer,outbuffer,misc_desc,iter_info); break;
        default: break;
    }  
}


void maxpool_write(
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<18> outbuffer0[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );

void linear_write(
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<18> outbuffer0[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );

void eltwise_write(
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<18> outbuffer0[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info );

//* declaration of write function
void misc_layer_write(
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info 
)
{
    switch(misc_desc.layer_type)
    {
        // case MAXPOOL: 
        //     maxpool_write(out0,out1,outbuffer,misc_desc,iter_info); break;
        // case LINEAR: 
        //     linear_write(out0,out1,outbuffer,misc_desc,iter_info); break;
        // case ELTWISE: 
        //     eltwise_write(out0,out1,outbuffer,misc_desc,iter_info); break;
        default: break;
    }  
}







void misc_layer(
    ap_uint<128>* in0,
    ap_uint<128>* in1,
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<128>* weight0,
    ap_uint<128>* weight1,
    ap_uint<128>* weight2,
    ap_uint<128>* weight3,
    MiscDesc_t &misc_desc
)
{
    static ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH];
    #pragma HLS array_partition variable = inbuffer  complete dim =1
    static ap_uint<18> outbuffer0[2][8][MISC_OUTBUFFER_DEPTH];
    #pragma HLS array_partition variable = outbuffer0  complete dim =1
    #pragma HLS array_partition variable = outbuffer0  complete dim =2
    static ap_uint<18> outbuffer1[2][8][MISC_OUTBUFFER_DEPTH];
    #pragma HLS array_partition variable = outbuffer1  complete dim =1
    #pragma HLS array_partition variable = outbuffer1  complete dim =2



    MiscIterInfo_t iter_info;

    //initialize the layer for the first read

        
    init_iter_info(misc_desc,iter_info);
    std::cout<<"Iter bound "<<misc_desc.iter_bound<<std::endl;
    std::cout<<"Iter iter_idx "<<iter_info.iter_idx<<std::endl;
    misc_layer_read(in0,in1,inbuffer,misc_desc,iter_info);
    attach_misc_inbuffer_content<MISC_INBUFF_DEPTH>(inbuffer,"misc_buffer_content.txt");
    std::cout<<"Iter iter_idx "<<iter_info.iter_idx<<std::endl;

    update_iter_info(misc_desc,iter_info);
    std::cout<<"Iter iter_idx "<<iter_info.iter_idx<<std::endl;
    std::cout<<"Iter bound "<<misc_desc.iter_bound<<std::endl;

    for(;iter_info.iter_idx<misc_desc.iter_bound; iter_info.iter_idx++)
    {
        std::cout<<"iter_info.iter_idx "<<iter_info.iter_idx<<std::endl;

        if(iter_info.pingpong==0)
        {

            misc_layer_read(in0,in1,inbuffer,misc_desc,iter_info);
            attach_misc_inbuffer_content<MISC_INBUFF_DEPTH>(inbuffer,"misc_buffer_content.txt");
            misc_layer_process(weight0,weight1,weight2,weight3,inbuffer,outbuffer0,misc_desc,iter_info);
            attach_misc_outbuffer_content<MISC_INBUFF_DEPTH>(outbuffer0,"misc_buffer_content.txt");
            misc_layer_write(out0,out1,outbuffer1,misc_desc,iter_info);
        }
        else
        {
            misc_layer_read(in0,in1,inbuffer,misc_desc,iter_info);
            attach_misc_inbuffer_content<MISC_INBUFF_DEPTH>(inbuffer,"misc_buffer_content.txt");
            misc_layer_process(weight0,weight1,weight2,weight3,inbuffer,outbuffer1,misc_desc,iter_info);
            attach_misc_outbuffer_content<MISC_INBUFF_DEPTH>(outbuffer1,"misc_obuffer_content.txt");
            misc_layer_write(out0,out1,outbuffer0,misc_desc,iter_info);           
        }
        update_iter_info(misc_desc,iter_info);
    }

    if(iter_info.pingpong==0)
    {
        misc_layer_write(out0,out1,outbuffer1,misc_desc,iter_info);
    }
    else
    {
        misc_layer_write(out0,out1,outbuffer0,misc_desc,iter_info);           
    }

}



void maxpool_process(
    ap_uint<16> inbuffer[2][8][MISC_INBUFF_DEPTH],
    ap_uint<18> outbuffer[2][8][MISC_OUTBUFFER_DEPTH],
    MiscDesc_t misc_desc,
    MiscIterInfo_t iter_info )
{
    std::cout<<iter_info.process_featuremap_start_buffer_addr<<std::endl;
    // getchar();
    int out_row_idx = iter_info.process_update_cnt_lvl0;
    int in_row_offset=-misc_desc.pad_size+out_row_idx*misc_desc.stride;
   

    for(int depth_tile_idx=0;depth_tile_idx<misc_desc.depth_tile_size;depth_tile_idx++)
    for(int out_col_idx=0;out_col_idx<misc_desc.outwidth;out_col_idx++)
    for(int win_row_idx=0;win_row_idx<misc_desc.kernel_size;win_row_idx++)
    for(int win_col_idx=0;win_col_idx<misc_desc.kernel_size;win_col_idx++)
    {
        int in_row_idx=in_row_offset+win_row_idx;
        int in_col_idx=out_col_idx*misc_desc.stride-misc_desc.pad_size+win_col_idx;
        int in_col_buffer_addr_offset=in_col_idx;
        int depth_tile_buffer_addr_offset=misc_desc.inwidth_align8*depth_tile_idx;

        int in_row_buffer_addr_offset= in_row_idx*misc_desc.inwidth_align8*misc_desc.depth_tile_size;


        ap_uint<MISC_INBUFF_DEPTH_BITS> inbuffer_address=
                            iter_info.process_featuremap_start_buffer_addr+
                            in_row_buffer_addr_offset+
                            depth_tile_buffer_addr_offset+
                            in_col_buffer_addr_offset;
        
        ap_int<8> in_value[2][2][8];
        
        for(int b=0;b<2;b++)
        for(int i=0;i<8;i++)
        {
            #pragma HLS unroll
            if(in_row_idx>=0 && in_row_idx<misc_desc.inheight && in_col_idx >=0 && in_col_idx<misc_desc.inwidth)
                (in_value[1][b][i],in_value[0][b][i])=inbuffer[b][i][inbuffer_address];
            else
                (in_value[1][b][i],in_value[0][b][i])=(ap_uint<16>) 0;
        }


        ap_int<8> out_value_residual[2][2][8];
        
        int out_col_address=out_col_idx;
        int out_depth_buffer_addr=depth_tile_idx*misc_desc.outwidth_align8;
        ap_uint<MISC_OUTBUFFER_DEPTH_BITS> outbuffer_addr=out_depth_buffer_addr+out_col_address;

        for(int b=0;b<2;b++)
        for(int i=0;i<8;i++)
        {
            #pragma HLS unroll
                if( win_row_idx==0 && win_col_idx==0)
                    (out_value_residual[1][b][i],out_value_residual[0][b][i])=(ap_uint<16>) 0;
                else
                    (out_value_residual[1][b][i],out_value_residual[0][b][i])=outbuffer[b][i][outbuffer_addr];
            
        }

     
        ap_int<8> out_value[2][2][8];

        for(int b=0;b<2;b++)
        for(int i=0;i<8;i++)
        {
            out_value[0][b][i]=in_value[0][b][i]>out_value_residual[0][b][i]?in_value[0][b][i]:out_value_residual[0][b][i];
            out_value[1][b][i]=in_value[1][b][i]>out_value_residual[1][b][i]?in_value[1][b][i]:out_value_residual[1][b][i];
        }
        
    
        for(int b=0;b<2;b++)
        for(int i=0;i<8;i++)
        {
            #pragma HLS unroll
            outbuffer[b][i][outbuffer_addr]=(out_value[1][b][i],out_value[0][b][i]);
        }
    }
}




// void FC_layer(
//     ap_uint<128>* in,
//     ap_uint<128>* output,
//     ap_uint<128>* weight0,
//     ap_uint<128>* weight1,
//     ap_uint<128>* weight2,
//     ap_uint<128>* weight3,
//     int indepth_ceildiv8,
//     int outdepth_ceildiv8,
//     int dumpflag,
//     int offset_bit
//     #ifdef __SDSVHLS__
//     ,ap_uint<1> ap_clk_div2
//     #endif
//     )
// {

//     #ifndef __SDSVHLS__
//     ap_uint<1> ap_clk_div2=0;
//     #endif
//     int length=indepth_ceildiv8*outdepth_ceildiv8;

//     static ap_uint<16> inbuffer[8][1024];
//     #pragma HLS array_partition variable = inbuffer  complete dim =1
//     static ap_uint<18> outbuffer[8][2][1024];
//     #pragma HLS array_partition variable = outbuffer  complete dim =1
//     #pragma HLS array_partition variable = outbuffer  complete dim =2

//     LOOP_READ:for(int memaddr=0;memaddr<indepth_ceildiv8;memaddr++)
//     {
//         #pragma HLS pipeline
//         ap_uint<128> temp=in[memaddr];
//         for(int bank_idx=0;bank_idx<8;bank_idx++){
//             #pragma HLS unroll factor=8
//             inbuffer[bank_idx][memaddr]=temp.range(bank_idx*8+8,bank_idx*8);
//         }
//     }

//     ap_uint<10> outbuffer_addr=0;
//     ap_uint<10> inbuffer_ddr=0;
    
//     LOOP_COMPUTE:for(int cycle=0;cycle<length;cycle++)
//     {
//         #pragma HLS pipeline
//         #pragma HLS dependence variable=outbuffer intra false 
//         #pragma HLS dependence variable=outbuffer inter false 
//         ap_uint<128> temp0=weight0[cycle];
//         ap_uint<128> temp1=weight1[cycle];
//         ap_uint<128> temp2=weight2[cycle];
//         ap_uint<128> temp3=weight3[cycle];

//         ap_int<8> weight_value[8][8];
//         #pragma HLS array_partition variable= weight_value  dim=1 complete
//         #pragma HLS array_partition variable= weight_value  dim=2 complete
    
//         for(int i=0;i<8;i++){
//             #pragma HLS unroll
//             weight_value[0][i]= temp0.range(i*8+7,i*8);
//             weight_value[1][i]= temp0.range(i*8+71,i*8+64);
//             weight_value[2][i]= temp1.range(i*8+7,i*8);
//             weight_value[3][i]= temp1.range(i*8+71,i*8+64);
//             weight_value[4][i]= temp2.range(i*8+7,i*8);
//             weight_value[5][i]= temp2.range(i*8+71,i*8+64);
//             weight_value[6][i]= temp3.range(i*8+7,i*8);   
//             weight_value[7][i]= temp3.range(i*8+71,i*8+64);   
//         }


//         ap_int<8> invalue[8][2];
//         #pragma HLS array_partition variable= invalue complete dim=2
//         #pragma HLS array_partition variable= invalue complete dim=1 
        
//         for(int i=0;i<8;i++)
//         {
//             #pragma HLS unroll
//             (invalue[i][1],invalue[0])=inbuffer[i][inbuffer_ddr];
//         }

//         ap_int<18> product[8][4][2];
//         #pragma HLS array_partition variable= product complete dim=1 
//         #pragma HLS array_partition variable= product complete dim=2
//         #pragma HLS array_partition variable= product complete dim=3


//         compute<0>(weight_value,invalue,product,ap_clk_div2);
//         ap_int<18> out_residual[8][2];
//         #pragma HLS array_partition variable= out_residual complete dim=2
//         #pragma HLS array_partition variable= out_residual complete dim=1 
        
//         bool accum=(dumpflag!=0 ||  inbuffer_ddr!=0 );
//         for(int i=0;i<8;i++)
//         {
//             #pragma HLS unroll
//             if(accum)
//             {
//                 out_residual[i][0]=outbuffer[i][0][outbuffer_addr];
//                 out_residual[i][1]=outbuffer[i][1][outbuffer_addr];
//             }
//             else
//             {
//                 out_residual[i][0]=0;
//                 out_residual[i][1]=0;
//             }
//         }

 


//         for(int i=0;i<8;i++)
//         {
//             #pragma HLS unroll
//             outbuffer[i][0][outbuffer_addr]= product[i][0][0] +product[i][1][0]+product[i][2][0]+product[i][3][0] +accum*out_residual[i][0];
//             outbuffer[i][1][outbuffer_addr]= product[i][0][1] +product[i][1][1]+product[i][2][1]+product[i][3][1] +accum*out_residual[i][0];
//         }  

//         if(outbuffer_addr==outdepth_ceildiv8)
//         {
//             outbuffer_addr=0;
//             inbuffer_ddr++;
//         }
//         else
//         {
//             outbuffer_addr++;
//         }        
//     }

//     if(dumpflag)
//     {

//         LOOP_WRITE:for(int memaddr=0;memaddr<outdepth_ceildiv8;memaddr++)
//         {
//             ap_uint<128> temp0=weight3[length+memaddr];
            

//             ap_int<16> bias_value[8];
//             #pragma HLS array_partition variable= bias_value  dim=1 complete
//             for(int i=0;i<8;i++)
//             {
//                 #pragma HLS unroll 
//                 bias_value[i]=temp0.range(i*16+15,i*16);
//             }
            
//             ap_int<8> result[8][2];
//             #pragma HLS array_partition variable = result dim=1
//             #pragma HLS array_partition variable = result dim=2

//             for(int i=0;i<8;i++)
//             {
//                 #pragma HLS unroll
//                 result[i][0]= (outbuffer[i][0][memaddr]+bias_value[i]).range(OUTPUT_BIT+7,OUTPUT_BIT);
//                 result[i][1]= (outbuffer[i][1][memaddr]+bias_value[i]).range(OUTPUT_BIT+7,OUTPUT_BIT);
//             }

//             ap_uint<128> write_back_temp0;

//             for(int i=0;i<8;i++)
//             {
//                 #pragma HLS unroll
//                 write_back_temp0.range(16*i+15,16*i)=(result[i][1],result[i][0]);
//             }
//             output[memaddr]=write_back_temp0;
//         }
//     }
// }
