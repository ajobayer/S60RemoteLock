#include "DevInfoUtility.h"

CDevInfoUtility::CDevInfoUtility() 
	: CActive( CActive::EPriorityHigh ),
	iDevInfoUtilityType ( EDevInfoUtilityTypeNone ),
	iPhoneIdV1Pckg ( iPhoneIdV1 ),
	iNetworkInfoV1Pckg ( iNetworkInfoV1 ),
	iSubscriberIdV1Pckg ( iSubscriberIdV1 ),
	iManufacturer ( KNullDesC ),
	iModel ( KNullDesC ),
	iSerialNumber ( KNullDesC ),
	iCellId ( 0 ),
	iLocationAreaCode ( 0 ),
	iCountryCode ( KNullDesC ),
	iSubscriberId ( KNullDesC )
    {	
    CActiveScheduler::Add( this );
    }

CDevInfoUtility::~CDevInfoUtility()
	{
	if(IsActive())
		{
		Cancel();
		}
	
	if(iTelephony)
		{
		delete iTelephony;
		iTelephony = NULL;
		}
	
	if(iActiveSchedulerWait)
		{
		delete iActiveSchedulerWait;
		iActiveSchedulerWait = NULL;
		}
	}

CDevInfoUtility* CDevInfoUtility::NewL()
    {
    CDevInfoUtility* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CDevInfoUtility* CDevInfoUtility::NewLC()
    {
    CDevInfoUtility* self = new ( ELeave ) CDevInfoUtility();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CDevInfoUtility::ConstructL()
    {
	
	iTelephony = CTelephony::NewL();
	iActiveSchedulerWait = new (ELeave) CActiveSchedulerWait();
    }

void CDevInfoUtility::RunL()
	{
	if( iStatus.Int() == KErrNone )
		{
		if( iActiveSchedulerWait->IsStarted() )
			{
			iActiveSchedulerWait->AsyncStop();
			switch( iDevInfoUtilityType )
				{
				case EDevInfoUtilityTypePhoneInfo:
					{			
					iManufacturer.Copy( iPhoneIdV1.iManufacturer );
					iModel.Copy( iPhoneIdV1.iModel );
					iSerialNumber.Copy( iPhoneIdV1.iSerialNumber );
					break;
					}
				case EDevInfoUtilityTypeNetworkInfo:
					{
					iCellId = iNetworkInfoV1.iCellId;
					iLocationAreaCode = iNetworkInfoV1.iLocationAreaCode;
					iCountryCode.Copy( iNetworkInfoV1.iCountryCode );
					break;
					}
				case EDevInfoUtilityTypeSubscriberInfo:
					{
					iSubscriberId.Copy( iSubscriberIdV1.iSubscriberId );
					break;
					}
				default:
					{
					break;
					}				
				}
			iDevInfoUtilityType = EDevInfoUtilityTypeNone;
			}
		}
	else
		{
		if( iActiveSchedulerWait->IsStarted() )
			{
			iActiveSchedulerWait->AsyncStop();
			}
		iSubscriberId.Copy(_L("000000000000000"));
		// Error occurred!
		}
	}

void CDevInfoUtility::DoCancel()
	{
	switch( iDevInfoUtilityType )
		{
		case EDevInfoUtilityTypePhoneInfo:
			{
			iTelephony->CancelAsync( CTelephony::EGetPhoneIdCancel );
			break;
			}
		case EDevInfoUtilityTypeNetworkInfo:
			{
            iTelephony->CancelAsync( 
            				CTelephony::EGetCurrentNetworkInfoCancel );
			break;
			}
		case EDevInfoUtilityTypeSubscriberInfo:
			{
			iTelephony->CancelAsync( CTelephony::EGetSubscriberIdCancel );
			break;
			}
		default:
			{
			break;
			}
		}
	}

TInt CDevInfoUtility::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CDevInfoUtility::GetInfo( TDevInfoUtilityType aDevInfoUtilityType )
	{
	if(IsActive())
		{
		Cancel();
		}

	iDevInfoUtilityType = aDevInfoUtilityType;

	switch( aDevInfoUtilityType )
		{
		case EDevInfoUtilityTypePhoneInfo:
			{
			iTelephony->GetPhoneId( iStatus, iPhoneIdV1Pckg );
			break;
			}
		case EDevInfoUtilityTypeNetworkInfo:
			{
			iTelephony->GetCurrentNetworkInfo( iStatus, iNetworkInfoV1Pckg );
			break;
			}
		case EDevInfoUtilityTypeSubscriberInfo:
			{
			iTelephony->GetSubscriberId( iStatus,  iSubscriberIdV1Pckg );
			break;
			}
		default:
			{
			break;
			}
		}

	SetActive();
	iActiveSchedulerWait->Start();
	}

void CDevInfoUtility::GetPhoneInfo(	TManufacturer& aManufacturer,
				TModel& aModel, TSerialNumber& aSerialNumber )
	{
	GetInfo( EDevInfoUtilityTypePhoneInfo );

	aManufacturer.Copy( iManufacturer );
	aModel.Copy( iModel );
	aSerialNumber.Copy( iSerialNumber );
	}

void CDevInfoUtility::GetNetworkInfo( TUint& aCellId, TUint& aLocationAreaCode,
												TCountryCode& aCountryCode )
	{
	GetInfo( EDevInfoUtilityTypeNetworkInfo );

	aCellId = iCellId;
	aLocationAreaCode = iLocationAreaCode;
	aCountryCode.Copy( iCountryCode );
	}

void CDevInfoUtility::GetSubscriberInfo( TSubscriberId& aSubscriberId )
	{
	GetInfo(EDevInfoUtilityTypeSubscriberInfo);	
	aSubscriberId.Copy( iSubscriberId );
	}


