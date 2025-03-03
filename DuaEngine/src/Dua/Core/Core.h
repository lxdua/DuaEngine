#pragma once

#include <memory>

namespace Dua {

	template<typename T>
	using Sole = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}