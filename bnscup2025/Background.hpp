#pragma once
#include <Siv3D.hpp>

class HorizonGrid
{
private:
	double lineT = 0;
	double yStart = Scene::Height() + 100;
	double yEnd = 0;
	Grid<Vec2> points;
	double xEnd = Scene::Width();
	double xStart = 20000;

public:
	HorizonGrid()
		:lineT{ 0 },
		yStart{ Scene::Height() + 100.0 },
		yEnd{ 0 },
		points{ Grid<Vec2>(40, 10) },
		xEnd{ (double)Scene::Width() },
		xStart{ 20000 }
	{
	}

	HorizonGrid(double yEnd, uint32 xDivisionNum, uint32 yDivisionNum)
		:lineT{ 0 },
		yStart{ Scene::Height() + 100.0 },
		yEnd{ yEnd },
		points{ Grid<Vec2>(xDivisionNum, yDivisionNum) },
		xEnd{ (double)Scene::Width() },
		xStart{ 20000 }
	{
	}

	void update()
	{
		lineT = Periodic::Sawtooth0_1(10s);

		for (auto y : step(points.height()))
		{
			// y座標(0.0～1.0)を計算 イージングで3Dっぽい変化に見せてる
			double yRatio = EaseOutQuad((y + lineT) / (points.height()));
			// そのy座標での横線の幅を計算 y座標で0.0～1.0にしたものをxEndとxStartの範囲にMapしてる
			double xWidth = Math::Map(yRatio, 0.0, 1.0, xStart, xEnd);
			// 横幅を計算 求めた横線の幅をGridの列の数で分割
			double width = xWidth / points.width();
			// y座標を計算
			double yPos = yStart - (yRatio * (yStart - yEnd));

			for (auto x : step(points.width()))
			{
				points[y][x].x = Scene::Width() / 2.0 - xWidth / 2.0 + width * x;
				points[y][x].y = yPos - (20 * EaseOutQuint(1 - yRatio));
			}
		}
	}

	void draw(double thick = 1, ColorF color = Palette::White) const
	{
		for (auto y : step(points.height()))
		{
			for (auto x : step(points.width()))
			{
				// 横線
				if (x < points.width() - 1) Line(points[y][x], points[y][x + 1]).draw(thick, color);
				// 縦線
				if (y < points.height() - 1) Line(points[y][x], points[y + 1][x]).draw(thick, color);
			}
		}

		// 地平線
		Line(Vec2(0, yEnd), Vec2(Scene::Width(), yEnd)).draw(thick, color);
	}
};
