#include "passwordrecord.h"

PasswordRecord::PasswordRecord()
{

}

void PasswordRecord::crypt()
{
    site = encu(site);
    user = encu(user);
    password = encu(password);
    additional = encu(additional);
}

void PasswordRecord::decrypt()
{
    site = decu(site);
    user = decu(user);
    password = decu(password);
    additional = decu(additional);
}

QString PasswordRecord::encu(QString password)
{
    string plain = password.toStdString();
    string ciphertext;
    // Hex decode symmetric key:
    HexDecoder decoder;
    decoder.Put( (byte *)PRIVATE_KEY, 32*2 );
    decoder.MessageEnd();
    word64 size = decoder.MaxRetrievable();
    char *decodedKey = new char[size];
    decoder.Get((byte *)decodedKey, size);
    // Generate Cipher, Key, and CBC
    byte key[ AES::MAX_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(decodedKey), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::MAX_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    CBC_Mode<AES>::Encryption Encryptor( key, sizeof(key), iv );
    StringSource( plain, true, new StreamTransformationFilter( Encryptor,
                  new HexEncoder(new StringSink( ciphertext ) ) ) );
    return QString::fromStdString(ciphertext);
}

QString PasswordRecord::decu(QString password)
{
    string plain;
    string encrypted = password.toStdString();
    // Hex decode symmetric key:
    HexDecoder decoder;
    decoder.Put( (byte *)PRIVATE_KEY,32*2 );
    decoder.MessageEnd();
    word64 size = decoder.MaxRetrievable();
    char *decodedKey = new char[size];
    decoder.Get((byte *)decodedKey, size);
    // Generate Cipher, Key, and CBC
    byte key[ AES::MAX_KEYLENGTH ], iv[ AES::BLOCKSIZE ];
    StringSource( reinterpret_cast<const char *>(decodedKey), true,
                  new HashFilter(*(new SHA256), new ArraySink(key, AES::MAX_KEYLENGTH)) );
    memset( iv, 0x00, AES::BLOCKSIZE );
    try {
        CBC_Mode<AES>::Decryption Decryptor
        ( key, sizeof(key), iv );
        StringSource( encrypted, true,
                      new HexDecoder(new StreamTransformationFilter( Decryptor,
                                     new StringSink( plain ) ) ) );
    }
    catch (Exception &e) { // ...
    }
    catch (...) { // ...
    }
    return QString::fromStdString(plain);
}
