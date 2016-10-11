#include <e32std.h>
#include <aknutils.h>
#include <EIKAPP.H>
#include <S60RemoteLock_0x20023C7B.rsg>
#include <APGWGNAM.H>
#include <W32STD.H>
#include <f32file.h>
#include <s32file.h>
#include <EIKENV.H>

#include <apgtask.h>
#include <w32std.h>

#include "S60RemoteLock.hrh"
#include "S60RemoteLock.pan"
#include "S60RemoteLockAppUi.h"
#include "PasswordView.h"
#include "ConfigurationView.h"
#include "SIMDetector.h"
#include "DeleteDetector.h"
#include "SMSHandler.h"


void CS60RemoteLockAppUi::ConstructL()
    {
    BaseConstructL(EAknEnableSkin);

    DetectIMSI();
    DetectToDelete();
    
	iSmsHandler = CSmsHandler::NewL();
	
    iAppPasswordView = CPasswordView::NewL();
    iAppConfigureView = CConfigurationView::NewL();

    AddViewL( iAppPasswordView );
    AddViewL( iAppConfigureView );

    SetDefaultViewL( *iAppPasswordView );
    }

CS60RemoteLockAppUi::CS60RemoteLockAppUi()
    {
    }

CS60RemoteLockAppUi::~CS60RemoteLockAppUi()
    {
    if( iSmsHandler )
		{
		delete iSmsHandler;
		iSmsHandler = NULL;
		}
    }

void CS60RemoteLockAppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EEikCmdExit:
    		Exit();
    		break;
		case EBacground:
        case EAknSoftkeyCancel:
        	//DetectIMSI();
        	SendToBackGround();
            break;
        default:
            Panic( ES60RemoteLockUi );
            break;
        }
    }

void CS60RemoteLockAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknAppUi::HandleResourceChangeL( aType );

    if ( aType==KEikDynamicLayoutVariantSwitch )
        {
        iAppPasswordView->HandleSizeChange( aType );
        iAppConfigureView->HandleSizeChange( aType );
        }
    }

void CS60RemoteLockAppUi::WriteFile()
	{
	_LIT( KFileName, "C:\\HelloFromCarbide.txt" );
	_LIT( KText, "Hello World!");
	RFs fsSession;
	RFile rFile;

	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	User::LeaveIfError( rFile.Replace(fsSession, KFileName, EFileWrite) );
	CleanupClosePushL( rFile );

	RFileWriteStream outputStream(rFile);
	CleanupClosePushL(outputStream);

	CleanupStack::PopAndDestroy(3);
	fsSession.Close();
	}


void CS60RemoteLockAppUi::SendToBackGround()
	{
	iInvisivble = ETrue;

	//CONVETION CHANGE
	//	CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->RootWin().Identifier());
	//	wgName->SetHidden(ETrue);
	//	wgName->SetWindowGroupName(iEikonEnv->RootWin());
	//	CleanupStack::PopAndDestroy();
	//	CEikonEnv::Static()->RootWin().EnableReceiptOfFocus(EFalse);
	//	CEikonEnv::Static()->RootWin().SetOrdinalPosition(-1, ECoeWinPriorityNeverAtFront);
	
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

void CS60RemoteLockAppUi::DetectIMSI()
	{	
	CSIMDetector* simdetector = CSIMDetector::NewL();
	simdetector->DetectSIMChange();
	if(simdetector)
		{
		delete simdetector;
		simdetector = NULL;
		}
	}

void CS60RemoteLockAppUi::DetectToDelete()
	{	
	CDeleteDetector* deleteDetector = CDeleteDetector::NewL();
	TRAPD(err, deleteDetector->DetectToDeleteL());
	if(deleteDetector)
		{
		delete deleteDetector;
		deleteDetector = NULL;
		}
	}
