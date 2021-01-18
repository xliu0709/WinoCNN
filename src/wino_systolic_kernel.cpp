#include <ap_int.h>
#include "wino_macro.h"
void wino_systolic_kernel(    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
	ap_uint<OUT_WIDTH*2> out_buffer0_0_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUT_WIDTH*2> out_buffer1_0_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUT_WIDTH*2> out_buffer2_0_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
	ap_uint<OUT_WIDTH*2> out_buffer3_0_0[WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
	ap_uint<16> start_output_row,
    ap_int<16> start_row_idx_minus_pad_size,
    ap_int<16> start_col_idx_minus_pad_size,
    ap_uint<32> reset_DDR_offset,
    bool first_flag,
    bool last_flag,
    bool clear_flag,
    ConvDesc_t conv_desc,
    ap_uint<1> ap_clk_div2
    ){
	#pragma HLS array_partition variable=out_buffer0_0_0 dim=1 complete
	#pragma HLS array_partition variable=out_buffer0_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer0_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer1_0_0 dim=1 complete
	#pragma HLS array_partition variable=out_buffer1_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer1_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer2_0_0 dim=1 complete
	#pragma HLS array_partition variable=out_buffer2_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer2_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer3_0_0 dim=1 complete
	#pragma HLS array_partition variable=out_buffer3_0_0 dim=2 complete
	#pragma HLS array_partition variable=out_buffer3_0_0 dim=2 complete

	#pragma HLS dataflow
    #pragma HLS interface ap_stable port=conv_desc
    #pragma HLS array_partition variable =input_buffer dim=1 complete
	#pragma HLS array_partition variable =input_buffer dim=2 complete
	static hls::stream< ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_stream[WINO_WIDTH];
    #pragma HLS stream variable=input_tile_stream depth=1
	static hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_transformed_stream[WINO_HEIGHT/WINO_H2][WINO_WIDTH/WINO_W2][WINO_W2];
    #pragma HLS stream variable=input_tile_transformed_stream depth=2
    #pragma HLS resource variable=input_tile_transformed_stream core=FIFO_SRL
    static hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream[WINO_HEIGHT/WINO_H2][WINO_WIDTH/WINO_W2-1][WINO_H2];
    #pragma HLS stream variable=weight_stream depth=2
    #pragma HLS resource variable=weight_stream core=FIFO_SRL
	static hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream_out[WEIGHT_PORT_NUM][WEIGHT_FEED_NUMBER_PER_PORT];
    #pragma HLS stream variable=weight_stream_out depth=2
    #pragma HLS resource variable=weight_stream_out core=FIFO_SRL

	input_feed_underconstruction(
        input_buffer,
        input_tile_stream,
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream1, 
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream2,
        conv_desc.inwidth,
        // conv_desc.pad_size,
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
            input_tile_transformed_stream[0][i/WINO_W2][i%WINO_W2],
            conv_desc.input_transform_feeding_loop_bound,
            conv_desc.wino3x3_flag,
            i
        );
    }

	weight_feed_one_port<0>(
        weight_DDR0,
        weight_stream_out[0],
        conv_desc.weightDDR_burst_number,
        conv_desc.weightDDR_buffer_burst_length,
        conv_desc.weightDDR_port_burst_length,
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,
        conv_desc.loop_start_output_baserowcol_reset_cycle,
        conv_desc.loop_weight_feed_bound,
        conv_desc.weightbuffer_outdepth_minitile_number,
        reset_DDR_offset,
        conv_desc.wino3x3_flag,
        first_flag,
        last_flag
        #if DEBUG_CONV_DESC
        ,conv_desc	
        #endif
    );

	winoPEB_CORN(
		input_tile_transformed_stream[0][0],
		weight_stream_out[0],
		out_buffer0_0_0,
		out_buffer1_0_0,
		out_buffer2_0_0,
		out_buffer3_0_0,
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
        ,ap_clk_div2);

}
void wino_systolic_kernel_wrapper(
    WEIGHT_PORTS_DECLARE(weight_DDR),
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],
    ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/WINO_H2][WINO_WIDTH/WINO_W2][WINO_H2][WINO_W2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],
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
	#pragma HLS array_partition variable=out_buffer dim=1 complete
	#pragma HLS array_partition variable=out_buffer dim=2 complete
	#pragma HLS array_partition variable=out_buffer dim=3 complete
	#pragma HLS array_partition variable=out_buffer dim=4 complete
	wino_systolic_kernel(
        WEIGHT_PORTS_CALL(weight_DDR),
        input_buffer,
		out_buffer[0][0][0],
		out_buffer[1][0][0],
		out_buffer[2][0][0],
		out_buffer[3][0][0],
		start_output_row,
        start_row_idx_minus_pad_size,
        start_col_idx_minus_pad_size,
        reset_DDR_offset,
        first_flag,
        last_flag,
        clear_flag,
        conv_desc,
        ap_clk_div2
        );
}
