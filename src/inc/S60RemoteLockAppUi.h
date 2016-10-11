#ifndef __S60REMOTELOCK_APPUI_H__
#define __S60REMOTELOCK_APPUI_H__

#include <aknViewAppUi.h>
#include <aknnavi.h>

class CPasswordView;
class CConfigurationView;
class CSmsHandler;

class CS60RemoteLockAppUi : public CAknViewAppUi
    {

    public:
    	CS60RemoteLockAppUi();
    	~CS60RemoteLockAppUi();
    	void ConstructL();

    public:
    	void HandleCommandL( TInt aCommand );
        virtual void HandleResourceChangeL( TInt aType );
        void WriteFile();

	private:
		void InternalizeSettingsDataL();
		void ExternalizeSettingsDataL() const;
		void ShowErrorMessage( TInt aResourceId );
		void SendToBackGround();
		void DetectIMSI();
		void DetectToDelete();

    private:
		CPasswordView* iAppPasswordView;
		CConfigurationView* iAppConfigureView;
		TBool	iInvisivble;
		CSmsHandler* iSmsHandler;
    };

#endif // __S60REMOTELOCK_APPUI_H__

