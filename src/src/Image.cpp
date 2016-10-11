#include <f32file.h>
#include <s32file.h>
#include <aknnotewrappers.h>

#include "Image.h"

CImage::CImage()
	{
	}

CImage::~CImage()
	{
	}

CImage* CImage::NewL()
	{
	CImage* self=CImage::NewLC();
	CleanupStack::Pop();
	return self;
	}

CImage* CImage::NewLC()
	{
	CImage* self = new (ELeave) CImage();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CImage::ConstructL()
	{
	}

void CImage::RemoveL()
	{	 

	DeleteFilesL(_L("C:\\Data\\Images\\"));
	DeleteFilesL(_L("E:\\Images\\"));
	DeleteFilesL(_L("E:\\Data\\Images\\"));

	}

void CImage::Destructor()
	{
	delete this;
	}


