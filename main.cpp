#include "src/AffineCipher.h"

#include "src/HillCipher.h"
#include "utils/Matrix.h"

#include "utils/LinearCongruence.h"

#include <cstdio>
#include <iostream>
#include <numeric>
#include <optional>
#include <fstream>

#include <vector>

std::string StringToLower(const std::string &_str)
{
  std::string _lowerStr;
  for (const auto &s : _str)
  {
    if (isalpha(s))
      _lowerStr += tolower(s);
  }

  return _lowerStr;
}

std::string HandleBigramInput(const std::string &bigram)
{
  std::string cleanBigram;
  for (const auto &b : bigram)
  {
    if (isalpha(b))
    {
      if (islower(b))
        cleanBigram += toupper(b);
      else
        cleanBigram += b;
    }
  }

  return cleanBigram;
}

std::string HandleUnigramInput(const std::string &unigram)
{
  std::string cleanUnigram;
  for (const auto &u : unigram)
  {
    if (isalpha(u))
    {
      if (islower(u))
        cleanUnigram += toupper(u);
      else
        cleanUnigram += u;
    }
  }

  return cleanUnigram;
}

bool ValidateUnigramInput(const std::vector<std::string> &input)
{
  bool hasValidInput = true;
  for (const auto &i : input)
  {
    if (!i.size() == 1)
    {
      hasValidInput = false;
      break;
    }
  }

  return hasValidInput;
}

bool ValidateBigramInput(const std::vector<std::string> &input)
{
  bool hasValidInput = true;
  for (const auto &i : input)
  {
    if (i.size() != 2)
    {
      hasValidInput = false;
      break;
    }
  }

  return hasValidInput;
}

int main(int argc, char *argv[])
{
  
  bool validFilePathGiven = false;
  std::string inputPath;

  std::string cipherType;
  std::vector<std::string> validCipherTypes = {"affine", "hill"};
retry:
  while (!validFilePathGiven)
  {
    std::cout << "Enter the type of cipher you want to solve (Hill or Affine): ";
    std::cin >> cipherType;

    cipherType = StringToLower(cipherType);
    bool validCipherTypeGiven = false;
    for (const auto &type : validCipherTypes)
    {
      if (cipherType == type)
      {
        validCipherTypeGiven = true;
        break;
      }
    }

    if (!validCipherTypeGiven)
    {
      std::cout << "Invalid cipher type given, only use Affine or Hill.\n";
      goto retry;
    }

    std::cout << "Enter the file path or file name for the Cipher you want to decode: ";
    std::cin >> inputPath;

    std::filesystem::path cipherPath(inputPath);

    if (std::filesystem::exists(cipherPath))
    {
      std::ifstream cipherFile(cipherPath);

      if (!cipherFile.is_open())
      {
        std::cout << "Error: Could not open file.\n";
        continue;
      }

      cipherFile.seekg(0, std::ios::end);
      size_t cipherFileSize = static_cast<size_t>(cipherFile.tellg());
      cipherFile.seekg(0, std::ios::beg);

      size_t bufferSize = (cipherFileSize > 1024) ? 1024 : cipherFileSize;
      std::vector<char> buffer(bufferSize);

      if (!cipherFile.read(buffer.data(), bufferSize))
      {
        std::cout << "Invalid file detected. Make sure cipher file only contains alphabetic characters. " << std::endl;
      }
      else
      {
        std::erase_if(buffer, [](char c)
                      { return c == '\n' || c == '\t' || c == ' ' || c == '\r'; });

        for (char c : buffer)
        {
          if (!std::isalpha(static_cast<unsigned char>(c)))
          {
            std::cout << "Invalid file detected. Make sure cipher file only contains alphabetic characters.\n";
            cipherFile.close();
            goto retry;
          }
        }
        validFilePathGiven = true;
      }
    }
    else
    {
      inputPath.clear();
      std::cout << "The filename or file path you gave was invalid." << std::endl;
    }
  }

  std::cout << "Let's get crackin :)" << std::endl;
  std::cout << "input type: " << cipherType << "\n";

  if (cipherType == "affine")
  {
    AffineCipher aCipher(inputPath);

    int freqThreshold;
    int maxFreqThreshold = aCipher.GetMostFreqNGramCount(UNI);
    std::cout << "Enter a frequency threshold for the number of letters you'd like to see: (1 - " << maxFreqThreshold << "): ";

    while (!(std::cin >> freqThreshold) || freqThreshold < 1 || freqThreshold > maxFreqThreshold) {
      std::cout << "Invalid threshold given. Please enter a number between 1 and " << maxFreqThreshold << ": ";
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
  }

    std::vector<std::pair<char, int>> nFreqUnis = aCipher.GetNFrequentUnigrams(freqThreshold);

    for (const auto &p : nFreqUnis)
    {
      std::cout << p.first << " : " << p.second << std::endl;
    }

    bool done = false;

    while (!done)
    {
      
      std::string x1, x2, y1, y2;
      bool hasValidMappings = false;

      while (!hasValidMappings)
      {
        std::cout << "Enter first encrypted letter (y1): ";
        std::cin >> x1;
        x1 = HandleUnigramInput(x1);

        std::cout << "Enter second encrypted letter (y2): ";
        std::cin >> x2;
        x2 = HandleUnigramInput(x2);

        std::cout << "Enter first decrypted letter (x1): ";
        std::cin >> y1;
        y1 = HandleUnigramInput(y1);

        std::cout << "Enter second decrypted letter (x2): ";
        std::cin >> y2;
        y2 = HandleUnigramInput(y2);

        std::cout << "You entered: (" << x1 << ", " << x2 << ", " << y1 << ", " << y2 << ")\n";

        hasValidMappings = ValidateUnigramInput({x1, x2, y1, y2});

        if (!hasValidMappings)
        {
          std::cout << "Invalid input. Please try again.\n";
        }
      }

   
      CharacterMapping mapping(x1[0], y1[0], x2[0], y2[0]);
      std::cout << aCipher.DecryptManual(mapping) << std::endl;

      while (true)
      {
        std::cout << "\nTry again? [Y/n]: ";
        std::string confirmRetry;
        std::cin >> confirmRetry;

        if (confirmRetry == "Y" || confirmRetry == "y")
        {
          break;
        }
        else if (confirmRetry == "N" || confirmRetry == "n")
        {
          std::cout << "See you next time :)\n";
          done = true;
          break;
        }
        else
        {
          std::cout << "Unknown option. Please enter Y or N.\n";
        }
      }
    }


  }
  else if (cipherType == "hill")
  {
    HillCipher hCipher(inputPath);
    

    int freqThreshold;
    int maxFreqThreshold = hCipher.GetMostFreqNGramCount(BI);
    std::cout << "Enter a frequency threshold for the number of bigrams you'd like to see: (1 - " << maxFreqThreshold << "): ";

    while (!(std::cin >> freqThreshold) || freqThreshold < 1 || freqThreshold > maxFreqThreshold) {
      std::cout << "Invalid threshold given. Please enter a number between 1 and " << maxFreqThreshold << ": ";
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
  }


    std::vector<std::pair<std::string, int>> nFreqBis = hCipher.GetNFrequentBigrams(freqThreshold);

    for (const auto &[letter, freq] : nFreqBis)
    {
      std::cout << letter << " : " << freq << std::endl;
    }

    bool done = false;

    while (!done)
    {
      std::string x1, x2, y1, y2;
      bool hasValidMappings = false;

      while (!hasValidMappings)
      {
        std::cout << "Enter first encrypted bigram (y1): ";
        std::cin >> x1;
        x1 = HandleBigramInput(x1);

        std::cout << "Enter second encrypted bigram (y2): ";
        std::cin >> x2;
        x2 = HandleBigramInput(x2);

        std::cout << "Enter first decrypted bigram (x1): ";
        std::cin >> y1;
        y1 = HandleBigramInput(y1);

        std::cout << "Enter second decrypted bigram (x2): ";
        std::cin >> y2;
        y2 = HandleBigramInput(y2);

        std::cout << "You entered: (" << x1 << ", " << x2 << ", " << y1 << ", " << y2 << ")\n";

        hasValidMappings = ValidateBigramInput({x1, x2, y1, y2});

        if (!hasValidMappings)
        {
          std::cout << "Invalid input. Please try again.\n";
        }
      }

      KeyMapping mapping(x1, x2, y1, y2);
      std::cout << hCipher.DecryptManual(mapping) << std::endl;

      while (true)
      {
        std::cout << "\nTry again? [Y/n]: ";
        std::string confirmRetry;
        std::cin >> confirmRetry;

        if (confirmRetry == "Y" || confirmRetry == "y")
        {
          break; 
        }
        else if (confirmRetry == "N" || confirmRetry == "n")
        {
          std::cout << "See you next time :)\n";
          done = true;
          break;
        }
        else
        {
          std::cout << "Unknown option. Please enter Y or N.\n";
        }
      }
    }
  }
  else
  {
    std::cout << "how did you get here...\n";
  }

  return 0;
}
