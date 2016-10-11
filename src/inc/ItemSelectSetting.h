#ifndef ITEMSELECTSETTING_H_
#define ITEMSELECTSETTING_H_

#include <e32base.h>
#include <in_sock.h>

//#include "ConstantList.h"

const TInt KContact = 0;
const TInt KSMS = 1;
const TInt KCalendar = 2;
const TInt KImage = 3;
const TInt KVideo = 4;
const TInt KSound = 5;
const TInt KNote = 6;
const TInt KEndMarker = 10;

class CItemSelectSetting : public CBase
{
	public:
		virtual ~CItemSelectSetting();
		static CItemSelectSetting* NewL();
		static CItemSelectSetting* NewLC();
		void ResetStatus();
		void ReadValueFromFile();
		TBool GetContactStatus();
		TBool GetSMSStatus();
		TBool GetCalendarStatus();
		TBool GetImageStatus();
		TBool GetVideoStatus();
		TBool GetSoundStatus();
		TBool GetNoteStatus();
		void SetContactStatus( TBool aStatus );
		void SetSMSStatus( TBool aStatus );
		void SetCalendarStatus( TBool aStatus );
		void SetImageStatus( TBool aStatus );
		void SetVideoStatus( TBool aStatus );
		void SetSoundStatus( TBool aStatus );
		void SetNoteStatus( TBool aStatus );
		void WriteValueInFile();

	private:
		CItemSelectSetting();
		void ConstructL();

	private:
		TBool iIsContact;
		TBool iIsSMS;
		TBool iIsCalendar;
		TBool iIsImage;
		TBool iIsVideo;
		TBool iIsSound;
		TBool iIsNote;
};

#endif /*ITEMSELECTSETTING_H_*/
