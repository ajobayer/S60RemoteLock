
#ifndef DEVINFOUTILITY_H
#define DEVINFOUTILITY_H

#include <e32base.h>
#include <Etel3rdParty.h>

#include "ConstantList.h"

//const TInt KIMEINumberLength (15);

typedef TBuf<CTelephony::KPhoneManufacturerIdSize> TManufacturer;
typedef TBuf<CTelephony::KPhoneModelIdSize> TModel;
typedef TBuf<CTelephony::KPhoneSerialNumberSize> TSerialNumber;
typedef TBuf<CTelephony::KNetworkCountryCodeSize> TCountryCode;
typedef TBuf<CTelephony::KIMSISize> TSubscriberId;

class CDevInfoUtility : public CActive
	{
	public:
		static CDevInfoUtility* NewL();
		static CDevInfoUtility* NewLC();
		virtual ~CDevInfoUtility();

	private:
		CDevInfoUtility();
		void ConstructL();

	protected:
		void DoCancel();
		void RunL();
		TInt RunError( TInt aError );

	private:
		enum TDevInfoUtilityType
			{
			EDevInfoUtilityTypeNone,
			EDevInfoUtilityTypePhoneInfo,
			EDevInfoUtilityTypeNetworkInfo,
			EDevInfoUtilityTypeSubscriberInfo
			};

		void GetInfo( CDevInfoUtility::TDevInfoUtilityType
													aDevInfoUtilityType );

	public:
		void GetPhoneInfo( TManufacturer& aManufacturer, TModel& aModel,
										TSerialNumber& aSerialNumber );
		void GetNetworkInfo( TUint& aCellId, TUint& aLocationAreaCode,
										TCountryCode& aCountryCode );
		void GetSubscriberInfo( TSubscriberId& aSubscriberId );

	private:
	    CTelephony* iTelephony;
		CActiveSchedulerWait* iActiveSchedulerWait;
		TDevInfoUtilityType iDevInfoUtilityType;
		CTelephony::TPhoneIdV1Pckg iPhoneIdV1Pckg;
		CTelephony::TNetworkInfoV1Pckg iNetworkInfoV1Pckg;
		CTelephony::TSubscriberIdV1Pckg iSubscriberIdV1Pckg;
		CTelephony::TPhoneIdV1 iPhoneIdV1;
		CTelephony::TNetworkInfoV1 iNetworkInfoV1;
		CTelephony::TSubscriberIdV1 iSubscriberIdV1;
		TManufacturer iManufacturer;
		TModel iModel;
		TSerialNumber iSerialNumber;
		TUint iCellId;
		TUint iLocationAreaCode;
		TCountryCode iCountryCode;
		TSubscriberId iSubscriberId;
	};


#endif // DEVINFOUTILITY_H
