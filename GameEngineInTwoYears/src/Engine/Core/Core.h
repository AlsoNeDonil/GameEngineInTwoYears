#pragma once
#include <memory>

#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_DYNAMIC_LINK
		#ifdef ENGINE_BUILD_DLL
			#define ENGINE_API __declspec(dllexport)
		#else 
			#define ENGINE_API __declspec(dllimport)
		#endif
	#else 
		#define ENGINE_API
	#endif
#else
	#error This Engine support only Windows
#endif

#ifdef ENGINE_ENABLE_ASSERTS
	#define ENGINE_CORE_ASSERT(x, ...) { (if(!x)){ENGINE_CORE_ERROR("Assertion Failed : {0}", __VA__ARGS__); __debugbreak;} } 
	#define ENGINE_CLIENT_ASSERT(x, ...) { (if(!x)){ENGINE_CLIENT_ERROR("Assertion Failed : {0}", __VA__ARGS__); __debugbreak;} } 
#else
	#define ENGINE_CORE_ASSERT(x, ...)
	#define ENGINE_CLIENT_ASSERT(x, ...)
#endif

#define BIT(x) (x << 1)

#define ENGINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Engine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}