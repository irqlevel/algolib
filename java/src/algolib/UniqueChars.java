package algolib;

public class UniqueChars {
	
	public static boolean isUniqueChars(String s)
	{
		boolean charSet[] = new boolean[65536];
		for (int i = 0; i < s.length(); i++) {
			if (charSet[s.charAt(i)])
				return false;
			else
				charSet[s.charAt(i)] = true;
		}
		
		return true;
	}
	
	public static void main(String args[])
	{
		String s = new String("abcdefghq");
				
		System.out.println(isUniqueChars(s));
	}
}
