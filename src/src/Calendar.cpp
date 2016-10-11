#include <calsession.h>
#include <caliterator.h>
#include <e32cmn.h>
#include <calentry.h>
#include <calentryview.h>
#include <caldataexchange.h>
#include <caleninterimutils.h>
#include <caldataformat.h>
#include <calalarm.h>
#include <calrrule.h>
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <e32des8.h>

#include "ItemRemover.h"
#include "Calendar.h"

CCalendar::CCalendar()
	{
	}

CCalendar::~CCalendar()
	{
	}

CCalendar* CCalendar::NewLC()
	{
	CCalendar* self = new (ELeave) CCalendar();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CCalendar* CCalendar::NewL()
	{
	CCalendar* self = CCalendar::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CCalendar::ConstructL()
	{
	}

void CCalendar::Progress( TInt /*aPercentageCompleted*/ )
	{
	}

void CCalendar::Completed( TInt /*aError*/ )
	{
	CActiveScheduler::Stop();
	}

TBool CCalendar::NotifyProgress()
	{
	return EFalse;
	}

void CCalendar::DeleteEntryL()
	{
	CCalSession* calSession = CCalSession::NewL();
	CleanupStack::PushL( calSession );
	TBuf<256> file;
	file.Copy( calSession->DefaultFileNameL() );
	TRAPD( err, calSession->OpenL(file) );
	if(err)
		{
		User::LeaveIfError(err);
		}
	
	CCalIter* calIter = CCalIter::NewL( *calSession );
	CleanupStack::PushL( calIter );
	TBuf8<50> iterUID;
	RPointerArray< CCalEntry > calEntryArray;
	CCalEntryView* calEntryView = CCalEntryView::NewL( *calSession, *this );
	CleanupStack::PushL( calEntryView );
	CActiveScheduler::Start();
	
	CCalDataExchange* calDataExchange = CCalDataExchange::NewL( *calSession );
	CleanupStack::PushL( calDataExchange );
	iterUID = calIter->FirstL();
	while( iterUID != KNullDesC8 )
		{
		calEntryView->FetchL( iterUID, calEntryArray );
		iterUID = calIter->NextL();
		};
		
	CCalEntry* aEntry = NULL;
	TInt totalCount = calEntryArray.Count();
	for( TInt i=0;i<totalCount;i++ )
		{
		aEntry = calEntryArray[ i ];
		calEntryView->DeleteL( *aEntry );
		}
	calEntryArray.ResetAndDestroy();
	
	CleanupStack::PopAndDestroy( calDataExchange );
	CleanupStack::PopAndDestroy( calEntryView );
	CleanupStack::PopAndDestroy( calIter );
	CleanupStack::PopAndDestroy( calSession );
	}

void CCalendar::DialogDismissedL( TInt aButtonId )
	{
	if ( aButtonId == EAknSoftkeyCancel )
		{
		CActiveScheduler::Stop();
		}
	}

void CCalendar::RemoveL()
	{
	DeleteEntryL();
	}

void CCalendar::Destructor()
	{
	delete this;
	}
