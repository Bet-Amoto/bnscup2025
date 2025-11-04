#pragma once
#include <Siv3D.hpp>
#include "Rarity.hpp"
#include "ItemBase.hpp"
#include "Effect.hpp"

struct Die;
struct Status;

void addGold(Status& s, int64 amount);

enum class RollOrder {
	PRIMARY = 10,
	MID = 20,
	FINAL = 30
};

// 苦し紛れのグローバル変数
inline PerlinNoise noise{ 0 };

using RollFunc = std::function<int(const Die&, const Array<Die>&)>;
using DrawFunc = std::function<void(const Vec2&, const Die)>;
using AfterRollFunc = std::function<void(Die&, Array<Die>&, Status&)>;
using AddAfterEffectFunc = std::function<void(Die&, const Vec2&, Effect& effect)>;

struct Die : ItemBase
{
	/// @brief 面
	Array<int> faces;
	Optional<int> value = none;
	RollOrder order = RollOrder::PRIMARY;
	RollFunc rollFunc;
	DrawFunc drawFunc;
	AfterRollFunc afterSelfFunc = nullptr;
	AfterRollFunc afterAllFunc = nullptr;
	AddAfterEffectFunc afterSelfEffect = nullptr;
	AddAfterEffectFunc afterAllEffect = nullptr;
	double selfEffectDur = 0.0;
	double allEffectDur = 0.0;

	bool canUnlock = true;
	bool locked = false;
	bool playAfterSelf = false;
	bool playAfterAll = false;

	Optional<int> displayValue = none;
	bool isSpinning = false;
	Stopwatch flickSw{ StartImmediately::No };

	String itemType() const override { return U"ダイス"; }

	bool apply(Status& s) override { return true; }

	std::shared_ptr<ItemBase> clone() const override
	{
		return std::make_shared<Die>(*this);
	}

	void drawIcon(const Vec2& pos) const override
	{
		//仮
		Die die = *this;
		die.value = 1;
		if (die.faces.isEmpty())die.value = 1;
		else die.value = die.faces.sorted().front();
		die.displayValue = die.value;
		drawFunc(pos, die);
	}

	void beginSpin()
	{
		if (locked) return;
		isSpinning = true;
		flickSw.restart();
		displayValue = faces.isEmpty() ? Random(1, 6) : faces.choice();
		playAfterSelf = false;
		playAfterAll = false;
	}

	void updateSpin()
	{
		if (!isSpinning || locked) return;
		if (flickSw.sF() >= 0.05)
		{
			displayValue = faces.isEmpty() ? Random(1, 6) : faces.choice();
			flickSw.restart();
		}
	}

	void stopSpin(Array<Die>& dices, Status& status)
	{
		if (locked) return;
		isSpinning = false;
		flickSw.reset();
		value = rollFunc(*this, dices);
		displayValue = value;
		// afterSelfFuncはDiceBoxのスケジューラが行う
		// if (afterSelfFunc) afterSelfFunc(*this, dices, status);
	}

	void roll(Array<Die>& dices, Status& status)
	{
		if (locked) return;
		value = rollFunc(*this, dices);
		if (afterSelfFunc)
		{
			afterSelfFunc(*this, dices, status);
		}
		displayValue = value;
	}
	void clear()
	{
		value = none;
		locked = false;
	}
	void draw(const Vec2& centerPos) const
	{
		drawFunc(centerPos, *this);
	}
	void setMinVal() {
		if (faces.isEmpty())value = 1;
		else value = faces.sorted().front();
	}
};

namespace Dice{
	/// @brief 通常のダイス
	inline Die StandardDie()
	{
		Die d;
		d.name = U"新ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"新品のダイス。1～6の普通のダイス。" + faceText;
		d.textureKey = U"StandardDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		return d;
	}

	inline Die SleepDie()
	{
		Die d;
		d.name = U"寝ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.faces = Array<int>{ 2, 3, 4, 5, 6, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"寝ているダイス。高い出目を持つが、一度振ると寝てしまいリロールをすることができない。" + faceText;
		d.textureKey = U"SleepDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(264, 0.45, 0.66) : HSV(264, 0.45, 0.96));
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				FontAsset(U"iconL")(U"\U000F0904").drawAt(faceRect.center(), (self.locked || !self.value) ? HSV(49, 0.79, 0.69) : HSV(49, 0.79, 0.99));
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				self.locked = true;
			};
		d.selfEffectDur = 0.30;
		d.afterSelfEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<SleepEffect>(pos, self.selfEffectDur);
			};

		d.canUnlock = false;

		return d;
	}

	inline Die HeartDie()
	{
		Die d;
		d.name = U"心ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"心を通わせているダイス。場にある心ダイスの数÷2の値だけ出目が高くなる。" + faceText;
		d.textureKey = U"HeartDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });

				const Spline2D sp1
				{
					{ centerPos.movedBy(-30, -5), centerPos.movedBy(-25, -7), centerPos.movedBy(-20, -5),
					  centerPos.movedBy(-10, 5), centerPos.movedBy(-5, 7), centerPos.movedBy(0, 5),
					  centerPos.movedBy(10, -5), centerPos.movedBy(15, -7), centerPos.movedBy(20, -5),
					  centerPos.movedBy(30, 5) }
				};

				const Spline2D sp2
				{
					{ centerPos.movedBy(-30, 5), centerPos.movedBy(-25, 7), centerPos.movedBy(-20, 5),
					  centerPos.movedBy(-10, -5), centerPos.movedBy(-5, -7), centerPos.movedBy(0, -5),
					  centerPos.movedBy(10, 5), centerPos.movedBy(15, 7), centerPos.movedBy(20, 5),
					  centerPos.movedBy(30, -5) }
				};

				sp1.draw(1, ColorF(0));
				sp2.draw(1, ColorF(0));

				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterAllFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				int32 heartNum = 0;

				for (auto& dice : dices)
				{
					if (dice.name == U"心ダイス") heartNum++;
				}

				if (self.value && !self.locked) self.value.value() += heartNum / 2;
				self.displayValue = self.value;
			};
		d.allEffectDur = 0.35;
		d.afterAllEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<BuffEffect>(pos, self.allEffectDur);
			};

		return d;
	}

	inline Die MirageDie()
	{
		Die d;
		d.name = U"蜃ダイス";
		d.rarity = Rarity::Rare;
		d.cost = 50;
		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"蜃気楼が写るダイス。場の最も左にあるダイスの目をコピーする。" + faceText;
		d.textureKey = U"MirageDice";

		d.value = none;
		d.order = RollOrder::FINAL;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				for (int y : step(12))
				{
					for (int x : step(12))
					{
						double color = noise.octave2D0_1(x / 10.0, y / 10.0, 4);
						color = Math::Lerp(0.7, 1.0, color);
						color -= (self.locked || !self.value) ? 0.2 : 0;
						RectF{ centerPos.movedBy(-30 + x * 5, -30 + y * 5), 5 }.draw(ColorF(color));
					}
				}
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (dices.front().value) self.value = dices.front().value;
				self.displayValue = self.value;
			};

		d.selfEffectDur = 0.35;
		d.afterSelfEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<MirrorEffect>(pos, self.selfEffectDur);
			};

		return d;
	}

	inline Die QuakeDie()
	{
		Die d;
		d.name = U"震ダイス";
		d.rarity = Rarity::Epic;
		d.cost = 200;
		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"震えるダイス。振った後、ロックされていない全てのダイスをもう一度振る。" + faceText;
		d.textureKey = U"QuakeDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(16, 0.3, 0.34) : HSV(16, 0.3, 0.54));
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				Shape2D::NStar(20, 30, 25, centerPos).draw((self.locked || !self.value) ? HSV(16, 0.4, 0.54) : HSV(16, 0.4, 0.74));
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterAllFunc = [](Die& self, Array<Die>& dices, Status& status)
			{

				for (auto& t : dices)
				{
					if (&t == &self) continue;
					if (t.name == U"震ダイス") continue;
					if (t.locked) continue;
					t.roll(dices, status);
				}
			};
		d.allEffectDur = 0.35;
		d.afterAllEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<QuakeEffect>(pos, self.allEffectDur);
			};


		return d;
	}

	inline Die GoldDie()
	{
		Die d;
		d.name = U"金ダイス";
		d.rarity = Rarity::Epic;
		d.cost = 200;
		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"黄金に輝くダイス。出た目と同じ値のゴールドを獲得する。" + faceText;
		d.textureKey = U"GoldDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (self.value) addGold(status, self.value.value());
			};

		d.selfEffectDur = 0.35;
		d.afterSelfEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<GoldPopEffect>(pos, self.value.value(), self.selfEffectDur);
			};

		return d;
	}

	inline Die Coin()
	{
		Die d;
		d.name = U"コイン";
		d.rarity = Rarity::Common;
		d.cost = 50;
		d.faces = Array<int>{ 1, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"普通のコイン。二分の一の確率で1か6が出る。" + faceText;
		d.textureKey = U"Coin";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				Circle faceCircle(centerPos, 30);
				faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceCircle.drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
			};

		return d;
	}

	inline Die GoldCoin()
	{
		Die d;
		d.name = U"金コイン";
		d.rarity = Rarity::Rare;
		d.cost = 100;
		d.faces = Array<int>{ 0, 5 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"黄金に輝くコイン。二分の一の確率で0か5が出る。出た目と同じ値のゴールドを獲得する。" + faceText;
		d.textureKey = U"GoldCoin";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				Circle faceCircle(centerPos, 30);
				faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceCircle.drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (self.value) addGold(status, self.value.value());
			};
		d.selfEffectDur = 0.35;
		d.afterSelfEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<GoldPopEffect>(pos, self.value.value(), self.selfEffectDur);
			};

		return d;
	}

	inline Die GodDie()
	{
		Die d;
		d.name = U"神ダイス";
		d.rarity = Rarity::Legendary;
		d.cost = 1000;
		d.faces = Array<int>{ 6, 6, 6 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"神が作り出したダイス。全てのダイスの出目を2倍にする。" + faceText;
		d.textureKey = U"GodDice";
		d.isUnique = true;

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				Shape2D::NStar(20, 30, 25, centerPos).draw((self.locked || !self.value) ? HSV(64, 0.54, 0.8) : HSV(64, 0.54, 1));
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{0.1});
			};

		d.afterAllFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				for (auto& die : dices)
				{
					if (die.value && !die.locked) die.value.value() *= 2;
					die.displayValue = die.value;
				}
			};
		d.allEffectDur = 0.35;
		d.afterAllEffect = [](Die& self, const Vec2& pos, Effect& effect)
			{
				effect.add<GodEffect>(pos, self.allEffectDur);
			};

		return d;
	}

	inline Die AbyssDie()
	{
		Die d;
		d.name = U"深ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.faces = Array<int>{ 0, 1, 1, 2, 2, 3 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"非常に深いダイス。ダイスの出目が低い。" + faceText;
		d.textureKey = U"AbyssDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				Circle faceCircle(centerPos, 29);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(199, 0.31, 0.3) : HSV(199, 0.31, 0.5));
				faceCircle.draw((self.locked || !self.value) ? HSV(0, 0, 0.8) : HSV(0, 0, 1));
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		return d;
	}

	inline Die SinkDie()
	{
		Die d;
		d.name = U"沈ダイス";
		d.rarity = Rarity::Rare;
		d.cost = 50;
		d.faces = Array<int>{ -2, -1, 0, 1, 2, 3 };
		String faceText = U"\n出る目:" + Format(d.faces);
		d.description = U"底に沈んだダイス。ダイスの出目が非常に低い。" + faceText;
		d.textureKey = U"SinkDice";

		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				//SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(2).draw((self.locked || !self.value) ? HSV(176, 0.11, 0.57) : HSV(176, 0.11, 0.77));
				FontAsset(U"iconL")(U"\U000F1978").drawAt(50, centerPos);
				faceRect.rounded(2).drawFrame(1, ColorF{ 0 });
				if (self.displayValue)FontAsset(U"Bold")(Format(self.displayValue.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		return d;
	}

	const Array<Die> AllDice = {
		StandardDie(),
		SleepDie(),
		HeartDie(),
		MirageDie(),
		GoldDie(),
		QuakeDie(),
		Coin(),
		GoldCoin(),
		GodDie(),
		AbyssDie(),
		SinkDie(),
	};
}
