### Program Overview and Functionalities

This C++ program is designed as an interactive encryption and decryption interface that includes user registration and login functionalities. It supports three types of ciphers: Caesar, Vigenère, and Playfair. Below is a detailed explanation of each component and its role in the program.

#### Libraries and Definitions:
- **Libraries:** The program includes standard libraries for input/output operations, string manipulation, and using maps.
- **Constant Definitions:** `SIZE` is defined as 100, used for array size declarations.

#### Caesar Cipher:
1. **Encryption (`caesar_enc`):**
   - This function shifts each letter of the input text by a specified number of positions (`s`) in the alphabet. It handles both uppercase and lowercase letters separately to maintain case.

2. **Decryption (`caesar_dec`):**
   - Decryption is achieved by using the same encryption function with a shift value of `26 - s`, effectively reversing the encryption process.

#### Vigenère Cipher:
1. **Encryption (`vigenere_enc`):**
   - This function encrypts the input text using a repeating key to shift each character. The shift for each character is determined by the corresponding character in the key.

2. **Decryption (`vigenere_dec`):**
   - Decryption is performed similarly, using the key to reverse the shifts applied during encryption.

#### Playfair Cipher:
1. **Helper Functions:**
   - Various helper functions handle tasks such as converting text to lowercase, removing spaces, generating the key table, searching for character positions in the key table, preparing text for encryption, and performing modular arithmetic for wrapping around the table.

2. **Encryption (`encryptByPlayfairCipher`):**
   - This function encrypts the input text using the Playfair cipher method, which involves creating a 5x5 key table and encrypting digraphs (pairs of letters).

3. **Decryption (`decryptByPlayfairCipher`):**
   - Decryption follows the same principles as encryption but reverses the steps to retrieve the original text.

#### Main Program:
1. **User Interface:**
   - The main program provides an interactive user interface with options for registration, login, encryption, decryption, and quitting the program.

2. **User Registration and Login:**
   - Users can register by providing a username and password, which are stored in a map (`user_data`). Registered users can log in by providing their credentials.

3. **Encryption and Decryption Options:**
   - After logging in, users can choose to encrypt or decrypt messages.
   - Users select the encryption or decryption method (Caesar, Vigenère, or Playfair) and provide the necessary inputs (message, shift/key).

4. **Encryption/Decryption Process:**
   - Depending on the user's choice, the program calls the appropriate encryption or decryption function, processes the input, and displays the result.

5. **Logout and Exit:**
   - Users can log out to return to the main menu or choose to exit the program.

### Summary
The program effectively combines user management with encryption and decryption functionalities. It allows users to register, log in, and securely encrypt/decrypt messages using three different cipher techniques. The interface guides users through selecting the appropriate method and entering the required data, making it an educational tool for understanding basic cryptographic principles.
