#pragma once
# include <Siv3D.hpp>
# include "Common.hpp"
# include "Status.hpp"

// ゲームオーバーシーン
class GameOver : public App::Scene
{
public:

	GameOver(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Status& status;
};
