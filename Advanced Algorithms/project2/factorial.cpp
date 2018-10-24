// Authors: Jacob Hallman and Nedo Skobalj

#include <stdio.h>
#include <iostream>

// Own header files
#include "factorial.h"

using namespace std;

/**
 * @brief      Perform trial division
 *
 * @param[in]  n     Number to factorize
 *
 * @return     Vector of prime factors
 */
vector<mpz_class> trial_division(mpz_class n) {
	vector<mpz_class> prime_factors;
	mpz_class divisor = 2;

	while(n >= divisor * divisor) {
		if(n % divisor == 0) {
			prime_factors.push_back(divisor);
			n = n / divisor;
		} else {
			divisor++;
		}
	}

	if(n > 1)
		prime_factors.push_back(n);

	return prime_factors;
}


/**
 * @brief      Determines if prime.
 *
 * @param[in]  n     Number to check if prime
 * @param[in]  reps  Number of repetitions to perform (the higher, the more accurate)
 *
 * @return     2 if n is prime, 1 if it is prime by a certain probability, 0 if it is not prime.
 */
int is_prime(mpz_t n, int reps) {
	if(mpz_probab_prime_p(n, reps) > 0)
		return true;
	return false;
}

/**
 * @brief      Perform pollard rho.
 *
 * @param[in]  n     Number to find factor for.
 *
 * @return     A factor of n.
 */
mpz_class pollard_rho(mpz_class n) {

	if (n == 1) return n;

    if (n % 2 == 0) return 2;

	mpz_class x = 2;
	mpz_class y = 2;
	mpz_class p = 1;

	int steps = 0;
	const int steps_limit = 10000;

	do {
		y = f(f(y, n), n);
		p = gcd((mpz_class) abs(y - x), (mpz_class) n);
		// Return if we found a factor
		if (p > 1 && p < n) {
			return p;
		}
		if (isPowerOfTwo(steps)) {
			x = y;
		}
		++steps;
	} while(steps < steps_limit);

	// No factor found
	return -1;
}

/**
 * @brief      Generate a random number.
 *
 * @return     A random number between 0 and Integer max value.
 */
mpz_class random_number() {
	srand(time(0));
   	mpz_class random = rand() % std::numeric_limits<int>::max();
	return random;
}

/**
 * @brief      Generate a pseudo random number.
 *
 * @param[in]  x     { parameter_description }
 * @param[in]  n     { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
mpz_class f(mpz_class x, mpz_class n) {
	mpz_class a = random_number();
	/*
	cout << "a: " << a << endl;
	cout << "(x * x + a): " << (x * x + a) << endl;
	cout << "n: " << n << endl;
	*/
	mpz_class res = (x * x + a) % n;
	return res;
}

/**
 * @brief      Get the Greatest Common Divisor for x and y.
 *
 * @param[in]  x     First number.
 * @param[in]  y     Second number.
 *
 * @return     GCD of x and y.
 */
mpz_class gcd(mpz_class x, mpz_class y) {

	mpz_class remainder;
	while (y != 0) {
		remainder = x % y;
		x = y;
		y = remainder;
	}
	return x;

/*
	if(y == 0) {
		return x;
	} else {
		return gcd(y, x%y);
	}
	*/
}
/**
 * checks if int i is a integral power of two
 * @param  i int to test
 * @return   true if int i is a integral power of two
 */
bool isPowerOfTwo(int i) {
	int powerOfTwo = 1;
	while (powerOfTwo <= i) {
		if (powerOfTwo == i) {
			return true;
		}
		powerOfTwo *= 2;
	}
	return false;
}
