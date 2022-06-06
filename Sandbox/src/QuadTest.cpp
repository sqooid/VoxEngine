#include "voxpch.h"

#include "QuadTest.h"

#include "Engine/VoxCore.h"
#include "Engine/Util/ObjLoader.h"
#include <glm/glm.hpp>

QuadTest::QuadTest()
{
	max = random.max();
}

void QuadTest::onUpdate()
{
	vox::Renderer* renderer = dynamic_cast<vox::Renderer*>(getSub(vox::SubType::Renderer));

	int triCount = m_LoadedMesh->size() / 11;
	uint32_t* ptr = &(*m_LoadedMesh)[0];
	for (int i = 0; i < triCount; ++i)
	{
		renderer->batchTriangle(ptr);
		ptr += 11;
	}
}

void QuadTest::onAttach()
{
	getEventHandler()->subscribeEvent<vox::KeyEvent>(EVENT_CALLBACK(print));

	vox::ObjLoader loader;
	loader.loadFile("assets/magica-export.obj", 3);
	m_LoadedMesh = loader.getData();
}

void QuadTest::print(const vox::KeyEvent& event)
{
	fmt::print("{}\n", event.getKey());
}

float QuadTest::randFloat()
{
	const float gen = static_cast<float>(random());
	return (gen / max * 2.0f) - 1.0f;
}

uint8_t QuadTest::randByte()
{
	const float gen = static_cast<float>(random());
	return gen / max * 255;
}
