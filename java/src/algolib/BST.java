package algolib;

import java.security.SecureRandom;


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
		
	public static void main(String args[]) {
		SecureRandom rnd = new SecureRandom();
		
		BST<Integer, String> t = new BST<Integer, String>();
		for (int i = 0; i < 10; i++) {
			t.insert(rnd.nextInt(), "Petya");
		}
		
		t.print();
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
		System.out.println("isBST=" + t.isBST() + " isBST2=" + t.isBST2() + " isBST3=" + t.isBST3(Integer.MIN_VALUE, Integer.MAX_VALUE));
	}
}
