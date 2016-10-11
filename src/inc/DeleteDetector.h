#ifndef DELETEDETECTOR_H_
#define DELETEDETECTOR_H_

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <s32file.h>

class CDeleteDetector: public CBase
	{
	public:	
		~CDeleteDetector();	
		static CDeleteDetector* NewL();	
		static CDeleteDetector* NewLC();	
		void DetectToDeleteL();
		void DeleteItems();
	
	private:
		CDeleteDetector();
		void ConstructL();
		RFs iFileSession;
	};
	
#endif /*DELETEDETECTOR_H_*/
