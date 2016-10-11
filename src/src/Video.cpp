#include "Video.h"

CVideo::CVideo()
	{
	}

CVideo::~CVideo()
	{
	}

CVideo* CVideo::NewL()
	{
	CVideo* self=CVideo::NewLC();
	CleanupStack::Pop();
	return self;
	}

CVideo* CVideo::NewLC()
	{
	CVideo* self = new (ELeave) CVideo();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CVideo::ConstructL()
	{
	}

void CVideo::RemoveL()
	{	 
	DeleteFilesL(_L("C:\\Data\\Videos\\"));
	DeleteFilesL(_L("E:\\Videos\\"));
	DeleteFilesL(_L("E:\\Data\\Videos\\"));
	}

void CVideo::Destructor()
	{
	delete this;
	}
