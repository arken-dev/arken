#include <arken/base>
#include <arken/crypto/credential.h>
#include <arken/crypto/aes_key.h>

#ifndef ARKEN_CRYPTO_AES_KEY
#define ARKEN_CRYPTO_AES_KEY ""
#endif


namespace arken {


std::unordered_map<string, string> * s_credential = new std::unordered_map<string, string>;

void crypto::Credential::load(string fileName)
{

  using aes    = arken::crypto::aes;
  using string = arken::string;
  using List   = arken::string::List;



  if ( os::exists(fileName) ) {
    throw "file credential not exists";
  } else{
    string value = os::read(fileName);
    string data  = aes::decrypt(value, ARKEN_CRYPTO_AES_KEY);
    List   list  = data.split("\n");

    for( int i=0; i< list.size(); i++ ) {
      string row = list[i];
      string key = row.prefix("=").squish();
      string val = row.suffix("=").squish();
      s_credential->insert({key, val});
    }
  }
}


const char * crypto::Credential::get(string key)
{
  if (s_credential->find(key) != s_credential->end()) {
    return s_credential->at(key).data();
  } else {
    return nullptr;
  }
}

} // arken
