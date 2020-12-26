#include <ap_int.h>
#include "param.h"
#include "buffer.h"
#include "../src/wino_macro.h"
#include "../src/wino.h"
#include "softlayer.hpp"
#include "../src_gold/wino_gold.h"
#include "../testbench/debug.hpp"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

template<class T1,class T2>
bool compare(T1* result, T2* gold, int length)
{
    for(int i=0;i<length;i++)
    {
        if(result[i]!=gold[i]) return false;
    }
    return true;
}



void check_error_char(char* result, float* gold, int dim[3],float scale_factor)
{
        int depth=dim[0];
        int height=dim[1];
        int width=dim[2];
        int total=0;
        std::cout<<"Check error ["<<depth <<" "<<height <<" "<<width <<"]"<<std::endl;
        for(int d=0;d<depth;d++)
        {
            for(int h=0;h<height;h++)
            {
                for(int w=0;w<width;w++)
                {
                    int gold_rst=gold[(d*height+h)*width+w]*scale_factor;
                    float error=fabs(result[(d*height+h)*width+w]-gold_rst);

                    if( error<2 ) continue;
                        total++;
                        printf("[%3d %3d]",result[(d*height+h)*width+w],gold_rst );
                }
            }
        }
        printf("\n");
        printf("Error rate %f\n",((float) total)/(depth*width*height) );
        fflush(stdout);
}



void check_error(float* result, float* gold, int dim[3],float scale_factor)
{
        int depth=dim[0];
        int height=dim[1];
        int width=dim[2];
        // std::cout<<"Check error ["<<depth <<" "<<height <<" "<<width <<"]"<<std::endl;
        for(int d=0;d<depth;d++)
        {
            // printf("Depth %d\n",d);
            for(int h=0;h<height;h++)
            {
                for(int w=0;w<width;w++)
                {                    
                    if( fabs(result[(d*height+h)*width+w]-gold[(d*height+h)*width+w]*scale_factor)> 1)
                        printf("(%3d)[%8f %8f]",w, result[(d*height+h)*width+w], gold[(d*height+h)*width+w]*scale_factor );
                    else
                        printf("(%3d)[                 ]",w);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
        fflush(stdout);
}




int find_max(float* array, int length)
{
    float max=array[0];
    int imax=0;
    for(int i=0;i<length;i++)
    {
        if(array[i]>max)
        {
            max=array[i];
            imax=i;
        }
    }
    return imax;
}



void feature_map_int_to_float(
    float* float_buffer,
    char* char_buffer,
    int size,
    int qbit,
    float Scale_blob
)
{
    std::cout<<"Scale Blob "<<Scale_blob<< std::endl;
    std::cout<<"Quant bit"<<qbit<< std::endl;
    fflush(stdout);

    for(int i=0;i<size;i++)
    {
        // assert( char_buffer[i]<=( (1<<(qbit-1))-1) && char_buffer[i]>( -(1<<(qbit-1)) ) );
        float_buffer[i]=char_buffer[i]/Scale_blob;
    }
}




void exec_plot(
    std::vector<LayerInfo_t > & layerinfo_vect,
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        std::cout<<"Exec Layer "<<layerinfo_vect[i].layer_name<<" "<<layerinfo_vect[i].layer_type<<"\n";
        fflush(stdout);
        // for(int j=0;j<layerinfo_vect[i].bottoms.size();j++)
        // {
        //     std::cout<<layerinfo_vect[i].bottoms[j]<<" ";
        // }
        // std::cout<<"][";
        // for(int j=0;j<layerinfo_vect[i].tops.size();j++)
        // {
        //     std::cout<<layerinfo_vect[i].tops[j]<<" ";
        // }
        // std::cout<<"]"<<std::endl;
        if(layerinfo_vect[i].layer_type=="conv")
        {
       
            // Float gold declaration region
            float* bottom_gold = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_gold[0];
            float* top_gold = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_gold[0];
            
            // Float undertest declaration region
            float* bottom0_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* bottom1_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* top0_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[0];
            float* top1_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[1];
            

            // Float scaled declaration region
            float* bottom0_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* bottom1_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* top0_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[0];
            float* top1_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[1];

            //integer region
            char* bottom_int0 = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[0];
            char* bottom_int1 = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[1];
            char* top_int0 = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[0];
            char* top_int1 = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[1];


            // Hardware declaration region
            char* input_DDR=feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_hw[0];
            char* output_DDR=feature_map_dict[layerinfo_vect[i].tops[0]].buffers_hw[0];

            
            //$ layer skip region
            //$ please clean here after use

            // if(layerinfo_vect[i].layer_name!="conv1_1") continue;
           

            // std::size_t found = layerinfo_vect[i].layer_name.find("DS");
            // if(found==std::string::npos) continue;
            

            //$ Buffer preprocessing region
            //$ please clean here after use

                // scale_on<float,float>(bottom_gold,bottom0_scale,layerinfo_vect[i].indim, feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob);
                // scale_on<float,char>(bottom_gold,bottom_int0,layerinfo_vect[i].indim, feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob);
                // // feature_map_float_to_int( 
                //     bottom0_sw,
                //     bottom_int0,
                //     feature_map_dict[layerinfo_vect[i].bottoms[0]].buffer_size_int,
                //     IN_WIDTH,
                //     feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob);
                // feature_map_float_to_int( 
                //     bottom0_sw,
                //     bottom_int1,
                //     feature_map_dict[layerinfo_vect[i].bottoms[0]].buffer_size_int,
                //     IN_WIDTH,
                //     feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob);

            // feature_map_init( feature_map_dict[layerinfo_vect[i].bottoms[0]],"order");

            // featuremap_int_to_hw(
            //     feature_map_dict[layerinfo_vect[i].bottoms[0]],
            //     layerinfo_vect[i].conv_desc.group_indepth_offset,
            //     layerinfo_vect[i].conv_desc.group_indepth
            // );
            // init_weight<char>(layerinfo_vect[i].weightbuffer_quant[0],
            //         layerinfo_vect[i].indim[0],
            //         layerinfo_vect[i].outdim[0],
            //         layerinfo_vect[i].kernel_size,
            //         "kernel_order");
            // init_bias_int(layerinfo_vect[i].biasbuffer_quant[0],layerinfo_vect[i].outdim[0],"zero");
            //$ convsoft running, donot comment out this line


                // feature_map_buffer_init<float>(bottom0_scale,
                // layerinfo_vect[i].indim[0],
                // layerinfo_vect[i].indim[1],
                // layerinfo_vect[i].indim[2],
                // "order");
                // init_weight<float>(layerinfo_vect[i].weightbuffers_float_scaled[0],
                // layerinfo_vect[i].indim[0],
                // layerinfo_vect[i].outdim[0],
                // layerinfo_vect[i].kernel_size,
                // "kernel_order"); 

       
                // conv_soft<float>(
                // bottom_gold,
                // top0_sw,
                // layerinfo_vect[i].weightbuffers_sw[0],
                // layerinfo_vect[i].biasbuffers_sw[0],
                // layerinfo_vect[i].indim[0],
                // layerinfo_vect[i].indim[1],
                // layerinfo_vect[i].indim[2],
                // layerinfo_vect[i].outdim[0],
                // layerinfo_vect[i].outdim[1],
                // layerinfo_vect[i].outdim[2],
                // layerinfo_vect[i].pad_size,
                // layerinfo_vect[i].kernel_size,
                // layerinfo_vect[i].stride,
                // true,
                // layerinfo_vect[i].relu_flag);

            
       
            //$ wino_float running, donot comment out this line



                // wino_model_float_offset(
                // bottom0_scale,
                // layerinfo_vect[i].indim[0],
                // layerinfo_vect[i].indim[1],
                // layerinfo_vect[i].indim[2],
                // top0_scale,
                // layerinfo_vect[i].outdim[0],
                // layerinfo_vect[i].outdim[1],
                // layerinfo_vect[i].outdim[2],
                // layerinfo_vect[i].weightbuffers_float_scaled[0],
                // layerinfo_vect[i].bias_float_scaled[0],
                // layerinfo_vect[i].kernel_size,
                // layerinfo_vect[i].pad_size,
                // layerinfo_vect[i].stride,
                // layerinfo_vect[i].relu_flag,
                // layerinfo_vect[i].Scale_oback_int);
                // // 1);
                

                // check_error(top0_scale,top0_sw,layerinfo_vect[i].outdim,feature_map_dict[layerinfo_vect[i].tops[0]].Scale_blob);
                // getchar();
                // exit(0);

            //$ wino_int running, donot comment out this line
                // char* top_temp0 = new char[ feature_map_dict[layerinfo_vect[i+1].bottoms[0]].buffer_size_int*layerinfo_vect[i].stride*layerinfo_vect[i].stride];
                // char* top_temp1 = new char[ feature_map_dict[layerinfo_vect[i+1].bottoms[0]].buffer_size_int*layerinfo_vect[i].stride*layerinfo_vect[i].stride];
                
                wino_model_int(
                bottom_int0,
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                top_int0,
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].weightbuffer_quant[0],
                layerinfo_vect[i].biasbuffer_quant[0],
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].stride,
                layerinfo_vect[i].relu_flag,
                layerinfo_vect[i].Scale_oback_int);

                // wino_model_int(
                // bottom_int1,
                // layerinfo_vect[i].indim[0],
                // layerinfo_vect[i].indim[1],
                // layerinfo_vect[i].indim[2],
                // top_int1,
                // layerinfo_vect[i].outdim[0],
                // layerinfo_vect[i].outdim[1],
                // layerinfo_vect[i].outdim[2],
                // layerinfo_vect[i].weightbuffer_quant[0],
                // layerinfo_vect[i].biasbuffer_quant[0],
                // layerinfo_vect[i].kernel_size,
                // layerinfo_vect[i].pad_size,
                // layerinfo_vect[i].stride,
                // layerinfo_vect[i].relu_flag,
                // layerinfo_vect[i].Scale_oback_int);

            //$ wino_hw running, donot comment out this line

            // process_hw_weight_buffer_single_layer(layerinfo_vect[i]);
            // layerinfo_vect[i].conv_desc.scale_oback_int=(1<<(OBACK_QUANT_BIT-2));
            // print_weight<0>( (ap_uint<128>*)layerinfo_vect[i].weightbuffers_hw[0], "weight_content.txt",layerinfo_vect[i].port_segment_size[0]*36);
            // getchar();
            wino_systolic_top(
                (ap_uint<128>*) input_DDR,
                (ap_uint<128>*) input_DDR,
                (ap_uint<128>*) input_DDR,
                (ap_uint<128>*) input_DDR,
                (ap_uint<128>*) layerinfo_vect[i].weightbuffers_hw[0],
                (ap_uint<128>*) layerinfo_vect[i].weightbuffers_hw[1],
                (ap_uint<128>*) layerinfo_vect[i].weightbuffers_hw[2],
                (ap_uint<128>*) layerinfo_vect[i].weightbuffers_hw[3],
                (ap_uint<128>*) output_DDR,
                (ap_uint<128>*) output_DDR,
                (ap_uint<128>*) output_DDR,
                (ap_uint<128>*) output_DDR,
                (ap_int<32>*)  & (layerinfo_vect[i].conv_desc),
                (ap_int<32>*) layerinfo_vect[i].biasbuffer_quant[0]
                #ifdef __SDSVHLS__
                ,0
                #endif
            );


            //$ buffer post_process


            // if(found!=std::string::npos)
            // {

                feature_map_init( feature_map_dict[layerinfo_vect[i].tops[0]],"order");

                featuremap_int_to_hw(
                    feature_map_dict[layerinfo_vect[i].tops[0]],
                    layerinfo_vect[i].conv_desc.group_outdepth_offset,
                    layerinfo_vect[i].conv_desc.group_outdepth
                );

                char* out_hw1 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int*layerinfo_vect[i].stride*layerinfo_vect[i].stride];
                char* out_hw2 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int*layerinfo_vect[i].stride*layerinfo_vect[i].stride];

                featuremap_hw_to_int_pointers(
                    output_DDR,
                    out_hw1,
                    out_hw2,
                    layerinfo_vect[i].outdim[1],
                    layerinfo_vect[i].outdim[2],
                    layerinfo_vect[i].outdim[0],
                    0,
                    layerinfo_vect[i].outdim[0]
                );




                print_feature_map<char>(top_int0, 
                "C1_mdoel_int_out.txt",
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0]    
                );

                print_feature_map<char>(out_hw1, 
                "C1_hw_out.txt",
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0]
                );

                print_feature_map<char>(top_int1, 
                "C2_mdoel_int_out.txt",
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0]    
                );

                print_feature_map<char>(out_hw2, 
                "C2_hw_out.txt",
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0]
                );

                int dimsize= layerinfo_vect[i].outdim[0]*layerinfo_vect[i].outdim[1]*layerinfo_vect[i].outdim[2];

                
                scale_off<char,float>(top_int0,top0_sw,layerinfo_vect[i].outdim,feature_map_dict[layerinfo_vect[i].tops[0]].Scale_blob );
                scale_off<char,float>(out_hw1,top1_sw,layerinfo_vect[i].outdim,feature_map_dict[layerinfo_vect[i].tops[0]].Scale_blob);


                FILE* rptr=fopen("outputr.bin","wb");
                FILE* gptr=fopen("outputg.bin","wb");
                
                fwrite(top0_sw,sizeof(float),dimsize,rptr);
                fwrite(top1_sw,sizeof(float),dimsize,gptr);                
                fclose(rptr);
                fclose(gptr);
                



                // delete [] out_hw1;
                // delete [] out_hw2;
                getchar();
            // }


            


        
    
        }
        else
        if(layerinfo_vect[i].layer_type=="maxpool")
        {
            // Float gold declaration region
            float* bottom_gold = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_gold[0];
            float* top_gold = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_gold[0];
            
            // Float undertest declaration region
            float* bottom0_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* bottom1_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* top0_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[0];
            float* top1_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[1];
            
            // Float scaled declaration region
            float* bottom0_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* bottom1_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* top0_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[0];
            float* top1_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[1];

            //integer region
            char* bottom0_int = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[0];
            char* bottom1_int = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[1];
            char* top0_int = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[0];
            char* top1_int = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[1];


            // check_error(bottom0,bottom_gold,layerinfo_vect[i].indim);
            char* input_DDR=feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_hw[0];
            char* output_DDR=feature_map_dict[layerinfo_vect[i].tops[0]].buffers_hw[0];
            if(layerinfo_vect[i].layer_name=="avgpool")
            {
                avg_pooling_int<char>(bottom0_int,top0_int,
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].stride);

                avg_pooling_soft<float>(bottom0_scale,top0_scale,
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].stride);
            }
            else
            {
                max_pooling_soft<char>(bottom0_int,top0_int,
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].stride);



                max_pooling_soft<char>(bottom1_int,top1_int,
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].stride);
              
                max_pooling_hw<char>(
                input_DDR,
                output_DDR,
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].pad_size,
                layerinfo_vect[i].kernel_size,
                layerinfo_vect[i].stride);

            }

            
            // char* out_hw1 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int];
            // char* out_hw2 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int];

            // featuremap_hw_to_int_pointers(
            //     output_DDR,
            //     out_hw1,
            //     out_hw2,
            //     layerinfo_vect[i].outdim[1],
            //     layerinfo_vect[i].outdim[2],
            //     layerinfo_vect[i].outdim[0],
            //     0,
            //     layerinfo_vect[i].outdim[0]
            // );



            // int dimsize= layerinfo_vect[i].outdim[0]*layerinfo_vect[i].outdim[1]*layerinfo_vect[i].outdim[2];
            // scale_off<char,float>(out_hw1,top1_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            // scale_off<char,float>(top0_int,top0_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
        
            // FILE* fptr=fopen("outputr.bin","wb");
            // FILE* gptr=fopen("outputg.bin","wb");
    
            // fwrite(top1_sw,sizeof(float),dimsize,fptr);
            // fwrite(top0_sw,sizeof(float),dimsize,gptr);                
            // fclose(fptr);
            // fclose(gptr);
            
            // delete [] out_hw1;
            // delete [] out_hw2;
            // getchar();

            // getchar();
            // check_error(top0_sw,top_gold,layerinfo_vect[i].outdim,1);
        
        }
        else 
        if(layerinfo_vect[i].layer_type=="residual")
        {

            float* bottom_left_gold = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_gold[0];
            float* bottom_right_gold = feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_gold[0];
            float* bottom_left_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* bottom_right_sw = feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_sw[0];
            float* bottom_left_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* bottom_right_scale = feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_scale[0];
            
            char* bottom_left_int0 = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[0];
            char* bottom_left_int1 = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[1];
            char* bottom_right_int0 = feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_int[0];
            char* bottom_right_int1 = feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_int[1];
            
            float* top_gold = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_gold[0];
            float* top0_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[0];
            float* top1_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[1];
            float* top_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[0];
            char* top_int0 = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[0];
            char* top_int1 = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[1];

    


            // check_error(bottom0,bottom_gold,layerinfo_vect[i].indim);
            char* input_DDR_left=feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_hw[0];
            char* input_DDR_right=feature_map_dict[layerinfo_vect[i].bottoms[1]].buffers_hw[0];
            char* output_DDR=feature_map_dict[layerinfo_vect[i].tops[0]].buffers_hw[0];

            elt_soft<char>(
                bottom_left_int0,
                bottom_right_int0,
                top_int0,
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].Scale_oback_int,
                layerinfo_vect[i].relu_flag
            );

            elt_soft<char>(
                bottom_left_int1,
                bottom_right_int1,
                top_int1,
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].Scale_oback_int,
                layerinfo_vect[i].relu_flag
            );
        
            elt_hw<char>(
                input_DDR_left,
                input_DDR_right,
                output_DDR,
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].Scale_oback_int,
                layerinfo_vect[i].relu_flag
            );
            char* out_hw1 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int];
            char* out_hw2 = new char[ feature_map_dict[layerinfo_vect[i].tops[0]].buffer_size_int];

            featuremap_hw_to_int_pointers(
                output_DDR,
                out_hw1,
                out_hw2,
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                0,
                layerinfo_vect[i].outdim[0]
            );



            int dimsize= layerinfo_vect[i].outdim[0]*layerinfo_vect[i].outdim[1]*layerinfo_vect[i].outdim[2];
            scale_off<char,float>(out_hw1,top1_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            scale_off<char,float>(top_int0,top0_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
        
            FILE* fptr=fopen("outputr.bin","wb");
            FILE* gptr=fopen("outputg.bin","wb");
    
            fwrite(top1_sw,sizeof(float),dimsize,fptr);
            fwrite(top0_sw,sizeof(float),dimsize,gptr);                
            fclose(fptr);
            fclose(gptr);

            delete [] out_hw1;
            delete [] out_hw2;
            getchar();
        }
        else    
        if(layerinfo_vect[i].layer_type=="flatten")
        {
            // Float gold declaration region
            float* bottom_gold = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_gold[0];
            float* top_gold = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_gold[0];
            
            // Float undertest declaration region
            float* bottom0_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* bottom1_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* top0_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[0];
            float* top1_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[1];
            
            // Float scaled declaration region
            float* bottom0_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* bottom1_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* top0_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[0];
            float* top1_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[1];

            //integer region
            char* bottom0_int = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[0];
            char* top0_int = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[0];


            // check_error(bottom0,bottom_gold,layerinfo_vect[i].indim);
            // flatten_soft<float>(bottom0_sw,top0_sw,
            //     layerinfo_vect[i].indim[0],
            //     layerinfo_vect[i].indim[1],
            //     layerinfo_vect[i].indim[2]);

            flatten_soft<char>(bottom0_int,top0_int,
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2]);
            flatten_soft<float>(bottom0_scale,top0_scale,
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2]);
            // // getchar();
            // // check_error(top0_sw,top_gold,layerinfo_vect[i].outdim,1);
            // scale_off<float,float>(top0_scale,top_gold,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            // scale_off<char,float>(top0_int,top0_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            // FILE* fptr=fopen("outputr.bin","wb");
            // FILE* gptr=fopen("outputg.bin","wb");
            // int dimsize= layerinfo_vect[i].outdim[0]*layerinfo_vect[i].outdim[1]*layerinfo_vect[i].outdim[2];
            // fwrite(top_gold,sizeof(float),dimsize,fptr);
            // fwrite(top0_sw,sizeof(float),dimsize,gptr);                
            // fclose(fptr);
            // fclose(gptr);
            // getchar();
        }
        else
        if(layerinfo_vect[i].layer_type=="linear")
        {  

            std::cout<< feature_map_dict[layerinfo_vect[i].bottoms[0]].blob_info->goldfilename[0]<<std::endl;
            // Float gold declaration region
            float* bottom_gold = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_gold[0];
            float* top_gold = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_gold[0];
            
            // Float undertest declaration region
            float* bottom0_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* bottom1_sw = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_sw[0];
            float* top0_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[0];
            float* top1_sw = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_sw[1];
            
            // Float scaled declaration region
            float* bottom0_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* bottom1_scale = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_scale[0];
            float* top0_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[0];
            float* top1_scale = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_scale[1];

            //integer region
            char* bottom0_int = feature_map_dict[layerinfo_vect[i].bottoms[0]].buffers_int[0];
            char* top0_int = feature_map_dict[layerinfo_vect[i].tops[0]].buffers_int[0];


            // scale_off<float,float>(bottom0_scale,bottom0_sw,layerinfo_vect[i].indim,layerinfo_vect[i].Scale_i);
            // FILE* rptr=fopen("inputr.bin","wb");
            // FILE* sptr=fopen("inputg.bin","wb");
            // int idimsize= layerinfo_vect[i].indim[0]*layerinfo_vect[i].indim[1]*layerinfo_vect[i].indim[2];
            // fwrite(bottom_gold,sizeof(float),idimsize,rptr);
            // fwrite(bottom0_sw,sizeof(float),idimsize,sptr);                
            // fclose(rptr);
            // fclose(sptr);
            // getchar();



            // if(layerinfo_vect[i].layer_name=="fc1")
            //     feature_map_int_to_float( 
            //         bottom0,
            //         bottom_int0,
            //         layerinfo_vect[i].indim[0],
            //         IN_WIDTH,
            //         feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob
            //     );

            // check_error(bottom0,bottom_gold,layerinfo_vect[i].indim);
            // fc_soft<float>(bottom0_sw,top0_sw,
            //     layerinfo_vect[i].weightbuffers_sw[0],
            //     layerinfo_vect[i].biasbuffers_sw[0],
            //     layerinfo_vect[i].indim[0],
            //     layerinfo_vect[i].outdim[0],
            //     i!=layerinfo_vect.size()-1);
            fc_soft_int<1>(bottom0_int,top0_int,
                layerinfo_vect[i].weightbuffer_quant[0],
                layerinfo_vect[i].biasbuffer_quant[0],
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].Scale_oback_int,
                layerinfo_vect[i].relu_flag);

            fc_soft<float>(bottom0_scale,top0_scale,
                layerinfo_vect[i].weightbuffers_float_scaled[0],
                layerinfo_vect[i].bias_float_scaled[0],
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].Scale_oback_int,
                layerinfo_vect[i].relu_flag);
            // check_error(top0,top_gold,layerinfo_vect[i].outdim);


            scale_off<float,float>(top0_scale,top_gold,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            scale_off<char,float>(top0_int,top0_sw,layerinfo_vect[i].outdim,layerinfo_vect[i].Scale_o);
            FILE* fptr=fopen("outputr.bin","wb");
            FILE* gptr=fopen("outputg.bin","wb");
            int dimsize= layerinfo_vect[i].outdim[0]*layerinfo_vect[i].outdim[1]*layerinfo_vect[i].outdim[2];
            fwrite(top_gold,sizeof(float),dimsize,fptr);
            fwrite(top0_sw,sizeof(float),dimsize,gptr);                
            fclose(fptr);
            fclose(gptr);
            getchar();

            if( i==layerinfo_vect.size()-1)
            {
                int imax=find_max(top0_sw,layerinfo_vect[i].outdim[0]);
                std::cout<<"max index sw"<<imax<<std::endl;
                imax=find_max(top_gold,layerinfo_vect[i].outdim[0]);
                std::cout<<"max index scale"<<imax<<std::endl;
            }
        }        
    }
}

