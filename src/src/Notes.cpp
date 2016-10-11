#include "Notes.h"

CNotes::CNotes()
	{
	}

CNotes::~CNotes()
	{
	}

CNotes* CNotes::NewL()
	{
	CNotes* self=CNotes::NewLC();
	CleanupStack::Pop();
	return self;
	}

CNotes* CNotes::NewLC()
	{
	CNotes* self = new (ELeave)CNotes();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CNotes::ConstructL()
	{
	}

void CNotes::RemoveL()
	{
	DeleteFilesL(_L("C:\\Documents\\"));
	DeleteFilesL(_L("C:\\Data\\Documents\\"));
	DeleteFilesL(_L("E:\\Documents\\"));
	DeleteFilesL(_L("E:\\Data\\Documents\\"));
	//DeleteFilesL( _L("C:\\System\\Data\\Notepad.dat") );
	}

void CNotes::Destructor()
	{
	delete this;
	}
