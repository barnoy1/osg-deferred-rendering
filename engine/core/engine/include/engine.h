#ifndef __ENGINE_H__
#define __ENGINE_H__

// standard headers
#include <Windows.h>
#include <process.h>
#include <chrono>
#include <memory>
#include <ctime>

// local headers
#include "core_EXPORT.h"
#include "structs.h"
#include "engine_version.h"

// enable C / C++ linkage
#ifdef __cplusplus
extern "C" {
#endif

	namespace core
	{
		
		class CORE_EXPORT engine
		{
		public:

			
			engine() = delete;
			explicit engine(const core::properties& properties);
			~engine();
				
			std::string get_version();

		private:

			std::unique_ptr<core::properties> properties_;
	
		};
	}

#ifdef __cplusplus
}
#endif

#endif // __ENGINE_H__
