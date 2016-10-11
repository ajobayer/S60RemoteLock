//CONVETION CHANGE [top to bottom]
// INCLUDE FILES
#include <eikenv.h>
#include <coemain.h>
#include <e32std.h>
#include <msvids.h>
#include <msvstd.h>
#include <smsclnt.h>
#include <smut.h>
#include <mtclreg.h>
#include <txtrich.h>
#include <smscmds.h>
#include <mtmuibas.h>
#include <mtmdef.h>
#include <stringloader.h>
#include <TXTRICH.H>

#include "SmsHandler.h"
#include "smutset.h"
#include "smuthdr.h"

const TMsvId KInbox = KMsvGlobalInBoxIndexEntryId;
#ifdef __WINS__
const TMsvId KObservedFolderId = KMsvDraftEntryId;
#else
const TMsvId KObservedFolderId =  KMsvGlobalInBoxIndexEntryId;
#endif


CSmsHandler::CSmsHandler()
: CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    iNextUnread = 0;            // index of next unread message in iSelection
    }

void CSmsHandler::ConstructL()
    {
 // Session to message server is opened synchronously.
    iSession = CMsvSession::OpenSyncL( *this );

    iMtmRegistry = CClientMtmRegistry::NewL(*iSession);
    //iMtm = iMtmRegistry->NewMtmL(KUidMsgTypeSMS);
    iSmsMtm = STATIC_CAST(CSmsClientMtm*,iMtmRegistry->NewMtmL(KUidMsgTypeSMS));
    
    // Entry selection for all received messages.
    iSelection = new ( ELeave ) CMsvEntrySelection();

    }

CSmsHandler* CSmsHandler::NewL( )
    {
    CSmsHandler* self = NewLC( );
    CleanupStack::Pop( self );
    return self;
    }

CSmsHandler* CSmsHandler::NewLC()
    {
    CSmsHandler* self = new ( ELeave ) CSmsHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSmsHandler::~CSmsHandler()
    {
    Cancel();           // cancel any outstanding request

    if(iOperation)
    	{    	    	
    	delete iOperation;
    	}
    if(iMtmUiRegistry)
    	{
    	delete iMtmUiRegistry;
    	}
    if(iSelection)
    	{
    	delete iSelection;
    	}
    if(iSmsMtm)
    	{
    	delete iSmsMtm;
    	}
    if(iMtmRegistry)
    	{
    	delete iMtmRegistry;
    	}
    if(iSession)
    	{
    	delete iSession;    // session must be deleted last
    	}
    if(iMsvEntry)
    	{
    	 delete iMsvEntry;
    	}
    }

void CSmsHandler::DoCancel()
    {
    if ( iOperation )
        {
        iOperation->Cancel();
        }
    }

void CSmsHandler::RunL()
    {
    User::LeaveIfError( iStatus != KErrNone );
  
    TBufC8<KMsvProgressBufferLength> progress( iOperation->ProgressL() );
    _LIT8( KCompare, "KErrNone" );
    User::LeaveIfError( !progress.Compare( KCompare ) );

    delete iOperation;
    iOperation = NULL;
    
    switch ( iState )
        {
        case EWaitingForMoving:
            ScheduleL();
            break;

        case EWaitingForScheduling:
            {
            TMsvEntry entry( iSmsMtm->Entry().Entry() );
            TInt state( entry.SendingState() );

            if ( state == KMsvSendStateWaiting || state == KMsvSendStateScheduled)
                {
                }

            break;
            }

        default:
            break;
        }
    }

void CSmsHandler::HandleSessionEventL( TMsvSessionEvent aEvent,
                                      TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
	{
	switch ( aEvent )
		{
		case EMsvServerReady:
            {
            if (!iMsvEntry)
                {
                iMsvEntry = CMsvEntry::NewL(*iSession, KInbox,
                                            TMsvSelectionOrdering());
                }            
            
            break;
            }

        case EMsvEntriesCreated:
        	{

        if (aArg2 &&  *(static_cast<TMsvId*>(aArg2)) == KObservedFolderId)
            {
            CMsvEntrySelection* entries =
                                static_cast<CMsvEntrySelection*>(aArg1);
            if( entries->Count() >= 1 )
                {
                iNewMessageId = entries->At(0);
                }
            else
                {
                	//return;
                }
            }
        break;
		}
        //case EMsvEntriesCreated:
        case EMsvEntriesChanged:
            {    
            if( !iMsvEntry )
            	{
            	iMsvEntry = CMsvEntry::NewL(*iSession, KInbox,
            								TMsvSelectionOrdering());
            	}
            iListeningForIncoming = ETrue;
            if (aArg2 &&  *(static_cast<TMsvId*>(aArg2)) == KObservedFolderId
                      && iListeningForIncoming )
                {
                
                CMsvEntrySelection* entries =
                                    static_cast<CMsvEntrySelection*>(aArg1);

                if( entries->Count() < 1 )
                {
                break;
                }
                else if (iNewMessageId == entries->At(0))
                    {                    

                    if( !iMsvEntry )
                        {                        
                        return;
                        }
                    iMsvEntry->SetEntryL(iNewMessageId);

                    if ( iMsvEntry->Entry().iMtm != KUidMsgTypeSMS ||
                         !iMsvEntry->Entry().Complete() )
                        {
                        return;
                        }                    
                    CMsvStore* store = iMsvEntry->ReadStoreL();
                    CleanupStack::PushL(store);

                    TBuf<KSmsMessageLength> iAddress(
                                                   iMsvEntry->Entry().iDetails
                                                   );               
                    if (store->HasBodyTextL())
                        {
                        CRichText* richText = CRichText::NewL(
                           CEikonEnv::Static()->SystemParaFormatLayerL(),
                           CEikonEnv::Static()->SystemCharFormatLayerL());
                        CleanupStack::PushL(richText);
                        store->RestoreBodyTextL(*richText);
                        const TInt length = richText->DocumentLength();

                        TBuf<KSmsMessageLength> number;
                        TPtrC ptr = richText->Read(0, length);
                        iMessage.Copy(ptr);
                        CleanupStack::PopAndDestroy(richText);
                        DoOnReceivedSMS(iAddress, iMessage);
                        }
                    else
                        {
                        DoOnReceivedSMS(iAddress, iMessage);                       
                        }

                    CleanupStack::PopAndDestroy(store);
                    }
                }
            break;
            }
        case EMsvCloseSession:
        case EMsvServerTerminated:
        case EMsvGeneralError:
        case EMsvServerFailedToStart:
            {
//            iSmsAppUi->ServerDown( aEvent );    // close application
            break;
            }

        // All other events are ignored.
        default:
            break;
        }
    }

void CSmsHandler::AccessMtmL()
	{
    // Create an MTM Registry object.
    iMtmRegistry = CClientMtmRegistry::NewL( *iSession );

    // Create an SMS Client MTM object.
    iSmsMtm = STATIC_CAST( CSmsClientMtm*, iMtmRegistry->NewMtmL( KUidMsgTypeSMS ) );
    }

TBool CSmsHandler::SendL( const TDesC& aRecipientNumber,
                            const TDesC& aMessageText )
    {
    iRecipientNumber = aRecipientNumber;
    iMessageText = aMessageText;

    if ( CreateMsgL() )
        {
        return ETrue;
        }

    return EFalse;
    }

TBool CSmsHandler::CreateMsgL()
    {
    // Current entry is the Draft folder.
    iSmsMtm->SwitchCurrentEntryL( KMsvDraftEntryId );

    // Create a new SMS message entry as a child of the current context.
    iSmsMtm->CreateMessageL( KUidMsgTypeSMS.iUid );

    CMsvEntry& serverEntry = iSmsMtm->Entry();
    TMsvEntry entry( serverEntry.Entry() );

    CRichText& body = iSmsMtm->Body();   // the body of the message
    body.Reset();
    // Insert the message text gotten as input from user.
    body.InsertL( 0, iMessageText );

    // Message will be sent immediately.
    entry.SetSendingState( KMsvSendStateWaiting );

//Added by Kiran:for 3rd edition
//    entry.iDate.HomeTime(); // insert current time //This was causing problem:SMS stays into Outbox only.
	entry.iDate.UniversalTime(); // insert current time //Solution for HomeTime()
//Code Ends-Kiran
    // Set the SMS message settings for the message.
    CSmsHeader& header = iSmsMtm->SmsHeader();
    CSmsSettings* settings = CSmsSettings::NewL();
    CleanupStack::PushL( settings );

    settings->CopyL( iSmsMtm->ServiceSettings() );    // restore settings
    settings->SetDelivery( ESmsDeliveryImmediately ); // to be delivered immediately
	settings->SetDeliveryReport(ETrue);
    header.SetSmsSettingsL( *settings );              // new settings

	// Let's check if there is a service center address.
	if ( header.Message().ServiceCenterAddress().Length() == 0 )
		{
		// No, there isn't. We assume there is at least one service center
        // number set and use the default service center number.
		CSmsSettings* serviceSettings = &( iSmsMtm->ServiceSettings() );

        // Check if number of service center addresses in the list is null.

//Changed for 3rd Edition specially- Kiran - 30-10-06
//        if ( !serviceSettings->NumSCAddresses() )
		if ( !serviceSettings->ServiceCenterCount() )
            {
			return EFalse;     // quit creating the message
            }

		else
			{
//Changed for 3rd Edition specially- Kiran - 30-10-06
//          CSmsNumber* smsCenter = &( serviceSettings->SCAddress( serviceSettings->DefaultSC() ) );
				CSmsNumber* smsCenter= CSmsNumber::NewL();
				CleanupStack::PushL(smsCenter);
				smsCenter->SetAddressL((serviceSettings->GetServiceCenter( serviceSettings->DefaultServiceCenter())).Address());
				header.Message().SetServiceCenterAddressL( smsCenter->Address() );
				CleanupStack::PopAndDestroy(smsCenter);
		    }
        }

	CleanupStack::PopAndDestroy( settings );

    // Recipient number is displayed also as the recipient alias.
    entry.iDetails.Set( iRecipientNumber );
    // Add addressee.
    iSmsMtm->AddAddresseeL( iRecipientNumber, entry.iDetails );

    // Validate message.
    if ( !ValidateL() )
        {
        return EFalse;
        }

    entry.SetVisible( ETrue );          // set message as visible
    entry.SetInPreparation( EFalse );   // set together with the visibility flag
    serverEntry.ChangeL( entry );       // commit changes		
    iSmsMtm->SaveMessageL();            // save message

    TMsvSelectionOrdering selection;
	CMsvEntry* parentEntry = CMsvEntry::NewL( iSmsMtm->Session(), KMsvDraftEntryId, selection );
    CleanupStack::PushL( parentEntry );

    // Move message to Outbox.
    iOperation =parentEntry->MoveL( entry.Id(), KMsvGlobalOutBoxIndexEntryId, iStatus );

    CleanupStack::PopAndDestroy( parentEntry );

    iState = EWaitingForMoving;
    SetActive();

    return ETrue;
  }

// -----------------------------------------------------------------------------
// CSmsHandler::ValidateL()
// Validate an SMS message.
// -----------------------------------------------------------------------------
//
TBool CSmsHandler::ValidateL()
    {
    // Empty part list to hold the result.
    TMsvPartList result( KMsvMessagePartNone );

    // Validate message body.
    result = iSmsMtm->ValidateMessage( KMsvMessagePartBody );

    if ( result != KMsvMessagePartNone )
        {
        return EFalse;
        }

    // Validate recipient.
    result = iSmsMtm->ValidateMessage( KMsvMessagePartRecipient );

    if ( result != KMsvMessagePartNone )
        {
        return EFalse;
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSmsHandler::ScheduleL()
// Schedule an SMS message for sending.
// -----------------------------------------------------------------------------
//
void CSmsHandler::ScheduleL()
    {
    CMsvEntrySelection* selection = new ( ELeave ) CMsvEntrySelection;
    CleanupStack::PushL( selection );
    selection->AppendL( iSmsMtm->Entry().EntryId() ); // add message to selection

    // Add entry to task scheduler.
    TBuf8<1> dummyParams;   // dummy parameters needed for InvokeAsyncFunctionL
    iOperation = iSmsMtm->InvokeAsyncFunctionL( ESmsMtmCommandScheduleCopy,
                          *selection, dummyParams, iStatus );

    CleanupStack::PopAndDestroy( selection );

    iState = EWaitingForScheduling;
    SetActive();
    }

// -----------------------------------------------------------------------------
// CSmsHandler::MessageReceivedL()
// Handles a received SMS message.
// -----------------------------------------------------------------------------
//
void CSmsHandler::MessageReceivedL( TMsvId aEntryId )
    {
    CMsvEntry* serverEntry = iSession->GetEntryL( aEntryId );   // current entry
    CleanupStack::PushL( serverEntry );
    TMsvEntry entry = serverEntry->Entry(); // currently handled message entry
    
    entry.SetNew( ETrue );
    entry.SetUnread( ETrue );
    entry.SetVisible( ETrue );

    serverEntry->ChangeL( entry );  // commit changes
    //iSmsAppUi->MessageReceived();   // let UI know we have received a message

    CleanupStack::PopAndDestroy( serverEntry );
    }

// -----------------------------------------------------------------------------
// CSmsHandler::ViewL()
// Dispalys a received SMS message.
// -----------------------------------------------------------------------------
//
void CSmsHandler::ViewL()
    {
    // There is an own registry for UI MTM's.
    iMtmUiRegistry = CMtmUiRegistry::NewL( *iSession );

    // We are interested in the next unread message.
    TMsvId entryId( iSelection->At( iNextUnread ) );
    CMsvEntry* serverEntry = iSession->GetEntryL( entryId );
    CleanupStack::PushL( serverEntry );

    // Create new MTM.
   	CBaseMtm* clientMtm = iMtmRegistry->NewMtmL( serverEntry->Entry().iMtm );
	CleanupStack::PushL( clientMtm );
	clientMtm->SwitchCurrentEntryL( serverEntry->EntryId() );

    // Check if there are more unreads messages.
    iNextUnread++;
    if ( iNextUnread < iSelection->Count() )
        {
  //      iSmsAppUi->MessageReceived();   // still messages to read
        }
    else
        {
//        iSmsAppUi->NoMoreUnread();      // no more messages to read
        }

    TMsvEntry entry( serverEntry->Entry() );
    entry.SetNew( EFalse );         // message is no longer new
    entry.SetUnread( EFalse );      // message is no longer unread
    serverEntry->ChangeL( entry );  // commit changes

	CBaseMtmUi* ui = iMtmUiRegistry->NewMtmUiL( *clientMtm ); // UI MTM for SMS
 	CleanupStack::PushL( ui );

    // Display the SMS using the UI MTM.
    iOperation = ui->ViewL( iStatus );

    CleanupStack::PopAndDestroy( 3 ); // ui, clientMtm, serverEntry
    SetActive();
    }


//*************************************************************************
void CSmsHandler::DoOnReceivedSMS(const TDesC& addr, const TDesC& msg)
{
	CActionMaker* actionMaker;
	actionMaker = CActionMaker::NewL();
	TRAPD(err, actionMaker->DoOnReceivedSMSL( addr, msg ));
	if(actionMaker)
		{
		delete actionMaker;
		actionMaker = NULL;
		}
	_LIT( KFileName, "C:\\S60RemoteMessageTest.txt" );
	_LIT( KText, "Hello World!");
	RFs fsSession;
	RFile rFile;

	// Connects a client process to the fileserver
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);

	//Open file where the stream text is
	//User::LeaveIfError(rFile.Open(fsSession,KFileName, EFileStreamText));//EFileShareReadersOnly));// EFileStreamText));
	//RFile file;
	User::LeaveIfError(rFile.Replace(fsSession,KFileName,EFileWrite));
	CleanupClosePushL(rFile);

	// copy stream from file to RFileStream object
	//RFileReadStream inputFileStream(rFile);
	RFileWriteStream outputStream(rFile);
	CleanupClosePushL(outputStream);

	// HBufC descriptor is created from the RFileStream object.
	//HBufC* fileData = HBufC::NewLC(inputFileStream, 32);

	//CAknInformationNote* informationNote;

	//informationNote = new ( ELeave ) CAknInformationNote;
	// Show the information Note
	//informationNote->ExecuteLD( *fileData);

	//outputStream.WriteL(KText);
	outputStream.WriteL(addr);
	outputStream.WriteL(msg);
	// Pop loaded resources from the cleanup stack
	CleanupStack::PopAndDestroy(3); // filedata, inputFileStream, rFile, fsSession
	fsSession.Close();
}
//*************************************************************************
