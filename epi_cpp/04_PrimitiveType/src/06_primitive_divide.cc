#include "test_framework/generic_test.h"

// x / y = 
// 1) 0, if x < y
// 2) 1 + (x-y) / y, otherwise
//    -> computing the largest k such that 2^ky <= x, recusrive step is 2^k + (x-2^ky) / y
int Divide(int x, int y) {
  int result = 0;
  int power = 32;
  unsigned long long y_power = static_cast<unsigned long long>(y) << power;
  while (x >= y) {
    while (y_power > x) {
      y_power >>= 1;
      --power;
    }

    result += 1 << power;
    x -= y_power;
  }
  return result;
}

unsigned divide_bsearch(unsigned x, unsigned y) {
  if (x < y) {
    return 0;
  }

  int power_left = 0;
  int power_right = sizeof(unsigned) << 3;
  int power_mid = -1;
  while (power_left < power_right) {
    int tmp = power_mid;
    power_mid = power_left + ((power_right - power_left) >> 1);
    if (tmp == power_mid) {
      break;
    }
    unsigned yshift = y << power_mid;
    if ((yshift >> power_mid) != y) {
      // yshift overflowed, use a smaller shift.
      power_right = power_mid;
      continue;
    }
    if ((y << power_mid) > x) {
      power_right = power_mid;
    } else if ((y << power_mid) < x) {
      power_left = power_mid;
    } else {
      return (1U << power_mid);
    }
  }
  unsigned part = 1U << power_left;
  return part | divide_bsearch(x - (y << power_left), y);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  GenericTestMain(args, "primitive_divide.cc", "primitive_divide.tsv",
                  &Divide, DefaultComparator{}, param_names);
  GenericTestMain(args, "primitive_divide.cc", "primitive_divide.tsv",
                  &divide_bsearch, DefaultComparator{}, param_names);
}