#pragma once
#include <Siv3D.hpp>

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
