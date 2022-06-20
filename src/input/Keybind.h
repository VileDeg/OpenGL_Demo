#pragma once
class Keybind
{
public:
	Keybind() {}
	~Keybind() {}

	virtual void OnPress() const {}
	inline const int GlId() const { return m_GlId; }
private:
	int m_GlId;
};

//class Keybind_Forward
//{
//public:
//	Keybind_Forward();
//	~Keybind_Forward();
//
//	void OnPress() const;
//	inline const int GlId() const { return m_GlId; }
//private:
//	int m_GlId;
//};