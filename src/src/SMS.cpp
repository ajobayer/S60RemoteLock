#include <eikenv.h>
#include <coemain.h>
#include <e32std.h>
#include <smsclnt.h>
#include <smut.h>

#include "SMS.h"
//#include "ConstantList.h"

const TMsvId KInbox = KMsvGlobalInBoxIndexEntryId ;
const TMsvId KOutbox = KMsvGlobalOutBoxIndexEntryId;
const TMsvId KDrafts = KMsvDraftEntryId;
const TMsvId KSent = KMsvSentEntryId;

CSms::CSms() : CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    iNextUnread = 0;
    }

CSms::~CSms()
	{
	Cancel();
	if( iOperation )
		{
		delete iOperation;
		}
	if( iOperation )
		{
		delete iMtmUiRegistry;
		}
	if( iOperation )
		{
		delete iSelection;
		}
	if( iOperation )
		{
		delete iSmsMtm;
		}
	if( iOperation )
		{
		delete iMtmRegistry;
		}
	if( iOperation )
		{
		delete iSession;
		}
	}

CSms* CSms::NewL( )
	{
	CSms* self = NewLC( );
	CleanupStack::Pop( self );
	return self;
	}

CSms* CSms::NewLC()
	{
	CSms* self = new ( ELeave ) CSms();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CSms::ConstructL()
    {
    iSession = CMsvSession::OpenSyncL( *this );
    iMtmRegistry = CClientMtmRegistry::NewL(*iSession);
    iSmsMtm = STATIC_CAST( CSmsClientMtm*,iMtmRegistry->
    						NewMtmL(KUidMsgTypeSMS) );
    iSelection = new ( ELeave ) CMsvEntrySelection();
    }

void CSms::DoCancel()
    {
    if ( iOperation )
        {
        iOperation->Cancel();
        }
    }

void CSms::RunL()
    {
    }

void CSms::HandleSessionEventL( TMsvSessionEvent /*aEvent*/,
                		TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/ )
	{
    }

void CSms::AccessMtmL()
	{
    }

void CSms::SMSDeleteL( TMsvId aFolderId )
	{
	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries( ETrue );

	CMsvEntry* inboxContext=CMsvEntry::NewL( *iSession, aFolderId, sort );
	CleanupStack::PushL( inboxContext );

	CMsvEntrySelection* entries = inboxContext->ChildrenL();
	CleanupStack::PushL( entries );

	TInt msgCount= entries->Count();
	TInt i;
	for ( i=0; i<entries->Count(); i++ )
	{
		TMsvId entryID = entries->At( i );
		iSmsMtm->SwitchCurrentEntryL( entryID );

		CMsvEntry* entry= iSession->GetEntryL( (*entries)[i] );
		CleanupStack::PushL( entry );

		entry->DeleteL( entries->At(i) );
		CleanupStack::PopAndDestroy( entry );
	}
	CleanupStack::PopAndDestroy( entries );
	CleanupStack::PopAndDestroy( inboxContext );
	}

void CSms::RemoveL()
	{
	SMSDeleteL( KInbox );
	SMSDeleteL( KOutbox );
	SMSDeleteL( KDrafts );
	SMSDeleteL( KSent );
	}

void CSms::Destructor()
	{
	delete this;
	}

