package com.algolib;

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
	
	public Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>> searchNode(K key) {
		BinaryTreeNode<K, V> curr = root, prev = null;
		if (curr == null)
			return null;
		
		while (curr != null) {
			if (key.compareTo(curr.key) == 0)
				break;
			
			prev = curr;
			if (key.compareTo(curr.key) < 0)
				curr = curr.left;
			else
				curr = curr.right;
		}
		
		if (curr == null)
			return null;
		
		return new Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>>(curr, prev);
	}
	
	public Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>> 
		searchPredecessor(BinaryTreeNode<K,V> node) {
		BinaryTreeNode<K,V> curr = node.left, prev = node;
		if (curr == null)
			return null;
		
		while (curr.right != null) {
			prev = curr;
			curr = curr.right;
		}
		return new Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>>(curr, prev);
	}

	public Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>>
		searchSuccessor(BinaryTreeNode<K,V> node) {
		BinaryTreeNode<K,V> curr = node.right, prev = node;
		if (curr == null)
			return null;
		
		while (curr.left != null) {
			prev = curr;
			curr = curr.left;
		}
		
		return new Pair<BinaryTreeNode<K,V>, BinaryTreeNode<K,V>>(curr, prev);
	}
	
	public V search(K key) {
		Pair<BinaryTreeNode<K, V>, BinaryTreeNode<K, V>> found = searchNode(key);
		if (found == null)
			return null;
		
		return found.first.value;
	}
	
	private boolean removeOneChildNode(BinaryTreeNode<K, V> node, BinaryTreeNode<K, V> prev) {
		if (prev != null && (prev.left != node && prev.right != node))
			return false;
		
		if (node.left == null && node.right == null) {
			if (prev == null) {
				root = null;
			} else {
				if (prev.left == node)
					prev.left = null;
				else
					prev.right = null;
			}
			return true;
		}
		
		BinaryTreeNode<K, V> child = null;
		if ((node.left == null && ((child = node.right) != null)) ||
			(node.right == null && ((child = node.left) != null))) {
			if (prev == null) {
				root = child;
			} else if (prev.left == node)
				prev.left = child;
			else
				prev.right = child;
			return true;
		}
		
		return false;
	}
	
	public boolean remove(K key) throws Exception {
		Pair<BinaryTreeNode<K, V>, BinaryTreeNode<K, V>> found = searchNode(key);
		if (found == null)
			return false;
		
		if (removeOneChildNode(found.first, found.second))
			return true;
		
		Rng rng = new Rng();
		Pair<BinaryTreeNode<K, V>, BinaryTreeNode<K, V>> succ = 
				(rng.nextBoolean()) ? searchPredecessor(found.first) : searchSuccessor(found.first);
		if (succ == null)
			throw new Exception("Incorrect algorithm");
		
		found.first.key = succ.first.key;
		found.first.value = succ.first.value;
		
		if (!removeOneChildNode(succ.first, succ.second))
			throw new Exception("Incorrect algorithm");
		
		return true;
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
	public static void main(String args[]) throws Exception {
		BST<Integer, String> t = new BST<Integer, String>();
		for (int i = 0; i < 100; i++) {
			t.insert(i, "Petya");
		}
		
		t.print();
		t = t.balance();
		t.print();
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
		//System.out.println("isBST=" + t.isBST() + " isBST2=" + t.isBST2() + " isBST3=" + t.isBST3(Integer.MIN_VALUE, Integer.MAX_VALUE));
		
		for (int i = 1; i < 100; i+=2) {
			t.remove(i);
		}
		
		t.print();
	}
}
