#include "socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/engine.h>
#include <iostream>
#include <sstream>


int cert_verify_callback(int ok, X509_STORE_CTX *store) {
    char data[256];
    if (!ok) {
	X509 *cert = X509_STORE_CTX_get_current_cert(store);
	int depth = X509_STORE_CTX_get_error_depth(store);
	int err = X509_STORE_CTX_get_error(store);

	// SYZYGY -- this needs to get logged
	std::cerr << "-Error with certificate at depth: " << depth << std::endl;
	std::cerr << "  issuer = " << X509_NAME_oneline(X509_get_issuer_name(cert), data, 256) << std::endl;
	std::cerr << "  subject = " << X509_NAME_oneline(X509_get_subject_name(cert), data, 256) << std::endl;
	std::cerr << "  err " << err << ":" << X509_verify_cert_error_string(err) << std::endl;
    }
    return ok;
}


long post_connection_check(SSL *ssl, char *host) {
    // SYZYGY -- they have GOT to be kidding!

    return SSL_get_verify_result(ssl);
}


struct CRYPTO_dynlock_value *Socket::dynamic_mutex_create_callback(const char *file, int line) {
    struct CRYPTO_dynlock_value *result;

    result = new CRYPTO_dynlock_value;
    pthread_mutex_init(&result->mutex, NULL);
    return result;
}


void Socket::dynamic_mutex_lock_callback(int mode, struct CRYPTO_dynlock_value *lock, const char *file, int line) {
    if (0 != (mode & CRYPTO_LOCK)) {
	pthread_mutex_lock(&lock->mutex);
    }
    else {
	pthread_mutex_unlock(&lock->mutex);
    }
}


void Socket::dynamic_mutex_destroy_callback(struct CRYPTO_dynlock_value *lock, const char *file, int line) {
    pthread_mutex_destroy(&lock->mutex);
    delete lock;
}


void Socket::static_locking_callback(int mode, int n, const char *file, int line) {
    if (0 != (mode & CRYPTO_LOCK)) {
	pthread_mutex_lock(ssl_static_mutexes+n);
    }
    else {
	pthread_mutex_unlock(ssl_static_mutexes+n);
    }
}


unsigned long Socket::static_thread_id_callback(void) {
    return pthread_self();
}


// #define CERTFILE  "/etc/ssl/certs/ca-certificates.crt"
#define CERTFILE NULL
#define CAPATH "/etc/ssl/certs"
#define SERVER "jabber.brokersys.com"
#define CIPHER_LIST "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"

SSL_CTX *Socket::ctx = NULL;
pthread_mutex_t *Socket::ssl_static_mutexes;

void Socket::init(const std::string &destination, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException) {
    char *d;

    if (NULL == ctx) {
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	if (!RAND_load_file("/dev/random", 4)) {
	    throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

	// Set up locking
	ssl_static_mutexes = new pthread_mutex_t[CRYPTO_num_locks()];
	for (int i=0; i<CRYPTO_num_locks(); ++i) {
	    pthread_mutex_init(ssl_static_mutexes+i, NULL);
	}
	CRYPTO_set_id_callback(static_thread_id_callback);
	CRYPTO_set_locking_callback(static_locking_callback);

	CRYPTO_set_dynlock_create_callback(dynamic_mutex_create_callback);
	CRYPTO_set_dynlock_lock_callback(dynamic_mutex_lock_callback);
	CRYPTO_set_dynlock_destroy_callback(dynamic_mutex_destroy_callback);

	if (NULL == (Socket::ctx = SSL_CTX_new(SSLv23_method()))) {
	    throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

	if (1 != SSL_CTX_load_verify_locations(ctx, CERTFILE, CAPATH)) {
	    throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT, cert_verify_callback);
	SSL_CTX_set_options(ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2);
	if (1 != SSL_CTX_set_cipher_list(ctx, CIPHER_LIST)) {
	    throw SocketSSLErrorException("Error setting cipher list (no valid ciphers)");
	}
    }

    d = new char[destination.size()+1];
    strcpy(d, destination.c_str());

    if (NULL == (bio = BIO_new_connect(d))) {
	throw SocketConnectErrorException();
    }

    if (0 >= BIO_do_connect(bio)) {
	throw SocketConnectErrorException();
    }

    isSecure = startEncrypted;
    if (startEncrypted) {
	if (!(ssl = SSL_new(ctx))) {
	    throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

	SSL_set_bio(ssl, bio, bio);
	if (0 >= SSL_connect(ssl)) {
	    throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

	int err;
	if ((err = post_connection_check(ssl, (const char *)SERVER)) != X509_V_OK) {
	  // SYZYGY -- log
	  std::cerr << "-Error: peer certificate: " << X509_verify_cert_error_string(err) << std::endl;
	  throw SocketSSLErrorException(ERR_error_string(ERR_get_error(), NULL));
	}

    }

    delete[] d;
}

Socket::Socket(const std::string &host, unsigned short port, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException) {
    std::ostringstream destination;

    destination << host << ":" << port;
    init(destination.str(), startEncrypted);
}

Socket::Socket(const std::string &destination, bool startEncrypted) throw(SocketSocketErrorException, SocketConnectErrorException, SocketSSLErrorException) {
    init(destination, startEncrypted);
}


Socket::~Socket() {
    if (isSecure) {
	SSL_shutdown(ssl);

	SSL_free(ssl);
    }
    else {
	BIO_reset(bio);

	BIO_free_all(bio);
    }
}

ssize_t Socket::Send(const std::string &string) {
  std::cout << "Sending: \"" << string << "\"" << std::endl;
  Send((uint8_t *) string.c_str(), string.size());
}

ssize_t Socket::Send(const uint8_t *data, size_t length) {
    int result = 0;

    if (isSecure) {
	size_t err = 0;

	for (result=0; result< length; ) {
	    err = SSL_write(ssl, data + result, length - result);
#if 0
	    // std::cerr << "Send returned: " << err << std::endl;
	    // std::cerr << "Send returned an error of code: " << SSL_get_error(ssl, err) << std::endl;
#endif // 0
	    
	    switch(SSL_get_error(ssl, err)) {
	    case SSL_ERROR_NONE:
		result += err;
		break;

	    case SSL_ERROR_ZERO_RETURN:
		// SYZYGY -- cleanup exit because the connection is closed
		return 0;
		break;

	    case SSL_ERROR_WANT_READ:
	    case SSL_ERROR_WANT_WRITE:
		// first and ten, do it again
		break;

	    default:
		// ERROR!  ABORT!  ABORT!  ABORT!
		// SYZYGY -- throw exception
		SSL_clear(ssl);
		return err;
	    }
	}
    }
    else {
	int result = BIO_write(bio, data, length);
	if (0 >= result) {
	    if (BIO_should_retry(bio)) {
		BIO_write(bio, data, length);
	    }
	}
    }
    return result;
}

ssize_t Socket::Receive(uint8_t *buffer, size_t size) {
    int result = 0;
    if (isSecure) {
	int code = SSL_read(ssl, buffer, size);
	switch(SSL_get_error(ssl, code)) {
	case SSL_ERROR_NONE:
	    result = code;
	    break;

	case SSL_ERROR_ZERO_RETURN:
	    result = -1;
	    break;

	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
	    // first and ten, do it again
	    result = 0;
	    break;

	default:
	    // ERROR!  ABORT!  ABORT!  ABORT!
	    // SYZYGY -- throw exception
	    SSL_clear(ssl);
	    result = code;
	    break;
	}
    }
    else {
	result = BIO_read(bio, buffer, size);
	if (0 < result) {

	}
	else {
	    if (0 == result) {
		// The connection is closed
	    }
	    else {
		if (BIO_should_retry(bio)) {
		    //retry 
		}
		else {
		    // SYZYGY throw exception
		}
	    }
	}
    }
    return result;
}

