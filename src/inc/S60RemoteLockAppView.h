/*
============================================================================
 Name		: S60RemoteLockAppView.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares view class for application.
============================================================================
*/

#ifndef __S60REMOTELOCKAPPVIEW_h__
#define __S60REMOTELOCKAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
class CS60RemoteLockAppView : public CCoeControl
	{
	public: // New methods

		/**
		* NewL.
		* Two-phased constructor.
		* Create a CS60RemoteLockAppView object, which will draw itself to aRect.
		* @param aRect The rectangle this view will be drawn to.
		* @return a pointer to the created instance of CS60RemoteLockAppView.
		*/
		static CS60RemoteLockAppView* NewL( const TRect& aRect );

		/**
		* NewLC.
		* Two-phased constructor.
		* Create a CS60RemoteLockAppView object, which will draw itself
		* to aRect.
		* @param aRect Rectangle this view will be drawn to.
		* @return A pointer to the created instance of CS60RemoteLockAppView.
		*/
		static CS60RemoteLockAppView* NewLC( const TRect& aRect );

		/**
		* ~CS60RemoteLockAppView
		* Virtual Destructor.
		*/
		virtual ~CS60RemoteLockAppView();

	public:  // Functions from base classes

		/**
		* From CCoeControl, Draw
		* Draw this CS60RemoteLockAppView to the screen.
		* @param aRect the rectangle of this view that needs updating
		*/
		void Draw( const TRect& aRect ) const;

		/**
		* From CoeControl, SizeChanged.
		* Called by framework when the view size is changed.
		*/
		virtual void SizeChanged();

	private: // Constructors

		/**
		* ConstructL
		* 2nd phase constructor.
		* Perform the second phase construction of a
		* CS60RemoteLockAppView object.
		* @param aRect The rectangle this view will be drawn to.
		*/
		void ConstructL(const TRect& aRect);

		/**
		* CS60RemoteLockAppView.
		* C++ default constructor.
		*/
		CS60RemoteLockAppView();

	};

#endif // __S60REMOTELOCKAPPVIEW_h__

// End of File
