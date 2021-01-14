#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "wino_macro.h"
#include <ap_int.h>
#include <hls_stream.h>


#include "wino_IO.cpp"
#include "wino_cell.cpp"


#include "wino_struct.h"



void load_params(
    ap_int<32>* mem_params,
    ConvDesc_t &conv_desc
)
{
    ap_int<32> params[128];

    for(int i=0;i<128;i++)
    {
        #pragma HLS pipeline   
        params[i]=mem_params[i];
    }
        //Original signal
    conv_desc.inheight=params[0];
    conv_desc.inwidth=params[1];
    conv_desc.indepth=params[2];
    conv_desc.outheight=params[3];
    conv_desc.outwidth=params[4];
    conv_desc.outdepth=params[5];
    conv_desc.kernel_size=params[6];
    conv_desc.pad_size=params[7];
    conv_desc.stride=params[8];

        //wino related
    conv_desc.wino3x3_flag=params[9]; 
        // 1: 3x3, 0:5x5
    conv_desc.wino_output_tile_size=params[10];

        //input buffer related
    conv_desc.indepth_align_minitile_size=params[11];
    conv_desc.indepth_align8=params[12];
    conv_desc.indepth_ceildiv8=params[13];
    conv_desc.inwidth_ceildiv_inbufferwidth=params[14];
    conv_desc.inwidth_align8=params[15];
    conv_desc.group_indepth_offset=params[16];
    conv_desc.group_indepth=params[17];
    conv_desc.input_ddr_bytes=params[18];
    conv_desc.input_ddr_128bits=params[19];
    conv_desc.group_indepth_x_inwidth_align8_by8=params[20];
    conv_desc.group_indepth_offset_x_inwidth_align8_by8=params[21];
    conv_desc.input_load_burst_length=params[22];
    conv_desc.buffer_address_mid_increment_step=params[23];
    conv_desc.row_address_bitnumber_flag=params[24];
        


        // ouput_buffer_related
    conv_desc.outwidth_align8=params[25];
    conv_desc.outdepth_align8=params[26];
    conv_desc.outheight_align4=params[27];
    conv_desc.outdepth_align_minitile_size=params[28];
    conv_desc.group_outdepth_offset=params[29];
    conv_desc.group_outdepth=params[30];

    conv_desc.output_ddr_bytes=params[31];
    conv_desc.output_ddr_128bits=params[32];


        // Weight_related
    conv_desc.weightbuffer_load_indepth_number=params[33];
    conv_desc.weightbuffer_load_indepth_step=params[34];
    conv_desc.weightbuffer_load_outdepth_number=params[35];
    conv_desc.weightbuffer_load_outdepth_step=params[36];
    conv_desc.weightbuffer_indepth_minitile_number=params[37];
    conv_desc.weightbuffer_outdepth_minitile_number=params[38];
    conv_desc.weightbuffer_total_load_number=params[39];

        //weight_load hardware
    conv_desc.weightDDR_buffer_burst_length=params[40];
    conv_desc.weightDDR_port_burst_length=params[41];
    conv_desc.weightDDR_burst_number=params[42];
    conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1=params[43];
    conv_desc.loop_start_output_baserowcol_reset_cycle=params[44];
    conv_desc.loop_weight_feed_bound=params[45];
    
        // input buffer feeding related
    conv_desc.wino_out_size_by_wino_width=params[46];
    conv_desc.wino_tile_number_in_outwidth=params[47];

    conv_desc.loop_outdepth_minitile_baseidx_reset_cycle=params[48];
    conv_desc.loop_wino_tile_col_reset_cycle=params[49];
    conv_desc.loop_wino_tile_row_reset_cycle=params[50];

    conv_desc.buffer_address_mid_minitile_depth_step=params[51];
    conv_desc.input_buffer_feeding_loop_bound=params[52];
    
    conv_desc.input_transform_feeding_loop_bound=params[53];

        // row_tile calculation , these parameter have to be solved after weight parameters are decided.
    conv_desc.out_rowstep=params[54];
    conv_desc.wino_tile_number_in_out_rowstep=params[55];


        // wino computation
    conv_desc.total_input_stream_tile=params[56];
    conv_desc.loop_omini_base_reset_cycle=params[57];
    conv_desc.loop_wino_cell_bound=params[58];
    conv_desc.loop_wino_tile_rowcol_self_reset_cycle_min1=params[59];
    conv_desc.loop_iload_reset_cycle=params[60];
    conv_desc.outbuffer_oload_increment_step=params[61];
    conv_desc.outbuffer_omini_increment_step=params[62];
        
        //output write back 
    conv_desc.outdepth_ceildiv8=params[63];
    conv_desc.output_burst_length=params[64];
    conv_desc.write_back_flag=params[65];
    conv_desc.wino_col_pix_upper_bound=params[66];
    conv_desc.wino_tile_number_rowcol=params[67];
    conv_desc.out_ddr_increment_step=params[68];

    conv_desc.merge_kernel_size=params[69];
    conv_desc.merge_kernel_step=params[70];
    conv_desc.merge_kernel_flag=params[71];
    conv_desc.scale_oback_int=params[72];
    conv_desc.merge_weight_row_step=params[73];
    conv_desc.merge_weight_col_step=params[74];
    conv_desc.relu_flag=params[75];
}

void write_params(
    ap_uint<128>* mem_params,
    ConvDesc_t &conv_desc
)
{

    mem_params[0]=(ap_uint<128>) conv_desc.inheight;
    mem_params[1]=(ap_uint<128>) conv_desc.inwidth;
    mem_params[2]=(ap_uint<128>) conv_desc.indepth;
    mem_params[3]=(ap_uint<128>) conv_desc.outheight;
    mem_params[4]=(ap_uint<128>) conv_desc.outwidth;
    mem_params[5]=(ap_uint<128>) conv_desc.outdepth;
    mem_params[6]=(ap_uint<128>) conv_desc.kernel_size;
    mem_params[7]=(ap_uint<128>) conv_desc.pad_size;
    mem_params[8]=(ap_uint<128>) conv_desc.stride;
    mem_params[9]=(ap_uint<128>) conv_desc.wino3x3_flag ;
    mem_params[10]=(ap_uint<128>) conv_desc.wino_output_tile_size;
    mem_params[11]=(ap_uint<128>) conv_desc.indepth_align_minitile_size;
    mem_params[12]=(ap_uint<128>) conv_desc.indepth_align8;
    mem_params[13]=(ap_uint<128>) conv_desc.indepth_ceildiv8;
    mem_params[14]=(ap_uint<128>) conv_desc.inwidth_ceildiv_inbufferwidth;
    mem_params[15]=(ap_uint<128>) conv_desc.inwidth_align8;
    mem_params[16]=(ap_uint<128>) conv_desc.group_indepth_offset;
    mem_params[17]=(ap_uint<128>) conv_desc.group_indepth;
    mem_params[18]=(ap_uint<128>) conv_desc.input_ddr_bytes;
    mem_params[19]=(ap_uint<128>) conv_desc.input_ddr_128bits;
    mem_params[20]=(ap_uint<128>) conv_desc.group_indepth_x_inwidth_align8_by8;
    mem_params[21]=(ap_uint<128>) conv_desc.group_indepth_offset_x_inwidth_align8_by8;
    mem_params[22]=(ap_uint<128>) conv_desc.input_load_burst_length;
    mem_params[23]=(ap_uint<128>) conv_desc.buffer_address_mid_increment_step;
    mem_params[24]=(ap_uint<128>) conv_desc.row_address_bitnumber_flag;
    mem_params[25]=(ap_uint<128>) conv_desc.outwidth_align8;
    mem_params[26]=(ap_uint<128>) conv_desc.outdepth_align8;
    mem_params[27]=(ap_uint<128>) conv_desc.outheight_align4;
    mem_params[28]=(ap_uint<128>) conv_desc.outdepth_align_minitile_size;
    mem_params[29]=(ap_uint<128>) conv_desc.group_outdepth_offset;
    mem_params[30]=(ap_uint<128>) conv_desc.group_outdepth;
    mem_params[31]=(ap_uint<128>) conv_desc.output_ddr_bytes;
    mem_params[32]=(ap_uint<128>) conv_desc.output_ddr_128bits;
    mem_params[33]=(ap_uint<128>) conv_desc.weightbuffer_load_indepth_number;
    mem_params[34]=(ap_uint<128>) conv_desc.weightbuffer_load_indepth_step;
    mem_params[35]=(ap_uint<128>) conv_desc.weightbuffer_load_outdepth_number;
    mem_params[36]=(ap_uint<128>) conv_desc.weightbuffer_load_outdepth_step;
    mem_params[37]=(ap_uint<128>) conv_desc.weightbuffer_indepth_minitile_number;
    mem_params[38]=(ap_uint<128>) conv_desc.weightbuffer_outdepth_minitile_number;
    mem_params[39]=(ap_uint<128>) conv_desc.weightbuffer_total_load_number;
    mem_params[40]=(ap_uint<128>) conv_desc.weightDDR_buffer_burst_length;
    mem_params[41]=(ap_uint<128>) conv_desc.weightDDR_port_burst_length;
    mem_params[42]=(ap_uint<128>) conv_desc.weightDDR_burst_number;
    mem_params[43]=(ap_uint<128>) conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1;
    mem_params[44]=(ap_uint<128>) conv_desc.loop_start_output_baserowcol_reset_cycle;
    mem_params[45]=(ap_uint<128>) conv_desc.loop_weight_feed_bound;
    mem_params[46]=(ap_uint<128>) conv_desc.wino_out_size_by_wino_width;
    mem_params[47]=(ap_uint<128>) conv_desc.wino_tile_number_in_outwidth;
    mem_params[48]=(ap_uint<128>) conv_desc.loop_outdepth_minitile_baseidx_reset_cycle;
    mem_params[49]=(ap_uint<128>) conv_desc.loop_wino_tile_col_reset_cycle;
    mem_params[50]=(ap_uint<128>) conv_desc.loop_wino_tile_row_reset_cycle;
    mem_params[51]=(ap_uint<128>) conv_desc.buffer_address_mid_minitile_depth_step;
    mem_params[52]=(ap_uint<128>) conv_desc.input_buffer_feeding_loop_bound;
    mem_params[53]=(ap_uint<128>) conv_desc.input_transform_feeding_loop_bound;
    mem_params[54]=(ap_uint<128>) conv_desc.out_rowstep;
    mem_params[55]=(ap_uint<128>) conv_desc.wino_tile_number_in_out_rowstep;
    mem_params[56]=(ap_uint<128>) conv_desc.total_input_stream_tile;
    mem_params[57]=(ap_uint<128>) conv_desc.loop_omini_base_reset_cycle;
    mem_params[58]=(ap_uint<128>) conv_desc.loop_wino_cell_bound;
    mem_params[59]=(ap_uint<128>) conv_desc.loop_wino_tile_rowcol_self_reset_cycle_min1;
    mem_params[60]=(ap_uint<128>) conv_desc.loop_iload_reset_cycle;
    mem_params[61]=(ap_uint<128>) conv_desc.outbuffer_oload_increment_step;
    mem_params[62]=(ap_uint<128>) conv_desc.outbuffer_omini_increment_step;
    mem_params[63]=(ap_uint<128>) conv_desc.outdepth_ceildiv8;
    mem_params[64]=(ap_uint<128>) conv_desc.output_burst_length;
    mem_params[65]=(ap_uint<128>) conv_desc.write_back_flag;
    mem_params[66]=(ap_uint<128>) conv_desc.wino_col_pix_upper_bound;
    mem_params[67]=(ap_uint<128>) conv_desc.wino_tile_number_rowcol;
    mem_params[68]=(ap_uint<128>) conv_desc.out_ddr_increment_step;
    mem_params[69]=(ap_uint<128>) conv_desc.merge_kernel_size;
    mem_params[70]=(ap_uint<128>) conv_desc.merge_kernel_step;
    mem_params[71]=(ap_uint<128>) conv_desc.merge_kernel_flag;
    mem_params[72]=(ap_uint<128>) conv_desc.scale_oback_int;
    mem_params[73]=(ap_uint<128>) conv_desc.merge_weight_row_step;
    mem_params[74]=(ap_uint<128>) conv_desc.merge_weight_col_step;
    mem_params[75]=(ap_uint<128>) conv_desc.relu_flag;
}



#include "wino_systolic_kernel.cpp"



void wino_flatten_kernel(
    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
    ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
    ap_uint<16> start_output_row,
    ap_int<16> start_row_idx_minus_pad_size,
    ap_int<16> start_col_idx_minus_pad_size,
    ap_uint<32> reset_DDR_offset,
    bool first_flag,
    bool last_flag,
    bool clear_flag,
    ConvDesc_t conv_desc,
    ap_uint<1> ap_clk_div2
)
{
	printf("****wino_systolic_kernel****\n");
    #pragma HLS interface ap_stable port=conv_desc
    #pragma HLS array_partition variable =input_buffer dim=1 complete
	#pragma HLS array_partition variable =input_buffer dim=2 complete
    #pragma HLS array_partition variable =out_buffer dim=4 complete
    #pragma HLS array_partition variable =out_buffer dim=3 complete
    #pragma HLS array_partition variable =out_buffer dim=2 complete
    #pragma HLS array_partition variable =out_buffer dim=1 complete

    #pragma HLS dataflow
    hls::stream< ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_stream[WINO_WIDTH];
    #pragma HLS stream variable=input_tile_stream depth=2
    hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_transformed_stream[WINO_WIDTH];
    #pragma HLS stream variable=input_tile_transformed_stream depth=2
    hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream[WEIGHT_PORT_NUM][WEIGHT_FEED_NUMBER_PER_PORT];
    #pragma HLS stream variable=weight_stream depth=2






    input_feed_underconstruction(
        input_buffer,
        input_tile_stream,
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream1, 
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream2,
        conv_desc.inwidth,
        conv_desc.pad_size,
        conv_desc.weightbuffer_load_outdepth_number,
        conv_desc.wino_output_tile_size,
        conv_desc.input_buffer_feeding_loop_bound,
        conv_desc.loop_wino_tile_row_reset_cycle,
        conv_desc.loop_wino_tile_col_reset_cycle,
        conv_desc.buffer_address_mid_minitile_depth_step,
        conv_desc.wino_out_size_by_wino_width,
        conv_desc.row_address_bitnumber_flag,
        start_row_idx_minus_pad_size,
        start_col_idx_minus_pad_size
    );

    for(int i=0;i<WINO_WIDTH;i++)
    {
        #pragma HLS unroll
        input_transform(
            input_tile_stream[i],
            input_tile_transformed_stream[i],
            conv_desc.input_transform_feeding_loop_bound,
            conv_desc.wino3x3_flag,
            i
        );
    }

    weight_feed_one_port<0>(
        weight_DDR0,
        #if WINO_HEIGT==8
        weight_stream[0],
        #else
        weight_stream[0][0],
        #endif
        conv_desc.weightDDR_burst_number,
        conv_desc.weightDDR_buffer_burst_length,
        conv_desc.weightDDR_port_burst_length,
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,
        conv_desc.loop_start_output_baserowcol_reset_cycle,
        conv_desc.loop_weight_feed_bound,
        conv_desc.weightbuffer_outdepth_minitile_number,
        reset_DDR_offset,
        conv_desc.kernel_size,
        first_flag,
        last_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
    );
    #if WINO_HEIGHT  >=4 
    weight_feed_one_port<1>(
        weight_DDR1,
        #if WINO_HEIGT==8
        weight_stream[1],
        #else
        weight_stream[1][0],
        #endif
        conv_desc.weightDDR_burst_number,
        conv_desc.weightDDR_buffer_burst_length,
        conv_desc.weightDDR_port_burst_length,
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,
        conv_desc.loop_start_output_baserowcol_reset_cycle,
        conv_desc.loop_weight_feed_bound,
        conv_desc.weightbuffer_outdepth_minitile_number,
        reset_DDR_offset,
        conv_desc.kernel_size,
        first_flag,
        last_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
    );
    #endif

    #if WINO_HEIGHT >=4
    weight_feed_one_port<2>(
        weight_DDR2,
        #if WINO_HEIGT==8
        weight_stream[2],
        #else
        weight_stream[2][0],
        #endif
        conv_desc.weightDDR_burst_number,
        conv_desc.weightDDR_buffer_burst_length,
        conv_desc.weightDDR_port_burst_length,
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,
        conv_desc.loop_start_output_baserowcol_reset_cycle,
        conv_desc.loop_weight_feed_bound,
        conv_desc.weightbuffer_outdepth_minitile_number,
        reset_DDR_offset,
        conv_desc.kernel_size,
        first_flag,
        last_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
    );
    
    weight_feed_one_port<3>(
        weight_DDR3,
        #if WINO_HEIGT==8
        weight_stream[3],
        #else
        weight_stream[3][0],
        #endif
        conv_desc.weightDDR_burst_number,
        conv_desc.weightDDR_buffer_burst_length,
        conv_desc.weightDDR_port_burst_length,
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,
        conv_desc.loop_start_output_baserowcol_reset_cycle,
        conv_desc.loop_weight_feed_bound,
        conv_desc.weightbuffer_outdepth_minitile_number,
        reset_DDR_offset,
        conv_desc.kernel_size,
        first_flag,
        last_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
    );
    #endif

    wino_stream_block2(
        input_tile_transformed_stream,
        weight_stream,
        out_buffer,
		conv_desc.weightbuffer_outdepth_minitile_number,
		conv_desc.total_input_stream_tile,
		conv_desc.loop_omini_base_reset_cycle,
		conv_desc.loop_wino_tile_rowcol_self_reset_cycle_min1,
		conv_desc.loop_iload_reset_cycle,
		conv_desc.loop_wino_cell_bound,
		conv_desc.outbuffer_oload_increment_step,
		conv_desc.outbuffer_omini_increment_step,
		conv_desc.wino3x3_flag,
        clear_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
        ,ap_clk_div2
    );
}




void wino_kernel_merge_row(
    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
    ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
    ap_uint<16> start_output_row,
    ap_int<16> start_row_idx_minus_pad_size,
    bool first_flag,
    bool last_flag,
    ap_uint<8> row_offset,
    bool last_row_flag,
    ap_uint<32> merge_weight_row_offset,
    bool clear_flag,
    ConvDesc_t conv_desc,
    ap_uint<1> ap_clk_div2
)
{
    int merge_weight_offset=merge_weight_row_offset+conv_desc.merge_weight_col_step;
    for(ap_uint<4> col_offset=0;col_offset<conv_desc.merge_kernel_size;col_offset+=3)
    {
        ap_uint<32> reset_merge_weight_offset;
        if(last_row_flag && col_offset==conv_desc.merge_kernel_size-3)
        {
            reset_merge_weight_offset=0;
        }
        else
        {
            reset_merge_weight_offset=merge_weight_offset;
        }

        // wino_flatten_kernel(
        wino_systolic_kernel_wrapper(
        WEIGHT_PORTS_CALL(weight_DDR),
        input_buffer,
        out_buffer,
        start_output_row,
        start_row_idx_minus_pad_size,
        col_offset-conv_desc.pad_size,
        reset_merge_weight_offset,
        first_flag && (col_offset==0),
        last_flag && (col_offset+3>conv_desc.merge_kernel_size),
        clear_flag && ( col_offset==0),
        conv_desc,
        ap_clk_div2);
        merge_weight_offset+=conv_desc.merge_weight_col_step;
    }

}

void wino_input_compute(
    INPUT_PORTS_DECLARE(DDR_port),
    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
    ap_uint<16> start_output_row,
    ap_uint<16> next_start_row,
    ap_int<16> start_row_idx_minus_pad_size,
    ap_uint<1> first_input_flag,
    bool first_flag,
    ap_uint<1> last_flag,
    ConvDesc_t conv_desc,
    ap_uint<1> ap_clk_div2
)
{
    std::cout<<"$$$$$$$$$$$$$ wino_input_compute $$$$$$$$$$$$$"<<std::endl;
    static ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH];
    #pragma HLS array_partition variable=input_buffer complete dim=1 
    #pragma HLS array_partition variable=input_buffer complete dim=2 
    #pragma HLS resource variable=input_buffer core=RAM_S2P_BRAM 
    
    
    ap_int<16> next_required_loaded_input_row_number = (next_start_row+conv_desc.out_rowstep)- conv_desc.pad_size + conv_desc.merge_kernel_step-1; //TODO
    ap_int<16> new_start_input_row = next_start_row-conv_desc.pad_size-1;
    // if(new_start_input_row<0) new_start_input_row=0;
    ap_int<16> curr_required_loaded_input_row_number = start_output_row+conv_desc.out_rowstep - conv_desc.pad_size + conv_desc.merge_kernel_step-1; //TODO
    std::cout<<"conv_desc.out_rowstep"  <<conv_desc.out_rowstep<<std::endl;
    std::cout<<"conv_desc.pad_size "  <<conv_desc.pad_size<<std::endl;
    std::cout<<"curr_required_loaded_input_row_number "  <<curr_required_loaded_input_row_number<<std::endl;    

        load_input_rowtile_from_ddr(
            INPUT_PORTS_CALL(DDR_port),
            input_buffer,
            conv_desc.inheight,
            conv_desc.inwidth,
            conv_desc.stride,
            conv_desc.pad_size,
            conv_desc.inwidth_align8,
            conv_desc.indepth_align8,
            conv_desc.group_indepth_x_inwidth_align8_by8,
            conv_desc.group_indepth_offset_x_inwidth_align8_by8,
            conv_desc.inwidth_ceildiv_inbufferwidth,
            conv_desc.buffer_address_mid_increment_step,
            conv_desc.input_load_burst_length,
            conv_desc.row_address_bitnumber_flag,
            conv_desc.out_rowstep,
            0,
            next_required_loaded_input_row_number,
            1,
            ~first_input_flag);



int idx=0;
    #if DEBUG_FILE_PRINT
        
        attach_input_buffer_content_uniformed<INBUFFER_HEIGHT,INBUFFER_WIDTH, INPUT_BUFFER_DEPTH>(input_buffer,idx,(char*) "input_buffer_content.txt");
        idx++;
        getchar();
    #endif

        printf("first_input_flag \n", (int)first_input_flag );
        if(first_input_flag) return;

        printf("OK %d \n",(int) conv_desc.merge_kernel_size);
       

        ap_uint<1> merge_clear_flag= (conv_desc.merge_kernel_size>3);
        ap_uint<32> merge_weight_row_offset=0;
        bool last_row_flag;
        for(ap_uint<8> row_offset=0; row_offset<conv_desc.merge_kernel_size; row_offset+=3)
        {
            #pragma HLS DEPENDENCE variable=input_buffer intra false
            ap_int<16> required_loaded_input_row_number;
            ap_uint<1> skip_flag;
            ap_uint<1> clear_flag;

            if( row_offset+3<conv_desc.merge_kernel_size )
            {
                required_loaded_input_row_number=curr_required_loaded_input_row_number+3+row_offset;
                skip_flag=0;
                clear_flag=0;
                last_row_flag=0;
            }
            else
            {
                required_loaded_input_row_number=next_required_loaded_input_row_number;
                skip_flag=last_flag;
                clear_flag=merge_clear_flag;  
                last_row_flag=1;
            }
            std::cout<<"conv_desc.merge_kernel_size "<<conv_desc.merge_kernel_size<<std::endl;
            std::cout<<"merge row_offset "<<row_offset<<std::endl;
  
            load_input_rowtile_from_ddr(
                    INPUT_PORTS_CALL(DDR_port),
                    input_buffer,
                    conv_desc.inheight,
                    conv_desc.inwidth,
                    conv_desc.stride,
                    conv_desc.pad_size,
                    conv_desc.inwidth_align8,
                    conv_desc.indepth_align8,
                    conv_desc.group_indepth_x_inwidth_align8_by8,
                    conv_desc.group_indepth_offset_x_inwidth_align8_by8,
                    conv_desc.inwidth_ceildiv_inbufferwidth,
                    conv_desc.buffer_address_mid_increment_step,
                    conv_desc.input_load_burst_length,
                    conv_desc.row_address_bitnumber_flag,
                    conv_desc.out_rowstep,
                    new_start_input_row,
                    required_loaded_input_row_number,
                    clear_flag,
                    skip_flag);
    #if DEBUG_FILE_PRINT
        attach_input_buffer_content_uniformed<INBUFFER_HEIGHT,INBUFFER_WIDTH, INPUT_BUFFER_DEPTH>(input_buffer,idx,(char*) "input_buffer_content.txt");
        idx++;
        getchar();
    #endif  

            wino_kernel_merge_row(
                WEIGHT_PORTS_CALL(weight_DDR),
                input_buffer,
                out_buffer,
                start_output_row,
                start_row_idx_minus_pad_size,
                first_flag && (row_offset==0),
                last_flag,
                row_offset,
                last_row_flag,
                merge_weight_row_offset,
                (row_offset==0),
                conv_desc,
                ap_clk_div2);
            merge_weight_row_offset+=conv_desc.merge_weight_row_step;
            start_row_idx_minus_pad_size+=3;
            
        }



}




// #pragma SDS data zero_copy(input_DDR0[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(input_DDR1[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(input_DDR2[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(input_DDR3[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(weight_DDR0[0:WEIGHT_PORT_DEPTH])
// #pragma SDS data zero_copy(weight_DDR1[0:WEIGHT_PORT_DEPTH])
// #pragma SDS data zero_copy(weight_DDR2[0:WEIGHT_PORT_DEPTH])
// #pragma SDS data zero_copy(weight_DDR3[0:WEIGHT_PORT_DEPTH])
// #pragma SDS data zero_copy(output_DDR0[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(output_DDR1[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(output_DDR2[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(output_DDR3[0:FEATURE_PORT_DEPTH])
// #pragma SDS data zero_copy(mem_params[0:128])
// #pragma SDS data zero_copy(bias_mem[0:128])

// #pragma SDS data sys_port(input_DDR0:ps_e_S_AXI_HP0_FPD)
// #pragma SDS data sys_port(input_DDR1:ps_e_S_AXI_HP1_FPD)
// #pragma SDS data sys_port(input_DDR2:ps_e_S_AXI_HP2_FPD)
// #pragma SDS data sys_port(input_DDR3:ps_e_S_AXI_HP3_FPD)
// #pragma SDS data sys_port(weight_DDR0:ps_e_S_AXI_HP0_FPD)
// #pragma SDS data sys_port(weight_DDR1:ps_e_S_AXI_HP1_FPD)
// #pragma SDS data sys_port(weight_DDR2:ps_e_S_AXI_HP2_FPD)
// #pragma SDS data sys_port(weight_DDR3:ps_e_S_AXI_HP3_FPD)
// #pragma SDS data sys_port(output_DDR0:ps_e_S_AXI_HP0_FPD)
// #pragma SDS data sys_port(output_DDR1:ps_e_S_AXI_HP1_FPD)
// #pragma SDS data sys_port(output_DDR2:ps_e_S_AXI_HP2_FPD)
// #pragma SDS data sys_port(output_DDR3:ps_e_S_AXI_HP3_FPD)
// #pragma SDS data sys_port(mem_params:ps_e_S_AXI_HP0_FPD)
// #pragma SDS data sys_port(bias_mem:ps_e_S_AXI_HP1_FPD)




void wino_systolic_top(
    INPUT_PORTS_DECLARE(input_DDR),
    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR0,
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR1,
    // ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR2,
    // ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR3,
    // ConvDesc_t conv_desc,
    ap_int<32> *mem_params
    // ap_int<32> *bias_mem
    // #ifdef __SDSVHLS__
    // ,ap_uint<1> ap_clk_div2
    // #endif
    )
{

    // #ifndef __SDSVHLS__
    ap_uint<1> ap_clk_div2=0;
    // #endif

    // #pragma HLS interface m_axi port= input_DDR3 offset=slave   bundle=input_DDR3 depth=65535
    // #pragma HLS interface m_axi port= input_DDR2 offset=slave   bundle=input_DDR2 depth=65535
    #pragma HLS interface m_axi port= input_DDR1 offset=slave   bundle=input_DDR1 depth=524288
    #pragma HLS interface m_axi port= input_DDR0 offset=slave   bundle=input_DDR0 depth=524288
    #pragma HLS interface m_axi port= weight_DDR0 offset=slave  bundle=weight_DDR0 depth=1179648
    #pragma HLS interface m_axi port= weight_DDR1 offset=slave  bundle=weight_DDR1 depth=1179648
    #pragma HLS interface m_axi port= weight_DDR2 offset=slave  bundle=weight_DDR2 depth=1179648
    #pragma HLS interface m_axi port= weight_DDR3 offset=slave  bundle=weight_DDR3 depth=1179648
    // #pragma HLS interface m_axi port= output_DDR3 offset=slave  bundle=output_DDR3 depth=65535
    // #pragma HLS interface m_axi port= output_DDR2 offset=slave  bundle=output_DDR2 depth=65535
    #pragma HLS interface m_axi port= output_DDR1 offset=slave  bundle=output_DDR1 depth=524288
    #pragma HLS interface m_axi port= output_DDR0 offset=slave  bundle=output_DDR0 depth=524288
    // #pragma HLS interface m_axi port= weight_DDR3 offset=slave depth=65535
    // #pragma HLS interface m_axi port= weight_DDR2 offset=slave depth=65535
    // #pragma HLS interface m_axi port= weight_DDR1 offset=slave depth=65535

    #pragma HLS interface m_axi port= mem_params offset=slave  bundle=mem_params depth=65535
    // #pragma HLS interface m_axi port= bias_mem offset=slave  bundle=bias_mem depth=65535
	#pragma HLS INTERFACE s_axilite register port=return

    //input buffer declaration
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH];
    #pragma HLS array_partition variable=input_buffer complete dim=1 
    #pragma HLS array_partition variable=input_buffer complete dim=2 
    #pragma HLS resource variable=input_buffer core=RAM_S2P_BRAM 
    ap_uint<OUT_WIDTH*2> output_buffer0[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH];
    #pragma HLS array_partition variable=output_buffer0 complete dim=1 
    #pragma HLS array_partition variable=output_buffer0 complete dim=2
    #pragma HLS resource variable=output_buffer0 core=RAM_T2P_BRAM  
    ap_uint<OUT_WIDTH*2> output_buffer1[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH];
    #pragma HLS array_partition variable=output_buffer1 complete dim=1 
    #pragma HLS array_partition variable=output_buffer1 complete dim=2 
    #pragma HLS resource variable=output_buffer1 core=RAM_T2P_BRAM 
    ConvDesc_t conv_desc;

	ap_int<16> bias_buffer0[8][BIAS_BUFFER_DEPTH];
    #pragma HLS array_partition variable=bias_buffer0 complete dim=1 

	ap_int<16> bias_buffer1[8][BIAS_BUFFER_DEPTH];
    #pragma HLS array_partition variable=bias_buffer1 complete dim=1 
    
    ap_uint<1> pingpong=0;

    #if DEBUG_FILE_PRINT
        // clear_buffer_content<INBUFFER_HEIGHT,INBUFFER_WIDTH, INPUT_BUFFER_DEPTH>(input_buffer);
        clear_output_buffer_content<OUT_WIDTH,BATCH_SIZE,WINO_HEIGHT,WINO_WIDTH,WINO_OUT_SIZE_CELL,OUTPUT_BUFFER_DEPTH>(output_buffer0);
        clear_output_buffer_content<OUT_WIDTH,BATCH_SIZE,WINO_HEIGHT,WINO_WIDTH,WINO_OUT_SIZE_CELL,OUTPUT_BUFFER_DEPTH>(output_buffer1);
    #endif



    load_params(mem_params,conv_desc);
    #if WEIGHT_PORT_NUM==1
    load_bias_value(weight_DDR0, bias_buffer0, bias_buffer1,conv_desc.outdepth_align8);
    #elif WEIGHT_PORT_NUM==2
    load_bias_value(weight_DDR1, bias_buffer0, bias_buffer1,conv_desc.outdepth_align8);
    #else
    load_bias_value(weight_DDR3, bias_buffer0, bias_buffer1,conv_desc.outdepth_align8);
    #endif
    
    wino_input_compute(
        INPUT_PORTS_CALL(input_DDR),
        WEIGHT_PORTS_CALL(weight_DDR),
        output_buffer1,
        0, //ap_uint<16> start_output_row,
        0, //ap_uint<16> next_start_row,
        0, //ap_int<16> start_row_idx_minus_pad_size,
        1, //ap_uint<1> first_input_flag,
        0, //ap_uint<1> first_flag,
        0, //ap_uint<1> last_flag,
        conv_desc,
        ap_clk_div2
    );

   


  
    // write_params(weight_DDR0, conv_desc);
    
    #if DEBUG_FILE_PRINT
        // attach_input_buffer_content_uniformed<INBUFFER_HEIGHT,INBUFFER_WIDTH, INPUT_BUFFER_DEPTH>(input_buffer,0,(char*) "input_buffer_content.txt");
    #endif



    
    ap_int<16> write_start_row= -conv_desc.out_rowstep;
    ap_int<16> next_start_row= conv_desc.out_rowstep;





    for( ap_int<16> compute_start_row =0; compute_start_row < conv_desc.outheight; compute_start_row+=conv_desc.out_rowstep)
    {

        ap_uint<16> start_row_idx_minus_pad_size=compute_start_row-conv_desc.pad_size;
       
        if(pingpong )
        {

            // #if WINO_HEIGHT==2

            write_output_to_DDR3(
            output_DDR0,
            output_DDR1,
            // output_DDR2,
            // output_DDR3,
            output_buffer1,
            write_start_row,
            write_start_row==0,
            conv_desc
            );
            // #else

            // write_output_to_DDR2(
            // output_DDR0,
            // output_DDR1,
            // output_DDR2,
            // output_DDR3,
            // output_buffer1,
            // write_start_row,
            // write_start_row==0,
            // bias_buffer0,
            // bias_buffer1,
            // conv_desc
            // );

            // #endif

            wino_input_compute(
                INPUT_PORTS_CALL(input_DDR),
                WEIGHT_PORTS_CALL(weight_DDR),
                output_buffer0,
                compute_start_row,
                next_start_row,
                start_row_idx_minus_pad_size,
                0,
                compute_start_row==0,
                next_start_row > conv_desc.outheight,
                conv_desc,
                ap_clk_div2
            );

        // //     #if DEBUG_FILE_PRINT
        // //     char outfilename[100];
        // //     sprintf(outfilename,"outbuffer.txt");
        // //     attach_output_buffer_content_uniformed_hw<OUT_WIDTH,BATCH_SIZE,WINO_HEIGHT,WINO_WIDTH,WINO_OUT_SIZE_CELL,OUTPUT_BUFFER_DEPTH>(
        // //         output_buffer0,0,outfilename);
        // //     getchar();
        // //     #endif



        }
        else
        {
            // #if WINO_HEIGHT==2
            write_output_to_DDR3(
            output_DDR0,
            output_DDR1,
            // output_DDR2,
            // output_DDR3,
            output_buffer0,
            write_start_row,
            write_start_row==0,
            conv_desc
            );
            // #else

            // write_output_to_DDR2(
            // output_DDR0,
            // output_DDR1,
            // output_DDR2,
            // output_DDR3,
            // output_buffer0,
            // write_start_row,
            // write_start_row==0,
            // bias_buffer0,
            // bias_buffer1,
            // conv_desc
            // );

            // #endif


            wino_input_compute(
                INPUT_PORTS_CALL(input_DDR),
                WEIGHT_PORTS_CALL(weight_DDR),
                output_buffer1,
                compute_start_row,
                next_start_row,
                start_row_idx_minus_pad_size,
                0,
                compute_start_row==0,
                next_start_row > conv_desc.outheight,
                conv_desc,
                ap_clk_div2);

        // //     #if DEBUG_FILE_PRINT
        // //     char outfilename[100];
        // //     sprintf(outfilename,"outbuffer.txt");
        // //     attach_output_buffer_content_uniformed_hw<OUT_WIDTH,BATCH_SIZE,WINO_HEIGHT,WINO_WIDTH,WINO_OUT_SIZE_CELL,OUTPUT_BUFFER_DEPTH>(
        // //         output_buffer1,0,outfilename);
        // //     getchar();
        // //     #endif




        }


        pingpong =~pingpong;
        write_start_row+=conv_desc.out_rowstep;
        next_start_row+=conv_desc.out_rowstep;
        // #if DEBUG_FILE_PRINT
        //     // attach_input_buffer_content_uniformed<INBUFFER_HEIGHT,INBUFFER_WIDTH, INPUT_BUFFER_DEPTH>(input_buffer,0,(char*) "input_buffer_content.txt");
        // #endif
 
        
    }

    if(pingpong )
    {
        // #if WINO_HEIGHT==2
        write_output_to_DDR3(
        output_DDR0,
        output_DDR1,
        // output_DDR2,
        // output_DDR3,
        output_buffer1,
        write_start_row,
        write_start_row==0,
        conv_desc
        );
        // #else
        // write_output_to_DDR2(
        // output_DDR0,
        // output_DDR1,
        // output_DDR2,
        // output_DDR3,
        // output_buffer1,
        // write_start_row,
        // write_start_row==0,
        // bias_buffer0,
        // bias_buffer1,
        // conv_desc
        // );
        // #endif

    }
    else
    {
        // #if WINO_HEIGHT==2
        write_output_to_DDR3(
        output_DDR0,
        output_DDR1,
        // output_DDR2,
        // output_DDR3,
        output_buffer0,
        write_start_row,
        write_start_row==0,
        conv_desc
        );
        // #else
        // write_output_to_DDR2(
        // output_DDR0,
        // output_DDR1,
        // output_DDR2,
        // output_DDR3,
        // output_buffer0,
        // write_start_row,
        // write_start_row==0,
        // bias_buffer0,
        // bias_buffer1,
        // conv_desc
        // );
        // #endif

    }
}
