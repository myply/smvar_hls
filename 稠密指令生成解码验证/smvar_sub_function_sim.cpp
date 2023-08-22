#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
#include "smvar_isa_sim.h"
#include "sub_function.h"
void smvar_slice_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				if(h%2==0 && w%2==0)
					ddr[res_ddr_addr+h/2*Wi/2*Ci*4+w/2*Ci*4+c]=ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c];
				else if(h%2==1 && w%2==0)
					ddr[res_ddr_addr+h/2*Wi/2*Ci*4+w/2*Ci*4+Ci+c]=ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c];
				else if(h%2==0 && w%2==1)
					ddr[res_ddr_addr+h/2*Wi/2*Ci*4+w/2*Ci*4+Ci*2+c]=ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c];
				else if(h%2==1 && w%2==1)
					ddr[res_ddr_addr+h/2*Wi/2*Ci*4+w/2*Ci*4+Ci*3+c]=ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c];
			}
		}
	}
}
void smvar_act_code(in_fixed * ddr,
		int ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				in_fixed temp=ddr[ddr_addr+h*Wi*Ci+w*Ci+c];
//				cout<<" before act: "<<temp<<" after act: "<<temp/(1+exp(-temp))<<endl;
				ddr[ddr_addr+h*Wi*Ci+w*Ci+c]=temp/(1+exp(-temp));

			}
		}
	}
}
void  smvar_add_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				int offset=h*Wi*Ci+w*Ci+c;
				ddr[res_ddr_addr+offset]=ddr[src1_ddr_addr+offset]+ddr[src2_ddr_addr+offset];
			}
		}
	}
}
void  smvar_cat_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				ddr[res_ddr_addr+h*Wi*Ci*2+w*Ci*2+c]=ddr[src1_ddr_addr+h*Wi*Ci+w*Ci+c];
				ddr[res_ddr_addr+h*Wi*Ci*2+w*Ci*2+c+Ci]=ddr[src2_ddr_addr+h*Wi*Ci+w*Ci+c];
			}
		}
	}
}
void  smvar_cat4_code(in_fixed * ddr,
		int src1_ddr_addr,
		int src2_ddr_addr,
		int src3_ddr_addr,
		int src4_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
//				if(c==0){
//					cout<<ddr[src1_ddr_addr+h*Wi*Ci+w*Ci+c]<<" "<<ddr[src2_ddr_addr+h*Wi*Ci+w*Ci+c]<<" "<<ddr[src3_ddr_addr+h*Wi*Ci+w*Ci+c]<<" "<<ddr[src4_ddr_addr+h*Wi*Ci+w*Ci+c]<<" "<<endl;;
//				}
				ddr[res_ddr_addr+h*Wi*Ci*4+w*Ci*4+c]=ddr[src1_ddr_addr+h*Wi*Ci+w*Ci+c];
				ddr[res_ddr_addr+h*Wi*Ci*4+w*Ci*4+c+Ci]=ddr[src2_ddr_addr+h*Wi*Ci+w*Ci+c];
				ddr[res_ddr_addr+h*Wi*Ci*4+w*Ci*4+c+Ci*2]=ddr[src3_ddr_addr+h*Wi*Ci+w*Ci+c];
				ddr[res_ddr_addr+h*Wi*Ci*4+w*Ci*4+c+Ci*3]=ddr[src4_ddr_addr+h*Wi*Ci+w*Ci+c];
			}
		}
	}
}
void smvar_maxpool_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi,
	int kernel_size,
	int padding){
	in_fixed *fea_pad = new in_fixed[(Hi+2*padding)*(Wi+2*padding)*Ci];
	for(int h=0; h<Hi+2*padding; h++)
		for(int w=0; w<Wi+2*padding; w++)
			for(int c=0; c<Ci; c++){
				if(h<padding || h>=Hi+padding){
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = -99999;
				}
				else if(w<padding || w>=Wi+padding){
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = -99999;
				}
				else{
					fea_pad[h*(Wi+2*padding)*Ci+w*Ci+c] = ddr[vec_ddr_addr+(h-padding)*Wi*Ci+(w-padding)*Ci+c];
				}
			}


	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				in_fixed max=fea_pad[(h+0)*(Wi+2*padding)*Ci+(w+0)*Ci+c];
				for(int kh = 0; kh < kernel_size; kh=kh+1){
					for(int kw=0; kw<kernel_size; kw=kw+1){
						in_fixed temp=fea_pad[(h+kh)*(Wi+2*padding)*Ci+(w+kw)*Ci+c];
						if(max<temp){
							max=temp;
						}
					}
				}
				ddr[res_ddr_addr+h*Wi*Ci+w*Ci+c]=max;

			}
		}
	}
	delete[] fea_pad;
}
void smvar_upsample_code(in_fixed * ddr,
		int vec_ddr_addr,
		int res_ddr_addr,
	int Ci,
	int Hi,
	int Wi){
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				in_fixed temp=ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c];
				ddr[res_ddr_addr+h*2*Wi*2*Ci+w*2*Ci+c]=temp;
				ddr[res_ddr_addr+h*2*Wi*2*Ci+(w*2+1)*Ci+c]=temp;
				ddr[res_ddr_addr+(h*2+1)*Wi*2*Ci+w*2*Ci+c]=temp;
				ddr[res_ddr_addr+(h*2+1)*Wi*2*Ci+(w*2+1)*Ci+c]=temp;
			}
		}
	}
}
void smvar_bias_sim(in_fixed * ddr,
		int vec_ddr_addr,
		int bias_ddr_addr,
		int  Ci,
		int Hi,
		int Wi){

//	cout<<"bias "<<endl;
//	for (int c=0;c<Ci;c++){
//			cout<<ddr[bias_ddr_addr+c]<<endl;
//	}
	for (int h=0;h<Hi;h++){
		for (int w=0;w<Wi;w++){
			for (int c=0;c<Ci;c++){
				ddr[vec_ddr_addr+h*Wi*Ci+w*Ci+c]+=ddr[bias_ddr_addr+c];
			}
		}
	}
}
void store_fea_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Ci,
		int Hi,
		int Wi,
		int vec_ddr_addr){
	string     	lineStr, str;
	in_fixed   	in_num;
	double 		tmp;
	int i = 0, k = 0,Ci_pad=Ci;
	if(Ci/32*32<Ci){
		Ci_pad=(Ci/32+1)*32;
	}
	in_fixed   	*feature = new in_fixed[Hi*Wi*Ci];
	in_fixed   	*feature_pad = new in_fixed[Hi*Wi*Ci_pad];
	ifstream inFile_input(csv_name, ios::in);
	while (getline(inFile_input, lineStr)) {
		stringstream ss(lineStr);
	    k = 0;
	    while (getline(ss, str, ',')) {
	    	tmp = atof(str.c_str());
	        in_num = (in_fixed)tmp;

	        feature[i*Ci*Wi+k] = in_num;
	        k++;
	    }
	    i++;
	}

    for(int i=0; i<Hi*Wi; i++){
    	for(int j=0;j<Ci_pad;j++){
    		if(j<Ci){
    			feature_pad[i*Ci_pad+j] = feature[i*Ci+j];
    		}else{
    			feature_pad[i*Ci_pad+j] = 0;
    		}
    	}
    }

    //store feature and weight to ddr
    for(int i=0; i<Hi*Wi*Ci_pad; i++){
    	ddr[vec_ddr_addr+i] = feature_pad[i];
    }

    inFile_input.close();
    delete[]  feature;
    delete[] feature_pad;
}
void store_weight_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Co,
		int Ci,
		int K,
		int mat_ddr_addr){
	string     	lineStr, str;
	in_fixed   	in_num;
	double 		tmp;
	int i = 0, k = 0,Co_pad,Ci_pad;
	if(Co%32==0){
    	Co_pad = Co;
	}else{
    	Co_pad = (Co/32+1)*32;
	}
	if(Ci%32==0){
    	Ci_pad = Ci;
	}else{
    	Ci_pad = (Ci/32+1)*32;
	}
	int BCI=Ci_pad/32;
    in_fixed *weight = new in_fixed[Co*Ci*K*K];
    in_fixed *weight2 = new in_fixed[Co_pad*Ci_pad*K*K];
	ifstream inFile_Wi1_real(csv_name, ios::in);
	i = 0;
	k = 0;
	while (getline(inFile_Wi1_real, lineStr)) {
		stringstream ss(lineStr);
	    k = 0;
	    while (getline(ss, str, ',')) {
	    	tmp = atof(str.c_str());
	        in_num = (in_fixed)tmp;
	        weight[i*Ci*K*K+k] = in_num;
	        k++;
	    }
	    i++;
	}
	for(int bci=0; bci<BCI; bci++){
    	for(int k=0; k<K*K; k++){
    		for(int co=0; co<Co_pad; co++){
        		for(int ci=0; ci<32; ci++){
        			if(co>=Co){
        				weight2[bci*Co_pad*K*K*32+k*Co_pad*32+co*32+ci]=0;
        			}else{
        				if(bci*32+ci<Ci){
        					weight2[bci*Co_pad*K*K*32+k*Co_pad*32+co*32+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
        				}
        				else{
        					weight2[bci*Co_pad*K*K*32+k*Co_pad*32+co*32+ci]=0;
        				}
        			}
        		}

//    			if((bci+1)*32>Ci){
//    				size_of_a_batch=Ci%32;
//        			for(int ci=0; ci<size_of_a_batch; ci++){
//        				weight2[bci*Co*K*K*32+k*Co*size_of_a_batch+co*size_of_a_batch+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
//        			}
//    			}else{
//        			for(int ci=0; ci<size_of_a_batch; ci++){
//        				weight2[bci*Co*K*K*32+k*Co*32+co*32+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
//        			}
//    			}

    		}
    	}
    }
    for(int i=0; i<K*K*Co_pad*Ci_pad; i++){
    	ddr[mat_ddr_addr+i] = weight2[i];
    }

//    in_fixed *w_nonzero = new in_fixed[Co_pad*Ci*K*K];
//    short *non_dest = new short[Co_pad*Ci*K*K];
//    short *non_vn = new short[Co_pad*Ci*K*K];
//    int *non_numb = new int[Co_pad*K*K/32];
//
    inFile_Wi1_real.close();
    delete[]  weight;
    delete[]  weight2;
}
//void store_weight_from_csv_2_ddr(in_fixed * ddr,
//		string csv_name,
//		int  Co,
//		int Ci,
//		int K,
//		int mat_ddr_addr){
//	string     	lineStr, str;
//	in_fixed   	in_num;
//	double 		tmp;
//	int i = 0, k = 0;
//
//    in_fixed *weight = new in_fixed[Co*Ci*K*K];
//    in_fixed *weight2 = new in_fixed[Co*Ci*K*K];
//	ifstream inFile_Wi1_real(csv_name, ios::in);
//	i = 0;
//	k = 0;
//	while (getline(inFile_Wi1_real, lineStr)) {
//		stringstream ss(lineStr);
//	    k = 0;
//	    while (getline(ss, str, ',')) {
//	    	tmp = atof(str.c_str());
//	        in_num = (in_fixed)tmp;
//	        weight[i*Ci*K*K+k] = in_num;
//	        k++;
//	    }
//	    i++;
//	}
//	int BCI=Ci/32;
//	if(Ci/32*32<Ci)
//		BCI=Ci/32+1;
//	for(int bci=0; bci<BCI; bci++){
//    	for(int k=0; k<K*K; k++){
//    		for(int co=0; co<Co; co++){
//
//    			int size_of_a_batch=32;
//    			if((bci+1)*32>Ci){
//    				size_of_a_batch=Ci%32;
//    			}
//        		for(int ci=0; ci<size_of_a_batch; ci++){
//        			weight2[bci*Co*K*K*32+k*Co*size_of_a_batch+co*size_of_a_batch+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
//        		}
//
////    			if((bci+1)*32>Ci){
////    				size_of_a_batch=Ci%32;
////        			for(int ci=0; ci<size_of_a_batch; ci++){
////        				weight2[bci*Co*K*K*32+k*Co*size_of_a_batch+co*size_of_a_batch+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
////        			}
////    			}else{
////        			for(int ci=0; ci<size_of_a_batch; ci++){
////        				weight2[bci*Co*K*K*32+k*Co*32+co*32+ci] = weight[co*K*K*Ci+k*Ci+bci*32+ci];
////        			}
////    			}
//
//    		}
//    	}
//    }
//    for(int i=0; i<K*K*Co*Ci; i++){
//    	ddr[mat_ddr_addr+i] = weight2[i];
//    }
//    inFile_Wi1_real.close();
//    delete[]  weight;
//    delete[]  weight2;
//}
void store_bias_from_csv_2_ddr(in_fixed * ddr,
		string csv_name,
		int  Co,
		int bias_ddr_addr){
	string     	lineStr, str;
	in_fixed   	in_num;
	double 		tmp;
	int k = 0,Co_pad;
	if(Co%32==0){
    	Co_pad = Co;
	}else{
    	Co_pad = (Co/32+1)*32;
	}
    in_fixed *bias = new in_fixed[Co_pad];
    for(int co=0; co<Co_pad; co++){
    	 bias[co]=0;
    }
	ifstream inFile_Wi1_real(csv_name, ios::in);
	if(inFile_Wi1_real.is_open()){
		k = 0;
		while (getline(inFile_Wi1_real, lineStr)) {

		    	tmp = atof(lineStr.c_str());
		        in_num = (in_fixed)tmp;
		        bias[k] = in_num;
		        k++;
		}
	}else{
		cout<<csv_name<<" not exsits"<<endl;
	}

    for(int i=0; i<Co_pad; i++){
    	ddr[bias_ddr_addr+i] = bias[i];
    }
    inFile_Wi1_real.close();
    delete[]  bias;
}
//void store_bias_from_csv_2_ddr(in_fixed * ddr,
//		string csv_name,
//		int  Co,
//		int bias_ddr_addr){
//	string     	lineStr, str;
//	in_fixed   	in_num;
//	double 		tmp;
//	int k = 0;
//
//    in_fixed *bias = new in_fixed[Co];
//    for(int co=0; co<Co; co++){
//    	 bias[co]=0;
//    }
//	ifstream inFile_Wi1_real(csv_name, ios::in);
//	if(inFile_Wi1_real.is_open()){
//		k = 0;
//		while (getline(inFile_Wi1_real, lineStr)) {
//
//		    	tmp = atof(lineStr.c_str());
//		        in_num = (in_fixed)tmp;
//		        bias[k] = in_num;
//		        k++;
//		}
//	}else{
//		cout<<csv_name<<" not exsits"<<endl;
//	}
//
//    for(int i=0; i<Co; i++){
//    	ddr[bias_ddr_addr+i] = bias[i];
//    }
//    inFile_Wi1_real.close();
//    delete[]  bias;
//}
void get_short_from_hex_file(ifstream &isaFile, unsigned short &val)
{
	char *pr=new char[5];
    char *stop=NULL;
    isaFile>>pr[0];
    isaFile>>pr[1];
    isaFile>>pr[2];
    isaFile>>pr[3];
    //const char* end with \0
    pr[4]=0;
    val=strtol(pr,&stop,16);
    delete[]  pr;
}
void mat_block(
	in_fixed *mat,
	int H_i,
	int W_i,
	in_fixed *spmat_block,
    short *spdest_block,
    short *spvn_block,
	int *non_numb
){
    int w_bloc = W_i/32;

    int h_bloc = H_i/32;

    int vn;
    int index;
    int nozero_flag = 0;
    in_fixed **spbmat = new in_fixed*[w_bloc];
    short **bdest = new short *[w_bloc];
    short **bvn = new short *[w_bloc];
    int *spbmat_size = new int [w_bloc];
    int sss;
    for(int i=0;i<w_bloc;i++){
		spbmat[i] = new in_fixed[H_i*32];
		bdest[i] = new short[H_i*32];
		bvn[i] = new short[H_i*32];
	}

    for(int wb = 0; wb < w_bloc; wb++){
    	vn = 0;
    	spbmat_size[wb] = 0;
    	index = 0;
    	for(int hb = 0; hb < h_bloc; hb++){
    		for(int hi = 0; hi < 32; hi++){
				for(int wi = 0; wi < 32; wi++){
					if(mat[hb*32*W_i+hi*W_i+wb*32+wi] != 0){
						spbmat[wb][index] = mat[hb*32*W_i+hi*W_i+wb*32+wi];
						bdest[wb][index] = wi;
						bvn[wb][index] = vn;
						index++;
						spbmat_size[wb] = spbmat_size[wb] + 1;
						nozero_flag = nozero_flag + 1;
						//cout<<"vn = "<<vn<<endl;
					}
					//cout<<"vn = "<<vn<<endl;
				}
				if(nozero_flag < 32 && hi == 31){
					sss = 32 - nozero_flag;
				    for(int wi = 0; wi < sss; wi++){
				    	spbmat[wb][index] = 0;
				    	bdest[wb][index] = wi;
				    	bvn[wb][index] = vn;
				    	index++;

				    	spbmat_size[wb] = spbmat_size[wb] + 1;
				    	//cout<<"zn ="<<vn<<endl;
				    }
				    //
				}
				//cout<<"vn = "<<vn<<endl;
				vn++;
    		}
    		nozero_flag = 0;
    		//cout<<endl;
    	}
    	//cout<<"spbmat_size = "<<spbmat_size[wb]<<endl;
    }
    int non_zero_128 = 0;
    int Aidx = 0;
    int s4 = 0;
	for(int wb = 0; wb < w_bloc; wb++){
		index = 0;
        non_zero_128 = 0;
		for(int i = 0; i < int(spbmat_size[wb]/32); i++){
			index = index + 32;
			for(int j = 31; j >= 0; j--){
				spmat_block[Aidx] = spbmat[wb][index-1];
                spdest_block[Aidx] = bdest[wb][index-1];
                spvn_block[Aidx] = bvn[wb][index-1];

                Aidx++;
				index--;
				s4 = s4 + 1;
				non_zero_128 = non_zero_128 + 1;
				if(s4 == 32){
					s4 = 0;
					//cout<<endl;
				}
			}
			//cout<<endl;
			index = index + 32;
		}
		if(int(spbmat_size[wb]/32)*32<spbmat_size[wb]){
			index = index + spbmat_size[wb] - int(spbmat_size[wb]/32)*32;
			for(int j = 31; j >= 0; j--){
				if(j< spbmat_size[wb] - int(spbmat_size[wb]/32)*32){
					spmat_block[Aidx] = spbmat[wb][index-1];
                    spdest_block[Aidx] = bdest[wb][index-1];
                    spvn_block[Aidx] = bvn[wb][index-1];

                    Aidx++;
					index--;
					s4 = s4 + 1;
					non_zero_128++ ;
					if(s4 == 32){
						s4 = 0;
						//cout<<endl;

					}
				}
				else{
                    spmat_block[Aidx] = 0;
                    spdest_block[Aidx] = 0;
                    spvn_block[Aidx] = bvn[wb][index-1];

                    Aidx++;
					s4 = s4 + 1;
					non_zero_128++ ;
					if(s4 == 32){
						s4 = 0;
						//cout<<endl;
					}
				}
			}
			index = index + spbmat_size[0] - int(spbmat_size[0]/32)*32;
		}

        non_numb[wb] = non_zero_128;
	}
	//cout<<"Aidx = "<<Aidx<<endl;

}
