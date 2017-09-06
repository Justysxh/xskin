#ifndef __EVENT_H_INCLUDED__
#define __EVENT_H_INCLUDED__

#include "../core/UIObject.h"

class XSKIN_API UIResponser;
class XSKIN_API UIEvent : public UIObject
{
public:
	enum EventType
	{
		EVENT_UNKNOWN = 0,
		EVENT_MOUSE_PRESSED,
		EVENT_MOUSE_RELEASED,
		EVENT_MOUSE_MOVED,
		EVENT_MOUSE_DRAGGED,
		EVENT_MOUSE_ENTERED,
		EVENT_MOUSE_EXITED,
		EVENT_MOUSE_CLICKED,
		EVENT_MOUSE_DBLCLICKED,
		EVENT_MOUSE_WHEEL,
		EVENT_KEY_PRESSED,
		EVENT_KEY_RELEASED,
		EVENT_FOCUS_GAIN,
		EVENT_FOCUS_LOST,
		EVENT_ACTION,
		EVENT_TIMER,
		EVENT_HIERARCHY_CHILD_ADDED,
		EVENT_HIERARCHY_CHILD_REMOVED,
	};

	enum EventFlags
	{
		ALT_MASK			= 1 << 0,
		CTRL_MASK			= 1 << 1,
		SHIFT_MASK			= 1 << 2,
		LEFT_BUTTON_MASK	= 1 << 3,
		RIGHT_BUTTON_MASK	= 1 << 4,
		MIDDLE_BUTTON_MASK	= 1 << 5
	};

					UIEvent(UIResponser* pSource, EventType nType, int nFlags);
					~UIEvent();

public:
	UIResponser*	GetSource() const;
	int				GetTimestamp() const;
	EventType		GetType() const;
	int				GetFlags() const;
	static int		GetKeyStateFlags();

private:
	UIResponser* m_pSource;
	EventType m_nType;
	int m_nTimestamp;
	int m_nFlags;
};
#endif //! __EVENT_H_INCLUDED__