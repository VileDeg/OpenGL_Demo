#include "pch.h"
#include "MeshManager.h"
#include "imgui.h"

std::vector<Ref<Mesh>> MeshManager::Meshes{};
std::vector<MeshData>  MeshManager::MData{};

void MeshManager::OnImGuiRender(ImGuiWindowFlags panelFlags)
{
	ImGui::Begin("Mesh Manager", (bool*)0, panelFlags);

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
		Ref<Mesh> m = Ref<Mesh>(new Mesh(data));
		Meshes.push_back(m);
		return m;
	}
	return Ref<Mesh>(Meshes[index]);
}

const MeshData& MeshManager::GetMData(Ref<Mesh> mesh)
{
	std::cout << "Meshes " << Meshes.size() << '\n';
	std::cout << "MData " << MData.size() << '\n';
	size_t index = 0;
	ASSERT(getIndex<Ref<Mesh>>(Meshes, mesh, index), "Mesh data not found.");

	return MData[index];
}