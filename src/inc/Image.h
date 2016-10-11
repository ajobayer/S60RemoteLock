#ifndef IMAGE_H_
#define IMAGE_H_

#include <e32std.h>
#include <e32base.h>

#include "File.h"

class CImage: public CBase, public CFile
	{
	public:	
		~CImage();	
		static CImage* NewL();	
		static CImage* NewLC();	
		void RemoveL();
		void Destructor();
	
	private:
		CImage();
		void ConstructL();
	};

#endif /*IMAGE_H_*/
