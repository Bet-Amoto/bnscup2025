#include "DiceBox.hpp"

DiceBox::DiceBox(const Vec2& position, const Array<Die>& dice)
	: m_position(position), m_dice(dice)
{
	for(auto i : step(m_dice.size())) {
		m_boxes << RectF{ position.x + i * (faceSize + 10), position.y, faceSize, faceSize };
	}
}

void DiceBox::roll()
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
	}
}

void DiceBox::draw() const
{
	for (size_t i = 0; i < m_dice.size(); ++i)
	{
		m_dice[i].draw(m_boxes[i].center());
	}
}

void DiceBox::update()
{
	for (size_t i = 0; i < m_boxes.size(); ++i)
	{
		if (m_boxes[i].leftClicked())
		{
			m_dice[i].locked = !m_dice[i].locked;
		}
	}
}
