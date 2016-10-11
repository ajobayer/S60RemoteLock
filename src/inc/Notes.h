#ifndef NOTES_H_
#define NOTES_H_

#include <e32std.h>
#include <e32base.h>

#include "File.h"

class CNotes: public CBase, public CFile
	{
	public:	
		~CNotes();	
		static CNotes* NewL();	
		static CNotes* NewLC();	
		void RemoveL();
		void Destructor();
	
	private:
		CNotes();
		void ConstructL();
	};
	
#endif /*NOTES_H_*/
