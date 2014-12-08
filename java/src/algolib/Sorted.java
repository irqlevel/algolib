package algolib;

public class Sorted<T extends Comparable<T>> {
	public boolean isSorted(T arr[]) {
		if (arr.length == 1)
			return true;
		
		T prev = arr[0];
		for (int i = 1; i < arr.length; i++) {
			if (arr[i].compareTo(prev) < 0)
				return false;
			prev = arr[i];
		}
		
		return true;
	}
}
