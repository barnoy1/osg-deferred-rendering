


// public headers
#include "engine.h"

namespace core
{
	engine::engine(const core::properties& properties)
	{		
		properties_ = std::make_unique<core::properties>(properties);		
	}

	engine::~engine()
	{
	}

	std::string engine::get_version()
	{
		return VERSION;	
	}
}