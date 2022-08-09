#include "pch.h"
#include "MeshManager.h"

std::vector<Ref<Mesh>> MeshManager::Meshes{};
std::vector<MeshData>  MeshManager::MData{};

Ref<Mesh> MeshManager::GetMesh(const MeshData& data)
{
	size_t index = 0;
	if (!getIndex<MeshData>(MData, data, index))
	{
		MData.push_back(data);
		Meshes.push_back(CreateRef<Mesh>(data));
		return Meshes.back();
	}
	return Meshes[index];
}
const MeshData& MeshManager::GetMData(Ref<Mesh> mesh)
{
	size_t index = 0;
	ASSERT(getIndex<Ref<Mesh>>(Meshes, mesh, index), "Mesh data not found.");

	return MData[index];
}