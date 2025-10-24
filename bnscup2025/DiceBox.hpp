#pragma once
#include "Siv3D.hpp"
#include "Die.hpp"

class DiceBox
{
public:
	DiceBox(const Vec2& position, const Array<Die>& dice);
	void roll();
	void draw() const;
	void update();
	Array<Die> getDice() const{ return m_dice; }
	void clear()
	{
		for (auto& die : m_dice) {
			die.clear();
		}
	}
private:
	Vec2 m_position;
	Array<Die> m_dice;
	Array<RectF> m_boxes;
	const double faceSize = 60;
	bool m_locked = false;
};
