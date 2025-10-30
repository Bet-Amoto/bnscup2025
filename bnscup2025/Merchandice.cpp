# include "Merchandice.hpp"

void Merchandise::update()
{
	if (m_box.mouseOver()) {
		GetExplanation().setItem(m_item.get());
	}
}

void Merchandise::draw() const
{
	ColorF color = m_item ? RarityToColor(m_item->rarity) : RarityToColor(Rarity::Common);
	m_box.draw(color);
	m_box.drawFrame(2, color * 0.5);
	
	if (m_item)
	{
		m_item->drawIcon(m_pos.movedBy(0,-40));
		FontAsset(U"Bold")(m_item->name).drawAt(28, m_pos.x, m_pos.y + 10, ColorF{ 0.1 });
		FontAsset(U"Regular")(Format(m_item->cost) + U"G").drawAt(24, m_pos.x, m_pos.y + 40, ColorF{ 0.1 });
		if(m_soldOut)m_box.draw(ColorF{ 0,0.5 });
	}
}
