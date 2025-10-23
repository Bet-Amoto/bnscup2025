#pragma once
#include "Common.hpp"
#include "Siv3D.hpp"
#include "Category.hpp"

class CategoryBox
{
public:
	CategoryBox(const Vec2& position, const Category& category);
	void draw(const Array<Dice>& dices) const;
	bool isClicked() const;
	void setScore(int score) { m_score = score; }
	Optional<int> getScore() const;
	int getProvisionalScore(const Array<Dice>& dices) const;
	Category getCategory() const { return m_category; }
	void reset() { m_score = none; }
private:
	Vec2 m_position;
	RectF m_boxRect;
	RectF m_nameRect;
	RectF m_scoreRect;
	Category m_category;
	Optional<int> m_score;
};
