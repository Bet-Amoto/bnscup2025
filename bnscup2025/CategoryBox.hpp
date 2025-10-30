#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"
#include "Explanation.hpp"
class CategoryBox
{
public:
	CategoryBox(const Vec2& position, Category& category);
	void draw(const Array<Die>& dices) const;
	bool isClicked() const;
	void setScore(int score) { m_category->score = score; }
	Optional<int> getScore() const { return m_category->score; }
	int getProvisionalScore(const Array<Die>& dices) const;
	const Category& getCategory() const { return *m_category; } // ★ constバージョンを追加
	void reset() { m_category->score = none; }
private:
	Vec2 m_position;
	RectF m_boxRect;
	RectF m_nameRect;
	RectF m_scoreRect;
	Category* m_category;
};
