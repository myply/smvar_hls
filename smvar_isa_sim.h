#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
void smvar_spmat_dma_isa_sim(
    in_fixed *ddr,
    in_fixed *matSramData0,
    in_fixed *matSramIdx0,
    in_fixed *matSramData1,
    in_fixed *matSramIdx1,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb
);
void smvar_mat_dma_isa_sim(
	in_fixed *ddr,
	in_fixed *matSram0,
	in_fixed *matSram1,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb
);
void smvar_vec_dma_isa_sim(
	in_fixed *ddr,
	half *vecSram0,
	half *vecSram1,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb
);
void smvar_vec_load_isa_sim(
	half *vecSram0,
	half *vecSram1,
	half **vecRegs0,
	half **vecRegs1,
	unsigned short src_addr,
	unsigned short src_stride,
	unsigned short src_step,
	unsigned short dest_addr,
	unsigned short dest_step,
	unsigned short src_bloc_size,
	unsigned short src_bloc_numb,
	unsigned short dest_bloc_size,
	unsigned short dest_bloc_numb
);
void smvar_mul_mult_isa_sim(
	in_fixed *matSram0,
	in_fixed *matSram1,
	half **vecRegs0,
	half **vecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	half ** vecReg,
	half *matReg,
	half *res_bus,
	half *sum,
	half *virtual_mat_block,
	int exe_src0_addr,
	//unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link
);
void smvar_mul_mult_isa_sim_test(
	in_fixed *matSram0,
	in_fixed *matSram1,
	half **vecRegs0,
	half **vecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	half ** vecReg,
	half *matReg,
	half *res_bus,
	half *sum,
	half *virtual_mat_block,
	int exe_src0_addr,
	//unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link
);
void smvar_fence_isa_sim(

);

void smvar_vec_add_isa_sim(
	half **ResVSram,
	half **ResMSram,
	unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link,
	unsigned short after_mul
);
void smvar_res_dma_isa_sim(
	in_fixed *ddr,
	half ** ResVSram,
	half * biasSram,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short acti,
	unsigned short bias,
	unsigned short bloc_size,
	unsigned short bloc_numb
);
void smvar_bias_dma_isa_sim(
	in_fixed *ddr,
	half * biasSram,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb
);
void smvar_isend_isa_sim(
);
