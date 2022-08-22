#pragma once
#include "pch.h"
#include <cereal/archives/json.hpp>
#include <cereal/access.hpp>

namespace Crave
{
	class Texture
	{
	public:
		enum class MMFilter : int //Min/mag filter
		{
			//values from glad.h
			None = -1, Nearest = 0x2600, Linear = 0x2601
		};
		enum class WrapMode : int
		{
			//values from glad.h
			None = -1, Repeat = 0x2901, ClampToEdge = 0x812F
		};
		enum class Target : int
		{
			//values from glad.h
			None = -1, Texture2D = 0x0DE1, TextureCubeMap = 0x8513
		};
		enum class Type : int
		{
			None = -1, RGBA, Depth, DepthNStencil, Integer
		};

		struct Config //Texture configuration
		{
			Type	 type     = Type::None;
			Target   target	  = Target::None;
			MMFilter filter   = MMFilter::None;
			WrapMode wrapMode = WrapMode::None;
			
			Config(Type tp)
				: type(tp), target(Target::Texture2D), filter(MMFilter::None), wrapMode(WrapMode::None) {}
			Config(Type tp, Target trg)
				: type(tp), target(trg), filter(MMFilter::None), wrapMode(WrapMode::None) {}
			Config(Type tp, Target trg, MMFilter fil, WrapMode wm)
				: type(tp), target(trg), filter(fil), wrapMode(wm) {}
		};

	private:
		static constexpr const char* BASE_TEXTURE_PATH = "res/textures/";
		static constexpr const char* BASE_CUBEMAP_PATH = "res/textures/cubemaps/";
	public:
		Texture() {}
		//For texture loading from "res/textures" folder
		Texture(const std::string& texName, bool useRelativePath = true);//, TexConfig config = {
		//Target::Texture2D, Type::Color, MMFilter::Linear, WrapMode::Repeat}
		 
		//For loading of cubemaps from "res/textures/cubemaps" folder
		Texture(const std::string& folderName,
			const char* faces[]);

		//For texture creatiion with set parameters
		Texture(glm::vec2 dimensions, Config config);

		void Bind(int slot);
		void Unbind() const;

		unsigned Id()      const { return m_Id; }
		unsigned Slot()    const { return m_BoundSlot; }
		int		 Target()  const { return m_Target; }
		//Type	 GetType() const { return m_Type; }
		
		Texture(const Texture&) = delete;
		Texture(Texture&& t) = delete;

		~Texture();
	private:
		int		 m_Target = -1;
		unsigned m_BoundSlot{};
		unsigned m_Id{};
		//Type	 m_Type;

	private:
		friend class cereal::access;
		template<typename Archive>
		void serialize(Archive& ar)
		{
			ar& cereal::make_nvp("ID", m_Id);
			ar& cereal::make_nvp("Type", m_Target);
			ar& cereal::make_nvp("BoundSlot", m_BoundSlot);
		}
	};
}