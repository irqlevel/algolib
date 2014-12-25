package com.algolib;

import java.util.ArrayList;
import java.util.List;

public class PairSumSearch {
	public static List<Pair<Integer, Integer>> search(Integer[] arr, Integer target) {
		List<Pair<Integer, Integer>> result = new ArrayList<Pair<Integer, Integer>>();
		QuickSort.sort(arr);
		
		int i = 0, j = arr.length - 1;
		while (i < j) {
			Integer sum = arr[i] + arr[j];
			if (sum < target)
				i++;
			else if (sum > target)
				j--;
			else {
				result.add(new Pair<Integer, Integer>(arr[i], arr[j]));
				i++;
				j--;
			}
		}
		
		return result;
	}
	
	public static void main(String args[]) {
		List<Pair<Integer, Integer>> results = search(new Integer[]{1, 3, 4, 2, 7, 1, 3, 4, 2, 4, 2, 3, 5, 3, 4, 6, 10}, 12);
		for (Pair<Integer, Integer> res : results) {
			System.out.println(res.first + " " + res.second);
		}
	}
}
