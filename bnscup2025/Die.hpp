#pragma once
#include <Siv3D.hpp>
#include "Rarity.hpp"
#include "ItemBase.hpp"

struct Die;
struct Status;

void addGold(Status& s, int32 amount);

enum class RollOrder {
	PRIMARY = 10,
	MID = 20,
	FINAL = 30
};


using RollFunc = std::function<int(const Die&, const Array<Die>&)>;
using DrawFunc = std::function<void(const Vec2&, const Die)>;
using AfterRollFunc = std::function<void(Die&, Array<Die>&, Status&)>;

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

	bool canUnlock = true;
	bool locked = false;

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
		drawFunc(pos, die);
	}

	void roll(Array<Die>& dices, Status& status)
	{
		if (locked) return;
		value = rollFunc(*this, dices);
		if (afterSelfFunc)
		{
			afterSelfFunc(*this, dices, status);
		}
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
		d.description = U"新品のダイス。1～6の普通のダイス。";
		d.textureKey = U"StandardDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		return d;
	}

	inline Die SleepDie()
	{
		Die d;
		d.name = U"寝ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.description = U"寝ているダイス。高い出目を持つが、一度振ると寝てしまいリロールをすることができない。";
		d.textureKey = U"SleepDice";

		d.faces = Array<int>{ 2, 3, 4, 5, 6, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				self.locked = true;
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
		d.description = U"心を通わせているダイス。場にある心ダイスの数÷2の値だけ出目が高くなる。";
		d.textureKey = U"HeartDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterAllFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				int32 heartNum = 0;

				for (auto& dice : dices)
				{
					if (dice.name == U"心ダイス") heartNum++;
				}

				if (self.value && !self.locked) self.value.value() += heartNum / 2;
			};

		return d;
	}

	inline Die MirageDie()
	{
		Die d;
		d.name = U"蜃ダイス";
		d.rarity = Rarity::Rare;
		d.cost = 50;
		d.description = U"蜃気楼が写るダイス。場の最も左にあるダイスの目をコピーする。";
		d.textureKey = U"MirageDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::FINAL;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (dices.front().value) self.value = dices.front().value;
			};

		return d;
	}

	inline Die QuakeDie()
	{
		Die d;
		d.name = U"震ダイス";
		d.rarity = Rarity::Epic;
		d.cost = 200;
		d.description = U"震えるダイス。振った後、ロックされていない全てのダイスをもう一度振る。";
		d.textureKey = U"QuakeDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
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

		return d;
	}

	inline Die GoldDie()
	{
		Die d;
		d.name = U"金ダイス";
		d.rarity = Rarity::Epic;
		d.cost = 200;
		d.description = U"黄金に輝くダイス。出た目と同じ値のゴールドを獲得する。";
		d.textureKey = U"GoldDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (self.value) addGold(status, self.value.value());
			};

		return d;
	}

	inline Die Coin()
	{
		Die d;
		d.name = U"コイン";
		d.rarity = Rarity::Common;
		d.cost = 50;
		d.description = U"普通のコイン。二分の一の確率で1か6が出る。";
		d.textureKey = U"Coin";

		d.faces = Array<int>{ 1, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				Circle faceCircle(centerPos, 30);
				faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceCircle.drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
			};

		return d;
	}

	inline Die GoldCoin()
	{
		Die d;
		d.name = U"金コイン";
		d.rarity = Rarity::Rare;
		d.cost = 100;
		d.description = U"黄金に輝くコイン。二分の一の確率で0か5が出る。出た目と同じ値のゴールドを獲得する。";
		d.textureKey = U"GoldCoin";

		d.faces = Array<int>{ 0, 5 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				SimpleGUI::GetFont()(self.name).drawAt(centerPos.x, centerPos.y - 45, ColorF(0));
				Circle faceCircle(centerPos, 30);
				faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceCircle.drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
			};

		d.afterSelfFunc = [](Die& self, Array<Die>& dices, Status& status)
			{
				if (self.value) addGold(status, self.value.value());
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
	};
}
