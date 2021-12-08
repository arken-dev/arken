// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include <arken/base>
#include <arken/base64.h>

namespace arken {

char * base64::encode(const char *input, int length)
{
  BIO *buff, *b64;
  BUF_MEM *bptr;

  b64 = BIO_new(BIO_f_base64());
  buff = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, buff);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr); //NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

  auto result = new char[bptr->length];
  memcpy(result, bptr->data, bptr->length-1);
  result[bptr->length-1] = 0;
  BIO_free_all(b64);

  return result;
}

char * base64::decode(const char *input, size_t * size)
{
  BIO *b64, *buff;
  size_t length = strlen(input);

  b64 = BIO_new(BIO_f_base64());
  buff = BIO_new_mem_buf((void*) input, length);
  buff = BIO_push(b64, buff);

  auto buffer = new char[length];
  memset(buffer, 0, length);
  BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
  BIO_set_close(buff, BIO_CLOSE);

  (*size) = BIO_read(buff, buffer, length);
  BIO_free_all(buff);

  return buffer;
}

} // namespace arken
