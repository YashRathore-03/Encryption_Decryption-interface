#include <iostream>
#include <string>
#include <map>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

std::string caesar_cipher_encrypt(const std::string &text, int shift) {
    std::string result = text;
    for (char &c : result) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + shift) % 26 + base;
        }
    }
    return result;
}

std::string caesar_cipher_decrypt(const std::string &text, int shift) {
    return caesar_cipher_encrypt(text, 26 - shift);
}

std::string vigenere_cipher_encrypt(const std::string &text, const std::string &key) {
    std::string result = text;
    int keyLength = key.length();
    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(result[i])) {
            char base = isupper(result[i]) ? 'A' : 'a';
            char shift = key[i % keyLength] - base;
            result[i] = caesar_cipher_encrypt(result.substr(i, 1), shift)[0];
        }
    }
    return result;
}

std::string vigenere_cipher_decrypt(const std::string &text, const std::string &key) {
    std::string result = text;
    int keyLength = key.length();
    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(result[i])) {
            char base = isupper(result[i]) ? 'A' : 'a';
            char shift = key[i % keyLength] - base;
            result[i] = caesar_cipher_decrypt(result.substr(i, 1), shift)[0];
        }
    }
    return result;
}

bool rsa_generate_key_pair(std::string &privateKey, std::string &publicKey) {
    RSA *rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    if (rsa) {
        BIO *privateBio = BIO_new(BIO_s_mem());
        BIO *publicBio = BIO_new(BIO_s_mem());
        if (PEM_write_bio_RSAPrivateKey(privateBio, rsa, nullptr, nullptr, 0, nullptr, nullptr) &&
            PEM_write_bio_RSA_PUBKEY(publicBio, rsa)) {
            char *privateKeyData;
            long privateKeySize = BIO_get_mem_data(privateBio, &privateKeyData);
            privateKey.assign(privateKeyData, privateKeySize);

            char *publicKeyData;
            long publicKeySize = BIO_get_mem_data(publicBio, &publicKeyData);
            publicKey.assign(publicKeyData, publicKeySize);

            RSA_free(rsa);
            BIO_free_all(privateBio);
            BIO_free_all(publicBio);
            return true;
        }
        RSA_free(rsa);
        BIO_free_all(privateBio);
        BIO_free_all(publicBio);
    }
    return false;
}

std::string rsa_encrypt(const std::string &publicKey, const std::string &message) {
    RSA *rsa = RSA_new();
    BIO *publicBio = BIO_new_mem_buf(publicKey.c_str(), -1);
    PEM_read_bio_RSA_PUBKEY(publicBio, &rsa, nullptr, nullptr);
    int rsaSize = RSA_size(rsa);
    unsigned char *encrypted = new unsigned char[rsaSize];
    int result = RSA_public_encrypt(message.length(), reinterpret_cast<const unsigned char *>(message.c_str()), encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    std::string encryptedMessage;
    if (result != -1) {
        encryptedMessage.assign(reinterpret_cast<const char *>(encrypted), result);
    }
    RSA_free(rsa);
    delete[] encrypted;
    return encryptedMessage;
}

std::string rsa_decrypt(const std::string &privateKey, const std::string &encryptedMessage) {
    RSA *rsa = RSA_new();
    BIO *privateBio = BIO_new_mem_buf(privateKey.c_str(), -1);
    PEM_read_bio_RSAPrivateKey(privateBio, &rsa, nullptr, nullptr);
    int rsaSize = RSA_size(rsa);
    unsigned char *decrypted = new unsigned char[rsaSize];
    int result = RSA_private_decrypt(encryptedMessage.length(), reinterpret_cast<const unsigned char *>(encryptedMessage.c_str()), decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    std::string decryptedMessage;
    if (result != -1) {
        decryptedMessage.assign(reinterpret_cast<const char *>(decrypted), result);
    }
    RSA_free(rsa);
    delete[] decrypted;
    return decryptedMessage;
}

int main() {
    std::map<std::string, std::string> user_data;

    while (true) {
        std::cout << "\n1. Register" << std::endl;
        std::cout << "2. Login" << std::endl;
        std::cout << "3. Quit" << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string username;
            std::cout << "Enter a username: ";
            std::cin >> username;

            std::string password = "password"; // Change this to generate a random password.
            user_data[username] = password;

            std::cout << "Registration successful. Your password is: " << password << std::endl;
        } else if (choice == 2) {
            std::string username;
            std::string password;

            std::cout << "Enter your username: ";
            std::cin >> username;
            std::cout << "Enter your password: ";
            std::cin >> password;

            if (user_data.find(username) != user_data.end() && user_data[username] == password) {
                std::cout << "Login successful." << std::endl;

                while (true) {
                    std::cout << "\n1. Encrypt" << std::endl;
                    std::cout << "2. Decrypt" << std::endl;
                    std::cout << "3. Logout" << std::endl;

                    int sub_choice;
                    std::cin >> sub_choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (sub_choice == 1) {
                        std::string message;
                        std::cout << "Enter the message to encrypt: ";
                        std::getline(std::cin, message);

                        std::cout << "\nSelect an encryption method:" << std::endl;
                        std::cout << "1. Caesar Cipher" << std::endl;
                        std::cout << "2. Vigenère Cipher" << std::endl;
                        std::cout << "3. RSA" << std::endl;

                        int encryption_method;
                        std::cin >> encryption_method;

                        if (encryption_method == 1) {
                            int shift;
                            std::cout << "Enter the Caesar Cipher shift: ";
                            std::cin >> shift;
                            std::string encrypted_message = caesar_cipher_encrypt(message, shift);
                            std::cout << "Encrypted message: " << encrypted_message << std::endl;
                        } else if (encryption_method == 2) {
                            std::string key;
                            std::cout << "Enter the Vigenère Cipher key: ";
                            std::cin >> key;
                            std::string encrypted_message = vigenere_cipher_encrypt(message, key);
                            std::cout << "Encrypted message: " << encrypted_message << std::endl;
                        } else if (encryption_method == 3) {
                            std::string private_key, public_key;
                            if (rsa_generate_key_pair(private_key, public_key)) {
                                std::string encrypted_message = rsa_encrypt(public_key, message);
                                std::cout << "Public key:\n" << public_key << std::endl;
                                std::cout << "Encrypted message: " << encrypted_message << std::endl;
                            }
                        } else {
                            std::cout << "Invalid option. Please try again." << std::endl;
                        }
                    } else if (sub_choice == 2) {
                        std::string encrypted_message;
                        std::cout << "Enter the message to decrypt: ";
                        std::cin.ignore();
                        std::getline(std::cin, encrypted_message);

                        std::cout << "\nSelect a decryption method:" << std::endl;
                        std::cout << "1. Caesar Cipher" << std::endl;
                        std::cout << "2. Vigenère Cipher" << std::endl;
                        std::cout << "3. RSA" << std::endl;

                        int decryption_method;
                        std::cin >> decryption_method;

                        if (decryption_method == 1) {
                            int shift;
                            std::cout << "Enter the Caesar Cipher shift: ";
                            std::cin >> shift;
                            std::string decrypted_message = caesar_cipher_decrypt(encrypted_message, shift);
                            std::cout << "Decrypted message: " << decrypted_message << std::endl;
                        } else if (decryption_method == 2) {
                            std::string key;
                            std::cout << "Enter the Vigenère Cipher key: ";
                            std::cin >> key;
                            std::string decrypted_message = vigenere_cipher_decrypt(encrypted_message, key);
                            std::cout << "Decrypted message: " << decrypted_message << std::endl;
                        } else if (decryption_method == 3) {
                            std::cout << "RSA decryption is not supported in this example." << std::endl;
                        } else {
                            std::cout << "Invalid option. Please try again." << std::endl;
                        }
                    } else if (sub_choice == 3) {
                        std::cout << "Logged out." << std::endl;
                        break;
                    } else {
                        std::cout << "Invalid option. Please try again." << std::endl;
                    }
                }
            } else {
                std::cout << "Invalid username or password. Please try again." << std::endl;
            }
        } else if (choice == 3) {
            std::cout << "Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}
