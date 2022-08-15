#include "pch.h"
#include <glm/glm.hpp>
#include "GeoData.h"

namespace GeoData
{
	namespace
	{
		extern const float CUBE_DATA[504];
		extern const float SKYBOX_DATA[108];

		float m_QuadVertices[84] = {};
		
		void CalcPlaneVertices();
	}

	void Init()
	{
		CalcPlaneVertices();
	}

	VBOData GetData(Primitive primType)
	{
		switch (primType)
		{
		case Primitive::Cube:
			return { CUBE_DATA, sizeof(CUBE_DATA), 36 };
		case Primitive::Plane:
			return { m_QuadVertices, sizeof(m_QuadVertices), 6 };
		case Primitive::Skybox:
			return { SKYBOX_DATA, sizeof(SKYBOX_DATA), 36 };
		default:
			return{ nullptr, 0, 0 };
		}
		ASSERT(false, "GetData error.");
	}

	namespace
	{
		void CalcPlaneVertices()
		{
			// positions
			glm::vec3 pos1(-0.5f, 0.5f, 0.0f);
			glm::vec3 pos2(-0.5f, -0.5f, 0.0f);
			glm::vec3 pos3(0.5f, -0.5f, 0.0f);
			glm::vec3 pos4(0.5f, 0.5f, 0.0f);
			// texture coordinates
			glm::vec2 uv1(0.0f, 1.0f);
			glm::vec2 uv2(0.0f, 0.0f);
			glm::vec2 uv3(1.0f, 0.0f);
			glm::vec2 uv4(1.0f, 1.0f);
			// normal vector
			glm::vec3 nm(0.0f, 0.0f, 1.0f);

			// calculate tangent/bitangent vectors of both triangles
			glm::vec3 tangent1, bitangent1;
			glm::vec3 tangent2, bitangent2;
			// triangle 1
			// ----------
			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			// triangle 2
			// ----------
			edge1 = pos3 - pos1;
			edge2 = pos4 - pos1;
			deltaUV1 = uv3 - uv1;
			deltaUV2 = uv4 - uv1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


			bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


			float quadVertices[84] = {
				// positions            // normal         // texcoords  // tangent                          // bitangent
				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
			};

			for (size_t i = 0; i < 84; ++i)
			{
				m_QuadVertices[i] = quadVertices[i];
			}
		}
		
		const float CUBE_DATA[504] =
		{
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right         
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			// Right face
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right         
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-right     
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// top-left
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f// bottom-left        

		};
		const float SKYBOX_DATA[108] =
		{
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
	}
}