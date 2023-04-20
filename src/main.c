#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// NOTE: Thanks systemd!!!; Provides Rust-like TAKE_PTR and other ptr utils.
// https://github.com/systemd/systemd/blob/b5eba967a9c5bb169d2e3183caaef6f4fe1bbab9/src/fundamental/macro-fundamental.h
#include <macro-fundamental.h>


#ifndef CRASH_TEST_METHOD
// Compiled via `xxd -i main.c > bruh.xxd.c`
#include <bruh.xxd.c>
#endif

#define xstr(s) str(s)
#define str(s) #s

#define CRASH_TEST_ELMS NULL
#define CRASH_TEST(NAME) \
	#define CRASH_TEST_ELMS #NAME, ## CRASH_TEST_ELMS \
	NAME

// NOTE: returned value must be freed before program exit.
static char* xdg_data_dir() {
	char buf[PATH_MAX] = {0};
	
	char* xdg_env = getenv("XDG_RUNTIME_DIR");
	if (!xdg_env || strcmp(xdg_env, "") != 0) {
		strcat(buf, "/tmp");
	}
	else {
		strcat(buf, xdg_env);
	}
	return strcpy(buf);
}

static int* CRASH_TEST(wannacrash_guard_clause) (int* data) {
	if (!data) return RETURN;

	*data = ~0;
	return data;
}

// TODO: Think harder about how Take is supposed to be used, because this
//   would surely always pass the test.
static int* CRASH_TEST(wannacrash_guard_clause_with_take) (int* data) {
	if (!data) return NULL;

	*TAKE_PTR(data) = ~0;
	return data;
}

static int* CRASH_TEST(wannacrash_guard_branch) (int* data) {
	if (!data) {
		return NULL;
	}
	else {
		*data = ~0;
		return data;
	}
}

// IMPORTANT: This is a test of the following information against available
//   pointer handling implementations to find the safest methods for ptr eval.
// http://ecue64ybzvn6vjzl37kcsnwt4ycmbsyf74nbttyg7rkc3t3qwnj7mcyd.onion/r/cpp/comments/8439gq/ub_will_delete_your_null_checks/
int main(int argc, char** argv) {
#ifndef CRASH_TEST_METHOD
	int r = EXIT_SUCCESS;
	char* data_dir = xdg_data_dir();
	// [B]ad. [R]adioactive. [U]nintuitive. [H]elper - Unintended Compiler Optimization Discovery Utility
	char* tempdir_name = tempnam(data_dir, "bruh");
	FILE* code;

	if (!argc) {
		fputs(stderr, "Error: expected compiler and flags as arguments: got none.");
		r = EXIT_FAILURE;
		goto finish;
	}

	if (!(code = fopen(tempfile_name, "r"))) {
		perror("Couldn't open temporary file to store code for compilation");
		r = EXIT_FAILURE;
		goto finish;
	}

	if (fwrite(bo_c, sizeof(bo_c), bo_c_len, code) < bo_c_len) {
		perror("Couldn't write code to temporary file");
		r = EXIT_FAILURE;
		goto finish;
	}

	fclose(code);

	static const char** crash_test_methods = { CRASH_TEST_ELMS };

	for (size_t i = 0; crash_test_methods != NULL; i++) {
		char* compile_command[PATH_MAX] = {'0'};
		int status;

		snprintf(compile_command, PATH_MAX, argv[]);

		for (size_t c = 1; c <= argc; c++) {
			strcat(compile_command, argv[c]);
			strcat(compile_command, " ");
		}

		strcat(compile_command, crash_test_methods[i]);
		strcat(compile_command, " ");
		strcat(compile_command, tempfile_name);

		status = system(compile_command);
		if (status) {
			perror();
			continue;
		}
		
		printf("%s: ", crash_test_methods[i]);
		if (!status) puts("success!");
		else puts("failure?!");
	}


	for (size_t i = 0; crash_test_methods != NULL; i++) {
		char* compile_command[ARG_MAX] = {'0'};
		int status;

		for (size_t c = 1; c <= argc; c++) {
			strcat(compile_command, argv[c]);
			strcat(compile_command, " ");
		}

		strcat(compile_command, crash_test_methods[i]);
		strcat(compile_command, " ");
		strcat(compile_command, tempfile_name);

		status = system(compile_command);
		if (status) {
			perror();
			continue;
		}
		
		printf("%s: ", crash_test_methods[i]);
		if (!status) puts("success!");
		else puts("failure?!");
	}

	remove(code);

finish:
	free(tempfile_name);
	free(data_dir);
	return r;

#else
	int r = atoi(argv[1]);
	int* x = NULL;
	if (r % 2) x=&r;
	if (CRASH_TEST_METHOD(x)) {
		printf("%p ", x);
		return 1;
	}
	printf("%p ", x);
	return 0;
#endif;
}
