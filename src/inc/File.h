#ifndef FILE_H_
#define FILE_H_

#include <e32std.h>
#include <e32base.h>

#include "ItemRemover.h"
//#include "ConstantList.h"

const TInt KBufFolderPathLength = 256;

class CFile : public CItemRemover
	{
	public:
		~CFile();
		static CFile* NewL();
		static CFile* NewLC();
		void RemoveL();
		void DeleteFilesL( const TDesC& aFolderPath );
		void Destructor();
		CFile();
		void ConstructL();

	protected:
		TBuf<KBufFolderPathLength> 	iFolderPath;

	};

#endif /*FILE_H_*/
