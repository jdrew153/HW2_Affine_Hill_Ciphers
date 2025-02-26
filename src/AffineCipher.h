#pragma once

#include "Cipher.h"

#include "../utils/LinearCongruence.h"

const std::vector<char> COMMON_UNIGRAMS = {
    'E', 'T', 'A', 'O', 'I', 'N', 'S', 'H'};

struct CharacterMapping
{
  char x1;
  char y1;
  char x2;
  char y2;

  CharacterMapping() = default;
  CharacterMapping(char _x1, char _y1, char _x2, char _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

  void Print()
  {
    printf("(%c - %c), (%c - %c)\n", x1, y1, x2, y2);
  }
};

struct AffineDecryptionKey
{
  int aInv;
  int b;

  AffineDecryptionKey() = default;
  AffineDecryptionKey(int _aInv, int _b) : aInv(_aInv), b(_b) {}
};

class AffineCipher : public Cipher
{
public:
  explicit AffineCipher(const std::string &path) : Cipher(path) {}

  std::string Decrypt() override;

  std::string DecryptManual(const CharacterMapping &mapping);

  AffineDecryptionKey key;

private:
  std::optional<AffineDecryptionKey> TrySetKey(const CharacterMapping &mapping);
};