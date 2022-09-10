#include "pch.h"
#include "Component.h"

namespace Crave
{
	namespace Component
	{
		//Transform-------------------------------------------------
		Transform::Transform(Entity owner, Entity parent)
			: Owner(owner), Parent(parent)
		{
		}

		void Transform::ScaleF(const float units)
		{
			Scale = glm::vec3(units);
		}

		void Transform::RotateTo(const float angle, const glm::vec3& axis)
		{
			Quaternion = glm::angleAxis(glm::radians(angle), axis);
			EulerAngles = glm::degrees(glm::eulerAngles(Quaternion));
		}

		void Transform::RotateTo(const glm::vec3& eulerAngles)
		{
			EulerAngles = eulerAngles;
			Quaternion = glm::quat(glm::radians(eulerAngles));
		}

		void Transform::RotateAroundPoint(const glm::vec3& point, const float angle, const glm::vec3& axis)
		{
			glm::vec3 iniPos = Position - point;
			glm::quat quatRot = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
			glm::mat4 rotMat = glm::toMat4(quatRot);
			Position = glm::vec3(rotMat * glm::vec4(iniPos, 1.f)) + point;
		}

		glm::mat4 Component::Transform::GetTransformNoParent() const
		{
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(Quaternion)
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 Transform::GetTransform() const
		{
			glm::mat4 parentMat = glm::mat4(1.f);
			if (Parent)
				parentMat = Parent.GetComponent<Transform>().GetTransform();

			return parentMat * GetTransformNoParent();
		}


		glm::mat4 Component::Transform::GetParentTransform() const
		{
			return Parent.GetComponent<Transform>().GetTransform();
		}

		//Light-------------------------------------------------

		//void Light::ApplyBrightness()
		//{
		//	if (Brightness == 0.f) return;
		//	//_UpdatedLastFrame = true;
		//	static glm::vec3 iniAmbient = Data.ambient;
		//	static glm::vec3 iniDiffuse = Data.diffuse;
		//	static glm::vec3 iniSpecular = Data.specular;
		//	
		//	Data.ambient = iniAmbient * Brightness;
		//	Data.diffuse = iniDiffuse * Brightness;
		//	Data.specular = iniSpecular * Brightness;
		//	//Brightness = br;
		//	Renderer::UploadLightData(Data, ShaderIndex);
		//}

		//void Light::UpdateRadius()
		//{
		//	glm::vec3 lightColor = Data.ambient + Data.diffuse + Data.specular;
		//	float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
		//	float con = Data.constant;
		//	float lin = Data.linear;
		//	float quad = Data.quadratic;
		//	//Calculate current radius based on attenuation parameters
		//	Radius =
		//		(-lin + std::sqrtf(lin * lin - 4.f * quad * (con - (256.f / 5.f) * lightMax)))
		//		/ (2.f * quad);
		//}

		//void Light::UpdateAttenuation()
		//{
		//	glm::vec3 lightColor = Data.ambient + Data.diffuse + Data.specular;
		//	float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
		//	float con = Data.constant;
		//	float lin = Data.linear;
		//	float quad = Data.quadratic;
		//	Data.linear =
		//		(-5.f * con + 256.f * lightMax - 5.f * quad * Radius * Radius) / 5.f * Radius;
		//}


		//void Light::SetRadius(float rad)
		//{
		//	_UpdatedLastFrame = true;
		//	//Calculate current radius based on attenuation parameters
		//	UpdateRadius();
		//	//If new radius doesn't correspond to attenuation, tweak attenuation parameters
		//	if (std::fabs(Radius - rad) > std::numeric_limits<float>::epsilon())
		//	{
		//		Radius = rad;
		//		UpdateAttenuation();
		//	}
		//}

		void Light::UpdateViewMat(const glm::mat4& transform)
		{
			glm::vec3 pos = transform[3];
			glm::vec3 front = glm::normalize(transform[2]);
			glm::vec3 up = glm::normalize(transform[1]);
			Data.position = pos;
			switch (Data.type)
			{
			case LightType::Directional:
			{
				glm::mat4 lightView = glm::lookAt(Data.position, glm::vec3(0.0f), glm::vec3(0.f, 1.f, 0.f));
				Data.projViewMat = Renderer::GetDirLightProjMat() * lightView;
				break;
			}
			case LightType::Spot:
			{
				Data.direction = front;
				//glm::vec3 front = glm::normalize(Data.direction);
				glm::mat4 lightView = glm::lookAt(Data.position, Data.position + front, up);
				Data.projViewMat = Renderer::GetSpotLightProjMat() * lightView;
				
				break;
			}
			case LightType::Point:
				//Renderer::UpdateLightPosition(glm::value_ptr(pos), SSBOindex);
				break;
			default:
				DEBUG_BREAK("");
			}
			//ApplyValues(*this);
			Renderer::UploadLightData(Data, ShaderIndex);
		}
	}
}