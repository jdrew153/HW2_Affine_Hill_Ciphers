#pragma once

#include "Cipher.h"

#include "../utils/Matrix.h"
#include <vector>
#include <optional>
#include <variant>
#include <map>

const std::vector<std::string> COMMON_BIGRAMS = {
    "TH", "HE", "IN", "ER", "AN", "RE", "ED", "ON", "ES", "ST",
    "EN", "AT", "TO", "NT", "HA", "ND", "OU", "EA", "NG", "AS",
    "OR", "TI", "IS", "ET", "IT", "AR", "TE", "SE", "HI", "OF", "CS", "CI"};

struct KeyMapping
{
  std::string x1;
  std::string x2;
  std::string y1;
  std::string y2;

  KeyMapping() = default;
  explicit KeyMapping(std::string _x1, std::string _x2, std::string _y1, std::string _y2) : x1(_x1), x2(_x2), y1(_y1), y2(_y2) {}
};



class HillCipher : public Cipher
{
public:
  // default value for m = 2, can change though
  explicit HillCipher(const std::string &path) : Cipher(path), m(2) {};

  explicit HillCipher(const std::string &path, int _m) : Cipher(path), m(_m) {};

  std::string Decrypt() override;
  std::string Decrypt(ModMatrix::Matrix<int> key);

  int m;

  // default 2x2 matrix
  std::optional<ModMatrix::Matrix<int>> m_Key;

  std::string DecryptManual(KeyMapping k);

  std::optional<ModMatrix::Matrix<int>> TryGetKey(const std::string &x1, const std::string &x2,
                                                  const std::string &y1, const std::string &y2);

};
