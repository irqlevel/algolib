package com.algolib;

import java.util.ArrayList;

public class StringPerms {

	public static ArrayList<String> getAllPerms(String s)
	{
		ArrayList<String> perms = new ArrayList<String>();
		if (s.length() == 0) {
			return perms;
		} else if (s.length() == 1) {
			perms.add(s);
			return perms;
		}
		
		char first = s.charAt(0);
		String remainder = s.substring(1);
			
		ArrayList<String> remPerms = getAllPerms(remainder);
		for (int i = 0; i < remainder.length() + 1; i++) {
			for (String perm : remPerms) {
				String p1 = perm.substring(0, i);
				String p2 = perm.substring(i);
				perms.add(p1 + first + p2);
			}
		}

		return perms;	
	}
	
	public static void main(String args[]) {
		ArrayList<String> perms = getAllPerms("as");
		for (String s : perms) {
			System.out.println(s);
		}
	}
}
