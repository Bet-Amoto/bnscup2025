#include "Category.hpp"
#include "Status.hpp"

Category Categories::Exchange()  // 等価交換
{
	Category cat;
	cat.name = U"等価交換";
	cat.rarity = Rarity::Rare;
	cat.cost = 200;
	cat.description = U"所持金の1/3を消費し、消費した金額の1/2をスコアとして得る。";
	cat.textureKey = U"Exchange";
	cat.type = CategoryType::Lower;

	// プレビュー：⌊ Gold/6 ⌋
	cat.calculateScoreWS = [](const Array<Die>&, const Status& s)->int {
		return (s.gold / 3) / 2;  // = floor(gold / 6)
		};
	// Goldを1/3 消費
	cat.onSelected = [](const Array<Die>&, Status& s) {
		const int spent = s.gold / 3;
		addGold(s, -spent);
		};
	return cat;
}

Category Categories::CashOut()  // 換金（スコア0でゴールド獲得）
{
	Category cat;
	cat.name = U"換金";
	cat.rarity = Rarity::Rare;
	cat.cost = 120;
	cat.description = U"スコアは0。場の出目合計の1/2だけゴールドを獲得する。";
	cat.textureKey = U"CashOut";
	cat.type = CategoryType::Lower;

	cat.calculateScore = [](const Array<Die>&)->int { return 0; };

	// Gold += ⌊(合計)/2⌋
	cat.onSelected = [](const Array<Die>& dices, Status& s) {
		int sum = 0;
		for (const auto& d : dices) if (d.value) sum += *d.value;
		addGold(s, sum / 2);
		};
	return cat;
}
