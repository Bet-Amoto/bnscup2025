#include "CategoryBox.hpp"
#include "Status.hpp"

CategoryBox::CategoryBox(const Vec2& position, Category& category)
	: m_position(position)
	, category(&category)
	, m_boxRect(position, 350, 60)
	, m_nameRect(position + Vec2{ 5, 5 }, 290, 50)
	, m_scoreRect(position + Vec2{295, 5}, 50, 50)
{
}

void CategoryBox::draw(const Array<Die>& dices, Status& status) const
{
	m_boxRect.draw(ColorF{ 1.0 }).drawFrame(1, ColorF{ 0 });
	m_scoreRect.rounded(5).draw(category->score ? ColorF{ 0.7 } : ColorF{ 1.0 }).drawFrame(3, ColorF{ 0.1 });
	FontAsset(U"Category")(category->name).draw(28, m_boxRect.x + 5, m_boxRect.y + 11, ColorF{ 0.1 });
	if (category->score.has_value())
	{
		DrawGlyphs(FontAsset(U"Category"), U"{}"_fmt(category->score.value()), 36, m_scoreRect.center(), ColorF{ 0.1 });
	}
	else {
		DrawGlyphs(FontAsset(U"Category"), U"{}"_fmt(getProvisionalScore(dices, status)), 36, m_scoreRect.center(), ColorF{ 0.1 });
	}
}

bool CategoryBox::isClicked() const
{
	return m_scoreRect.leftClicked();
}

bool CategoryBox::mouseOver() const
{
	return m_boxRect.mouseOver();
}

int64 CategoryBox::getProvisionalScore(const Array<Die>& dices, Status& status) const {
	// 基本スコアを計算
	int64 baseScore = (category->calculateScoreWS)
		? category->calculateScoreWS(dices, status)
		: category->calculateScore(dices);
	
	// アイテムの効果を適用したスコアを計算
	int64 modifiedScore = baseScore;
	
	// scoreModifierを持つアイテムを適用
	for (const auto& artifact : status.artifacts)
	{
		if (!artifact.isActive) continue;
		
		if (artifact.scoreModifier)
		{
			modifiedScore = artifact.scoreModifier(modifiedScore, *category, dices, status);
		}
	}
	
	return modifiedScore;
}

void CategoryBox::DrawGlyphs(const Font& font, const String& text, const double MaxfontSize, const Vec2& center, const ColorF& color) const
{
	const Array<Glyph> glyphs = font.getGlyphs(text);
	const Vec2 glyphSize = GlyphsSize(font, text, MaxfontSize);
	const double fontSize = Min(MaxfontSize, MaxfontSize * (m_scoreRect.w - 4) / glyphSize.x);
	const double scale = (fontSize / font.fontSize());
	const double fontHeight = (font.height() * scale);
	const Vec2 basePos = (glyphSize.x < (m_scoreRect.w - 4)) ? center - glyphSize / 2 : center - Vec2{ m_scoreRect.w / 2 - 2,glyphSize.y * (fontSize / MaxfontSize) / 2 };
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


Vec2 CategoryBox::GlyphsSize(const Font& font, const String& text, const double fontSize) const
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
