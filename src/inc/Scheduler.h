#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <e32std.h>
#include <e32base.h>
#include <S32MEM.H>
#include <e32cmn.h>

class CItemRemover;
class ActionMaker;

class CScheduler : public CBase
	{
	public:
		~CScheduler();
		static CScheduler* NewL();
		static CScheduler* NewLC();
		void ClearTasksL();
		void AddTaskToRemoveContactL();
		void AddTaskToRemoveSMSL();
		void AddTaskToRemoveCalendarL();
		void AddTaskToRemoveImageL();
		void AddTaskToRemoveVideoL();
		void AddTaskToRemoveSoundL();
		void AddTaskToRemoveNoteL();
		void RemoveItemsL();
		void DestructItemObjectsL();
	
	private:
		CScheduler();
		void ConstructL();

	private:
		RPointerArray<CItemRemover> iItemsToRemove;
	};
	
#endif /*SCHEDULER_H_*/
