
//////////////////////////////////////////////
//	fully connected multilayered feed		//
//	forward	artificial neural network using	//
//	Backpropogation	algorithm for training.	//
//////////////////////////////////////////////


#ifndef bpnet_h
#define bpnet_h

#include<assert.h>
#include<iostream.h>
#include<stdio.h>
#include<math.h>


class CBpnet{

//	output of each neuron
	double **out;

//	delta error value for each neuron
	double **delta;

//	vector of weights for each neuron
	double ***weight;

//	no of layers in net
//	including input layer
	int numl;

//	vector of numl elements for size 
//	of each layer
	int *lsize;

//	learning rate
	double beta;

//	momentum parameter
	double alpha;

//	storage for weight-change made
//	in previous epoch
	double ***prevDwt;

//	squashing function
	double sigmoid(double in);


public:

	CBpnet();

	~CBpnet();

//	initializes and allocates memory on heap
	CBpnet(int nl,int *sz,double b,double a);

//	initializes and allocates memory on heap
	init(int nl,int *sz,double b,double a);

//	backpropogates error for one set of input
	bpgt(double *in,double *tgt);

//	feed forwards activations for one set of inputs
	ffwd(double *in);

//	returns mean square error of the net
	double mse(double *tgt);	

//	return projection value of net
	double proj(double *tgt);
	
//	returns i'th output of the net
	double Out(int i) const;

//	update alpha
	void setAlpha(double a);

//	update beta
	void setBeta(double b);

};

#endif