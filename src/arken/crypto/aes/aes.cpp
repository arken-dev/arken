// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/crypto/aes.h>
#include <cstring>
#include <utility>

extern "C" {
#include <openssl/evp.h>
#include <openssl/rand.h>
}

namespace arken {
namespace crypto {

static const EVP_CIPHER *cipher_for(int bits)
{
  switch (bits) {
    case 128: return EVP_aes_128_cbc();
    case 192: return EVP_aes_192_cbc();
    case 256: return EVP_aes_256_cbc();
    default:  throw "aes: invalid bits, expected 128, 192 or 256";
  }
}

string aes::encrypt(string value, const char *password, int bits)
{
  std::cout << "teste " << ARKEN_CRYPTO_AES_KEY << std::endl;
  auto cipher = cipher_for(bits);
  int ivLength = EVP_CIPHER_iv_length(cipher);

  unsigned char key[EVP_MAX_KEY_LENGTH];
  unsigned char iv[EVP_MAX_IV_LENGTH];

  if (EVP_BytesToKey(cipher, EVP_sha256(), nullptr,
        reinterpret_cast<const unsigned char *>(password), strlen(password),
        1, key, nullptr) == 0)
    throw "aes: failed to derive key from password";

  if (RAND_bytes(iv, ivLength) != 1)
    throw "aes: failed to generate iv";

  auto ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
    throw "aes: failed to create cipher context";

  auto buffer = new char[ivLength + value.size() + EVP_MAX_BLOCK_LENGTH];
  memcpy(buffer, iv, ivLength);

  int len = 0, total = ivLength;

  if (EVP_EncryptInit_ex(ctx, cipher, nullptr, key, iv) != 1 ||
      EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(buffer) + total, &len,
        reinterpret_cast<const unsigned char *>(value.data()), value.size()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    delete[] buffer;
    throw "aes: encrypt failed";
  }
  total += len;

  if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(buffer) + total, &len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    delete[] buffer;
    throw "aes: encrypt failed";
  }
  total += len;

  EVP_CIPHER_CTX_free(ctx);
  return string(std::move(buffer), total);
}

string aes::decrypt(string value, const char *password, int bits)
{
  auto cipher = cipher_for(bits);
  int ivLength = EVP_CIPHER_iv_length(cipher);

  if (value.size() < static_cast<size_t>(ivLength))
    throw "aes: decrypt failed, wrong password or corrupted data";

  unsigned char key[EVP_MAX_KEY_LENGTH];
  auto iv = reinterpret_cast<const unsigned char *>(value.data());
  auto ciphertext = reinterpret_cast<const unsigned char *>(value.data()) + ivLength;
  size_t ciphertextLength = value.size() - ivLength;

  if (EVP_BytesToKey(cipher, EVP_sha256(), nullptr,
        reinterpret_cast<const unsigned char *>(password), strlen(password),
        1, key, nullptr) == 0)
    throw "aes: failed to derive key from password";

  auto ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
    throw "aes: failed to create cipher context";

  auto buffer = new char[ciphertextLength + EVP_MAX_BLOCK_LENGTH];
  int len = 0, total = 0;

  if (EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv) != 1 ||
      EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(buffer), &len,
        ciphertext, ciphertextLength) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    delete[] buffer;
    throw "aes: decrypt failed, wrong password or corrupted data";
  }
  total = len;

  if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(buffer) + total, &len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    delete[] buffer;
    throw "aes: decrypt failed, wrong password or corrupted data";
  }
  total += len;

  EVP_CIPHER_CTX_free(ctx);
  return string(std::move(buffer), total);
}

} // namespace crypto
} // namespace arken
