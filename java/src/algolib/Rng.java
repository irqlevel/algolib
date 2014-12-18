package algolib;

import java.nio.ByteBuffer;
import java.security.SecureRandom;

public class Rng {
	private SecureRandom rng = new SecureRandom();
	
	public byte nextByte() {
		byte[] b = new byte[1];
		rng.nextBytes(b);
		return b[0];
	}
		
	public int nextInt() {
		byte[] b = new byte[4];
		rng.nextBytes(b);
		ByteBuffer bf = ByteBuffer.wrap(b);
		return bf.getInt();
	}
	
	public long nextLong() {
		byte[] b = new byte[8];
		rng.nextBytes(b);
		ByteBuffer bf = ByteBuffer.wrap(b);
		return bf.getLong();
	}

	public double nextDouble() {
		long v = nextLong();
		double a = (double)v - (double)Long.MIN_VALUE;
		double b = (double)Long.MAX_VALUE - (double)Long.MIN_VALUE;
		
		return a/b;
	}
	
	public static void main(String args[]) {
		Rng rng = new Rng();
		for (int i = 0; i < 100; i++)
			System.out.println(rng.nextDouble());
	}
}
