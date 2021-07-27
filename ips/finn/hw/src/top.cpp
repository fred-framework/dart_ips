#include "config.h"

#include "top.hpp"

#include "./bnn/bnn-library.h"
#include "./bnn/weights.hpp"
#include "./bnn/activations.hpp"
#include "./bnn/interpret.hpp"
#include "./bnn/mvau.hpp"

#include "params_weights.hpp"
#include "params_thresholds.hpp"

void do_compute(ap_uint<64> *in, ap_uint<64> *out)
{
#pragma HLS DATAFLOW
  stream<ap_uint<64>> inter0("DoCompute.inter0");
  stream<ap_uint<192>> inter0_1("DoCompute.inter0_1");
  stream<ap_uint<24>> inter0_2("DoCompute.inter0_2");
#pragma HLS STREAM variable=inter0_2 depth=128
  stream<ap_uint<64>> inter1("DoCompute.inter1");
#pragma HLS STREAM variable=inter1 depth=128
  stream<ap_uint<64>> inter2("DoCompute.inter2");
  stream<ap_uint<64>> inter3("DoCompute.inter3");
#pragma HLS STREAM variable=inter3 depth=128
  stream<ap_uint<128>> inter4("DoCompute.inter4");
#pragma HLS STREAM variable=inter4 depth=128
  stream<ap_uint<128>> inter5("DoCompute.inter5");
  stream<ap_uint<128>> inter6("DoCompute.inter6");
#pragma HLS STREAM variable=inter6 depth=81
  stream<ap_uint<256>> inter7("DoCompute.inter7");
#pragma HLS STREAM variable=inter7 depth=1
  stream<ap_uint<256>> inter8("DoCompute.inter8");
#pragma HLS STREAM variable=inter8 depth=1
  stream<ap_uint<64>> inter9("DoCompute.inter9");
#pragma HLS STREAM variable=inter9 depth=128
  stream<ap_uint<64>> inter10("DoCompute.inter10");
#pragma HLS STREAM variable=inter10 depth=3
  stream<ap_uint<64>> memOutStrm("DoCompute.memOutStrm");

  static const unsigned int inBits = 32 * 32 * 3 * 8;
  // const unsigned int inBitsPadded = paddedSize(inBits, 64);
  static const unsigned int outBits = L8_MH*16;

  static const unsigned int NUM_REPS = 256;

// partition PE arrays
#pragma HLS ARRAY_PARTITION variable=m_weights_0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_0 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_1 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_2 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_2 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_3 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_3 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_3 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_4 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_4 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_4 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_5 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_5 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_5 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_6 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_6 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_6 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_7 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_7 complete dim=1
#pragma HLS ARRAY_PARTITION variable=m_thresholds_7 complete dim=3
#pragma HLS ARRAY_PARTITION variable=m_weights_8 complete dim=1

  static BinaryWeights<L0_SIMD, L0_PE, L0_WMEM>  weights0(m_weights_0);
  static BinaryWeights<L1_SIMD, L1_PE, L1_WMEM>  weights1(m_weights_1);
  static BinaryWeights<L2_SIMD, L2_PE, L2_WMEM>  weights2(m_weights_2);
  static BinaryWeights<L3_SIMD, L3_PE, L3_WMEM>  weights3(m_weights_3);
  static BinaryWeights<L4_SIMD, L4_PE, L4_WMEM>  weights4(m_weights_4);
  static BinaryWeights<L5_SIMD, L5_PE, L5_WMEM>  weights5(m_weights_5);
  static BinaryWeights<L6_SIMD, L6_PE, L6_WMEM>  weights6(m_weights_6);
  static BinaryWeights<L7_SIMD, L7_PE, L7_WMEM>  weights7(m_weights_7);
  static BinaryWeights<L8_SIMD, L8_PE, L8_WMEM>  weights8(m_weights_8);

  //static ThresholdsActivation<L0_TMEM, L0_PE, L0_API, ap_fixed<24, 16>, ap_uint<L0_API> > threshs0(reinterpret_cast<const ap_fixed<24, 16> (*)[L0_TMEM][L0_API]>(m_thresholds_0));
  static ThresholdsActivation<L0_TMEM, L0_PE, L0_API, ap_fixed<64, 56>, ap_uint<L0_API> >  	threshs0(reinterpret_cast<const ap_fixed<64,56>(*)[L0_TMEM][L0_API]>(m_thresholds_0));
  static ThresholdsActivation<L1_TMEM, L1_PE, L1_API, ap_int<16>, ap_uint<L1_API>>  		threshs1(m_thresholds_1);
  static ThresholdsActivation<L2_TMEM, L2_PE, L2_API, ap_int<16>, ap_uint<L2_API>>  		threshs2(m_thresholds_2);
  static ThresholdsActivation<L3_TMEM, L3_PE, L3_API, ap_int<16>, ap_uint<L3_API>>  		threshs3(m_thresholds_3);
  static ThresholdsActivation<L4_TMEM, L4_PE, L4_API, ap_int<16>, ap_uint<L4_API>>  		threshs4(m_thresholds_4);
  static ThresholdsActivation<L5_TMEM, L5_PE, L5_API, ap_int<16>, ap_uint<L5_API>>  		threshs5(m_thresholds_5);
  static ThresholdsActivation<L6_TMEM, L6_PE, L6_API, ap_int<16>, ap_uint<L6_API>>  		threshs6(m_thresholds_6);
  static ThresholdsActivation<L7_TMEM, L7_PE, L7_API, ap_int<16>, ap_uint<L7_API>>  		threshs7(m_thresholds_7);

  Mem2Stream_Batch<64, inBits / 8>(in, inter0, NUM_REPS);
  StreamingDataWidthConverter_Batch<64, 192, (32 * 32 * 3 * 8) / 64>(inter0, inter0_1, NUM_REPS);
  StreamingDataWidthConverter_Batch<192, 24, (32 * 32 * 3 * 8) / 192>(inter0_1, inter0_2, NUM_REPS);

  // convolutional layers
  ConvLayer_Batch<L0_K, L0_IFM_CH, L0_IFM_DIM, L0_OFM_CH, L0_OFM_DIM, L0_SIMD, L0_PE, Slice<ap_fixed<8, 1, AP_TRN, AP_SAT>>, Identity, Recast<Binary>>(inter0_2, inter1, weights0, threshs0, NUM_REPS, ap_resource_lut());
  ConvLayer_Batch<L1_K, L1_IFM_CH, L1_IFM_DIM, L1_OFM_CH, L1_OFM_DIM, L1_SIMD, L1_PE, Recast<XnorMul>>(inter1, inter2, weights1, threshs1, NUM_REPS, ap_resource_lut());

  StreamingMaxPool_Batch<L1_OFM_DIM, 2, L1_OFM_CH>(inter2, inter3, NUM_REPS);

  ConvLayer_Batch<L2_K, L2_IFM_CH, L2_IFM_DIM, L2_OFM_CH, L2_OFM_DIM, L2_SIMD, L2_PE, Recast<XnorMul>>(inter3, inter4, weights2, threshs2, NUM_REPS, ap_resource_lut());
  ConvLayer_Batch<L3_K, L3_IFM_CH, L3_IFM_DIM, L3_OFM_CH, L3_OFM_DIM, L3_SIMD, L3_PE, Recast<XnorMul>>(inter4, inter5, weights3, threshs3, NUM_REPS, ap_resource_lut());

  StreamingMaxPool_Batch<L3_OFM_DIM, 2, L3_OFM_CH>(inter5, inter6, NUM_REPS);

  ConvLayer_Batch<L4_K, L4_IFM_CH, L4_IFM_DIM, L4_OFM_CH, L4_OFM_DIM, L4_SIMD, L4_PE, Recast<XnorMul>>(inter6, inter7, weights4, threshs4, NUM_REPS, ap_resource_lut());
  ConvLayer_Batch<L5_K, L5_IFM_CH, L5_IFM_DIM, L5_OFM_CH, L5_OFM_DIM, L5_SIMD, L5_PE, Recast<XnorMul>>(inter7, inter8, weights5, threshs5, NUM_REPS, ap_resource_lut());

  // fully connected layers
  WidthAdjustedOutputStream<16 * L8_PE, 64, L8_MH / L8_PE>  wa_out(memOutStrm, NUM_REPS);
  StreamingFCLayer_Batch<L6_MW, L6_MH, L6_SIMD, L6_PE, Recast<XnorMul>>
    (inter8, inter9,  weights6, threshs6, NUM_REPS, ap_resource_lut());
  StreamingFCLayer_Batch<L7_MW, L7_MH, L7_SIMD, L7_PE, Recast<XnorMul>>
    (inter9, inter10, weights7, threshs7, NUM_REPS, ap_resource_lut());
  StreamingFCLayer_Batch<L8_MW, L8_MH, L8_SIMD, L8_PE, Recast<XnorMul>, Slice<ap_uint<16> >>
    (inter10, static_cast<hls::stream<ap_uint<16 * L8_PE>>&>(wa_out), weights8, PassThroughActivation<ap_uint<16>>(), NUM_REPS, ap_resource_lut());

  Stream2Mem_Batch<64, outBits/8>(memOutStrm, out, NUM_REPS);
}
