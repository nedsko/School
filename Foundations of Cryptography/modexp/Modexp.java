// Implementation of modular exponentiation from modular multiplication
// Author: Nedo Skobalj

import java.math.BigInteger;
import java.util.Scanner;

public class Modexp2 {
	public static void main(String[] args) {
		// Define variables needed inside the algorithm
		Scanner sc = new Scanner(System.in); // For reading input
		BigInteger bigOne = new BigInteger("1"); // Just the value 1 in BigInteger form.
		BigInteger bigZero = new BigInteger("0"); // Just the value 0 in BigInteger form.
		BigInteger base, exp, intMod, ans;
		int index;
		// Run algorithm as long as new test cases are coming in.
		while(sc.hasNext()) {
			// read input and initialize values.
			base = sc.nextBigInteger();
			exp = sc.nextBigInteger();
			intMod = sc.nextBigInteger();
			// Any value mod 1 is always equal to 0, and 0 mod anything is always equal to 0.
			// No need to compute answer if this is the case.
			if (intMod.compareTo(bigOne)==0  || base.compareTo(bigZero)==0) {
				System.out.println("0");
				continue;
			}
			// Here I used the square-and-multiply algorithm.
			ans = bigOne;
			index = getHighestSetBit(exp); // see below
			// run until(and including) we get to the least significant bit(index 0).
			while (index >= 0) {
				// We always have to square, no matter if the bit is 0 or 1.
				ans = (ans.multiply(ans)).mod(intMod);
				// If the bit we are "on" now is set to 1, then we have to multiply
				// with the base as well.
				if (exp.testBit(index)) {
					ans = (ans.multiply(base)).mod(intMod);
				}
				index--;
			}
			// Print out answer on new line.
			System.out.println(ans.toString());
		}
	}

	/**
	 * returns index of left most(most significant) bit set to 1 in num.
	 * @param  num 		number to be examined
	 * @return index of left most bit set to 1 in num, otherwise -1.
	 */
	private static int getHighestSetBit(BigInteger num) {
		// We work backwards, meaning we check the most significant bit first
		// then we move on to the second most significant and so on.
		for (int i=num.bitLength()-1; i >= 0; i--) {
			if (num.testBit(i)) { // returns true if bit at index i == 1.
				return i;
			}
		}
		// No set bit found.
		return -1;
	}
}
