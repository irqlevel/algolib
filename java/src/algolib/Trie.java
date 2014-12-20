package algolib;

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
	}
}
