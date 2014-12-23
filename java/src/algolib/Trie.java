package algolib;

import java.util.ArrayList;
import java.util.List;

public class Trie<V> {
	private TrieNode<V> root = new TrieNode<V>();

	public boolean insert(String key, V value) throws Exception {
		TrieNode<V> node = root, prev = null;
		int i = 0;
		int n = key.length();
		if (n == 0)
			throw new Exception("invalid key");
		
		prev = node;
		while (i < n) {
			if ((node = node.childs.get(key.charAt(i))) != null) {
				prev = node;
				i++;
			} else
				break;
		}
		
		while (i < n) {
			node = new TrieNode<V>();
			prev.childs.put(key.charAt(i), node);
			prev = node;
			i++;
		}
		
		prev.value = value;
		return true;
	}

	public V find(String key) {
		TrieNode<V> curr = root;
		for (Character c : key.toCharArray()) {
			curr = curr.childs.get(c);
			if (curr == null)
				return null;
		}
		return (curr != null) ? curr.value : null;
	}
	
	private void print(TrieNode<V> node, String sep) {
		System.out.println(sep + "node.value=" + node.value);
		System.out.println(sep + "node.childs:");
		for (Character c : node.childs.keySet()) {
			TrieNode<V> child = node.childs.get(c);
			System.out.println(sep + "	" + c + " -> " + child.value);
			print(child, sep + "	");
		}
	}
	
	public void print() {
		print(root, "");
	}
	
	public TrieNode<V> lookupPrefixNode(String prefix)
	{
		TrieNode<V> curr = root;
		int i = 0;
		
		while (i < prefix.length()) {
			curr = curr.childs.get(prefix.charAt(i));
			if (curr == null)
				return null;
			i++;
		}
		
		return curr;
	}
	
	public List<String> getStringsByNode(TrieNode<V> root, String prefix)
	{
		TrieNode<V> child;
		List<String> resList = new ArrayList<String>();
		
		for (Character c : root.childs.keySet()) {
			child = root.childs.get(c);
			if (child != null) {
				List<String> subList = getStringsByNode(child, prefix + c);
				for (String s : subList)
					resList.add(s);
			}
		}
		
		if (root.value != null) {
			resList.add(prefix);
		}
		
		return resList;
	}
	
	public List<String> autoComplete(String prefix) {
		TrieNode<V> root;
		root = lookupPrefixNode(prefix);
		if (root == null)
			return null;
		
		return getStringsByNode(root, prefix);
	}
	
	public static void main(String args[]) throws Exception {
		Trie<Integer> t = new Trie<Integer>();
		String keys[] = new String[] {"ca", "car", "caret", "caka", "cakapet",
				"cro", "crosy", "crussy", "caret", "caka"};
		
		for (int i = 0; i < keys.length; i++) {
			t.insert(keys[i], i);
		}
		for (int i = 0; i < keys.length; i++) {
			System.out.println("key[" + keys[i] + "]=" + t.find(keys[i]));
		}
		
		String prefix = "cr";
		System.out.println("autoComplete for " + prefix);
		for (String s : t.autoComplete(prefix)) {
			System.out.println("	" + s);
		}
	}
}
