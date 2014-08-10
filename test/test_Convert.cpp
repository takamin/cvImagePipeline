#include "stdafx.h"
#include "test.h"

bool test_Convert()
{
	test_counter counter;
	Convert target;
	bool status = true;
	cerr << "--- test class Convert" << endl;
	for (int i = 0; i < test_counter::typesCount(); i++) {
		cerr << "type = " << depth_string(test_counter::types[i] & CV_MAT_DEPTH_MASK) << endl;
		target.rtype = test_counter::types[i];
		if (!testProcessor(&target)) {
			status = false;
		}
		counter.testMatDepth(
			string("Convert To ") + depth_string(test_counter::types[i] & CV_MAT_DEPTH_MASK),
			target.getOutputMat(), test_counter::types[i]);
	}
	return status;
}
