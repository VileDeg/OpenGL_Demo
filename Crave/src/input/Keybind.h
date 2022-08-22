#pragma once
//#include <cereal/access.hpp>
//#include <cereal/archives/json.hpp>

namespace Crave
{
	namespace Input
	{
		enum class Key : int
		{
			//from glfw3.h
			None = -1,
			Space = 32,
			Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
			Semicolon, Equal,
			A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Esc = 256, LeftCtrl = 341, LeftAlt
		};

		enum class KeyEvent
		{
			None = -1, Release = 0, Press, Repeat
		};

		enum class MouseButton
		{
			None = -1, Left = 0, Right, Middle
		};

		class Keybind
		{
		public:
			using actionType = std::function<void(void)>;
		public:
			Keybind(Key id, KeyEvent glEventType)
				: m_GlId(id), m_GlEventType(glEventType), m_Action(nullptr)
			{}
			Keybind() {}
			~Keybind() {}

			void Exec() { m_Action(); };
			void BindAction(actionType action) { m_Action = action; }

			Key GlId() const { return m_GlId; }
			KeyEvent GlType() const { return m_GlEventType; }
		private:
			Key m_GlId;
			KeyEvent m_GlEventType;
			std::function<void(void)> m_Action;
		//private:
		//	friend class cereal::access;
		//	template<typename Archive>
		//	void serialize(Archive& ar)
		//	{
		//		ar& cereal::make_nvp("GL_ID", m_GlId);
		//		ar& cereal::make_nvp("GLEventType", m_GlEventType);
		//		//ar& cereal::make_nvp("Action", m_Action);
		//	}
		};
	}
}