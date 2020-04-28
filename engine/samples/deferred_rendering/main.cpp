#include "engine.h"
#include <osg/Group>


int main()
{
	osg::Group* g = nullptr;

	core::properties properties = {};
	properties.counter = 1;

	std::unique_ptr<core::engine> obj_ptr = std::make_unique<core::engine>(properties);

}