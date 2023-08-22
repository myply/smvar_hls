#include "flexdpe-para.h"
#include "smvar_isa_sim.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
void smvar_spmat_dma_isa_gen(
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
	unsigned short bloc_numb,
	int &isa_idx,
	int* isa_ddr
){

    ddr_step = ddr_step*4;
    for(int i=0; i<bloc_size; i=i+1){
        for(int j = 0; j<32; j++){
            if(ram_addr/32384 == 0){
                matSramData0[ram_addr-ram_addr/32384*32384+i*32+j] = ddr[ddr_addr+i*ddr_step+j*2+0];
                matSramIdx0[ram_addr-ram_addr/32384*32384+i*32+j] = ddr[ddr_addr+i*ddr_step+j*2+1];
            }
            else{
                matSramData1[ram_addr-ram_addr/32384*32384+i*32+j] = ddr[ddr_addr+i*ddr_step+j*2+0];
                matSramIdx1[ram_addr-ram_addr/32384*32384+i*32+j] = ddr[ddr_addr+i*ddr_step+j*2+1];
            }
        }
    }
	isa_idx = isa_idx + 1;
}
void smvar_mat_dma_isa_gen(
    in_fixed *ddr,
    in_fixed *matSram0,
    in_fixed *matSram1,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb,
	int &isa_idx,
	int* isa_ddr
){
//	cout<<"smvar_vec_dma_isa_sim"<<endl;
//	cout<<"isa_idx"<<isa_idx<<endl;
//	cout<<"ddr_addr="<<ddr_addr<<endl;
//	cout<<"ddr_step="<<ddr_step<<endl;
//	cout<<"ram_addr="<<ram_addr<<endl;
//	cout<<"ram_step="<<ram_step<<endl;
//	cout<<"bloc_size="<<bloc_size<<endl;
//	cout<<"bloc_numb="<<bloc_numb<<endl<<endl;
	//isa gen
	short isa_code = 0;
	short h_ddr_addr = ddr_addr/65536;
	short l_ddr_addr = ddr_addr - (ddr_addr/65536*65536);
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = h_ddr_addr;
	isa_ddr[isa_idx*8+2] = l_ddr_addr;
	isa_ddr[isa_idx*8+3] = ddr_step;
	isa_ddr[isa_idx*8+4] = ram_addr;
	isa_ddr[isa_idx*8+5] = ram_step;
	isa_ddr[isa_idx*8+6] = bloc_size;
	isa_ddr[isa_idx*8+7] = bloc_numb;
	isa_idx = isa_idx+ 1;

//	cout<<"smvar_mat_dma_isa_sim"<<endl;
//	if(ram_addr/16384 == 0)
//		cout<<"matSram0"<<endl;
//	else
//		cout<<"matSram1"<<endl;
	ram_addr = ram_addr - ram_addr/32768*32768;
    ddr_step = ddr_step*4;
//    cout<<"ddr_addr="<<ddr_addr<<endl;
//    cout<<"bloc_size="<<bloc_size<<endl;
//    cout<<"ram_addr="<<ram_addr<<endl;
//    cout<<"ddr_step="<<ddr_step<<endl;
    for(int i=0; i<bloc_size; i=i+1){
        for(int j = 0; j<ddr_step; j++){
            if(ram_addr/16384 == 0){
                matSram0[ram_addr-ram_addr/16384*16384+i*32+j] = ddr[ddr_addr+i*ddr_step+j];
//                cout<<matSram0[ram_addr-ram_addr/16384*16384+i*32+j]<<" ";
            }
            else{
                matSram1[ram_addr-ram_addr/16384*16384+i*32+j] = ddr[ddr_addr+i*ddr_step+j];
//                cout<<matSram1[ram_addr-ram_addr/16384*16384+i*32+j]<<" ";
            }
        }
//        cout<<endl;
    }
//    cout<<"smvar_mat_dma_isa_sim end"<<endl;


}
void smvar_vec_dma_isa_gen(
    in_fixed *ddr,
    in_fixed *vecSram0,
    in_fixed *vecSram1,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb,
	int &isa_idx,
	int* isa_ddr
){

//	cout<<"smvar_vec_dma_isa_sim"<<endl;
//	cout<<"isa_idx"<<isa_idx<<endl;
//	cout<<"ddr_addr="<<ddr_addr<<endl;
//	cout<<"ddr_step="<<ddr_step<<endl;
//	cout<<"ram_addr="<<ram_addr<<endl;
//	cout<<"ram_step="<<ram_step<<endl;
//	cout<<"bloc_size="<<bloc_size<<endl;
//	cout<<"bloc_numb="<<bloc_numb<<endl<<endl;
	//isa gen
	short isa_code = 1;
	short h_ddr_addr = 0;
	short l_ddr_addr = 0;
	h_ddr_addr = ddr_addr/65536;
	l_ddr_addr = ddr_addr - (ddr_addr/65536*65536);
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = h_ddr_addr;
	isa_ddr[isa_idx*8+2] = l_ddr_addr;
	isa_ddr[isa_idx*8+3] = ddr_step;
	isa_ddr[isa_idx*8+4] = ram_addr;
	isa_ddr[isa_idx*8+5] = ram_step;
	isa_ddr[isa_idx*8+6] = bloc_size;
	isa_ddr[isa_idx*8+7] = bloc_numb;
	isa_idx = isa_idx+ 1;




	ddr_step = ddr_step*4;
    short pad = ram_step/4096;
    ram_step = (ram_step-ram_step/4096*4096)*32;
    int r_ddr_addr;
    int r_bloc_step = bloc_numb/32*ddr_step;
    bloc_numb = bloc_numb - bloc_numb/32*32;
    int P=ram_addr%32768;
    ram_addr=ram_addr- ram_addr%32768;
//    cout<<"ram_addr_P= "<<P<<endl;
//    cout<<"ddr_addr="<<ddr_addr<<endl;
//    cout<<"ddr_step="<<ddr_step<<endl;
//    cout<<"r_bloc_step="<<r_bloc_step<<endl;
//    cout<<"ram_addr="<<ram_addr<<endl;
//    cout<<"ram_step="<<ram_step<<endl;
//    cout<<"bloc_size="<<bloc_size<<endl;
//    cout<<"bloc_numb="<<bloc_numb<<endl;

    if(P==0){
        for(int bloc = 0; bloc < bloc_numb; bloc++){
            for(int i=0; i<bloc_size; i++){
                for(int j=0; j<32; j++){
                    if(ram_addr/32768==0){
                        vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                    }
                    else{
                        vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                    }
                }
            }
        }
    }else{
        if(pad == 5){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                        	//cout<<ddr_addr+bloc*r_bloc_step+i*ddr_step+j<<endl;
                        	//cout<<ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j<<endl;
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];

                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 7){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                        	//cout<<ddr_addr+bloc*r_bloc_step+i*ddr_step+j<<endl;
                        	//cout<<ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j<<endl;
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];

                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 1){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 3){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 9){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == bloc_size - 1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == bloc_size - 1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 11){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == bloc_size - 1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(bloc == bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == bloc_size - 1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 13){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size - 1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }

                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size - 1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 14){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size - 1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc == bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size - 1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 4){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 12){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            else if(i == bloc_size-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 0){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                        }
                    }
                }
            }
        }
        else if(pad == 8){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i==bloc_size-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i==bloc_size-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 6){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 2){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc== bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(bloc== bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 10){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == bloc_size - 1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == bloc_size - 1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else if(pad == 15){
            for(int bloc = 0; bloc < bloc_numb; bloc++){
                for(int i=0; i<bloc_size; i++){
                    for(int j=0; j<32; j++){
                        if(ram_addr/32768==0){
                            vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == bloc_size - 1 || i == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1 || bloc == 0){
                                vecSram0[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                        else{
                            vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = ddr[ddr_addr+bloc*r_bloc_step+i*ddr_step+j];
                            if(i == bloc_size - 1 || i == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                            if(bloc== bloc_numb-1 || bloc == 0){
                                vecSram1[ram_addr-ram_addr/32768*32768+bloc*ram_step+i*32+j] = 0;
                            }
                        }
                    }
                }
            }
        }
        else{
            cout<<"error !!!!"<<pad<<endl;
        }
    }
//    if(ram_addr/32768==0)
//    	cout<<"vecsram0 = "<<endl;
//    else
//    	cout<<"vecsram1 = "<<endl;
//    cout<<"smvar_vec_dma_isa vecsram"<<endl;
//    cout<<"ci 0"<<endl;
//    for(int i=0; i<12; i++){
//    	cout<<"row = "<<i<<" =";
//    	for(int j=0; j<12; j++){
//    		if(ram_addr/32768==0)
//    			cout<<vecSram0[i*12*32+j*32+0]<<" ";
//    		else
//    			cout<<vecSram1[i*12*32+j*32+0]<<" ";
//    	}
//    	cout<<endl;
//    }
//    cout<<"ci 1"<<endl;
//    for(int i=0; i<12; i++){
//    	cout<<"row = "<<i<<" =";
//    	for(int j=0; j<12; j++){
//    		if(ram_addr/32768==0)
//    			cout<<vecSram0[i*12*32+j*32+1]<<" ";
//    		else
//    			cout<<vecSram1[i*12*32+j*32+1]<<" ";
//    	}
//    	cout<<endl;
//    }
//    if(ram_addr/32768==0)
//    	cout<<"vecsram0 = "<<endl;
//    else
//    	cout<<"vecsram1 = "<<endl;
//    cout<<"smvar_vec_dma_isa vecsram"<<endl;
//    cout<<"ci 0"<<endl;
//    for(int i=0; i<26; i++){
//    	cout<<"row = "<<i<<" =";
//    	for(int j=0; j<26; j++){
//    		if(ram_addr/32768==0)
//    			cout<<vecSram0[i*26*32+j*32+0]<<" ";
//    		else
//    			cout<<vecSram1[i*26*32+j*32+0]<<" ";
//    	}
//    	cout<<endl;
//    }
//    cout<<"ci 1"<<endl;
//    for(int i=0; i<26; i++){
//    	cout<<"row = "<<i<<" =";
//    	for(int j=0; j<26; j++){
//    		if(ram_addr/32768==0)
//    			cout<<vecSram0[i*26*32+j*32+1]<<" ";
//    		else
//    			cout<<vecSram1[i*26*32+j*32+1]<<" ";
//    	}
//    	cout<<endl;
//    }

}
void smvar_vec_load_isa_gen(
    in_fixed *vecSram0,
    in_fixed *vecSram1,
    in_fixed **vecRegs0,
    in_fixed **vecRegs1,
	unsigned short src_addr,
	unsigned short src_stride,
	unsigned short src_step,
	unsigned short dest_addr,
	unsigned short dest_step,
	unsigned short src_bloc_size,
	unsigned short src_bloc_numb,
	unsigned short dest_bloc_size,
	unsigned short dest_bloc_numb,
	int &isa_idx,
	int* isa_ddr
){

//	cout<<"smvar_vec_load_isa_sim"<<endl;
//	cout<<"isa_idx"<<isa_idx<<endl;
//	cout<<"src_addr="<<src_addr<<endl;
//	cout<<"src_stride="<<src_stride<<endl;
//	cout<<"src_step="<<src_step<<endl;
//	cout<<"dest_addr="<<dest_addr<<endl;
//	cout<<"src_step="<<src_step<<endl;
//	cout<<"src_bloc_size="<<src_bloc_size<<endl;
//	cout<<"src_bloc_numb="<<src_bloc_numb<<endl;
//	cout<<"dest_bloc_size="<<dest_bloc_size<<endl;
//	cout<<"dest_bloc_numb="<<dest_bloc_numb<<endl<<endl;
	//isa gen
	short isa_code = 14;
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = src_addr;
	isa_ddr[isa_idx*8+2] = src_stride;
	isa_ddr[isa_idx*8+3] = src_step;
	isa_ddr[isa_idx*8+4] = dest_addr;
	isa_ddr[isa_idx*8+5] = dest_step;
	isa_ddr[isa_idx*8+6] = src_bloc_size*16+src_bloc_numb;
	isa_ddr[isa_idx*8+7] = dest_bloc_size*16+dest_bloc_numb;
	isa_idx+=1;


    src_step = src_step*32;
    int src_pp_flag = src_addr/32768;
    int dest_pp_flag = dest_addr/32768;
    src_addr = (src_addr-src_addr/32768*32768)*32;
    dest_addr = dest_addr - dest_addr/32768*32768;
//    cout<<"smvar_vec_load_isa "<<" src_addr "<<src_addr<<endl;
//    cout<<"smvar_vec_load_isa "<<" src_step "<<src_step<<endl;
//    cout<<"smvar_vec_load_isa "<<" src_bloc_numb "<<src_bloc_numb<<endl;
//    cout<<"smvar_vec_load_isa "<<" src_stride "<<src_stride<<endl;
	for(int cu = 0; cu < 12; cu++){
        for(int bloc = 0; bloc < src_bloc_numb; bloc++){
            for(int i=0; i < src_bloc_size; i++){
                for(int j=0; j < 32; j++){
                    if(src_pp_flag == 0 && dest_pp_flag == 0)
                        vecRegs0[cu][bloc*src_bloc_size*32+i*32+j] = vecSram0[src_addr+src_step*bloc+(i+cu*src_stride)*32+j];
                    else if(src_pp_flag == 0 && dest_pp_flag == 1)
                        vecRegs1[cu][bloc*src_bloc_size*32+i*32+j] = vecSram0[src_addr+src_step*bloc+i*32+cu*src_stride*32+j];
                    else if(src_pp_flag == 1 && dest_pp_flag == 0)
                        vecRegs0[cu][bloc*src_bloc_size*32+i*32+j] = vecSram1[src_addr+src_step*bloc+i*32+cu*src_stride*32+j];
                    else
                        vecRegs1[cu][bloc*src_bloc_size*32+i*32+j] = vecSram1[src_addr+src_step*bloc+i*32+cu*src_stride*32+j];
                }
            }
        }
    }
//	cout<<"smvar_vec_load_isa "<<" vecSram0 "<<src_addr<<endl;
//	if(src_pp_flag == 0){
//		cout<<"vecSram0::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//				cout<<vecSram0[src_addr+i*384+j]<<" ";
//			}
//			cout<<endl;
//		}
//	}
//	else{
//		cout<<"vecSram1::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//				cout<<vecSram1[src_addr+i*384+j]<<" ";
//			}
//			cout<<endl;
//		}
//	}

//	cout<<"smvar_vec_load_isa "<<" vecRegs "<<src_addr<<endl;
//	if(dest_pp_flag == 0){
//		cout<<"vecRegs0::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//				cout<<vecRegs0[i][j]<<" ";
//			}
//			cout<<endl;
//		}
//	}
//	else{
//		cout<<"vecRegs1::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//				cout<<vecRegs1[i][j]<<" ";
//			}
//			cout<<endl;
//		}
//	}
//	cout<<"smvar_vec_load_isa "<<" vecRegs "<<src_addr<<endl;
//	if(dest_pp_flag == 0){
//		cout<<"vecRegs0::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<12;j++){
//				for(int k=0;k<32;k++){
//					cout<<vecRegs0[j][i*32+k]<<" ";
//				}
//			}
//			cout<<endl;
//		}
//	}
//	else{
//		cout<<"vecRegs1::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<12;j++){
//				for(int k=0;k<32;k++){
//					cout<<vecRegs1[j][i*32+k]<<" ";
//				}
//			}
//			cout<<endl;
//		}
//	}

}
void smvar_mul_mult_isa_gen(
    in_fixed *matSram0,
    in_fixed *matSram1,
    in_fixed **vecRegs0,
    in_fixed **vecRegs1,
    in_fixed **SumSram,
    in_fixed **ResVSram,
    in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	int exe_src0_addr,
	//unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link,
	int &isa_idx,
	int* isa_ddr
){
//	cout<<" mul_isa_gen: "<<endl;
//	cout<<" isa_idx: "<<isa_idx<<endl;
//	cout<<" exe_src0_addr: "<<exe_src0_addr<<endl;
//	cout<<" exe_src1_addr: "<<exe_src1_addr<<endl;
//	cout<<" exe_src1_size: "<<exe_src1_size<<endl;
//	cout<<" exe_dest_addr: "<<exe_dest_addr<<endl;
//	cout<<" exe_dest_size: "<<exe_dest_size<<endl;
//	cout<<" exe_dest_acti: "<<exe_dest_acti<<endl;
//	cout<<" exe_dest_link: "<<exe_dest_link<<endl;
//	cout<<endl;

	//src0_addr may be changed to short
	short isa_code = 3;
	short h_exe_src0_addr = 0;
	short l_exe_src0_addr = 0;
	h_exe_src0_addr = exe_src0_addr/65536;
	l_exe_src0_addr = exe_src0_addr - (exe_src0_addr/65536*65536);
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = h_exe_src0_addr;
	isa_ddr[isa_idx*8+2] = l_exe_src0_addr;
	isa_ddr[isa_idx*8+3] = exe_src0_size;
	isa_ddr[isa_idx*8+4] = exe_src1_addr;
	isa_ddr[isa_idx*8+5] = exe_src1_size;
	isa_ddr[isa_idx*8+6] = exe_dest_addr;
	isa_ddr[isa_idx*8+7] = exe_dest_size*8+exe_dest_acti*2+exe_dest_link;
	isa_idx+=1;

//	in_fixed vecReg[12][33];
//	in_fixed matReg[32];
//	in_fixed res_bus[1280];

	int src0_pp_flag = (exe_src0_addr - exe_src0_addr/32768*32768)/16384;
	exe_src0_addr = exe_src0_addr - exe_src0_addr/32768*32768;
	exe_src0_addr = (exe_src0_addr - exe_src0_addr/16384*16384)*32;
	int src1_pp_flag = (exe_src1_addr-exe_src1_addr/32768*32768)/16384;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/32768*32768;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/16384*16384;
	int res_mv_flag = exe_dest_addr/32768;
	exe_dest_addr = exe_dest_addr - exe_dest_addr/32768*32768;

	for(int cu=0; cu<12; cu++){
	    for(int j=0; j<32; j++){
	        if(src1_pp_flag == 0)
	            vecReg[cu][j] = vecRegs0[cu][exe_src1_addr*32+j];
	        else
	            vecReg[cu][j] = vecRegs1[cu][exe_src1_addr*32+j];
	    }
	    vecReg[cu][32] = 0;
	}

//	in_fixed sum[12];


	for(int cu=0; cu<12; cu++){
		for(int i = 0; i<exe_src0_size; i++){
			sum[cu] = 0;
			for(int j=0; j<32;j++){
				if(src0_pp_flag==0){
					matReg[j] = matSram0[exe_src0_addr+i*32+j];
				}
				else{
					matReg[j] = matSram1[exe_src0_addr+i*32+j];
				}
				sum[cu] = sum[cu] + vecReg[cu][j]*matReg[j];
				//if(cu == 0)
				//	cout<<matReg[j]<<" ";
			}
			//if(cu == 0)
			//	cout<<endl;
			res_bus[i] = sum[cu];
			//cout<<res_bus[i]<<" ";

		}

//		if(res_mv_flag==0&&exe_dest_link==0)
//			cout<<"ResVSram =";
//		else if(exe_dest_link==0)
//			cout<<"ResMSram =";
		for(int i = 0; i < exe_dest_size*32; i++){
			if(exe_dest_link){
				SumSram[cu][i] = SumSram[cu][i] + res_bus[i];
			}
			else{
				if(res_mv_flag==0)
					ResVSram[cu][exe_dest_addr*32+i] = SumSram[cu][i]+res_bus[i];
				else
					ResMSram[cu][exe_dest_addr*32+i] = SumSram[cu][i]+res_bus[i];
				SumSram[cu][i] = 0;
//				if(res_mv_flag==0)
//					cout<<" addr:"<<exe_dest_addr*32+i<<" vals:"<<ResVSram[cu][exe_dest_addr*32+i]<<" ";
//				else
//					cout<<" addr:"<<exe_dest_addr*32+i<<" vals:"<<ResMSram[cu][exe_dest_addr*32+i]<<" ";
			}

		}
//		if(exe_dest_link==0)
//			cout<<endl;
	}
}
void smvar_mul_mult_isa_gen_test(
    in_fixed *matSram0,
    in_fixed *matSram1,
    in_fixed **vecRegs0,
    in_fixed **vecRegs1,
    in_fixed **SumSram,
    in_fixed **ResVSram,
    in_fixed **ResMSram,
	in_fixed ** vecReg,
	in_fixed *matReg,
	in_fixed *res_bus,
	in_fixed *sum,
	int exe_src0_addr,
	//unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link,
	int &isa_idx,
	int* isa_ddr
){
//	in_fixed vecReg[12][33];
//	in_fixed matReg[32];
//	in_fixed res_bus[1024];

	int src0_pp_flag = (exe_src0_addr - exe_src0_addr/32768*32768)/16384;
	exe_src0_addr = exe_src0_addr - exe_src0_addr/32768*32768;
	exe_src0_addr = (exe_src0_addr - exe_src0_addr/16384*16384)*32;
	int src1_pp_flag = (exe_src1_addr-exe_src1_addr/32768*32768)/16384;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/32768*32768;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/16384*16384;
	int res_mv_flag = exe_dest_addr/32768;
	exe_dest_addr = exe_dest_addr - exe_dest_addr/32768*32768;
    cout<<"This is mul isa test version!!!!"<<endl;
	for(int cu=0; cu<12; cu++){
	    for(int j=0; j<32; j++){
	        if(src1_pp_flag == 0)
	            vecReg[cu][j] = vecRegs0[cu][exe_src1_addr*32+j];
	        else
	            vecReg[cu][j] = vecRegs1[cu][exe_src1_addr*32+j];
	    }
	    vecReg[cu][32] = 0;
//	    cout<<vecReg[cu][8]<<" ";
	}

	//check vecreg
//	if(src1_pp_flag == 0){
//		cout<<"vecRegs0::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<12;j++){
//				for(int k=0;k<32;k++){
//					cout<<vecRegs0[j][i*32+k]<<" ";
//				}
//			}
//			cout<<endl;
//		}
//	}
//	else{
//		cout<<"vecRegs1::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<12;j++){
//				for(int k=0;k<32;k++){
//					cout<<vecRegs1[j][i*32+k]<<" ";
//				}
//			}
//			cout<<endl;
//		}
//	}
//	if(src1_pp_flag == 0){
//		cout<<"vecRegs0::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//					cout<<vecRegs0[i][j]<<" ";
//				}
//			cout<<endl;
//		}
//	}
//	else{
//		cout<<"vecRegs1::"<<endl;
//		for(int i=0;i<12;i++){
//			for (int j=0;j<384;j++){
//					cout<<vecRegs1[i][j]<<" ";
//				}
//			cout<<endl;
//		}
//	}
//
//	cout<<"vecReg::"<<endl;
//	for(int cu=0; cu<12; cu++){
//	    for(int j=0; j<32; j++){
//	            cout<<vecReg[cu][j]<<" ";
//	    }
//	    cout<<endl;
//	}

	cout<<endl;
//	in_fixed sum[12];
    cout<<"smvar_mul_mult_isa "<<" exe_dest_size "<<exe_dest_size<<endl;
    cout<<"smvar_mul_mult_isa "<<" exe_dest_addr "<<exe_dest_addr<<endl;
    cout<<"smvar_mul_mult_isa "<<" exe_src0_size "<<exe_src0_size<<endl;
    cout<<"smvar_mul_mult_isa "<<" exe_src0_addr "<<exe_src0_addr<<endl;
    cout<<" SumSram[0][0] before mul:"<<SumSram[0][0]<<endl;

    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<1024; i++){
    			cout<<SumSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
    cout<<" ResVSram[0][0] before mul:"<<ResVSram[0][0]<<endl;
    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<3072; i++){
    			cout<<ResVSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
    cout<<" ResMSram[0][0] before mul:"<<ResMSram[0][0]<<endl;
    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<3072; i++){
    			cout<<ResMSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
//	if(src0_pp_flag==0){
//		cout<<"matsram0:"<<endl;
//		for(int i=0;i<64;i++){
//			for (int j=0;j<32;j++){
//					cout<<matSram0[i*32+j]<<" ";
//			}
//			cout<<endl;
//		}
//	}else{
//		cout<<"matsram1:"<<endl;
//		for(int i=0;i<64;i++){
//			for (int j=0;j<32;j++){
//					cout<<matSram1[i*32+j]<<" ";
//			}
//			cout<<endl;
//		}
//	}
	for(int cu=0; cu<12; cu++){
		//exe_src0_size=Co
		for(int i = 0; i<exe_src0_size; i++){
			sum[cu] = 0;
			for(int j=0; j<32;j++){
				if(src0_pp_flag==0){
					matReg[j] = matSram0[exe_src0_addr+i*32+j];
				}
				else{
					matReg[j] = matSram1[exe_src0_addr+i*32+j];
				}
				sum[cu] = sum[cu] + vecReg[cu][j]*matReg[j];
			}

			if(i==0 &&cu==0){
				cout<<" vecReg[0][j]:"<<endl;
				for (int j=0;j<32;j++){
					cout<<vecReg[0][j]<<" ";
				}
				cout<<endl;
			}
			if(i==0 &&cu==0){
				cout<<" matReg:"<<endl;
				for (int j=0;j<32;j++){
					cout<<matReg[j]<<" ";
				}
				cout<<endl;
			}

			res_bus[i] = sum[cu];
//			cout<<res_bus[i]<<" ";

		}
		cout<<" res_bus:"<<" cu "<<cu<<endl;
		for(int i = 0; i<1024; i++){
			cout<<" i "<<i<<" "<<res_bus[i]<<" ";
			}
		cout<<endl;
//		if(res_mv_flag==0&&exe_dest_link==0)
//			cout<<"ResVSram ="<<endl;
//		else if(exe_dest_link==0)
//			cout<<"ResMSram ="<<endl;

		//exe_dest_size*32  equals Co
		for(int i = 0; i < exe_dest_size*32; i++){
			if(exe_dest_link){
				SumSram[cu][i] = SumSram[cu][i] + res_bus[i];
			}
			else{
//				if(cu==0 && (exe_dest_addr*32+i)==0)
//					cout<<" SumSram[0][0] before"<<SumSram[cu][i]<<" SumSram[0][0] after"<<SumSram[cu][i]+res_bus[i]<<endl;
				if(res_mv_flag==0)
					ResVSram[cu][exe_dest_addr*32+i] = SumSram[cu][i]+res_bus[i];
				else
					ResMSram[cu][exe_dest_addr*32+i] = SumSram[cu][i]+res_bus[i];
				SumSram[cu][i] = 0;
//				if(res_mv_flag==0)
//					cout<<" addr:"<<exe_dest_addr*32+i<<" vals:"<<ResVSram[cu][exe_dest_addr*32+i]<<" ";
//				else
//					cout<<" addr:"<<exe_dest_addr*32+i<<" vals:"<<ResMSram[cu][exe_dest_addr*32+i]<<" ";
			}

		}
//		cout<<endl;
		if(exe_dest_link==0)
			cout<<endl;
	}

    cout<<" ResVSram[0][0] after mul:"<<ResVSram[0][0]<<endl;
    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<3072; i++){
    			cout<<ResVSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
    cout<<" ResMSram[0][0] after mul:"<<ResMSram[0][0]<<endl;
    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<3072; i++){
    			cout<<ResMSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
	cout<<" SumSram[0][0] after mul:"<<SumSram[0][0]<<endl;
    for(int cu=0; cu<12; cu++){
    		//exe_src0_size=64
    		for(int i = 0; i<1024; i++){
    			cout<<SumSram[cu][i]<<" ";
    			}
    		cout<<endl;
    		}
}
void smvar_fence_isa_gen(
){
//	cout<<"this is fence !!!!"<<endl;
}


void smvar_vec_add_isa_gen(
    in_fixed **ResVSram,
    in_fixed **ResMSram,
	unsigned short exe_src0_addr,
	unsigned short exe_src0_size,
	unsigned short exe_src1_addr,
	unsigned short exe_src1_size,
	unsigned short exe_dest_addr,
	unsigned short exe_dest_size,
	unsigned short exe_dest_acti,
	unsigned short exe_dest_link,
	int &isa_idx,
	int* isa_ddr
){

	short isa_code = 5;
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = short(0);
	isa_ddr[isa_idx*8+2] = exe_src0_addr;
	isa_ddr[isa_idx*8+3] = exe_src0_size;
	isa_ddr[isa_idx*8+4] = exe_src1_addr;
	isa_ddr[isa_idx*8+5] = exe_src1_size;
	isa_ddr[isa_idx*8+6] = exe_dest_addr;
	isa_ddr[isa_idx*8+7] = exe_dest_size*8+exe_dest_acti*2+exe_dest_link;
	isa_idx+=1;

	exe_src0_addr = exe_src0_addr - exe_src0_addr/32768*32768;
	exe_src0_addr = exe_src0_addr - exe_src0_addr/16384*16384;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/32768*32768;
	exe_src1_addr = exe_src1_addr - exe_src1_addr/16384*16384;

	exe_dest_addr = exe_dest_addr - exe_dest_addr/32768*32768;

	exe_src0_size = exe_src0_size*32;

	for(int cu=0; cu<12; cu++){
        for(int i=0; i<exe_src0_size; i++){
//        	if((exe_dest_addr+i)==0 && cu==0){
//        		cout<<"smvar_vec_add_isa_sim "<<endl;
//        		cout<<" exe_src0_size "<<exe_src0_size<<endl;
//        		cout<<" exe_src1_addr "<<exe_src1_addr<<endl;
//        		cout<<" exe_src0_addr "<<exe_src0_addr<<endl;
//        		cout<<" exe_dest_addr "<<exe_dest_addr<<endl;
//        		cout<<" i"<<i<<" "<<ResMSram[cu][exe_src0_addr+i]<<"+"<<ResVSram[cu][exe_src1_addr+i]<<"=";
//        	}
            ResVSram[cu][exe_dest_addr+i] = ResMSram[cu][exe_src0_addr+i]+ResVSram[cu][exe_src1_addr+i];
//            if((exe_dest_addr+i)==0 && cu==0)
//            	cout<<ResVSram[cu][exe_dest_addr+i]<<endl;
        }
        //cout<<endl;
    }
}


//have different
void smvar_res_dma_isa_gen(
	in_fixed *ddr,
	in_fixed ** ResVSram,
	in_fixed * biasSram,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short acti,
	unsigned short bias,
	unsigned short bloc_size,
	unsigned short bloc_numb,
	int &isa_idx,
	int* isa_ddr
){
//	cout<<"smvar_res_dma_isa_sim"<<endl;
//	cout<<"isa_idx"<<isa_idx<<endl;
//	cout<<"ddr_addr="<<ddr_addr<<endl;
//	cout<<"ddr_step="<<ddr_step<<endl;
//	cout<<"ram_addr="<<ram_addr<<endl;
//	cout<<"ram_step="<<ram_step<<endl;
//	cout<<"acti="<<acti<<endl;
//	cout<<"bias="<<bias<<endl;
//	cout<<"bloc_size="<<bloc_size<<endl;
//	cout<<"bloc_numb="<<bloc_numb<<endl<<endl;
	short isa_code = 31;
	short h_ddr_addr = 0;
	short l_ddr_addr = 0;
	h_ddr_addr = ddr_addr/65536;
	l_ddr_addr = ddr_addr - (ddr_addr/65536*65536);
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = h_ddr_addr;
	isa_ddr[isa_idx*8+2] = l_ddr_addr;
	isa_ddr[isa_idx*8+3] = ddr_step;
	isa_ddr[isa_idx*8+4] = ram_addr;
	isa_ddr[isa_idx*8+5] = ram_step;
	isa_ddr[isa_idx*8+6] = bloc_size+bias*4096+acti*8192;
	isa_ddr[isa_idx*8+7] = bloc_numb;
	isa_idx+=1;


//	cout<<"smvar_res_dma_isa_sim "<<" bloc_size "<<bloc_size<<endl;
//	cout<<"smvar_res_dma_isa_sim "<<" bloc_size "<<bloc_size+bias*4096+acti*8192<<endl;
    ddr_step = ddr_step * 4;
    unsigned short ram_pad  = ram_step/4096;
    ram_step = ram_step - ram_step/4096*4096;
    bloc_size = bloc_size*32;
    unsigned short bloc_step = bloc_size*(bloc_numb/32);
    bloc_numb = bloc_numb-bloc_numb/32*32;
//    cout<<"smvar_res_dma_isa_sim "<<" ram_pad "<<ram_pad<<endl;
//    cout<<"smvar_res_dma_isa_sim "<<" bloc_numb "<<bloc_numb<<endl;
//    cout<<"smvar_res_dma_isa_sim "<<" bloc_step "<<bloc_step<<endl;
//    cout<<"smvar_res_dma_isa_sim "<<" bloc_size "<<bloc_size<<endl;
    for(int i=0; i<bloc_numb; i++){
    	for(int j=0; j<ram_pad; j++){
    		for(int co=0; co<bloc_size; co++){
    			///correct the code ,32---bloc_size
    			if(bias==1){
    				ddr[ddr_addr+i*bloc_step+j*bloc_size+co]=ResVSram[j][i*bloc_size+co]+biasSram[co];
    			}else{
        			ddr[ddr_addr+i*bloc_step+j*bloc_size+co]=ResVSram[j][i*bloc_size+co];
    			}
    			if(acti==4){
    				in_fixed temp=ddr[ddr_addr+i*bloc_step+j*bloc_size+co];

    				ddr[ddr_addr+i*bloc_step+j*bloc_size+co]=temp/(1+exp(-temp));
    			}
    			///correct the code ,32---bloc_size
//    			ddr[ddr_addr+i*bloc_step+j*bloc_size+co]=ResVSram[j][i*32+co];

    			//res_ddr_addr+wh*Co+co
//    			if((ddr_addr+i*bloc_step+j*bloc_size+co)==2097152){
//    				cout<<"i is "<<i<<endl;
//    				cout<<"j is "<<j<<endl;
//    				cout<<"co is "<<co<<endl;
//    				cout<<"resaddr is "<<ddr_addr+i*bloc_step+j*bloc_size+co<<endl;
//    				cout<<"ResVSram addr is "<<i*bloc_size+co<<endl;
//    				cout<<"val is "<<ddr[ddr_addr+i*bloc_step+j*bloc_size+co]<<endl;
//    			}
    		}
    	}
    }

}


void smvar_bias_dma_isa_gen(
	in_fixed *ddr,
	in_fixed * biasSram,
	int ddr_addr,
	unsigned short ddr_step,
	unsigned short ram_addr,
	unsigned short ram_step,
	unsigned short bloc_size,
	unsigned short bloc_numb,
	int &isa_idx,
	int* isa_ddr
){
//	cout<<"smvar_bias_dma_isa_sim"<<endl;
//	cout<<"isa_idx"<<isa_idx<<endl;
//	cout<<"ddr_addr="<<ddr_addr<<endl;
//	cout<<"ddr_step="<<ddr_step<<endl;
//	cout<<"ram_addr="<<ram_addr<<endl;
//	cout<<"ram_step="<<ram_step<<endl;
//	cout<<"bloc_size="<<bloc_size<<endl;
//	cout<<"bloc_numb="<<bloc_numb<<endl<<endl;
	short isa_code = 2;
	short h_ddr_addr = 0;
	short l_ddr_addr = 0;
	h_ddr_addr = ddr_addr/65536;
	l_ddr_addr = ddr_addr - (ddr_addr/65536*65536);
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = h_ddr_addr;
	isa_ddr[isa_idx*8+2] = l_ddr_addr;
	isa_ddr[isa_idx*8+3] = ddr_step;
	isa_ddr[isa_idx*8+4] = ram_addr;
	isa_ddr[isa_idx*8+5] = ram_step;
	isa_ddr[isa_idx*8+6] = bloc_size;
	isa_ddr[isa_idx*8+7] = bloc_numb;
	isa_idx+=1;


	bloc_size = bloc_size*32;

	for(int co=0; co<bloc_size; co++){
		biasSram[co]=ddr[ddr_addr+co];
	}

}
void smvar_isend_isa_gen(
	int &isa_idx,
	int* isa_ddr
){
	short isa_code = 30;
	isa_ddr[isa_idx*8+0] = isa_code;
	isa_ddr[isa_idx*8+1] = short(0);
	isa_ddr[isa_idx*8+2] = short(0);
	isa_ddr[isa_idx*8+3] = short(0);
	isa_ddr[isa_idx*8+4] = short(0);
	isa_ddr[isa_idx*8+5] = short(0);
	isa_ddr[isa_idx*8+6] = short(0);
	isa_ddr[isa_idx*8+7] = short(0);

	isa_idx+=1;
}
