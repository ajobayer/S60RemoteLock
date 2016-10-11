#ifndef VIDEO_H_
#define VIDEO_H_

#include <e32std.h>
#include <e32base.h>

#include "File.h"

class CVideo: public CBase, public CFile
	{
	public:	
		~CVideo();	
		static CVideo* NewL();	
		static CVideo* NewLC();	
		void RemoveL();
		void Destructor();
	
	private:
		CVideo();
		void ConstructL();
	};
	
#endif /*VIDEO_H_*/
