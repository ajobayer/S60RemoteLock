#include <aknviewappui.h>
#include <aknconsts.h>
#include <aknnotewrappers.h>
#include <S60RemoteLock_0x20023C7B.rsg>
#include <stringloader.h>
#include <barsread.h>

#include <apgtask.h>
#include <w32std.h>
#include <APGWGNAM.H>
#include <W32STD.H>

#include <hlplch.h>
#include "S60RemoteLock_0x20023C7B.hlp.hrh"
#include "S60RemoteLockApplication.h"
#include "ConfigurationView.h"
#include "ConfigurationContainer.h"
#include "S60RemoteLock.hrh"
#include "S60RemoteLock.pan"
#include "SettingsData.h"
#include "ItemSelectSetting.h"

#include "ConstantList.h"

/*const TInt KMinSmsLength = 10;
const TInt KMaxSmsLength = 50;
const TInt KMaxPasswordLength = 8;
const TInt KMinPasswordLength = 2;
const TInt KMaxNumberLength = 20;*/

CConfigurationView::CConfigurationView()
: CAknView()
	{
	}

CConfigurationView::~CConfigurationView()
	{
	if(iSettingsData)
		{
		delete iSettingsData;
		iSettingsData = NULL;
		}
	}

CConfigurationView* CConfigurationView::NewLC()
	{
	CConfigurationView* self = new (ELeave) CConfigurationView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CConfigurationView* CConfigurationView::NewL()
	{
	CConfigurationView* self = CConfigurationView::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

void CConfigurationView::ConstructL()
	{
	BaseConstructL(R_S60REMOTELOCK_CONFIGURE_VIEW);
	iSettingsData = CSettingsData::NewL();
	iSettingsData->InternalizeSettingsDataL();
	}

void CConfigurationView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
									TUid /*aCustomMessageId*/,
									const TDesC8& /*aCustomMessage*/ )
	{
	if( !iSettingList )
		{
		iSettingList = CConfigurationContainer::NewL( iSettingsData );
		iSettingList->SetMopParent( this );

		TResourceReader reader;
		CEikonEnv::Static()->CreateResourceReaderLC( reader,
								R_S60REMOTELOCK_SETTINGS );
		iSettingList->ConstructFromResourceL( reader );

		CleanupStack::PopAndDestroy();

		AppUi()->AddToViewStackL( *this, iSettingList );
		iSettingList->ActivateL();
		}
	}

void CConfigurationView::DoDeactivate()
	{
	if( iSettingList )
		{
		AppUi()->RemoveFromStack( iSettingList );
		delete iSettingList;
		iSettingList = NULL;
		}
	}

TUid CConfigurationView::Id() const
	{
	return TUid::Uid( EConfigureViewId );
	}

void CConfigurationView::HandleSizeChange( TInt aType )
    {
    if( iSettingList )
        {
        iSettingList->HandleResourceChange( aType );

        if ( aType==KEikDynamicLayoutVariantSwitch )
            {
            TRect rect;
            AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
            iSettingList->SetRect(rect);
            }
        }
    }

void CConfigurationView::HandleCommandL( TInt aCommand )
    {
	switch( aCommand )
		{
		case EEikCmdExit:
			{
			iSettingsData->InternalizeSettingsDataL();
			AppUi()->HandleCommandL( aCommand );
			}
			break;
		case EAknSoftkeyDone:
			{
			if( !IsValidSmsTextLength() )
				{
				ShowErrorMessage( R_INVALID_SMS_LENGTH );
				}
			else if( !IsValidPhoneNumber() )
				{
				ShowErrorMessage( R_INVALID_MOBILE_NUMBER );
				}
			else if( !IsValidPasswordLength() )
				{
				ShowErrorMessage( R_INVALID_PASSWORD_LENGTH );
				}
			else
				{
				iSettingsData->ExternalizeSettingsDataL();
				iSettingsData->InternalizeSettingsDataL();
		       	AppUi()->ActivateLocalViewL( TUid::Uid(EPasswordViewId) );
				}
			}
			break;
		case ESelectItems:
			{
			ShowSelectionListQueryDialog();
			}
			break;
		case EHelp:
			{
			CArrayFix<TCoeHelpContext>* buf =
										CConfigurationView::HelpContextL();
			HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(), buf );
			}
			break;
		case EBack:
			iSettingsData->InternalizeSettingsDataL();
			AppUi()->ActivateLocalViewL( TUid::Uid(EPasswordViewId) );
			break;
		default:
			Panic( ES60RemoteLockUi );
			break;
		}
    }

void CConfigurationView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	iSettingList->SetRect(ClientRect());
	}

void CConfigurationView::ShowSelectionListQueryDialog()
	{

	CItemSelectSetting* itemSelectSetting = CItemSelectSetting::NewL();
	itemSelectSetting->ReadValueFromFile();

	CArrayFixFlat<TInt>* indexArray = new(ELeave)CArrayFixFlat<TInt>( 7 );
	CleanupStack::PushL( indexArray );

	CAknListQueryDialog* dlg = new (ELeave) CAknListQueryDialog( indexArray );
	dlg->PrepareLC( R_ITEM_SELECTION_LIST_QUERY_DIALOG );

	CListBoxView::CSelectionIndexArray* selArray  =
		const_cast< CListBoxView::CSelectionIndexArray* >( dlg->ListBox()->
													SelectionIndexes() );

	if( itemSelectSetting->GetContactStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KContact );
		}

	if( itemSelectSetting->GetSMSStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KSMS );
		}

	if( itemSelectSetting->GetCalendarStatus() == (TInt) ETrue )
		{
		selArray->AppendL(KCalendar);
		}

	if( itemSelectSetting->GetImageStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KImage );
		}

	if( itemSelectSetting->GetVideoStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KVideo );
		}

	if( itemSelectSetting->GetSoundStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KSound );
		}

	if( itemSelectSetting->GetNoteStatus() == (TInt) ETrue )
		{
		selArray->AppendL( KNote );
		}

	itemSelectSetting->ResetStatus();
	if( dlg->RunLD() )
		{
		for( TInt i = 0; i < indexArray->Count(); i++ )
			{
			TInt selectedInxd = ( *indexArray )[ i ];
			if( selectedInxd == KContact )
				{
				itemSelectSetting->SetContactStatus( ETrue );
				}
			else if( selectedInxd == KSMS )
				{
				itemSelectSetting->SetSMSStatus( ETrue );
				}
			else if( selectedInxd == KCalendar )
				{
				itemSelectSetting->SetCalendarStatus( ETrue );
				}
			else if( selectedInxd == KImage )
				{
				itemSelectSetting->SetImageStatus( ETrue );
				}
			else if( selectedInxd == KVideo )
				{
				itemSelectSetting->SetVideoStatus( ETrue );
				}
			else if( selectedInxd == KSound )
				{
				itemSelectSetting->SetSoundStatus( ETrue );
				}
			else if( selectedInxd == KNote )
				{
				itemSelectSetting->SetNoteStatus( ETrue );
				}
			}

		itemSelectSetting->WriteValueInFile();
		}

		if( itemSelectSetting )
		{
		delete itemSelectSetting;
		itemSelectSetting = NULL;
		}
	}

void CConfigurationView::HandleForegroundEventL(TBool aForeground)
	{
	 // Call Base class method
	// CAknAppUi::HandleForegroundEventL(aForeground);

	if(aForeground)
		{
	    // We have gained the focus
		//...
		TInt wgId = iEikonEnv->RootWin().Identifier();
		RWsSession session = iEikonEnv->WsSession();
		CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC( session, wgId );
		wgName->SetHidden( EFalse );
		wgName->SetWindowGroupName( iEikonEnv->RootWin() );
		CleanupStack::PopAndDestroy();
		}
	else
		{
	    // We have lost the focus
		//...
		}
	}

TBool CConfigurationView::IsValidSmsTextLength()
	{
	TBuf< KMaxSmsLength > inputSms;

	inputSms = iSettingsData->GetSmsText();

	if( inputSms.Length() >= KMinSmsLength )
		{
		return ETrue;
		}

	return EFalse;
	}

TBool CConfigurationView::IsValidPhoneNumber()
	{
	_LIT( KMultiPlusSign, "+" );
	TBuf< KMaxNumberLength > inputNumber;
	TBuf< KMaxNumberLength > tempNumber;
	TInt count(1);

	inputNumber = iSettingsData->GetNumberText();

	while( count < (inputNumber.Length()) )
		   {
		   tempNumber.Append(inputNumber[count]);
		   count++;
		   }

	if( tempNumber.Find(KMultiPlusSign) == KErrNotFound )
	   {
	   return ETrue;
	   }

	return EFalse;
	}

TBool CConfigurationView::IsValidPasswordLength()
	{
	TBuf< KMaxPasswordLength > inputPassword;
	inputPassword = iSettingsData->GetPasswordText();

	if( inputPassword.Length() >= KMinPasswordLength )
		{
		return ETrue;
		}

	return EFalse;
	}

void CConfigurationView::ShowErrorMessage( TInt aResourceId )
	{
	HBufC* textResource = StringLoader::LoadLC( aResourceId );
	CAknInformationNote* informationNote;

	informationNote = new ( ELeave ) CAknInformationNote;
	informationNote->ExecuteLD( *textResource );
	CleanupStack::PopAndDestroy( textResource );
	}

CArrayFix<TCoeHelpContext>* CConfigurationView::HelpContextL() const
	{
	CArrayFixFlat<TCoeHelpContext>* array =
						new (ELeave) CArrayFixFlat< TCoeHelpContext >( 1 );
	CleanupStack::PushL( array );
	array->AppendL( TCoeHelpContext(KUidS60RemoteLockApp, KConfigure) );
	CleanupStack::Pop( array );
	return array;
	}

