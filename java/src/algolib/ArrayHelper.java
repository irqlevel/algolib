package algolib;

import java.lang.reflect.Array;
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
	
	public static int intArrAverage(int arr[]) throws Exception {
		double sum = 0;
		if (arr.length == 0)
			throw new Exception("invalid parameters");
		for (int i = 0; i < arr.length; i++) {
			sum+= arr[i];
		}
		double av = sum/arr.length;
		double deltaMin = Double.MAX_VALUE;
		int avPos = 0;
		for (int i = 0; i < arr.length; i++) {
			double delta = Math.abs(arr[i] - av);
			if (delta < deltaMin) {
				deltaMin = delta;
				avPos = i;
			}
				
		}
		return arr[avPos];
	}
	
	public static void intArrSwap(int arr[], int i, int j) {
		if (arr[i] != arr[j]) {
			arr[i] = arr[i] ^ arr[j];
			arr[j] = arr[i] ^ arr[j];
			arr[i] = arr[i] ^ arr[j];
		}
	}
	
	public static void printIntArr(int arr[]) {
		for (int i = 0; i < arr.length; i++)
			System.out.println("[" + i + "]=" + arr[i]);
	}
		
	private static void countSortValues(int values[], int counts[]) {
		for (int i = 0; i < values.length; i++) {
			for (int j = 0; j < i; j++) {
				if (values[j] > values[i]) {
					intArrSwap(values, i, j);
					intArrSwap(counts, i, j);
				}
			}
		}
	}
	
	public static void countSort(int arr[], int numUniques) throws Exception {
	    if (numUniques < 1 || arr == null || arr.length < 1)
	    	throw new Exception("Invalid parameter");
	    
		int counts[] = new int[numUniques];
	    int values[] = new int[numUniques];
	    int values_filled[] = new int[numUniques];
	    int i, j;
	    
	    for (i = 0; i < arr.length; i++) {
	        for (j = 0; j < values.length; j++)
	            if (values_filled[j] != 0) {
	                if (values[j] == arr[i]) {
	                    counts[j]++;
	                    break;
	                }
	            } else {
	                values[j] = arr[i];
	                values_filled[j] = 1;
	                counts[j] = 1;
	                break;
	            }
	        if (j == values.length)
	        	throw new Exception("More unique values found");
	    }
	    
	    countSortValues(values, counts);    
	    int pos = 0;
	    for (i = 0; i < counts.length; i++)
	        for (j = 0; j < counts[i]; j++)
	            arr[pos++] = values[i];
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
	
	public static void main(String args[]) throws Exception {
		System.out.println("av=" + intArrAverage(new int[]{4, 5, 2, 1, 23, 3, 2, 4, 2, 4, 5, 3, 3, 4}));
		
		int uniques[] = {1, 2, 3, 3, 2, 2, 2, 1, 2, 3, 2, 2, 3, 3, 1};
		countSort(uniques, 3);
		printIntArr(uniques);
		
		int uniques2[] = {1};
		countSort(uniques2, 3);
		printIntArr(uniques2);
	}
}
