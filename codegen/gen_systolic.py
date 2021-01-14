   
   




   
class Config_t():
    def __init__(self, filname):
        file=open(filname,"r");

        Lines = file.readlines()
        for line in Lines: 
            a=line.split(" ")
            if(a[0]=="#define" and len(a)==3):
                self.__dict__[a[1]]=int(a[2])

        self.WINO_OUT_SIZE_CELL=self.WINO_DOMAIN_SIZE
        self.INPUT_BUFFER_DEPTH=1<<self.INPUT_BUFFER_DEPTH_BITWIDTH
        
        self.OUTPUT_BUFFER_DEPTH=(1<<self.OUTPUT_BUFFER_DEPTH_BITWIDTH)
        self.WEIGHT_BUFFER_DEPTH=(1<<self.WEIGHT_BUFFER_DEPTH_BITWIDTH)



        self.INDEPTH_MINITILE_SIZE=(1<<self.INDEPTH_MINITILE_SIZE_BITWIDTH)

        self.OUTDEPTH_MINITILE_SIZE=self.WINO_HEIGHT

        if(self.WINO_HEIGHT >4):
            self.WEIGHT_PORT_NUM=4
            self.WEIGHT_FEED_NUMBER_PER_PORT=self.WINO_HEIGHT/4
        else:
            self.WEIGHT_PORT_NUM=self.WINO_HEIGHT/2
            self.WEIGHT_FEED_NUMBER_PER_PORT=self.WINO_HEIGHT
        
    
        if (self.WINO_HEIGHT==2):
            self.OUT_PORT_BATCH_NUM= 4
        else:
            self.OUT_PORT_BATCH_NUM= 8 


def generate_wino_systolic_kernel_wrapper( config: Config_t):
    ret_string=""
    ret_string+="void wino_systolic_kernel_wrapper(\n\
    WEIGHT_PORTS_DECLARE(weight_DDR),\n\
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],\n\
    ap_uint<OUT_WIDTH*2> out_buffer[WINO_OUT_SIZE_CELL][OUTDEPTH_MINITILE_SIZE/2][WINO_WIDTH/2][2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],\n\
    ap_uint<16> start_output_row,\n\
    ap_int<16> start_row_idx_minus_pad_size,\n\
    ap_int<16> start_col_idx_minus_pad_size,\n\
    ap_uint<32> reset_DDR_offset,\n\
    bool first_flag,\n\
    bool last_flag,\n\
    bool clear_flag,\n\
    ConvDesc_t conv_desc,\n\
    ap_uint<1> ap_clk_div2\n\
    )\n{\n"

    ret_string+="\t#pragma HLS array_partition variable=out_buffer dim=1 complete\n"
    ret_string+="\t#pragma HLS array_partition variable=out_buffer dim=2 complete\n"
    ret_string+="\t#pragma HLS array_partition variable=out_buffer dim=3 complete\n"
    ret_string+="\t#pragma HLS array_partition variable=out_buffer dim=4 complete\n"
    ret_string+="\twino_systolic_kernel(\n\
        WEIGHT_PORTS_CALL(weight_DDR),\n\
        input_buffer,\n"

    for i in range(config.WINO_OUT_SIZE_CELL):
        for j in range(config.OUTDEPTH_MINITILE_SIZE//2):
            for k in range(config.WINO_WIDTH//2):
                ret_string+="\t\tout_buffer[{}][{}][{}],\n".format(i,j,k)
    ret_string+="\t\tstart_output_row,\n\
        start_row_idx_minus_pad_size,\n\
        start_col_idx_minus_pad_size,\n\
        reset_DDR_offset,\n\
        first_flag,\n\
        last_flag,\n\
        clear_flag,\n\
        conv_desc,\n\
        ap_clk_div2\n\
        );\n"
    ret_string+="}\n"
    print(ret_string)
    return ret_string





def generate_wino_systolic(config:Config_t):
    ret_string=""
    ret_string+="void wino_systolic_kernel(\
    WEIGHT_PORTS_DECLARE(weight_DDR),\n\
    ap_uint<16> input_buffer[INBUFFER_HEIGHT][INBUFFER_WIDTH][INPUT_BUFFER_DEPTH],\n"
    for i in range(config.WINO_OUT_SIZE_CELL):
        for j in range(config.OUTDEPTH_MINITILE_SIZE//2):
            for k in range(config.WINO_WIDTH//2):
                ret_string+="\tap_uint<OUT_WIDTH*2> out_buffer{}_{}_{}[2][2][WINO_OUT_SIZE_CELL][OUTPUT_BUFFER_DEPTH],\n".format(i,j,k)
    ret_string+="\tap_uint<16> start_output_row,\n\
    ap_int<16> start_row_idx_minus_pad_size,\n\
    ap_int<16> start_col_idx_minus_pad_size,\n\
    ap_uint<32> reset_DDR_offset,\n\
    bool first_flag,\n\
    bool last_flag,\n\
    bool clear_flag,\n\
    ConvDesc_t conv_desc,\n\
    ap_uint<1> ap_clk_div2\n\
    ){\n"

    for i in range(config.WINO_OUT_SIZE_CELL):
        for j in range(config.OUTDEPTH_MINITILE_SIZE//2):
            for k in range(config.WINO_WIDTH//2):
                ret_string+="\t#pragma HLS array_partition variable=out_buffer{}_{}_{} dim=1 complete\n".format(i,j,k)
                ret_string+="\t#pragma HLS array_partition variable=out_buffer{}_{}_{} dim=2 complete\n".format(i,j,k)
                ret_string+="\t#pragma HLS array_partition variable=out_buffer{}_{}_{} dim=2 complete\n".format(i,j,k)

    ret_string+="\n\t#pragma HLS dataflow\n\
    #pragma HLS interface ap_stable port=conv_desc\n\
    #pragma HLS array_partition variable =input_buffer dim=1 complete\n\
	#pragma HLS array_partition variable =input_buffer dim=2 complete\n\
    static hls::stream< ap_uint<8*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_stream[WINO_WIDTH];\n\
    #pragma HLS stream variable=input_tile_stream depth=16\n\
    static hls::stream< ap_uint<BTB_WIDTH*BATCH_SIZE*WINO_DOMAIN_SIZE_SQUARE> > input_tile_transformed_stream[WINO_HEIGHT/2][WINO_WIDTH/2][2];\n\
    #pragma HLS stream variable=input_tile_transformed_stream depth=2\n\
    static hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream[WINO_HEIGHT/2][WINO_WIDTH/2-1][2];\n\
    #pragma HLS stream variable=weight_stream depth=2\n"

    if(config.WINO_HEIGHT==8):
        ret_string+="\tstatic hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream_out[WEIGHT_PORT_NUM][WEIGHT_FEED_NUMBER_PER_PORT];\n\
    #pragma HLS stream variable=weight_stream_out depth=16\n\n"
    else:
        ret_string+="\tstatic hls::stream<ap_uint<W_WIDTH*INDEPTH_MINITILE_SIZE*WINO_DOMAIN_SIZE_SQUARE> >  weight_stream_out[WEIGHT_PORT_NUM/2][2];\n\
    #pragma HLS stream variable=weight_stream_out depth=16\n\n"   

    ret_string+="\tinput_feed_underconstruction(\n\
        input_buffer,\n\
        input_tile_stream,\n\
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream1, \n\
        // hls::stream< ap_uint<16*BATCH_SIZE*36> > &input_tile_stream2,\n\
        conv_desc.inwidth,\n\
        conv_desc.pad_size,\n\
        conv_desc.weightbuffer_load_outdepth_number,\n\
        conv_desc.wino_output_tile_size,\n\
        conv_desc.input_buffer_feeding_loop_bound,\n\
        conv_desc.loop_wino_tile_row_reset_cycle,\n\
        conv_desc.loop_wino_tile_col_reset_cycle,\n\
        conv_desc.buffer_address_mid_minitile_depth_step,\n\
        conv_desc.wino_out_size_by_wino_width,\n\
        conv_desc.row_address_bitnumber_flag,\n\
        start_row_idx_minus_pad_size,\n\
        start_col_idx_minus_pad_size\n\
    );\n\n"


    ret_string+="\tfor(int i=0;i<WINO_WIDTH;i++)\n\
    {\n\
        #pragma HLS unroll\n\
        input_transform(\n\
            input_tile_stream[i],\n\
            input_tile_transformed_stream[0][i/2][i%2],\n\
            conv_desc.input_transform_feeding_loop_bound,\n\
            conv_desc.wino3x3_flag,\n\
            i\n\
        );\n\
    }\n\n"


    if(config.WINO_HEIGHT==8):
        ret_string+="\tweight_feed_one_port<0>(\n\
        weight_DDR0,\n\
        weight_stream_out[0],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<1>(\n\
        weight_DDR1,\n\
        weight_stream_out[1],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<2>(\n\
        weight_DDR2,\n\
        weight_stream_out[2],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<3>(\n\
        weight_DDR3,\n\
        weight_stream_out[3],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\n"
    elif(config.WINO_HEIGHT==4):
        ret_string+="\tweight_feed_one_port<0>(\n\
        weight_DDR0,\n\
        weight_stream_out[0][0],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<1>(\n\
        weight_DDR1,\n\
        weight_stream_out[0][1],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<2>(\n\
        weight_DDR2,\n\
        weight_stream_out[1][0],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<3>(\n\
        weight_DDR3,\n\
        weight_stream_out[1][1],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\n"
    elif(config.WINO_HEIGHT==2):
        ret_string+="\tweight_feed_one_port<0>(\n\
        weight_DDR0,\n\
        weight_stream_out[0][0],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\
    weight_feed_one_port<1>(\n\
        weight_DDR1,\n\
        weight_stream_out[0][1],\n\
        conv_desc.weightDDR_burst_number,\n\
        conv_desc.weightDDR_buffer_burst_length,\n\
        conv_desc.weightDDR_port_burst_length,\n\
        conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1,\n\
        conv_desc.loop_start_output_baserowcol_reset_cycle,\n\
        conv_desc.loop_weight_feed_bound,\n\
        conv_desc.weightbuffer_outdepth_minitile_number,\n\
        reset_DDR_offset,\n\
        conv_desc.kernel_size,\n\
        first_flag,\n\
        last_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
    );\n\n"





            
    for h in range(config.WINO_HEIGHT//2):
        for w in range(config.WINO_WIDTH//2):
            ret_string+=generate_wino_cell_call(config,h,w)
    ret_string+="}\n"
    return ret_string


  

    












def generate_wino_cell_call(config:Config_t, wino_h, wino_w):
    

    inport_t="\t\tinput_tile_transformed_stream[{}][{}],\n".format(wino_h,wino_w)

    if(wino_h==config.WINO_HEIGHT//2-1):
        inport_b=""
    else:
        inport_b="\t\tinput_tile_transformed_stream[{}][{}],\n".format(wino_h+1,wino_w)


    if(wino_w==0):
        wport_l="\t\tweight_stream_out[{}],\n".format(wino_h)
    else:
        wport_l="\t\tweight_stream[{}][{}],\n".format(wino_h,wino_w-1)

    if(wino_w==config.WINO_WIDTH//2-1):
        wport_r=""
    else:
        wport_r="\t\tweight_stream[{}][{}],\n".format(wino_h,wino_w)
        
    if(wino_w== config.WINO_WIDTH//2-1 and wino_h== config.WINO_HEIGHT//2-1):
        call="\twinoPEB_CORN(\n"
    elif(wino_w== config.WINO_WIDTH//2-1 ):
        call="\twinoPEB_EDG(\n"
    elif(wino_h== config.WINO_HEIGHT//2-1):
        call="\twinoPEB_BOT(\n"
    else:
        call="\twinoPEB_CENT(\n"

    
    ret_string=""
    ret_string+=call
    ret_string+=inport_t
    ret_string+=inport_b
    ret_string+=wport_l
    ret_string+=wport_r
    ret_string+="\t\tout_buffer0_{}_{},\n".format(wino_h,wino_w)
    ret_string+="\t\tout_buffer1_{}_{},\n".format(wino_h,wino_w)
    ret_string+="\t\tout_buffer2_{}_{},\n".format(wino_h,wino_w)
    ret_string+="\t\tout_buffer3_{}_{},\n".format(wino_h,wino_w)
    ret_string+="\t\tconv_desc.weightbuffer_outdepth_minitile_number,\n\
        conv_desc.total_input_stream_tile,\n\
        conv_desc.loop_omini_base_reset_cycle,\n\
        conv_desc.loop_wino_tile_rowcol_self_reset_cycle_min1,\n\
        conv_desc.loop_iload_reset_cycle,\n\
        conv_desc.loop_wino_cell_bound,\n\
        conv_desc.outbuffer_oload_increment_step,\n\
        conv_desc.outbuffer_omini_increment_step,\n\
        conv_desc.wino3x3_flag,\n\
        clear_flag\n\
        #if DEBUG_CONV_DESC\n\
        ,conv_desc	\n\
        #endif\n\
        ,ap_clk_div2);\n\n"
    return ret_string
    

    




if __name__ == "__main__":
    
    config=Config_t("src/wino_hw_config.h")
    
    file=open("src/wino_systolic_kernel.cpp","w")
    file.write("#include <ap_int.h>\n")
    file.write("#include \"wino_macro.h\"\n")
    file.write(generate_wino_systolic(config))
    string=generate_wino_systolic_kernel_wrapper(config)

    file.write(string)
    file.close()
