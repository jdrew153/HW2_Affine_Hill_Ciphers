# HW2_Affine and Hill Ciphers
CSC1 6100 - HW2 Affine and Hill Ciphers.

## How to use the program
1. To build and run the project, use "make && make run".

2. After running the program, the user will be prompted with the following,
    - "Enter the type of cipher you want to solve (Hill or Affine): "
     - Type "Hill" or "Affine" corresponding to which cipher you want to solve. If you make the wrong selection, press Ctrl-C or Cmd-C to kill the program and start back at step 1. because there is no functionality to change the selection once you've already entered a valid response.

3. Once the cipher type is chosen, enter the path or filename of the file containing the cipher you want to decode.
    - "Enter the file path or file name for the Cipher you want to decode: " - (Enter your the path or filename of your cipher).
    - If the file isn't located in the same directory as the main exectuable, be sure to enter the filepath of the cipher i.e. "usr/jdrew/cipher"
    *  Note: There is some input validation of the cipher to ensure only files containing 
    alphabetic characters. The cipher files given in the assignment are the basis 
    for the validation, so files that have similar content should pass the initial content checks.

4. The program will then attempt to print the frequency maps corresponding to the cipher type you chose, i.e hill -> bigrams, affine -> unigrams (letters). First you'll be asked what threshold you want to limit the frequency values to, such as if you only want to see bigrams that occur 15 or more times, you'd enter 15. The default is 10 if you don't enter a value, or enter something other than a number. Once you enter a threshold, the frequencies will print out in a list format (XX : 10)

5. Next, you'll start entering values for y1, y2, x1, x2, to create the key for the respective cipher type.
    - Affine Cipher example,
        - (I, Y) , (B, A) -> (y1, y2) , (x1, x2)
        * You'll be asked to enter in the the letters one at a time in the below order
            - Enter first encrypted letter (y1): I
            - Enter second encrypted letter (y2): Y
            - Enter first decrypted letter (x1): B
            - Enter second decrypted letter (x2): A
        * After entering x2, the program will read your input back to you.
            - You entered: (I, Y, B, A)
        * Note, there is some input validation to the letters, i.e must be alphabetical character, only one letter (no bigrams). However, capitalization shouldn't matter, they'll all get capitalized in an input validation function.
    
     - Hill Cipher example,
        - (IH, YZ) , (BA, BY) -> (y1, y2) , (x1, x2)
        * You'll be asked to enter in the the bigrams one at a time in the below order
            - Enter first encrypted bigram (y1): IH
            - Enter second encrypted bigram (y2): YZ
            - Enter first decrypted bigram (x1): BA
            - Enter second decrypted bigram (x2): BY
        * After entering x2, the program will read your input back to you.
            - You entered: (IH - BA), (YZ - BY)
        * Note, there is some input validation to the bigrams, i.e must be alphabetical characters, only two letters (no single letters or trigrams). However, capitalization shouldn't matter, they'll all get capitalized in an input validation function.

6. When the last mapping is entered, (should be after entering x2), the program will attempt to solve the cipher given the mappings that you input. There is some debug / informative output corresponding to the type of cipher you're solving. If no valid key was able to be found using the input mappings, no decrypted cipher will be displayed and a short message explaining why the cipher couldn't be decrypted. For example, no modular inverse could be found, or an inverse matrix doesn't exist, etc.

7. Once the decryption has successfully printed out, or a short message explaining an error is shown, a prompt to try again will appear.
    - "Try again? [Y/n]: ", input "Y" to go back to step 5 and enter new mappings, or input "n" to quit the program.

# General Programming notes
The Affine and Hill cipher classes all inherit from a base Cipher class located in Cipher.h to make the specific cipher classes a bit easier to manage. The specific decryption implmentations are located in the AffineCipher.cpp and HillCipher.cpp files.

In the utils folder, there are some helper functions for solving Linear Congruences, specifically functions for the Extended Euclidean algorithm and a Multiplicative Inverse function. There is also a Matrix.h file used for custom matrix math using mod 26. I'd like to refine it in the future, but it gets the job done for now (I think). There is a limitation on only allowing 2x2 matrices with this Matrix implementation. The Matrix struct has a default constructor that will create a 2x2 matrix of all 0s ([(0, 0), (0, 0)]), but you can also initialize it with a vector<int> only with a size of 4, i.e. ModMatrix::Matrix<int> M({ 1, 2, 3, 4 }) which will create the matrix, ([1,2], [3,4]). I'm sure there are some bugs in this implementation, but following these general rules was able to keep the program running and the math mathing :)

 # Project Structure

.
+-- src
|   +-- AffineCipher.cpp
|   +-- AffineCipher.h
|   +-- Cipher.cpp 
|   +-- Cipher.h
|   +-- HillCipher.cpp
|   +-- HillCipher.h
+-- utils
|   +-- LinearCongruence.cpp
|   +-- LinearCongruence.h
|   +-- Matrix.h
+-- main.cpp
+-- Makefile
+-- README.md (this file)
