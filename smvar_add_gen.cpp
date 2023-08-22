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
//Ci=32,Hi>12;
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
	int* isa_ddr){

	int ddr_addr;
	short ddr_step;

	short ram_addr;
	short ram_step;//bloc_w = 64;
	short bloc_size;
	short bloc_numb;



    int hbsize=12;
	int bc_w = 0;
	int bc_h = 0;
    int bc_ci = 0;
	if(Wi/(12*S)*(12*S)<Wi)
		bc_w = Wi/(12*S)+1;
	else
		bc_w = Wi/(12*S);
	if(Hi/(12*S)*(12*S)<Hi)
		bc_h = Hi/(12*S)+1;
	else
		bc_h = Hi/(12*S);
    if(Ci/32*32<Ci)
        bc_ci = Ci/32+1;
    else
        bc_ci = Ci/32;
    cout<<"Hi = "<<bc_h<<" Wi="<<bc_w<<" bc_ci="<<bc_ci<<endl;
    int mat_ddr_addr2=fea_ddr_addr2;
    int vec_ddr_addr = fea_ddr_addr1;
    int res_ddr_addr_t = res_ddr_addr;
	//vec_ddr_addr = fea_ddr_addr-Wi*Ci-1*Ci;
    //vec_ddr_addr = fea_ddr_addr-Wi*P*Ci/32-P*Ci/32;
    vec_ddr_addr = fea_ddr_addr1-Wi*P*Ci-P*Ci;
	ddr_addr = vec_ddr_addr;
	ddr_step = Ci*16/64;
	ram_addr = 0+P;

	if(bc_h==1 && bc_w == 1)
		ram_step = (12*S+2*P)*32/32+4096*15;//bloc_w = 64;
	else if(bc_h==1 && bc_w > 1)
		ram_step = (12*S+2*P)*32/32+4096*7;
	else if(bc_h>1 && bc_w == 1)
		ram_step = (12*S+2*P)*32/32+4096*13;
	else
		ram_step = (12*S+2*P)*32/32+4096*5;
	if(Wi<12)
        bloc_size = Wi+2*P;
    else
        bloc_size = (12*S+2*P);
    if(Hi<12)
	    bloc_numb = Hi+2*P+Wi*32;
    else
        bloc_numb = (12*S+2*P)+Wi*32;
    smvar_vec_dma_isa_gen(
	    ddr,
        VecSram0,
        VecSram1,
		ddr_addr,
	    ddr_step,
	    ram_addr,
	    ram_step,
	    bloc_size,
	    bloc_numb,
		isa_idx,
		isa_ddr
    );
    //pang

	unsigned short vec_base_addr = 0;
	unsigned short mat_base_addr = 0;
	int   res_dest_addr = 0;

	for(int h = 0; h < bc_h; h=h+1){
		for(int w = 0; w < bc_w; w++){
            for(int ci = 0; ci < bc_ci; ci++){

                if(bc_h == 1){
                    if(bc_w == 1){
                        if(ci < bc_ci - 1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*15;//bloc_w = 64;
			    	        bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else {
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*15;
                            bloc_size = (12*S+2*P);
			            	bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                    }
                    else if(bc_w == 2){
                        if(w==0 && ci < bc_ci - 1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*7;//bloc_w = 64;
			    	        bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w==0 && ci == bc_ci - 1){
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*11;
                            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w==1 && ci < bc_ci - 1){
                            vec_ddr_addr = vec_ddr_addr+32;
                            ram_step = (12*S+2*P)*32/32+4096*11;
                            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else{
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*7;
                            bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                    }
                    else if(bc_w > 2){
                        if(w == 0 && ci < bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*7;//bloc_w = 64;
			    	        bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w==0 && ci == bc_ci - 1){
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2)*32/32+4096*3;
                            bloc_size = (12*S+2);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2+Wi*32;
                        }
                        else if(w < bc_w-2 && ci < bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*3;//bloc_w = 64;
			    	        bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w < bc_w-2 && ci == bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*3;
                            bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w == bc_w-2 && ci < bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*3;//bloc_w = 64;
			    	        bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w == bc_w-2 && ci == bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*11;
                            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w == bc_w-1 && ci < bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+32;
			    	        ram_step = (12*S+2*P)*32/32+4096*11;//bloc_w = 64;
			    	        bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                        else if(w == bc_w-1 && ci == bc_ci-1){
                            vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                            ram_step = (12*S+2*P)*32/32+4096*7;
                            bloc_size = (12*S+2*P);
			    	        bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                        }
                    }
                    else{
                        cout<<"blocking error !!!!"<<endl;
                    }
                }
                else if(bc_h == 2){
                    if(bc_w == 1){
                        if(h==0){
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*13;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*14;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			                	bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else if(h==1){
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*14;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*13;
                                bloc_size = (12*S+2*P);
			                	bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else{
                            cout<<"blocking error !!!!"<<endl;
                        }
                    }
                    else if(bc_w == 2){
                        if(h==0){
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*5;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*6;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else{
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*6;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*5;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                    }
                    else if(bc_w > 2){
                        if(h==0){
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*5;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*1;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*1;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*1;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*1;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*9;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*6;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else{
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*6;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*2;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*2;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*2;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*2;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*10;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*5;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                    }
                    else{
                        cout<<"blocking error !!!!"<<endl;
                    }
                }
                else if(bc_h > 2){
                    if(bc_w == 1){
                        if(h==0){
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*13;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*12;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			                	bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h<bc_h-2){
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*12;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*12;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			                	bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h==bc_h-2){
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*12;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*14;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			                	bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else{
                            if(ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*14;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else {
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*13;
                                bloc_size = (12*S+2*P);
			                	bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                    }
                    else if(bc_w == 2){
                        if(h==0){
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*5;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*4;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h<bc_h-2){
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*4;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*4;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h==bc_h-2){
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*4;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*6;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else{
                            if(w==0 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*6;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==1 && ci < bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else{
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*5;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                    }
                    else if(bc_w > 2){
                        if(h==0){
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*5;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*1;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*1;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*1;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*1;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*9;
                                ////*********change
                                //bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
                                ////*********change
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*9;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*4;
                                ////*********change
                                //bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
                                ////*********change
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h<bc_h-2){
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*4;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*0;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*0;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*0;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*0;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*8;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*4;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                        }
                        else if(h==bc_h-2){
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*4;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*0;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*0;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*0;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*0;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*8;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*8;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = (12*S+2*P)+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*6;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                        else{
                            if(w == 0 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*6;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w==0 && ci == bc_ci - 1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*2;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w < bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*2;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w < bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*2;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-2 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*2;//bloc_w = 64;
			    	            bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-2 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+12*S*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*10;
                                bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-1 && ci < bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+32;
			    	            ram_step = (12*S+2*P)*32/32+4096*10;//bloc_w = 64;
			    	            bloc_size = Wi-(bc_w-1)*(S*12)+2*P;
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                            else if(w == bc_w-1 && ci == bc_ci-1){
                                vec_ddr_addr = vec_ddr_addr+(12*S)*Wi*Ci-(bc_w-1)*(12*S)*Ci-(bc_ci-1)*32;
                                ram_step = (12*S+2*P)*32/32+4096*5;
                                bloc_size = (12*S+2*P);
			    	            bloc_numb = Hi-(bc_h-1)*(12*S)+2*P+Wi*32;
                            }
                        }
                    }
                    else{
                        cout<<"blocking error !!!!"<<endl;
                    }
                }
                else{
                    cout<<"blocking error!!!!";
                }
			    
			    ddr_addr = vec_ddr_addr;
			    ddr_step = Ci*16/64;
			    ram_addr = 32768*((h*bc_w*bc_ci+w*bc_ci+ci+1)%2)+P;

			    smvar_vec_dma_isa_gen(
			    	ddr,
        	    	VecSram0,
        	    	VecSram1,
			    	ddr_addr,
			    	ddr_step,
			    	ram_addr,
	            	ram_step,
			    	bloc_size,
			    	bloc_numb,
					isa_idx,
					isa_ddr
			    );

			    ddr_step = 8*Co/32;
			    ram_addr = 32768;
			    ram_step = 0;
			    bloc_size = 12;
			    bloc_numb = 12+Wi*32;

			    ddr_addr = mat_ddr_addr2;
			    smvar_mat_dma_isa_gen(
					ddr,
					MatSram0,
					MatSram1,
					ddr_addr,
					ddr_step,
					ram_addr,
					ram_step,
					bloc_size,
					bloc_numb,
					isa_idx,
					isa_ddr
			    );
			    if(w < bc_w - 1 && ci < bc_ci - 1)
			    	mat_ddr_addr2 = mat_ddr_addr2 + 32;
			    else if(w < bc_w - 1 && ci == bc_ci - 1)
			    	mat_ddr_addr2 = mat_ddr_addr2 + 12*Co - Co + 32;
			    else if(w == bc_w - 1 && ci < bc_ci - 1)
			    	mat_ddr_addr2 = mat_ddr_addr2 + 32;
			    else
			    	mat_ddr_addr2 = mat_ddr_addr2 + 12*Co*Wi/S - (bc_w-1)*12*Co - Co + 32;


			    vec_base_addr = 32768*((h*bc_w*bc_ci+w*bc_ci+ci)%2);//judge vec sram ping pang flag
			    //diff from conv
			    mat_base_addr = 32768;
			    //mat_base_addr = (ci%2)*16384;//judge mat sram ping pang flag, different ci on different mat sram
			    res_dest_addr = 0;//when bc_ci==1, don't need vec add, the res must on vec res sram. when bc_ci > 1, vec add is needed, so mul result must store on mat res sram.
			    subadd_12x12x32(ddr,
						MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,ResVSram, ResMSram,
						vec_base_addr, mat_base_addr, res_dest_addr, 0,P, Co, S, isa_idx,isa_ddr);


				smvar_fence_isa_gen(isa_idx,isa_ddr);
				ddr_addr = res_ddr_addr_t;
				ddr_step = 8*Co/32;
				ram_addr = 0;
			    if(w == bc_w-1 && Wi/(12*S)*(12*S)<Wi)
			        ram_step = 0+4096*(Wi-Wi/(12*S)*(12*S))/S;//bloc_w = 64;
			    else
			        ram_step = 0+4096*12;
				bloc_size = 1;
				bloc_numb = 12+Wi*Co/32/S*32;
//				cout<<"add_layer smvar_res_dma_isa_gen = "<<endl;
		        smvar_res_dma_isa_gen(
		        	ddr,
		        	ResVSram,
					biasSram,
		        	ddr_addr,
		        	ddr_step,
		        	ram_addr,
		        	ram_step,
			        0,
			        0,
		        	bloc_size,
		        	bloc_numb,
					isa_idx,
					isa_ddr
		        );

			    if(w < bc_w - 1 && ci < bc_ci - 1)
				    res_ddr_addr_t = res_ddr_addr_t + 32;
			    else if(w < bc_w - 1 && ci == bc_ci - 1)
			    	res_ddr_addr_t = res_ddr_addr_t + 12*Co - Co + 32;
			    else if(w == bc_w - 1 && ci < bc_ci - 1)
			    	res_ddr_addr_t = res_ddr_addr_t + 32;
			    else
			    	res_ddr_addr_t = res_ddr_addr_t + 12*Co*Wi/S - (bc_w-1)*12*Co - Co + 32;
			    //diff version from ddr
//			    if(w < bc_w - 1 && ci < bc_ci - 1)
//				    res_ddr_addr_t = res_ddr_addr_t + 1;
//			    else if(w < bc_w - 1 && ci == bc_ci - 1)
//			    	res_ddr_addr_t = res_ddr_addr_t + 12*Co/32 - Co/32 + 1;
//			    else if(w == bc_w - 1 && ci < bc_ci - 1)
//			    	res_ddr_addr_t = res_ddr_addr_t + 1;
//			    else
//			    	res_ddr_addr_t = res_ddr_addr_t + 12*Co*Wi/32/S - (bc_w-1)*12*Co/32 - Co/32 + 1;
            }
		}
	}
}
