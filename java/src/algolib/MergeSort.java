package algolib;

public class MergeSort {
	private static <T extends Comparable<T>> void merge(T arr[], T buf[], int start, int pivot, int end)
	{
		int i = start;
		int j = pivot;
		int pos = start;
		
		Array.copy(buf, arr, start, end);
		
		while (i < pivot && j < end) {
			if (buf[i].compareTo(buf[j]) < 0) {
				arr[pos++] = buf[i++];
			} else {
				arr[pos++] = buf[j++];
			}
		}
		
		while (i < pivot)
			arr[pos++] = buf[i++];
		
		while (j < end)
			arr[pos++] = buf[j++];
	}
	
	private static <T extends Comparable<T>> void sort_sub(T arr[], T buf[], int start, int end) {
		if (end - start > 1) {
			int p = (start + end)/2;
			sort_sub(arr, buf, start, p);
			sort_sub(arr, buf, p, end);
			merge(arr, buf, start, p, end);
		}
	}
	
	public static <T extends Comparable<T>> void sort(T arr[], T buf[]) throws Exception {
		if (buf.length < arr.length)
			throw new Exception("invalid buf length");
		sort_sub(arr, buf, 0, arr.length);
	}
	
	public static void main(String args[]) throws Exception {
		Integer[] arr = {105, 7, 5, 4, -33, 3, 9, 1};
		Integer[] buf = new Integer[arr.length];
		System.out.println("isSorted=" + Array.isSorted(arr));
		MergeSort.sort(arr, buf);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + arr[i]);
		}
		System.out.println("isSorted=" + Array.isSorted(arr));
		String[] sarr = {"Petya", "Vasya", "kolya", "Olya", "natasha", "petr", "Kostya", "koly", "Pet"};
		String[] sbuf = new String[sarr.length];

		System.out.println("isSorted=" + Array.isSorted(sarr));
		MergeSort.sort(sarr, sbuf);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + sarr[i]);
		}
		System.out.println("isSorted=" + Array.isSorted(sarr));
	}
}
