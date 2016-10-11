#ifndef CONTACT_H_
#define CONTACT_H_

#include <e32std.h>
#include <e32base.h>

#include "ItemRemover.h"

class CContact: public CBase, public CItemRemover
	{
	public:	
		~CContact();	
		static CContact* NewL();	
		static CContact* NewLC();	
		void RemoveL();
		void Destructor();
	
	private:
		CContact();
		void ConstructL();
		void DeleteContactsL();
	};
	
#endif /*CONTACT_H_*/
