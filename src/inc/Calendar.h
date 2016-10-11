#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <e32std.h>
#include <e32base.h>
#include <EIKENV.H>
#include <S32MEM.H>
#include <S32FILE.H>
#include <calsession.h>
#include <caliterator.h>
#include <e32cmn.h>
#include <calentry.h>
#include <calentryview.h>
#include <caldataexchange.h>
#include <caleninterimutils.h> 
#include <caldataformat.h>
#include <calalarm.h>
#include <calrrule.h>
#include <calprogresscallback.h>
#include <eikprogi.h>
#include <AknProgressDialog.h>

#include "ItemRemover.h"

class CCalendar :	public MCalProgressCallBack,
					public MProgressDialogCallback,
					public CItemRemover
	{
	public:
		~CCalendar();
		static CCalendar* NewL();
		static CCalendar* NewLC();
		void RemoveL();
		void Destructor();

	private:
		CCalendar();
		void ConstructL();

	private:
		void Progress( TInt aPercentageCompleted );
		void Completed( TInt aError );
		TBool NotifyProgress();

	public:
		void DeleteEntryL();

	protected:
		void DialogDismissedL (TInt aButtonId);
	};
	
#endif /*CALENDAR_H_*/
