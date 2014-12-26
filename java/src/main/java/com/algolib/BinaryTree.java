package com.algolib;

import java.util.HashMap;
import java.util.Map;

public class BinaryTree<K extends Comparable<K>, V> {
	protected  BinaryTreeNode<K, V> root = null;
	
	private boolean enumInOrder(BinaryTreeNode<K, V> root, BinaryTreeNodeEnum<K, V> enumerator, int height, Object context) {
		BinaryTreeNode<K, V> curr = root;
		if (curr.left != null)
			if (!enumInOrder(curr.left, enumerator, height+1, context))
				return false;
		
		if (!enumerator.enumClb(curr, height, context))
			return false;
		
		if (curr.right != null)
			if (!enumInOrder(curr.right, enumerator, height+1, context))
				return false;
		return true;
	}
	
	public boolean enumInOrder(BinaryTreeNodeEnum<K, V> enumerator, Object context) {
		return enumInOrder(root, enumerator, 0, context);
	}
	
	private int minDepth(BinaryTreeNode<K,V> root) {
		if (root == null)
			return 0;
		return 1 + Math.min(minDepth(root.left), minDepth(root.right));
	}

	private int maxDepth(BinaryTreeNode<K,V> root) {
		if (root == null)
			return 0;
		return 1 + Math.max(maxDepth(root.left), maxDepth(root.right));
	}
	
	public boolean isBalanced() {
		return (maxDepth(root) - minDepth(root) <= 1) ? true : false;
	}
	
	public int maxDepth() {
		return maxDepth(root);
	}
	
	public int minDepth() {
		return minDepth(root);
	}
	
	private boolean isLess(BinaryTreeNode<K, V> root, K key, boolean less) {
		System.out.println("isLess");
		if (root == null)
			return true;
		if (less) {
			if (root.key.compareTo(key) >= 0) {
				System.out.println("less=" + less + " key=" + root.key + " key=" + key);
				return false;
			}
		} else {
			if (root.key.compareTo(key) < 0) {
				System.out.println("less=" + less + " key=" + root.key + " key=" + key);
				return false;
			}
		}

		return (isLess(root.left, root.key, true) && isLess(root.right, root.key, false));
	}
		
	public boolean isBST() {
		return (isLess(root.left, root.key, true) && isLess(root.right, root.key, false));
	}
	
	public boolean isBST2() {
		BinaryTreeNode<K,V> prev = new BinaryTreeNode<K, V>();
		
		return this.enumInOrder(new BinaryTreeNodeEnum<K, V>()   {
			@Override
			public boolean enumClb(BinaryTreeNode<K, V> node, int height, Object context) {
				// TODO Auto-generated method stub
				BinaryTreeNode<K,V> prev = (BinaryTreeNode<K,V>)context;
				System.out.println("BST2 node.key=" + node.key + " prev.key=" + ((prev.left != null) ? prev.left.key : null));
				if (prev.left != null && prev.left.key.compareTo(node.key) > 0)
					return false;
				prev.left = node;
				return true;
			}
		}, prev);
	}
	
	private boolean isKeyInRange(BinaryTreeNode<K, V> root, K min, K max) {
		System.out.println("isKeyInRange");
		if (root == null)
			return true;
		if (root.key.compareTo(min) < 0)
			return false;
		if (root.key.compareTo(max) >= 0)
			return false;
		return (isKeyInRange(root.left, min, root.key) && isKeyInRange(root.right, root.key, max));
	}
	
	public boolean isBST3(K min, K max) {
		return isKeyInRange(root, min, max);
	}
	
	private void print(BinaryTreeNode<K,V> node, String sep) {
		System.out.println(sep + " k=" + node.key + " v=" + node.value);
		if (node.left != null)
			print(node.left, sep + " ");
		if (node.right != null)
			print(node.right, sep + " ");
	}
	
	public void print() {
		if (this.root != null)
			print(this.root, "");
	}

	public static <K extends Comparable<K>, V> BinaryTree<K, V> generate(Map<K, V> map) {
		if (map.isEmpty())
			return null;
		
		Map<K, V> local = new HashMap<K, V>();
		for (K key : map.keySet()) {
			V value = map.get(key);
			local.put(key, value);
		}
		
		KeyValue<K, V> kv = Helper.popMap(local);
		BinaryTree<K, V> t = new BinaryTree<K, V>();
		t.root = new BinaryTreeNode<K, V>(kv.key, kv.value);
		t.root.genChilds(map);

		return t;
	}
	
	public static void main(String args[]) throws Exception {
		Rng rng = new Rng();
		
		Map<Integer, String> kvMap = new HashMap<Integer, String>();
		for (int i = 0; i < 10; i++) {
			kvMap.put(rng.nextInt(), "Petya");
		}
		
		BinaryTree<Integer, String> t = BinaryTree.generate(kvMap);
		t.print();
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
		System.out.println("isBST=" + t.isBST() + " isBST2=" + t.isBST2() + " isBST3=" + t.isBST3(Integer.MIN_VALUE, Integer.MAX_VALUE));
	}
}
