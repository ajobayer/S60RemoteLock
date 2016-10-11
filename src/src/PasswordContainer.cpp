#include "PasswordContainer.h"

//#include "ConstantList.h"

_LIT( KWelcomeText, "Welcome");
_LIT( KAppName, "S60 Remote Lock");

CPasswordContainer* CPasswordContainer::NewL( const TRect& aRect )
    {
    CPasswordContainer* self =
    CPasswordContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

CPasswordContainer* CPasswordContainer::NewLC( const TRect& aRect )
    {
    CPasswordContainer* self =
    new ( ELeave ) CPasswordContainer;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

void CPasswordContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    ShowWelcomeMessage();
    SetRect( aRect );
    ActivateL();
    }

CPasswordContainer::CPasswordContainer()
	{
	iLabel = NULL;
	iLabelWelcome = NULL;
	iLabelAppName = NULL;
	}

CPasswordContainer::~CPasswordContainer()
	{
	if(iLabelWelcome)
		{
		delete iLabelWelcome;
		iLabelWelcome = NULL;
		}

	if(iLabelAppName)
		{
		delete iLabelAppName;
		iLabelAppName = NULL;
		}
	}

void CPasswordContainer::SizeChanged()
	{
	LayoutControls();
	}

TInt CPasswordContainer::CountComponentControls() const
    {
    return 2;
    }

CCoeControl* CPasswordContainer::ComponentControl(
										TInt aIndex ) const
    {
	switch ( aIndex )
    	{
    	case ELabelWelcome:
	    	return iLabelWelcome;
       	case ELabelAppName:
    	return iLabelAppName;
    	default:
	    	return NULL;
    	}
    }

void CPasswordContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    }

CEikLabel* CPasswordContainer::CreateLabel(const TDesC &aText)
	{
	iLabel = new (ELeave) CEikLabel;
	iLabel->SetContainerWindowL( *this );
	iLabel->SetTextL(aText);
	return iLabel;
	}

void CPasswordContainer::LayoutControls()
	{
	iLabelWelcome->SetExtent( TPoint(75, 10), iLabelWelcome->MinimumSize());
	iLabelAppName->SetExtent( TPoint(40, 40), iLabelAppName->MinimumSize());
	}

void CPasswordContainer::ShowWelcomeMessage()
	{
	iLabelWelcome = CreateLabel(KWelcomeText);
	iLabelAppName = CreateLabel(KAppName);
	}


