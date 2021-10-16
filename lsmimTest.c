#include <criterion/criterion.h>
#include "lsmim.h"

Test(simple, contains_slash) {
  cr_assert(containsSlash("/"));
}

Test(simple, does_not_contain_slash) {
  cr_assert(!containsSlash("."));
}
