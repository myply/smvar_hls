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
void subconv_12x12x32(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	in_fixed *VecSram0,
	in_fixed *VecSram1,
	in_fixed **VecRegs0,
	in_fixed **VecRegs1,
	in_fixed **SumSram,
	in_fixed **ResVSram,
	in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short   res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
)
{
	int exe_src0_addr = 0;
	unsigned short exe_src0_size = 0;
	unsigned short exe_src1_addr = 0;
	unsigned short exe_src1_size = 0;
	unsigned short exe_dest_addr = 0;
	unsigned short exe_dest_size = 0;
	unsigned short exe_dest_acti = 0;
	unsigned short exe_dest_link = 0;

	unsigned short src_addr = 0;
	unsigned short src_stride = S;
	unsigned short src_step = (12*S+2);
	unsigned short dest_addr= 0;
	unsigned short dest_step= K*32;
	unsigned short src_bloc_size = K;
	unsigned short src_bloc_numb = K;
	unsigned short dest_bloc_size = K;
	unsigned short dest_bloc_numb = K;

	int ddr_addr;
	unsigned short ddr_step;
	unsigned short ram_addr;
	unsigned short ram_step;
	unsigned short bloc_size;
	unsigned short bloc_numb;
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
    	//cout<<"S="<<S<<endl;
    	src_addr = (12*S+2)*(he*2+1)*S+vec_base_addr;
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
    	for(int ie=0;ie<9;ie++){
    		if(ie == 0)
    			exe_src0_addr = 0+mat_base_addr;
    		else
    			exe_src0_addr = exe_src0_addr + Co;
    		exe_src0_size = Co;
    		exe_src1_addr = ie;
    		exe_src1_size = 1;
    		exe_dest_addr = 0+he*2*Co/32+res_dest_addr;
    		exe_dest_size = Co/32;
    		exe_dest_acti = 0;
    		if(ie == 8)
    			exe_dest_link = 0;
    		else
    			exe_dest_link = 1;
    		smvar_mul_mult_isa_gen(
				MatSram0,
		    	MatSram1,
		    	VecRegs0,
		    	VecRegs1,
		    	SumSram,
		    	ResVSram,
		    	ResMSram,
				 vecReg,
				matReg,
				res_bus,
				sum,
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
    	smvar_fence_isa_gen();

    	src_addr = (S*12+2)*(he*2+2)*S+vec_base_addr;
    	src_stride = S;
		src_step = (S*12+2)*32/32;
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
    	for(int ie=0;ie<9;ie++){
    		if(ie == 0)
    			exe_src0_addr = 0+mat_base_addr;
    		else
    			exe_src0_addr = exe_src0_addr + Co;
    		exe_src0_size = Co;
    		exe_src1_addr = ie+16384;
    		exe_src1_size = 1;
    		exe_dest_addr = Co/32+he*Co*2/32+res_dest_addr;
    		exe_dest_size = Co/32;
    		exe_dest_acti = 0;
//    		cout<<"***************exe_dest_addr:"<<exe_dest_addr<<endl;
    		if(ie == 8)
    			exe_dest_link = 0;
    		else
    			exe_dest_link = 1;
    		smvar_mul_mult_isa_gen(
				MatSram0,
		    	MatSram1,
		    	VecRegs0,
		    	VecRegs1,
		    	SumSram,
		    	ResVSram,
		    	ResMSram,
				vecReg,matReg,res_bus,sum,
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
    	smvar_fence_isa_gen();
    	//cout<<"sss = "<<he<<endl;

	}
    //cout<<"ennenne"<<endl;
}
void subconv_Hx12x32_k1(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	in_fixed *VecSram0,
	in_fixed *VecSram1,
	in_fixed **VecRegs0,
	in_fixed **VecRegs1,
	in_fixed **SumSram,
	in_fixed **ResVSram,
	in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short   res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
)
{
	int H=12;


	unsigned short exe_src0_addr = 0;
	unsigned short exe_src0_size = 0;
	unsigned short exe_src1_addr = 0;
	unsigned short exe_src1_size = 0;
    unsigned short exe_dest_addr = 0;
    unsigned short exe_dest_size = 0;
    unsigned short exe_dest_acti = 0;
    unsigned short exe_dest_link = 0;

    unsigned short src_addr = 0;
    unsigned short src_stride = S;
    unsigned short src_step = (12*S+2*P)*32/32;
    unsigned short dest_addr= 0;
    unsigned short dest_step= 1*1*32/32;
    unsigned short src_bloc_size = 1*32/32;
    unsigned short src_bloc_numb = H;
    unsigned short dest_bloc_size = 1*32/32;
    unsigned short dest_bloc_numb = H;

	int ddr_addr;
	unsigned short ddr_step;
	unsigned short ram_addr;
	unsigned short ram_step;
	unsigned short bloc_size;
	unsigned short bloc_numb;
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

    src_addr = vec_base_addr + (12*S+2*P)*S*6;
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


//	exe_src1_addr = ie;
//	exe_src1_size = 1;
//	exe_dest_addr = 0+he*2*Co/32+res_dest_addr;
//	exe_dest_size = Co/32;
//	exe_dest_acti = 0;
    for(int ie=0;ie<H;ie++){
    	exe_src0_addr = 0+mat_base_addr;
    	exe_src0_size = Co;
//    	exe_src0_size = non_numb[0]/32;
    	exe_src1_addr = ie;
    	exe_src1_size = 1;
    	exe_dest_addr = 0+ie*Co/32+res_dest_addr;
    	exe_dest_size = Co/32;
    	exe_dest_acti = 0;
    	exe_dest_link = 0;
		smvar_mul_mult_isa_gen(
			MatSram0,
	    	MatSram1,
	    	VecRegs0,
	    	VecRegs1,
	    	SumSram,
	    	ResVSram,
	    	ResMSram,
			vecReg,matReg,res_bus,sum,
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
void subconv_12x12x32_test(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	in_fixed *VecSram0,
	in_fixed *VecSram1,
	in_fixed **VecRegs0,
	in_fixed **VecRegs1,
	in_fixed **SumSram,
	in_fixed **ResVSram,
	in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short   res_dest_addr,
	int K,
	int Co,
	int S,
	int &isa_idx,
	int *isa_ddr
)
{
	unsigned short exe_src0_addr = 0;
	unsigned short exe_src0_size = 0;
	unsigned short exe_src1_addr = 0;
	unsigned short exe_src1_size = 0;
	unsigned short exe_dest_addr = 0;
	unsigned short exe_dest_size = 0;
	unsigned short exe_dest_acti = 0;
	unsigned short exe_dest_link = 0;

	unsigned short src_addr = 0;
	unsigned short src_stride = S;
	unsigned short src_step = (12*S+2);
	unsigned short dest_addr= 0;
	unsigned short dest_step= K*32;
	unsigned short src_bloc_size = K;
	unsigned short src_bloc_numb = K;
	unsigned short dest_bloc_size = K;
	unsigned short dest_bloc_numb = K;

	int ddr_addr;
	unsigned short ddr_step;
	unsigned short ram_addr;
	unsigned short ram_step;
	unsigned short bloc_size;
	unsigned short bloc_numb;
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
    	//cout<<"S="<<S<<endl;
    	src_addr = (12*S+2)*(he*2+1)*S+vec_base_addr;
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
    	for(int ie=0;ie<9;ie++){
    		if(ie == 0)
    			exe_src0_addr = 0+mat_base_addr;
    		else
    			exe_src0_addr = exe_src0_addr + Co;
    		exe_src0_size = Co;
    		exe_src1_addr = ie;
    		exe_src1_size = 1;
    		exe_dest_addr = 0+he*2*Co/32+res_dest_addr;
    		exe_dest_size = Co/32;
    		exe_dest_acti = 0;
    		if(ie == 8)
    			exe_dest_link = 0;
    		else
    			exe_dest_link = 1;
    		if(he==0){
    			smvar_mul_mult_isa_gen_test(
    							MatSram0,
    					    	MatSram1,
    					    	VecRegs0,
    					    	VecRegs1,
    					    	SumSram,
    					    	ResVSram,
    					    	ResMSram,
								 vecReg,
								matReg,
								res_bus,
								sum,
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
    		else{
    			smvar_mul_mult_isa_gen(
    							MatSram0,
    					    	MatSram1,
    					    	VecRegs0,
    					    	VecRegs1,
    					    	SumSram,
    					    	ResVSram,
    					    	ResMSram,
								vecReg,
								matReg,
								res_bus,
								sum,
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
    	smvar_fence_isa_gen();

    	src_addr = (S*12+2)*(he*2+2)*S+vec_base_addr;
    	src_stride = S;
		src_step = (S*12+2)*32/32;
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
    	for(int ie=0;ie<9;ie++){
    		if(ie == 0)
    			exe_src0_addr = 0+mat_base_addr;
    		else
    			exe_src0_addr = exe_src0_addr + Co;
    		exe_src0_size = Co;
    		exe_src1_addr = ie+16384;
    		exe_src1_size = 1;
    		exe_dest_addr = Co/32+he*Co*2/32+res_dest_addr;
    		exe_dest_size = Co/32;
    		exe_dest_acti = 0;
//    		cout<<"***************exe_dest_addr:"<<exe_dest_addr<<endl;
    		if(ie == 8)
    			exe_dest_link = 0;
    		else
    			exe_dest_link = 1;
    			smvar_mul_mult_isa_gen(
    							MatSram0,
    					    	MatSram1,
    					    	VecRegs0,
    					    	VecRegs1,
    					    	SumSram,
    					    	ResVSram,
    					    	ResMSram,
								 vecReg,
								matReg,
								res_bus,
								sum,
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
    	smvar_fence_isa_gen();
    	//cout<<"sss = "<<he<<endl;

	}
    //cout<<"ennenne"<<endl;
}
void subconv_Hx12x32_k1_test(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	in_fixed *VecSram0,
	in_fixed *VecSram1,
	in_fixed **VecRegs0,
	in_fixed **VecRegs1,
	in_fixed **SumSram,
	in_fixed **ResVSram,
	in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short   res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
)
{
	int H=12;


	unsigned short exe_src0_addr = 0;
	unsigned short exe_src0_size = 0;
	unsigned short exe_src1_addr = 0;
	unsigned short exe_src1_size = 0;
    unsigned short exe_dest_addr = 0;
    unsigned short exe_dest_size = 0;
    unsigned short exe_dest_acti = 0;
    unsigned short exe_dest_link = 0;

    unsigned short src_addr = 0;
    unsigned short src_stride = S;
    unsigned short src_step = (12*S+2*P)*32/32;
    unsigned short dest_addr= 0;
    unsigned short dest_step= 1*1*32/32;
    unsigned short src_bloc_size = 1*32/32;
    unsigned short src_bloc_numb = H;
    unsigned short dest_bloc_size = 1*32/32;
    unsigned short dest_bloc_numb = H;

	int ddr_addr;
	unsigned short ddr_step;
	unsigned short ram_addr;
	unsigned short ram_step;
	unsigned short bloc_size;
	unsigned short bloc_numb;
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

    src_addr = vec_base_addr + (12*S+2*P)*S*6;
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


//	exe_src1_addr = ie;
//	exe_src1_size = 1;
//	exe_dest_addr = 0+he*2*Co/32+res_dest_addr;
//	exe_dest_size = Co/32;
//	exe_dest_acti = 0;
    for(int ie=0;ie<H;ie++){
    	exe_src0_addr = 0+mat_base_addr;
    	exe_src0_size = Co;
    	//modified here!!!
//    	exe_src0_size = non_numb[0]/32;
    	exe_src1_addr = ie;
    	exe_src1_size = 1;
    	if(Co%32==0){
        	exe_dest_addr = 0+ie*Co/32+res_dest_addr;
        	exe_dest_size = Co/32;
    	}else{
        	exe_dest_addr = 0+ie*Co/32+res_dest_addr;
        	exe_dest_size = Co/32;
    	}
    	//modified here!!!
//    	exe_dest_addr = 0+ie*Co/32+res_dest_addr;
//    	exe_dest_size = Co/32;
    	exe_dest_acti = 0;
    	exe_dest_link = 0;
    	if(ie<1)
    		smvar_mul_mult_isa_gen_test(
			MatSram0,
	    	MatSram1,
	    	VecRegs0,
	    	VecRegs1,
	    	SumSram,
	    	ResVSram,
	    	ResMSram,
			vecReg,matReg,res_bus,sum,
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
    	else
    	smvar_mul_mult_isa_gen(
			MatSram0,
	    	MatSram1,
	    	VecRegs0,
	    	VecRegs1,
	    	SumSram,
	    	ResVSram,
	    	ResMSram,
			vecReg,matReg,res_bus,sum,
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
