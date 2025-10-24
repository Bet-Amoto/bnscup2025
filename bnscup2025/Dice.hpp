#pragma once
#include "Siv3D.hpp"

class Dice
{
public:
	Dice(const Vec2& position);
	void roll();
	Optional<int> getValue() const;
	void draw() const;
	bool isClicked() const;
	void setLocked(bool locked) { m_locked = locked; }
	bool isLocked() const { return m_locked; }
	void Clear() { m_value = none; m_locked = false; }
private:
	Optional<int> m_value;
	Vec2 m_position;
	const double faceSize = 60;
	bool m_locked = false;
};
