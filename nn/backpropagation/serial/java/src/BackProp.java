import java.util.*;

class BackProp implements Runnable{

boolean runThread = true;

// output of each neuron
 double out[][];

// delta error value for each neuron
 double delta[][];

// vector of weights for each neuron
 double weight[][][];

// no of layers in net 
// including input layer
 int numl;

// vector of numl elements for size
// of each layer
 int lsize[];

// learning rate
 double beta;

// momentum parameter
 double alpha;

// storage for weight-change made 
// in previous epoch
 double prevDwt[][][];

/* Private Methods

// squashing function
 double sigmoid(double in);

/* Public Methods

 BackProp();

 ~BackProp();

// initializes and allocates memory on heap 
 BackProp(int nl,int sz[],double b,double a);

// initializes and allocates memory on heap
 void init(int nl,int sz[],double b,double a);

// backpropogates error for one set of input
 void bpgt(double in[],double tgt[]); 

// feed forwards activations for one set of inputs
 void ffwd(double in[]);

// returns mean square error of the net
 double mse(double tgt[]); 
 
// returns i'th output of the net
 double Out(int i) const; 
*/

public BackProp()
	{}


// initializes and allocates memory on heap
public BackProp(int nl,int sz[],double b,double a)
{

 beta = b;
 alpha = a;

 // set no of layers and their sizes
 numl=nl;
 lsize=new int[numl]; 

 for(int i=0;i<numl;i++){
  lsize[i]=sz[i];
 }

 // allocate memory for output of each neuron
 out = new double[numl][];

 for( int i=0;i<numl;i++){
  out[i]=new double[lsize[i]];
 }

 // allocate memory for delta
 delta = new double[numl][];

 for(int i=1;i<numl;i++){
  delta[i]=new double[lsize[i]];
 }

 // allocate memory for weights
 weight = new double[numl][][]; 

 // weight[0] is intentionally redundant for simplicity in backpropagation code
 for(int i=1;i<numl;i++){
  weight[i]=new double[lsize[i]][];
 }
 for(int i=1;i<numl;i++){
  for(int j=0;j<lsize[i];j++){ 
   weight[i][j]=new double[lsize[i-1]+1];
  }
 }

 // allocate memory for previous weights
 prevDwt = new double[numl][][];

 for(int i=1;i<numl;i++){
  prevDwt[i]=new double[lsize[i]][];

 }
 for(int i=1;i<numl;i++){
  for(int j=0;j<lsize[i];j++){
   prevDwt[i][j]=new double[lsize[i-1]+1];
  }
 }


 // seed and assign random weights
// srand((unsigned)(time(NULL)));
Random rand = new Random();
for(int i=1;i<numl;i++) 
  for(int j=0;j<lsize[i];j++)
   for(int k=0;k<lsize[i-1]+1;k++)
    weight[i][j][k]=rand.nextDouble();//(double)(rand())/(RAND_MAX/2) - 1;//32767

 // initialize previous weights to 0 for first iteration
 for(int i=1;i<numl;i++) 
  for(int j=0;j<lsize[i];j++)
   for(int k=0;k<lsize[i-1]+1;k++)
    prevDwt[i][j][k]=(double)0.0;
 
}

/* 

~BackProp()
{
 // free out
 for(int i=0;i<numl;i++)
  delete[] out[i];
 delete[] out;

 // free delta
 for(i=1;i<numl;i++)
  delete[] delta[i];
 delete[] delta;

 // free weight
 for(i=1;i<numl;i++)
  for(int j=0;j<lsize[i];j++)
   delete[] weight[i][j]; 
 for(i=1;i<numl;i++)
  delete[] weight[i];
 delete[] weight;

 // free prevDwt
 for(i=1;i<numl;i++)
  for(int j=0;j<lsize[i];j++)
   delete[] prevDwt[i][j];
 for(i=1;i<numl;i++)
  delete[] prevDwt[i];
 delete[] prevDwt;

 // free layer info
 delete[] lsize;
}

*/

// sigmoid function
double sigmoid(double in)
{
  return (double)((1/(1+ Math.exp(-in))));
}

// mean square error 
double mse(double tgt)
{
 double mse=0;
// there always be one output!!! for(int i=0;i<lsize[numl-1];i++){
//  mse+=(tgt[i]-out[numl-1][i])*(tgt[i]-out[numl-1][i]);
// }
mse=(tgt-out[numl-1][0])*(tgt-out[numl-1][0]);
 return mse;
}


// returns i'th output of the net 
double Out(int i)
{
 return out[numl-1][i];
}

// feed forward one set of input
void ffwd(double in[])
{
 double sum;

 // assign content to input layer
 for(int i=0;i<lsize[0];i++) 
  out[0][i]=in[i];  // output_from_neuron(i,j) Jth neuron in Ith Layer

 // assign output(activation) value
 // to each neuron usng sigmoid func
 for(int i=1;i<numl;i++){    // For each layer
  for(int j=0;j<lsize[i];j++){  // For each neuron in current layer 
   sum=0.0;
   for(int k=0;k<lsize[i-1];k++){  // For input from each neuron in preceeding layer
    sum+= out[i-1][k]*weight[i][j][k]; // Apply weight to inputs and add to sum
   }

//   if( i == numl-1)
//	  {		sum+=weight[i][j][lsize[i-1]];  // Apply bias 
//			out[i][j] = (sigmoid(sum)+1)/2;//sum/lsize[i];
//	  }
//	else
	  {
		   sum+=weight[i][j][lsize[i-1]];  // Apply bias 
		   out[i][j]=sigmoid(sum);    // Apply sigmoid function
	  }
  }
 }
}


// backpropogate errors from output
// layer uptill the first hidden layer
void bpgt(double in[] ,double tgt)
{
 double sum; 

 // update output values for each neuron
 ffwd(in);

 // find delta for output layer
 for(int i=0;i<lsize[numl-1];i++){
  delta[numl-1][i]=out[numl-1][i]*
  (1-out[numl-1][i])*(tgt-out[numl-1][i]); 
 }

 // find delta for hidden layers 
 for(int i=numl-2;i>0;i--){
  for(int j=0;j<lsize[i];j++){
   sum=0.0;
   for(int k=0;k<lsize[i+1];k++){
    sum+=delta[i+1][k]*weight[i+1][k][j];
   } 
   delta[i][j]=out[i][j]*(1-out[i][j])*sum;
  }
 }

 // apply momentum ( does nothing if alpha=0 )
 for(int i=1;i<numl;i++){
  for(int j=0;j<lsize[i];j++){
   for(int k=0;k<lsize[i-1];k++){
    weight[i][j][k]+=alpha*prevDwt[i][j][k];
   }
   weight[i][j][lsize[i-1]]+=alpha*prevDwt[i][j][lsize[i-1]];
  }
 }

 // adjust weights usng steepest descent 
 for(int i=1;i<numl;i++){
  for(int j=0;j<lsize[i];j++){ 
   for(int k=0;k<lsize[i-1];k++){
    prevDwt[i][j][k]=beta*delta[i][j]*out[i-1][k];
    weight[i][j][k]+=prevDwt[i][j][k];
   }
   prevDwt[i][j][lsize[i-1]]=beta*delta[i][j];
   weight[i][j][lsize[i-1]]+=prevDwt[i][j][lsize[i-1]]; 
  }
 }
}



public static void main(String args[])
{

 // Creating the net
 BackProp bp = new BackProp();
	Thread bpThread = new Thread(bp);
	bpThread.start();
}

public void run()
{
 // prepare XOR traing data
 double data[][]={
	{0, 0, 0, 0},
	{0, 0, 1, 1},
	{0, 1, 0, 1},
	{0, 1, 1, 0},
	{1, 0, 0, 1}, 
	{1, 0, 1, 0},
	{1, 1, 0, 0},
	{1, 1, 1, 1} };

 // prepare test data 
 double testData[][]={
	{0, 0, 0},  
	{0, 0, 1},  
	{0, 1, 0},  
	{0, 1, 1},  
	{1, 0, 0},  
	{1, 0, 1},  
	{1, 1, 0},  
	{1, 1, 1} }; 


 // defining a net with 4 layers having 3,3,3, and 1 neuron respectively
 // *the first layer is input layer i.e. simply holder for the input parameters
 //  and has to be the same size as the no of input parameters, in out example 3 
 int numLayers = 4, lSz[] = {3,3,3,1};

 // Learing rate - beta
 // momentum - alpha
 // Threshhold - thresh (value of target mse, training stops once it is acquired)
 double beta = 0.1, alpha = 0.01 , Thresh =  0.00001;

 // maximum no of iterations during training
 long num_iter = 1000000;

 
 // Creating the net
 BackProp bp = new BackProp(numLayers, lSz, beta, alpha);
 


 System.out.println("Now training the network...." );
 long index;
 int i = 0;
 for ( index=0; index<num_iter && runThread; index++)
 {
  i = (new Long(index%8)).intValue();

  bp.bpgt(data[i%8],data[i%8][3] );

  if( bp.mse(data[i%8][3]) < Thresh) {
   System.out.println("Network Trained. Threshold value achieved in " + i + " iterations." );
   System.out.println("MSE:  " + bp.mse(data[i%8][3]));
   break;
  }
  if ( index%(num_iter/10) == 0 )
   System.out.println("MSE:  " + bp.mse( data[i%8][3]) + "... Training..." ); 

 }
 
 if ( index == num_iter )
  System.out.println("iterations completed..." + "MSE: " + bp.mse( data[(i-1)%8][3]) );   

 System.out.println("Now using the trained network to make predctions on our test data...."); 
 for ( i = 0 ; i < 8 ; i++ )
 {
  bp.ffwd(testData[i]);
  System.out.println(testData[i][0] + "  " + testData[i][1] + "  "  + testData[i][2] + "  " + bp.Out(0)); 
 }

}

}// end class def

 

