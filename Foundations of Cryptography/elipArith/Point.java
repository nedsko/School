/**
 * Author: Nedo Skobalj
 * Class to represent a point on a elliptical curve
 */

import java.math.BigInteger;

public class Point {
	// x and y of the point
	public BigInteger x;
	public BigInteger y;

	public Point(BigInteger x, BigInteger y) {
		this.x = x;
		this.y = y;
	}
}