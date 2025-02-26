#include "Cipher.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>

Cipher::Cipher(const std::string &path)
{
  std::filesystem::path cipherPath(path);

  if (!std::filesystem::exists(cipherPath))
  {
    std::cerr << "Invalid path given: " << cipherPath.string() << std::endl;
    return;
  }

  std::ifstream cipherFile(cipherPath);

  if (!cipherFile || !cipherFile.is_open())
  {
    std::cerr << "Failed to open: " << cipherPath.string() << std::endl;
    std::cerr << "Verify the path is correct." << "\n";
    return;
  }

  std::string tempEncryptedCipher;
  std::vector<uint8_t> buffer;

  cipherFile.seekg(0, std::ios::end);
  size_t cipherFileSize = cipherFile.tellg();
  cipherFile.seekg(0, std::ios::beg);

  buffer.resize(cipherFileSize);

  if (!cipherFile.read(reinterpret_cast<char *>(buffer.data()),
                       cipherFileSize))
  {
    std::cerr << "Failed to read: " << cipherPath.string() << std::endl;
    return;
  }

  tempEncryptedCipher.assign(buffer.begin(), buffer.end());
  // remove null
  tempEncryptedCipher.erase(std::remove(tempEncryptedCipher.begin(), tempEncryptedCipher.end(), '\0'), tempEncryptedCipher.end());
  for (const auto &c : tempEncryptedCipher)
  {
    if (std::isalpha(c))
      EncryptedCipher += c;
  }

  PopulateFrequencyMaps();
}

void Cipher::PopulateFrequencyMaps()
{
  if (EncryptedCipher.size() <= 1)
    return;

  // unigram first
  for (const auto &c : EncryptedCipher)
  {
    m_UnigramFrequencyMap[c]++;
  }

  // bigram
  for (int i = 0; i < EncryptedCipher.size(); i += 2)
  {
    std::string_view bigram = EncryptedCipher.substr(i, 2);

    m_BigramFrequencyMap[std::string(bigram)]++;
  }

  // trigram
  for (int j = 0; j < EncryptedCipher.size(); j += 3)
  {
    std::string_view trigram = EncryptedCipher.substr(j, 3);

    m_TrigramFrequencyMap[std::string(trigram)]++;
  }
}

void Cipher::PrintFrequencyMap(NGRAMTYPE ngramType, int threshold)
{
  switch (ngramType)
  {
  case (UNI):
  {
    for (const auto &[key, value] : m_UnigramFrequencyMap)
    {
      if (value >= threshold)
        std::cout << key << " : " << value << std::endl;
    }

    break;
  }
  case (BI):
  {
    for (const auto &[key, value] : m_BigramFrequencyMap)
    {
      if (value >= threshold)
        std::cout << key << " : " << value << std::endl;
    }

    break;
  }
  case (TRI):
  {
    for (const auto &[key, value] : m_TrigramFrequencyMap)
    {
      if (value >= threshold)
        std::cout << key << " : " << value << std::endl;
    }

    break;
  }

  default:
    break;
  }
}

int Cipher::GetIndexFromLetter(char letter)
{

  return m_Alphabet.at(letter);
}

char Cipher::GetLetterFromIndex(int index)
{
  char letter;
  for (const auto &[l, i] : m_Alphabet)
  {
    if (i == index)
    {
      letter = l;
      break;
    }
  }

  return letter;
}

std::vector<std::pair<char, int>>
Cipher::GetNFrequentUnigrams(int threshold)
{

  std::vector<std::pair<char, int>> sortedFrequencies(
      m_UnigramFrequencyMap.begin(), m_UnigramFrequencyMap.end());

  sortedFrequencies.erase(
      std::remove_if(sortedFrequencies.begin(), sortedFrequencies.end(),
                     [&](const std::pair<char, int> &p)
                     {
                       return p.second < threshold;
                     }),
      sortedFrequencies.end());

  std::sort(
      sortedFrequencies.begin(), sortedFrequencies.end(),
      [](const std::pair<char, int> &a,
         const std::pair<char, int> &b)
      { return a.second > b.second; });

  return sortedFrequencies;
}

std::vector<std::pair<std::string, int>>
Cipher::GetNFrequentBigrams(int threshold)
{

  std::vector<std::pair<std::string, int>> sortedFrequencies(
      m_BigramFrequencyMap.begin(), m_BigramFrequencyMap.end());

  sortedFrequencies.erase(
      std::remove_if(sortedFrequencies.begin(), sortedFrequencies.end(),
                     [&](const std::pair<std::string, int> &p)
                     {
                       return p.second < threshold;
                     }),
      sortedFrequencies.end());

  std::sort(
      sortedFrequencies.begin(), sortedFrequencies.end(),
      [](const std::pair<std::string, int> &a,
         const std::pair<std::string, int> &b)
      { return a.second > b.second; });

  return sortedFrequencies;
}

std::vector<std::pair<std::string, int>> Cipher::GetNFrequentTrigrams(int threshold)
{

  std::vector<std::pair<std::string, int>> sortedFrequencies(
      m_TrigramFrequencyMap.begin(), m_TrigramFrequencyMap.end());

  sortedFrequencies.erase(
      std::remove_if(sortedFrequencies.begin(), sortedFrequencies.end(),
                     [&](const std::pair<std::string, int> &p)
                     {
                       return p.second < threshold;
                     }),
      sortedFrequencies.end());

  std::sort(
      sortedFrequencies.begin(), sortedFrequencies.end(),
      [](const std::pair<std::string, int> &a,
         const std::pair<std::string, int> &b)
      { return a.second > b.second; });

  return sortedFrequencies;
}

int Cipher::GetMostFreqNGramCount(NGRAMTYPE type)
{
  std::vector<int> values;

  switch (type)
  {
  case UNI:
  {
    std::transform(m_UnigramFrequencyMap.begin(), m_UnigramFrequencyMap.end(), std::back_inserter(values),
                   [](const auto &pair)
                   { return pair.second; });

    return GetHighestFrequencyCt(values);
  }
  case BI:
  {
    std::transform(m_BigramFrequencyMap.begin(), m_BigramFrequencyMap.end(), std::back_inserter(values),
                   [](const auto &pair)
                   { return pair.second; });
                   
    return GetHighestFrequencyCt(values);
  }
  case TRI:
  {
    std::transform(m_TrigramFrequencyMap.begin(), m_TrigramFrequencyMap.end(), std::back_inserter(values),
                   [](const auto &pair)
                   { return pair.second; });
                   
    return GetHighestFrequencyCt(values);
  }

  default:
    break;
  }
}

int Cipher::GetHighestFrequencyCt(const std::vector<int> &freqs)
{
  int maxCt = 0;

  for (const auto &f : freqs)
  {
    if (f > maxCt)
    {
      maxCt = f;
    }
  }

  return maxCt;
}