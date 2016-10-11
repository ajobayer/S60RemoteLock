#include <avkon.hrh>

#include <aknviewappui.h>
#include <aknconsts.h>
#include <aknutils.h>
#include <aknquerydialog.h>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <hlplch.h>
#include <stringloader.h>
#include <S60RemoteLock_0x20023C7B.rsg>

#include <apgtask.h>
#include <w32std.h>
#include <APGWGNAM.H>
#include <W32STD.H>

#include "S60RemoteLock_0x20023C7B.hlp.hrh"
#include "S60RemoteLockApplication.h"
#include "PasswordView.h"
#include "PasswordContainer.h"
#include "S60RemoteLock.hrh"
#include "S60RemoteLock.pan"
#include "SettingsData.h"

#include "ActionMaker.h"
//#include "SIMDetector.h"

#include "Contact.h"
#include "SMS.h"
#include "Calendar.h"
#include "Image.h"
#include "Video.h"
#include "Sound.h"
#include "Notes.h"

CPasswordView::CPasswordView()
    {
    }

CPasswordView::~CPasswordView()
	{
	if(iSettingsData)
		{
		delete iSettingsData;
		iSettingsData = NULL;
		}
	}

CPasswordView* CPasswordView::NewL()
    {
    CPasswordView* self = CPasswordView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CPasswordView* CPasswordView::NewLC()
    {
    CPasswordView* self = new ( ELeave ) CPasswordView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CPasswordView::ConstructL()
    {
    BaseConstructL( R_S60REMOTELOCK_PASSWORD_VIEW );
    iSettingsData = CSettingsData::NewL();
    iCount = 0;
    iHide = EFalse;
    }

TUid CPasswordView::Id() const
    {
    return TUid::Uid( EPasswordViewId );
    }

void CPasswordView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
    iContainer = CPasswordContainer::NewL( ClientRect() );
    }

void CPasswordView::DoDeactivate()
    {

    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

void CPasswordView::HandleCommandL( TInt aCommand )
    {
	switch( aCommand )
		{
		//CONVETION CHANGE
		/******************************** Test Menu **************************/
		case ETestContact:
			{
			//CSIMDetector* simdetectionTest = CSIMDetector::NewL();
			//simdetectionTest->DetectSIMChange();
			CContact* testRemoveContact = CContact::NewL();
			testRemoveContact->RemoveL();
			testRemoveContact->Destructor();
			}
		break;
		case ETestSms:
			{
			CSms* testRemoveCSMS = CSms::NewL();
			testRemoveCSMS->RemoveL();
			testRemoveCSMS->Destructor();
			}
		break;
		case ETestCalender:
			{
			CCalendar* testRemoveCalendar = CCalendar::NewL();
			testRemoveCalendar->RemoveL();
			testRemoveCalendar->Destructor();
			}
		break;
		case ETestImage:
			{
			CImage* testRemoveCImage = CImage::NewL();
			testRemoveCImage->RemoveL();
			testRemoveCImage->Destructor();
			}
		break;
		case ETestVideo:
			{
			CVideo* testRemoveCVideo = CVideo::NewL();
			testRemoveCVideo->RemoveL();
			testRemoveCVideo->Destructor();
			}
		break;
		case ETestSound:
			{
			CSound* testRemoveCSound = CSound::NewL();
			testRemoveCSound->RemoveL();
			testRemoveCSound->Destructor();
			}
		break;
		case ETestNote:
			{
			CNotes* testRemoveCNote = CNotes::NewL();
			testRemoveCNote->RemoveL();
			testRemoveCNote->Destructor();
			}
		break;
		/**********************************************************************/		
		case EAknSoftkeyCancel:
		case EBacground:
			{
			AppUi()->HandleCommandL( aCommand );
			}
			break;
		case EStart:
			{
			ShowPasswordDialogL();
			}
			break;
		case EHelp:
			{
			CArrayFix< TCoeHelpContext >* buf = CPasswordView::HelpContextL();
			HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(), buf );
			}
			break;
		case EAbout:
			{
			CAknMessageQueryDialog* dlg = 
									new (ELeave) CAknMessageQueryDialog();
			dlg->PrepareLC( R_ABOUT_QUERY_DIALOG );
			HBufC* title = iEikonEnv->AllocReadResourceLC( 
													R_ABOUT_DIALOG_TITLE );
			dlg->QueryHeading()->SetTextL( *title );
			CleanupStack::PopAndDestroy();
			HBufC* msg = iEikonEnv->AllocReadResourceLC( R_ABOUT_DIALOG_TEXT );
			dlg->SetMessageTextL( *msg );
			CleanupStack::PopAndDestroy();
			dlg->RunLD();
			}
			break;
		default:
			Panic( ES60RemoteLockUi );
			break;
		}
    }

void CPasswordView::HandleSizeChange( TInt aType )
    {
    if( iContainer )
        {
        iContainer->HandleResourceChange( aType );

        if ( aType==KEikDynamicLayoutVariantSwitch )
            {
            TRect rect;
            AknLayoutUtils::LayoutMetricsRect( 
            							AknLayoutUtils::EMainPane, rect );
            iContainer->SetRect( rect );
            }
        }
    }

TBool CPasswordView::ShowPasswordDialogL()
	{
    TBuf<KMaxPasswordLength> password;
    TBuf<KMaxPasswordLength> savedPassword;
    
    CAknTextQueryDialog* dlg = new(ELeave) CAknTextQueryDialog(
    					password, CAknQueryDialog::ENoTone );

    if( !dlg->ExecuteLD(R_S60REMOTELOCK_DIALOG_PASSWORD_QUERY) )
        {
        return EFalse;
        }
    else
        {
        iSettingsData->InternalizeSettingsDataL();	    
	    savedPassword = iSettingsData->GetPasswordText();
   
        if( password.Compare(savedPassword) == 0 )
        	{
        	AppUi()->ActivateLocalViewL( TUid::Uid(EConfigureViewId) );
        	iCount = 0;
        	}
        else
        	{
        	iCount++;
        	if( iCount == 3 )
        		{
        		ShowErrorMessage( R_INVALID_PASSWORD_TEXT );
        		iCount = 0;
        		}
        	else
        		{
        		ShowPasswordDialogL();
        		}
        	}
        return ETrue;
        }
	}

void CPasswordView::ShowErrorMessage( TInt aResourceId )
	{
	HBufC* textResource = StringLoader::LoadLC( aResourceId );
	CAknInformationNote* informationNote;

	informationNote = new ( ELeave ) CAknInformationNote;
	informationNote->ExecuteLD( *textResource );
	CleanupStack::PopAndDestroy( textResource );	
	}

CArrayFix<TCoeHelpContext>* CPasswordView::HelpContextL() const
	{
	//CONVETION CHANGE
	//#warning "Please see comment about help and UID3..."
	// Note: help will not work if the application uid3 is not in the
	// protected range.  The default uid3 range for projects created
	// from this template (0xE0000000 - 0xEFFFFFFF) are not in the protected range so they
	// can be self signed and installed on the device during testing.
	// Once you get your official uid3 from Symbian Ltd. and find/replace
	// all occurrences of uid3 in your project, the context help will
	// work.
	CArrayFixFlat<TCoeHelpContext>* array = 
							new (ELeave) CArrayFixFlat<TCoeHelpContext>( 1 );
	CleanupStack::PushL( array );
	array->AppendL( TCoeHelpContext(KUidS60RemoteLockApp, KSetPassword) );
	CleanupStack::Pop( array );
	return array;
	}

void CPasswordView::HandleForegroundEventL(TBool aForeground)
	{
	 // Call Base class method
	// CAknAppUi::HandleForegroundEventL(aForeground);

	if(aForeground)
		{
	    // We have gained the focus
		//...
		if(iHide == (TInt)EFalse)
			{
			iHide = ETrue;
			TInt wgId = iEikonEnv->RootWin().Identifier();
			RWsSession session = iEikonEnv->WsSession();
			CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC( session, wgId );
			wgName->SetHidden( ETrue );
			wgName->SetWindowGroupName( iEikonEnv->RootWin() );
			CleanupStack::PopAndDestroy();
			
			TApaTask appTask( iEikonEnv->WsSession() );
			appTask.SetWgId( CEikonEnv::Static()->RootWin().Identifier() );
			appTask.SendToBackground();			
			}
		else
			{
			TInt wgId = iEikonEnv->RootWin().Identifier();
			RWsSession session = iEikonEnv->WsSession();
			CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC( session, wgId );
			wgName->SetHidden( EFalse );
			wgName->SetWindowGroupName( iEikonEnv->RootWin() );
			CleanupStack::PopAndDestroy();			
			}
		}
	else
		{
	    // We have lost the focus
		//...
		}
	}

//void CPasswordView::DetectIMSI()
//	{	
//	CSIMDetector* simdetector = CSIMDetector::NewL();
//	simdetector->DetectSIMChange();
//	if(simdetector)
//		{
//		delete simdetector;
//		simdetector = NULL;
//		}
//	}
