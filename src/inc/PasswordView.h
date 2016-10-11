#ifndef __PASSWORD_VIEW_H__
#define __PASSWORD_VIEW_H__

#include <aknappui.h>
#include <aknview.h>
#include <e32std.h>
#include <e32base.h>
#include <eikseced.h>

//#include "ConstantList.h"

const TInt KMaxPasswordLength = 8;

class CPasswordContainer;
class CSettingsData;

class CPasswordView: public CAknView
    {
    public:
    	static CPasswordView* NewL();
        static CPasswordView* NewLC();
        virtual ~CPasswordView();

    public:
        TUid Id() const;
        void HandleCommandL( TInt aCommand );
        void HandleSizeChange( TInt aType );
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        void DoDeactivate();
        TBool ShowPasswordDialogL();

    private:
        CPasswordView();
        void ConstructL();
        CArrayFix<TCoeHelpContext>* HelpContextL() const;
        void ShowErrorMessage( TInt aResourceId );
        void HandleForegroundEventL(TBool aForeground);
        void DetectIMSI();

    private:
    	CSettingsData* 				iSettingsData;
        CPasswordContainer* 		iContainer;
        TUid 						iIdentifier;
        TInt						iCount;
        TBool						iHide;
    };

#endif // __S60REMOTELOCK_PASSWORD_VIEW_H__
