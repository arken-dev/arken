#include <arken/base>
#include <arken/crypto/credential.h>
#include <arken/crypto/aes_key.h>
#include <iostream>
#include <fstream>

#ifndef ARKEN_CRYPTO_AES_KEY
#define ARKEN_CRYPTO_AES_KEY ""
#endif


namespace arken {


std::unordered_map<string, string> * s_credential = new std::unordered_map<string, string>;

void crypto::Credential::save(string fileName)
{
  using aes    = arken::crypto::aes;

  if ( ! os::exists(fileName) ) {
    throw "file not exists";
  } else{
    string value = os::read(fileName);
    string data;
    try {
      data  = aes::encrypt(value, ARKEN_CRYPTO_AES_KEY);
    } catch (const char *msg) {
      std::cout << "deu ruim " << msg << std::endl;
      return;
    }

    fileName.append(".cred");
    std::ofstream my_file(fileName.data());

    // 2. Check if the file opened successfully
    if (!my_file.is_open()) {
        throw "Error opening the file!";
    }

    // 3. Write data to the file using the insertion operator (<<)
    my_file << data ;

    // 4. Close the file to free up system resources
    my_file.close();
  }
}

void crypto::Credential::load(string fileName)
{

  using aes    = arken::crypto::aes;
  using string = arken::string;
  using List   = arken::string::List;

  if ( ! os::exists(fileName) ) {
    throw "file credential not exists";
  } else{
	  std::cout << "estou aqui" << std::endl;
    string value = os::read(fileName);
    std::cout << "size " << value.size() << "dpeois do read" << value;
    string data;
    try {
      data  = aes::decrypt(value, ARKEN_CRYPTO_AES_KEY);
    } catch (const char *msg) {
      std::cout << "deu ruim " << msg << std::endl;
      return;
    }
    std::cout << "decript----------------------" << std::endl;
    std::cout << data << std::endl;
    List   list  = data.split("\n");

    for( int i=0; i< list.size(); i++ ) {
      string row = list[i];
      string key = row.prefix("=").squish();
      string val = row.suffix("=").squish();
      std::cout << "key " << key << "value:" << val << std::endl;
      s_credential->insert({key, val});
    }
  }
  std::cout << "\n\nfuncionou (TESTE): " << crypto::Credential::get("TESTE") << std::endl;
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
