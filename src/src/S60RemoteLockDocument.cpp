#include "S60RemoteLockAppUi.h"
#include "S60RemoteLockDocument.h"

CS60RemoteLockDocument::CS60RemoteLockDocument( CEikApplication& aApp )
	: CAknDocument( aApp )
	{
	}

CS60RemoteLockDocument::~CS60RemoteLockDocument()
	{
	}

CS60RemoteLockDocument* CS60RemoteLockDocument::NewL( CEikApplication& aApp )
	{
	CS60RemoteLockDocument* self = NewLC( aApp );
	CleanupStack::Pop( self );
	return self;
	}

CS60RemoteLockDocument* CS60RemoteLockDocument::NewLC( CEikApplication& aApp )
	{
	CS60RemoteLockDocument* self =
		new ( ELeave ) CS60RemoteLockDocument( aApp );

	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CS60RemoteLockDocument::ConstructL()
	{
	}

CEikAppUi* CS60RemoteLockDocument::CreateAppUiL()
	{
	return ( static_cast <CEikAppUi*> ( new ( ELeave )
										CS60RemoteLockAppUi ) );
	}

