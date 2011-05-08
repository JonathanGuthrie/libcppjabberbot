/*
 * Copyright 2010 Jonathan R. Guthrie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(_SOCKET_HPP_INCLUDED_)
#define _SOCKET_HPP_INCLUDED_

#include <string>
#include <exception>

#include <stdint.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class SocketSocketErrorException {
};

class SocketConnectErrorException {
};

class SocketSSLErrorException : public std::exception {
private:
    const std::string message;
    
public:
    SocketSSLErrorException(const std::string error) : message(error) {
    }

    virtual const char* what() const throw() {
	return message.c_str();
    }

    virtual ~SocketSSLErrorException() throw() {
    }
};


struct CRYPTO_dynlock_value {
    pthread_mutex_t mutex;
};

class Socket {
private:
    static SSL_CTX *ctx;
    static pthread_mutex_t *ssl_static_mutexes;
    BIO *bio;
    SSL *ssl;
    bool isSecure;
    static void static_locking_callback(int mode, int n, const char *file, int line);
    static unsigned long static_thread_id_callback(void);
    static struct CRYPTO_dynlock_value *dynamic_mutex_create_callback(const char *file, int line);
    static void dynamic_mutex_lock_callback(int mode, struct CRYPTO_dynlock_value *lock, const char *file, int line);
    static void dynamic_mutex_destroy_callback(struct CRYPTO_dynlock_value *lock, const char *file, int line);
    void init(const std::string &destination, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException);

public:
    Socket(const std::string &host, unsigned short port, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException);
    Socket(const std::string &destination, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException);
    ~Socket();
    ssize_t Receive(uint8_t *buffer, size_t size);
    ssize_t Send(const std::string &string);
    ssize_t Send(const uint8_t *data, int length);
    bool GoEncrypted() throw(SocketSSLErrorException);
    bool IsEncrypted() const { return isSecure; }
};

#endif // _SOCKET_HPP_INCLUDED_
