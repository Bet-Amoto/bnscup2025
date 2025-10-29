# pragma once
# include "Siv3D.hpp"
# include "ItemBase.hpp"

class Merchandise
{
public:
	Merchandise() = default;
	Merchandise(std::shared_ptr<ItemBase> item,const Vec2& pos)
		: m_item(item), m_pos(pos), m_box(pos.x - 70, pos.y - 100, 140, 200, 10)
	{
	}
	std::shared_ptr<ItemBase> getItem() const { return m_item; }
	void update();
	void draw() const;
	bool isClicked() const { return m_box.leftClicked(); }
	void setSoldOut(bool soldOut) { m_soldOut = soldOut; }
	bool isSoldOut() const { return m_soldOut; }
private:

	std::shared_ptr<ItemBase> m_item;
	Vec2 m_pos;
	RoundRect m_box;
	bool m_soldOut = false;
};
