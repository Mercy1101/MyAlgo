#include <stdio.h>

#include "alloc-testing.h"
#include "framework.h"

static test_1()
{
    assert(1);
}

static test_2()
{
    assert(1);
}

static test_3()
{
    assert(1);
}

static UnitTestFunction tests[] = {
	test_1,
	test_3,
	test_3,
	NULL
};

int main(int argc, char *argv[])
{
	run_tests(tests);

	return 0;
}