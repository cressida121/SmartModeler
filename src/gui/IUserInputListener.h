#ifndef IUSER_INPUT_LISTENER_H
#define IUSER_INPUT_LISTENER_H

enum KeyID : unsigned {
	UNKNOWN = 0,
	LMB = 1,
	MMB = 2,
	RMB = 3,
	SHIFT = 4,
	CONTROL = 5,
	ALT = 6,
};

class IMouseEventListener {
public:
	virtual ~IMouseEventListener() = default;
	virtual void OnMouseMove(int posx, int posy) = 0;
	virtual void OnMouseMoveDelta(int dx, int dy) = 0;
	virtual void OnMouseButtonPress(KeyID keyid) = 0;
	virtual void OnMouseButtonRelease(KeyID keyid) = 0;
	virtual void OnMouseWheelScroll(int delta) = 0;
};

class IKeyboardEventListener {
public:
	virtual ~IKeyboardEventListener() = default;
	virtual void OnKeyPress(KeyID keyid) = 0;
	virtual void OnKeyRelease(KeyID keyid) = 0;
};

#endif // !IUSER_INPUT_LISTENER_H