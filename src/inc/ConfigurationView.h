#ifndef __CONFIGURE_VIEW_H__
#define __CONFIGURE_VIEW_H__

#include <aknview.h>

class CConfigurationContainer;
class CSettingsData;

class CConfigurationView : public CAknView
    {
	public:
		virtual ~CConfigurationView();
		static CConfigurationView* NewL();
		static CConfigurationView* NewLC();
		void DoActivateL( const TVwsViewId& aPrevViewId,
						 	   TUid aCustomMessageId,
						 	   const TDesC8& aCustomMessage );
		void DoDeactivate();
		TUid Id() const;
		void HandleCommandL( TInt aCommand );
		void HandleSizeChange( TInt aType );
		void ShowSelectionListQueryDialog();		

	private:
		CConfigurationView();
		void ConstructL();
		void HandleStatusPaneSizeChange ();
		TBool IsValidSmsTextLength();
		TBool IsValidPhoneNumber();
		TBool IsValidPasswordLength();
		void ShowErrorMessage( TInt aResourceId );
		CArrayFix<TCoeHelpContext>* HelpContextL() const;
		
		void HandleForegroundEventL(TBool aForeground);
		
	private:
		CConfigurationContainer* 	iSettingList;
		CSettingsData* 				iSettingsData;
	};

#endif // __S60REMOTELOCK_CONFIGURE_VIEW_H__
