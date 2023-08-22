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
void submaxpool_12x12x32(
	in_fixed *ddr,
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
	short dest_step= K*K*32/32;
	short src_bloc_size = K*32/32;
	short src_bloc_numb = K;
	short dest_bloc_size = K*32/32;
	short dest_bloc_numb = K;

	int ddr_addr;
	short ddr_step;
	short ram_addr;
	short ram_step;
	short bloc_size;
	short bloc_numb;
//	 cout<<"VecSram0 co:1"<<endl;
//    for(int i=0;i<11;i++){
//    	cout<<" h:"<<i<<" ";
//    	for(int j=0;j<16;j++){
//    		cout<<VecSram0[i*16*32+j*32+1]<<" ";
//    	}
//    	cout<<endl;
//    }

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


    for(int he=0; he<6; he++){
    	src_addr = (12*S+2*P)*S*(he*2+1)+vec_base_addr;
    	dest_addr = 0+32768;
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
		exe_src1_addr = 0;
		exe_src1_size = K*K;
		exe_dest_addr = 0+he*1*2+res_dest_addr;
		exe_dest_size = 1;
		exe_dest_acti = 0;
		exe_dest_link = 0;
    	smvar_maxpool_isa_gen(
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
			isa_idx,
			isa_ddr
    	);

    	src_addr = (S*12+2*P)*S*(he*2+2)+vec_base_addr;
    	src_stride = S;
		src_step = (S*12+2*P)*32/32;
		dest_addr= 0;
		dest_step= K*K*32/32;
		src_bloc_size = K*32/32;
		src_bloc_numb = K;
		dest_bloc_size = K*32/32;
		dest_bloc_numb = K;
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

		exe_src1_addr = 16384;
		exe_src1_size = K*K;
		exe_dest_addr = 1+he*1*2+res_dest_addr;
		exe_dest_size = 1;
		exe_dest_acti = 0;
		exe_dest_link = 0;
		smvar_maxpool_isa_gen(
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
			isa_idx,
			isa_ddr
		);
    }
}
