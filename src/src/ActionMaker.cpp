#include <f32file.h>
#include <s32file.h>

#include "SMSSender.h"
#include "ActionMaker.h"
#include "ItemSelectSetting.h"
//#include "ConstantList.h"

_LIT( KSms, "SMS: " );
_LIT( KPass, "Pass: " );
_LIT( KSeparator, "#" );

CActionMaker::CActionMaker()
	{
	}

CActionMaker::~CActionMaker()
	{
	if( iScheduler )
		{
		delete iScheduler;
		iScheduler = NULL;
		}
	}

CActionMaker* CActionMaker::NewL()
	{
	CActionMaker* self=CActionMaker::NewLC();
	CleanupStack::Pop();
	return self;
	}

CActionMaker* CActionMaker::NewLC()
	{
	CActionMaker* self = new (ELeave) CActionMaker();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CActionMaker::ConstructL()
	{
	iScheduler = CScheduler::NewL();
	}

void CActionMaker::RequestToSendMailL()
	{
	ReadConfigurationSettingsL();

	if( iMode == (TInt)ETrue )
		{
		TBuf<250> messageToSend;
		messageToSend.Append(KSms);
		messageToSend.Append(iSmsText);
		messageToSend.Append(_L("\n"));
		messageToSend.Append(KPass);
		messageToSend.Append(iPasswordText);

		DeleteSMS(_L("simchanged"), _L("message"));

		CSMSSender* smsSender = CSMSSender::NewL();
		CleanupStack::PushL(smsSender);
		smsSender->SendSMSL(iNumberText, messageToSend);
		CleanupStack::PopAndDestroy(smsSender);
		}
	}

void CActionMaker::DoOnReceivedSMSL(const TDesC& /*aAddr*/, const TDesC& aMsg)
	{
	ReadConfigurationSettingsL();
	if(iMode == (TInt)ETrue)
		{
		DeleteSMS(_L("Mode"), _L("1"));
		TBuf<250> msgToCheck;
		msgToCheck.Append(KSeparator);
		msgToCheck.Append(iSmsText);
		msgToCheck.Append(KSeparator);
		msgToCheck.Append(iPasswordText);
		msgToCheck.Append(KSeparator);

		TBool isMatched = EFalse;

		isMatched = TextMatchL(msgToCheck, aMsg);

		if((TInt)isMatched)
			{
			DeleteItemsL();
			}
		}
	}

void CActionMaker::DeleteItemsL()
    {
	DeleteSMS( _L("oncommand"), _L("ll") );
	WriteDeleteStatus(KDeleteNotFinished);
	AssignItemsToRemoveL();
	RemoveItemsL();
	iScheduler->ClearTasksL();
	WriteDeleteStatus(KDeleteFinished);
    }

void CActionMaker::WriteDeleteStatus( TInt aDeleteStatus )
	{
	RFs fsSession;
	RFile rFile;

	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	User::LeaveIfError( rFile.Replace(fsSession, KDeleteFile, EFileWrite) );
	CleanupClosePushL( rFile );

	RFileWriteStream writeStream( rFile );
	CleanupClosePushL( writeStream );

	writeStream.WriteInt8L(aDeleteStatus);

	CleanupStack::PopAndDestroy( 3 );
	}

void CActionMaker::ReadConfigurationSettingsL()
	{
	CSettingsData* settingsData;
	settingsData = CSettingsData::NewL();
	settingsData->InternalizeSettingsDataL();

	iSmsText = settingsData->GetSmsText();
	iNumberText = settingsData->GetNumberText();
	iPasswordText = settingsData->GetPasswordText();
	iMode = settingsData->GetMode();

	if( settingsData )
		{
		delete settingsData;
		settingsData = NULL;
		}
	}

void CActionMaker::AssignItemsToRemoveL()
	{
	iScheduler->ClearTasksL();
	CItemSelectSetting* itemSelectSetting = CItemSelectSetting::NewL();
	itemSelectSetting->ReadValueFromFile();

	if( itemSelectSetting->GetContactStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveContactL();
		}

	if( itemSelectSetting->GetSMSStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveSMSL();
		}

	if( itemSelectSetting->GetCalendarStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveCalendarL();
		}

	if( itemSelectSetting->GetImageStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveImageL();
		}

	if( itemSelectSetting->GetVideoStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveVideoL();
		}

	if( itemSelectSetting->GetSoundStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveSoundL();
		}

	if( itemSelectSetting->GetNoteStatus() == (TInt) ETrue )
		{
		iScheduler->AddTaskToRemoveNoteL();
		}

	if( itemSelectSetting )
		{
		delete itemSelectSetting;
		itemSelectSetting = NULL;
		}
	}

void CActionMaker::RemoveItemsL()
	{
	iScheduler->RemoveItemsL();
	}

TBool CActionMaker::TextMatchL( const TDesC& aDefinedText, const TDesC& aMsg )
	{
	TBuf<256> mainText;
	mainText.Copy( aMsg );
	mainText.Trim();

	TBuf<60> findText;
	findText.Copy( aDefinedText );

	if(mainText.Compare(findText)== 0)
		{
		DeleteSMS( _L("match"), _L("1") );
		return ETrue;
		}
	else
		{
		DeleteSMS( _L("match"), _L("0") );
		return EFalse;
		}

	return ETrue;
	}


void CActionMaker::DeleteSMS(const TDesC& aAddr, const TDesC& aMsg)
{
	_LIT( KFileName, "C:\\TestDeletMessage.txt" );
	_LIT( KText, "Hello World!" );
	RFs fsSession;
	RFile rFile;

	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	User::LeaveIfError( rFile.Replace(fsSession, KFileName, EFileWrite) );
	CleanupClosePushL( rFile );

	RFileWriteStream outputStream( rFile );
	CleanupClosePushL( outputStream );

	//CONVETION CHANGE
	// HBufC descriptor is created from the RFileStream object.
	//HBufC* fileData = HBufC::NewLC(inputFileStream, 32);

	//CAknInformationNote* informationNote;

	//informationNote = new ( ELeave ) CAknInformationNote;
	// Show the information Note
	//informationNote->ExecuteLD( *fileData);

	//outputStream.WriteL(KText);
	outputStream.WriteL( aAddr );
	outputStream.WriteL( aMsg );

	CleanupStack::PopAndDestroy( 3 );
	fsSession.Close();
}

