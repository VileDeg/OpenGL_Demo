#pragma once

namespace Crave
{
	enum class Primitive
	{
		None = -1, Cube, Plane, Skybox
	};

	namespace GeoData
	{
		struct VBOData
		{
			const void* data;
			const std::size_t size;
			const unsigned count;

		};

		void Init();
		VBOData GetData(Primitive primType);
	};

}