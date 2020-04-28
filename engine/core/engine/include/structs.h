#ifndef __STRUCTS__
#define __STRUCTS__

// standard headers
#include <string>
#include <unordered_map>

// local headers
#include "core_EXPORT.h"

// enable C / C++ linkage
#ifdef __cplusplus
extern "C" {
#endif

namespace core
{
	// set struct aligment 
	#pragma pack(1)
	struct CORE_EXPORT properties
	{
		unsigned int counter;
	};
}

#ifdef __cplusplus
}
#endif

#endif // __STRUCTS__ 
