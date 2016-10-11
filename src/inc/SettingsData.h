#ifndef __SettingsData_H__
#define __SettingsData_H__

#include <e32base.h>
#include <in_sock.h>

//#include "ConstantList.h"

const TInt KBufSmsLength = 50;
const TInt KBufNumberLength = 20;
const TInt KBufPasswordLength = 8;

class RReadStream;
class RWriteStream;

class CSettingsData : public CBase
{
	public:
		static CSettingsData* NewL();
		static CSettingsData* NewLC();
		virtual ~CSettingsData();
	    void LoadL( RReadStream& aStream );
		void SaveL( RWriteStream& aStream ) const;
		void SetDefaultValues();
		void InternalizeSettingsDataL();
		void ExternalizeSettingsDataL() const;

	public:
		TBuf<KBufSmsLength>& GetSmsText();
		TBool& GetMode();
		TBuf<KBufPasswordLength>& GetPasswordText();
		TBuf<KBufNumberLength>& GetNumberText();

	private:
		CSettingsData();
		void ConstructL();
		void ShowErrorMessage( TInt aResourceId );

	private:
		TBuf<KBufSmsLength> iSmsText;
		TBuf<KBufNumberLength> iNumberText;
		TBuf<KBufPasswordLength> iPasswordText;
		TBool iMode;
	};

#endif // __SettingsData_H__

