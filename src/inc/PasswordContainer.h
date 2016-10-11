#ifndef __PASSWORD_CONTAINER_H__
#define __PASSWORD_CONTAINER_H__

#include <aknview.h>
#include <akndef.h>
#include <eiklabel.h>

class CPasswordContainer : public CCoeControl
    {
    public:
        static CPasswordContainer* NewL( const TRect& aRect );
        static CPasswordContainer* NewLC( const TRect& aRect );
        void ConstructL( const TRect& aRect );
        virtual ~CPasswordContainer();

    private:
    	CPasswordContainer();

    public:
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl( TInt aIndex ) const;
        void Draw( const TRect& aRect ) const;
        virtual void SizeChanged();
    	CEikLabel* CreateLabel(const TDesC &aText);
    	void LayoutControls();
     	void ShowWelcomeMessage();

    public:
		enum TControls
			{
			ELabelWelcome,
			ELabelAppName,
			ELastControl
			};

    private:
    	CEikLabel* iLabel;
    	CEikLabel* iLabelWelcome;
    	CEikLabel* iLabelAppName;
    };

#endif // __S60REMOTELOCK_PASSWORD_CONTAINER_H__

