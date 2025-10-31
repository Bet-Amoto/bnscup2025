#include "Shop.hpp"

Shop::Shop(const InitData& init)
	: IScene{ init },
	m_status{ getData().status },
	m_diceBox{ Vec2{ 100, 300 }, m_status.dices }
{
	Scene::SetBackground(ColorF{ 0.9, 0.9, 0.8 });
	reroll();
	m_status.ShopRerollPrice = m_status.ShopRerollBasePrice;
	for (auto& die : m_status.dices) {
		die.setMinVal();
	}
	m_status.gold = 10000;
}

void Shop::update()
{
	if (KeySpace.down())
	{
		changeScene(State::Game);
	}

	if(m_holdedItem)
	{
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(240,60) };
		const auto item = m_holdedItem->getItem();
		if(item && item->itemType() == U"ダイス")
		{
			m_diceBox.update(getData().status);
			const auto clickedDie = m_diceBox.getClickedDie();
			if (clickedDie)
			{
				m_selected = true;
				for(auto& die : m_status.dices)
				{
					die.locked = (&die != clickedDie);
				}
			}
			if(boughtButtonRect.leftClicked() && m_selected)
			{
				for (auto& die : m_status.dices)
				{
					if (!die.locked) {
						die = *std::dynamic_pointer_cast<Die>(item->clone());
						die.setMinVal();
						for(auto& d : m_status.dices)
						{
							d.locked = false;
						}
						m_status.gold -= item->cost;
						m_holdedItem->setSoldOut(true);
						break;
					}
				}
				m_holdedItem = nullptr;
			}
			
		}
		else if(item && item->itemType() == U"役")
		{
			categorySelect();
			if (boughtButtonRect.leftClicked() && selectedCategory) {
				*selectedCategory = *std::dynamic_pointer_cast<Category>(item->clone());
				m_status.gold -= item->cost;
				m_holdedItem->setSoldOut(true);
				m_holdedItem = nullptr;
				clearSelect();
			}
		}

		if(cancelButtonRect.leftClicked())
		{
			m_holdedItem = nullptr;
			clearSelect();
		}
		return;
	}

	for(auto& merch : m_merchandises)
	{
		merch.update();
		if (merch.isClicked() && !merch.isSoldOut())
		{
			const auto item = merch.getItem();
			if (item)
			{
				if (item->itemType() == U"アイテム" && m_status.gold >= item->cost)
				{
					Artifact artifact = *std::dynamic_pointer_cast<Artifact>(item->clone());
					m_status.artifacts.push_back(artifact);
					artifact.apply(m_status);
					m_status.gold -= item->cost;
					merch.setSoldOut(true);
				}
				else if (m_status.gold >= item->cost)
				{
					m_holdedItem = &merch;
					m_selected = false;
				}
			}
		}
	}

	if (RerollButtonRect.leftClicked() && m_status.gold > m_status.ShopRerollPrice) {
		addGold(m_status, -m_status.ShopRerollPrice);
		m_status.ShopRerollPrice += m_status.ShopRerollPriceIncrease;
		reroll();
	}
	if (NextTurnButtonRect.leftClicked()) {
		changeScene(State::Game);
	}
}

void Shop::draw() const
{
	FontAsset(U"Bold")(U"ショップ").drawAt(Scene::CenterF().x, 50, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"所持: {}G"_fmt(m_status.gold)).drawAt(40,Scene::CenterF().x + 300, 50, ColorF{ 0.1 });
	for (const auto& merch : m_merchandises)
	{
		merch.draw();
	}

	const ColorF rerollButtonCol = m_status.gold > m_status.ShopRerollPrice ? ColorF{ 0.9 } : ColorF{ 0.5 };
	RerollButtonRect.rounded(5).draw(rerollButtonCol).drawFrame(3, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"更新").drawAt(28, RerollButtonRect.center().movedBy(0,-50), ColorF{0.1});
	FontAsset(U"Bold")(U"{}G"_fmt(m_status.ShopRerollPrice)).drawAt(32, RerollButtonRect.center(), ColorF{0.1});
	FontAsset(U"Bold")(U"次のターンへ").drawAt(40, NextTurnButtonRect.center(), ColorF{ 0.1 });
	if (m_holdedItem) {
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(240,60) };

		Rect{ 0,0,Scene::Size() }.draw(ColorF{ 0, 0, 0, 0.5 });
		viewportRect.rounded(10).draw(ColorF{ 1.0 }).drawFrame(3, ColorF{ 0.1 });
		{
			const ScopedViewport2D viewport{ viewportRect };
			boughtButtonRect.rounded(5).draw(ColorF{ 0.2, 0.8, 0.2 });
			FontAsset(U"Bold")(U"{}G"_fmt(m_holdedItem->getItem()->cost)).drawAt(32, boughtButtonRect.center(), ColorF{ 1.0 });
			cancelButtonRect.rounded(5).draw(ColorF{ 0.8, 0.2, 0.2 });
			FontAsset(U"Bold")(U"キャンセル").drawAt(32, cancelButtonRect.center(), ColorF{ 1.0 });

			const auto item = m_holdedItem->getItem();
			if (item && item->itemType() == U"ダイス")
			{
				m_diceBox.draw(viewportRect.stretched(-10));
			}
			if(item && item->itemType() == U"役")
			{
				categorySelectDraw();
			}
			GetExplanation().draw(Cursor::PosF(), viewportRect.stretched(-10));
		}
	}
	else
	{
		GetExplanation().draw(Cursor::PosF());
	}

}

void Shop::reroll() {
	int marchX = 100;
	m_merchandises.clear();
	for (const auto i : step(m_status.ShopDiceCount))
	{
		const Rarity rarity = DiscreteSample(AllRarities, m_status.distribution);
		const Array<Die> filteredDices = m_status.availableDices.filter([&](const Die& item) {
			if (item.rarity != rarity) return false;

			if (!item.isUnique) return true;
			for (const auto& owned : m_status.artifacts)
			{
				if (owned.name == item.name)
				{
					return false;
				}
			}
			return true;
		});
		if (not filteredDices.empty())
		{
			const Die dice = filteredDices.choice();
			m_merchandises << Merchandise{ dice.clone(), Vec2{ marchX, 200 } };
			marchX += 150;
		}
	}
	for (const auto i : step(m_status.ShopCategoryCount))
	{
		const Rarity rarity = DiscreteSample(AllRarities, m_status.distribution);
		const Array<Category> filteredCategories = m_status.availableCategories.filter([&](const Category& item) {
			if (item.rarity != rarity) return false;

			if (!item.isUnique) return true;
			for (const auto& owned : m_status.artifacts)
			{
				if (owned.name == item.name)
				{
					return false;
				}
			}
			return true;
		});
		if (not filteredCategories.empty())
		{
			const Category category = filteredCategories.choice();
			m_merchandises << Merchandise{ category.clone(), Vec2{ marchX, 200 } };
			marchX += 150;
		}
	}
	for (const auto i : step(m_status.ShopArtifactCount)) {
		const Rarity rarity = DiscreteSample(AllRarities, m_status.distribution);
		
		// ユニークアイテムを所持している場合は除外する
		const Array<Artifact> filteredItems = m_status.availableArtifacts.filter([&](const Artifact& item){
			if (item.rarity != rarity) return false;
			
			if (!item.isUnique) return true;
			for (const auto& owned : m_status.artifacts)
			{
				if (owned.name == item.name)
				{
					return false;
				}
			}
			return true;
		});
		
		if (not filteredItems.empty())
		{
			const Artifact item = filteredItems.choice();
			m_merchandises << Merchandise{ item.clone(), Vec2{ marchX, 200 } };
			marchX += 150;
		}
	}
}

void Shop::categorySelect() {
	const auto item = m_holdedItem->getItem();
	const Category holdedCategory = *std::dynamic_pointer_cast<Category>(item->clone());

	if (holdedCategory.type == CategoryType::Lower)
	{
		for (int i : step(m_status.lowerCategories.size())) {
			const Rect selectBox = Rect(150, 100 + i * (categorySelectRectSize.y), categorySelectRectSize.x, categorySelectRectSize.y);
			if (selectBox.mouseOver()) {
				GetExplanation().setItem(&m_status.lowerCategories[i]);
			}
			if (selectBox.leftClicked()) {
				selectedCategory = &m_status.lowerCategories[i];
			}
		}
	}
	else
	{
		for (int i : step(m_status.upperCategories.size())) {
			const Rect selectBox = Rect(150, 100 + i * (categorySelectRectSize.y), categorySelectRectSize.x, categorySelectRectSize.y);
			if (selectBox.mouseOver()) {
				GetExplanation().setItem(&m_status.upperCategories[i]);
			}
			if (selectBox.leftClicked()) {
				selectedCategory = &m_status.upperCategories[i];
			}
		}
	}
}

void Shop::categorySelectDraw() const {
	const auto item = m_holdedItem->getItem();
	const Category holdedCategory = *std::dynamic_pointer_cast<Category>(item->clone());

	// 直接m_statusのカテゴリーを参照
	const Array<Category>& categorys = (holdedCategory.type == CategoryType::Lower)
		? m_status.lowerCategories
		: m_status.upperCategories;

	for (int i : step(categorys.size())) {
		FontAsset(U"Category")(categorys[i].name).draw(28, 180, 100 + i * (categorySelectRectSize.y), ColorF{ 0.1 });
		Circle(165, 100 + i * (categorySelectRectSize.y) + categorySelectRectSize.y / 2, 8).drawFrame(2, ColorF{ 0 });
		if (selectedCategory == &categorys[i]) {
			const double x = FontAsset(U"Category")(categorys[i].name).region(28, 180, 100 + i * (categorySelectRectSize.y)).rightX();
			const double y = 100 + i * (categorySelectRectSize.y) + categorySelectRectSize.y / 2;
			Shape2D::Arrow(Vec2{ x + 10,y }, Vec2{ x + 60, y }, 12, Vec2{ 17,20 }).draw(ColorF{ 1.0,0.1,0.1 });
			FontAsset(U"Category")(holdedCategory.name).draw(28, x + 70, 100 + i * (categorySelectRectSize.y), ColorF{ 1.0,0.1,0.1 });
			Circle(165, y, 5).draw(ColorF{ 1.0,0.1,0.1 });
		}
	}
}
void Shop::clearSelect() {
	selectedCategory = nullptr;
	m_selected = false;
	for (auto& die : m_status.dices)
	{
		die.locked = false;
	}
}
