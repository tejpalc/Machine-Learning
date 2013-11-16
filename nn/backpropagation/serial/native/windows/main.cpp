#include<stdio.h>
#include<iostream.h>
#include"bpnet.h"

#define ITER 800000

void main(){
	int s[3]={3,50,1};
	CBpnet bp(3,s);

	double	dat[8][3]= {0,0,0,
						0,0,1,
						0,1,0,
						0,1,1,
						1,0,0,
						1,0,1,
						1,1,0,
						1,1,1,};

	double d[8][1]={0,1,1,0,1,0,0,1};

	int j=1;
	for(int i=0;i< ITER ;i++){
		j++;
		bp.bpgt(dat[0],d[0]);
		bp.bpgt(dat[1],d[1]);
		bp.bpgt(dat[2],d[2]);
		bp.bpgt(dat[3],d[3]);
		bp.bpgt(dat[4],d[4]);
		bp.bpgt(dat[5],d[5]);
		bp.bpgt(dat[6],d[6]);
		bp.bpgt(dat[7],d[7]);
		if(j==50000){
			cout<<"mse: "<<bp.mse(d[7])<<endl;
			j=1;
		}
	}

	bp.ffwd(dat[0]);
	cout<<"0,0,0="<<bp.Out(0)<<endl;
	bp.ffwd(dat[1]);
	cout<<"0,0,1="<<bp.Out(0)<<endl;
	bp.ffwd(dat[2]);
	cout<<"0,1,0="<<bp.Out(0)<<endl;
	bp.ffwd(dat[3]);
	cout<<"0,1,1="<<bp.Out(0)<<endl;
	bp.ffwd(dat[4]);
	cout<<"1,0,0="<<bp.Out(0)<<endl;
	bp.ffwd(dat[5]);
	cout<<"1,0,1="<<bp.Out(0)<<endl;
	bp.ffwd(dat[6]);
	cout<<"1,1,0="<<bp.Out(0)<<endl;
	bp.ffwd(dat[7]);
	cout<<"1,1,1="<<bp.Out(0)<<endl;
}