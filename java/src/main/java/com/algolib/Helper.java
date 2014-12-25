package com.algolib;

import java.util.Map;
import java.util.Set;

public class Helper {

	public static <K,V> KeyValue<K, V> popMap(Map<K, V> map) {
		for (K key : map.keySet()) {
			V value = map.remove(key);
			return new KeyValue<K, V>(key, value);
		}
		return null;	
	}
	
	public static <K> K popSet(Set<K> set) {
		for (K key : set) {
			if (set.remove(key))
				return key;
		}
		return null;	
	}
	
	public static <K> void pushSet(Set<K> set, K key) {
		set.add(key);
	}
	
	public static <K,V> void pushMap(Map<K, V> map, KeyValue<K, V> kv) {
		map.put(kv.key, kv.value);
	}
	
	public static Integer[] intArrToIntegerArr(int [] arr) {
		Integer[] result = new Integer[arr.length];
		for (int i = 0; i < arr.length; i++)
			result[i] = arr[i];
		return result;
	}
}
