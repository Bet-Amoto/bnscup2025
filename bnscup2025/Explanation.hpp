# pragma once
# include "Siv3D.hpp"
# include "ItemBase.hpp"

class Explanation
{
public:
	Explanation() = default;
	void update(Vec2 position);
	void draw(const ItemBase& item, Vec2 position, const Rect& drawArea) const;
	void draw(const ItemBase& item, Vec2 position) const;

private:
	const double m_maxWidth = 400.0;
	const Rect m_drawArea = Rect{ 0, 0, Scene::Size() }.stretched(-10);
	const double m_startTime = 0.2;

	double m_t = 0;
	Vec2 m_lastPos;
	void DrawGlyphs(const Font& font, const String& text, const double fontSize, const Vec2& basePos, const ColorF& color) const;
	Vec2 GlyphsSize(const Font& font, const String& text, const double fontSize) const;
};
