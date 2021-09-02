#pragma once

#include "Engine/VoxCore.h"
#include <random>

class QuadTest :
	public vox::Subsystem
{
public:
	QuadTest();

	void onUpdate() override;

	void onAttach() override;

private:
	void print(const vox::KeyEvent& event);

	std::default_random_engine random;
	float max;
	float randFloat();
	uint8_t randByte();
};

