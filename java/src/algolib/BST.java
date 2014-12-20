package algolib;

import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class BST<K extends Comparable<K>, V> extends BinaryTree<K, V> {
	public boolean insert(K key, V value) {
		BinaryTreeNode<K, V> curr = root, prev = null;
		boolean left = false;
		
		while (curr != null) {
			prev = curr;
			if (key.compareTo(curr.key) == 0)
				return false;
			else if (key.compareTo(curr.key) < 0) {
				curr = curr.left;
				left = true;
			} else {
				curr = curr.right;
				left = false;
			}
		}
		
		BinaryTreeNode<K, V> node = new BinaryTreeNode<K, V>();
		node.key = key;
		node.value = value;
		if (prev == null)
			root = node;
		else {
			if (left)
				prev.left = node;
			else
				prev.right = node;
		}
		
		return true;
	}
	
	public V search(K key) {
		BinaryTreeNode<K, V> curr = root;
		while (curr != null) {
			if (key.compareTo(curr.key) == 0)
				return curr.value;
			else if (key.compareTo(curr.key) < 0)
				curr = curr.left;
			else
				curr = curr.right;
		}
		
		return null;
	}
	
	public boolean remove(K key) {
		return false;
	}

	private static <K extends Comparable<K>, V> BinaryTreeNode<K, V> __balance( 
			K[] keys,
			int a,
			int b,
			Map<K, V> map) {
		if (a >= b)
			return null;
		
		int mid = (a+b)/2;
		BinaryTreeNode<K, V> root = 
				new BinaryTreeNode<K, V>(keys[mid], map.get(keys[mid]));
		
		if (a < mid)
			root.left = __balance(keys, a, mid, map);
		
		if ((mid+1) < b)
			root.right = __balance(keys, mid+1, b, map);
		
		return root;
	}
	
	public BST<K, V> balance() {
		List<KeyValue<K,V>> kvs = new ArrayList<KeyValue<K,V>>();		
		this.enumInOrder(new BinaryTreeNodeEnum<K,V>() {

			@Override
			public boolean enumClb(BinaryTreeNode<K, V> node, int height,
					Object context) {
				// TODO Auto-generated method stub
				List<KeyValue<K,V>> kvs = (List<KeyValue<K,V>>)context;
				kvs.add(new KeyValue<K,V>(node.key, node.value));
				return true;
			}}, kvs);
		
		Map<K, V> map = new HashMap<K, V>();
		List<K> keys = new ArrayList<K>();
		for (KeyValue<K,V> kv : kvs)
			keys.add(kv.key);
		
		K[] keysArr = ArrayHelper.toArray(keys);
		for (KeyValue<K,V> kv : kvs) 
			map.put(kv.key, kv.value);
		
		BST<K, V> t = new BST<K, V>();
		t.root = __balance(keysArr, 0, keysArr.length, map);
		
		return t;
	}
	
	public static void main(String args[]) {
		Rng rng = new Rng();
		
		BST<Integer, String> t = new BST<Integer, String>();
		for (int i = 0; i < 100; i++) {
			t.insert(i, "Petya");
		}
		
		t.print();
		t = t.balance();
		t.print();
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
		//System.out.println("isBST=" + t.isBST() + " isBST2=" + t.isBST2() + " isBST3=" + t.isBST3(Integer.MIN_VALUE, Integer.MAX_VALUE));
	}
}
