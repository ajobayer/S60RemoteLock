#include <CNTDB.H>
#include <CNTFIELD.H>
#include <CNTITEM.H>
#include <CNTFLDST.H>
#include <CNTDEF.H>
#include <CNTFILT.H>
#include <S32MEM.H>
#include <avkon.hrh>
#include <aknnotewrappers.h>

#include "Contact.h"

CContact::CContact()
	{
	}

CContact::~CContact()
	{
	}

CContact* CContact::NewL()
	{
	CContact* self = CContact::NewLC();
	CleanupStack::Pop();
	return self;
	}

CContact* CContact::NewLC()
	{
	CContact* self = new (ELeave) CContact();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CContact::ConstructL()
	{
	}

void CContact::DeleteContactsL()
    {    
	TBuf<60> dbFile;
	CContactDatabase::GetDefaultNameL( dbFile );
	CContactDatabase* contactDb = CContactDatabase::OpenL( dbFile );
	CleanupStack::PushL( contactDb );
	TContactIter contactIterator( *contactDb );
	TContactItemId contactId;

	while( (contactId = contactIterator.NextL() ) != KNullContactId )
		{
		contactDb->DeleteContactL( contactId );
		}
	CleanupStack::PopAndDestroy( contactDb );
    }

void CContact::RemoveL()
	{	
	DeleteContactsL();
	}

void CContact::Destructor()
	{
	delete this;
	}

