#ifndef SIMDETECTOR_H_
#define SIMDETECTOR_H_

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <s32file.h>

class CSIMDetector: public CBase
	{
	public:	
		~CSIMDetector();	
		static CSIMDetector* NewL();	
		static CSIMDetector* NewLC();	
		void DetectSIMChange();
	
	private:
		CSIMDetector();
		void ConstructL();
		void StoreSubscriberInfo(const TDesC& aIMSI);
		void RequestToSendMail();
		RFs fileSession;
	};
	
#endif 
