package algolib;

public class QuickSort<T extends Comparable<T>> {
	public void qsort(T arr[]) {
		qsort_sub(arr, 0, arr.length);
	}

	private void qsort_swap(T[] arr, int i, int j) {
		if (i != j) {	
			T tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}
	
	private int qsort_partition(T[] arr, int start, int end) {
		int pivotI = (start + end)/2;
		T pivot = arr[pivotI];
		qsort_swap(arr, end - 1, pivotI);
		int storeIndex = start;
		for (int i = start; i < end -1 ; i++) {
			if (arr[i].compareTo(pivot) < 0) {
				qsort_swap(arr, storeIndex, i);
				storeIndex++;
			}
		}
		qsort_swap(arr, storeIndex, end -1);
		return storeIndex;
	}
	
	private void qsort_sub(T[] arr, int start, int end) {
		if (start < end) {
			int p = qsort_partition(arr, start, end);
			qsort_sub(arr, start, p);
			qsort_sub(arr, p+1, end);
		}
	}
	
	public static void main(String args[]) {
		Integer[] arr = {105, 7, 5, 4, -33, 3, 9, 1};
		System.out.println("isSorted=" + new Sorted<Integer>().isSorted(arr));
		new QuickSort<Integer>().qsort(arr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + arr[i]);
		}
		System.out.println("isSorted=" + new Sorted<Integer>().isSorted(arr));
		String[] sarr = {"Petya", "Vasya", "kolya", "Olya", "natasha", "petr", "Kostya", "koly", "Pet"};
		System.out.println("isSorted=" + new Sorted<String>().isSorted(sarr));
		new QuickSort<String>().qsort(sarr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + sarr[i]);
		}
		System.out.println("isSorted=" + new Sorted<String>().isSorted(sarr));
	}
}
