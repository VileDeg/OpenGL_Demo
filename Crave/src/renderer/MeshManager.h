#pragma once
#include "renderer/Mesh.h"

namespace Crave
{
	class MeshManager
	{
	private:
		typedef int ImGuiWindowFlags;
	public:
		static Ref<Mesh> GetPrimitiveMesh(const Mesh::PrimitiveData& data);
		static Ref<Mesh> GetModelMesh(const Mesh::ModelData& data);

		static const Mesh::PrimitiveData& GetPrimitiveMeshData(Ref<Mesh> mesh);
		static const Mesh::ModelData& GetModelMeshData(Ref<Mesh> mesh);

		static void Clear();
		static void OnImGuiRender(ImGuiWindowFlags panelFlags);

		static std::vector<Ref<Mesh>>			PrimitiveMeshes;
		static std::vector<Ref<Mesh>>			ModelMeshes;
		static std::vector<Mesh::PrimitiveData> PrimitiveMeshData;
		static std::vector<Mesh::ModelData>		ModelMeshData;
	private:
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
}