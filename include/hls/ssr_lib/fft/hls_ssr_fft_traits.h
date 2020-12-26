// File Name : hls_ssr_fft_traits.h
#ifndef HLS_SSR_FFT_TRAITS_H_
#define HLS_SSR_FFT_TRAITS_H_
//#include <hls_dsp.h>

#include <ap_fixed.h>
#include <complex>
#ifdef __HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__
//======================================================================
// If source files are used for development of hls ssr fft IP locally
// then set the files path as local.
//=======================================================================
#include "hls_ssr_fft_utilities.h"
#include "hls_ssr_fft_enums.h"
// Include other files that define traits
#include "hls_ssr_fft_exp_table_traits.h"
#include "hls_ssr_fft_twiddle_table_traits.h"
#include "hls_ssr_fft_output_traits.h"
#include "hls_ssr_fft_butterfly_traits.h"

#else //__HLS_SSR_FFT_LOCAL_LIB_DEVLOPMENT_PROJECT__ not defied
//======================================================================
// if the ssr fft source files are to be used in vivado_hls library with
// released version of the tool then set path according to the placement
// of the library
//=======================================================================
#include "hls/ssr_lib/fft/hls_ssr_fft_utilities.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_enums.h"
// Include other files that define traits
#include "hls/ssr_lib/fft/hls_ssr_fft_exp_table_traits.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_twiddle_table_traits.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_output_traits.h"
#include "hls/ssr_lib/fft/hls_ssr_fft_butterfly_traits.h"
#endif



namespace hls {
	namespace ssr_fft {




		template <scaling_mode_enum tp_scaling_mode,int tp_L,int  tp_R, int stage, typename T_twiddleTab, typename T_expTab, typename T_in, typename T_out>
		struct ssr_fft_traits
		{

		};

		/*===================================================================================================================
		 * COMPLEX DOUBLE TYPE TRAITS
		 * For double type the scaling mode has no effect so all the traits stay the same
		 * ==================================================================================================================
		 **/

		template <scaling_mode_enum tp_scaling_mode,int tp_L,int  tp_R, int stage >
		struct ssr_fft_traits<tp_scaling_mode,tp_L,tp_R,stage,std::complex<double>,std::complex<double>,std::complex<double>,std::complex<double> >
		{
			typedef std::complex<double> TTR_stageOutType;
			//typedef std::complex<double> TTR_fftOutType;
			typedef std::complex<double> TTR_stageInType;
			typedef std::complex<double> TTR_twiddleType;
			typedef std::complex<double> TTR_expTabType;
			static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
			static const int tp_log2R = ssr_fft_log2<tp_R>::val;
			static const unsigned int s = NO_OF_FFT_STAGES - stage;

			typedef double TTR_innerStageOutType;
			typedef double TTR_innerFftOutType;
			typedef double TTR_InnerStageInType;
			typedef double TTR_innerTwiddleType;
			typedef double TTR_innerExpTabType;

			//const int nextStage_WL = tp_IWL+tp_log2R;
			//const int nextStage_IL= tp_IIL+tp_log2R;
		};



		/*===================================================================================================================
		 * COMPLEX                  "ap_fixed"                TYPE TRAITS
		 * ==================================================================================================================
		 **/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// (((1))) : Fixed point type with Word length and Integer length specs only


		//////////////////////////Generic Template without any scaling type specification ///////////////////////////////////
		//                               The next three template will be specializations for SCALING MODE

		template <scaling_mode_enum tp_scaling_mode,int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,
				 int expTab_WL, int expTab_IL,
				 int in_WL, int in_IL,
				 int out_WL, int out_IL


				 >
		struct ssr_fft_traits<tp_scaling_mode,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL> >,
								std::complex<ap_fixed<in_WL,in_IL> >,
								std::complex<ap_fixed<out_WL,out_IL> >
								>
		{


				static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
				static const int tp_log2R = ssr_fft_log2<tp_R>::val;
				static const unsigned int s = NO_OF_FFT_STAGES - stage;
				static const int nextStage_WL = in_WL+tp_log2R;
				static const int nextStage_IL= in_IL+tp_log2R;


			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0> > TTR_stageOutType;
			//typedef std::complex<ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0> >  TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0> >  		TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0> >  		TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0> > TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0>  TTR_innerStageOutType;
			typedef ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0>   TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0>   		TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0>   		TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0>  TTR_innerExpTabType;


		};
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((1.1)))

		//Specialization for ap_fixed with SSR_FFT_NO_SCALING : in this mode the ssr fft stage
		// level output will grow by log2(tp_R) For every stage.

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,
				 int expTab_WL, int expTab_IL,
				 int in_WL, int in_IL,
				 int out_WL, int out_IL


				 >
		struct ssr_fft_traits<SSR_FFT_NO_SCALING,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL> >,
								std::complex<ap_fixed<in_WL,in_IL> >,
								std::complex<ap_fixed<out_WL,out_IL> >
								>
		{


				static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
				static const int tp_log2R = ssr_fft_log2<tp_R>::val;
				static const unsigned int s = NO_OF_FFT_STAGES - stage;
				static const int nextStage_WL = in_WL+tp_log2R;
				static const int nextStage_IL= in_IL+tp_log2R;


			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0> > TTR_stageOutType;
			//typedef std::complex<ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0> >  TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0> >  		TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0> >  		TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0> > TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0>  TTR_innerStageOutType;
			typedef ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0>   TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0>   		TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0>   		TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0>  TTR_innerExpTabType;


		};

		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((1.2)))

		//Specialization for ap_fixed with SSR_FFT_SCALE : in this mode the ssr fft stage
		// level output will not grow but output will get scaled, every stage will loose one bit resolution

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,
				 int expTab_WL, int expTab_IL,
				 int in_WL, int in_IL,
				 int out_WL, int out_IL


				 >
		struct ssr_fft_traits<SSR_FFT_SCALE,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL> >,
								std::complex<ap_fixed<in_WL,in_IL> >,
								std::complex<ap_fixed<out_WL,out_IL> >
								>
		{


				static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
				static const int tp_log2R = ssr_fft_log2<tp_R>::val;
				static const unsigned int s = NO_OF_FFT_STAGES - stage;
				static const int nextStage_WL = in_WL;
				static const int nextStage_IL= in_IL+tp_log2R;


			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0> > TTR_stageOutType;
			//typedef std::complex<ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0> >  ;
			typedef std::complex<ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0> >  		TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0> >  		TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0> > TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0>  TTR_innerStageOutType;
			typedef ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0>   TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0>   		TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0>   		TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0>  TTR_innerExpTabType;


		};



		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((1.3)))

		//Specialization for ap_fixed with SSR_FFT_GROW_TO_MAX_WIDTH : in this mode the ssr fft stage
		// level output will grow but output and finally it will saturate to max width decided based
		// on DSP48 multiplier input bit width

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,
				 int expTab_WL, int expTab_IL,
				 int in_WL, int in_IL,
				 int out_WL, int out_IL


				 >
		struct ssr_fft_traits<SSR_FFT_GROW_TO_MAX_WIDTH,   // specialization for max growth scaling mode
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL> >,
								std::complex<ap_fixed<in_WL,in_IL> >,
								std::complex<ap_fixed<out_WL,out_IL> >
								>
		{


				static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
				static const int tp_log2R = ssr_fft_log2<tp_R>::val;
				static const unsigned int s = NO_OF_FFT_STAGES - stage;
				static const int EXPECTED_NEXT_STAGE_WL = in_WL+tp_log2R;
				static const int EXPECTED_NEXT_STAGE_IL = in_IL+tp_log2R;

				static const int MAX_ALLOWED_BIT_WIDTH_MARGIN = DSP48_OP2_BIT_WIDTH - in_WL;

				// If the expected grown output has bit width larger then max allowed saturate it.
				static const int nextStage_WL = ( EXPECTED_NEXT_STAGE_WL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_WL:DSP48_OP2_BIT_WIDTH;

				//replaced//static const int nextStage_IL= ( EXPECTED_NEXT_STAGE_WL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_IL:in_IL+MAX_ALLOWED_BIT_WIDTH_MARGIN;

				static const int nextStage_IL= ( EXPECTED_NEXT_STAGE_IL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_IL:DSP48_OP2_BIT_WIDTH;



			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0> > TTR_stageOutType;
			//typedef std::complex<ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0> >  ;
			typedef std::complex<ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0> >  		TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0> >  		TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0> > TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,AP_TRN,AP_WRAP,0>  TTR_innerStageOutType;
			typedef ap_fixed<out_WL,out_IL,AP_TRN,AP_WRAP,0>   TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,AP_TRN,AP_WRAP,0>   		TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,AP_TRN,AP_WRAP,0>   		TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,AP_TRN,AP_WRAP,0>  TTR_innerExpTabType;


		};


		/*===================================================================================================================
		 * COMPLEX                  "ap_fixed"                TYPE TRAITS
		 * ==================================================================================================================
		 **/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// (((2))) : Fixed point type with Word length, Integer length , quantization mode, overflow mode and sat bit specs


		//////////////////////////Generic Template without any scaling type specification ///////////////////////////////////
		//                               The next three template will be specializations for SCALING MODE

		template <scaling_mode_enum tp_scaling_mode,
				 int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,ap_q_mode tw_q_mode, ap_o_mode tw_o_mode, int tw_sat_bits,
				 int expTab_WL, int expTab_IL,ap_q_mode expTab_q_mode, ap_o_mode expTab_o_mode, int expTab_sat_bits,
				 int in_WL, int in_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits,
				 int out_WL, int out_IL,ap_q_mode out_q_mode, ap_o_mode out_o_mode, int out_sat_bits
				 >
		struct ssr_fft_traits<tp_scaling_mode,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> >,
								std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> >,
								std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> >
							>
		{
			static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
					static const int tp_log2R = ssr_fft_log2<tp_R>::val;
					static const unsigned int s = NO_OF_FFT_STAGES - stage;
					static const int nextStage_WL = in_WL+tp_log2R;
					static const int nextStage_IL= in_IL+tp_log2R;





			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_stageOutType;


			//typedef std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> > 						TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> > 						TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> > 	TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerStageOutType;


			typedef ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> 						TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> 						TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> 	TTR_innerExpTabType;

		};

		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((2.1))) std::complex < ap_fixed < WL,IL,ap_q_mode, ap_ovf_mode, int sat_bits > >

		//Specialization for ap_fixed with ::SSR_FFT_NO_SCALING:: in this mode the ssr fft stage
		// level output will grow by log2(tp_R) For every stage.

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <
				 int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,ap_q_mode tw_q_mode, ap_o_mode tw_o_mode, int tw_sat_bits,
				 int expTab_WL, int expTab_IL,ap_q_mode expTab_q_mode, ap_o_mode expTab_o_mode, int expTab_sat_bits,
				 int in_WL, int in_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits,
				 int out_WL, int out_IL,ap_q_mode out_q_mode, ap_o_mode out_o_mode, int out_sat_bits
				 >
		struct ssr_fft_traits<SSR_FFT_NO_SCALING,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> >,
								std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> >,
								std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> >
							>
		{
			static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
					static const int tp_log2R = ssr_fft_log2<tp_R>::val;
					static const unsigned int s = NO_OF_FFT_STAGES - stage;
					static const int nextStage_WL = in_WL+tp_log2R;
					static const int nextStage_IL= in_IL+tp_log2R;





			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_stageOutType;


			//typedef std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> > 						TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> > 						TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> > 	TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerStageOutType;


			typedef ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> 						TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> 						TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> 	TTR_innerExpTabType;

		};


		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((2.2))) std::complex < ap_fixed < WL,IL,ap_q_mode, ap_ovf_mode, int sat_bits > >

		//Specialization for ap_fixed with ::SSR_FFT_SCALE:: in this mode the ssr fft stage
		// level output will grow by log2(tp_R) For every stage.

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <
				 int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,ap_q_mode tw_q_mode, ap_o_mode tw_o_mode, int tw_sat_bits,
				 int expTab_WL, int expTab_IL,ap_q_mode expTab_q_mode, ap_o_mode expTab_o_mode, int expTab_sat_bits,
				 int in_WL, int in_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits,
				 int out_WL, int out_IL,ap_q_mode out_q_mode, ap_o_mode out_o_mode, int out_sat_bits
				 >
		struct ssr_fft_traits<SSR_FFT_SCALE,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> >,
								std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> >,
								std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> >
							>
		{
			static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
					static const int tp_log2R = ssr_fft_log2<tp_R>::val;
					static const unsigned int s = NO_OF_FFT_STAGES - stage;
					static const int nextStage_WL = in_WL;
					static const int nextStage_IL= in_IL+tp_log2R;





			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_stageOutType;


			//typedef std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> > 						TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> > 						TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> > 	TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerStageOutType;


			typedef ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> 						TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> 						TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> 	TTR_innerExpTabType;

		};


		/////////////////////////////////////////////////////////////////////////////////////////////////

		//(((2.3))) std::complex < ap_fixed < WL,IL,ap_q_mode, ap_ovf_mode, int sat_bits > >

		//Specialization for ap_fixed with ::SSR_FFT_GROW_TO_MAX_WIDTH:: in this mode the ssr fft stage
		// level output will grow by log2(tp_R) For every stage and then finally saturate to max allowed
		// bit width

		/////////////////////////////////////////////////////////////////////////////////////////////////
		template <
				 int tp_L,int  tp_R, int stage,
				 int tw_WL, int tw_IL,ap_q_mode tw_q_mode, ap_o_mode tw_o_mode, int tw_sat_bits,
				 int expTab_WL, int expTab_IL,ap_q_mode expTab_q_mode, ap_o_mode expTab_o_mode, int expTab_sat_bits,
				 int in_WL, int in_IL,ap_q_mode in_q_mode, ap_o_mode in_o_mode, int in_sat_bits,
				 int out_WL, int out_IL,ap_q_mode out_q_mode, ap_o_mode out_o_mode, int out_sat_bits
				 >
		struct ssr_fft_traits<SSR_FFT_GROW_TO_MAX_WIDTH,
								tp_L,tp_R,stage,
								std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> >,
								std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> >,
								std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> >,
								std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> >
							>
		{
			static const int NO_OF_FFT_STAGES = ssr_fft_log2<tp_L>::val / ssr_fft_log2<tp_R>::val;
					static const int tp_log2R = ssr_fft_log2<tp_R>::val;
					static const unsigned int s = NO_OF_FFT_STAGES - stage;

					static const int EXPECTED_NEXT_STAGE_WL = in_WL+tp_log2R;
					static const int EXPECTED_NEXT_STAGE_IL = in_IL+tp_log2R;

					static const int MAX_ALLOWED_BIT_WIDTH_MARGIN = DSP48_OP2_BIT_WIDTH - in_WL;

					// If the expected grown output has bit width larger then max allowed saturate it.
					static const int nextStage_WL = ( EXPECTED_NEXT_STAGE_WL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_WL:DSP48_OP2_BIT_WIDTH;

					//replaced//static const int nextStage_IL= ( EXPECTED_NEXT_STAGE_WL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_IL:in_WL+MAX_ALLOWED_BIT_WIDTH_MARGIN;

					static const int nextStage_IL= ( EXPECTED_NEXT_STAGE_IL <= DSP48_OP2_BIT_WIDTH ) ? EXPECTED_NEXT_STAGE_IL:DSP48_OP2_BIT_WIDTH;






			typedef std::complex<ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_stageOutType;


			//typedef std::complex<ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> > 				TTR_fftOutType;
			typedef std::complex<ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> > 						TTR_stageInType;
			typedef std::complex<ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> > 						TTR_twiddleType;
			typedef std::complex<ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> > 	TTR_expTabType;


			typedef ap_fixed<nextStage_WL,nextStage_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerStageOutType;


			typedef ap_fixed<out_WL,out_IL,out_q_mode,out_o_mode,out_sat_bits> 				TTR_innerFftOutType;
			typedef ap_fixed<in_WL,in_IL,in_q_mode,in_o_mode,in_sat_bits> 						TTR_innerStageInType;
			typedef ap_fixed<tw_WL,tw_IL,tw_q_mode,tw_o_mode,tw_sat_bits> 						TTR_innerTwiddleType;
			typedef ap_fixed<expTab_WL,expTab_IL,expTab_q_mode,expTab_o_mode,expTab_sat_bits> 	TTR_innerExpTabType;

		};
	} //namespace hls
} //namespace ssr_fft

///////////////////////////////////////////////////////////////////////

#endif //HLS_SSR_FFT_TRAITS_H_

