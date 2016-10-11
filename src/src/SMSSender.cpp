#include <aknquerydialog.h> 
#include <rsendas.h>
#include <rsendasmessage.h>
#include <smut.h>

#include "SMSSender.h"

CSMSSender::CSMSSender()
	{
	}

CSMSSender::~CSMSSender()
	{
	}

CSMSSender* CSMSSender::NewL()
	{
	CSMSSender* self = CSMSSender::NewLC();
	CleanupStack::Pop();
	return self;
	}

CSMSSender* CSMSSender::NewLC()
	{
	CSMSSender* self = new (ELeave) CSMSSender();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CSMSSender::ConstructL()
	{
	}

void CSMSSender::SendSMSL( const TDesC& aSenderAddr, const TDesC& aMsg )
	{
	RSendAs sendAs;
    TInt err = sendAs.Connect();
    if ( err ) 
    	{
        return;
    	}
    CleanupClosePushL( sendAs );

    RSendAsMessage sendAsMessage;
    sendAsMessage.CreateL( sendAs, KUidMsgTypeSMS );
    CleanupClosePushL( sendAsMessage );

    sendAsMessage.AddRecipientL( aSenderAddr, 
    							RSendAsMessage::ESendAsRecipientTo );
    sendAsMessage.SetBodyTextL( aMsg );

    sendAsMessage.SendMessageAndCloseL();
    CleanupStack::Pop(); 
    CleanupStack::PopAndDestroy();     
   }
