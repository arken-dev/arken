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

  if ( ! os::exists(fileName) ) {
    throw "file credential not exists";
  } else{
    //std::cout << "estou aqui" << std::endl;
    string value = os::read(fileName);
    //std::cout << "size " << value.size() << "dpeois do read" << value;
    string data;
    try {
      data  = aes::decrypt(value, ARKEN_CRYPTO_AES_KEY);
    } catch (const char *msg) {
      std::cout << "deu ruim " << msg << std::endl;
      return;
    }
    //std::cout << "decript----------------------" << std::endl;
    //std::cout << data << std::endl;
    List   list  = data.split("\n");

    for( int i=0; i< list.size(); i++ ) {
      string row = list.at(i);
      if(row.indexOf("=") != -1) {
        string key = row.prefix("=").squish();
        string val = row.suffix("=").squish();
        //std::cout << "key " << key << "value:" << val << std::endl;
        s_credential->insert({key, val});
      }
    }
  }
  //std::cout << "funcionou: " << crypto::Credential::get("alexandre") << std::endl;
  std::cout << "este é a senha meli_1" << crypto::Credential::get("meli_1") << std::endl;
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
