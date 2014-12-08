package algolib;

public class QuickSort {
	public static <T extends Comparable<T>> void sort(T arr[]) {
		qsort_sub(arr, 0, arr.length);
	}

	private static <T extends Comparable<T>> int qsort_partition(T[] arr, int start, int end) {
		int pivotI = (start + end)/2;
		T pivot = arr[pivotI];
		Array.swap(arr, end - 1, pivotI);
		int storeIndex = start;
		for (int i = start; i < end -1 ; i++) {
			if (arr[i].compareTo(pivot) < 0) {
				Array.swap(arr, storeIndex, i);
				storeIndex++;
			}
		}
		Array.swap(arr, storeIndex, end -1);
		return storeIndex;
	}
	
	private static <T extends Comparable<T>> void qsort_sub(T[] arr, int start, int end) {
		if (end - start > 1) {
			int p = qsort_partition(arr, start, end);
			qsort_sub(arr, start, p);
			qsort_sub(arr, p+1, end);
		}
	}
	
	public static void main(String args[]) {
		Integer[] arr = {105, 7, 5, 4, -33, 3, 9, 1};
		System.out.println("isSorted=" + Array.isSorted(arr));
		QuickSort.sort(arr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + arr[i]);
		}
		System.out.println("isSorted=" + Array.isSorted(arr));
		String[] sarr = {"Petya", "Vasya", "kolya", "Olya", "natasha", "petr", "Kostya", "koly", "Pet"};
		System.out.println("isSorted=" + Array.isSorted(sarr));
		QuickSort.sort(sarr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + sarr[i]);
		}
		System.out.println("isSorted=" + Array.isSorted(sarr));
	}
}
