#include "Dice.hpp"

Dice::Dice(const Vec2& position)
	: m_value(none), m_position(position)
{
}

void Dice::roll()
{
	if (isLocked()) return;
	m_value = Random(1, 6);
}

Optional<int> Dice::getValue() const
{
	return m_value;
}

void Dice::draw() const
{
	RectF faceRect(m_position, faceSize, faceSize);
	faceRect.draw((m_locked || !m_value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
	if(m_value)FontAsset(U"Bold")(Format(m_value.value())).drawAt(faceRect.center(), ColorF{0.1});
}

bool Dice::isClicked() const
{
	RectF faceRect(m_position, faceSize, faceSize);
	return faceRect.leftClicked();
}
