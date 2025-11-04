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
	void draw(const Array<Die>& dices, Status& status) const;
	bool isClicked() const;
	bool mouseOver() const;
	void setScore(int64 score) { category->score = score; }
	Optional<int64> getScore() const { return category->score; }
	int64 getProvisionalScore(const Array<Die>& dices, Status& status) const;
	const Category& getCategory() const { return *category; }
	void reset() { category->score = none; }

	Category* category;
private:
	Vec2 m_position;
	RectF m_boxRect;
	RectF m_nameRect;
	RectF m_scoreRect;

	void DrawGlyphs(const Font& font, const String& text, const double fontSize, const Vec2& center, const ColorF& color) const;
	Vec2 GlyphsSize(const Font& font, const String& text, const double fontSize) const;
};
