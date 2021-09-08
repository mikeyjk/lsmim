#include <criterion/criterion.h>

Test(simple, test) {
  cr_assert("hello world\n", "hello world\n");
}
