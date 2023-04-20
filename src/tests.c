#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../deps/munit/munit.h"
#include "string.h"

static void* alloc_from_param(
		const MunitParameter params[],
		void* user_data
)
{
	const char* data = munit_parameters_get(params, "Data");
	if (strcmp(data, "Allocated") == 0)
		return munit_malloc(sizeof(size_t));
	else
		return NULL;
}

static MunitResult wannacrash_guard_clause (
		const MunitParameter params[],
		size_t* data
)
{
	if (!data) {
		munit_assert_null(data);
		return MUNIT_OK;
	}

	munit_assert_not_null(data);
	
	*data = (size_t) ~0;
	size_t fart = *data;
	munit_assert_size(fart, ==, ~0);
	
	munit_assert_size(*data, ==, ~0);
	return MUNIT_OK;
}

static char* allocation_status[] = {
  "NULL", "Allocated", NULL
};

static MunitParameterEnum alloc_params[] = {
  { "Data", allocation_status },
  { NULL, NULL },
};

static MunitTest units[] = {
  {
    "Guard-Clause for NULL Dereference", /* name */
    wannacrash_guard_clause, /* test */
    alloc_from_param, /* setup */
    free, /* tear_down */
    MUNIT_TEST_OPTION_NONE, /* options */
    alloc_params /* parameters */
  },
  /* Mark the end of the array with an entry where the test
   * function is NULL */
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite bruh = {
  "Bruh, is your compiler leaving invisible vulns around?\n", /* name */
  units, /* tests */
  NULL, /* suites */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char** argv) {
	int status = munit_suite_main(&bruh, NULL, argc, argv);
	return status;
}
