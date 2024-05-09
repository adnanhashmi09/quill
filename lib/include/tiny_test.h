#ifndef TINY_TEST_H
#include<stdlib.h>

#define MAX_TESTCASE_NAME_LENGTH 100

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

struct TestCaseType {
  char name[MAX_TESTCASE_NAME_LENGTH];
  void (*testCaseFunction)(void);
  struct TestCaseType *next;
};
typedef struct TestCaseType TestCase;

void addTest(char *name, void (*test_function)(void));
void assert_condition(int condition, const char *failure_message);
void runTests();

#endif // !TINY_TEST_H
