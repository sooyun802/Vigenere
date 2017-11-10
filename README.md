Program Environment: Ubuntu 14.04 LTS

## Breaking a Vigenere Variant Cipher

* Breaks a Vigenere variant cipher where byte-wise XOR is used instead of addition modulo 26
* Prints out the key and the plaintext decrypted from a given cipher text in the input file named "input.txt" into the output file named "output.txt"
* The key length will be less than or equal to 10 bytes; the plaintext will be between 1,000 and 5,000 bytes.

## Methodology

* The methodology of breaking a Vigenere variant cipher is divided into two parts
	* (1) finding the length of the key
	* (2) finding the actual key
* Finding the length of the key
	* Use Index of Coincidence (I.C.) to find the distributions of the ciphertext
	* Try calculating the I.C. value for every possible key length
	* The key length with the highest I.C. value will be the answer
* Finding the actual key
	* Use chi-squared statistics to compare the distributions of two different data sets
	* The chi-squared statistic value will be close to 0 as more as the two data sets possess similar distributions

## Command Line Arguments
```
$ gcc VigenereCipher.c
$ ./a.out
```
