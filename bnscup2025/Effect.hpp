#pragma once
#include <Siv3D.hpp>


//ライトブルームクラス
class LightBloom {
public:
	const RenderTexture gaussianA1, gaussianB1;
	const RenderTexture gaussianA4, gaussianB4;
	const RenderTexture gaussianA8, gaussianB8;
	const Size area;
	const ColorF colorA1, colorA4, colorA8;

	LightBloom(const Size& size, const ColorF& colorA1 = ColorF(0.1), const ColorF& colorA4 = ColorF(0.4), const ColorF& colorA8 = ColorF(0.8))
		:area{ size },
		gaussianA1{ size }, gaussianB1{ size },
		gaussianA4{ size / 4 }, gaussianB4{ size / 4 },
		gaussianA8{ size / 8 }, gaussianB8{ size / 8 },
		colorA1{ colorA1 },
		colorA4{ colorA4 },
		colorA8{ colorA8 } {
	}

	void draw()
	{
		Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		Shader::Downsample(gaussianA1, gaussianA4);
		Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		Shader::Downsample(gaussianA4, gaussianA8);
		Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		const ScopedRenderStates2D blend{ BlendState::Additive };
		gaussianA1.resized(area).draw(colorA1);
		gaussianA4.resized(area).draw(colorA4);
		gaussianA8.resized(area).draw(colorA8);
	}
};


struct GoldPopEffect : IEffect
{
	Vec2 m_pos;
	int m_amount = 0;
	double m_dur = 0.50; //秒

	explicit GoldPopEffect(const Vec2& _pos, int _amount, double _dur = 0.35)
		: m_pos(_pos),
		m_amount(_amount),
		m_dur(_dur) { }

	bool update(double t) override
	{
		double k = t / m_dur;
		double e = EaseOutCirc(k);
		double a = 1.0 - e;

		FontAsset(U"Bold")(U"+{}G"_fmt(m_amount)).drawAt(m_pos.movedBy(0, -30 + e * -50), ColorF(1.0, 0.9, 0.2, a));

		return (t < m_dur);
	}
};


struct QuakeEffect : IEffect
{
	Vec2 m_pos;
	double m_dur = 0.25;

	QuakeEffect(const Vec2& _pos, double _dur = 0.25)
		: m_pos(_pos),
		m_dur(_dur) { }

	bool update(double t) override
	{
		double k = t / m_dur;
		double e = EaseOutExpo(k);
		double a = 1.0 - e;

		Circle(m_pos, e * 300).drawFrame(a * 5, HSV(16, 0.3, 0.54));

		return (t < m_dur);
	}
};


struct GodEffect : IEffect
{
	Vec2 m_pos;
	double m_dur = 0.35;
	const Texture m_texture{ U"example/particle.png", TextureDesc::Mipped };
	LightBloom light{ Scene::Size(), ColorF{1}, ColorF{1}, ColorF{1} };

	GodEffect(const Vec2& _pos, double _dur)
		: m_pos(_pos),
		m_dur(_dur) { }

	bool update(double t) override
	{
		double k = t / m_dur;
		double e = EaseOutExpo(k);
		double a = EaseInOutExpo(k);

		{
			const ScopedRenderTarget2D target{ light.gaussianA1.clear(ColorF{ 0.0 }) };
			const ScopedRenderStates2D blend{ BlendState::Additive };
			m_texture.scaled(2.0 - a * 2.0).drawAt(m_pos, HSV(48, 1, 0.8, 1.2 - a));
			m_texture.scaled(2.0 - a * 2.0).drawAt(m_pos, HSV(48, 0.3, 1, 1.2 - a));

			m_texture.scaled(Vec2(e * 10.0, 4 * (1 - e))).drawAt(m_pos, HSV(300, 0.5, 0.8, 1 - a));
		}
		light.draw();

		return (t < m_dur);
	}
};


struct StopDiceEffect : IEffect
{
	Vec2 m_pos;
	double m_dur = 0.20;

	StopDiceEffect(const Vec2& _pos, double _dur = 0.20)
		: m_pos(_pos),
		  m_dur(_dur) { }

	bool update(double t) override
	{
		double k = t / m_dur;
		double e = EaseOutCirc(k);
		double a = 1.0 - e;

		RectF faceRect(m_pos.x - 30, m_pos.y - 30, 60, 60);
		faceRect.rounded(3).stretched(e * 5.0).drawFrame(3, ColorF(1, a));

		return (t < m_dur);
	}
};


struct BuffEffect : IEffect
{
	struct BuffIcon
	{
		Vec2 m_pos;
		double offset;
		HSV color;

		BuffIcon(const Vec2& _pos, double _offset)
			:m_pos{ _pos },
			offset{ _offset },
			color{ HSV(Random(240, 280), 1.0, 1.0) }
		{

		}

		void update(double t)
		{
			double k = Max(t - offset, 0.0);
			double e = EaseOutExpo(k);
			double a = Max(Sin(e * Math::Pi), 0.0);
			color.a = a;
			FontAsset(U"icon")(U"\U000F013F").drawAt(m_pos.movedBy(0, e * -40 + 25), color);
		}
	};

	Vec2 m_pos;
	double m_dur = 0.35;
	Array<BuffIcon> icons;

	BuffEffect(const Vec2& pos, double _dur)
		:m_pos{ pos },
		m_dur{ _dur }
	{
		for (auto i : step(5))
		{
			icons << BuffIcon(m_pos + RandomVec2(30), i * 0.05);
		}
	}

	bool update(double t) override
	{
		double k = t / m_dur;
		for (auto& icon : icons) icon.update(k);
		return (t < m_dur);
	}

};
