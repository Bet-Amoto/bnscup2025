#pragma once
#include "Siv3D.hpp"
#include "Die.hpp"

class DiceBox
{
public:
	DiceBox(const Vec2& position, Array<Die>& dice);
	void roll(Status& status);
	void draw() const;
	void update();
	Die* getClickedDie() const { return m_clickedDie; }
	void clear()
	{
		for (auto& die : m_dice) {
			die.clear();
		}
	}
private:
	Vec2 m_position;
	Die* m_clickedDie;
	Array<Die>& m_dice;
	const double faceSize = 60;
	bool m_locked = false;
};
