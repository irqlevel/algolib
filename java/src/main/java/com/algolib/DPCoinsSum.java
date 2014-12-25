package com.algolib;

public class DPCoinsSum {

	public static int[] getMinNumCoinsEqSum(int values[], int sum) {
		int min[] = new int[sum+1];
		int used[] = new int[sum+1];
		
		for (int i = 1; i < min.length; i++) {
			min[i] = Integer.MAX_VALUE;
			used[i] = Integer.MAX_VALUE;
		}
		
		min[0] = 0;
		used[0] = 0;
		
		for (int i = 1; i < sum+1; i++) {
			for (int j = 0; j < values.length; j++) {
				if (values[j] <= i && min[i-values[j]]+1 < min[i]) {
					min[i] = min[i-values[j]]+1;
					used[i] = values[j];
				}
			}
		}
		int lsum = sum;
		int ncoins = min[lsum];
		int coins[] = new int[ncoins];
		int i = coins.length-1;
		while (lsum > 0) {
			coins[i]= used[lsum];
			lsum-= coins[i];
			i--;
		}
		
		return coins;
	}
	
	public static void main(String[] args) {
		int values[] = new int[]{1, 2, 5};
		int coins[] = getMinNumCoinsEqSum(values, 13);
		
		System.out.println("numCoins=" + coins.length);
		System.out.println("coins:");
		ArrayHelper.print(Helper.intArrToIntegerArr(coins));
	}
}
