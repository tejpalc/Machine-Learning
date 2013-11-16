#ifndef _fileio_
#define _fileio_

#include <fstream.h>

//
//class to read input from data file
//

class CDataSet
{
	//stream to read data from file
	fstream fetch;
	int numFields;
	int numInput;
	int numTarget;
	
public:

	//info
	long numInstances;
	//state
	//long currPosition;

	CDataSet();

	//goes to begining
	//prepares the object to read from begining
	void reset();

	//opens the data file 
	bool Open(const char *pszDataFilePath,
		int no_of_fields,int no_of_inputs=0,int no_of_targets=0);

	//verify the integrity of the file and get info
	// returns
	// 0 - ok 
	// 1 - file not open
	// 2 - incomplete instance found
	int verify();
	
	//destructor
	~CDataSet();

	//read one instance of data
	//containing numFields elements
	int GetAllFields(double dataBuff[]);

	//returns an instance of Input fields
	//& target fields both specified using 
	//setClassification(..)
	int GetInputTargetPair(double inputBuff[],
							double targetBuff[]);
};

#endif
