#include "AffineCipher.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

std::string AffineCipher::Decrypt()
{
    return "";
}

std::string AffineCipher::DecryptManual(const CharacterMapping &mapping)
{
    std::string decryptedCipher;

    std::optional<AffineDecryptionKey> potentialKey = TrySetKey(mapping);

    if (!potentialKey.has_value())
        return decryptedCipher;

    std::unordered_map<char, char> mappedValues;

    printf("Solving using equation: d(y) = %d(y - %d)\n", potentialKey.value().aInv, potentialKey.value().b);
    for (const auto &e : EncryptedCipher)
    {

        int encryptedIndex = Cipher::GetIndexFromLetter(e);

        int decryptedIndx = LinearAlg::Mod(potentialKey.value().aInv * encryptedIndex + potentialKey.value().b, 26);

        char decryptedLetter = GetLetterFromIndex(decryptedIndx);

        decryptedCipher += decryptedLetter;

        mappedValues[e] = decryptedLetter;
    }

    std::ofstream outSolution("affine-solution-mappings.txt");

    for (const auto &[enc, dec] : mappedValues)
    {
        outSolution << enc;
        outSolution << " - ";
        outSolution << dec;
        outSolution << "\n\n";
    }

    return decryptedCipher;
}

std::optional<AffineDecryptionKey> AffineCipher::TrySetKey(const CharacterMapping &mapping)
{
    // solving Linear congruence with given values in CharacterMapping struct

    // have two equations:
    // 1. ax1 + b = y1 (mod m)
    // 2. ax2 + b = y2 (mod m)

    // start with a(x1 - x2) = (y1 - y2) (mod m)
    // --> has the form ax = c (mod m), therefore, solve for x to get a in above equation

    // using x as variable name for generic Linear Congruence form
    int x = LinearAlg::Mod(Cipher::GetIndexFromLetter(mapping.x1) - Cipher::GetIndexFromLetter(mapping.x2), 26);
    int c = LinearAlg::Mod(Cipher::GetIndexFromLetter(mapping.y1) - Cipher::GetIndexFromLetter(mapping.y2), 26);
    printf("Eq 1: a*%d +b = %d (mod 26)\n", Cipher::GetIndexFromLetter(mapping.x1), Cipher::GetIndexFromLetter(mapping.y1));
    printf("Eq 2: a*%d +b = %d (mod 26)\n", Cipher::GetIndexFromLetter(mapping.x2), Cipher::GetIndexFromLetter(mapping.y2));

    printf("solving: %dx = %d\n", x, c);

    int d = LinearAlg::ExtendedEuclideanAlg(x, 26);
    std::cout << "GCD: " << d << std::endl;

    // just find multiplicative inverse to start with
    std::optional<int> xInv = LinearAlg::MultiplicativeInverse(x, 26);

    if (xInv.has_value())
    {
        printf("Xinv: %d\n", xInv.value());
        // multiply c  by x^-1, which we'll call a at this point
        int a = LinearAlg::Mod((c * xInv.value()), 26);

        // find A^-1 for decryption equation, then use A^-1 to find b
        std::optional<int> aInv = LinearAlg::MultiplicativeInverse(a, 26);

        printf("a: %d, a^-1: %d\n", a, aInv.value());

        // solve b = y1 - ax1 (mod m)
        int ax1 = LinearAlg::Mod(a * Cipher::GetIndexFromLetter(mapping.x1), 26);
        int b = (Cipher::GetIndexFromLetter(mapping.y1) - ax1) % 26;

        // have a and b, can make Affine Key now
        printf("Encryption function: e(x) = %dx + %d \n", aInv.value(), b);

        return AffineDecryptionKey(a, b);
    } 

    return std::nullopt;
   
}
