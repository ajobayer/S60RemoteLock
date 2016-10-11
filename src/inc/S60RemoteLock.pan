/*
============================================================================
 Name		: S60RemoteLock.pan
 Author	  : 
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
============================================================================
*/

#ifndef __S60REMOTELOCK_PAN__
#define __S60REMOTELOCK_PAN__

/** S60RemoteLock application panic codes */
enum TS60RemoteLockPanics
	{
	ES60RemoteLockUi = 1
	// add further panics here
	};

inline void Panic(TS60RemoteLockPanics aReason)
	{
	_LIT(applicationName,"S60RemoteLock");
	User::Panic(applicationName, aReason);
	}

#endif // __S60REMOTELOCK_PAN__
