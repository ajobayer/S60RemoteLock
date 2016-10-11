#ifndef __S60REMOTELOCKAPPLICATION_H__
#define __S60REMOTELOCKAPPLICATION_H__

#include <aknapp.h>
#include "S60RemoteLock.hrh"

const TUid KUidS60RemoteLockApp = { _UID3 };

class CS60RemoteLockApplication : public CAknApplication
	{
	public:
		TUid AppDllUid() const;

	protected:
		CApaDocument* CreateDocumentL();
	};

#endif // __S60REMOTELOCKAPPLICATION_H__

