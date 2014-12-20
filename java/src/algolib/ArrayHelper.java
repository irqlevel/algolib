package algolib;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class ArrayHelper {
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
	
	public static int minIntArr(int arr[]) {
		int min = arr[0];
		for (int i = 1; i < arr.length; i++) {
			if (arr[i] < min)
				min = arr[i];
		}
		return min;
	}
		
	public static <T> T[] toArray(final List<T> obj) {
	    if (obj == null || obj.isEmpty()) {
	        return null;
	    }
	    final T t = obj.get(0);
	    final T[] res = (T[]) Array.newInstance(t.getClass(), obj.size());
	    for (int i = 0; i < obj.size(); i++) {
	        res[i] = obj.get(i);
	    }
	    return res;
	}
}
