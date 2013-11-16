#include"stdafx.h"
#include "FileIO.h"

CDataSet::CDataSet():numInstances(0),//currPosition(0),
					numFields(0),numInput(0),numTarget(0)
{ }

//
//open the data file for reading
//
bool CDataSet::Open(const char *pszDataFilePath,int no_of_fields,
					int no_of_inputs,int no_of_targets){
	numFields=no_of_fields;
	numInput=no_of_inputs;
	numTarget=no_of_targets;
	fetch.open(pszDataFilePath,ios::in|ios::nocreate);
	if(fetch.fail())
		return 1;
	return 0;
}

//
//close when done
//
CDataSet::~CDataSet(){
	fetch.close();
}

//
//verify the integrity of the file and get info
//
CDataSet::verify(){
	double dummy;
	//is everything allright
		if(!fetch.is_open())
			return 1;
	while(!fetch.eof())	{
		for(int i=0 ; i<numFields ; i++){
			//to avoid returning incomplete instance
			//the record was smaller than numFields
			if(fetch.peek() =='\n' && i>0){
				fetch.clear();
				fetch.seekg(0,ios::beg);
				return 2;
			}
			if(fetch.eof() && i>0){
				fetch.clear();
				fetch.seekg(0,ios::beg);
				return 2;
			}
			fetch>>dummy;
		}
		numInstances++; // count instances
	}
	fetch.clear();
	fetch.seekg(0,ios::beg);
	return 0;
}

//goes to begining
//prepares the object to read from begining
void CDataSet::reset(){
	//currPosition=0;
	fetch.clear();
	fetch.seekg(0,ios::beg);
}



//
//fetch an instance of iNumFields elements
//
int CDataSet::GetAllFields(double dataBuff[]){
	//is everything allright
	if(!fetch.is_open())
			return 1;
	//out of records
	if(fetch.eof())
			return 2;
		
	for(int i=0 ; i<numFields ; i++){
		//fill the double array
		fetch >> dataBuff[i];
	}
	return 0;
}


int CDataSet::GetInputTargetPair(double inputBuff[],
								   double targetBuff[]){
	//is everything allright
	if(!fetch.is_open())
		return 1;

	if(fetch.eof())
		reset();
		//return 2;

	int icount=0,tcount=0;
	
	// assumption: file integrity has been verified
	for (int i=0 ; i < numInput ; i++ )
		fetch>>inputBuff[i];
	//i--;
	for( int j=0 ; j <  numTarget ;j++) 
		fetch>>targetBuff[j];
	return 0;
}