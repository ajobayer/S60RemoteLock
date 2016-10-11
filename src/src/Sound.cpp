#include "Sound.h"

CSound::CSound()
	{
	}

CSound::~CSound()
	{
	}

CSound* CSound::NewL()
	{
	CSound* self=CSound::NewLC();
	CleanupStack::Pop();
	return self;
	}

CSound* CSound::NewLC()
	{
	CSound* self = new (ELeave) CSound();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CSound::ConstructL()
	{
	}

void CSound::RemoveL()
	{	 
	DeleteFilesL(_L("C:\\Data\\Sounds\\"));
	DeleteFilesL(_L("E:\\Sounds\\"));
	DeleteFilesL(_L("E:\\Data\\Sounds\\"));
	}

void CSound::Destructor()
	{
	delete this;
	}
