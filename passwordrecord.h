#ifndef PASSWORDRECORD_H
#define PASSWORDRECORD_H

#define PRIVATE_KEY "8293B8A8E215F6A44EB3A2AE6D1463CA7B64A0E2E11BCF6A68116FC7697E727A37011"

#include <QString>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/md5.h>

#include <cstring>
using namespace std;
using namespace CryptoPP;
class PasswordRecord
{
public:
    PasswordRecord();
    void crypt();
    void decrypt();

    int id;
    QString site;
    QString user;
    QString password;
    QString additional;
private:
    QString encu(QString);
    QString decu(QString);
};

#endif // PASSWORDRECORD_H
