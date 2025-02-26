#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <type_traits>

enum NGRAMTYPE
{
  UNI,
  BI,
  TRI
};


struct DecryptedValue
{
  std::string mappings;
  std::string value;
  float entropy;
};

class Cipher
{
protected:
  Cipher(const std::string &path);

public:
  virtual ~Cipher() = default;

  virtual std::string Decrypt() = 0;

  std::string EncryptedCipher;

  int GetIndexFromLetter(char letter);
  char GetLetterFromIndex(int index);

  void PrintFrequencyMap(NGRAMTYPE ngramType, int threshold = 0);

  // get sorted list of letter -> frequency pairs, default threshold is 10
  std::vector<std::pair<std::string, int>> GetNFrequentBigrams(int threshold = 10);
  std::vector<std::pair<std::string, int>> GetNFrequentTrigrams(int threshold = 10);

  std::vector<std::pair<char, int>> GetNFrequentUnigrams(int threshold = 10);

  int GetMostFreqNGramCount(NGRAMTYPE type);


private:
  std::unordered_map<char, int> m_Alphabet = {
      {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}};

  std::unordered_map<char, int> m_UnigramFrequencyMap;
  std::unordered_map<std::string, int> m_BigramFrequencyMap;
  std::unordered_map<std::string, int> m_TrigramFrequencyMap;

  int GetHighestFrequencyCt(const std::vector<int>& freqs);

  void PopulateFrequencyMaps();
};


