package algolib;

public class Atoi {

	public static int atoi(char[] s, int base) throws Exception {
		int result = 0;
		
		for (int i = 0; i < s.length; i++) {
			if (s[i] == '\0')
				break;
			if (s[i] < '0')
				throw new Exception("invalid string");
			int v = s[i] - '0';
			if (v < 0 || v >= base)
				throw new Exception("invalid string");
			result = base*result + v;
		}
		
		return result;
	}
	
	public static void main(String args[]) throws Exception {
<<<<<<< HEAD
		System.out.println(atoi(new String("10000").toCharArray(), 2));
=======
		System.out.println(atoi(new String("1000").toCharArray(), 2));
>>>>>>> 621439c32d1ddef1bc0c701ba2f9015ac9c56ac3
	}
}
