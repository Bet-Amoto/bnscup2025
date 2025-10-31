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
		m_item->drawIcon(m_pos.movedBy(0,-30));
		DrawGlyphs(FontAsset(U"Bold"), m_item->name, 24, Vec2{ m_pos.x, m_pos.y + 30 }, ColorF{ 0.1 });
		//FontAsset(U"Bold")(m_item->name).drawAt(24, m_pos.x, m_pos.y + 10, ColorF{ 0.1 });
		FontAsset(U"Regular")(Format(m_item->cost) + U"G").drawAt(24, m_pos.x, m_pos.y + 70, ColorF{ 0.1 });
		if(m_soldOut)m_box.draw(ColorF{ 0,0.5 });
	}
}

void Merchandise::DrawGlyphs(const Font& font, const String& text,const double MaxfontSize, const Vec2& center, const ColorF& color) const
{
	const Array<Glyph> glyphs = font.getGlyphs(text);
	const Vec2 glyphSize = GlyphsSize(font, text, MaxfontSize);
	const double fontSize = Min(MaxfontSize, MaxfontSize * (m_box.w - 10) / glyphSize.x);
	const double scale = (fontSize / font.fontSize());
	const double fontHeight = (font.height() * scale);
	const Vec2 basePos = (glyphSize.x < (m_box.w - 10)) ? center - glyphSize / 2 : center - Vec2{ m_box.w / 2 - 5,glyphSize.y * (fontSize / MaxfontSize) / 2 };
	// このオブジェクトが存在する間、すべての 2D 描画に SDF / MSDF シェーダが適用される
	const ScopedCustomShader2D shader{ Font::GetPixelShader(font.method()) };

	Vec2 penPos{ basePos };

	// 文字単位で描画を制御するためのループ
	for (const auto& glyph : glyphs)
	{
		// 改行文字なら
		if (glyph.codePoint == U'\n')
		{
			// ペンの X 座標をリセットする
			penPos.x = basePos.x;

			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += fontHeight;

			continue;
		}

		// 文字のテクスチャをペンの位置に文字ごとのオフセットを加算して描画する
		glyph.texture.scaled(scale).draw((penPos + glyph.getOffset(scale)), color);

		// ペンの X 座標を文字の幅の分進める
		penPos.x += (glyph.xAdvance * scale);
	}
}


Vec2 Merchandise::GlyphsSize(const Font& font, const String& text, const double fontSize) const
{
	const Array<Glyph> glyphs = font.getGlyphs(text);
	const double scale = (fontSize / font.fontSize());
	const double fontHeight = (font.height() * scale);
	Vec2 penPos{ 0.0, 0.0 };
	double maxX = 0.0;
	// 文字単位で描画を制御するためのループ
	for (const auto& glyph : glyphs)
	{
		// 改行文字なら
		if (glyph.codePoint == U'\n')
		{
			// ペンの X 座標をリセットする
			penPos.x = 0.0;
			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += fontHeight;
			continue;
		}
		// ペンの X 座標を文字の幅の分進める
		penPos.x += (glyph.xAdvance * scale);
		maxX = Max(maxX, penPos.x);
	}
	return Vec2{ maxX, penPos.y + fontHeight };
}
