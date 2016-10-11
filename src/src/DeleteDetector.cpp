#include <BAUTILS.H>
#include <aknenv.h> 

#include "DeleteDetector.h"
#include "ActionMaker.h"

CDeleteDetector::CDeleteDetector()
	{
	}

CDeleteDetector::~CDeleteDetector()
	{
	iFileSession.Close();
	}

CDeleteDetector* CDeleteDetector::NewL()
	{
	CDeleteDetector* self = CDeleteDetector::NewLC();
	CleanupStack::Pop();
	return self;
	}

CDeleteDetector* CDeleteDetector::NewLC()
	{
	CDeleteDetector* self = new (ELeave) CDeleteDetector();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CDeleteDetector::ConstructL()
	{
	User::LeaveIfError(iFileSession.Connect());
	}

void CDeleteDetector::DetectToDeleteL()
	{
	TBool result = BaflUtils::FileExists( iFileSession, KDeleteFile );
	if( result == (TInt)ETrue )
		{
		RFile file;
		TInt error = file.Open( iFileSession, KDeleteFile, EFileRead );
		RFileReadStream readStream( file );
		TInt deleteStatus = readStream.ReadInt8L();
		readStream.Close();	
		file.Close();
		
		if( deleteStatus == KDeleteNotFinished )
			{
			DeleteItems();
			}		
		}
	}

void CDeleteDetector::DeleteItems()
    {
	CActionMaker* actionMaker;
	actionMaker = CActionMaker::NewL();
	TRAPD( err, actionMaker->RequestToSendMailL() );
	if( actionMaker )
		{
		delete actionMaker;
		actionMaker = NULL;
		}
    }

