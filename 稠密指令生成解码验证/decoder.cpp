#include "flexdpe-para.h"
#include "fixed2hex.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "smvar_isa_sim.h"
#include "smvar_isa_gen.h"
#include "sub_function.h"
using namespace std;

int main(){
	int Ci,Co,K,Hi,Wi,stride,Ho,Wo,padding;
	int isa_idx = 0;
    int isa_count = 0;

	in_fixed    *VecSram0 = new in_fixed[26*26*32];
    in_fixed    *VecSram1 = new in_fixed[26*26*32];
    in_fixed    *MatSram0 = new in_fixed[9*1280*32];
    in_fixed    *MatSram1 = new in_fixed[9*1280*32];
    in_fixed   **VecRegs0 = new in_fixed*[12];
    in_fixed   **VecRegs1 = new in_fixed*[12];
    in_fixed	**SumSram = new in_fixed*[12];
    in_fixed    **ResVSram = new in_fixed*[12];
    in_fixed    **ResMSram = new in_fixed*[12];
    in_fixed    *ddr = new in_fixed[65536*32*160];

	in_fixed **vecReg=new in_fixed*[12];
	in_fixed *matReg=new in_fixed[32];
	in_fixed *res_bus=new in_fixed[1280];
	in_fixed *sum=new in_fixed[12];

	in_fixed *biasSram=new in_fixed[1024];


    int * isa_ddr=new int[65536*32*32];
    int vec_ddr_addr = 65536*32;//store isa code
    int res_ddr_addr = 65536*32*32+16384;//
    int mat_ddr_addr = 65536*32*96;
    int bias_ddr_addr = 65536*32*72-65536;
    for(int i=0;i<12;i++){
    	//288=32*9
//    	VecRegs0[i] = new in_fixed[288];
//    	VecRegs1[i] = new in_fixed[288];
    	//384=32*12
    	VecRegs0[i] = new in_fixed[384];
    	VecRegs1[i] = new in_fixed[384];
    	SumSram[i] = new in_fixed[2560];
    	ResVSram[i] = new in_fixed[2560*12];
    	ResMSram[i] = new in_fixed[2560*12];
    	vecReg[i]=new in_fixed[33];
    }


    store_fea_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\input\\input0.csv",12,112,176,26673152);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight0.csv",32,12,3,201326592);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias0.csv",32,208389120);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight1.csv",64,32,3,201335808);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias1.csv",64,208389152);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight2.csv",32,64,1,201354240);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias2.csv",32,208389216);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight3.csv",32,32,1,201356288);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias3.csv",32,208389248);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight4.csv",32,32,3,201357312);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias4.csv",32,208389280);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight5.csv",32,64,1,201366528);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias5.csv",32,208389312);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight6.csv",64,64,1,201368576);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias6.csv",64,208389344);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight7.csv",128,64,3,201372672);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias7.csv",128,208389408);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight8.csv",64,128,1,201446400);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias8.csv",64,208389536);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight9.csv",64,64,1,201454592);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias9.csv",64,208389600);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight10.csv",64,64,3,201458688);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias10.csv",64,208389664);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight11.csv",64,64,1,201495552);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias11.csv",64,208389728);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight12.csv",64,64,3,201499648);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias12.csv",64,208389792);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight13.csv",64,64,1,201536512);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias13.csv",64,208389856);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight14.csv",64,64,3,201540608);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias14.csv",64,208389920);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight15.csv",64,128,1,201577472);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias15.csv",64,208389984);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight16.csv",128,128,1,201585664);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias16.csv",128,208390048);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight17.csv",256,128,3,201602048);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias17.csv",256,208390176);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight18.csv",128,256,1,201896960);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias18.csv",128,208390432);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight19.csv",128,128,1,201929728);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias19.csv",128,208390560);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight20.csv",128,128,3,201946112);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias20.csv",128,208390688);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight21.csv",128,128,1,202093568);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias21.csv",128,208390816);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight22.csv",128,128,3,202109952);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias22.csv",128,208390944);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight23.csv",128,128,1,202257408);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias23.csv",128,208391072);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight24.csv",128,128,3,202273792);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias24.csv",128,208391200);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight25.csv",128,256,1,202421248);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias25.csv",128,208391328);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight26.csv",256,256,1,202454016);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias26.csv",256,208391456);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight27.csv",512,256,3,202519552);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias27.csv",512,208391712);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight28.csv",256,512,1,203699200);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias28.csv",256,208392224);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight29.csv",512,1024,1,203830272);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias29.csv",512,208392480);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight30.csv",256,512,1,204354560);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias30.csv",256,208392992);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight31.csv",256,256,1,204485632);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias31.csv",256,208393248);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight32.csv",256,256,3,204551168);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias32.csv",256,208393504);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight33.csv",256,512,1,205140992);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias33.csv",256,208393760);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight34.csv",512,512,1,205272064);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias34.csv",512,208394016);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight35.csv",256,512,1,205534208);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias35.csv",256,208394528);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight36.csv",128,512,1,205665280);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias36.csv",128,208394784);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight37.csv",128,128,1,205730816);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias37.csv",128,208394912);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight38.csv",128,128,3,205747200);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias38.csv",128,208395040);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight39.csv",128,512,1,205894656);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias39.csv",128,208395168);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight40.csv",256,256,1,205960192);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias40.csv",256,208395296);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight41.csv",128,256,1,206025728);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias41.csv",128,208395552);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight42.csv",64,256,1,206058496);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias42.csv",64,208395680);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight43.csv",64,64,1,206074880);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias43.csv",64,208395744);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight44.csv",64,64,3,206078976);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias44.csv",64,208395808);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight45.csv",64,256,1,206115840);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias45.csv",64,208395872);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight46.csv",128,128,1,206132224);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias46.csv",128,208395936);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight47.csv",128,128,3,206148608);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias47.csv",128,208396064);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight48.csv",128,256,1,206296064);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias48.csv",128,208396192);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight49.csv",128,128,1,206328832);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias49.csv",128,208396320);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight50.csv",128,128,3,206345216);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias50.csv",128,208396448);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight51.csv",128,256,1,206492672);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias51.csv",128,208396576);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight52.csv",256,256,1,206525440);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias52.csv",256,208396704);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight53.csv",256,256,3,206590976);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias53.csv",256,208396960);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight54.csv",256,512,1,207180800);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias54.csv",256,208397216);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight55.csv",256,256,1,207311872);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias55.csv",256,208397472);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight56.csv",256,256,3,207377408);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias56.csv",256,208397728);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight57.csv",256,512,1,207967232);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias57.csv",256,208397984);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight58.csv",512,512,1,208098304);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias58.csv",512,208398240);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight59.csv",18,128,1,208360448);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias59.csv",18,208398752);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight60.csv",18,256,1,208364544);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias60.csv",18,208398784);
    store_weight_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\weight\\weight61.csv",18,512,1,208372736);
    store_bias_from_csv_2_ddr(ddr,"D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\bias\\bias61.csv",18,208398816);
    //smvar_slice_code(ddr,2097152,26673152,3,224,352);


    ifstream isaFileIn;
    isaFileIn.open("./isaFile.coe", ios::in);
	//mat_dma,vec_dma,res_dma
	unsigned short isa_code;
	unsigned short h_ddr_addr;
	unsigned short l_ddr_addr;
	unsigned short ddr_step;
	unsigned short ram_addr;
	unsigned short ram_step;
	unsigned short bloc_size;
	unsigned short bloc_numb;
	int ddr_addr;
	//vec_load
	unsigned short src_addr;
	unsigned short src_stride;
	unsigned short src_step;
	unsigned short dest_addr;
	unsigned short dest_step;
	unsigned short src_bloc;
	unsigned short dest_bloc;
	unsigned short src_bloc_size;
	unsigned short src_bloc_numb;
	unsigned short dest_bloc_size;
	unsigned short dest_bloc_numb;
	//mul
	unsigned short h_exe_src0_addr;
	unsigned short l_exe_src0_addr;
	unsigned short exe_src0_size;
	unsigned short exe_src1_addr;
	unsigned short exe_src1_size;
	unsigned short exe_dest_addr;
	unsigned short exe_dest;
	int exe_src0_addr;
	unsigned short exe_dest_size;
	unsigned short exe_dest_acti;
	unsigned short exe_dest_link;

	//res_dma
	unsigned short acti;
	unsigned short bias;
	char temp;
	while (1){
		//mat_dma
		get_short_from_hex_file(isaFileIn,isa_code);
		if(isa_code==0){
			cout<<"index: "<<isa_idx<<" mat_dma_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_ddr_addr);
			get_short_from_hex_file(isaFileIn,l_ddr_addr);
			get_short_from_hex_file(isaFileIn,ddr_step);
			get_short_from_hex_file(isaFileIn,ram_addr);
			get_short_from_hex_file(isaFileIn,ram_step);
			get_short_from_hex_file(isaFileIn,bloc_size);
			get_short_from_hex_file(isaFileIn,bloc_numb);
			ddr_addr=h_ddr_addr*65536+l_ddr_addr;
			smvar_mat_dma_isa_sim(
				ddr,
				MatSram0,
				MatSram1,
				ddr_addr,
				ddr_step,
				ram_addr,
				ram_step,
				bloc_size,
				bloc_numb
			);
			isa_idx+=1;
		}else if(isa_code==1){
			cout<<"index: "<<isa_idx<<" vec_dma_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_ddr_addr);
			get_short_from_hex_file(isaFileIn,l_ddr_addr);
			get_short_from_hex_file(isaFileIn,ddr_step);
			get_short_from_hex_file(isaFileIn,ram_addr);
			get_short_from_hex_file(isaFileIn,ram_step);
			get_short_from_hex_file(isaFileIn,bloc_size);
			get_short_from_hex_file(isaFileIn,bloc_numb);
			ddr_addr=h_ddr_addr*65536+l_ddr_addr;
			smvar_vec_dma_isa_sim(
				ddr,
				VecSram0,
				VecSram1,
				ddr_addr,
				ddr_step,
				ram_addr,
				ram_step,
				bloc_size,
				bloc_numb
			);
			isa_idx+=1;
		}else if(isa_code==2){
			cout<<"index: "<<isa_idx<<" bias_dma_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_ddr_addr);
			get_short_from_hex_file(isaFileIn,l_ddr_addr);
			get_short_from_hex_file(isaFileIn,ddr_step);
			get_short_from_hex_file(isaFileIn,ram_addr);
			get_short_from_hex_file(isaFileIn,ram_step);
			get_short_from_hex_file(isaFileIn,bloc_size);
			get_short_from_hex_file(isaFileIn,bloc_numb);
			ddr_addr=h_ddr_addr*65536+l_ddr_addr;
			smvar_bias_dma_isa_sim(
				ddr,
				biasSram,
				ddr_addr,
				ddr_step,
				ram_addr,
				ram_step,
				bloc_size,
				bloc_numb
			);
			isa_idx+=1;
		}else if(isa_code==14){
			cout<<"index: "<<isa_idx<<" vec_load_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,src_addr);
			get_short_from_hex_file(isaFileIn,src_stride);
			get_short_from_hex_file(isaFileIn,src_step);
			get_short_from_hex_file(isaFileIn,dest_addr);
			get_short_from_hex_file(isaFileIn,dest_step);
			get_short_from_hex_file(isaFileIn,src_bloc);
			get_short_from_hex_file(isaFileIn,dest_bloc);
			src_bloc_size=src_bloc/16;
			src_bloc_numb=src_bloc-src_bloc_size*16;
			dest_bloc_size=dest_bloc/16;
			dest_bloc_numb=dest_bloc-dest_bloc_size*16;
			smvar_vec_load_isa_sim(
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
				dest_bloc_numb
			);
			isa_idx+=1;
		}else if(isa_code==3){
			cout<<"index: "<<isa_idx<<" mul_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_exe_src0_addr);
			get_short_from_hex_file(isaFileIn,l_exe_src0_addr);
			get_short_from_hex_file(isaFileIn,exe_src0_size);
			get_short_from_hex_file(isaFileIn,exe_src1_addr);
			get_short_from_hex_file(isaFileIn,exe_src1_size);
			get_short_from_hex_file(isaFileIn,exe_dest_addr);
			get_short_from_hex_file(isaFileIn,exe_dest);
			exe_src0_addr=h_exe_src0_addr*65536+l_exe_src0_addr;
			exe_dest_size=exe_dest/8;
			exe_dest_acti=(exe_dest-exe_dest_size*8)/2;
			exe_dest_link=exe_dest-exe_dest_size*8-exe_dest_acti*2;
			smvar_mul_mult_isa_sim(
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
				exe_dest_link
			);
			isa_idx+=1;
		}else if(isa_code==5){
			cout<<"index: "<<isa_idx<<" vec_add_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_exe_src0_addr);
			get_short_from_hex_file(isaFileIn,l_exe_src0_addr);
			get_short_from_hex_file(isaFileIn,exe_src0_size);
			get_short_from_hex_file(isaFileIn,exe_src1_addr);
			get_short_from_hex_file(isaFileIn,exe_src1_size);
			get_short_from_hex_file(isaFileIn,exe_dest_addr);
			get_short_from_hex_file(isaFileIn,exe_dest);
			exe_src0_addr=h_exe_src0_addr*65536+l_exe_src0_addr;
			exe_dest_size=exe_dest/8;
			exe_dest_acti=(exe_dest-exe_dest_size*8)/2;
			exe_dest_link=exe_dest-exe_dest_size*8-exe_dest_acti*2;
			smvar_vec_add_isa_sim(
			    ResVSram,
			    ResMSram,
				exe_src0_addr,
				exe_src0_size,
				exe_src1_addr,
				exe_src1_size,
				exe_dest_addr,
				exe_dest_size,
				exe_dest_acti,
				exe_dest_link
			);
			isa_idx+=1;
		}else if(isa_code==31){
			cout<<"index: "<<isa_idx<<" res_dma_isa:"<<endl;
			get_short_from_hex_file(isaFileIn,h_ddr_addr);
			get_short_from_hex_file(isaFileIn,l_ddr_addr);
			get_short_from_hex_file(isaFileIn,ddr_step);
			get_short_from_hex_file(isaFileIn,ram_addr);
			get_short_from_hex_file(isaFileIn,ram_step);
			get_short_from_hex_file(isaFileIn,bloc_size);
			get_short_from_hex_file(isaFileIn,bloc_numb);
			ddr_addr=h_ddr_addr*65536+l_ddr_addr;
			cout<<" ddr_addr: "<<ddr_addr<<endl;
			acti=bloc_size/8192;
			bias=(bloc_size-acti*8192)/4096;
			bloc_size=bloc_size-acti*8192-bias*4096;

//			isa_ddr[isa_idx*8+6] = bloc_size+bias*4096+acti*8192;
			smvar_res_dma_isa_sim(
				ddr,
				ResVSram,
				biasSram,
				ddr_addr,
				ddr_step,
				ram_addr,
				ram_step,
				acti,
				bias,
				bloc_size,
				bloc_numb
			);
			isa_idx+=1;
		}else if(isa_code==30){
			cout<<"index: "<<isa_idx<<" end_isa:"<<endl;
			break;
		}else {
			cout<<"index: "<<isa_idx<<" other_isa:"<<isa_code<<endl;
			get_short_from_hex_file(isaFileIn,h_ddr_addr);
			get_short_from_hex_file(isaFileIn,l_ddr_addr);
			get_short_from_hex_file(isaFileIn,ddr_step);
			get_short_from_hex_file(isaFileIn,ram_addr);
			get_short_from_hex_file(isaFileIn,ram_step);
			get_short_from_hex_file(isaFileIn,bloc_size);
			get_short_from_hex_file(isaFileIn,bloc_numb);
			isa_idx+=1;
		}
		if(isa_idx%4==0){
			isaFileIn>>temp;
			cout<<"temp val:"<<temp<<endl;
		}
	}
	isaFileIn.close();

//    smvar_conv_code_nCi(ddr,26673152,2097152,201326592,208389120,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,32,112.0,176.0,3,1,1,1,4,isa_idx,isa_ddr);
//    smvar_conv_code_nCi(ddr,2097152,26673152,201335808,208389152,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,32,112,176,3,2,1,1,4,isa_idx,isa_ddr);
//    smvar_conv_code_nCi(ddr,26673152,2097152,201354240,208389216,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,64,56,88,1,1,0,1,4,isa_idx,isa_ddr);
//    smvar_conv_code_nCi(ddr,2097152,51249152,201356288,208389248,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,32,56,88,1,1,0,1,4,isa_idx,isa_ddr);
//    smvar_conv_code_nCi(ddr,51249152,75825152,201357312,208389280,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,32,56,88,3,1,1,1,4,isa_idx,isa_ddr);
//    smvar_isend_isa_gen(isa_idx,isa_ddr);
    smvar_add_code(ddr,2097152,75825152,51249152,32,56,88);
    smvar_conv_code_nCi(ddr,26673152,2097152,201366528,208389312,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,64,56,88,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,51249152,2097152,26673152,32,56,88);
    smvar_conv_code_nCi(ddr,26673152,2097152,201368576,208389344,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,56,88,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,26673152,201372672,208389408,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,64,56,88,3,2,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,2097152,201446400,208389536,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,128,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,51249152,201454592,208389600,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,201458688,208389664,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,2097152,75825152,51249152,64,28,44);
    smvar_conv_code_nCi(ddr,51249152,2097152,201495552,208389728,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,75825152,201499648,208389792,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,51249152,75825152,2097152,64,28,44);
    smvar_conv_code_nCi(ddr,2097152,51249152,201536512,208389856,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,201540608,208389920,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,2097152,75825152,51249152,64,28,44);
    smvar_conv_code_nCi(ddr,26673152,2097152,201577472,208389984,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,128,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,51249152,2097152,26673152,64,28,44);
    smvar_conv_code_nCi(ddr,26673152,2097152,201585664,208390048,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,26673152,201602048,208390176,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,128,28,44,3,2,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,51249152,201896960,208390432,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,201929728,208390560,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,100401152,201946112,208390688,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,51249152,100401152,75825152,128,14,22);
    smvar_conv_code_nCi(ddr,75825152,51249152,202093568,208390816,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,100401152,202109952,208390944,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,75825152,100401152,51249152,128,14,22);
    smvar_conv_code_nCi(ddr,51249152,75825152,202257408,208391072,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,100401152,202273792,208391200,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_add_code(ddr,51249152,100401152,75825152,128,14,22);
    smvar_conv_code_nCi(ddr,26673152,51249152,202421248,208391328,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,75825152,51249152,26673152,128,14,22);
    smvar_conv_code_nCi(ddr,26673152,51249152,202454016,208391456,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,26673152,202519552,208391712,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,512,256,14,22,3,2,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,75825152,203699200,208392224,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_maxpool_code(ddr,75825152,26673152,256,7,11,5,2);
    smvar_maxpool_code(ddr,75825152,100401152,256,7,11,9,4);
    smvar_maxpool_code(ddr,75825152,124977152,256,7,11,13,6);
    smvar_cat4_code(ddr,75825152,26673152,100401152,124977152,149553152,256,7,11);
    smvar_conv_code_nCi(ddr,149553152,26673152,203830272,208392480,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,512,1024,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,75825152,204354560,208392992,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,100401152,204485632,208393248,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,100401152,75825152,204551168,208393504,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,7,11,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,100401152,205140992,208393760,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,75825152,100401152,26673152,256,7,11);
    smvar_conv_code_nCi(ddr,26673152,75825152,205272064,208394016,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,512,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,26673152,205534208,208394528,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_upsample_code(ddr,26673152,75825152,256,7,11);
    smvar_cat_code(ddr,75825152,51249152,100401152,256,14.0,22.0);
    smvar_conv_code_nCi(ddr,100401152,51249152,205665280,208394784,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,512,14.0,22.0,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,205730816,208394912,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,51249152,205747200,208395040,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,100401152,75825152,205894656,208395168,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,512,14.0,22.0,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,51249152,75825152,100401152,128,14,22);
    smvar_conv_code_nCi(ddr,100401152,51249152,205960192,208395296,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,206025728,208395552,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_upsample_code(ddr,75825152,51249152,128,14,22);
    smvar_cat_code(ddr,51249152,2097152,100401152,128,28.0,44.0);
    smvar_conv_code_nCi(ddr,100401152,2097152,206058496,208395680,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,256,28.0,44.0,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,51249152,206074880,208395744,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,2097152,206078976,208395808,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,64,28,44,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,100401152,51249152,206115840,208395872,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,64,256,28.0,44.0,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,2097152,51249152,100401152,64,28,44);
    smvar_conv_code_nCi(ddr,100401152,2097152,206132224,208395936,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,28,44,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,51249152,206148608,208396064,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,28,44,3,2,1,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,51249152,75825152,100401152,128,14,22);
    smvar_conv_code_nCi(ddr,100401152,51249152,206296064,208396192,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,206328832,208396320,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,51249152,206345216,208396448,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,128,14,22,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,100401152,75825152,206492672,208396576,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,128,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,51249152,75825152,100401152,128,14,22);
    smvar_conv_code_nCi(ddr,100401152,51249152,206525440,208396704,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,14,22,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,75825152,206590976,208396960,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,14,22,3,2,1,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,75825152,26673152,100401152,256,7,11);
    smvar_conv_code_nCi(ddr,100401152,26673152,207180800,208397216,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,75825152,207311872,208397472,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,75825152,26673152,207377408,208397728,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,256,7,11,3,1,1,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,100401152,75825152,207967232,208397984,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,256,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_cat_code(ddr,26673152,75825152,100401152,256,7,11);
    smvar_conv_code_nCi(ddr,100401152,26673152,208098304,208398240,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,512,512,7,11,1,1,0,1,4,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,2097152,75825152,208360448,208398752,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,128,28,44,1,1,0,1,0,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,51249152,2097152,208364544,208398784,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,256,14,22,1,1,0,1,0,isa_idx,isa_ddr);
    smvar_conv_code_nCi(ddr,26673152,51249152,208372736,208398816,MatSram0,MatSram1,VecSram0,VecSram1,VecRegs0,VecRegs1,SumSram,ResVSram,ResMSram,vecReg,matReg,res_bus,sum,biasSram,32,512,7,11,1,1,0,1,0,isa_idx,isa_ddr);





//    ofstream isaFileOut;
//    isaFileOut.open("./isaFile.coe", ios::out);
//	for(int i = 0; i < isa_idx/4+1; i++){
//    	for(int j = 0; j < 32; j++){
//    		short2hex(isaFileOut, isa_ddr[i*32+j]);
//    	}
//    	isaFileOut<<","<<endl;
//    }
//	isaFileOut.close();


	////********* add  correct results from torch
    int flag=1;
	string     	lineStr, str;
	in_fixed   	in_num;
	double 		tmp;
	int i = 0, k = 0,Co_step;
//
//

//	Co=32,Ho=112,Wo=176,res_ddr_addr=2097152;
//	in_fixed   	*torch_output_feature = new in_fixed[Ho*Wo*Co];
//	ifstream inFile_torch_output0("D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\input\\input1.csv", ios::in);
//	i = 0;
//	while (getline(inFile_torch_output0, lineStr)) {
//		stringstream ss(lineStr);
//		k = 0;
//		while (getline(ss, str, ',')) {
//			tmp = atof(str.c_str());
//			in_num = (in_fixed)tmp;
//			torch_output_feature[i*Co*Wo+k] = in_num;
//			k++;
//		}
//		i++;
//	}
//	if(Co%32==0)
//		Co_step=Co;
//	else
//		Co_step=(Co/32+1)*32;
//	for(int wh=0; wh<Ho*Wo; wh++){
//		for(int co=0; co<Co; co++){
//			if(1){
//				if((torch_output_feature[wh*Co+co]-ddr[res_ddr_addr+wh*Co_step+co])>0.0001 ||(ddr[res_ddr_addr+wh*Co_step+co]-torch_output_feature[wh*Co+co])>0.0001 ){
//					flag=0;
//					if(wh==0 && co==0)
//					cout<<"wrong!! wh:"<<wh<<" co:"<<co<<" ddr"<<ddr[res_ddr_addr+wh*Co+co]<<" torch_output_feature:"<<torch_output_feature[wh*Co+co]<<endl;
//				}
//			}
//		}
//	}
//	inFile_torch_output0.close();
//	delete[]  torch_output_feature;


//
//////    weight smvar_bias_sim(ddr,51249152,208380484,18,7,11);
    Co=18,Ho=7,Wo=11,res_ddr_addr=51249152;
	if(Co%32==0)
		Co_step=Co;
	else
		Co_step=(Co/32+1)*32;
	in_fixed   	*torch_output_feature = new in_fixed[Ho*Wo*Co];
	ifstream inFile_torch_output("D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\output\\output61.csv", ios::in);
	i = 0;
	while (getline(inFile_torch_output, lineStr)) {
		stringstream ss(lineStr);
		k = 0;
	    while (getline(ss, str, ',')) {
	    	tmp = atof(str.c_str());
	        in_num = (in_fixed)tmp;
	        torch_output_feature[i*Co*Wo+k] = in_num;
	        k++;
	    }
	    i++;
	}
    for(int wh=0; wh<Ho*Wo; wh++){
    	for(int co=0; co<Co; co++){
    		if(1){
    			if((torch_output_feature[wh*Co+co]-ddr[res_ddr_addr+wh*Co_step+co])>0.0001 ||(ddr[res_ddr_addr+wh*Co_step+co]-torch_output_feature[wh*Co+co])>0.0001 ){
    				flag=0;
    				if(wh==0 &&co==0)
    				cout<<"wrong!! wh:"<<wh<<" co:"<<co<<" ddr"<<ddr[res_ddr_addr+wh*Co+co]<<" torch_output_feature:"<<torch_output_feature[wh*Co+co]<<endl;
    			}
    		}
    	}
    }
    inFile_torch_output.close();
    delete[]  torch_output_feature;

    //weight 60 smvar_bias_sim(ddr,2097152,208380466,18,14,22);
    Co=18,Ho=14,Wo=22,res_ddr_addr=2097152;
	if(Co%32==0)
		Co_step=Co;
	else
		Co_step=(Co/32+1)*32;
	in_fixed   	*torch_output_feature1 = new in_fixed[Ho*Wo*Co];
	ifstream inFile_torch_output1("D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\output\\output60.csv", ios::in);
	i = 0;
	while (getline(inFile_torch_output1, lineStr)) {
		stringstream ss(lineStr);
		k = 0;
	    while (getline(ss, str, ',')) {
	    	tmp = atof(str.c_str());
	        in_num = (in_fixed)tmp;
	        torch_output_feature1[i*Co*Wo+k] = in_num;
	        k++;
	    }
	    i++;
	}

    for(int wh=0; wh<Ho*Wo; wh++){
    	for(int co=0; co<Co; co++){
    		if(1){
    			if((torch_output_feature1[wh*Co+co]-ddr[res_ddr_addr+wh*Co_step+co])>0.0001 ||(ddr[res_ddr_addr+wh*Co_step+co]-torch_output_feature1[wh*Co+co])>0.0001 ){
    				flag=0;
    				if(wh==0 &&co==0)
    				cout<<"wrong!! wh:"<<wh<<" co:"<<co<<" ddr"<<ddr[res_ddr_addr+wh*Co+co]<<" torch_output_feature:"<<torch_output_feature1[wh*Co+co]<<endl;
    			}
    		}
    	}
    }
    inFile_torch_output1.close();
    delete[]  torch_output_feature1;

    //weight 59 smvar_bias_sim(ddr,75825152,208380448,18,28,44);;
    Co=18,Ho=28,Wo=44,res_ddr_addr=75825152;
	if(Co%32==0)
		Co_step=Co;
	else
		Co_step=(Co/32+1)*32;
	in_fixed   	*torch_output_feature2 = new in_fixed[Ho*Wo*Co];
	ifstream inFile_torch_output2("D:\\file\\pg\\backup\\yolo\\yolov5\\csv\\conv\\output\\output59.csv", ios::in);
	i = 0;
	while (getline(inFile_torch_output2, lineStr)) {
		stringstream ss(lineStr);
		k = 0;
	    while (getline(ss, str, ',')) {
	    	tmp = atof(str.c_str());
	        in_num = (in_fixed)tmp;
	        torch_output_feature2[i*Co*Wo+k] = in_num;
	        k++;
	    }
	    i++;
	}

    for(int wh=0; wh<Ho*Wo; wh++){
    	for(int co=0; co<Co; co++){
    		if(1){
    			if((torch_output_feature2[wh*Co+co]-ddr[res_ddr_addr+wh*Co_step+co])>0.0001 ||(ddr[res_ddr_addr+wh*Co_step+co]-torch_output_feature2[wh*Co+co])>0.0001 ){
    				flag=0;
    				if(wh==0 &&co==0)
    				cout<<"wrong!! wh:"<<wh<<" co:"<<co<<" ddr"<<ddr[res_ddr_addr+wh*Co+co]<<" torch_output_feature:"<<torch_output_feature2[wh*Co+co]<<endl;
    			}
    		}
    	}
    }
    inFile_torch_output2.close();
    delete[]  torch_output_feature2;

	if(flag==0)
		cout<<"wrong result!!!!!"<<endl;
	else
		cout<<"correct result!!!!!"<<endl;

}
