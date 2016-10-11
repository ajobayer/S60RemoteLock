#include "S60RemoteLock.hrh"
#include "S60RemoteLockDocument.h"
#include "S60RemoteLockApplication.h"

CApaDocument* CS60RemoteLockApplication::CreateDocumentL()
	{
	return (static_cast<CApaDocument*>
					( CS60RemoteLockDocument::NewL( *this ) ) );
	}

TUid CS60RemoteLockApplication::AppDllUid() const
	{
	return KUidS60RemoteLockApp;
	}


