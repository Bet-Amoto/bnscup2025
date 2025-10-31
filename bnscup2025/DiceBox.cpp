#include "DiceBox.hpp"

DiceBox::DiceBox(const Vec2& position, Array<Die>& dice)
	: m_position(position), m_dice(dice), m_clickedDie(nullptr)
{
}

void DiceBox::roll(Status& status)
{
	Array<Die*> dice;
	for (auto& die : m_dice) {
		dice << &die;
	}
	dice.sort_by([](const Die* a, const Die* b) {
		return static_cast<int>(a->order) < static_cast<int>(b->order);
	});

	if (m_isRolling) return;
	m_isRolling = true;
	m_rollSw.restart();
	m_stopAt.clear();
	m_stopAt.resize(m_dice.size(), -1.0);

	for (auto& die : dice)
	{
		if (!die->locked) die->beginSpin();
	}

	for (size_t i = 0; i < m_dice.size(); i++)
	{
		if (m_dice[i].locked) continue;
		m_stopAt[i] = m_startStopping + i * m_step;
	}

	//for (auto* die : dice) {
	//	die->roll(m_dice, status);
	//}
	//for (auto* die : dice)
	//{
	//	if (die->afterAllFunc && !die->locked)
	//	{
	//		die->afterAllFunc(*die, m_dice, status);
	//	}
	//}
}

void DiceBox::draw() const
{
	for (size_t i = 0; i < m_dice.size(); ++i)
	{
		const RectF box{ m_position.x + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		m_dice[i].draw(box.center());
	}
}

void DiceBox::draw(const Rect& drawArea) const
{
	for (size_t i = 0; i < m_dice.size(); ++i)
	{
		const RectF box{ m_position.x + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		m_dice[i].draw(box.center());
	}
}

void DiceBox::update(Status& status)
{
	m_clickedDie = nullptr;
	m_hoveredDie = nullptr;
	for (const auto i : step(m_dice.size()))
	{
		const RectF box{ m_position.x + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		if (box.leftClicked() && !m_isRolling)
		{
			m_clickedDie = &m_dice[i];
		}
		if(box.mouseOver())
		{
			m_hoveredDie = &m_dice[i];
			GetExplanation().setItem(m_hoveredDie);
		}
	}

	if (!m_isRolling) return;

	for (auto& die : m_dice) die.updateSpin();

	bool allStopped = true;
	for (size_t i = 0; i < m_dice.size(); i++)
	{
		if (m_stopAt[i] < 0)
		{
			allStopped &= (!m_dice[i].isSpinning);
			continue;
		}
		if (m_rollSw.sF() >= m_stopAt[i] && m_dice[i].isSpinning)
		{
			m_dice[i].stopSpin(m_dice, status);
			m_stopAt[i] = -1.0;
		}
		allStopped &= (!m_dice[i].isSpinning);
	}

	if (allStopped)
	{
		for (auto& die : m_dice)
		{
			if (die.afterAllFunc && !die.locked) die.afterAllFunc(die, m_dice, status);
		}
		m_isRolling = false;
		m_rollSw.reset();
	}
}
