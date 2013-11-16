#include "stdafx.h"
#include "brain.h"

void DisplayErr(char *err);


//	MESSAGE HANDLERS

//	painting code here
//	will respond to WM_PAINT
void CBrain::paint(HDC hdc)
{
	//DisplayErr("paint not implemented");
}

//	draw controls
//	will respond to WM_CREATE
void CBrain::create()
{
	DisplayErr("create not implemented");
}


//	MENU HANDLERS	//

//	get parameters from user via dialogbox
//	called in IDM_CONFIGURE
int CBrain::configure()
{
	DisplayErr("Configure not implemented");
	return 0;
}

//	initialize ofn structure
//	for open/save...
void CBrain::initOFN()
{
	DisplayErr("initOFN not implemented");
}

//	open
//	not overridden
void CBrain::fileOpen()
{
	DisplayErr("Configure not implemented");
}

//	save
//	not overridden
void CBrain::fileSave()
{
	DisplayErr("Configure not implemented");
}

//	save as
//	not overridden
void CBrain::fileSaveAs()
{
	DisplayErr("Configure not implemented");
}

//	train
void CBrain::fileTrain()
{
	DisplayErr("Configure not implemented");
}

//	test
void CBrain::fileTest()
{
	DisplayErr("Configure not implemented");
}


// PRIVATE METHODS
// -VIRTUAL-
// IMPLEMENT IN DERIVED CLASS


//	will be called in context of child thread
void CBrain::train(char *fpath)
{
	DisplayErr("Train not implemented");
}

void CBrain::test()
{
	DisplayErr("Test not implemented");
}

//	write object to file
//	in 'save' and 'save as'
void CBrain::save_to_file(char * szFilePath)
{
	DisplayErr("Configure not implemented");
}

//	 read file and initialize object
//	for 'open'
void CBrain::init_from_file(char * szFilePath)
{
	DisplayErr("Configure not implemented");
}