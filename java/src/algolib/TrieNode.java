package algolib;

import java.util.HashMap;
import java.util.Map;

public class TrieNode<V> {
	public Map<Character, TrieNode<V>> childs = 
			new HashMap<Character, TrieNode<V>>();
	public V value = null;
}
