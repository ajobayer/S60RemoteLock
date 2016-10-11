#include "Scheduler.h"
#include "Contact.h"
#include "SMS.h"
#include "Image.h"
#include "Calendar.h"
#include "Video.h"
#include "Sound.h"
#include "Notes.h"
#include "ItemRemover.h"

CScheduler::CScheduler()
	{
	}

CScheduler::~CScheduler()
	{
	DestructItemObjectsL();
	ClearTasksL();
	iItemsToRemove.Close();
	}

CScheduler* CScheduler::NewL()
	{
	CScheduler* self=CScheduler::NewLC();
	CleanupStack::Pop();
	return self;
	}

CScheduler* CScheduler::NewLC()
	{
	CScheduler* self = new (ELeave) CScheduler();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CScheduler::ConstructL()
	{
	}

void CScheduler::ClearTasksL()
	{	
	iItemsToRemove.Reset();
	}

void CScheduler::AddTaskToRemoveContactL()
	{
	iItemsToRemove.Append( CContact::NewL() );
	}

void CScheduler::AddTaskToRemoveSMSL()
	{	
	iItemsToRemove.Append( CSms::NewL() );
	}

void CScheduler::AddTaskToRemoveCalendarL()
	{
	iItemsToRemove.Append( CCalendar::NewL() );
	}

void CScheduler::AddTaskToRemoveImageL()
	{
	iItemsToRemove.Append( CImage::NewL() );
	}

void CScheduler::AddTaskToRemoveVideoL()
	{
	iItemsToRemove.Append( CVideo::NewL() );
	}

void CScheduler::AddTaskToRemoveSoundL()
	{
	iItemsToRemove.Append( CSound::NewL() );
	}

void CScheduler::AddTaskToRemoveNoteL()
	{
	iItemsToRemove.Append( CNotes::NewL() );
	}

void CScheduler::RemoveItemsL()
	{	
	TInt totalItemsToRemove;
	totalItemsToRemove = iItemsToRemove.Count();
	for( TInt indx = 0; indx<totalItemsToRemove; indx++ )
		{
		iItemsToRemove[indx]->RemoveL();	
		}
	}

void CScheduler::DestructItemObjectsL()
	{	
	TInt totalItemsToRemove;
	totalItemsToRemove = iItemsToRemove.Count();
	for( TInt indx = 0; indx<totalItemsToRemove; indx++ )
		{		
		TRAPD(error, iItemsToRemove[indx]->Destructor());
		}
	}

