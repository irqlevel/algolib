package algolib;

import java.util.ArrayList;
import java.util.List;

class Pair<T> {
	public T first;
	public T second;
	
	Pair(T first, T second) {
		this.first = first;
		this.second = second;
	}
}

public class PairSumSearch {
	public static List<Pair<Integer>> search(Integer[] arr, Integer target) {
		List<Pair<Integer>> result = new ArrayList<Pair<Integer>>();
		QuickSort.sort(arr);
		
		int i = 0, j = arr.length - 1;
		while (i < j) {
			Integer sum = arr[i] + arr[j];
			if (sum < target)
				i++;
			else if (sum > target)
				j--;
			else {
				result.add(new Pair<Integer>(arr[i], arr[j]));
				i++;
				j--;
			}
		}
		
		return result;
	}
	
	public static void main(String args[]) {
		List<Pair<Integer>> results = search(new Integer[]{1, 3, 4, 2, 7, 1, 3, 4, 2, 4, 2, 3, 5, 3, 4, 6, 10}, 7);
		for (Pair<Integer> res : results) {
			System.out.println(res.first + " " + res.second);
		}
	}
}
