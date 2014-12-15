package algolib;

public interface BTreeNodeEnum<K, V> {
	public void enumClb(K key, V value, int height);
}
