package algolib;

public class InsertionSort {

	public static <T extends Comparable<T>> void sort(T arr[]) {
		for (int i = 0; i < arr.length; i++) {
			for (int j = 0; j < i; j++) {
				if (arr[j].compareTo(arr[i]) > 0)
					ArrayHelper.swap(arr, i, j);
			}
		}
	}
	
	public static void main(String args[]) {
		Integer[] arr = {105, 7, 5, 4, -33, 3, 9, 1};
		System.out.println("isSorted=" + ArrayHelper.isSorted(arr));
		InsertionSort.sort(arr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + arr[i]);
		}
		System.out.println("isSorted=" + ArrayHelper.isSorted(arr));
		String[] sarr = {"Petya", "Vasya", "kolya", "Olya", "natasha", "petr", "Kostya", "koly", "Pet"};
		System.out.println("isSorted=" + ArrayHelper.isSorted(sarr));
		InsertionSort.sort(sarr);
		for (int i = 0; i < arr.length; i++) {
			System.out.println("arr["+i+"]=" + sarr[i]);
		}
		System.out.println("isSorted=" + ArrayHelper.isSorted(sarr));
	}
}
