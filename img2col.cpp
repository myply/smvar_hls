#include "flexdpe-para.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
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
){

	in_fixed *fea_pad = new in_fixed[(h_in+2)*(w_in+2)*ch_in];
	for(int h=0; h<h_in+2; h++)
		for(int w=0; w<w_in+2; w++)
			for(int c=0; c<ch_in; c++){
				if(h==0 || h==h_in+1){
					fea_pad[h*(w_in+2)*ch_in+w*ch_in+c] = 0;
				}
				else if(w==0 || w==w_in+1){
					fea_pad[h*(w_in+2)*ch_in+w*ch_in+c] = 0;
				}
				else{
					fea_pad[h*(w_in+2)*ch_in+w*ch_in+c] = feature[(h-1)*w_in*ch_in+(w-1)*ch_in+c];
				}
			}
//	//cout first 26*26*32
//	cout<<"correct fisrt 26*26*32"<<endl;
//	for(int h=0; h<26; h++){
//		for(int w=0; w<26; w++){
//			for(int c=0; c<1; c++){
//					cout<<fea_pad[h*(w_in+2)*ch_in+w*ch_in+c]<<" ";
//				}
//			}
//		cout<<endl;
//		}


	for(int i = 0; i < h_in; i=i+stride_in)
		for(int j = 0; j < w_in; j=j+stride_in)
			for(int kh = 0; kh < k_in; kh=kh+1)
				for(int kw=0; kw<k_in; kw=kw+1)
					for(int c = 0; c < ch_in; c++){
						///update the new rule
						matrix[(i/stride_in)*(w_out)*k_in*k_in*ch_in+(j/stride_in)*k_in*k_in*ch_in+kh*k_in*ch_in+kw*ch_in+c] =
								fea_pad[(i+kh)*(w_in+2)*ch_in+(j+kw)*ch_in+c];
//						matrix[(i/stride_in)*(w_in/stride_in)*k_in*k_in*ch_in+(j/stride_in)*k_in*k_in*ch_in+kh*k_in*ch_in+kw*ch_in+c] =
//								fea_pad[(i+kh)*(w_in+2)*ch_in+(j+kw)*ch_in+c];
					}
//	cout<<"*****************im2col:"<<endl;
//	for(int h = 0; h < h_out*w_out; h++){
//		for(int w=0; w<k_in*k_in*ch_in; w++)
//		{
//			cout<<matrix[h*k_in*k_in*ch_in+w]<<" ";
//		}
//		cout<<endl;
//	}
};
