#include <f32file.h>
#include <s32file.h>
#include <BAUTILS.H>
#include <aknnotewrappers.h>

#include "File.h"

CFile::CFile()
	{
	}

CFile::~CFile()
	{
	}

CFile* CFile::NewL()
	{
	CFile* self=CFile::NewLC();
	CleanupStack::Pop();
	return self;
	}

CFile* CFile::NewLC()
	{
	CFile* self = new (ELeave) CFile();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CFile::ConstructL()
	{
	}

void CFile::RemoveL()
	{
	}

void CFile:: DeleteFilesL( const TDesC& aFolderPath )
	{
	RFs fsSession;
		
	User::LeaveIfError( fsSession.Connect() );
	TBool isFolderExists = BaflUtils::FolderExists( fsSession, aFolderPath );
	
	if( isFolderExists )
		{
		CFileMan* fileMan = CFileMan::NewL( fsSession );
		CleanupStack::PushL( fileMan );
		TFileName strName( aFolderPath );
		fileMan->RmDir( strName );
		CleanupStack::PopAndDestroy();		

		TInt error = fsSession.MkDirAll( aFolderPath );				
		 if( error != KErrAlreadyExists && error != KErrNone )
			 {
			 User::Leave(error);
			 }
		}
	fsSession.Close();
	}

void CFile::Destructor()
	{
	delete this;
	}

