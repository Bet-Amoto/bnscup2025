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
	for (auto* die : dice) {
		die->roll(m_dice);
		if (die->afterSelfFunc)
		{
			die->afterSelfFunc(*die, m_dice, status);
		}
	}
	for (auto* die : dice)
	{
		if (die->afterAllFunc)
		{
			die->afterAllFunc(*die, m_dice, status);
		}
	}
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

void DiceBox::update()
{
	m_clickedDie = nullptr;
	m_hoveredDie = nullptr;
	for (const auto i : step(m_dice.size()))
	{
		const RectF box{ m_position.x + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		if (box.leftClicked())
		{
			m_clickedDie = &m_dice[i];
		}
		if(box.mouseOver())
		{
			m_hoveredDie = &m_dice[i];
			GetExplanation().setItem(m_hoveredDie);
		}
	}
}
