#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "smvar_isa_sim.h"
#include "smvar_isa_gen.h"
#include "sub_function.h"
using namespace std;
//load vec to regs and complete calc
void subadd_12x12x32(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **ResVSram,
	half ** ResMSram,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short   res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
){
    short exe_src0_addr = 0;
    short exe_src0_size = 0;
    short exe_src1_addr = 0;
    short exe_src1_size = 0;
    short exe_dest_addr = 0;
    short exe_dest_size = 0;
    short exe_dest_acti = 0;
    short exe_dest_link = 0;

    short src_addr = 0;
    short src_stride = S;
	short src_step = (12*S+2*P)*32/32;
	short dest_addr= 0;
	short dest_step= 1*1*32/32;
	short src_bloc_size = 1*32/32;
	short src_bloc_numb = 12;
	short dest_bloc_size = 1*32/32;
	short dest_bloc_numb = 12;

	int ddr_addr;
	short ddr_step;
	short ram_addr;
	short ram_step;
	short bloc_size;
	short bloc_numb;

	src_addr = src_addr+vec_base_addr;
    smvar_vec_load_isa_gen(
		VecSram0,
        VecSram1,
        VecRegs0,
        VecRegs1,
	    src_addr,
		src_stride,
	    src_step,
	    dest_addr,
	    dest_step,
	    src_bloc_size,
	    src_bloc_numb,
	    dest_bloc_size,
	    dest_bloc_numb,
		isa_idx,
		isa_ddr
    );
//    src_addr = vec_base_addr + (12*S+2*P)*S*6;
//    dest_addr = 0+32768;
//    smvar_vec_load_isa_gen(
//		VecSram0,
//        VecSram1,
//        VecRegs0,
//        VecRegs1,
//	    src_addr,
//		src_stride,
//	    src_step,
//	    dest_addr,
//	    dest_step,
//	    src_bloc_size,
//	    src_bloc_numb,
//	    dest_bloc_size,
//	    dest_bloc_numb,
//		isa_idx,
//		isa_ddr
//    );

    exe_src0_addr = mat_base_addr;
    exe_src0_size = 12;
    exe_src1_addr = 0;
    exe_src1_size = 12;
    exe_dest_addr = res_dest_addr;
    exe_dest_size = 12;
    exe_dest_acti = 0;
    exe_dest_link = 0;
//    smvar_vec_add_isa_gen(
//    	ResVSram,
//		ResMSram,
//        32768,
//        12*Co/32,
//        32768,
//        12*Co/32,
//        0,
//        12*Co/32,
//        0,
//        0,
//		1,
//		isa_idx,
//		isa_ddr
//    );
    smvar_vec_add_isa_gen(
		MatSram0,
		MatSram1,
        VecRegs0,
        VecRegs1,
    	ResVSram,
		ResMSram,
    	exe_src0_addr,
		exe_src0_size,
		exe_src1_addr,
		exe_src1_size,
		exe_dest_addr,
		exe_dest_size,
		exe_dest_acti,
		exe_dest_link,
		0,
		isa_idx,
		isa_ddr
    );

	//smvar_fence_isa(
	//	isa_idx,
	//	outFile2
	//);
}
