#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "smvar_isa_sim.h"
#include "sub_function.h"
using namespace std;
//Ci=32,Hi>12;
void smvar_conv_code_generate(
		in_fixed *feature,
		in_fixed *spmat,
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
		int Co,
		int Ci,
		int Hi,
		int Wi,
		int K,
		int S){
	int isa_idx[1];
	isa_idx[0] = 0;
    int isa_length = 8192;
    //store isa to ddr;//ping
	int ddr_addr;
	short ddr_step;
	short ram_addr;
	short ram_step;//bloc_w = 64;
	short bloc_size;
	short bloc_numb;

    ddr_addr = isa_length+Ci*Hi*Wi*2;
    ddr_step = 8;
    ram_addr = 0;
    ram_step = 0;
    bloc_size = K*K*Co;
    bloc_numb = 1+1*16;
    smvar_mat_dma_isa_sim(
		ddr,
        MatSram0,
        MatSram1,
    	ddr_addr,
    	ddr_step,
    	ram_addr,
    	ram_step,
    	bloc_size,
    	bloc_numb,
		isa_idx
    );

    ddr_addr = isa_length+Ci*Hi*Wi+Co*32;
    ddr_step = 8;
    ram_addr = 16384;
    ram_step = 0;
    bloc_size = Co*9;
    bloc_numb = 1+1*16;
    smvar_mat_dma_isa_sim(
    	ddr,
        MatSram0,
        MatSram1,
		ddr_addr,
    	ddr_step,
    	ram_addr,
    	ram_step,
    	bloc_size,
    	bloc_numb,
		isa_idx
    );
    int vec_ddr_addr = 0;
	vec_ddr_addr = isa_length-Wi*Ci-1*Ci;
	ddr_addr = vec_ddr_addr;
	ddr_step = Ci*16/64;
	ram_addr = 0;
	ram_step = (12*S+2)*32/32+4096*5;//bloc_w = 64;
	bloc_size = (12*S+2)*32/32;
	bloc_numb = (12*S+2)+Wi*Ci/32*32;
    smvar_vec_dma_isa(
	    ddr,
        VecSram0,
        VecSram1,
		ddr_addr,
	    ddr_step,
	    ram_addr,
	    ram_step,
	    bloc_size,
	    bloc_numb,
		isa_idx
    );
    //pang
	int isa_subconv_count;
	short vec_base_addr = 0;
	short mat_base_addr = 0;
	int   res_ddr_addr  = 16384*32;
	int   res_dest_addr = 0;
	int bc_w = 0;
	int bc_h = 0;
	if(Wi/(12*S)*(12*S)<Wi)
		bc_w = Wi/(12*S)+1;
	else
		bc_w = Wi/(12*S);
	if(Hi/(12*S)*(12*S)<Hi)
		bc_h = Hi/(12*S)+1;
	else
		bc_h = Hi/(12*S);
	cout<<"Hi = "<<bc_h<<" Wi="<<bc_w<<endl;
	for(int h = 0; h < bc_h; h=h+1){
		for(int w = 0; w < bc_w; w++){
			cout<<"h = "<<h<<"w = "<<w<<endl;
			//outFile2<<"bc = "<<h*bc_w+w<<endl;
			if(h == 0 && w < bc_w-2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*1;//bloc_w = 64;
				bloc_size = (12*S+2);
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h == 0 && w == bc_w-2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*9;
				bloc_size = Wi-(bc_w-1)*(S*12)+2;
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h == 0 && bc_h > 2 && w == bc_w-1){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*4;
				bloc_size = (12*S+2);
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h >0 && h < bc_h - 1 && w < bc_w-2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*0;//bloc_w = 64;
				bloc_size = (12*S+2);
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h>0 && h < bc_h-1 && w == bc_w -2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*8;
				bloc_size = Wi-(bc_w-1)*(12*S)+2;
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h > 0 && h < bc_h - 2 && w == bc_w-1){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*4;
				bloc_size = (12*S+2);
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
			}
			else if(h == bc_h - 2 && w == bc_w-1){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*6;
				bloc_size = (12*S+2);
				bloc_numb = Hi-(bc_h-1)*(12*S)+2 + Wi*Ci/32*32;
			}
			else if(h == bc_h - 1 && w < bc_w-2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*2;//bloc_w = 64;
				bloc_size = (12*S+2);
				bloc_numb = Hi-(bc_h-1)*(12*S)+2 +Wi*Ci/32*32;
			}
			else if(h == bc_h-1 && w == bc_w -2){
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*10;
				bloc_size = Wi-(bc_w-1)*(12*S)+2;
				bloc_numb = Hi-(bc_h-1)*(12*S)+2+Wi*Ci/32*32;
			}
			else{
				vec_ddr_addr = vec_ddr_addr+(12*S)*Ci;
				ram_step = (12*S+2)*32/32+4096*10;//bloc_w = 64;
				bloc_numb = (12*S+2)+Wi*Ci/32*32;
				bloc_size = 14*32/32;//stride = 1;
			}
			ddr_addr = vec_ddr_addr;
			ddr_step = Ci*16/64;
			ram_addr = 32768*((h*bc_w+w+1)%2);
			smvar_vec_dma_isa(
				ddr,
        		VecSram0,
        		VecSram1,
				ddr_addr,
				ddr_step,
				ram_addr,
	        	ram_step,
				bloc_size,
				bloc_numb,
				isa_idx
			);
			vec_base_addr = 32768*((h*bc_w+w)%2);
			mat_base_addr = 0;

			res_dest_addr = 0;

			subconv_12x12x32(
				ddr,
				MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,
		    	SumSram, ResVSram, ResMSram,
				vec_base_addr, mat_base_addr, res_dest_addr, 3, Co, S, isa_idx);
			ddr_addr = res_ddr_addr;
			ddr_step = 8;
			ram_addr = 0;
			if(w == bc_w-1 && Wi/(12*S)*(12*S)<Wi)
				ram_step = 0+4096*(Wi-Wi/(12*S)*(12*S))/S;//bloc_w = 64;
			else
				ram_step = 0+4096*12;
			bloc_size = Co/32;
			bloc_numb = 12+Wi/S*32;
			smvar_res_dma_isa_sim(
				ddr,
				ResVSram,
				ddr_addr,
				ddr_step,
				ram_addr,
				ram_step,
				bloc_size,
				bloc_numb,
				isa_idx
			);
			if(w < bc_w - 1)
				res_ddr_addr = res_ddr_addr + 12*Co;
			else
				res_ddr_addr = res_ddr_addr + 12*Co*Wi/S - (bc_w-1)*12*Co;
		}
	}


	/*ddr_addr = 16384;
	ddr_step = 32*16/64;
	ram_addr = 0;
	ram_step = 0;//bloc_w = 64;
	bloc_size = Hi*Wi*Co/32;
	bloc_numb = 1+1*16;
    smvar_vec_dma_isa(
	    ddr,
        VecSram0,
        VecSram1,
		ddr_addr,
	    ddr_step,
	    ram_addr,
	    ram_step,
	    bloc_size,
	    bloc_numb,
		isa_idx
    );*/

}
