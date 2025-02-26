#include "HillCipher.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>

std::string HillCipher::Decrypt()
{

  std::string decryptedCipher;

  if (!m_Key.has_value())
  {
    std::cout << "key hasn't been set yet." << std::endl;
    return decryptedCipher;
  }

  for (int i = 0; i < EncryptedCipher.size(); i += 2)
  {
    std::string bi = EncryptedCipher.substr(i, 2);
    std::cout << bi << std::endl;
    std::vector<int> bigramIndices = {GetIndexFromLetter(bi[0]),
                                      GetIndexFromLetter(bi[1])};

    std::vector<int> decryptedIndices = m_Key.value() * bigramIndices;

    for (const auto &d : decryptedIndices)
    {
      decryptedCipher += std::tolower(GetLetterFromIndex(d));
    }
  }

  return decryptedCipher;
}

std::string HillCipher::Decrypt(ModMatrix::Matrix<int> key)
{
  std::string decryptedCipher;

  for (int i = 0; i < EncryptedCipher.size(); i += 2)
  {
    std::string bi = EncryptedCipher.substr(i, 2);

    std::vector<int> bigramIndices = {GetIndexFromLetter(bi[0]),
                                      GetIndexFromLetter(bi[1])};
    

    // Should follow y * K^-1 (in this order)

    std::vector<int> decryptedIndices = bigramIndices * key;


    for (const auto &d : decryptedIndices)
    {
      decryptedCipher += std::tolower(GetLetterFromIndex(d));
    }
  }

  return decryptedCipher;
}

std::optional<ModMatrix::Matrix<int>> HillCipher::TryGetKey(const std::string &x1, const std::string &x2,
                                                            const std::string &y1, const std::string &y2)
{

  std::vector<int> encryptedIndices = {GetIndexFromLetter(x1[0]), GetIndexFromLetter(x1[1]),
                                       GetIndexFromLetter(x2[0]), GetIndexFromLetter(x2[1])};

  std::vector<int> decryptedIndices = {GetIndexFromLetter(y1[0]), GetIndexFromLetter(y1[1]),
                                       GetIndexFromLetter(y2[0]), GetIndexFromLetter(y2[1])};

  ModMatrix::Matrix<int> X(decryptedIndices);
  std::cout << "-----X-----" << std::endl;
  X.Print();
  ModMatrix::Matrix<int> Y(encryptedIndices);
  std::cout << "-----Y-----" << std::endl;
  Y.Print();

  std::optional<ModMatrix::Matrix<int>> Xinv = X.Inverse(26);

  if (!Xinv.has_value())
  {
    printf("X has no inverse.\n");
    return std::nullopt;
  }
  std::cout << "-----Xinv-----" << std::endl;

  Xinv.value().Print();

  // Needs to follow X^-1 * Y
  ModMatrix::Matrix<int> K = Xinv.value() * Y;
  std::cout << "-----K-----" << std::endl;
  K.Print();
  K = K % 26;
  std::cout << "-----K mod 26-----" << std::endl;

  K.Print();

  // Need to find K^-1 ?

  std::optional<ModMatrix::Matrix<int>> Kinv = K.Inverse(26);
  if (!Kinv.has_value())
  {
    printf("K has no inverse.\n");
    return std::nullopt;
  }

  std::cout << "-----K^-1-----" << std::endl;
  Kinv.value().Print();

  return Kinv.value();
}

std::string HillCipher::DecryptManual(KeyMapping k)
{
  std::optional<ModMatrix::Matrix<int>> key = TryGetKey(k.x1, k.x2, k.y1, k.y2);

  if (!key.has_value())
  {
    std::cout << "(" << k.x1 << " - " << k.y1 << "), (" << k.x2 << " - " << k.y2 << ") ";
    std::cout << "are invalid.\n";
    return "";
  }

  std::string decrypted = Decrypt(key.value());
  std::cout << "(" << k.x1 << " - " << k.y1 << "), (" << k.x2 << " - " << k.y2 << ") \n\n";
  return decrypted;
}
