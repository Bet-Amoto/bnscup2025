#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"
#include "Explanation.hpp"

// 前方宣言
struct Status;

class CategoryBox
{
public:
	CategoryBox(const Vec2& position, Category& category);
	void draw(const Array<Die>& dices, const Status& status) const;
	bool isClicked() const;
	void setScore(int score) { category->score = score; }
	Optional<int> getScore() const { return category->score; }
	int getProvisionalScore(const Array<Die>& dices, const Status& status) const;
	const Category& getCategory() const { return *category; } // ★ constバージョンを追加
	void reset() { category->score = none; }

	Category* category;
private:
	Vec2 m_position;
	RectF m_boxRect;
	RectF m_nameRect;
	RectF m_scoreRect;
};
