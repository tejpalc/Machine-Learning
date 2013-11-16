#include"stdafx.h"
#include "backprop.h"
#include "resource.h"
#include <stdio.h>

//extern CBrain *brain;
extern HWND g_hWnd;
extern  bool bRunThread;

void DisplayErr(char *err);

/*void CBackProp::init(){

}*/

void CBackProp::train(char *fpath){
	
	sprintf(status,"%-25s","Training");
	double *input,*tgt;
	CDataSet dat;
	dat.Open(fpath/*"c:\\windows\\desktop\\xor-3x8.txt"*/,numIn+numOut,numIn,numOut);
	input=new double[numIn];
	tgt=new double[numOut];

	int updtAt=numEpoch/1000;
	int j=updtAt;
	double dmse,dproj;

	for(int i=0;i<numEpoch && bRunThread ;i++){
		dat.GetInputTargetPair(input,tgt);
        
		
		char buff[20];

        printf("aaaa %u",tbBeta.GetPos());
		itoa(tbAlfa.GetPos(),buff,strlen(buff));

        MessageBox(0,buff,"xx",0);

        bpnet.setAlpha((double)tbAlfa.GetPos() / 20);
        bpnet.setBeta((double)tbBeta.GetPos() / 20);



		bpnet.bpgt(input,tgt);
		if(j==updtAt){
			j=0;
			dmse=bpnet.mse(tgt);
			graph1.Plot(dmse*1000);
			dproj=bpnet.proj(tgt);
			graph2.Plot(abs(dproj*1000));//graph2.Plot(1000+dproj*1000);
			pbEpochs.Step();
			//pbEpochs.SetPos(i/updtAt);
		}
		j++;
	}
	delete[] input;
	delete[] tgt;
	memset(status,0,25);
}

CBackProp::CBackProp(){
	numEpoch=0;
	memset(status,0,25);
	for(int i=0;i<7;i++){
		memset(info[i],0,25);
	}
}

void CBackProp::test(){
}

void CBackProp::updateGraph(){
}

void CBackProp::getParams(){
}

void CBackProp::create(){

	graph1.SetRange(0,numEpoch,0,1000);
	graph2.SetRange(0,numEpoch,0,2000);

	InfoFont=CreateFont(12,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Ariel");

	PaneFont=CreateFont(12,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Ariel");

	StatusFont=CreateFont(14,0,0,0,500,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Courier New");
	
	hbrStatus = (HBRUSH)GetStockObject(LTGRAY_BRUSH);// CreateSolidBrush(RGB(130,130,130));
	
	hpenStatus = (HPEN)CreatePen(PS_SOLID,1,RGB(50,50,80));
	
	hbrPane = (HBRUSH)CreateSolidBrush(RGB(100,100,125));

	sprintf(ticLabel,"0.0                              1.0");
	//these hold the dimensions of the client window area
	static int cxClient, cyClient;
	//get the size of the client window
	RECT rect;
	GetClientRect(g_hWnd, &rect);
	cxClient = rect.right;
	cyClient = rect.bottom;
	graph1.SetRgn(140,0,cxClient,cyClient/2-10);//cyClient-20);
	graph2.SetRgn(140,cyClient/2-10,cxClient,cyClient-20);
	HDC dc;
	dc=GetDC(g_hWnd);
	paint(dc);
	ReleaseDC(g_hWnd,dc);
	//progress/////
	pbEpochs.CreateProgBar(g_hWnd,cxClient*3/4-5,cyClient-16,170,10);
	pbEpochs.SetRange(0,10);
	if(!pbEpochs.SetRange(0,1000))
	MessageBox(g_hWnd,"error bp range"," ",0);
	pbEpochs.SetStepSize(1);

	//pbEpochs.SetColor(RGB(110,110,145),CLR_DEFAULT);
	///////////////
	//trackbar/////
	tbBeta.Create(g_hWnd,8,220,122,20);
	tbBeta.SetRange(0,20);
	tbBeta.SetTicFreq(2);
	
	tbAlfa.Create(g_hWnd,8,270,122,20);
	tbAlfa.SetRange(0,20);
	tbAlfa.SetTicFreq(2);

	tbNoise.Create(g_hWnd,8,320,122,20);
	tbNoise.SetRange(0,10);
	tbNoise.SetTicFreq(1);
	
	tbDelay.Create(g_hWnd,8,370,122,20);
	tbDelay.SetRange(0,10);
	tbDelay.SetTicFreq(1);
	///////////////

}

void CBackProp::setTrainFpath(char *fpath){
	trainFpath=fpath;
}


void CBackProp::paint(HDC hdc){
	//these hold the dimensions of the client window area
	static int cxClient, cyClient;
	//get the size of the client window
	RECT rect;
	

	GetClientRect(g_hWnd, &rect);
	cxClient = rect.right;
	cyClient = rect.bottom;
	// draw graph
	graph1.SetCaption("Mean Square Error");
	graph2.SetCaption("Generalisation Error (By Cross Validation)");
	graph1.paint(hdc);
	graph2.paint(hdc);
	// draw statusbar
	SelectObject(hdc,hbrStatus);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	Rectangle(hdc,0,cyClient-20,cxClient,cyClient);
	// draw left pane
	SelectObject(hdc,hbrPane);
	Rectangle(hdc,0,0,140,cyClient-20);
	// draw tbar enclosure
	SelectObject(hdc,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	Rectangle(hdc,8,200,132,408);
	// draw upper rect
	Rectangle(hdc,8,8,132,192);
	// write info
	
	char buff[50];
	SelectObject(hdc,InfoFont);
	SetBkColor(hdc,RGB(192,192,192));
	SetTextColor(hdc,RGB(0,0,192));
	sprintf(buff,"%-25s",info[0]);
	TextOut( hdc,15,30,buff,25);
	
	sprintf(buff,"%-25s",info[1]);
	TextOut( hdc,15,50,buff,25);
	
	sprintf(buff,"%-25s",info[2]);
	TextOut( hdc,15,70,buff,25);
	
	sprintf(buff,"%-25s",info[3]);
	TextOut( hdc,15,90,buff,25);
	
	sprintf(buff,"%-25s",info[4]);
	TextOut( hdc,15,110,buff,25);

	sprintf(buff,"%-25s",info[5]);
	TextOut( hdc,15,130,buff,25);

	sprintf(buff,"%-25s",info[6]);
	TextOut( hdc,15,150,buff,25);

	// wtite tic label
	SelectObject(hdc,PaneFont);
	SetBkColor(hdc,RGB(192,192,192));
	SetTextColor(hdc,RGB(0,0,10));
	TextOut( hdc,12,242,ticLabel,36);
	TextOut( hdc,12,292,ticLabel,36);
	TextOut( hdc,12,342,ticLabel,36);
	TextOut( hdc,12,392,ticLabel,36);
	
	sprintf(buff,"%-25s","      Learning Rate");
	TextOut(hdc,15,205,buff,25);
	sprintf(buff,"%-25s","         Momentum");
	TextOut(hdc,15,255,buff,25);
	sprintf(buff,"%-25s","           Noise");
	TextOut(hdc,15,305,buff,25);
	sprintf(buff,"%-25s","        Time Delay");
	TextOut(hdc,15,355,buff,25);

	// write status
	SelectObject(hdc,PaneFont);
	SetBkColor(hdc,RGB(192,192,192));
	SetTextColor(hdc,RGB(255,10,10));

	sprintf(status,"%-25s"," TRAINING");
	TextOut(hdc,45,cyClient-17,status,25);


}

void CBackProp::setConfig(int numl,int *lsz,int nume,double lr,double mt,double mset,double noise){
	ns=noise;
	numEpoch=nume;
	tolerence=mset;
	numIn=lsz[0];
	numOut=lsz[numl-1];
	bpnet.init(numl,lsz,lr,mt);
}


extern int numLayers,numNeurons[25],numEpochs;
extern double lrate,momentum,tol,noise;

int CBackProp::configure(){
	char buff[25];
	if(DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, g_hWnd, (DLGPROC)Config)==IDOK){
		setConfig(numLayers,numNeurons,numEpochs,lrate,momentum,tol,noise);
		sprintf(info[0],"Layers: %d",numLayers);
		sprintf(info[1],"Neurons:");
		for(int i=0;i<numLayers;i++){
			sprintf(buff," %d",numNeurons[i]);
			strcat(info[1],buff);
		}
		sprintf(info[2],"MSE Tolerence: %3.2f",tol);	
		sprintf(info[3],"Epochs: %d",numEpochs);
		sprintf(info[4],"Learning Rate: %3.2f",lrate);
		sprintf(info[5],"Momnetum: %3.2f",momentum);
		sprintf(info[6],"Noise: %3.2f",noise);
        
		tbAlfa.SetPos(momentum*20);
		tbBeta.SetPos(lrate*20);

		return 1;
	}
	return 0;
	
}

