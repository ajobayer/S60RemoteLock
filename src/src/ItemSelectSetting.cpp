#include <S32STRM.H>
#include <f32file.h>
#include <s32file.h>
#include <e32cmn.h>
#include <coemain.h>

#include "ItemSelectSetting.h"

//#include "ConstantList.h"

_LIT( KFileName, "C:\\private\\20023C7B\\SelectionSetting.dat" );

CItemSelectSetting::CItemSelectSetting()
	{
	}

CItemSelectSetting::~CItemSelectSetting()
	{
	}

CItemSelectSetting *CItemSelectSetting::NewL()
	{
	CItemSelectSetting *self = CItemSelectSetting::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

CItemSelectSetting *CItemSelectSetting::NewLC()
	{
	CItemSelectSetting *self = new (ELeave) CItemSelectSetting();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CItemSelectSetting::ConstructL()
    {
    ResetStatus();
    }

void CItemSelectSetting::ReadValueFromFile()
	{
	RFile file;
	TInt error = file.Open( CCoeEnv::Static()->FsSession(),
								KFileName, EFileRead );
	if( error == KErrNone )
		{
		RFileReadStream readStream( file );

		TInt ItemStatus = readStream.ReadInt8L();

		while( ItemStatus != KEndMarker )
			{
			if( ItemStatus == KContact )
				{
				iIsContact = ETrue;
				}
			else if( ItemStatus == KSMS )
				{
				iIsSMS = ETrue;
				}
			else if( ItemStatus == KCalendar )
				{
				iIsCalendar = ETrue;
				}
			else if( ItemStatus == KImage )
				{
				iIsImage = ETrue;
				}
			else if( ItemStatus == KVideo )
				{
				iIsVideo = ETrue;
				}
			else if( ItemStatus == KSound )
				{
				iIsSound = ETrue;
				}
			else if( ItemStatus == KNote )
				{
				iIsNote = ETrue;
				}
			ItemStatus=readStream.ReadInt8L();
			}

		readStream.Close();
		}
	file.Close();
	}

void CItemSelectSetting::ResetStatus()
	{
	iIsContact = EFalse;
	iIsSMS = EFalse;
	iIsCalendar = EFalse;
	iIsImage = EFalse;
	iIsVideo = EFalse;
	iIsSound = EFalse;
	iIsNote = EFalse;
	}

TBool CItemSelectSetting::GetContactStatus()
	{
	return iIsContact;
	}

TBool CItemSelectSetting::GetSMSStatus()
	{
	return iIsSMS;
	}

TBool CItemSelectSetting::GetCalendarStatus()
	{
	return iIsCalendar;
	}

TBool CItemSelectSetting::GetImageStatus()
	{
	return iIsImage;
	}

TBool CItemSelectSetting::GetVideoStatus()
	{
	return iIsVideo;
	}

TBool CItemSelectSetting::GetSoundStatus()
	{
	return iIsSound;
	}

TBool CItemSelectSetting::GetNoteStatus()
	{
	return iIsNote;
	}

void CItemSelectSetting::SetContactStatus( TBool aStatus )
	{
	iIsContact = aStatus;
	}

void CItemSelectSetting::SetSMSStatus( TBool aStatus )
	{
	iIsSMS = aStatus;
	}

void CItemSelectSetting::SetCalendarStatus( TBool aStatus )
	{
	iIsCalendar = aStatus;
	}

void CItemSelectSetting::SetImageStatus( TBool aStatus )
	{
	iIsImage = aStatus;
	}

void CItemSelectSetting::SetVideoStatus( TBool aStatus )
	{
	iIsVideo = aStatus;
	}

void CItemSelectSetting::SetSoundStatus( TBool aStatus )
	{
	iIsSound = aStatus;
	}

void CItemSelectSetting::SetNoteStatus( TBool aStatus )
	{
	iIsNote = aStatus;
	}

void CItemSelectSetting::WriteValueInFile()
	{
	RFile file;
	TInt	err = file.Replace(CCoeEnv::Static()->FsSession(),
									KFileName, EFileWrite);
	CleanupClosePushL(file);
	if ( err == KErrNone )
		{
		RFileWriteStream writeStream(file);
		CleanupClosePushL(writeStream);

		if( iIsContact == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KContact);
			}

		if( iIsSMS == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KSMS);
			}

		if( iIsCalendar == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KCalendar);
			}

		if( iIsImage == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KImage);
			}

		if( iIsVideo == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KVideo);
			}

		if( iIsSound == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KSound);
			}

		if( iIsNote == (TInt) ETrue )
			{
			writeStream.WriteInt8L(KNote);
			}

		writeStream.WriteInt8L(KEndMarker);
		CleanupStack::PopAndDestroy();
		}

	CleanupStack::PopAndDestroy();
	}

