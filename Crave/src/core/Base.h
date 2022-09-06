#pragma once
#include "Log.h"

namespace Crave
{

#define ENABLE_ASSERTS
#ifdef ENABLE_ASSERTS

#ifdef _DEBUG
#define DEBUG_BREAK(msg) do{ LOG_ERROR("DEBUG_BREAK: {}", msg); __debugbreak(); }while(0)
#define ASSERT(x, msg) do{ if(!(x)) { DEBUG_BREAK("ASSERT: " msg); } }while(0)
#else
#define ASSERT(x,msg) do{ if(!(x)) { LOG_ERROR("Assertion Failed: {}", msg);\
exit(1); } }while(0)
#endif //_DEBUG
#else

#define ASSERT(x,msg)
#endif //ENABLE_ASSERTS

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Weak = std::weak_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Weak<T> CreateWeak(Args&& ... args)
	{
		return std::weak_ptr<T>(std::make_shared<T>(std::forward<Args>(args)...));
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}