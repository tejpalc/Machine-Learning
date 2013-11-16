// graph.h
#ifndef _graph_
#define _graph_

class CGraph
{
	HFONT labelFont,CaptionFont;
	HBRUSH hbrBk,hbrGraph;
	HPEN	hpenGrid,hpenPlot,hpenBound;
//	heading text
	char caption[200];
//	the points representing the graph
	POINT plot[1000];
	int numPoints;
//	logical parameters
	int xRangeMin,yRangeMin,xRangeMax,yRangeMax;
	int xPos,yPos;
//	dimensions of the enclosing rectangle
	RECT rect;
//	region for graph
	int gx,gy,gwidth,gheight;
public:
	~CGraph();
	SetRgn(int iLeft,int iTop,int iRight,int iBottom);
//	initialize graph rectangle and logical range
	void SetRange(int xRmin,int xRmax,int yRmin,int yRmax);
//	specify next logical position
//	~less than 1000 calls~
//	draw the line,add point to array---use getdc()-releasedc()
	void Plot(int yPos);
//	set caption for the graph
	void SetCaption(char text[]);
//	on WM_PAINT 
//	obtain dc from beginpaint()
	paint(HDC dc);
};

#endif