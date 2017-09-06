#ifndef __UIRESPONSER_H_INCLUDED__
#define __UIRESPONSER_H_INCLUDED__

#include "UIObject.h"
#include "../event/UIEvent.h"

class XSKIN_API UIResponser : public UIObject
{
public:
							UIResponser();
							~UIResponser();

public:
	virtual BOOL			Event(UIEvent* event);
	virtual UIResponser*	GetNextResponser() const;
};
#endif //! __UIRESPONSER_H_INCLUDED__