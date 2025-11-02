#include "Item.hpp"
#include "ItemBase.hpp"
#include "Status.hpp"
namespace Items
{
	Artifact TrueOne()
	{
		Artifact item;
		item.name = U"真・エース";
		item.rarity = Rarity::Legendary;
		item.cost = 500;
		item.description = U"エースの役のスコアが10倍になる";
		item.textureKey = U"TrueOne";
		item.isUnique = true; // ユニークアイテムとして設定
		item.timing = ActivationTiming::OnDiceResult;
		item.usageLimit = none;

		// 予想スコア計算時の処理
		item.scoreModifier = [](int baseScore, const Category& category, const Array<Die>& dices, const Status& status) -> int
			{
				if (category.name == U"エース") {
					return baseScore * 10;
				}
				return baseScore;
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(40, 0.9, 1.0));
				TextureAsset(U"Emoji_Star").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact Siso()
	{
		Artifact item;
		item.name = U"シソ";
		item.rarity = Rarity::Epic;
		item.cost = 500;
		item.description = U"青じそ。すべての役のスコアが1.2倍になる";
		item.timing = ActivationTiming::OnDiceResult;
		item.usageLimit = none;

		// 予想スコア計算時の処理
		item.scoreModifier = [](int baseScore, const Category& category, const Array<Die>& dices, const Status& status) -> int
			{
				baseScore *= 1.2;
				return baseScore;
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(40, 0.9, 1.0));
				TextureAsset(U"Emoji_yasai").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact LuckyCharm()
	{
		Artifact item;
		item.name = U"幸運のお守り";
		item.rarity = Rarity::Rare;
		item.cost = 80;
		item.description = U"幸運を呼ぶお守り。ショップでRare以上のアイテムが出る確率をあげる";
		item.textureKey = U"LuckyCharm";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = none; // 無制限

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				status.rarityWeightRare *= 1.2;
				status.rarityWeightEpic *= 1.3;
				status.rarityWeightLegendary *= 1.5;
				status.updateDistribution();

			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(160, 0.5, 0.9));
				TextureAsset(U"Emoji_Clover").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact ArtifactExpander()
	{
		Artifact item;
		item.name = U"アーティファクト入荷";
		item.rarity = Rarity::Rare;
		item.cost = 100;
		item.description = U"ショップに並ぶアーティファクトが1つ増える";
		item.textureKey = U"ArtifactExpander";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; // 1回使い切り
		item.isUnique = true;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				status.ShopArtifactCount++;
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(280, 0.6, 0.9));
				TextureAsset(U"Emoji_Package").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact DieExpander()
	{
		Artifact item;
		item.name = U"ダイス入荷";
		item.rarity = Rarity::Rare;
		item.cost = 100;
		item.description = U"ショップに並ぶダイスが1つ増える";
		item.textureKey = U"DieExpander";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; // 1回使い切り
		item.isUnique = true;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				status.ShopDiceCount++;
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(280, 0.6, 0.9));
				TextureAsset(U"Emoji_Package").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact CategoryExpander()
	{
		Artifact item;
		item.name = U"役入荷";
		item.rarity = Rarity::Rare;
		item.cost = 100;
		item.description = U"ショップに並ぶ役が1つ増える";
		item.textureKey = U"CategoryExpander";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; // 1回使い切り
		item.isUnique = true;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				status.ShopCategoryCount++;
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(280, 0.6, 0.9));
				TextureAsset(U"Emoji_Package").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact AddDie()
	{
		Artifact item;
		item.name = U"ダイス増し";
		item.rarity = Rarity::Legendary;
		item.cost = 100;
		item.description = U"1つ多くのダイスを振れるようになる";
		item.textureKey = U"CategoryExpander";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; 
		item.isUnique = true;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				status.dices << Dice::StandardDie();
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(280, 0.6, 0.9));
				TextureAsset(U"Emoji_Package").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact InTopForm() {
		Artifact item;
		item.name = U"絶好調";
		item.rarity = Rarity::Epic;
		item.cost = 200;
		item.description = U"ダイスの目が5つ以上同じである場合、ダイスをふれる回数が1回増える";
		item.textureKey = U"InTopForm";
		item.timing = ActivationTiming::OnDiceResult;
		item.usageLimit = none; // 無制限
		item.isUnique = true;
		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				for (auto& die : status.dices)
				{
					const int val = die.displayValue.value_or(0);
					const int count = status.dices.count_if([val](const Die& d) { return d.displayValue.value_or(0) == val; });
					if (count >= 5) {
						status.selectionsLeft += 1;
						break;
					}
				}
			};
		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(40, 0.9, 1.0));
				TextureAsset(U"Emoji_Fire").resized(40).drawAt(pos);
			};
		return item;
	}

	Artifact Diversity() {
		Artifact item;
		item.name = U"ダイバーシティ";
		item.rarity = Rarity::Rare;
		item.cost = 200;
		item.description = U"ダイスの種類1つにつき、スコアが+0.3倍される";
		item.timing = ActivationTiming::OnDiceResult;
		item.usageLimit = none;
		item.isUnique = true;
		// 予想スコア計算時の処理
		item.scoreModifier = [](int baseScore, const Category& category, const Array<Die>& dices, const Status& status) -> int
			{
				HashSet<String> diceName;
				for (const auto& d : dices) {
					diceName.insert(d.name);
				}
				return baseScore * (1.0 + diceName.size() * 0.3);
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(40, 0.9, 1.0));
				TextureAsset(U"Emoji_Handshake").resized(40).drawAt(pos);
			};

		return item;
	}

	Artifact TimeSale() {
		Artifact item;
		item.name = U"タイムセール";
		item.rarity = Rarity::Common;
		item.cost = 100;
		item.description = U"ショップに並ぶ商品のうち、ランダムに1つ30%OFFされる。";
		item.textureKey = U"TimeSale";
		item.timing = ActivationTiming::OnShopRoll;
		item.usageLimit = none;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				if (status.merchandises.size() == 0) return;
				const auto index = Random((unsigned long long)0, status.merchandises.size() - 1);
				status.merchandises[index].getItem()->cost = static_cast<int>(status.merchandises[index].getItem()->cost * 0.7);

			};
		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(50, 0.8, 0.9));
				TextureAsset(U"Emoji_Clock").resized(40).drawAt(pos);
			};
		return item;
	}

	Artifact Relatives() {
		Artifact item;
		item.name = U"親族";
		item.rarity = Rarity::Common;
		item.cost = 100;
		item.description = U"ショップをリロールしたとき、たまにお小遣いをくれる。";
		item.textureKey = U"TimeSale";
		item.timing = ActivationTiming::OnShopRoll;
		item.usageLimit = none;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing) return;
				if (RandomBool(0.75))return;
				status.merchandises << Merchandise{ GoldPouch().clone(),  status.calcMerchPos(status.merchandises.size())};

			};
		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos.movedBy(-30, -30), 60, 60, 5 }.draw(HSV(50, 0.8, 0.9));
				TextureAsset(U"Emoji_Clock").resized(40).drawAt(pos);
			};
		return item;
	}
}
