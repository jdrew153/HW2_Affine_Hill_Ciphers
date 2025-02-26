#pragma once

#include <cstdio>
#include <numeric>
#include <optional>
#include <utility>


namespace LinearAlg
{
  
int ExtendedEuclideanAlg(int a, int b);
   
std::optional<int> MultiplicativeInverse(int a, int m);

int Mod(int a, int m);


}

