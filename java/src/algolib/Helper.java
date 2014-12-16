package algolib;

import java.util.Map;

public class Helper {

	public static <K,V> KeyValue<K, V> popMap(Map<K, V> map) {
		for (K key : map.keySet()) {
			V value = map.remove(key);
			return new KeyValue<K, V>(key, value);
		}
		return null;	
	}
	
	public static <K,V> void pushMap(Map<K, V> map, KeyValue<K, V> kv) {
		map.put(kv.key, kv.value);
	}
}
