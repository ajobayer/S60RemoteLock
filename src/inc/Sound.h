#ifndef SOUND_H_
#define SOUND_H_

#include <e32std.h>
#include <e32base.h>
#include "File.h"

class CSound: public CBase, public CFile
	{
	public:	
		~CSound();	
		static CSound* NewL();	
		static CSound* NewLC();	
		void RemoveL();
		void Destructor();
	
	private:
		CSound();
		void ConstructL();
	};
	
#endif /*SOUND_H_*/
