#include <eikstart.h>

#include "S60RemoteLockApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CS60RemoteLockApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}

