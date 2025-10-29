# include "Explanation.hpp"

// 最初に呼ばれたときに一回だけ初期化される
Explanation& GetExplanation()
{
	static Explanation instance;
	return instance;
}

void Explanation::update(Vec2 position)
{
	m_item = nullptr;
	if (m_t < m_startTime)
	{
		m_t += Scene::DeltaTime();
	}
	else
	{
		m_t = m_startTime;
	}
	if(m_lastPos != position)
	{
		m_t = 0;
		m_lastPos = position;
	}
}

void Explanation::draw(Vec2 position, const Rect& drawArea) const
{
	if (m_t < m_startTime || !m_item) return;

	constexpr int padding = 10;
	const Vec2 NameGlyphsSize = GlyphsSize(FontAsset(U"Regular"), m_item->name, 20.0);
	const Vec2 DescriptionGlyphsSize = GlyphsSize(FontAsset(U"Regular"), m_item->description, 16.0);
	const Vec2 size = DescriptionGlyphsSize
		+ Vec2{ 0, NameGlyphsSize.y }
		+ Vec2{ padding * 2, padding * 3 + 20 };

	position.y = Min(position.y, drawArea.h - size.y);
	position.x = Min(position.x, drawArea.w - size.x);
	RectF box{ position, size.x, size.y };
	box.rounded(5).draw(ColorF{ 1.0, 0.97, 0.6 }).drawFrame(2, ColorF{ 0.1 });
	DrawGlyphs(FontAsset(U"Regular"), m_item->name, 20, Vec2{ position.x + padding, position.y + padding }, ColorF{ 0.0 });
	position.y += NameGlyphsSize.y + padding;
	DrawGlyphs(FontAsset(U"Regular"), m_item->description, 16, Vec2{ position.x + padding, position.y + padding }, ColorF{ 0.0 });
}

void Explanation::draw(Vec2 position) const {
	draw(position, m_drawArea);
}

void Explanation::DrawGlyphs(const Font& font, const String& text, const double fontSize, const Vec2& basePos, const ColorF& color) const
{
	const Array<Glyph> glyphs = font.getGlyphs(text);
	const double scale = (fontSize / font.fontSize());
	const double fontHeight = (font.height() * scale);

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

		if(penPos.x + (glyph.xAdvance * scale) - basePos.x > m_maxWidth)
		{
			// ペンの X 座標をリセットする
			penPos.x = basePos.x;
			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += fontHeight;
		}

		// 文字のテクスチャをペンの位置に文字ごとのオフセットを加算して描画する
		glyph.texture.scaled(scale).draw((penPos + glyph.getOffset(scale)), color);

		// ペンの X 座標を文字の幅の分進める
		penPos.x += (glyph.xAdvance * scale);
	}
}

Vec2 Explanation::GlyphsSize(const Font& font, const String& text, const double fontSize) const
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
		if (penPos.x + (glyph.xAdvance * scale) > m_maxWidth)
		{
			// ペンの X 座標をリセットする
			penPos.x = 0.0;
			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += fontHeight;
		}
		// ペンの X 座標を文字の幅の分進める
		penPos.x += (glyph.xAdvance * scale);
		maxX = Max(maxX, penPos.x);
	}
	return Vec2{ maxX, penPos.y + fontHeight };
}
