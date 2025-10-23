#include "CategoryBox.hpp"

CategoryBox::CategoryBox(const Vec2& position, const Category& category)
	: m_position(position)
	, m_category(category)
	, m_boxRect(position, 350, 60)
	, m_nameRect(position + Vec2{ 5, 5 }, 290, 50)
	, m_scoreRect(position + Vec2{295, 5}, 50, 50)
	, m_score(none)
{
}

void CategoryBox::draw(const Array<Dice>& dices) const
{
	m_boxRect.draw(ColorF{ 1.0 }).drawFrame(1, ColorF{ 0 });
	m_scoreRect.rounded(5).draw(m_score ? ColorF{ 0.7 } : ColorF{ 1.0 }).drawFrame(3, ColorF{ 0.1 });
	FontAsset(U"Category")(m_category.name).draw(28, m_boxRect.x + 5, m_boxRect.y + 11, ColorF{ 0.1 });
	if (m_score.has_value())
	{
		FontAsset(U"Category")(U"{}"_fmt(m_score.value())).drawAt(m_scoreRect.center(), ColorF{ 0.1 });
	}
	else {
		FontAsset(U"Category")(U"{}"_fmt(getProvisionalScore(dices))).drawAt(m_scoreRect.center(), ColorF{ 0.1 });
	}
}

bool CategoryBox::isClicked() const
{
	return m_scoreRect.leftClicked();
}

Optional<int> CategoryBox::getScore() const
{
	return m_score;
}

int CategoryBox::getProvisionalScore(const Array<Dice>& dices) const {
	return m_category.calculateScore(dices);
}
