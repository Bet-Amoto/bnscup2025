# pragma once
# include "Siv3D.hpp"
# include "ItemBase.hpp"
# include "Explanation.hpp"
# include "Rarity.hpp"
class Merchandise
{
public:
	Merchandise() = default;
	Merchandise(std::shared_ptr<ItemBase> item,const Vec2& pos)
		: m_item(item), m_pos(pos), m_box(pos.x - 70, pos.y - 90, 140, 180, 10)
	{
	}
	std::shared_ptr<ItemBase> getItem() const { return m_item; }
	void update();
	void draw() const;
	bool isClicked() const { return m_box.leftClicked(); }
	void setSoldOut(bool soldOut) { m_soldOut = soldOut; }
	bool isSoldOut() const { return m_soldOut; }
	void setPos(const Vec2& pos) {
		m_pos = pos;
		m_box = RoundRect{ pos.x - 70, pos.y - 90, 140, 180, 10 };
	}

private:

	std::shared_ptr<ItemBase> m_item;
	Vec2 m_pos;
	RoundRect m_box;
	bool m_soldOut = false;

	void DrawGlyphs(const Font& font, const String& text, const double fontSize, const Vec2& center, const ColorF& color) const;
	Vec2 GlyphsSize(const Font& font, const String& text, const double fontSize) const;
};
