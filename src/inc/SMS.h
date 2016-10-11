#ifndef SMS_H_
#define SMS_H_

#include <e32base.h>
#include <msvapi.h>
#include <mtuireg.h>
#include <txtrich.h>

#include "ItemRemover.h"

class CSmsAppUi;
class CClientMtmRegistry;
class CSmsClientMtm;

class CSms: public CActive, 
			public MMsvSessionObserver, 
			public CItemRemover
    {
    public: 
        static CSms* NewL( );
        static CSms* NewLC();
        virtual ~CSms();

    public: 
     	void HandleSessionEventL( TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/,
                                  TAny* /*aArg2*/, TAny* /*aArg3*/ );
        void SMSDeleteL( TMsvId aFolderId );
        void Destructor();
        void RemoveL();
  
    protected: 
        void DoCancel();
        void RunL();

    private: 
        CSms();
        void ConstructL(); 
        void AccessMtmL();
  
    private: 
        CMsvSession* iSession;
        CClientMtmRegistry* iMtmRegistry;
        CSmsClientMtm* iSmsMtm;
        CMsvOperation* iOperation;
        CSmsAppUi* iSmsAppUi;
        CMtmUiRegistry* iMtmUiRegistry;   
        CMsvEntrySelection* iSelection;
        TInt iNextUnread;
    };

#endif /*SMS_H_*/
