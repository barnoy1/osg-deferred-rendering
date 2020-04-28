
#include "common_fixture.h"

// Test case must be called the class above
// Also note: use TEST_F instead of TEST to access the test fixture (from goggle test primer)

TEST_F(common_fixture, test_1) {

	std::string ver = obj_ptr->get_version();
	EXPECT_STRNE(nullptr, ver.c_str() );
	std::cout << "\nversion found: " << ver << "\n";
}


TEST_F(common_fixture, test_2) {

	int c = unittest_core::configuration_json["counter"];
	std::cout << "\ncounter value: " << c << "\n";
}
