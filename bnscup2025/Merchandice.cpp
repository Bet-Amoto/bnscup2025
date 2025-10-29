# include "Merchandice.hpp"

void Merchandise::update()
{
	if (m_box.mouseOver()) {
		GetExplanation().setItem(m_item.get());
	}
}

void Merchandise::draw() const
{
	m_box.draw(ColorF{ 0.95, 0.9, 0.8 });
	m_box.drawFrame(2, ColorF{ 0.7, 0.5, 0.3 });
	if (m_item)
	{
		m_item->drawIcon(m_pos.movedBy(0,-40));
		FontAsset(U"Bold")(m_item->name).drawAt(28, m_pos.x, m_pos.y + 10, ColorF{ 0.1 });
		FontAsset(U"Regular")(Format(m_item->cost) + U"G").drawAt(24, m_pos.x, m_pos.y + 40, ColorF{ 0.1 });
		if(m_soldOut)m_box.draw(ColorF{ 0,0.5 });
	}
}
