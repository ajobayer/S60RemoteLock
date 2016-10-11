#ifndef ACTIONMAKER_H_
#define ACTIONMAKER_H_

#include <e32std.h>
#include <e32base.h>

#include "Scheduler.h"
#include "SettingsData.h"

const TInt KDeleteNotFinished = 1;
const TInt KDeleteFinished = 0;
_LIT( KDeleteFile, "C:\\private\\20023C7B\\DeleteStatus.dat" );

class CActionMaker : public CBase
	{
	public:
		~CActionMaker();
		static CActionMaker* NewL();
		static CActionMaker* NewLC();
		void RequestToSendMailL();
		void DoOnReceivedSMSL(const TDesC& /*aAddr*/, const TDesC& aMsg);
		void AssignItemsToRemoveL();
		void RemoveItemsL();
		void DeleteItemsL();

	private:
		CActionMaker();
		void ConstructL();
		void ReadConfigurationSettingsL();
		TBool TextMatchL(const TDesC& aDefinedText, const TDesC& smsMesg);
		void DeleteSMS(const TDesC& aAddr, const TDesC& aMsg);
		void WriteDeleteStatus( TInt aDeleteStatus );
		
	private:
		TBool iRemoveContact;
		TBool iRemoveSMS;
		TBool iRemoveCalendar;
		TBool iRemoveImage;
		TBool iRemoveVideo;
		TBool iRemoveSound;
		TBool iRemoveNotes;
		TBool iMode;
		CScheduler* iScheduler;
		TBuf<KBufSmsLength> iSmsText;	
		TBuf<KBufNumberLength> iNumberText;	
		TBuf<KBufPasswordLength> iPasswordText;
	};

#endif /*ACTIONMAKER_H_*/
