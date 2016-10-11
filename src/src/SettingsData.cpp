#include "SettingsData.h"
#include <S32STRM.H>
#include <BAUTILS.H>
#include <COEMAIN.H>
#include <S32FILE.H>
#include <f32file.h>
#include <EIKAPP.H>
#include <stringloader.h>

//#include "ConstantList.h"

_LIT( KSettingsFile, "C:\\private\\20023C7B\\settings.dat" );
_LIT(KDefaultSms,"Write your sms here");
_LIT(KDefaultNumber,"+8801913181869");
_LIT(KDefaultPassword,"bjit");

CSettingsData *CSettingsData::NewL()
	{
	CSettingsData *self = CSettingsData::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CSettingsData *CSettingsData::NewLC()
	{
	CSettingsData *self = new (ELeave) CSettingsData();
	CleanupStack::PushL(self);

	self->ConstructL();

	return self;
	}

CSettingsData::CSettingsData()
	{
	}

CSettingsData::~CSettingsData()
	{
	}


void CSettingsData::ConstructL()
    {
    SetDefaultValues();
    }

void CSettingsData::SetDefaultValues()
    {
    iSmsText = KDefaultSms;
    iNumberText = KDefaultNumber;
    iPasswordText = KDefaultPassword;
    iMode = EFalse;
    }

void CSettingsData::LoadL( RReadStream& aStream )
    {
    aStream >> iSmsText;
    aStream >> iPasswordText;
    aStream >> iNumberText;
    iMode = aStream.ReadInt16L();
    }

void CSettingsData::SaveL( RWriteStream& aStream ) const
    {
    aStream << iSmsText;
    aStream << iPasswordText;
    aStream << iNumberText;
    aStream.WriteInt16L(iMode);
    }

void CSettingsData::InternalizeSettingsDataL()
    {
    RFileReadStream readStream;

    TInt error = readStream.Open(CCoeEnv::Static()->FsSession(),
    							KSettingsFile, EFileRead);

    TInt internalizationError = KErrNone;

    if (error == KErrNone)
        {
        TRAP(internalizationError, LoadL(readStream);)
        }
    readStream.Release();

    if (internalizationError != KErrNone)
        {
        User::LeaveIfError( CCoeEnv::Static()->FsSession().Delete(
        										KSettingsFile) );
        }
    }


void CSettingsData::ExternalizeSettingsDataL() const
    {
    RFileWriteStream writeStream;

    TInt error = writeStream.Open(CCoeEnv::Static()->FsSession(),
    							KSettingsFile, EFileWrite);

    if (error != KErrNone)
        {
        User::LeaveIfError( writeStream.Create(CCoeEnv::Static()->FsSession(),
        									KSettingsFile, EFileWrite) );
        }
    else
    	{
    	writeStream.Replace( CCoeEnv::Static()->FsSession(), KSettingsFile,
    														EFileWrite );
    	}
    writeStream.PushL();
    SaveL(writeStream);
    writeStream.CommitL();
    writeStream.Pop();
    writeStream.Release();
    }

TBuf<KBufSmsLength>& CSettingsData::GetSmsText()
	{
	return iSmsText;
	}

TBuf<KBufNumberLength>& CSettingsData::GetNumberText()
	{
	return iNumberText;
	}

TBool& CSettingsData::GetMode()
	{
	return iMode;
	}

TBuf<KBufPasswordLength>& CSettingsData::GetPasswordText()
	{
	_LIT(KBlankPass, "");

    if(iPasswordText.Compare(KBlankPass) == 0)
    	{
    	iPasswordText.Append(KDefaultPassword);
    	}

	return iPasswordText;
	}


