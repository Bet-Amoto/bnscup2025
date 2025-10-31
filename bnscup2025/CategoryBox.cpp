#include "CategoryBox.hpp"
#include "Status.hpp"

CategoryBox::CategoryBox(const Vec2& position, Category& category)
	: m_position(position)
	, category(&category)
	, m_boxRect(position, 350, 60)
	, m_nameRect(position + Vec2{ 5, 5 }, 290, 50)
	, m_scoreRect(position + Vec2{295, 5}, 50, 50)
{
}

void CategoryBox::draw(const Array<Die>& dices, const Status& status) const
{
	m_boxRect.draw(ColorF{ 1.0 }).drawFrame(1, ColorF{ 0 });
	m_scoreRect.rounded(5).draw(category->score ? ColorF{ 0.7 } : ColorF{ 1.0 }).drawFrame(3, ColorF{ 0.1 });
	FontAsset(U"Category")(category->name).draw(28, m_boxRect.x + 5, m_boxRect.y + 11, ColorF{ 0.1 });
	if (category->score.has_value())
	{
		FontAsset(U"Category")(U"{}"_fmt(category->score.value())).drawAt(m_scoreRect.center(), ColorF{ 0.1 });
	}
	else {
		FontAsset(U"Category")(U"{}"_fmt(getProvisionalScore(dices, status))).drawAt(m_scoreRect.center(), ColorF{ 0.1 });
	}
}

bool CategoryBox::isClicked() const
{
	return m_scoreRect.leftClicked();
}

bool CategoryBox::mouseOver() const
{
	return m_boxRect.mouseOver();
}

int CategoryBox::getProvisionalScore(const Array<Die>& dices, const Status& status) const {
	// 基本スコアを計算
	int baseScore = category->calculateScore(dices);
	
	// アイテムの効果を適用したスコアを計算
	int modifiedScore = baseScore;
	
	// scoreModifierを持つアイテムを適用
	for (const auto& artifact : status.artifacts)
	{
		if (!artifact.isActive) continue;
		
		if (artifact.scoreModifier)
		{
			modifiedScore = artifact.scoreModifier(modifiedScore, *category, dices, status);
		}
	}
	
	return modifiedScore;
}
