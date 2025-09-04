
// Generated Fossil Logic Test Runner
#include <fossil/pizza/framework.h>

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test List
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_EXPORT(cpp_soap_tests);
FOSSIL_TEST_EXPORT(c_soap_tests);
FOSSIL_TEST_EXPORT(cpp_error_tests);
FOSSIL_TEST_EXPORT(cpp_string_tests);
FOSSIL_TEST_EXPORT(cpp_input_tests);
FOSSIL_TEST_EXPORT(c_input_tests);
FOSSIL_TEST_EXPORT(cpp_parser_test_cases);
FOSSIL_TEST_EXPORT(c_parser_test_cases);
FOSSIL_TEST_EXPORT(cpp_file_tests);
FOSSIL_TEST_EXPORT(c_error_tests);
FOSSIL_TEST_EXPORT(c_string_tests);
FOSSIL_TEST_EXPORT(c_file_tests);

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Runner
// * * * * * * * * * * * * * * * * * * * * * * * *
int main(int argc, char **argv) {
    FOSSIL_TEST_START(argc, argv);
    FOSSIL_TEST_IMPORT(cpp_soap_tests);
    FOSSIL_TEST_IMPORT(c_soap_tests);
    FOSSIL_TEST_IMPORT(cpp_error_tests);
    FOSSIL_TEST_IMPORT(cpp_string_tests);
    FOSSIL_TEST_IMPORT(cpp_input_tests);
    FOSSIL_TEST_IMPORT(c_input_tests);
    FOSSIL_TEST_IMPORT(cpp_parser_test_cases);
    FOSSIL_TEST_IMPORT(c_parser_test_cases);
    FOSSIL_TEST_IMPORT(cpp_file_tests);
    FOSSIL_TEST_IMPORT(c_error_tests);
    FOSSIL_TEST_IMPORT(c_string_tests);
    FOSSIL_TEST_IMPORT(c_file_tests);

    FOSSIL_RUN_ALL();
    FOSSIL_SUMMARY();
    return FOSSIL_END();
} // end of main
