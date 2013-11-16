#include "stdafx.h"
#include"graph.h"
#include<stdio.h>
#include<assert.h>

#define MARGIN 32

extern HWND g_hWnd;

void CGraph::SetRange(int xRmin,int xRmax,int yRmin,int yRmax){
	xRangeMin	=	xRmin;
	xRangeMax	=	xRmax;
	yRangeMin	=	yRmin;
	yRangeMax	=	yRmax;
}



// initialize graph rectangle and logical range
CGraph::SetRgn(int iLeft,int iTop,int iRight,int iBottom)
{
	numPoints	=	0;
	rect.top	=	iTop;
	rect.left	=	iLeft;
	rect.right	=	iRight;
	rect.bottom	=	iBottom;

	gx=rect.left+MARGIN;
	gy=rect.top+MARGIN;
	gwidth=rect.right-rect.left-2*MARGIN;
	gheight=rect.bottom-rect.top-2*MARGIN;

//	delete them in destructor

	labelFont=CreateFont(13,0,0,0,200,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Arial");
	CaptionFont=CreateFont(14,0,0,0,500,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Courier New");
	
	hbrBk = (HBRUSH)CreateSolidBrush(RGB(130,130,155));;
	hbrGraph = (HBRUSH)CreateSolidBrush(RGB(224,224,228));
	hpenBound = (HPEN)CreatePen(PS_SOLID,2,RGB(50,50,80));
	hpenGrid = (HPEN)CreatePen(PS_SOLID,1,RGB(200,200,200));
	hpenPlot = (HPEN)CreatePen(PS_SOLID,1,RGB(255,0,0));

}

CGraph::~CGraph()
{
	DeleteObject(hpenBound);
	DeleteObject(labelFont);
	DeleteObject(hbrBk);
	DeleteObject(hbrGraph);
	DeleteObject(hpenGrid);
	DeleteObject(hpenPlot);
}

// specify next position
// ~less than 1000 calls~
// WM_PAINT should follow
void CGraph::Plot(int yPos){
	HDC hdc;
	assert( numPoints<1000 && "error: graph coordinate out of range");
	plot[numPoints].x=gx + numPoints * gwidth /1000;
	plot[numPoints].y = gy +
	( (yRangeMax-yRangeMin) - (yPos-yRangeMin) ) * gheight / (yRangeMax-yRangeMin) ;
	hdc=GetDC(g_hWnd);
	SelectObject(hdc,hpenPlot);
	/*if(numPoints!=0){
		MoveToEx(hdc,plot[numPoints-1].x,plot[numPoints].y,NULL);
		LineTo(hdc,plot[numPoints].x,plot[numPoints].y);
	}*/
	Polyline(hdc,plot,numPoints);

	ReleaseDC(g_hWnd,hdc);
	SendMessage(g_hWnd,WM_PAINT,0,0);
	numPoints++;
}

CGraph::paint(HDC hdc){

	//SetRange(0,25000,0,1000);//DEBUG
	char buffer[25];

//	draw background
	SelectObject(hdc,hbrBk);
	SelectObject(hdc,GetStockObject(NULL_PEN));
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);

//	draw graph region
	SelectObject(hdc,hbrGraph);
	Rectangle(hdc,gx,gy,gx+gwidth,gy+gheight);
	MoveToEx(hdc,0,0,NULL);

//	draw grid
	SelectObject(hdc, hpenGrid);
	int xSpace,ySpace;
	xSpace = gwidth / 10;
	ySpace = gheight / 10;
	for(int i=1;i<10;i++){
		MoveToEx(hdc,gx+xSpace*i,gy,NULL);
		LineTo(hdc,gx+xSpace*i,gy+gheight);
		MoveToEx(hdc,gx,gy+ySpace*i,NULL);
		LineTo(hdc,gx+gwidth,gy+ySpace*i);
	}

//	write labels
	SelectObject(hdc,labelFont);
	SetBkColor(hdc,RGB(130,130,155));
	SetTextColor(hdc,RGB(0,0,80));
	int diff=(xRangeMax-xRangeMin)/10;
	for(i=1;i<=10;i++){
		sprintf(buffer,"%-8d",xRangeMin + i*diff);
		TextOut(hdc,rect.left+MARGIN*1/3+i*xSpace,rect.bottom-MARGIN*3/4,buffer,8);
		sprintf(buffer,"%2.1f",(float)0.1*i);
		TextOut(hdc,rect.left+MARGIN*1/3,rect.bottom-MARGIN-5-i*ySpace,buffer,3);
	}

//	draw bounding perpendicular lines
	SelectObject(hdc,hpenBound);
	MoveToEx(hdc,gx,gy,NULL);
	LineTo(hdc,gx,gy+gheight);
	MoveToEx(hdc,gx,gy+gheight,NULL);
	LineTo(hdc,gx+gwidth,gy+gheight);
	
	SelectObject(hdc,hpenPlot);
	Polyline(hdc,plot,numPoints);

//	SetCaption("X - No of Epochs    Y - Mean Square Error");//DEBUG 
//	write caption
	SelectObject(hdc,CaptionFont);
	SetBkColor(hdc,RGB(130,130,155));
	SetTextColor(hdc,RGB(200,200,230));//RGB(40,40,120));
	TextOut(hdc,gx+20,gy-24,caption,70);

}

//	set caption for the graph
void CGraph::SetCaption(char text[])
{
	sprintf(caption,"%-70s",text);
}