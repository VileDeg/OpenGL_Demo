#pragma once
#include "renderer/Mesh.h"


class MeshManager
{
public:
	static Ref<Mesh> GetMesh(const MeshData& data);
	static const MeshData& GetMData(Ref<Mesh> mesh);
	static void Clear();
	static void UIStats();

private:
	static std::vector<Mesh> Meshes;
	static std::vector<MeshData> MData;

	template<typename T>
	static bool getIndex(const std::vector<T>& vec, const T& item, size_t& index)
	{
		auto& iter = std::find(vec.begin(), vec.end(), item);
		if (iter == vec.end())
			return false;
		index = iter - vec.begin();
		return true;
	}
};