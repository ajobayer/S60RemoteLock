#ifndef __S60REMOTELOCKDOCUMENT_h__
#define __S60REMOTELOCKDOCUMENT_h__

#include <akndoc.h>

class CS60RemoteLockAppUi;
class CEikApplication;

class CS60RemoteLockDocument : public CAknDocument
	{
	public:
		static CS60RemoteLockDocument* NewL( CEikApplication& aApp );
		static CS60RemoteLockDocument* NewLC( CEikApplication& aApp );
		virtual ~CS60RemoteLockDocument();

	public:
		CEikAppUi* CreateAppUiL();

	private:
		void ConstructL();
		CS60RemoteLockDocument( CEikApplication& aApp );
	};

#endif // __S60REMOTELOCKDOCUMENT_h__

