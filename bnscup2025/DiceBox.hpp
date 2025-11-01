#pragma once
#include "Siv3D.hpp"
#include "Die.hpp"
#include "Explanation.hpp"

class DiceBox
{
public:
	DiceBox(const Vec2& centerPos, Array<Die>& dice);
	void roll(Status& status);
	void draw() const;
	void draw(const Rect& drawArea) const;
	void update(Status& status);
	Die* getClickedDie() const { return m_clickedDie; }
	void clear()
	{
		for (auto& die : m_dice) {
			die.clear();
		}
	}
	bool isRolling() { return m_isRolling; }
private:
	Vec2 m_position;
	Die* m_clickedDie;
	Die* m_hoveredDie;
	Array<Die>& m_dice;
	const double faceSize = 60;
	bool m_locked = false;

	bool m_isRolling = false;
	Stopwatch m_rollSw{ StartImmediately::No };
	Array<double> m_stopAt;	//各ダイスの停止予定時刻
	double m_step = 0.25;	//各ダイスのロール停止間隔
	double m_startStopping = 0.50;	//回し始めてから最初が止まるまでの時間

	mutable Effect m_effect;

	Array<double> m_startSelfAt;
	double m_endSelfEffect = 0.0;
	Array<double> m_startAllAt;
	double m_endAllEffect = 0.0;
};
