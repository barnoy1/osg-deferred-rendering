
#ifndef __COMMON_FIXTURE__
#define __COMMON_FIXTURE__

#include "json_fixture.h"
#include "engine.h"
// The fixture for testing class Project1. From goggle test primer.
class common_fixture : public ::json_fixture
{
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	
	core::properties properties = {};
	std::unique_ptr<core::engine> obj_ptr;
	
	common_fixture()
	{
		properties.counter = unittest_core::configuration_json["counter"];
		obj_ptr = std::make_unique<core::engine>(properties);
	}

	virtual ~common_fixture()
	{
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:
	virtual void SetUp() 
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() 
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

};

#endif // __COMMON_FIXTURE__