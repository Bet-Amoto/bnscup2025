#include "DiceBox.hpp"

DiceBox::DiceBox(const Vec2& centerPos, Array<Die>& dice)
	: m_position(centerPos), m_dice(dice), m_clickedDie(nullptr)
{
}

void DiceBox::roll(Status& status)
{
	Array<Die*> dice;
	for (auto& die : m_dice) {
		dice << &die;
	}
	dice.sort_by([](const Die* a, const Die* b) {
		return static_cast<int>(a->order) < static_cast<int>(b->order);
	});

	if (m_isRolling) return;
	m_isRolling = true;
	m_rollSw.restart();
	m_stopAt.clear();
	m_stopAt.resize(m_dice.size(), -1.0);
	m_startSelfAt.clear();
	m_startSelfAt.resize(m_dice.size(), -1.0);
	m_startAllAt.clear();
	m_startAllAt.resize(m_dice.size(), -1.0);

	for (auto& die : dice)
	{
		if (!die->locked) die->beginSpin();
	}

	m_stopAt[0] = m_startStopping;
	m_startSelfAt[0] = m_startStopping + m_step;
	for (size_t i = 0; i < m_dice.size() - 1; i++)
	{
		m_stopAt[i + 1] = m_stopAt[i] + m_step + m_dice[i].selfEffectDur;
		m_startSelfAt[i + 1] = m_stopAt[i + 1] + m_step;
	}
	m_endSelfEffect = m_startSelfAt.back() + m_dice.back().selfEffectDur;

	m_startAllAt[0] = m_endSelfEffect;
	for (size_t i = 0; i < m_dice.size() - 1; i++)
	{
		m_startAllAt[i + 1] = m_startAllAt[i] + m_dice[i].allEffectDur;
	}
	m_endAllEffect = m_startAllAt.back() + m_dice.back().allEffectDur;

	//for (auto* die : dice) {
	//	die->roll(m_dice, status);
	//}
	//for (auto* die : dice)
	//{
	//	if (die->afterAllFunc && !die->locked)
	//	{
	//		die->afterAllFunc(*die, m_dice, status);
	//	}
	//}
}

void DiceBox::draw() const
{
	const double basePosX = m_position.x - m_dice.size() * (faceSize + 10) / 2;
	for (size_t i = 0; i < m_dice.size(); ++i)
	{
		const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		m_dice[i].draw(box.center());
	}
	m_effect.update();
}

void DiceBox::draw(const Rect& drawArea) const
{
	const double basePosX = m_position.x - m_dice.size() * (faceSize + 10) / 2;
	for (size_t i = 0; i < m_dice.size(); ++i)
	{
		const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		m_dice[i].draw(box.center());
	}
}

void DiceBox::update(Status& status)
{
	m_clickedDie = nullptr;
	m_hoveredDie = nullptr;
	const double basePosX = m_position.x - m_dice.size() * (faceSize + 10) / 2;
	for (const auto i : step(m_dice.size()))
	{
		const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
		if (box.leftClicked() && !m_isRolling)
		{
			m_clickedDie = &m_dice[i];
		}
		if(box.mouseOver())
		{
			m_hoveredDie = &m_dice[i];
			GetExplanation().setItem(m_hoveredDie);
		}
	}

	if (!m_isRolling) return;

	for (auto& die : m_dice) die.updateSpin();

	bool allStopped = true;
	for (size_t i = 0; i < m_dice.size(); i++)
	{
		if (m_stopAt[i] < 0)
		{
			allStopped &= (!m_dice[i].isSpinning);
			continue;
		}
		if (m_rollSw.sF() >= m_stopAt[i] && m_dice[i].isSpinning)
		{
			m_dice[i].stopSpin(m_dice, status);
			const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
			m_effect.add<StopDiceEffect>(box.center());
			m_stopAt[i] = -1.0;
		}
		allStopped &= (!m_dice[i].isSpinning);
	}

	for (size_t i = 0; i < m_dice.size(); i++)
	{
		if (m_startSelfAt[i] >= 0.0 && m_rollSw.sF() >= m_startSelfAt[i] && !m_dice[i].playAfterSelf
			&& !m_dice[i].locked)
		{
			if (m_dice[i].afterSelfFunc)
			{
				m_dice[i].afterSelfFunc(m_dice[i], m_dice, status);
			}
			if (m_dice[i].afterSelfEffect)
			{
				const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
				m_dice[i].afterSelfEffect(m_dice[i], box.center(), m_effect);
			}
			m_dice[i].playAfterSelf = true;
			m_startSelfAt[i] = -1.0;
		}
	}

	if (allStopped && m_rollSw.sF() >= m_endSelfEffect)
	{
		for (size_t i = 0; i < m_dice.size(); i++)
		{
			if (m_startAllAt[i] >= 0.0 && m_rollSw.sF() >= m_startAllAt[i] && !m_dice[i].playAfterAll
				&& !m_dice[i].locked)
			{
				if (m_dice[i].afterAllFunc)
				{
					m_dice[i].afterAllFunc(m_dice[i], m_dice, status);
				}
				if (m_dice[i].afterAllEffect)
				{
					const RectF box{ basePosX + i * (faceSize + 10), m_position.y, faceSize, faceSize };
					m_dice[i].afterAllEffect(m_dice[i], box.center(), m_effect);
				}
				m_dice[i].playAfterAll = true;
				m_startAllAt[i] = -1.0;
			}
		}
	}

	if (allStopped && m_rollSw.sF() >= m_endAllEffect)
	{
		m_isRolling = false;
		m_rollSw.reset();
	}
}
