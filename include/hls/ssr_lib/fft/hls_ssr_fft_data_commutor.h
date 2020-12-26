
#define __USE_ALAINS_MODIFICATION_DATACOMMUTOR__
#ifdef __USE_ALAINS_MODIFICATION_DATACOMMUTOR__

//File Name : hls_ssr_fft_data_commutor.h
#ifndef HLS_SSR_FFR_DATA_COMMUTOR_H_
#define HLS_SSR_FFR_DATA_COMMUTOR_H_

/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

	The data commuter is a basic block used to implement data re ordering
     at the input SSR FFT and also in between the SSR FFT Stages. The data commuter
     has two stages one read in R streams multiplexing them before storage to ping
     pong buffers in a circular rotate(word level rotation not bit like) fashion.
     The 2nd stage reads in R memories and streams it out to R different streams.
     The memory to stream mapping changes in every cycle. The Whole transformation
     is 4 phase:
     	 	 	 1- The input streams are rotated
     	 	 	 2- The input stream written to PIPO after rotation
     	 	 	 3- The ping pong memory is read
     	 	 	 4- The read data is shuffled and written to output
                    This file defines functions for phases 1,2,3
                    cacheData function : deals with phase 1 and 2
                    WriteCacheData  function deals with phase  3,4
                    and internally calls
                    commuteBarrelShifter::memReadCommuteBarrelShifter

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

#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_fork_merge_utils.h"
#include "hls_ssr_fft_data_commute_barrel_shifter.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_fork_merge_utils.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_data_commute_barrel_shifter.h"

#endif


#ifndef SSR_FFT_URAM_SELECTION_THRESHHOLD
#define SSR_FFT_URAM_SELECTION_THRESHHOLD 32   // Memories larger then threshold will be implemented using URAMs
#endif //SSR_FFT_URAM_SELECTION_THRESHHOLD

#ifndef SSR_FFT_USE_URAMS
#define SSR_FFT_USE_URAMS 0
#endif //SSR_FFT_USE_URAMS

namespace hls
{
namespace ssr_fft
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct write_out_wrapper
		{
		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void write_out( tt_T temp[tp_R], tt_T out[tp_R][tp_L/tp_R])
			{
			#pragma HLS PIPELINE
			#pragma HLS INLINE off

				static int f = 0;
				static int n = 0;
				write_out_SSRLoop:
				for (int s = 0; s < tp_R;s++)
				{

				#pragma HLS UNROLL
					int  p = n % tp_PF;
					int r1 = n / tp_PF;
					//replaced//int out1_index = (f*(tp_PF*tp_R)) + r1*tp_PF + p;
					int out1_index = ( f<<(ssr_fft_log2<(tp_PF*tp_R)>::val)  ) + (r1<<(ssr_fft_log2<tp_PF>::val)) + p;
					int out2_index =s;
					//int cshift = (s*tp_PF + p)/tp_PF;
					out[s][out1_index]=temp[s];

				}
				n++;
				if (n==tp_PF*tp_R)
				{
					n=0;
					f++;
					if (f == tp_L/(tp_PF*tp_R*tp_R))
					{
						f = 0;
					}
				}

			}
		};

		template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct write_out_pf1_wrapper
		{
			template <int XF,int tp_L, int tp_R,int tp_PF, typename tt_T>
			 void write_out_pf1( tt_T temp[tp_R], tt_T out[tp_R][tp_L/tp_R])
			{
			#pragma HLS PIPELINE
			#pragma HLS INLINE off
                static int test=0;

				static int f = 0;
				static int n = 0;

				write_out_SSRLoop:
				for (int s = 0; s < tp_R;s++)
				{
					//int  p = 0;
						  int r1 = n / tp_PF;
				#pragma HLS UNROLL
					int out1_index = test;//(f*(tp_PF*tp_R)) + r1*tp_PF;
					int out2_index =s;
					//int cshift = (s*tp_PF + p)/tp_PF;
					out[s][out1_index]=temp[s];

				}

				n++;
				if (n==tp_PF*tp_R)
				{
					n=0;
					f++;
					if (f == tp_L/(tp_PF*tp_R*tp_R*XF))
					{
						f = 0;
					}
				}

				if(test == (tp_L/tp_R  - 1))
					test=0;
				else
					test++;

			}
		};
		template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct __barrelShiftWrapper__wrapper
		{
			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			 void __barrelShiftWrapper__(tt_T buff[tp_R][tp_PF*tp_R], tt_T temp[tp_R])
			{
			#pragma HLS PIPELINE
				static int n=0;
				int  p = n % tp_PF;
				int r1 =n / tp_PF; //n>>(ssr_fft_log2<tp_PF>::val);
				commuteBarrelShifter <tp_R> commuteBarrelShifterObj;
				commuteBarrelShifterObj. template memReadCommuteBarrelShifter< tp_R,tp_L, tp_PF,tt_T>( ( (r1+(p)/tp_PF)%tp_R ) ,p,buff,temp);
				n++;
				if (n==tp_PF*tp_R)
					n=0;


			}
		};
		template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct __barrelShiftWrapper__pf1_wrapper
		{
			template <int XF,int tp_L, int tp_R,int tp_PF, typename tt_T>
			 void __barrelShiftWrapper__pf1(tt_T buff[tp_R][XF*tp_PF*tp_R], tt_T temp[tp_R])
			{
			#pragma HLS PIPELINE
				static int n=0;

			 // int  p = n % tp_PF;
			  int r1 =n; // n / tp_PF;
			#pragma HLS INLINE off
				commuteBarrelShifter_pf1 <tp_R> commuteBarrelShifterObj;
				commuteBarrelShifterObj. template memReadCommuteBarrelShifter_pf1<XF, tp_R,tp_L, tp_PF,tt_T>( r1,0,buff,temp);

				n++;
				if (n==tp_PF*tp_R*XF)
					n=0;


			}
		};
		template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct CacheData_wrapper
		{
			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void CacheData(tt_T in[tp_R][tp_L/tp_R], tt_T buff[tp_R][tp_PF*tp_R])
			{
				static int f = 0;

			#pragma HLS INLINE off
				CacheData_BuffLoop:
				for (int rw = 0; rw<tp_PF*tp_R; rw++)
				{

					#pragma HLS PIPELINE II=1 rewind
					tt_T temp[tp_R];
			#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
			#pragma HLS DATA_PACK variable=temp
					CacheData_SSRLoop1:
					for (int s = 0; s < tp_R;s++)
					{
						//replaced//int cshift= (rw+f*(tp_PF*tp_R))/tp_PF;
						int cshift= ( rw+ (f<<(ssr_fft_log2<(tp_PF*tp_R)>::val)) )>>(ssr_fft_log2<tp_PF>::val);

						//replaced//temp[(s+cshift)%tp_R]=in[s][rw+f*(tp_PF*tp_R)];
						temp[(s+cshift)&(ssr_fft_log2BitwiseAndModMask<tp_R>::val)]=in[s][rw + ( f<<ssr_fft_log2<(tp_PF*tp_R)>::val )    ];
						//CHECK_COVEARAGE;

					}
					CacheData_SSRLoop2:
					for (int s = 0; s < tp_R;s++)
					{
						buff[s][rw]=temp[s];
					}
					if (rw == tp_PF*tp_R-1)
					{
						f++;
						if (f == tp_L/(tp_PF*tp_R*tp_R))
						{
							f = 0;
						}
					}
				}
			}
		};

		template < int stage, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct writeCachedData_wrapper
		{
			template <int tp_L, int tp_R,int tp_PF, typename tt_T>
			void writeCachedData(tt_T buff[tp_R][tp_PF*tp_R], tt_T out[tp_R][tp_L/tp_R])
			{
			#pragma HLS INLINE off


				__barrelShiftWrapper__wrapper<stage,id> __barrelShiftWrapper__wrapper_obj;
				write_out_wrapper<stage,id> write_out_wrapper_obj;
				dataCommutorReOrder_radix_x_pf_LOOP:
				for (int r1 = 0; r1<(tp_R*tp_PF); r1++)
				{
					//#pragma HLS dataflow
					#pragma HLS PIPELINE II=1
					//pf_loop:
					//for (int p = 0; p<tp_PF; p++)
					//{
			//#pragma HLS loop_flatten
			//#pragma HLS PIPELINE II=1 //rewind    /// This loop has rewinding issues.: VERIFIED

						tt_T temp[tp_R];
						//tt_T temp2[tp_R];
			#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
			#pragma HLS DATA_PACK variable=temp


						for (int c = 0; c < tp_R; c++)
						{
						#pragma HLS UNROLL

							unsigned int out_r = ((tp_R+c-(r1/tp_PF))%tp_R)*tp_PF + (r1%tp_PF) ;   // equivalent to :  bitReversedIndex / tp_R;
							temp[(tp_R+c-(r1/tp_PF))%tp_R]= buff[c][out_r];

						}

						write_out_wrapper_obj.template write_out<tp_L,tp_R,tp_PF,tt_T>(temp,out);

					//}
				} // re_order_loop;
			}
		};

		template < int stage_no, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct CacheData_pf1_wrapper
		{
			template <int XF,int tp_L, int tp_R,int tp_PF, typename tt_T>
			void CacheData_pf1(tt_T in[tp_R][tp_L/tp_R], tt_T buff[tp_R][XF*tp_R])
			{
			#pragma HLS INLINE off

				static int f = 0;
				static int test=0;
				CacheData_loop_pf1:
					for (int rw = 0; rw<(tp_R*XF); rw++)
					{
			#pragma HLS PIPELINE II=1 rewind

					tt_T temp[tp_R];
			#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
			#pragma HLS DATA_PACK variable=temp


						for (int s = 0; s < tp_R;s++)
						{
						#pragma HLS UNROLL
							//replaced//temp[(s+rw)%tp_R]=in[s][rw+f*(tp_R)];
							temp[(s+rw)%tp_R]=in[s][test];   //in[s][(rw%tp_R)+((rw/tp_R)*tp_R)+ f*(tp_R)];
							//temp[(s+rw)%tp_R]=in[rw+f*(XF*tp_R)+(rw%tp_R)][s];
							//temp[(s+rw) & (ssr_fft_log2BitwiseAndModMask<XF*tp_R>::val)]=in[s][rw + ( f<<(ssr_fft_log2<tp_R>::val) )];
							//CHECK_COVEARAGE;
						}
						for (int s = 0; s < tp_R;s++)
						{
						#pragma HLS UNROLL
							buff[s][rw]=temp[s];
						}
						if (rw == tp_L*tp_R-1)
						{
							f++;
							if (f == tp_L/(tp_PF*tp_R*tp_R*XF))
							{
								f = 0;
							}
						}

						if(test== (tp_L/tp_R-1))
							test=0;
						else
							test++;
					}


			}
		};


		template < int stage, int id > // wrapper is created to have a unique copy of write_out whenever it is called with different id and stage such that static variables don't get shared.
		struct writeCachedData_pf1_wrapper
		{
			template <int XF,int tp_L, int tp_R,int tp_PF, typename tt_T>
			void writeCachedData_pf1(tt_T buff[tp_R][tp_R*XF], tt_T out[tp_R][tp_L/tp_R])
			{
	#pragma HLS dataflow
	#pragma HLS INLINE off


				__barrelShiftWrapper__pf1_wrapper<stage,id> __barrelShiftWrapper__pf1_wrapper_obj;
				write_out_pf1_wrapper<stage,id> write_out_pf1_wrapper_obj;
				writeCachedData_pf1_loop:
				for (int r1d = 0; r1d<XF*tp_R; r1d++)
				{
				#pragma HLS PIPELINE II=1 //rewind  /// This loop has rewinding issues : VERIFIED

				//#pragma HLS dataflow

						int r1 = r1d % tp_R;
						int offset = (r1d / tp_R) * tp_R;
						tt_T temp[tp_R];

			#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
			#pragma HLS DATA_PACK variable=temp

						//__barrelShiftWrapper__pf1_wrapper_obj.template __barrelShiftWrapper__pf1<XF,tp_L,tp_R,tp_PF,tt_T>(buff,temp);
						for (int c = 0; c < tp_R; c++)
						{
						#pragma HLS UNROLL

							unsigned int out_r = ((tp_R+c-(r1/tp_PF))%tp_R)*tp_PF + (r1%tp_PF) + offset ;   // equivalent to :  bitReversedIndex / tp_R;
							temp[(tp_R+c-(r1/tp_PF))%tp_R]= buff[c][out_r];

						}

						write_out_pf1_wrapper_obj.template write_out_pf1<XF,tp_L,tp_R,tp_PF,tt_T>(temp,out);
				} // re_order_loop;
			}
		};

		template <int stage,int id,int tp_PF, int tp_isLargeMemory>
		struct dataCommutations {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};
		template <int stage,int id,int tp_PF>
		struct dataCommutations<stage,id,tp_PF,1> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};

		template <int stage,int id,int tp_PF>
		struct dataCommutations<stage,id,tp_PF,0> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};
		template <int stage,int id>
		struct dataCommutations<stage,id,1,0> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};

		template <int stage,int id>
		struct dataCommutations<stage,id,1,1> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};


		/*===================================================================================================================
		 * tp_isLargeMemory=false  and Generic Packing Factor:: This specialization will be used for the case when the buffer
		 * ping pong memory is small and can be implemented using BRAMs are registers.
		 * ==================================================================================================================
		 */
		template <int stage,int id,int tp_PF>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<stage,id,tp_PF,0>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{

#pragma HLS DATAFLOW
#pragma HLS INLINE off

			//	assert( (tp_L%(tp_PF*tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be multiple of Radix
			CacheData_wrapper<stage,id> CacheData_wrapper_obj;
			writeCachedData_wrapper<stage,id> writeCachedData_wrapper_obj;
				dataCommutorBuffFrameLoop:
				for (int f = 0; f < (tp_L/(tp_PF*tp_R*tp_R)); f++)
				{

		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_PF*tp_R];
					//#pragma HLS STREAM variable=buff dim=2

					//#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS DATA_PACK variable=buff
		#pragma HLS ARRAY_PARTITION variable=buff complete dim=1


					CacheData_wrapper_obj.template CacheData<tp_L,tp_R,tp_PF,tt_T>(in,buff);
					writeCachedData_wrapper_obj.template writeCachedData<tp_L,tp_R,tp_PF,tt_T>(buff,out);
				}
		}
		/*===================================================================================================================
		 * tp_isLargeMemory=false  and Packing Factor=1:: This specialization will be used for the case when the buffer
		 * ping pong memory is SMALL and can be implemented using BRAMS or registers.
		 * ==================================================================================================================
		 */
		template <int stage,int id>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<stage,id,1,0>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{

			#pragma HLS DATAFLOW
			#pragma HLS INLINE off
			//	assert( (tp_L%(tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be power of Radix

				assert( (tp_L)==(ssr_fft_pow< 2, ssr_fft_log2<tp_L>::val >::val) ); // Length should be power of 2 always

				//assert((ssr_fft_log2<tp_L>::val % ssr_fft_log2<tp_R>::val)==0); // length should be power of radix also
				 // Length should be multiple of Radix
				CacheData_pf1_wrapper<stage,id> CacheData_wrapper_pf1_obj;
				writeCachedData_pf1_wrapper<stage,id> writeCachedData_wrapper_pf1_obj;
				const int XF = (tp_L > tp_R*tp_R)? (tp_L/(tp_R*tp_R)> 2 ? 4: tp_L/(tp_R*tp_R)) : 1;
				// This factor XF makes insides buffers XF times the size of minimum required but improves
			    // throughput : from ( L/R  * (1+  3/R)  ) to : ( L/R  * (1+  3/(R*XF))  ) only when ( R*R > L )
				dataCommutorBuffFrameLoop_PF1:
				for (int f = 0; f < (tp_L/(tp_R*tp_R*XF)); f++)
				{
		//#pragma HLS PIPELINE rewind   // This loop rewinds has an issue : loop rewind fails
		#pragma HLS DATAFLOW
					tt_T buff[tp_R][XF*tp_R];
					#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
					#pragma HLS DATA_PACK variable=buff

					CacheData_wrapper_pf1_obj.template CacheData_pf1<XF,tp_L,tp_R,1,tt_T>(in,buff);

					writeCachedData_wrapper_pf1_obj.template writeCachedData_pf1<XF,tp_L,tp_R,1,tt_T>(buff,out);
					//writeCachedData<tp_L,tp_R,1,tt_T>(buff,out);

				}
		}

		/*===================================================================================================================
		 * tp_isLargeMemory=TRUE  and Generic Packing Factor:: This specialization will be used for the case when the buffer
		 * ping pong memory is LARGEG and can be implemented using URAMS.
		 * ==================================================================================================================
		 */
		template <int stage,int id,int tp_PF>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<stage,id,tp_PF,1>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{

		#pragma HLS DATAFLOW
		#pragma HLS INLINE off


			//	assert( (tp_L%(tp_PF*tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be multiple of Radix
				CacheData_wrapper<stage,id> CacheData_wrapper_obj;
				writeCachedData_wrapper<stage,id> writeCachedData_wrapper_obj;
				dataCommutorBuffFrameLoop:
				for (int f = 0; f < (tp_L/(tp_PF*tp_R*tp_R)); f++)
				{

		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_PF*tp_R];
					//#pragma HLS STREAM variable=buff dim=2

					//#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS DATA_PACK variable=buff
		#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS RESOURCE variable=buff core=XPM_MEMORY uram

					CacheData_wrapper_obj.template CacheData<tp_L,tp_R,tp_PF,tt_T>(in,buff);
					writeCachedData_wrapper_obj.template writeCachedData<tp_L,tp_R,tp_PF,tt_T>(buff,out);
				}
		}
		/*===================================================================================================================
		 * tp_isLargeMemory=TRUE  and Packing Factor=1:: This specialization will be used for the case when the buffer
		 * ping pong memory is SMALL and can be implemented using URAMS.
		 * ==================================================================================================================
		 */
		template <int stage,int id>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<stage,id,1,1>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
	#pragma HLS DATAFLOW
	#pragma HLS INLINE off

			//	assert( (tp_L%(tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be power of Radix

				assert( (tp_L)==(ssr_fft_pow< 2, ssr_fft_log2<tp_L>::val >::val) ); // Length should be power of 2 always

				//assert((ssr_fft_log2<tp_L>::val % ssr_fft_log2<tp_R>::val)==0); // length should be power of radix also

				CacheData_pf1_wrapper<stage,id> CacheData_wrapper_pf1_obj;
				writeCachedData_pf1_wrapper<stage,id> writeCachedData_wrapper_pf1_obj;
				dataCommutorBuffFrameLoop_PF1:
				for (int f = 0; f < (tp_L/(tp_R*tp_R)); f++)
				{
		//#pragma HLS PIPELINE rewind   // This loop rewinds has an issue : loop rewind fails
		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_R];
					#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
					#pragma HLS DATA_PACK variable=buff
		#pragma HLS RESOURCE variable=buff core=XPM_MEMORY uram

					CacheData_wrapper_pf1_obj.template CacheData_pf1<tp_L,tp_R,1,tt_T>(in,buff);
					writeCachedData_wrapper_pf1_obj.template writeCachedData_pf1<tp_L,tp_R,1,tt_T>(buff,out);
				}
		}
		/* DataCommutorFork : This class will take a [L/R][R] type stream with R sample stream and
		 * break it down to R/F streams creating F new streams. Functionally it will take [L/R][R] 2 dimensional array
		 * and break it down to F new 2 dimensional arrays of size [L/R][R/F] to be used by F dataflow processes
		*/

		template<int stage,int tp_forkNumber, int tp_forkingFactor>
		struct DataCommutorFork {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		template<int stage,int tp_forkingFactor>
		struct DataCommutorFork<stage,1,tp_forkingFactor> {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		// Important Comments :::
		/* Forking Function: Copies data tp_R/Forking Factor Buffers for forking out the output
		* The input is [tp_L][tp_R] array and output is a also [tp_L][tp_R] array , but internally
		* The input array is split into smaller arrays like : [tp_L][tp_R/forking_factor] to create
		* tp_R/forking factor input arrays, each of these arrays is assumed to be a seperate function
		* and finally in hardware every such function will map to a seperate process. Essentially it will
		* create a fork, once process feeding multiple processes;
		*
		*
		*/

		/*
												   :ossooooss+.
												 `oy/`        .oh/
										   .+::/+ds/::::::::::::/yd/::::::::/+o::::::::::::://
										   -/`--mo----------------hy---------/:-------------`s
										   -/:/:m`````````````````.M`````````..````````````+-s
										   -/:/.N    PROCESS-1.1  .N                       +-s
					   -+ssoos+-           -/:/ ys               `h/                       +-s
					`oy+.     ./ys.        -/:/  oy.   `  ``    :h/                        +-s
				   +h-           .ys       -/:/   .sy+-`    `:oy+`                         +-s
				  yo               /d`     -/:/      ./oooooo:`                            +-s
				 h+                 :d`    -/:/                                            +-s
				oy    PROCESS-1      +y    -/:/                                            +-+:::::::::::::.
			   `N`                    m:   -/:/                                            :/::::::::::::::
		`::::::yh:::::::::::::::::::::yd:::+::/                                            ``````````````
		`//////ms/////////////////////+N//////-                                           .s-------------
		`::::::N+:::::::::::::::::::::/M/::::+-                                           .+/::::::::::::::.
		`::::::N+:::::::::::::::::::::/M/:/. ::                                           .+s
			   h/                     .N  :: ::                                           .+s
			   +y                     +y  :: ::                                           .+s
			   `N.                    m-  :: ::           ./ooooo+:`                      .+s
				+h                   oy   :: ::        -sy+--.``..:oy+`                   .+s
				 yo                 /d`   :: ::      `ys`   :..-:.   :d:                  .+s
				  ss               +h`    :: :o::::::dy:::::::::::::::/N+::++-::::::::::::/+s
				   /h:           -h+      :/  ``````/d`````````````````:m``..```````````````s
					 +yo-`    -+yo`       `:::::::::sd:::::::::::::::::+N:://:::::::::::::::.
					   ./ooooo/.                    .m.   PROCESS-1.2  ss
													 :d-              sy
													  `sy:`        .oh/
														`/sssooooss+.

		 */

		template<int stage,int tp_forkNumber, int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork<stage,tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredInputBuff core=FIFO_LUTRAM


#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredInputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#endif

			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredOutputBuff core=FIFO_LUTRAM


		//#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredOutputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#endif


			/*
			 * Function Signature :
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *
			 *			void copyToLocalBuff(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_L/tp_R][tp_R/tp_forkingFactor])
			 */



			copyToLocalBuff<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);


			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<stage,tp_forkNumber,tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);



			/* Function Signature:
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *				void copyFromLocalBuffToOuput(tt_T in[tp_L/tp_R][tp_R/tp_forkingFactor],tt_T out[tp_R][tp_L/tp_R])
			 */
			copyFromLocalBuffToOuput<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);



			 // Create a New Fork to Commute next factor of the output
			/* Function Signature:
			 * 	template<int tp_forkNumber, int tp_forkingFactor>
				template<int tp_PF,int tp_L, int tp_R,typename tt_T>
				void DataCommutorFork<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
			 */
			DataCommutorFork<stage,tp_forkNumber-1,tp_forkingFactor> DataCommutorFork_obj;
			DataCommutorFork_obj.template copyForkCommuteAndMerge<tp_PF,tp_L,tp_R,tt_T>(in,out);
		}



		// copyForkCommuteAndMerge base case specialization for fork number = 1, terminates forking/recursion
		template<int stage,int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork<stage,1,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];


			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredInputBuff core=FIFO_LUTRAM

		//#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1


#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredInputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#endif

			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredOutputBuff core=FIFO_LUTRAM


		//#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredOutputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#endif

			copyToLocalBuff<tp_L,tp_R,1,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);

			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<stage,1,tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);
			copyFromLocalBuffToOuput<tp_L,tp_R,1,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);
		}




		/* DataCommutorFork : This class will take a [L/R][R] type stream with R sample stream and
		 * break it down to R/F streams creating F new streams. Functionally it will take [L/R][R] 2 dimensional array
		 * and break it down to F new 2 dimensional arrays of size [L/R][R/F] to be used by F dataflow processes
		*/

		template<int stage,int tp_forkNumber, int tp_forkingFactor>
		struct DataCommutorFork_NI {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		template<int stage,int tp_forkingFactor>
		struct DataCommutorFork_NI<stage,1,tp_forkingFactor> {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		// Important Comments :::
		/* Forking Function: Copies data tp_R/Forking Factor Buffers for forking out the output
		* The input is [tp_L][tp_R] array and output is a also [tp_L][tp_R] array , but internally
		* The input array is split into smaller arrays like : [tp_L][tp_R/forking_factor] to create
		* tp_R/forking factor input arrays, each of these arrays is assumed to be a seperate function
		* and finally in hardware every such function will map to a seperate process. Essentially it will
		* create a fork, once process feeding multiple processes;
		*
		*
		*/

		/*
												   :ossooooss+.
												 `oy/`        .oh/
										   .+::/+ds/::::::::::::/yd/::::::::/+o::::::::::::://
										   -/`--mo----------------hy---------/:-------------`s
										   -/:/:m`````````````````.M`````````..````````````+-s
										   -/:/.N    PROCESS-1.1  .N                       +-s
					   -+ssoos+-           -/:/ ys               `h/                       +-s
					`oy+.     ./ys.        -/:/  oy.   `  ``    :h/                        +-s
				   +h-           .ys       -/:/   .sy+-`    `:oy+`                         +-s
				  yo               /d`     -/:/      ./oooooo:`                            +-s
				 h+                 :d`    -/:/                                            +-s
				oy    PROCESS-1      +y    -/:/                                            +-+:::::::::::::.
			   `N`                    m:   -/:/                                            :/::::::::::::::
		`::::::yh:::::::::::::::::::::yd:::+::/                                            ``````````````
		`//////ms/////////////////////+N//////-                                           .s-------------
		`::::::N+:::::::::::::::::::::/M/::::+-                                           .+/::::::::::::::.
		`::::::N+:::::::::::::::::::::/M/:/. ::                                           .+s
			   h/                     .N  :: ::                                           .+s
			   +y                     +y  :: ::                                           .+s
			   `N.                    m-  :: ::           ./ooooo+:`                      .+s
				+h                   oy   :: ::        -sy+--.``..:oy+`                   .+s
				 yo                 /d`   :: ::      `ys`   :..-:.   :d:                  .+s
				  ss               +h`    :: :o::::::dy:::::::::::::::/N+::++-::::::::::::/+s
				   /h:           -h+      :/  ``````/d`````````````````:m``..```````````````s
					 +yo-`    -+yo`       `:::::::::sd:::::::::::::::::+N:://:::::::::::::::.
					   ./ooooo/.                    .m.   PROCESS-1.2  ss
													 :d-              sy
													  `sy:`        .oh/
														`/sssooooss+.

		 */

		template<int stage,int tp_forkNumber, int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork_NI<stage,tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];

			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredInputBuff core=FIFO_LUTRAM


		//#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredInputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#endif

			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredOutputBuff core=FIFO_LUTRAM



		//#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredOutputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#endif


			/*
			 * Function Signature :
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *
			 *			void copyToLocalBuff(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_L/tp_R][tp_R/tp_forkingFactor])
			 */



			copyToLocalBuff<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);



			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;
			dataCommutations<stage,tp_forkNumber,tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);

			/* Function Signature:
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *				void copyFromLocalBuffToOuput(tt_T in[tp_L/tp_R][tp_R/tp_forkingFactor],tt_T out[tp_R][tp_L/tp_R])
			 */
			copyFromLocalBuffToOuput_NI<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);

			 // Create a New Fork to Commute next factor of the output
			/* Function Signature:
			 * 	template<int tp_forkNumber, int tp_forkingFactor>
				template<int tp_PF,int tp_L, int tp_R,typename tt_T>
				void DataCommutorFork<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
			 */
			DataCommutorFork_NI<stage,tp_forkNumber-1,tp_forkingFactor> DataCommutorFork_obj_NI;
			DataCommutorFork_obj_NI.template copyForkCommuteAndMerge_NI<tp_PF,tp_L,tp_R,tt_T>(in,out);
		}



		// copyForkCommuteAndMerge base case specialization for fork number = 1, terminates forking/recursion
		template<int stage,int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork_NI<stage,1,tp_forkingFactor>::copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];

			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredInputBuff core=FIFO_LUTRAM


		//#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredInputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1
#endif

			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
#pragma HLS RESOURCE variable=localFactoredOutputBuff core=FIFO_LUTRAM



		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#ifdef SSR_FFT_PARTITION_INTERFACE_ARRAYS
		#pragma HLS ARRAY_PARTITION variable=localFactoredOutputBuff complete dim=1
#else
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
#endif
			copyToLocalBuff<tp_L,tp_R,1,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);

			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<stage,1,tp_PF,isLargeMemFlag> commutor;

			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);

			copyFromLocalBuffToOuput_NI<tp_L,tp_R,1,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);

		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} //namespace hls
} //namespace ssr_fft

#endif //HLS_SSR_FFR_DATA_COMMUTOR_H_
#endif //__USE_ALAINS_MODIFICATION_DATACOMMUTOR__









//===============================================================
//===============================================================
#ifndef __USE_ALAINS_MODIFICATION_DATACOMMUTOR__

//File Name : hls_ssr_fft_data_commutor.h
#ifndef HLS_SSR_FFR_DATA_COMMUTOR_H_
#define HLS_SSR_FFR_DATA_COMMUTOR_H_


/*
=========================================================================================
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-                                                                                   -_-
-_-

	The data commuter is a basic block used to implement data re ordering
     at the input SSR FFT and also in between the SSR FFT Stages. The data commuter
     has two stages one read in R streams multiplexing them before storage to ping
     pong buffers in a circular rotate(word level rotation not bit like) fashion.
     The 2nd stage reads in R memories and streams it out to R different streams.
     The memory to stream mapping changes in every cycle. The Whole transformation
     is 4 phase:
     	 	 	 1- The input streams are rotated
     	 	 	 2- The input stream written to PIPO after rotation
     	 	 	 3- The ping pong memory is read
     	 	 	 4- The read data is shuffled and written to output
                    This file defines functions for phases 1,2,3
                    cacheData function : deals with phase 1 and 2
                    WriteCacheData  function deals with phase  3,4
                    and internally calls
                    commuteBarrelShifter::memReadCommuteBarrelShifter

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

#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_fork_merge_utils.h"
#include "hls_ssr_fft_data_commute_barrel_shifter.h"


#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_fork_merge_utils.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_data_commute_barrel_shifter.h"

#endif

#ifndef SSR_FFT_URAM_SELECTION_THRESHHOLD
#define SSR_FFT_URAM_SELECTION_THRESHHOLD 32   // Memories larger then threshold will be implemented using URAMs
#endif //SSR_FFT_URAM_SELECTION_THRESHHOLD

#ifndef SSR_FFT_USE_URAMS
#define SSR_FFT_USE_URAMS 0
#endif //SSR_FFT_USE_URAMS

namespace hls {
	namespace ssr_fft {

		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void write_out(int f, int r1, int p, tt_T temp[tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			write_out_SSRLoop:
			for (int s = 0; s < tp_R;s++)
			{

			#pragma HLS UNROLL
				//replaced//int out1_index = (f*(tp_PF*tp_R)) + r1*tp_PF + p;
				int out1_index = ( f<<(ssr_fft_log2<(tp_PF*tp_R)>::val)  ) + (r1<<(ssr_fft_log2<tp_PF>::val)) + p;
				int out2_index =s;
				int cshift = (s*tp_PF + p)/tp_PF;
				out[s][out1_index]=temp[s];

			}
		}

		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void __barrelShiftWrapper__(int r1, int p,tt_T buff[tp_R][tp_PF*tp_R], tt_T temp[tp_R])
		{
		#pragma HLS INLINE
			commuteBarrelShifter <tp_R> commuteBarrelShifterObj;
			commuteBarrelShifterObj. template memReadCommuteBarrelShifter< tp_R,tp_L, tp_PF,tt_T>( ( (r1+(p)/tp_PF)%tp_R ) ,p,buff,temp);
		}

		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void CacheData(tt_T in[tp_R][tp_L/tp_R], tt_T buff[tp_R][tp_PF*tp_R],int f)
		{
		#pragma HLS INLINE off
			CacheData_BuffLoop:
			for (int rw = 0; rw<tp_PF*tp_R; rw++)
			{

				#pragma HLS PIPELINE II=1 rewind
				tt_T temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp
				CacheData_SSRLoop1:
				for (int s = 0; s < tp_R;s++)
				{
					//replaced//int cshift= (rw+f*(tp_PF*tp_R))/tp_PF;
					int cshift= ( rw+ (f<<(ssr_fft_log2<(tp_PF*tp_R)>::val)) )>>(ssr_fft_log2<tp_PF>::val);

					//replaced//temp[(s+cshift)%tp_R]=in[s][rw+f*(tp_PF*tp_R)];
					temp[(s+cshift)&(ssr_fft_log2BitwiseAndModMask<tp_R>::val)]=in[s][rw + ( f<<ssr_fft_log2<(tp_PF*tp_R)>::val )    ];
					//CHECK_COVEARAGE;

				}
				CacheData_SSRLoop2:
				for (int s = 0; s < tp_R;s++)
				{
					buff[s][rw]=temp[s];
				}
			}
		}

		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void writeCachedData(tt_T buff[tp_R][tp_PF*tp_R], tt_T out[tp_R][tp_L/tp_R],int f)
		{
		#pragma HLS INLINE off
		/*
		#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS ARRAY_PARTITION variable=out complete dim=1
		*/
			dataCommutorReOrder_radixLOOP:
			for (int r1 = 0; r1<tp_R; r1++)
			{

				pf_loop:
				for (int p = 0; p<tp_PF; p++)
				{
		#pragma HLS loop_flatten
		#pragma HLS PIPELINE II=1 //rewind    /// This loop has rewinding issues.: VERIFIED

					tt_T temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp

					__barrelShiftWrapper__<tp_L,tp_R,tp_PF,tt_T>(r1,p,buff,temp);
					write_out<tp_L,tp_R,tp_PF,tt_T>(f,r1,p,temp,out);
				}
			} // re_order_loop;
		}


		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void CacheData_pf1(tt_T in[tp_R][tp_L/tp_R], tt_T buff[tp_R][tp_R*1],int f)
		{
		#pragma HLS INLINE off

		//#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		//#pragma HLS ARRAY_PARTITION variable=in complete dim=1
			CacheData_loop_pf1:
				for (int rw = 0; rw<tp_R; rw++)
				{
		#pragma HLS PIPELINE II=1 rewind

				tt_T temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp


					for (int s = 0; s < tp_R;s++)
					{
					#pragma HLS UNROLL
						//replaced//temp[(s+rw)%tp_R]=in[s][rw+f*(tp_R)];
						temp[(s+rw) & (ssr_fft_log2BitwiseAndModMask<tp_R>::val)]=in[s][rw + ( f<<(ssr_fft_log2<tp_R>::val) )];
						//CHECK_COVEARAGE;
					}
					for (int s = 0; s < tp_R;s++)
					{
					#pragma HLS UNROLL
						buff[s][rw]=temp[s];
					}
				}
		}



		template <int tp_L, int tp_R,int tp_PF, typename tt_T>
		void writeCachedData_pf1(tt_T buff[tp_R][tp_R*1], tt_T out[tp_R][tp_L/tp_R],int f)
		{

		#pragma HLS INLINE off
			writeCachedData_pf1_loop:
			for (int r1 = 0; r1<tp_R; r1++)
			{
			#pragma HLS PIPELINE II=1 //rewind  /// This loop has rewinding issues : VERIFIED


					tt_T temp[tp_R];
		#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
		#pragma HLS DATA_PACK variable=temp


					int offset  = r1;
					commuteBarrelShifter <tp_R> commuteBarrelShifterObj;
					commuteBarrelShifterObj. template memReadCommuteBarrelShifter< tp_R,tp_L, tp_PF,tt_T>(offset,0,buff,temp);

					writeCachedData_pf1_SSRLoop:
					for (int s = 0; s < tp_R;s++)
					{

					#pragma HLS UNROLL
						//replaced//
						//int out1_index = (f*(tp_R)) + r1;
						int out1_index = ( f<< (ssr_fft_log2<tp_R>::val) ) + r1;
					    //	CHECK_COVEARAGE;

						int out2_index =s;
						out[out2_index][out1_index]=temp[s];//buff[s][(r1+s)%tp_R];
					}
			} // re_order_loop;
		}




		template <int tp_PF, int tp_isLargeMemory>
		struct dataCommutations {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};
		template <int tp_PF>
		struct dataCommutations<tp_PF,1> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};

		template <int tp_PF>
		struct dataCommutations<tp_PF,0> {

			template <int tp_L, int tp_R,typename tt_T>
			void dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R]);
		};


		/*===================================================================================================================
		 * tp_isLargeMemory=false  and Generic Packing Factor:: This specialization will be used for the case when the buffer
		 * ping pong memory is small and can be implemented using BRAMs are registers.
		 * ==================================================================================================================
		 */
		template <int tp_PF>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<tp_PF,0>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
		/*
		#pragma HLS INLINE off
		#pragma HLS ARRAY_PARTITION variable=in complete dim=1
		#pragma HLS ARRAY_PARTITION variable=out complete dim=1
		#pragma HLS DATA_PACK variable=in
		#pragma HLS DATA_PACK variable=out
		*/

			//	assert( (tp_L%(tp_PF*tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be multiple of Radix
				dataCommutorBuffFrameLoop:
				for (int f = 0; f < (tp_L/(tp_PF*tp_R*tp_R)); f++)
				{

		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_PF*tp_R];
					//#pragma HLS STREAM variable=buff dim=2

					//#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS DATA_PACK variable=buff
		#pragma HLS ARRAY_PARTITION variable=buff complete dim=1


					CacheData<tp_L,tp_R,tp_PF,tt_T>(in,buff,f);
					writeCachedData<tp_L,tp_R,tp_PF,tt_T>(buff,out,f);
				}
		}
		/*===================================================================================================================
		 * tp_isLargeMemory=false  and Packing Factor=1:: This specialization will be used for the case when the buffer
		 * ping pong memory is SMALL and can be implemented using BRAMS or registers.
		 * ==================================================================================================================
		 */
		template <>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<1,0>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off

		/*
		#pragma HLS ARRAY_PARTITION variable=in complete dim=1
		//#pragma HLS STREAM variable=in dim=2

		#pragma HLS ARRAY_PARTITION variable=out complete dim=1
		//#pragma HLS STREAM variable=out dim=2

		#pragma HLS DATA_PACK variable=in
		#pragma HLS DATA_PACK variable=out
		*/
			//	assert( (tp_L%(tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be power of Radix

				assert( (tp_L)==(ssr_fft_pow< 2, ssr_fft_log2<tp_L>::val >::val) ); // Length should be power of 2 always

				//assert((ssr_fft_log2<tp_L>::val % ssr_fft_log2<tp_R>::val)==0); // length should be power of radix also

				dataCommutorBuffFrameLoop_PF1:
				for (int f = 0; f < (tp_L/(tp_R*tp_R)); f++)
				{
		//#pragma HLS PIPELINE rewind   // This loop rewinds has an issue : loop rewind fails
		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_R];
					#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
					#pragma HLS DATA_PACK variable=buff

					CacheData_pf1<tp_L,tp_R,1,tt_T>(in,buff,f);
					writeCachedData_pf1<tp_L,tp_R,1,tt_T>(buff,out,f);
				}
		}

		/*===================================================================================================================
		 * tp_isLargeMemory=TRUE  and Generic Packing Factor:: This specialization will be used for the case when the buffer
		 * ping pong memory is LARGEG and can be implemented using URAMS.
		 * ==================================================================================================================
		 */
		template <int tp_PF>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<tp_PF,1>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
		/*
		#pragma HLS INLINE off
		#pragma HLS ARRAY_PARTITION variable=in complete dim=1
		#pragma HLS ARRAY_PARTITION variable=out complete dim=1
		#pragma HLS DATA_PACK variable=in
		#pragma HLS DATA_PACK variable=out
		*/

			//	assert( (tp_L%(tp_PF*tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be multiple of Radix
				dataCommutorBuffFrameLoop:
				for (int f = 0; f < (tp_L/(tp_PF*tp_R*tp_R)); f++)
				{

		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_PF*tp_R];
					//#pragma HLS STREAM variable=buff dim=2

					//#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS DATA_PACK variable=buff
		#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
		#pragma HLS RESOURCE variable=buff core=XPM_MEMORY uram



					CacheData<tp_L,tp_R,tp_PF,tt_T>(in,buff,f);
					writeCachedData<tp_L,tp_R,tp_PF,tt_T>(buff,out,f);
				}
		}
		/*===================================================================================================================
		 * tp_isLargeMemory=TRUE  and Packing Factor=1:: This specialization will be used for the case when the buffer
		 * ping pong memory is SMALL and can be implemented using URAMS.
		 * ==================================================================================================================
		 */
		template <>
		template <int tp_L, int tp_R, typename tt_T>
		void dataCommutations<1,1>::dataCommutor (tt_T in[tp_R][tp_L/tp_R], tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE off

		/*
		#pragma HLS ARRAY_PARTITION variable=in complete dim=1
		//#pragma HLS STREAM variable=in dim=2

		#pragma HLS ARRAY_PARTITION variable=out complete dim=1
		//#pragma HLS STREAM variable=out dim=2

		#pragma HLS DATA_PACK variable=in
		#pragma HLS DATA_PACK variable=out
		*/
			//	assert( (tp_L%(tp_R*tp_R)) == 0 ); /// Length of FFT and R not selected properly
														 // Length should be power of Radix

				assert( (tp_L)==(ssr_fft_pow< 2, ssr_fft_log2<tp_L>::val >::val) ); // Length should be power of 2 always

				//assert((ssr_fft_log2<tp_L>::val % ssr_fft_log2<tp_R>::val)==0); // length should be power of radix also

				dataCommutorBuffFrameLoop_PF1:
				for (int f = 0; f < (tp_L/(tp_R*tp_R)); f++)
				{
		//#pragma HLS PIPELINE rewind   // This loop rewinds has an issue : loop rewind fails
		#pragma HLS DATAFLOW
					tt_T buff[tp_R][tp_R];
					#pragma HLS ARRAY_PARTITION variable=buff complete dim=1
					#pragma HLS DATA_PACK variable=buff
		#pragma HLS RESOURCE variable=buff core=XPM_MEMORY uram


					CacheData_pf1<tp_L,tp_R,1,tt_T>(in,buff,f);
					writeCachedData_pf1<tp_L,tp_R,1,tt_T>(buff,out,f);
				}
		}
		/* DataCommutorFork : This class will take a [L/R][R] type stream with R sample stream and
		 * break it down to R/F streams creating F new streams. Functionally it will take [L/R][R] 2 dimensional array
		 * and break it down to F new 2 dimensional arrays of size [L/R][R/F] to be used by F dataflow processes
		*/

		template<int tp_forkNumber, int tp_forkingFactor>
		struct DataCommutorFork {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		template<int tp_forkingFactor>
		struct DataCommutorFork<1,tp_forkingFactor> {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		// Important Comments :::
		/* Forking Function: Copies data tp_R/Forking Factor Buffers for forking out the output
		* The input is [tp_L][tp_R] array and output is a also [tp_L][tp_R] array , but internally
		* The input array is split into smaller arrays like : [tp_L][tp_R/forking_factor] to create
		* tp_R/forking factor input arrays, each of these arrays is assumed to be a seperate function
		* and finally in hardware every such function will map to a seperate process. Essentially it will
		* create a fork, once process feeding multiple processes;
		*
		*
		*/

		/*
												   :ossooooss+.
												 `oy/`        .oh/
										   .+::/+ds/::::::::::::/yd/::::::::/+o::::::::::::://
										   -/`--mo----------------hy---------/:-------------`s
										   -/:/:m`````````````````.M`````````..````````````+-s
										   -/:/.N    PROCESS-1.1  .N                       +-s
					   -+ssoos+-           -/:/ ys               `h/                       +-s
					`oy+.     ./ys.        -/:/  oy.   `  ``    :h/                        +-s
				   +h-           .ys       -/:/   .sy+-`    `:oy+`                         +-s
				  yo               /d`     -/:/      ./oooooo:`                            +-s
				 h+                 :d`    -/:/                                            +-s
				oy    PROCESS-1      +y    -/:/                                            +-+:::::::::::::.
			   `N`                    m:   -/:/                                            :/::::::::::::::
		`::::::yh:::::::::::::::::::::yd:::+::/                                            ``````````````
		`//////ms/////////////////////+N//////-                                           .s-------------
		`::::::N+:::::::::::::::::::::/M/::::+-                                           .+/::::::::::::::.
		`::::::N+:::::::::::::::::::::/M/:/. ::                                           .+s
			   h/                     .N  :: ::                                           .+s
			   +y                     +y  :: ::                                           .+s
			   `N.                    m-  :: ::           ./ooooo+:`                      .+s
				+h                   oy   :: ::        -sy+--.``..:oy+`                   .+s
				 yo                 /d`   :: ::      `ys`   :..-:.   :d:                  .+s
				  ss               +h`    :: :o::::::dy:::::::::::::::/N+::++-::::::::::::/+s
				   /h:           -h+      :/  ``````/d`````````````````:m``..```````````````s
					 +yo-`    -+yo`       `:::::::::sd:::::::::::::::::+N:://:::::::::::::::.
					   ./ooooo/.                    .m.   PROCESS-1.2  ss
													 :d-              sy
													  `sy:`        .oh/
														`/sssooooss+.

		 */

		template<int tp_forkNumber, int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1


			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1


			/*
			 * Function Signature :
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *
			 *			void copyToLocalBuff(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_L/tp_R][tp_R/tp_forkingFactor])
			 */
		#if DEBUG_SSR_FFT
			DEBUG_PRINT_VAL("DataCommutorFork Called with Parameters:");
			DEBUG_PRINT_VAL(tp_L);
			DEBUG_PRINT_VAL(tp_R);
			DEBUG_PRINT_VAL(tp_forkingFactor);
		#endif


			copyToLocalBuff<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);


			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);



			/* Function Signature:
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *				void copyFromLocalBuffToOuput(tt_T in[tp_L/tp_R][tp_R/tp_forkingFactor],tt_T out[tp_R][tp_L/tp_R])
			 */
			copyFromLocalBuffToOuput<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);



			 // Create a New Fork to Commute next factor of the output
			/* Function Signature:
			 * 	template<int tp_forkNumber, int tp_forkingFactor>
				template<int tp_PF,int tp_L, int tp_R,typename tt_T>
				void DataCommutorFork<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
			 */
			DataCommutorFork<tp_forkNumber-1,tp_forkingFactor> DataCommutorFork_obj;
			DataCommutorFork_obj.template copyForkCommuteAndMerge<tp_PF,tp_L,tp_R,tt_T>(in,out);
		}



		// copyForkCommuteAndMerge base case specialization for fork number = 1, terminates forking/recursion
		template<int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork<1,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];


			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1


			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1


			copyToLocalBuff<tp_L,tp_R,1,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);

			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);
			copyFromLocalBuffToOuput<tp_L,tp_R,1,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);
		}




		/* DataCommutorFork : This class will take a [L/R][R] type stream with R sample stream and
		 * break it down to R/F streams creating F new streams. Functionally it will take [L/R][R] 2 dimensional array
		 * and break it down to F new 2 dimensional arrays of size [L/R][R/F] to be used by F dataflow processes
		*/

		template<int tp_forkNumber, int tp_forkingFactor>
		struct DataCommutorFork_NI {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		template<int tp_forkingFactor>
		struct DataCommutorFork_NI<1,tp_forkingFactor> {
			template<int tp_PF,int tp_L, int tp_R,typename tt_T>
			void copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R]);

		};

		// Important Comments :::
		/* Forking Function: Copies data tp_R/Forking Factor Buffers for forking out the output
		* The input is [tp_L][tp_R] array and output is a also [tp_L][tp_R] array , but internally
		* The input array is split into smaller arrays like : [tp_L][tp_R/forking_factor] to create
		* tp_R/forking factor input arrays, each of these arrays is assumed to be a seperate function
		* and finally in hardware every such function will map to a seperate process. Essentially it will
		* create a fork, once process feeding multiple processes;
		*
		*
		*/

		/*
												   :ossooooss+.
												 `oy/`        .oh/
										   .+::/+ds/::::::::::::/yd/::::::::/+o::::::::::::://
										   -/`--mo----------------hy---------/:-------------`s
										   -/:/:m`````````````````.M`````````..````````````+-s
										   -/:/.N    PROCESS-1.1  .N                       +-s
					   -+ssoos+-           -/:/ ys               `h/                       +-s
					`oy+.     ./ys.        -/:/  oy.   `  ``    :h/                        +-s
				   +h-           .ys       -/:/   .sy+-`    `:oy+`                         +-s
				  yo               /d`     -/:/      ./oooooo:`                            +-s
				 h+                 :d`    -/:/                                            +-s
				oy    PROCESS-1      +y    -/:/                                            +-+:::::::::::::.
			   `N`                    m:   -/:/                                            :/::::::::::::::
		`::::::yh:::::::::::::::::::::yd:::+::/                                            ``````````````
		`//////ms/////////////////////+N//////-                                           .s-------------
		`::::::N+:::::::::::::::::::::/M/::::+-                                           .+/::::::::::::::.
		`::::::N+:::::::::::::::::::::/M/:/. ::                                           .+s
			   h/                     .N  :: ::                                           .+s
			   +y                     +y  :: ::                                           .+s
			   `N.                    m-  :: ::           ./ooooo+:`                      .+s
				+h                   oy   :: ::        -sy+--.``..:oy+`                   .+s
				 yo                 /d`   :: ::      `ys`   :..-:.   :d:                  .+s
				  ss               +h`    :: :o::::::dy:::::::::::::::/N+::++-::::::::::::/+s
				   /h:           -h+      :/  ``````/d`````````````````:m``..```````````````s
					 +yo-`    -+yo`       `:::::::::sd:::::::::::::::::+N:://:::::::::::::::.
					   ./ooooo/.                    .m.   PROCESS-1.2  ss
													 :d-              sy
													  `sy:`        .oh/
														`/sssooooss+.

		 */

		template<int tp_forkNumber, int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork_NI<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];

			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1


			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1



			/*
			 * Function Signature :
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *
			 *			void copyToLocalBuff(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_L/tp_R][tp_R/tp_forkingFactor])
			 */
		#if	DEBUG_SSR_FFT

			DEBUG_PRINT_VAL("DataCommutorFork_NI Called with Parameters:");
			DEBUG_PRINT_VAL(tp_L);
			DEBUG_PRINT_VAL(tp_R);
			DEBUG_PRINT_VAL(tp_forkingFactor);
		#endif

		#if	DEBUG_SSR_FFT
			std::cout<<"\n--------------------\n---------------\nThe Input for CB Partitioning ......\n---------------------\n";
			print2DArray<tp_R,tp_L/tp_R>(in);

		#endif

			copyToLocalBuff<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);

		#if	DEBUG_SSR_FFT
			std::cout<<"\n--------------------\n---------------\nThe Input for 1st Prong Partitioned ......\n---------------------\n";
			print2DArray<tp_R/tp_forkingFactor,tp_L/tp_R>(localFactoredInputBuff);
		#endif


			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;
			dataCommutations<tp_PF,isLargeMemFlag> commutor;
			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);

		#if	DEBUG_SSR_FFT
			std::cout<<"\n--------------------\n---------------\nThe Otuput for 1st Prong Partitioned ......\n---------------------\n";
			print2DArray<tp_R/tp_forkingFactor,tp_L/tp_R>(localFactoredOutputBuff);
		#endif
			/* Function Signature:
			 * template <int tp_L, int tp_R,int tp_forkNumber, int tp_forkingFactor, typename tt_T>
			 *				void copyFromLocalBuffToOuput(tt_T in[tp_L/tp_R][tp_R/tp_forkingFactor],tt_T out[tp_R][tp_L/tp_R])
			 */
			copyFromLocalBuffToOuput_NI<tp_L,tp_R,tp_forkNumber,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);
		#if	DEBUG_SSR_FFT
			std::cout<<"\n--------------------\n---------------\nThe output of one Prong Mixed in Final Output. ......\n---------------------\n";
			print2DArray<tp_R,tp_L/tp_R>(out);
		#endif
			 // Create a New Fork to Commute next factor of the output
			/* Function Signature:
			 * 	template<int tp_forkNumber, int tp_forkingFactor>
				template<int tp_PF,int tp_L, int tp_R,typename tt_T>
				void DataCommutorFork<tp_forkNumber,tp_forkingFactor>::copyForkCommuteAndMerge(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
			 */
			DataCommutorFork_NI<tp_forkNumber-1,tp_forkingFactor> DataCommutorFork_obj_NI;
			DataCommutorFork_obj_NI.template copyForkCommuteAndMerge_NI<tp_PF,tp_L,tp_R,tt_T>(in,out);
		}



		// copyForkCommuteAndMerge base case specialization for fork number = 1, terminates forking/recursion
		template<int tp_forkingFactor>
		template<int tp_PF,int tp_L, int tp_R,typename tt_T>
		void DataCommutorFork_NI<1,tp_forkingFactor>::copyForkCommuteAndMerge_NI(tt_T in[tp_R][tp_L/tp_R],tt_T out[tp_R][tp_L/tp_R])
		{
		#pragma HLS INLINE
			//tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
			//tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];

			tt_T localFactoredInputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredInputBuff
		#pragma HLS STREAM variable=localFactoredInputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredInputBuff complete dim=1


			tt_T localFactoredOutputBuff[tp_R/tp_forkingFactor][tp_L/tp_R];
		#pragma HLS DATA_PACK variable=localFactoredOutputBuff
		#pragma HLS STREAM variable=localFactoredOutputBuff depth=2 dim=1
		#pragma HLS ARRAY_RESHAPE variable=localFactoredOutputBuff complete dim=1
			copyToLocalBuff<tp_L,tp_R,1,tp_forkingFactor,tt_T>(in,localFactoredInputBuff);
		#if	DEBUG_SSR_FFT

			DEBUG_PRINT_VAL("DataCommutorFork_NI Base CAESE.................... Called with Parameters:");
			DEBUG_PRINT_VAL(tp_L);
			DEBUG_PRINT_VAL(tp_R);
			DEBUG_PRINT_VAL(tp_forkingFactor);
		#endif
			static const int isLargeMemFlag = (  (tp_PF*(tp_R/tp_forkingFactor) > SSR_FFT_URAM_SELECTION_THRESHHOLD ) && SSR_FFT_USE_URAMS ) ;

			dataCommutations<tp_PF,isLargeMemFlag> commutor;
		#if	DEBUG_SSR_FFT

			std::cout<<"\n--------------------\n---------------\n Last CB Input ..........\n---------------------\n";
			print2DArray<tp_R/tp_forkingFactor,tp_L/tp_R>(localFactoredInputBuff);
		#endif


			commutor.template dataCommutor<tp_L/tp_forkingFactor,tp_R/tp_forkingFactor,tt_T>(localFactoredInputBuff,localFactoredOutputBuff);


		#if	DEBUG_SSR_FFT

			std::cout<<"\n--------------------\n---------------\n Last CB Output*** ..........\n---------------------\n";
					print2DArray<tp_R/tp_forkingFactor,tp_L/tp_R>(localFactoredOutputBuff);

					std::cout<<"\n--------------------\n---------------\n The Final Output BEFORE MIXING *** Mixed UP. ......\n---------------------\n";
							print2DArray<tp_R,tp_L/tp_R>(out);
		#endif

			copyFromLocalBuffToOuput_NI<tp_L,tp_R,1,tp_forkingFactor,tt_T>(localFactoredOutputBuff,out);

		#if	DEBUG_SSR_FFT
			std::cout<<"\n--------------------\n---------------\n The Final Output Mixed UP. ......\n---------------------\n";
			print2DArray<tp_R,tp_L/tp_R>(out);
		#endif

		}

	} //namespace hls
} //namespace ssr_fft


#endif //HLS_SSR_FFR_DATA_COMMUTOR_H_

#endif //__USE_ALAINS_MODIFICATION_DATACOMMUTOR__
