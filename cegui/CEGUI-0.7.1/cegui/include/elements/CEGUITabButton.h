/***********************************************************************
	filename: 	CEGUITabButton.h
	created:	8/8/2004
	author:		Steve Streeting
	
	purpose:	Interface to base class for TabButton widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUITabButton_h_
#define _CEGUITabButton_h_

#include "../CEGUIBase.h"
#include "CEGUIButtonBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Base class for TabButtons.  A TabButton based class is used internally as
    the button that appears at the top of a TabControl widget to select the
    active tab pane.
*/
class CEGUIEXPORT TabButton : public ButtonBase
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by Window
	static const String EventClicked;					//!< The button was clicked.
	static const String EventDragged;					//!< Attempt to drag the button with middle button.
	static const String EventScrolled;					//!< Scroll wheel activated on top of the button.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for base TabButton class
	*/
	TabButton(const String& type, const String& name);


	/*!
	\brief
		Destructor for TabButton class
	*/
	virtual ~TabButton(void);

    /*!
    \brief
        Set whether this tab button is selected or not
    */
    virtual void setSelected(bool selected) { d_selected = selected; invalidate(); }

    /*!
    \brief
        Return whether this tab button is selected or not
    */
    bool isSelected(void) const { return d_selected; }


    /*!
    \brief
        Set the target window which is the content pane which this button is
        covering.
    */
    void setTargetWindow(Window* wnd);
    /*!
    \brief
        Get the target window which is the content pane which this button is
        covering.
    */
    Window* getTargetWindow(void) { return d_targetWindow; }

protected:
    /*************************************************************************
    Implementation Data
    *************************************************************************/
    bool    d_selected;             //!< Is this button selected?
    bool    d_dragging;             //!< In drag mode or not
    Window* d_targetWindow;         //!< The target window which this button is representing
	/*************************************************************************
		New Event Handlers
	*************************************************************************/
	/*!
	\brief
		handler invoked internally when the button is clicked.
	*/
	virtual void	onClicked(WindowEventArgs& e);


	/*************************************************************************
		Overridden Event Handlers
	*************************************************************************/
    virtual void onMouseButtonUp(MouseEventArgs& e);
    virtual void onMouseButtonDown(MouseEventArgs& e);
    virtual void onMouseWheel(MouseEventArgs& e);
    virtual void onMouseMove(MouseEventArgs& e);

	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool	testClassName_impl(const String& class_name) const
	{
		if (class_name=="TabButton")	return true;
		return ButtonBase::testClassName_impl(class_name);
	}
};


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUITabButton_h_
