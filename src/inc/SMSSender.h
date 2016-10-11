#ifndef SMSSENDER_H_
#define SMSSENDER_H_

#include <e32base.h>
#include <in_sock.h>

class CSMSSender : public CBase
{
	public:
		static CSMSSender* NewL();
		static CSMSSender* NewLC();
		virtual ~CSMSSender();
		
	public:
		void SendSMSL( const TDesC& aSenderAddr, const TDesC& aMsg );
		
	private:		
		CSMSSender();
		void ConstructL();
};

#endif /*SMSSENDER_H_*/
