/**
 * Finnthesizer Config-File Generation
 *
 **/

#ifndef __LAYER_CONFIG_H_
#define __LAYER_CONFIG_H_

/**
 * Convolutional Layer L0:
 *      IFM  =    32  IFM_CH =     3
 *      OFM  =    30  OFM_CH =    64
 *     SIMD  =     3    PE   =    16
 *     WMEM  =    36   TMEM  =     4
 *     #Ops  = 3110400   Ext Latency  = 32400
**/

#define L0_K 3
#define L0_IFM_CH 3
#define L0_IFM_DIM 32
#define L0_OFM_CH 64
#define L0_OFM_DIM 30
#define L0_SIMD 3
#define L0_PE 16
#define L0_WMEM 36
#define L0_TMEM 4
#define L0_WPI 1
#define L0_API 1
#define L0_WPF 0
#define L0_APF 0

/**
 * Convolutional Layer L1:
 *      IFM  =    30  IFM_CH =    64
 *      OFM  =    28  OFM_CH =    64
 *     SIMD  =     4    PE   =     4
 *     WMEM  =  2304   TMEM  =    16
 *     #Ops  = 57802752   Ext Latency  = 1806336
**/

#define L1_K 3
#define L1_IFM_CH 64
#define L1_IFM_DIM 30
#define L1_OFM_CH 64
#define L1_OFM_DIM 28
#define L1_SIMD 4
#define L1_PE 4
#define L1_WMEM 2304
#define L1_TMEM 16
#define L1_WPI 1
#define L1_API 1
#define L1_WPF 0
#define L1_APF 0

/**
 * Convolutional Layer L2:
 *      IFM  =    14  IFM_CH =    64
 *      OFM  =    12  OFM_CH =   128
 *     SIMD  =     4    PE   =     4
 *     WMEM  =  4608   TMEM  =    32
 *     #Ops  = 21233664   Ext Latency  = 663552
**/

#define L2_K 3
#define L2_IFM_CH 64
#define L2_IFM_DIM 14
#define L2_OFM_CH 128
#define L2_OFM_DIM 12
#define L2_SIMD 4
#define L2_PE 4
#define L2_WMEM 4608
#define L2_TMEM 32
#define L2_WPI 1
#define L2_API 1
#define L2_WPF 0
#define L2_APF 0

/**
 * Convolutional Layer L3:
 *      IFM  =    12  IFM_CH =   128
 *      OFM  =    10  OFM_CH =   128
 *     SIMD  =     4    PE   =     4
 *     WMEM  =  9216   TMEM  =    32
 *     #Ops  = 29491200   Ext Latency  = 921600
**/

#define L3_K 3
#define L3_IFM_CH 128
#define L3_IFM_DIM 12
#define L3_OFM_CH 128
#define L3_OFM_DIM 10
#define L3_SIMD 4
#define L3_PE 4
#define L3_WMEM 9216
#define L3_TMEM 32
#define L3_WPI 1
#define L3_API 1
#define L3_WPF 0
#define L3_APF 0

/**
 * Convolutional Layer L4:
 *      IFM  =     5  IFM_CH =   128
 *      OFM  =     3  OFM_CH =   256
 *     SIMD  =     4    PE   =     4
 *     WMEM  = 18432   TMEM  =    64
 *     #Ops  = 5308416   Ext Latency  = 165888
**/

#define L4_K 3
#define L4_IFM_CH 128
#define L4_IFM_DIM 5
#define L4_OFM_CH 256
#define L4_OFM_DIM 3
#define L4_SIMD 4
#define L4_PE 4
#define L4_WMEM 18432
#define L4_TMEM 64
#define L4_WPI 1
#define L4_API 1
#define L4_WPF 0
#define L4_APF 0

/**
 * Convolutional Layer L5:
 *      IFM  =     3  IFM_CH =   256
 *      OFM  =     1  OFM_CH =   256
 *     SIMD  =     4    PE   =     4
 *     WMEM  = 36864   TMEM  =    64
 *     #Ops  = 1179648   Ext Latency  = 36864
**/

#define L5_K 3
#define L5_IFM_CH 256
#define L5_IFM_DIM 3
#define L5_OFM_CH 256
#define L5_OFM_DIM 1
#define L5_SIMD 4
#define L5_PE 4
#define L5_WMEM 36864
#define L5_TMEM 64
#define L5_WPI 1
#define L5_API 1
#define L5_WPF 0
#define L5_APF 0

/**
 * Fully-Connected Layer L6:
 *     MatW =   256 MatH =   512
 *     SIMD =     4  PE  =     1
 *     WMEM = 32768 TMEM =   512
 *     #Ops  = 262144   Ext Latency  = 32768
**/

#define L6_SIMD 4
#define L6_PE 1
#define L6_WMEM 32768
#define L6_TMEM 512
#define L6_MW 256
#define L6_MH 512
#define L6_WPI 1
#define L6_API 1
#define L6_WPF 0
#define L6_APF 0

/**
 * Fully-Connected Layer L7:
 *     MatW =   512 MatH =   512
 *     SIMD =     2  PE  =     2
 *     WMEM = 65536 TMEM =   256
 *     #Ops  = 524288   Ext Latency  = 65536
**/

#define L7_SIMD 2
#define L7_PE 2
#define L7_WMEM 65536
#define L7_TMEM 256
#define L7_MW 512
#define L7_MH 512
#define L7_WPI 1
#define L7_API 1
#define L7_WPF 0
#define L7_APF 0

/**
 * Fully-Connected Layer L8:
 *     MatW =   512 MatH =    64
 *     SIMD =     1  PE  =     4
 *     WMEM =  8192 TMEM =    16
 *     #Ops  = 65536   Ext Latency  =  8192
**/

#define L8_SIMD 1
#define L8_PE 4
#define L8_WMEM 8192
#define L8_TMEM 16
#define L8_MW 512
#define L8_MH 64
#define L8_WPI 1
#define L8_API 1
#define L8_WPF 0
#define L8_APF 0

#endif //__LAYER_CONFIG_H_
