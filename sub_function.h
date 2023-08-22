#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
void mat_reshape(
	in_fixed *mat,
	int Co,
	int Ci,
	int K,
	in_fixed *re_mat
);
void mat_generate(
	int nonzero,
	in_fixed *spmat_block,
    short *spdest_block,
    short *spvn_block,
	ofstream &fout
);
void img2col(
	in_fixed *feature,
	short ch_in,
	short h_in,
	short w_in,
	short h_out,
	short w_out,
	short k_in,
	short stride_in,
	in_fixed *matrix
);

void subconv_12x12x32(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	half ** vecReg,
	half *matReg,
	half *res_bus,
	half *sum,
	half *virtual_mat_block,
	int *non_numb_ci,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
);
void subconv_Hx12x32_k1(
	in_fixed *ddr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	half ** vecReg,
	half *matReg,
	half *res_bus,
	half *sum,
	half *virtual_mat_block,
	int *non_numb_ci,
	unsigned short vec_base_addr, //12*12*32
	unsigned short mat_base_addr,
	unsigned short res_dest_addr,
	int K,
	int P,
	int Co,
	int S,
	int &isa_idx,
	int* isa_ddr
);
void subconv_12x12x32_test(
		in_fixed *ddr,
		in_fixed *MatSram0,
		in_fixed *MatSram1,
		half *VecSram0,
		half *VecSram1,
		half **VecRegs0,
		half **VecRegs1,
		half **SumSram,
		half **ResVSram,
		half **ResMSram,
		half ** vecReg,
		half *matReg,
		half *res_bus,
		half *sum,
		half *virtual_mat_block,
		int *non_numb_ci,
		unsigned short vec_base_addr, //12*12*32
		unsigned short mat_base_addr,
		unsigned short res_dest_addr,
		int K,
		int P,
		int Co,
		int S,
		int &isa_idx,
		int* isa_ddr
);
void subconv_Hx12x32_k1_test(
		in_fixed *ddr,
		in_fixed *MatSram0,
		in_fixed *MatSram1,
		half *VecSram0,
		half *VecSram1,
		half **VecRegs0,
		half **VecRegs1,
		half **SumSram,
		half **ResVSram,
		half **ResMSram,
		half ** vecReg,
		half *matReg,
		half *res_bus,
		half *sum,
		half *virtual_mat_block,
		int *non_numb_ci,
		unsigned short vec_base_addr, //12*12*32
		unsigned short mat_base_addr,
		unsigned short res_dest_addr,
		int K,
		int P,
		int Co,
		int S,
		int &isa_idx,
		int* isa_ddr
);
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
);
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
);
void subupsample_12x12x32(
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
);
void smvar_conv_code_nCi(
	in_fixed *ddr,
	int fea_ddr_addr,
	int res_ddr_addr,
	int mat_ddr_addr,
	int bias_ddr_addr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	half ** vecReg,
	half *matReg,
	half *res_bus,
	half *sum,
	half *biasSram,
	half *virtual_mat_block,
	 int *non_numb,
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S,
	int P,
	int bias,
	int acti,
	int &isa_idx,
	int* isa_ddr);
void smvar_maxpool_gen(
	in_fixed *ddr,
	unsigned int fea_ddr_addr1,
	unsigned int res_ddr_addr,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **ResVSram,
	half ** ResMSram,
	half *biasSram,
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S,
	int P,
	int &isa_idx,
	int* isa_ddr);
void smvar_upsample_gen(
	in_fixed *ddr,
	unsigned int fea_ddr_addr1,
	unsigned int res_ddr_addr,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **ResVSram,
	half ** ResMSram,
	half *biasSram,
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S,
	int P,
	int &isa_idx,
	int* isa_ddr);
void smvar_add_gen(
	in_fixed *ddr,
	int fea_ddr_addr1,
	int fea_ddr_addr2,
	int res_ddr_addr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **ResVSram,
	half ** ResMSram,
	half * biasSram,
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S,
	int P,
	int &isa_idx,
	int* isa_ddr);
void smvar_conv_k1_sim(
	in_fixed *ddr,
	int fea_ddr_addr,
	int res_ddr_addr,
	int mat_ddr_addr,
	in_fixed *MatSram0,
	in_fixed *MatSram1,
	half *VecSram0,
	half *VecSram1,
	half **VecRegs0,
	half **VecRegs1,
	half **SumSram,
	half **ResVSram,
	half **ResMSram,
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S);
void smvar_slice_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi);
void smvar_act_code(in_fixed * ddr,
		int ddr_addr,
	int Ci,
	int Hi,
	int Wi);
void smvar_maxpool_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi,
	int kernel_size,
	int padding);
void smvar_upsample_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi);
void smvar_add_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi);
void smvar_cat_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi);
void smvar_cat4_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int src3_ddr_addr,
		int src4_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi);

void store_fea_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Ci,
		int Hi,
		int Wi,
		int vec_ddr_addr);
void store_weight_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Co,
		int Ci,
		int K,
		int mat_ddr_addr);
void store_sp_weight_from_csv_2_ddr(in_fixed * ddr,
		int *non_numb,
		string csv_name,
		int  Co,
		int Ci,
		int K,
		int mat_ddr_addr);
void smvar_bias_sim(in_fixed * ddr,
		int vec_ddr_addr,
		int bias_ddr_addr,
		int  Ci,
		int Hi,
		int Wi);
void store_bias_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Co,
		int bias_ddr_addr);
void get_short_from_hex_file(ifstream &isaFile, unsigned short &val);
void mat_block(
half *mat,
	int H_i,
	int W_i,
	half *spmat_block,
	unsigned short *spdest_block,
	unsigned short *spvn_block,
	int *non_numb
);
