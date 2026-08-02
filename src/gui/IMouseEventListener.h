class IMouseEventListener {
public:
	virtual ~IMouseEventListener() = default;
	virtual void OnMouseMove(int posx, int y) = 0;
	virtual void OnMouseMoveDelta(int dx, int dy) = 0;
	virtual void OnMouseButtonPress(int button) = 0;
	virtual void OnMouseButtonRelease(int button) = 0;
	virtual void OnMouseWheelScroll(int delta) = 0;
};