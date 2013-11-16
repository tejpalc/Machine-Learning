// backprop.h
#ifndef backprop_h
#define backprop_h

#include "graph.h"
#include "fileio.h"
#include "brain.h"
#include "bpnet.h"
#include "trackbar.h"
#include "progbar.h"

class CBackProp:public CBrain{
	int numIn,numOut;
	CGraph graph1,graph2;
	char  *trainFpath,info[7][25],status[25];
	CBpnet bpnet;
	CTrackBar tbAlfa,tbBeta,tbNoise,tbDelay;
	CPBar pbEpochs;

	HBRUSH hbrStatus,hbrPane;
	HPEN hpenStatus;
	HFONT StatusFont,PaneFont,InfoFont;

	bool bAlfaAdapt,bBetaAdapt;
	double alfaMin,alfaMax,betaMin,betaMax;
	int iAlfa,iBeta;
	
	int numEpoch;
	double tolerence,ns;//ns - noise

	char ticLabel[100];

public:
	CBackProp();
	//void init();
	void setTrainFpath(char *fpath);
	void train(char *fpath);//by child
	void test();//by child
	void updateGraph();// by child
	void getParams();// by child

	void create();
	void paint(HDC hdc);
	int configure();

	void setConfig(int numl,int *lsz,int nume,double lr,double mt,double mset,double noise);
};

// dlg proc for config dialog
LRESULT CALLBACK Config(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif