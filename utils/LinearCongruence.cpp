#include "LinearCongruence.h"
#include <cmath>
#include <cstdio>
#include <optional>

namespace LinearAlg
{
  std::optional<int> MultiplicativeInverse(int a, int m)
  {

    if (ExtendedEuclideanAlg(a, m) > 1)
      return std::nullopt;

    for (int i = 0; i < m; i++)
    {
      if (((a % m) * (i % m)) % m == 1)
        return i;
    }

    return std::nullopt;
  }

  int ExtendedEuclideanAlg(int a, int b)
  {
    int a0 = a, b0 = b, t0 = 0, t = 1, s0 = 1, s = 0;

    int q = (a0 / b0);
    int r = (a0 - q * b0);

    while (r > 0)
    {
      int temp = t0 - q * t;
      t0 = t, t = temp;
      temp = s0 - q * s;

      s0 = s, s = temp, a0 = b0, b0 = r;

      q = (a0 / b0);
      r = a0 - q * b0;
    }

    r = b0;

    printf("GCD(%d, %d) = %d\n", a, b, r);

    // not sure if this is right...
    return r;
  }

  int Mod(int a, int m)
  {

    return ((a % m) + m) % m;
  }

} // namespace LinearAlg
