// brain.h

#ifndef _brain_
#define _brain_



class CBrain
{
	// time delay in child thread cycles
	int delay;

public:

	// state of the application
	static bool bReady;		//CBrain instatiated
	static bool bRunning;	//performing training/testing
	static bool bSaved;		//object saved to file

	// open
	// not overridden
	void fileOpen();

	// save
	// not overridden
	void fileSave();

	// save as
	// not overridden
	void fileSaveAs();

	// train
	void fileTrain();

	// test
	void fileTest();

	// get parameters from user via dialogbox
	// called in IDM_CONFIGURE
	virtual int configure();

	// draw controls
	// will respond to WM_CREATE
	virtual void create();

	// painting code here
	// will respond to WM_PAINT
	virtual void paint(HDC hdc);
	
	// will be called in context of child thread
	virtual void train(char *fpath);
	virtual void test();

	// DEBUG
	//virtual void setConfig(int numl,int *lsz,int nume,double lr,double mt,double mset);

private:
	// initialize ofn structure
	// for open/save...
	void initOFN();

	// write object to file
	// in 'save' and 'save as'
	virtual void save_to_file(char * szFilePath);

	// read file and initialize object
	// for 'open'
	virtual void init_from_file(char * szFilePath);

};

#endif