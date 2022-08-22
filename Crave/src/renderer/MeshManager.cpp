#include "pch.h"
#include "MeshManager.h"
#include "imgui.h"

namespace Crave
{
	std::vector<Ref<Mesh>>			 MeshManager::PrimitiveMeshes{};
	std::vector<Ref<Mesh>>			 MeshManager::ModelMeshes{};

	std::vector<Mesh::PrimitiveData> MeshManager::PrimitiveMeshData{};
	std::vector<Mesh::ModelData>	 MeshManager::ModelMeshData;

	void MeshManager::OnImGuiRender(ImGuiWindowFlags panelFlags)
	{

		ImGui::Begin("Mesh Manager", (bool*)0, panelFlags);

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Primitives"))
		{
			ImGui::Text("Meshes: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(PrimitiveMeshes.size()).c_str());
			ImGui::Text("Data:   ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(PrimitiveMeshData.size()).c_str());

			ImGui::TreePop();
		}

		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Models"))
		{
			ImGui::Text("Meshes: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(ModelMeshes.size()).c_str());
			ImGui::Text("Data:   ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(ModelMeshData.size()).c_str());

			ImGui::TreePop();
		}

		ImGui::End();
	}

	void MeshManager::Clear()
	{
		PrimitiveMeshes.clear();
		ModelMeshes.clear();

		PrimitiveMeshData.clear();
		ModelMeshData.clear();
	}

	Ref<Mesh> MeshManager::GetPrimitiveMesh(const Mesh::PrimitiveData& data)
	{
		size_t index = 0;
		if (!getIndex<Mesh::PrimitiveData>(PrimitiveMeshData, data, index))
		{
			PrimitiveMeshData.push_back(data);
			Ref<Mesh> m = Ref<Mesh>(new Mesh(data));
			PrimitiveMeshes.push_back(m);
			return m;
		}
		return Ref<Mesh>(PrimitiveMeshes[index]);
	}

	Ref<Mesh> MeshManager::GetModelMesh(const Mesh::ModelData& data)
	{
		size_t index = 0;
		if (!getIndex<Mesh::ModelData>(ModelMeshData, data, index))
		{
			ModelMeshData.push_back(data);
			Ref<Mesh> m = Ref<Mesh>(new Mesh(data));
			ModelMeshes.push_back(m);
			return m;
		}
		return Ref<Mesh>(ModelMeshes[index]);
	}
	const Mesh::PrimitiveData& MeshManager::GetPrimitiveMeshData(Ref<Mesh> mesh)
	{
		size_t index = 0;
		ASSERT(getIndex<Ref<Mesh>>(PrimitiveMeshes, mesh, index), "Mesh data not found.");

		return PrimitiveMeshData[index];
	}
	const Mesh::ModelData& MeshManager::GetModelMeshData(Ref<Mesh> mesh)
	{
		size_t index = 0;
		ASSERT(getIndex<Ref<Mesh>>(ModelMeshes, mesh, index), "Mesh data not found.");

		return ModelMeshData[index];
	}
}