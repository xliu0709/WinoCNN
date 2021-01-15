


import os
import sys
import math
import time
from pynq import Xlnk
from pynq import Overlay
import numpy as np






def CEIL_DIV(x,y):
    return (x+y-1)//y
def ALIGN(x,y):
    return (x+y-1)//y*y

def find_factor(number,factor_list):
    for i in range(1,number):
        if(number % i ==0):
            factor_list.append(i)

def log2ceil(x):
    return int( math.ceil(math.log2(x)) )

def find_max_factor_product(factor_list1, factor_list2, capacity):

    if( len(factor_list1)==0 or len(factor_list2)==0 ): return -1,-1

    if( factor_list2[0]*factor_list2[0]>capacity ): return -1,-1

    index1=0;
    index2=len(factor_list2)-1;

    product_candidate=0;


 

    while(1):
        if(index1 == len(factor_list1) ): break;
        product = factor_list1[index1]*factor_list2[index2];
        if(product > capacity):
            index2-=1;
            continue;

        if(product > product_candidate):
            product_candidate=product;
            factor1_candidate_index=index1;
            factor2_candidate_index=index2;

        index1+=1;        
    factor1 = factor_list1[factor1_candidate_index];
    factor2 = factor_list2[factor2_candidate_index];

    return factor1,factor2



class ConvDesc_t():

    def __init__(self, argv):

        

        self.input_width = int(argv[1])
        self.input_height = int(argv[2])
        self.input_depth = int(argv[3])
        self.output_width = int(argv[4])
        self.output_height = int(argv[5])
        self.output_depth = int(argv[6])
        self.kernel_size = int(argv[7])
        self.stride_size = int(argv[8])
        self.pad_size = int(argv[9])
        self.relu_flag = int(argv[10])
        self.scale_writeback = int(argv[11])
        self.hardware_conf_filename= argv[12]
    
        file=open(self.hardware_conf_filename,"r");

        Lines = file.readlines()
        for line in Lines: 
            a=line.split(" ")
            if(a[0]=="#define" and len(a)==3):
                self.__dict__[a[1].lower()]=int(a[2])


        self.wino_out_size_cell=self.wino_domain_size
        self.input_buffer_depth=1<<self.input_buffer_depth_bitwidth
        
        self.output_buffer_depth=(1<<self.output_buffer_depth_bitwidth)
        self.weight_buffer_depth=(1<<self.weight_buffer_depth_bitwidth)

        self.inbuffer_width_bitwidth= log2ceil(2+self.wino_out_size_cell*self.wino_width)
        self.inbuffer_width= (1<<self.inbuffer_width_bitwidth)
        self.inbuffer_height=  (1<<self.inbuffer_height_bitwidth)

        self.indepth_minitile_size=(1<<self.indepth_minitile_size_bitwidth)

        self.outdepth_minitile_size=(1<<self.wino_height)

        if(self.wino_height >4):
            self.weight_port_num=4
        else:
            self.weight_port_num=self.wino_height/2
        
    
        if (self.wino_height==2):
            self.out_port_batch_num= 4
        else:
            self.out_port_batch_num= 8 
        


    def gen_conv_desc(self):
        if(self.wino_domain_size==6):

            if( self.kernel_size == 3):
            
                self.wino3x3_flag = 1;
                self.wino_output_tile_size = 4;
                self.merge_kernel_flag=0;
                self.merge_kernel_size=ALIGN(3,3);
                self.merge_kernel_step=3;
            
            elif( self.kernel_size == 5):
            
                self.wino3x3_flag = 0;
                self.wino_output_tile_size =2;
                self.merge_kernel_flag=0;
                self.merge_kernel_size=3;
                self.merge_kernel_step=3;
            
            else:
            
                self.wino3x3_flag = 1;
                self.wino_output_tile_size = 4;
                self.merge_kernel_flag=1;
                self.merge_kernel_size=ALIGN(self.kernel_size,3);
                self.merge_kernel_step=3;
                self.kernel_size=3;
            

        else:
        
            if(self.kernel_size==3):
            
                self.wino3x3_flag = 1;
                self.wino_output_tile_size = 2;
                self.merge_kernel_flag=0;
                self.merge_kernel_size=ALIGN(3,3);
                self.merge_kernel_step=3;
            
            elif(self.kernel_size==1):
            
                self.wino3x3_flag = 0;
                self.wino_output_tile_size = 4;
                self.merge_kernel_flag=1;
                self.merge_kernel_size=ALIGN(self.kernel_size,3);
                self.merge_kernel_step=3;
                    
            else:
            
                self.wino3x3_flag = 1;
                self.wino_output_tile_size = 2;
                self.merge_kernel_flag=1;
                self.merge_kernel_size=ALIGN(self.kernel_size,3);
                self.merge_kernel_step=3;
            

        
        self.indepth_align8 = ALIGN(self.input_depth,8);
        self.indepth_ceildiv8 = CEIL_DIV(self.input_depth,8)
        self.indepth_align_minitile_size = ALIGN(self.input_depth,self.indepth_minitile_size);
        self.inwidth_ceildiv_inbufferwidth = CEIL_DIV(self.input_width,self.inbuffer_width);
        self.inwidth_align8 = ALIGN(self.input_width,8);
        self.group_indepth = self.indepth_align8;
        self.group_indepth_offset = 0;
        
        self.outwidth_align8 = ALIGN(self.output_width,8)*self.stride_size;

        self.wino_tile_number_in_outwidth = CEIL_DIV(self.output_width, self.wino_output_tile_size *self.wino_width);

        row_occupy_address = self.inwidth_ceildiv_inbufferwidth*self.indepth_align8;
        row_occupy_address_bits = log2ceil(row_occupy_address);


        max_row_bit = self.input_buffer_depth_bitwidth - row_occupy_address_bits;

        row_bit=0;

        
        if(max_row_bit <1):
        

            print("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n".format(self.inwidth_ceildiv_inbufferwidth, self.indepth_align8, input_buffer_depth) );
            exit();
        
        elif( max_row_bit >= 2):
        
            row_bit=2;
            self.row_address_bitnumber_flag=0;
        
        else:
        
            row_bit=1;
            self.row_address_bitnumber_flag=1;
        

        input_buffer_row_capacity = self.inbuffer_height *(1<<row_bit);

        self.input_ddr_bytes = self.group_indepth * self.inwidth_align8 * self.input_height * 2;
        self.input_ddr_128bits = self.group_indepth * self.inwidth_align8 * self.input_height / 8;
        self.group_indepth_x_inwidth_align8_by8 = self.group_indepth * self.inwidth_align8 /8;
        self.group_indepth_offset_x_inwidth_align8_by8 = self.group_indepth_offset * self.inwidth_align8 /8;

        self.input_load_burst_length = self.indepth_align8 * self.inwidth_align8/8;
        self.buffer_address_mid_increment_step = self.inwidth_ceildiv_inbufferwidth*(8-self.indepth_minitile_size)/self.indepth_minitile_size +1 ;
        
        

        self.outdepth_align8 = ALIGN(self.output_depth,8);
        self.outheight_align4 = ALIGN(self.output_height*self.stride_size,4);
        self.outdepth_align_minitile_size = ALIGN(self.output_depth,self. outdepth_minitile_size);
        self.group_outdepth=self.outdepth_align8;
        self.group_outdepth_offset=0;
        self.output_ddr_bytes = self.group_outdepth * self.outwidth_align8 * self.output_height*2;
        self.output_ddr_128bits = self.group_outdepth * self.outwidth_align8 * self.output_height/8;


        total_outdepth_minitile_number = CEIL_DIV(self.outdepth_align8,self.outdepth_minitile_size);
        total_indepth_minitile_number = CEIL_DIV(self.indepth_align8,self.indepth_minitile_size);

        

        weight_capacity = self.weight_buffer_depth/2; 

        outdepth_factor_list=[]
        indepth_factor_list=[]
        find_factor(total_outdepth_minitile_number,outdepth_factor_list);
        find_factor(total_indepth_minitile_number,indepth_factor_list);


        indepth_factor,outdepth_factor=find_max_factor_product(indepth_factor_list,outdepth_factor_list,weight_capacity);
        


        self.weightbuffer_indepth_minitile_number = indepth_factor;
        self.weightbuffer_load_indepth_step = indepth_factor * self.indepth_minitile_size;
        self.weightbuffer_load_indepth_number = self.indepth_align8 / self.weightbuffer_load_indepth_step;
        
        self.weightbuffer_outdepth_minitile_number = outdepth_factor;
        self.weightbuffer_load_outdepth_step = outdepth_factor*self.outdepth_minitile_size;
        self.weightbuffer_load_outdepth_number = self.outdepth_align8 / self.weightbuffer_load_outdepth_step;

        weightbuffer_load_size=indepth_factor*outdepth_factor;
        weightddr_indepth_minitile_128bit_step=self.indepth_minitile_size;

        if( self.kernel_size != 1):
            self.weightDDR_buffer_burst_length = (self.indepth_minitile_size/2)*weightbuffer_load_size;
        else:
            self.weightDDR_buffer_burst_length = (self.indepth_minitile_size/2)*weightbuffer_load_size;
        





        self.weightDDR_port_burst_length = (self.outdepth_minitile_size/self.weight_port_num)*self.weightDDR_buffer_burst_length;
        self.weightDDR_burst_number = self.weightbuffer_load_indepth_number * self.weightbuffer_load_outdepth_number;

        expected_row_step =CEIL_DIV(weightddr_indepth_minitile_128bit_step * self.outdepth_minitile_size/self.weight_port_num*5, self.wino_tile_number_in_outwidth*4)*self.wino_output_tile_size;
        expected_row_step=ALIGN(expected_row_step,self.wino_output_tile_size);
        
        maximum_row_step_input =(input_buffer_row_capacity+1-self.merge_kernel_step)/2;
        
        outdepth_minitile_number = self.weightbuffer_outdepth_minitile_number*self.weightbuffer_load_outdepth_number;
        maximum_row_step_output = self.output_buffer_depth / (outdepth_minitile_number * self.wino_tile_number_in_outwidth) * self.wino_output_tile_size;                               

        
        
        
        

        maximum_row_step =maximum_row_step_output if( maximum_row_step_output < maximum_row_step_input ) else maximum_row_step_input;
        assert( maximum_row_step >= self.wino_output_tile_size);
        row_step=0;
        if(expected_row_step <= maximum_row_step ):
            row_step=expected_row_step if(expected_row_step<self.output_height*self.stride_size) else self.output_height*self.stride_size;
        
        elif(input_buffer_row_capacity >= self.input_height ):
            row_step=ALIGN(self.output_height*self.stride_size,4);
        else:
            row_step=maximum_row_step//self.wino_output_tile_size*self.wino_output_tile_size;
        
        # print("row_step",row_step)


        self.out_rowstep=row_step;
        self.wino_tile_number_in_out_rowstep=row_step//self.wino_output_tile_size;


        self.loop_wino_tile_col_reset_cycle =  self.indepth_minitile_size*self.wino_tile_number_in_outwidth;
        self.loop_wino_tile_row_reset_cycle =  self.loop_wino_tile_col_reset_cycle *self.wino_tile_number_in_out_rowstep;
        self.input_buffer_feeding_loop_bound = self.weightbuffer_load_indepth_number*self.weightbuffer_indepth_minitile_number *self.loop_wino_tile_row_reset_cycle;
        
        self.buffer_address_mid_minitile_depth_step = self.inwidth_ceildiv_inbufferwidth;
        self.wino_out_size_by_wino_width=self.wino_output_tile_size*self.wino_width;
        
        self.input_transform_feeding_loop_bound = self.input_buffer_feeding_loop_bound*self.weightbuffer_load_outdepth_number;


        self.loop_omini_base_reset_cycle =self.weightbuffer_outdepth_minitile_number if( self.weightbuffer_outdepth_minitile_number>self.indepth_minitile_size) else self.indepth_minitile_size;
        self.total_input_stream_tile=   self.weightbuffer_load_outdepth_number * self.weightbuffer_load_indepth_number *self.weightbuffer_indepth_minitile_number * self.wino_tile_number_in_outwidth *self.wino_tile_number_in_out_rowstep;
        self.loop_wino_tile_rowcol_self_reset_cycle_min1 = self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth-1;

        self.loop_iload_reset_cycle =  self.weightbuffer_load_indepth_number*self.weightbuffer_indepth_minitile_number*self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth*self.loop_omini_base_reset_cycle;

        self.loop_wino_cell_bound =    self.weightbuffer_load_outdepth_number *self.weightbuffer_load_indepth_number*self.weightbuffer_indepth_minitile_number*self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth*self.loop_omini_base_reset_cycle;
        
        self.outbuffer_oload_increment_step =  self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth*self.weightbuffer_outdepth_minitile_number;
        
        self.outbuffer_omini_increment_step =  self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth;
                                    

        self.loop_outdepth_minitile_baseidx_reset_cycle_minus1=self.weightbuffer_outdepth_minitile_number-1;
        self.loop_start_output_baserowcol_reset_cycle= self.weightbuffer_outdepth_minitile_number * self.wino_tile_number_in_outwidth *self.wino_tile_number_in_out_rowstep;
        self.loop_weight_feed_bound =  self.weightbuffer_indepth_minitile_number * self.loop_start_output_baserowcol_reset_cycle;



        self.outdepth_ceildiv8 = CEIL_DIV(self.output_depth, 8);
        self.output_burst_length = self.outwidth_align8*CEIL_DIV(self.output_depth, self.out_port_batch_num)//self.stride_size;


        self.wino_col_pix_upper_bound= self.wino_output_tile_size-self.stride_size;
        self.wino_tile_number_rowcol=self.wino_tile_number_in_out_rowstep*self.wino_tile_number_in_outwidth;
        self.out_ddr_increment_step=self.output_burst_length*self.wino_output_tile_size//self.stride_size;


        self.scale_oback_int=self.scale_writeback;

        self.merge_weight_row_step=self.merge_kernel_size//3*self.weightDDR_port_burst_length*self.weightDDR_burst_number*4;
        self.merge_weight_col_step=self.weightDDR_port_burst_length*self.weightDDR_burst_number*4;

        self.relu_flag=self.relu_flag;

    def to_array(self, params):
        params[0]=self.input_height;
        params[1]=self.input_width;
        params[2]=self.input_depth;
        params[3]=self.output_height;
        params[4]=self.output_width;
        params[5]=self.output_depth;
        params[6]=self.kernel_size;
        params[7]=self.pad_size;
        params[8]=self.stride_size;
        params[9]=self.wino3x3_flag; 
        params[10]=self.wino_output_tile_size;
        params[11]=self.indepth_align_minitile_size;
        params[12]=self.indepth_align8;
        params[13]=self.indepth_ceildiv8;
        params[14]=self.inwidth_ceildiv_inbufferwidth;
        params[15]=self.inwidth_align8;
        params[16]=self.group_indepth_offset;
        params[17]=self.group_indepth;
        params[18]=self.input_ddr_bytes;
        params[19]=self.input_ddr_128bits;
        params[20]=self.group_indepth_x_inwidth_align8_by8;
        params[21]=self.group_indepth_offset_x_inwidth_align8_by8;
        params[22]=self.input_load_burst_length;
        params[23]=self.buffer_address_mid_increment_step;
        params[24]=self.row_address_bitnumber_flag;
        params[25]=self.outwidth_align8;
        params[26]=self.outdepth_align8;
        params[27]=self.outheight_align4;
        params[28]=self.outdepth_align_minitile_size;
        params[29]=self.group_outdepth_offset;
        params[30]=self.group_outdepth;
        params[31]=self.output_ddr_bytes;
        params[32]=self.output_ddr_128bits;
        params[33]=self.weightbuffer_load_indepth_number;
        params[34]=self.weightbuffer_load_indepth_step;
        params[35]=self.weightbuffer_load_outdepth_number;
        params[36]=self.weightbuffer_load_outdepth_step;
        params[37]=self.weightbuffer_indepth_minitile_number;
        params[38]=self.weightbuffer_outdepth_minitile_number;
        params[39]= 0; #self.weightbuffer_total_load_number;
        params[40]=self.weightDDR_buffer_burst_length;
        params[41]=self.weightDDR_port_burst_length;
        params[42]=self.weightDDR_burst_number;
        params[43]=self.loop_outdepth_minitile_baseidx_reset_cycle_minus1;
        params[44]=self.loop_start_output_baserowcol_reset_cycle;
        params[45]=self.loop_weight_feed_bound;
        params[46]=self.wino_out_size_by_wino_width;
        params[47]=self.wino_tile_number_in_outwidth;
        params[48]=0; #self.loop_outdepth_minitile_baseidx_reset_cycle;
        params[49]=self.loop_wino_tile_col_reset_cycle;
        params[50]=self.loop_wino_tile_row_reset_cycle;
        params[51]=self.buffer_address_mid_minitile_depth_step;
        params[52]=self.input_buffer_feeding_loop_bound;
        params[53]=self.input_transform_feeding_loop_bound;
        params[54]=self.out_rowstep;
        params[55]=self.wino_tile_number_in_out_rowstep;
        params[56]=self.total_input_stream_tile;
        params[57]=self.loop_omini_base_reset_cycle;
        params[58]=self.loop_wino_cell_bound;
        params[59]=self.loop_wino_tile_rowcol_self_reset_cycle_min1;
        params[60]=self.loop_iload_reset_cycle;
        params[61]=self.outbuffer_oload_increment_step;
        params[62]=self.outbuffer_omini_increment_step;
        params[63]=self.outdepth_ceildiv8;
        params[64]=self.output_burst_length;
        params[65]=0;#self.write_back_flag;
        params[66]=self.wino_col_pix_upper_bound;
        params[67]=self.wino_tile_number_rowcol;
        params[68]=self.out_ddr_increment_step;
        params[69]=self.merge_kernel_size;
        params[70]=self.merge_kernel_step;
        params[71]=self.merge_kernel_flag;
        params[72]=self.scale_oback_int;
        params[73]=self.merge_weight_row_step;
        params[74]=self.merge_weight_col_step;
        params[75]=self.relu_flag;



def running_test( argv,validate_dict):
    conv_desc=ConvDesc_t(argv)
    conv_desc.gen_conv_desc()

  


    sys_command=""
    for i in range(1,12):
        sys_command+=" "+str(argv[i])
    sys_command+=" random"
    sys_command+=" random"
    sys_command+=" random"
    sys_command+=" dump_bin"
    sys_command+=" ．"
    sys_command+=" 1 "

    ret_val=os.system("./single_csim.out "+sys_command +">output.txt")

    key=int(argv[1]),int(argv[3]),int(argv[7])
    prefix=""
    for i in key:
        prefix=prefix+str(i)+"_"



    validate_dict[ key ] = [0, argv[11]]
    
    
    # if(ret_val%256 !=0):
    #     validate_dict[ key ].append("NA")
    #     validate_dict[ key ].append("NA")
    #     return

    xlnk = Xlnk()
    xlnk.xlnk_reset()

    
    overlay = Overlay("design_1_wrapper.bit")
    print("bitstream loaded")
    test = overlay.wino_systolic_top_0
    input_FM = xlnk.cma_array(shape=(224*224*64), dtype=np.int16)
    weight = xlnk.cma_array(shape=(3*3*512*512), dtype=np.int32)
    output_FM = xlnk.cma_array(shape=(224*224*64), dtype=np.int16)
    params = xlnk.cma_array(shape=(128), dtype=np.int32)

    output_FM.fill(0)
    # conv_desc.to_array(params)



    

    params_load=np.fromfile("param.bin",dtype=np.int32)
    for i in range(74):
        if( params[i]!=params_load[i] ):
            params[i]=params_load[i]

    print("row_step ", params[54] )



    input_load=np.fromfile("input.bin",dtype=np.int16)
    for i in range(len(input_load)):
        input_FM[i]=input_load[i]


    weight_load=np.fromfile("weight.bin",dtype=np.int32)
    for i in range(len(weight_load)):
        weight[i]=weight_load[i]


    output_load=np.fromfile("output.bin",dtype=np.int16)


    for i in range(len(output_FM) ):
        output_FM[i]=0xAAAA
    
    output_FM.tofile("filecontent")

    physical_byte_num= conv_desc.input_depth*conv_desc.output_depth*8//4
    
    test.write(0x10,input_FM.physical_address)
    test.write(0x18,input_FM.physical_address)
    test.write(0x20,weight.physical_address+physical_byte_num*0)
    test.write(0x28,weight.physical_address+physical_byte_num*1)
    test.write(0x30,weight.physical_address+physical_byte_num*2)
    test.write(0x38,weight.physical_address+physical_byte_num*3)
    test.write(0x40,output_FM.physical_address)
    test.write(0x48,output_FM.physical_address)
    test.write(0x50,params.physical_address)

    print("start calling", test.read(0x00))
    start = time.time()

    test.write(0x00, 1)
    isready = test.read(0x00)
    while( isready == 1 ):
        isready = test.read(0x00)

    end = time.time()
    print( (end - start)*1e9/100 )
 

    # validate_dict[ key ].append(output_load)
    
    count=0;
    print(conv_desc.outdepth_align8, conv_desc.outwidth_align8, conv_desc.output_height)
    for i in range(conv_desc.outdepth_align8*conv_desc.outwidth_align8*conv_desc.output_height):
        if(output_load[i]!=
            output_FM[i]):
            # output_load[i]=output_FM[i]
            count+=1

    error_rate=count/(conv_desc.outdepth_align8*conv_desc.outwidth_align8*conv_desc.output_height)
    print(key, conv_desc.outdepth_align8*conv_desc.outwidth_align8*conv_desc.output_height)
    print("error_rate",error_rate)
    validate_dict[ key ].append((end - start)*1e9/100) 
    validate_dict[ key ].append(error_rate )
    
    output_FM.tofile("bin/cnm_"+prefix+"output.bin")

    os.system("cp output.bin bin/"+prefix+"output.bin")
    os.system("cp param.bin bin/"+prefix+"param.bin")
    os.system("cp input.bin bin/"+prefix+"input.bin")
    os.system("cp weight.bin bin/"+prefix+"weight.bin")


    input_FM.close()
    weight.close()
    output_FM.close()
    params.close()




if __name__ == "__main__":


    if(len(sys.argv)==1 ):
        # output_depth_test_case= [8,  8,  8,     8,8, 8,    8, 8, 8]
        # input_depth_test_case=  [8,  8,  8,     8,8, 8,    8, 8, 8]
        # input_dim_test_cases=   [224,224,112, 112,56, 56, 28, 28, 14]
        kernel_dim=             [1,3,5,7,9]
    else:
        depth_test_case=[int(sys.argv[2])]
        input_dim_test_cases=[int(sys.argv[1])]

    result_dict={}
    # for i in range(len(kernel_dim)):
    #     id=32
    #     od=32
    #     ih=112
    #     ks=kernel_dim[i]
    #     scale_fact=(1<<14)//id//ks;
    #     argv=[0,ih,ih,id,ih,ih,od,ks,1,ks//2,1,scale_fact,"src/wino_hw_config.h"]
    #     running_test(argv, result_dict)
    for i in range(len(kernel_dim)):
        id=64
        od=32
        ih=56
        ks=kernel_dim[i]
        scale_fact=(1<<14)//id//ks;
        argv=[0,ih,ih,id,ih,ih,od,ks,1,ks//2,1,scale_fact,"src/wino_hw_config.h"]
        running_test(argv, result_dict)

    for key, val in result_dict.items():
        print (key , val)

   



