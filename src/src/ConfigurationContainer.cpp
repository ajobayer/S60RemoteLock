#include <barsread.h>
#include <S60RemoteLock_0x20023C7B.rsg>

#include "S60RemoteLock.hrh"
#include "ConfigurationContainer.h"
#include "SettingsData.h"

CConfigurationContainer::CConfigurationContainer( CSettingsData* aData )
	: CAknSettingItemList(),
	iSettingsData( aData )
	{
	}

CConfigurationContainer::~CConfigurationContainer()
	{
	}

CConfigurationContainer *CConfigurationContainer::NewL( CSettingsData* aData )
	{
	CConfigurationContainer* self = CConfigurationContainer::NewLC( aData );
	CleanupStack::Pop(self);
	return self;
	}

CConfigurationContainer *CConfigurationContainer::NewLC( CSettingsData* aData )
	{
	CConfigurationContainer* self = 
								new (ELeave) CConfigurationContainer( aData );
	CleanupStack::PushL( self );
	return self;
	}

void CConfigurationContainer::SizeChanged()
    {
	CEikFormattedCellListBox *listbox = ListBox();
    if ( listbox )
        {
        listbox->SetRect( Rect() );
        }
    }

void CConfigurationContainer::HandleListBoxEventL( CEikListBox* /*aListBox*/,
											TListBoxEvent aEventType )
	{
	if( aEventType == EEventEnterKeyPressed )
		{
		EditCurrentItemL();
		}
	}

void CConfigurationContainer::EditCurrentItemL()
	{
	EditItemL( ListBox()->CurrentItemIndex(), ETrue );
//	if( ListBox()->CurrentItemIndex() < 4 )
//		{
//		(*(SettingItemArray()))[ListBox()->CurrentItemIndex()]->
//										UpdateListBoxTextL();
//		}

    StoreSettingsL();
	}

CAknSettingItem *CConfigurationContainer::CreateSettingItemL( 
														TInt aIdentifier )
	{
	CAknSettingItem* settingItem = NULL;

    switch( aIdentifier )
		{
		case ESettingSMS:
			settingItem = new (ELeave) CAknTextSettingItem
										(
										aIdentifier,
							 			iSettingsData->GetSmsText()
							 			);
			break;
		case ESettingNumber:
			settingItem = new (ELeave) CAknTextSettingItem
										(
										aIdentifier,
										iSettingsData->GetNumberText()
										);
			break;
		case ESettingMode:
			settingItem = new (ELeave) CAknBinaryPopupSettingItem
											(
											aIdentifier,
											iSettingsData->GetMode()
											);
			break;
		case ESettingPassword:
			settingItem = new (ELeave) CAknPasswordSettingItem
											(
											aIdentifier,
											CAknPasswordSettingItem::EAlpha,
											iSettingsData->GetPasswordText()
											);
			break;
		default:
			break;
        }

    return settingItem;	
	}

