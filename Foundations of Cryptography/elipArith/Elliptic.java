/**
 * Author: Nedo Skobalj
 * Eliptic curve arithmetic for homework 2 in the course Introduction To Cryptography at KTH
 */

import java.math.BigInteger;
import java.util.Scanner;


public class Elliptic {
	// Predetermined constants of the prime192v3 curve
	private static BigInteger gx = new BigInteger("7d29778100c65a1da1783716588dce2b8b4aee8e228f1896", 16);
	private static BigInteger gy = new BigInteger("38a90f22637337334b49dcb66a6dc8f9978aca7648a943b0", 16);
	private static BigInteger a  = new BigInteger("fffffffffffffffffffffffffffffffefffffffffffffffc", 16);
	private static BigInteger b  = new BigInteger("22123dc2395a05caa7423daeccc94760a7d462256bd56916", 16);
	private static BigInteger p  = new BigInteger("fffffffffffffffffffffffffffffffeffffffffffffffff", 16);
	private static BigInteger q  = new BigInteger("ffffffffffffffffffffffff7a62d031c83f4294f640ec13", 16);

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		Point gen = new Point(gx, gy); // Point we are working on
		while (sc.hasNext()) { // Work while there are test cases left
			sc.skip("0x"); // We don't need the "0x" part of the input
			BigInteger e = sc.nextBigInteger(16); // Read exponent input
			Point result = doubleAndAdd(e, gen); // Calculate e*gen
			System.out.println("0x" + result.x.toString(16) + " 0x" + result.y.toString(16)); // Print out in correct format
			sc.skip("\\n"); // Make sure to hop to the next line
		}

	}
	/**
	 * Calculates e*point using the well-known double-and-add algorithm.
	 * The algorithm works very similarly to the squareand-multiply algorithm
	 * @param  e     number of times to multiply the point
	 * @param  point the point to multiply
	 * @return       the point one gets after "multiplying" e with point 
	 */
	public static Point doubleAndAdd(BigInteger e, Point point) {
		// The algorithm works by looking at the bit pattern of e and then deciding
		// which operations are to be done depending on if the bit is a 0 or 1.
		Point resPoint = new Point(new BigInteger("0", 10), new BigInteger("0", 10));
		int bitLength = e.bitLength();
		for (int i = bitLength-1; i >= 0; i--) {
			if (resPoint.x.compareTo(BigInteger.ZERO) != 0 && resPoint.y.compareTo(BigInteger.ZERO) != 0) {
				resPoint = dub(resPoint);
			}
			if (e.testBit(i)) { // If the bit at position i is equal to 1
				if (resPoint.x.compareTo(BigInteger.ZERO) == 0 && resPoint.y.compareTo(BigInteger.ZERO) == 0) {
					resPoint = point;
				} else {
					resPoint = add(resPoint, point);
				}
			}
		}
		return resPoint;
	}

	/**
	 * Performs addition on two distict points on the curve. 
	 * @param  x first point
	 * @param  y second point
	 * @return   point one gets from the addition
	 */
	public static Point add(Point p1, Point p2) {
		// Check that p1 and p2 are not the same point
		if (!p1.x.equals(p2.x) && !p1.y.equals(p2.y)) {
			// Calculate "slope"
			BigInteger m = p1.y.subtract(p2.y);
			BigInteger demoInverse = p1.x.subtract(p2.x).modInverse(p);
			m = m.multiply(demoInverse);
			// Calculate new points
			BigInteger resultX = m.pow(2).subtract(p1.x).subtract(p2.x).mod(p);
			BigInteger resultY = m.multiply(resultX.subtract(p1.x)).add(p1.y).mod(p);
			resultY = resultY.negate().mod(p); // must negate result for new point Y because additon is defined as P + Q = -R
			Point resultPoint = new Point(resultX, resultY);
			return resultPoint;
		} else {
			// Use doubling instead if we are trying to add the same points
			return dub(p1);
		}
	}
	/**
	 * Doubles the point x (basically x + x)
	 * @param  x point top be doubled
	 * @return   Resulting point from doubling
	 */
	public static Point dub(Point point) {
		BigInteger big3 = new BigInteger("3", 10);
		BigInteger big2 = new BigInteger("2", 10);
		// Calculate the value of m aka. the "slope" of the curve.
		// Start with "numerator" or the left factor.
		BigInteger factor1 = point.x.pow(2).multiply(big3).add(a);
		// Then "denominator" or right factor. Don't want to call it denominator since no division is done.
		BigInteger factor2 = point.y.multiply(big2).modInverse(p);
		// Calculate m
		BigInteger m = factor1.multiply(factor2).mod(p);
		// Calculate the two points
		BigInteger resultX = m.pow(2).subtract(point.x).subtract(point.x).mod(p);
		BigInteger resultY = m.multiply(resultX.subtract(point.x)).add(point.y).mod(p);
		resultY = resultY.negate().mod(p); // must negate result for new point Y because additon is defined as P + Q = -R
		Point resultPoint = new Point(resultX, resultY);
		return resultPoint;
	}
}