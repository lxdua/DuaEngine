#pragma once

#ifdef DUA_BUILD_DLL
	#define DUA_API //__declspec(dllexport)
#else
	#define DUA_API //__declspec(dllimport)
#endif // DUA_BUILD_DLL

#define BIT(x) (1 << x)

#define DUA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

