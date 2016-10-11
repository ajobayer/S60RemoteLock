#ifndef __CONFIGURE_CONTAINER_H__
#define __CONFIGURE_CONTAINER_H__

#include <aknview.h>
#include <coecntrl.h>
#include <aknsettingitemlist.h>

#include "ConfigurationView.h"

class CSettingsData;

class CConfigurationContainer : public CAknSettingItemList
	{
	public:
		virtual ~CConfigurationContainer();
		static CConfigurationContainer* NewL( CSettingsData* aData );
		static CConfigurationContainer* NewLC( CSettingsData* aData );
		void SizeChanged();
		void EditCurrentItemL();

	private:
		CConfigurationContainer( CSettingsData* aData );
		CAknSettingItem* CreateSettingItemL( TInt aSettingId );
		void HandleListBoxEventL( CEikListBox* /*aListBox*/,
								TListBoxEvent aEventType );
		TBool IsValidSmsTextLength();
		void ShowErrorMessage( TInt aResourceId );

	private:
		CSettingsData* iSettingsData;
	};

#endif // __CONFIGURE_CONTAINER_H__
