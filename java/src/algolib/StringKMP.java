package algolib;

public class StringKMP {

	public static int subString(String s, String sub) {
		if (s.length() == 0 || sub.length() == 0)
			return -1;
		
		if (sub.length() > s.length())
			return -1;
		
		int next[] = new int[sub.length()];
		next[0] = -1;
		
		for (int i = 1, j = 0; i < sub.length(); i++, j++) {
			if (sub.charAt(i) != sub.charAt(j))
				next[i] = j;
			else
				next[i] = next[j];
			while (j >= 0 && sub.charAt(i) != sub.charAt(j)) {
				j = next[j];
			}
		}
		
		for (int i = 0; i < next.length; i++) {
			System.out.println("next[" + i + "]=" + next[i]);
		}
		
		int i, j;
		for (i = 0, j = 0; i < s.length() && j < sub.length(); i++, j++) {
			while (j >= 0 && s.charAt(i) != sub.charAt(j))
				j = next[j];
		}
		return (j == sub.length()) ? i - sub.length() : -1;
	}
	
	public static void main(String args[]) {
		System.out.println(subString(new String("habahaba"), new String("aba")));
		System.out.println(subString(new String("habahabaraahabra"), new String("ara")));
		System.out.println(subString(new String("habahabaraahabra"), new String("rah")));
		System.out.println(subString(new String("ABC ABCDAB ABCDABCDABDE"), new String("ABCDABD")));
	}
}
