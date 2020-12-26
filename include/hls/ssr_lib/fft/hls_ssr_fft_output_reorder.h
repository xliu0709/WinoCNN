//filename: hls_ssr_fft_output_reorder.h
#ifndef HLS_SSR_FFT_OUTPUT_REORDER_h_
#define HLS_SSR_FFT_OUTPUT_REORDER_h_
#include <iostream>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_criss_cross_multiplexer.h"
#include "hls_ssr_fft_rotate_criss_cross_multiplexer.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_criss_cross_multiplexer.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_rotate_criss_cross_multiplexer.h"

#endif

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

	The digitReversedDataReOrder  defined in other file function is used to
	perform data re ordering at the output of final stage in SSR FFT.
	The data produced by final stage is
	shuffled in SSR dimension ( R or SSR streams coming from SSR FFT last stage)
	and also it needs re ordering in time dimension. The re ordering is done
	using two PIPO buffers, in 4 phases.

     	 	 	 1- The input streams are rotated in SSR dimension ( R Streams)
     	 	 	 2- The input stream written to PIPO after rotation
     	 	 	 3- The ping pong memory is read
     	 	 	 4- The read data is shuffled and written to output
                    This file defines functions for phases 1,2,3
                    cacheData function : deals with phase 1 and 2
                    WriteCacheData  function deals with phase  3,4
                    and internally calls
                    readBarrelShifter::memReadBarrelShifter
     Note : This function only deals with the cases when tp_L is integer power of
     tp_R , for the cases when tp_L is not integer power of tp_R :
                    	OutputDataReOrder<>::digitReversal2Phase<> is used which
                    	deals with it through different specializations. is defined
                    	in this file

-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
 ========================================================================================
*/
namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void crissCrossAndTimeDigitSwap(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off

				unsigned int inputTimeIndex;
				unsigned int outputTimeIndex;
				const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;

				///////////////////////////////////// Shuffle the Data and Write to PingPong Buffer ////////////////////////////////////
				TIME_LOOP:
				for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
		#pragma HLS PIPELINE rewind
					inputTimeIndex  = t;
					outputTimeIndex = digitReversal_fracIsLSB<tp_L/tp_R,tp_R>(inputTimeIndex);
					tt_dtype sampleShufflingBuffer[tp_R];
		#pragma HLS ARRAY_PARTITION variable=sampleShufflingBuffer complete dim=1
		#pragma HLS DATA_PACK variable=sampleShufflingBuffer

					SSR_READ_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
			#pragma HLS UNROLL
						sampleShufflingBuffer[(r + outputTimeIndex) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val)] = inDatac[r][inputTimeIndex];
					}
					SSR_WRITE_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
			#pragma HLS UNROLL
						pingPongBuff[r][outputTimeIndex] = sampleShufflingBuffer[r]; 	// digit reversal write to the memory

					}
				}

		}
		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void crissCrossAndTimeDigitSwap_specialization(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off

				unsigned int inputTimeIndex;
				unsigned int outputTimeIndex;
				const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
				///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////
				TIME_LOOP:
				for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
		#pragma HLS PIPELINE rewind
					inputTimeIndex  = t;
					outputTimeIndex = digitReversal_m<tp_L/tp_R,tp_R>(inputTimeIndex);
					//replaced//unsigned int InputShuffleAmount = (t*F) % tp_R;//   minOf_R_and_L_over_R; // can be converted to masking
					unsigned int InputShuffleAmount = (t<< (ssr_fft_log2<F>::val)) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);//   minOf_R_and_L_over_R; // can be converted to masking
					tt_dtype sampleShufflingBuffer[tp_R];
		#pragma HLS ARRAY_PARTITION variable=sampleShufflingBuffer complete dim=1
		#pragma HLS DATA_PACK variable=sampleShufflingBuffer

					SSR_READ_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
		#pragma HLS UNROLL
						//replaced//sampleShufflingBuffer[(r + InputShuffleAmount) % tp_R] = inDatac[r][inputTimeIndex];
						sampleShufflingBuffer[(r + InputShuffleAmount) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val)] = inDatac[r][inputTimeIndex];
						//sampleShufflingBuffer[(outputTimeIndex) % tp_R] = inDatac[inputTimeIndex][r];

					}
					SSR_WRITE_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
		#pragma HLS UNROLL
						pingPongBuff[r][outputTimeIndex] = sampleShufflingBuffer[r]; 	// digit reversal write to the memory

					}
				}
		}

		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void diagonalWrapingIndexReadfromPIPOAndStreamOutput(tt_dtype pingPongBuff[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off

			const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
			const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;
			unsigned int pingPongTimeIndex;
			unsigned int pingPongSuperSampleIndex;
			TIME_LOOP:
				for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
					tt_dtype2 temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

		#pragma HLS PIPELINE //rewind      // This loop has a rewind issue.


					//replced// int ssrDimensionAddressOffset = ( t /(  tp_L/(tp_R*tp_R)) ) % tp_R;
					int ssrDimensionAddressOffset = ( t >> (ssr_fft_log2<tp_L/(tp_R*tp_R)>::val)) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);
					int timeIndexAddressOffset =  (t & (ssr_fft_log2BitwiseAndModMask<(tp_L/(tp_R*tp_R))>::val) )   <<(ssr_fft_log2<tp_R>::val);
					//int timeIndexAddressOffset = (t % (tp_L/(tp_R*tp_R)))*tp_R;
					//crissCrossMultiplexerClass<tp_R> crissCrossMultiplexerClass_obj;
					//crissCrossMultiplexerClass_obj.template crissCrossMultiplexer<tp_R,tp_L>(timeIndexAddressOffset,ssrDimensionAddressOffset,pingPongBuff,temp);
					SSR_LOOP0:
					for (unsigned int r = 0; r < tp_R; r++)
					{
					#pragma HLS UNROLL
						/*This expression is replaced below*///int pingPongSuperSampleIndex = ((stage-1) + r) % tp_R;
						int pingPongSuperSampleIndex = ((ssrDimensionAddressOffset) + r) & ssr_fft_log2BitwiseAndModMask<tp_R>::val;
						int pingPongTimeIndex=((tp_R+ r-ssrDimensionAddressOffset)%tp_R)+timeIndexAddressOffset;
						temp[(tp_R+ r-ssrDimensionAddressOffset)%tp_R]= pingPongBuff[r][pingPongTimeIndex];
					}
					SSR_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
		#pragma HLS UNROLL
						outDatac[r][t]=  temp[r];
					}
				}
		}
		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void diagonalWrapingIndexReadfromPIPOAndStreamOutput_specialization(tt_dtype pingPongBuff[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off
		///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////
				const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;   // tp_R is always greater than F
				unsigned int pingPongTimeIndex;
				unsigned int pingPongSuperSampleIndex;
				const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;
				TIME_LOOP:
				for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
		#pragma HLS PIPELINE  //rewind    // This loop has a reqind issue
					tt_dtype2 temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

					//replaced//int ssrDimensionAddressOffset =( F*t ) % tp_R;
					int ssrDimensionAddressOffset =( t<<(ssr_fft_log2<F>::val) ) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);

					//replaced//	int timeIndexAddressOffset = ( tp_R * (t / tp_R) ) % (tp_L/tp_R);
					int timeIndexAddressOffset = (   (t >>(ssr_fft_log2<tp_R>::val) )<<(ssr_fft_log2<tp_R>::val)    )  & (   ssr_fft_log2BitwiseAndModMask<tp_L/tp_R>::val     );
					//rotateCrissCrossMultiplexerClass<tp_R> rotateCrissCrossMultiplexerClass_obj;
					//rotateCrissCrossMultiplexerClass_obj.template rotateCrissCrossMultiplexer<tp_R,tp_L>(timeIndexAddressOffset,ssrDimensionAddressOffset,pingPongBuff,temp);
					SSR_LOOP0:
					for (unsigned int r = 0; r < tp_R; r++)
					{
					#pragma HLS UNROLL
						ap_uint<  ssr_fft_log2<tp_R>::val > rotated_r = r;
						rotated_r.lrotate(outputShuffleAmount);
						//replaced//int pingPongSuperSampleIndex = ((stage-1) + rotated_r) % tp_R;
						int pingPongSuperSampleIndex =r;// ((ssrDimensionAddressOffset) + rotated_r) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val);

						//replaced//int pingPongTimeIndex=(r+timeIndexAddressOffset)%(tp_L/tp_R);
						ap_uint<  ssr_fft_log2<tp_R>::val > rev_rotated_r = (tp_R + r - ssrDimensionAddressOffset)%tp_R;
						rev_rotated_r.rrotate(outputShuffleAmount);
						int pingPongTimeIndex=(rev_rotated_r+timeIndexAddressOffset)& (ssr_fft_log2BitwiseAndModMask<tp_L/tp_R>::val);
						temp[rev_rotated_r]= pingPongBuff[r][pingPongTimeIndex];
						//CHECK_COVEARAGE;
					}
					SSR_LOOP:
					for (unsigned int r = 0; r < tp_R; r++)
					{
		#pragma HLS UNROLL
						outDatac[r][t]=  temp[r];
					}

				}
		}

		//// OutputDataReOrder<skew>::digitReversal2Phase : is used for output data re-ordering for the case when the length of the SSR FFT is
		// not integer power of length, It has also a specialization
		template<int skew>
		struct OutputDataReOrder {
			template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
			void digitReversal2Phase(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R]);
		};


		template<int skew>
		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void OutputDataReOrder<skew>::digitReversal2Phase(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE


			tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R];
		#pragma HLS ARRAY_PARTITION variable=pingPongBuff complete dim=1

		#pragma HLS DATA_PACK variable=pingPongBuff
			//// %%%%%%%%%%%%%%%%% Process:1:Create a Process for buffering streaming data in digit swapped order also do the criss cross from different memories %%%%%%%%%%%%%%
			crissCrossAndTimeDigitSwap<tp_L,tp_R>(inDatac, pingPongBuff);


			///// %%%%%%%%%%%%%%% Process:2:Create 2nd process that connect to process-1 through PIPO and generates an output stream %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			diagonalWrapingIndexReadfromPIPOAndStreamOutput<tp_L,tp_R>(pingPongBuff,outDatac);

		}

		template<>
		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void OutputDataReOrder<0>::	digitReversal2Phase(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
		//#pragma HLS DATAFLOW
			tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R];
		#pragma HLS ARRAY_PARTITION variable=pingPongBuff complete dim=1
		//#pragma HLS ARRAY_PARTITION variable=inDatac complete dim=1
		#pragma HLS DATA_PACK variable=pingPongBuff


			///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////
			 //// %%%%%%%%%%%%%%%%% Process:1:Create a Process for buffering streaming data in digit swapped order also do the criss cross from different memories %%%%%%%%%%%%%%
			crissCrossAndTimeDigitSwap_specialization<tp_L,tp_R>(inDatac, pingPongBuff);

			///// %%%%%%%%%%%%%%% Process:2:Create 2nd process that connect to process-1 through PIPO and generates an output stream %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			diagonalWrapingIndexReadfromPIPOAndStreamOutput_specialization<tp_L,tp_R>(pingPongBuff,outDatac);
		}

#if 0
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////START
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////

		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void digitReversal2Phase_specialization2(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
		/*
		#pragma HLS ARRAY_PARTITION variable=inDatac complete dim=1
		*/

			tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R];
		#pragma HLS ARRAY_PARTITION variable=pingPongBuff complete dim=1
		#pragma HLS DATA_PACK variable=pingPongBuff

			unsigned int inputTimeIndex;
			unsigned int outputTimeIndex;
			const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
			const unsigned int minOf_R_and_L_over_R = (tp_R < (tp_L/tp_R) ) ? tp_R : tp_L/tp_R;
		#ifndef __SYNTHESIS__
			std::cout<<"\n\n\n\n The Folding Factor is :::::"<<F<<std::endl;
		#endif
			///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////

			for (unsigned int t = 0; t < tp_L/tp_R; t++)
			{
				inputTimeIndex  = t;
				outputTimeIndex = digitReversal_m<tp_L/tp_R,tp_R>(inputTimeIndex);
				unsigned int InputShuffleAmount = (t*F) % tp_R;//   minOf_R_and_L_over_R; // can be converted to masking
				tt_dtype sampleShufflingBuffer[tp_R];
		#pragma HLS DATA_PACK variable=sampleShufflingBuffer

				for (unsigned int r = 0; r < tp_R; r++)
				{
		#pragma HLS UNROLL
					sampleShufflingBuffer[(r + InputShuffleAmount) % tp_R] = inDatac[r][inputTimeIndex];
					//sampleShufflingBuffer[(outputTimeIndex) % tp_R] = inDatac[inputTimeIndex][r];

				}
				//DEBUG_PRINT_VAL(outputTimeIndex);

				for (unsigned int r = 0; r < tp_R; r++)
				{
		#pragma HLS UNROLL
					pingPongBuff[r][outputTimeIndex] = sampleShufflingBuffer[r]; 	// digit reversal write to the memory
		#if	DEBUG_SSR_FFT
				//	std::cout<<"sampleShufflingBuffer[r]:"<<sampleShufflingBuffer[r]<<"\n";
					//DEBUG_PRINT_VAL(sampleShufflingBuffer[])
		#endif
				}
			}


		#ifndef __SYNTHESIS__
			std::cout<<"The shuffeled and buffered Data.....\n";
		   ///////////////////////////////////// //////////////////////////////////// ////////////////////////////////////
			for (int i = 0;i <tp_R;i++)
											{
												for (int j = 0;j <tp_L / tp_R  ;j++)
												{
													//ind1 = j*(SSR_FFT_R)+i;
													//origArray[j][i]=ind1;
													std::cout<<pingPongBuff[i][j];
													if(j!=(tp_L / tp_R -1))
													std::cout<<" ";
												}
												std::cout<<"\n";

											}

		   ///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////
			std::cout<<"The shuffeled and buffered Data.....\n";
		#endif

			unsigned int pingPongTimeIndex;
			unsigned int pingPongSuperSampleIndex;
			const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;
			for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
					outputTimeIndex = digitReversal_m<tp_L/tp_R,tp_R>(inputTimeIndex);
					for (unsigned int r = 0; r < tp_R; r++)
					{
						ap_uint<  ssr_fft_log2<tp_R>::val > rotated_r = r;
						rotated_r.lrotate(outputShuffleAmount);

						pingPongSuperSampleIndex = ( rotated_r + F*t ) % tp_R ;
						pingPongTimeIndex        = ( tp_R * (t / tp_R) + r ) % (tp_L/tp_R);
						//pingPongTimeIndex        = ( (tp_L/tp_R) * (t / (tp_L/tp_R)) + r ) % (tp_L/tp_R);


						//DEBUG_PRINT_VAL(pingPongTimeIndex);
						//  DEBUG_PRINT_VAL(pingPongSuperSampleIndex);
						//   DEBUG_PRINT_VAL(pingPongTimeIndex);

						outDatac[r][t] = pingPongBuff[pingPongSuperSampleIndex][pingPongTimeIndex];
					}
				}

		}



		template <int tp_L, int tp_R,typename tt_dtype, typename tt_dtype2>
		void digitReversal2Phase2(tt_dtype inDatac[tp_R][tp_L/tp_R], tt_dtype2 outDatac[tp_R][tp_L/tp_R])
		{
			tt_dtype2 pingPongBuff[tp_R][tp_L/tp_R];
		#pragma HLS ARRAY_PARTITION variable=pingPongBuff complete dim=1
		#pragma HLS DATA_PACK variable=pingPongBuff
		/*
		#pragma HLS ARRAY_PARTITION variable=inDatac complete dim=1
		#pragma HLS ARRAY_PARTITION variable=outDatac complete dim=1

		*/
			unsigned int inputTimeIndex;
			unsigned int outputTimeIndex;
			const unsigned int F = ((tp_R*tp_R)/tp_L) > 1 ? ((tp_R*tp_R)/tp_L):1;
			const unsigned int minOf_R_and_L_over_R = (tp_R < (tp_L/tp_R) ) ? tp_R : tp_L/tp_R;
		#ifndef __SYNTHESIS__
			std::cout<<"\n\n\n\n The Folding Factor is :::::"<<F<<std::endl;
		#endif
			///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////

			for (unsigned int t = 0; t < tp_L/tp_R; t++)
			{
				inputTimeIndex  = t;
				outputTimeIndex = digitReversal_fracIsLSB<tp_L/tp_R,tp_R>(inputTimeIndex);
				unsigned int InputShuffleAmount = (t*F) % tp_R;//   minOf_R_and_L_over_R; // can be converted to masking
				tt_dtype sampleShufflingBuffer[tp_R];
		#pragma HLS DATA_PACK variable=sampleShufflingBuffer

				for (unsigned int r = 0; r < tp_R; r++)
				{
		#pragma HLS UNROLL
					//sampleShufflingBuffer[(r + InputShuffleAmount) % tp_R] = inDatac[inputTimeIndex][r];
					sampleShufflingBuffer[(r + outputTimeIndex) % tp_R] = inDatac[r][inputTimeIndex];
				}
				//DEBUG_PRINT_VAL(outputTimeIndex);

				for (unsigned int r = 0; r < tp_R; r++)
				{
		#pragma HLS UNROLL
					pingPongBuff[r][outputTimeIndex] = sampleShufflingBuffer[r]; 	// digit reversal write to the memory
		#ifndef __SYNTHESIS__

				//	std::cout<<"sampleShufflingBuffer[r]:"<<sampleShufflingBuffer[r]<<"\n";
					//DEBUG_PRINT_VAL(sampleShufflingBuffer[])
		#endif
				}
			}
		#ifndef __SYNTHESIS__

			std::cout<<"The shuffeled and buffered Data.....\n";
		   ///////////////////////////////////// //////////////////////////////////// ////////////////////////////////////
			for (int i = 0;i <tp_R;i++)
											{
												for (int j = 0;j <tp_L / tp_R  ;j++)
												{
													//ind1 = j*(SSR_FFT_R)+i;
													//origArray[j][i]=ind1;
													std::cout<<pingPongBuff[i][j];
													if(j!=(tp_L / tp_R -1))
													std::cout<<" ";
												}
												std::cout<<"\n";

											}

		   ///////////////////////////////////// Shuffle the Data and Write to Buffer ////////////////////////////////////
			std::cout<<"The shuffeled and buffered Data.....\n";
		#endif
			unsigned int pingPongTimeIndex;
			unsigned int pingPongSuperSampleIndex;
			const unsigned int outputShuffleAmount = ssr_fft_log2<F>::val;
			for (unsigned int t = 0; t < tp_L/tp_R; t++)
				{
					outputTimeIndex = digitReversal_m<tp_L/tp_R,tp_R>(inputTimeIndex);
					for (unsigned int r = 0; r < tp_R; r++)
					{
						//ap_uint<  ssr_fft_log2<tp_R>::val > rotated_r = r;
						//rotated_r.lrotate(outputShuffleAmount);

						//pingPongSuperSampleIndex = ( rotated_r + F*t ) % tp_R ;

						pingPongSuperSampleIndex = ( r + t /(  tp_L/(tp_R*tp_R))  ) % tp_R ;

					   // pingPongTimeIndex        = ( tp_R * (t / tp_R) + r ) % (tp_L/tp_R);
						pingPongTimeIndex        = r + (t % (tp_L/(tp_R*tp_R)))*tp_R;       //( (tp_L/tp_R) * (t / (tp_L/tp_R)) + r ) % (tp_L/tp_R);


						//DEBUG_PRINT_VAL(pingPongTimeIndex);
						DEBUG_PRINT_VAL(pingPongSuperSampleIndex);
						//   DEBUG_PRINT_VAL(pingPongTimeIndex);

						outDatac[r][t] = pingPongBuff[pingPongSuperSampleIndex][pingPongTimeIndex];
					}
				}

		}


		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////END
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
		//////////////////////////////// Simulation Versions :: The ones used before creating synthesizale version //////////////////////////////////////////////
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFT_OUTPUT_REORDER_h_
