package algolib;

public class Array {
	public static <T extends Comparable<T>> boolean isSorted(T arr[]) {
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

	public static <T> void copy(T[] dst, T[] src, int start, int end) {
		for (int i = start; i < end; i++)
			dst[i] = src[i];
	}
	
	public static <T extends Comparable<T>> void swap(T[] arr, int i, int j) {
		if (i != j) {	
			T tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}
	public static <T> void print(T[] arr) {
		for (int i = 0; i < arr.length; i++)
			System.out.println("[" + i + "]=" + arr[i]);
	}
}
