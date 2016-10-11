#include <aknenv.h>

#include "SIMDetector.h"
#include "DevInfoUtility.h"
#include "ActionMaker.h"
//#include "ConstantList.h"

_LIT( KIMSIFile, "C:\\private\\20023C7B\\IMSIFIle.dat" );
_LIT( KDefaultIMSI, "000000000000000");
const TInt KBufIMSILength = 256;

CSIMDetector::CSIMDetector()
	{
	}

CSIMDetector::~CSIMDetector()
	{
	fileSession.Close();
	}

CSIMDetector* CSIMDetector::NewL()
	{
	CSIMDetector* self=CSIMDetector::NewLC();
	CleanupStack::Pop();
	return self;
	}

CSIMDetector* CSIMDetector::NewLC()
	{
	CSIMDetector* self = new (ELeave) CSIMDetector();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CSIMDetector::ConstructL()
	{
	User::LeaveIfError(fileSession.Connect());
	}

void CSIMDetector::DetectSIMChange()
	{
	TBuf<250> subscriberIdentity;
	TSubscriberId subscriberId;
	CDevInfoUtility* devInfoUtility = CDevInfoUtility::NewL();
	CleanupStack::PushL(devInfoUtility);
	devInfoUtility->GetSubscriberInfo(subscriberId);
	CleanupStack::PopAndDestroy(devInfoUtility);
	subscriberIdentity.Copy(subscriberId);
	if(subscriberIdentity.Compare(KDefaultIMSI) == 0)
		{
		  return;
		}

	TInt imsiLength = 250;
	TText *imsiBuffer;
	TPtr *imsiValue;
	
	imsiBuffer = new (ELeave) TText[imsiLength];
	CleanupStack::PushL(imsiBuffer);
	imsiValue =new (ELeave) TPtr(imsiBuffer,0,imsiLength);
	CleanupStack::PushL(imsiValue);

	imsiValue->Copy(subscriberIdentity);

	RFile file;
	TInt error = file.Open(fileSession,KIMSIFile,EFileRead);
	if(error == KErrNone )
		{
		TInt readIMSILength;
		TText *readIMSIBuffer;
		TPtr *readIMSIValue;
		TBuf<KBufIMSILength> prevSubscriberID;

		RFileReadStream readStream(file);

		readIMSILength = readStream.ReadInt16L();
		readIMSIBuffer = new (ELeave) TText[readIMSILength];
		CleanupStack::PushL(readIMSIBuffer);
		readIMSIValue =new (ELeave) TPtr(readIMSIBuffer,0,readIMSILength);
		CleanupStack::PushL(readIMSIValue);
		readStream.ReadL(*readIMSIValue,readIMSILength);

		readStream.Close();
		file.Close();

		if(readIMSIValue->Compare(*imsiValue) != 0)
			{
			StoreSubscriberInfo(subscriberIdentity);
			RequestToSendMail();
			}
		CleanupStack::PopAndDestroy(readIMSIValue);
		CleanupStack::PopAndDestroy(readIMSIBuffer);

		}
	else
		{
		StoreSubscriberInfo(*imsiValue);
		}

	CleanupStack::PopAndDestroy(imsiValue);
	CleanupStack::PopAndDestroy(imsiBuffer);
	}

void CSIMDetector::StoreSubscriberInfo(const TDesC& aIMSI)
	{
	RFile rFile;

	User::LeaveIfError(rFile.Replace(fileSession, KIMSIFile, EFileWrite));
	CleanupClosePushL(rFile);
	RFileWriteStream outputStream(rFile);
	CleanupClosePushL(outputStream);

	outputStream.WriteInt16L(aIMSI.Length());
	outputStream.WriteL(aIMSI);
	CleanupStack::PopAndDestroy(2);
	}

void CSIMDetector::RequestToSendMail()
    {
	CActionMaker* actionMaker;
	actionMaker = CActionMaker::NewL();
	TRAPD(err, actionMaker->RequestToSendMailL());
	if(actionMaker)
		{
		delete actionMaker;
		actionMaker = NULL;
		}
    }

