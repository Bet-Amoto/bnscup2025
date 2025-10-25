# pragma once
#include "Common.hpp"
#include "Status.hpp"

class Shop : public App::Scene
{
public:
	Shop(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Status& m_status = getData().status;
};
