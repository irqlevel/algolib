package algolib;

public class BTreeNode<K extends Comparable<K>, V> {
	public K key = null;
	public V value = null;
	public BTreeNode<K, V> left = null, right = null;
}
