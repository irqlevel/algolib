package com.algolib;

public class RemoveDuplicates {
	public static void removeDuplicatesEff(char []s)
	{
		boolean hit[] = new boolean[65536];
		int pos;
		char c;
		
		hit[s[0]] = true;
		pos = 1;
		for (int i = 1; i < s.length; i++) {
			c = s[i];
			if (hit[c])
				continue;
			else {
				hit[c] = true;
				s[pos++] = c;
			}
		}
		if (pos < s.length)
			s[pos] = 0;
	}
	
	public static void removeDuplicates(char []s)
	{
		int tail;
		boolean found;

		tail = 1;
		for (int i = 1; i < s.length; i++) {
			found = false;
			for (int j = 0; j < tail; j++) {
				if (s[j] == s[i]) {
					found = true;
					break;
				}
			}
			if (!found)
				s[tail++] = s[i];
		}
		
		if (tail < s.length)
			s[tail] = 0;
	}
	
	public static void main(String args[])
	{
		char []raw = "aaaabbbbbccccdddd".toCharArray();
		removeDuplicatesEff(raw);
		System.out.println(new String(raw));
		
		char []raw2 = "aaaabbbbbccccdddd".toCharArray();
		removeDuplicates(raw2);
		System.out.println(new String(raw2));
	}
}
