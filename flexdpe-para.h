#ifndef _LSTM_FPGA_H_
#define _LSTM_FPGA_H_

#include "ap_fixed.h"
#include "hls_stream.h"
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <iostream>
//#define DOUBLE 1
#define IN_DATA_TYPE  16
#define OUT_DATA_TYPE  32
#define NUM_PES  32
#define INPUT_BW  32
#define LOG2_PES  5
#define FLEX_DPE_SIZE  32
#define FLEX_DPE_IN_BW  512

#define M_DIM 1024
#define N_DIM 1088
#define ZERO_RATIO_M 0.5
#define OFFSET 201326592


typedef unsigned int uint;
typedef ap_fixed<16,8> SpMV_fixed;

typedef unsigned int uint;

#ifdef DOUBLE
// double precision types
  // read-only weight
typedef double mult_fixed;
#else
// quantized data types
typedef float in_fixed;
//typedef ap_fixed<16,7> in_fixed;   // read-only weights
typedef ap_fixed<32,14> mult_fixed;  // read-only constants for fft/ifft
typedef ap_fixed<16,7> out_fixed; //32,14

#endif

#endif
