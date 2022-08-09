#include "pch.h"
#include "MeshManager.h"
#include "imgui.h"

std::vector<Mesh> MeshManager::Meshes{};
std::vector<MeshData>  MeshManager::MData{};

void MeshManager::UIStats()
{
	ImGui::Begin("Mesh Manager");

	ImGui::Text("Meshes: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(Meshes.size()).c_str());
	ImGui::Text("Meshes Data: ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(MData.size()).c_str());

	ImGui::End();
}

void MeshManager::Clear()
{
	Meshes.clear();
	MData.clear();
}

Ref<Mesh> MeshManager::GetMesh(const MeshData& data)
{
	size_t index = 0;
	if (!getIndex<MeshData>(MData, data, index))
	{
		MData.push_back(data);
		Meshes.push_back(Mesh(data));
		return CreateRef<Mesh>(Meshes.back());
	}
	return CreateRef<Mesh>(Meshes[index]);
}
const MeshData& MeshManager::GetMData(Ref<Mesh> mesh)
{
	size_t index = 0;
	ASSERT(getIndex<Mesh>(Meshes, *mesh, index), "Mesh data not found.");

	return MData[index];
}