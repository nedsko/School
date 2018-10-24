#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <vector>

// GMP
#include <gmpxx.h>
#include <gmp.h>

std::vector<long> trial_division(mpz_t);
std::vector<mpz_class> trial_division(mpz_class);

// 2 = Definitely prime, 1 = Probably prime, 0 = Definitely not prime
int is_prime(mpz_t, int);

mpz_class pollard_rho(mpz_class);

mpz_class random_number();

mpz_class f(mpz_class, mpz_class);

mpz_class gcd(mpz_class, mpz_class);

bool isPowerOfTwo(int);

#endif