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
void smvar_conv_code_nCi(
	in_fixed *ddr,
	int fea_ddr_addr,
	int res_ddr_addr,
	int mat_ddr_addr,
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
	int Co,
	int Ci,
	int Hi,
	int Wi,
	int K,
	int S,
	int P,
	int &isa_idx,
	int* isa_ddr){
    int isa_length = 16384;
    //store isa to ddr;//ping
	int ddr_addr;
	short ddr_step;

	short ram_addr;
	short ram_step;//bloc_w = 64;
	short bloc_size;
	short bloc_numb;

    ddr_addr = mat_ddr_addr;
    if(Ci%32==0 or Ci>32)
    	ddr_step = 8;
    else
		ddr_step = (Ci%32)/4;
    ram_addr = 0;
    ram_step = 0;
    bloc_size = K*K*Co;
    bloc_numb = 1+1*16;
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
    
    int vec_ddr_addr = 0;
	vec_ddr_addr = fea_ddr_addr-Wi*Ci-1*Ci;

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

	short vec_base_addr = 0;
	short mat_base_addr = 0;
	int   res_dest_addr = 0;

	for(int h = 0; h < bc_h; h=h+1){
		for(int w = 0; w < bc_w; w++){
            for(int ci = 0; ci < bc_ci; ci++){
            	//***********

            	//***********
            	if(bc_ci==1){
            		mat_ddr_addr=mat_ddr_addr+0;
            	}
            	else if(ci==(bc_ci-1)){
            		mat_ddr_addr=mat_ddr_addr-(ci*32*Co*9);
            	}
            	else{
            		mat_ddr_addr=mat_ddr_addr+Co*9*32;
            	}
            	//cout<<"h = "<<h<<" w="<<w<<" ci="<<ci<<" mat_ddr_addr="<<mat_ddr_addr<<endl;
            	//***********

                ddr_addr = mat_ddr_addr;

                //update new code
                if(Ci%32==0 or ci!=(bc_ci-1))
                	ddr_step = 8;
                else
            		ddr_step = (Ci%32)/4;
                ram_addr = 16384*((h*bc_w*bc_ci+w*bc_ci+ci+1)%2);
                //ddr_step = 8;
                //ram_addr = 16384*((ci+1)%2);

                ram_step = 0;
                bloc_size = Co*9;
                bloc_numb = 1+1*16;
//                cout<<"check 1"<<endl;
//                cout<<"mat_ddr "<<mat_ddr_addr<<endl;
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
//                cout<<"check 2"<<endl;
			    //cout<<"h = "<<h<<"w = "<<w<<"ci = "<<ci<<endl;
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

			    vec_base_addr = 32768*((h*bc_w*bc_ci+w*bc_ci+ci)%2);//judge vec sram ping pang flag
			    mat_base_addr = 16384*((h*bc_w*bc_ci+w*bc_ci+ci)%2);
			    //mat_base_addr = (ci%2)*16384;//judge mat sram ping pang flag, different ci on different mat sram
			    res_dest_addr = (ci>0)*32768;//when bc_ci==1, don't need vec add, the res must on vec res sram. when bc_ci > 1, vec add is needed, so mul result must store on mat res sram.


//			    if(h==0 && w==1 && ci == 0){
//				    subconv_12x12x32_test(
//				    	ddr,
//				    	MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,
//			        	SumSram, ResVSram, ResMSram,
//				    	vec_base_addr, mat_base_addr, res_dest_addr, 3, Co, S, isa_idx);
//			    }
//			    else
			    if(K>1)
				    subconv_12x12x32(
				    	ddr,
				    	MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,
			        	SumSram, ResVSram, ResMSram,vecReg,matReg,res_bus,sum,
				    	vec_base_addr, mat_base_addr, res_dest_addr, 3,P, Co, S, isa_idx,isa_ddr);
			    else{
			    	if(h==0 && w==0)
			    		subconv_Hx12x32_k1_test(
					    	ddr,
					    	MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,
				        	SumSram, ResVSram, ResMSram,vecReg,matReg,res_bus,sum,
					    	vec_base_addr, mat_base_addr, res_dest_addr, 1,P, Co, S, isa_idx,isa_ddr);
			    	else{
			    		subconv_Hx12x32_k1(
					    	ddr,
					    	MatSram0, MatSram1, VecSram0, VecSram1, VecRegs0, VecRegs1,
				        	SumSram, ResVSram, ResMSram,vecReg,matReg,res_bus,sum,
					    	vec_base_addr, mat_base_addr, res_dest_addr, 1,P, Co, S, isa_idx,isa_ddr);
			    	}
			    }
                //add mul result for different ci.
                if(ci>0){
                    smvar_vec_add_isa_gen(
                    	ResVSram,
						ResMSram,
                        32768,
	                    12*Co/32,
	                    32768,
	                    12*Co/32,
	                    0,
	                    12*Co/32,
	                    0,
	                    0,
						isa_idx,
						isa_ddr
                    );
                }
                if(ci == bc_ci-1){
			        ddr_addr = res_ddr_addr;
			        ddr_step = 8;
			        ram_addr = 0;
			        if(w == bc_w-1 && Wi/(12*S)*(12*S)<Wi)
			        	ram_step = 0+4096*(Wi-Wi/(12*S)*(12*S))/S;//bloc_w = 64;
			        else
			        	ram_step = 0+4096*12;
			        bloc_size = Co/32;
			        bloc_numb = 12+Wi/S*32;
			        //cout<<"ssssssssssssssss"<<endl;

			        smvar_res_dma_isa_gen(
			        	ddr,
			        	ResVSram,
			        	ddr_addr,
			        	ddr_step,
			        	ram_addr,
			        	ram_step,
			        	bloc_size,
			        	bloc_numb,
						isa_idx,
						isa_ddr
			        );
			        //12*12*Co  and the layout of res in ddr is Ho*Wo*32
			        if(w < bc_w - 1)
			        	res_ddr_addr = res_ddr_addr + 12*Co;
			        else
			        	res_ddr_addr = res_ddr_addr + 12*Co*Wi/S - (bc_w-1)*12*Co;
                }
            }
		}
	}
	smvar_isend_isa_gen(isa_idx,isa_ddr);
}
void smvar_conv_k1_sim(
	in_fixed *ddr,
	int fea_ddr_addr,
	int res_ddr_addr,
	int mat_ddr_addr,
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
	cout<<"conv sim version "<<endl;
	int padding=0;
	if(K==3)
		padding=1;
	int Ho=(Hi+2*padding - K) / S + 1;
	int Wo=(Wi+2*padding - K) / S + 1;

	in_fixed *fea_pad = new in_fixed[(Hi+2*padding)*(Wi+2*padding)*Ci];
	for(int h=0; h<Hi+2*padding; h++)
		for(int w=0; w<Wi+2*padding; w++)
			for(int c=0; c<Ci; c++){
				if(h<padding || h>=Hi+padding){
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = 0;
				}
				else if(w<padding || w>=Wi+padding){
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = 0;
				}
				else{
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = ddr[fea_ddr_addr+(h-padding)*Wi*Ci+(w-padding)*Ci+c];
				}
			}

	in_fixed *mat = new in_fixed[Ho*Wo*K*K*Ci];

	for(int i = 0; i < Ho; i=i+1)
		for(int j = 0; j < Wo; j=j+1)
			for(int kh = 0; kh < K; kh=kh+1)
				for(int kw=0; kw<K; kw=kw+1)
					for(int c = 0; c < Ci; c++){
						///update the new rule
						mat[i*Wo*K*K*Ci+j*K*K*Ci+kh*K*Ci+kw*Ci+c] =
								fea_pad[(i*S+kh)*(Wi+2*padding)*Ci+(j*S+kw)*Ci+c];
					}
////test  mat**** /////
//	in_fixed *mat1 = new in_fixed[Ho*Wo*K*K*Ci];
//	string     	lineStr, str;
//	in_fixed   	in_num;
//	double 		tmp;
//	int i = 0, k = 0;
//	ifstream inFile_torch_output("D:\\file\\pg\\xjw\\smvar_sim_2layers\\csv\\input_im2col.csv", ios::in);
//	i = 0;
//	while (getline(inFile_torch_output, lineStr)) {
//		stringstream ss(lineStr);
//		k = 0;
//	    while (getline(ss, str, ',')) {
//	    	tmp = atof(str.c_str());
//	        in_num = (in_fixed)tmp;
//	        mat1[i*K*K*Ci+k] = in_num;
//	        k++;
//	    }
//	    i++;
//	}
//    for(int i=0; i<Ho*Wo; i++){
//    	for (int j=0; j<K*K;j++){
//    		for(int k=0; k<Ci;k++){
//    			if(1)
//        		if((mat1[k+j*Ci+i*K*K*Ci]-mat[k+j*Ci+i*K*K*Ci])!=0 ){
//        				cout<<"wrong!! Hi*Wi:"<<i<<" K*K "<<j<<" Ci "<<k<<" mat:"<<mat[k+j*Ci+i*K*K*Ci]<<" mat1:"<<mat1[k+j*Ci+i*K*K*Ci]<<endl;
//        		}
//    		}
//    	}
//    }
////*********test1 **** /////

	int offset=0;
    for(int wh=0; wh<Ho*Wo; wh++){
    	for(int co=0; co<Co; co++){
    		in_fixed sss = 0;
    		//cout<<"wh = "<<wh<<" Co ="<<co<<endl;

    		int CB=Ci/32;
    		if(Ci/32*32<Ci)
    			CB=Ci/32+1;

    		for(int cb=0; cb<CB; cb++){
    			for(int i=0; i<K*K; i++){
    				in_fixed ss = 0;

    				int size_of_a_batch=32;
    	    		if((cb+1)*32>Ci){
    	    			size_of_a_batch=Ci%32;
    	    		}
    				for(int j=0; j<size_of_a_batch; j++){
    					ss = ss + ddr[mat_ddr_addr+cb*K*K*Co*32+i*Co*size_of_a_batch+co*size_of_a_batch+j]*mat[wh*K*K*Ci+i*Ci+cb*32+j];
    	    		}
    				sss = sss + in_fixed(ss);
    				if(wh==0 && co==0){
    					cout<<" sss: "<<sss<<endl;
    				}
    			}
    		}
    		ddr[res_ddr_addr+offset]=sss;
    		offset+=1;

    	}
    }
    delete[] mat;
    delete[] fea_pad;
}
