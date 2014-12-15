package algolib;

public interface BTreeNodeEnum<K, V> {
	public boolean enumClb(K key, V value, int height, Object context);
}
