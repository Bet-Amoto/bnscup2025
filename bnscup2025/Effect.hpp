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
