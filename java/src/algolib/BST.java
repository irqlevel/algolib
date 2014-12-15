package algolib;

import java.security.SecureRandom;


public class BST<K extends Comparable<K>, V> {
	private  BTreeNode<K, V> root = null;

	public boolean insert(K key, V value) {
		BTreeNode<K, V> curr = root, prev = null;
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
		
		BTreeNode<K, V> node = new BTreeNode<K, V>();
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
		BTreeNode<K, V> curr = root;
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
	
	private boolean enumInOrder(BTreeNode<K, V> root, BTreeNodeEnum<K, V> enumerator, int height, Object context) {
		BTreeNode<K, V> curr = root;
		if (curr.left != null)
			if (!enumInOrder(curr.left, enumerator, height+1, context))
				return false;
		
		if (!enumerator.enumClb(curr.key, curr.value, height, context))
			return false;
		
		if (curr.right != null)
			if (!enumInOrder(curr.right, enumerator, height+1, context))
				return false;
		return true;
	}
	
	public boolean enumInOrder(BTreeNodeEnum<K, V> enumerator, Object context) {
		return enumInOrder(root, enumerator, 0, context);
	}
	
	private int minDepth(BTreeNode<K,V> root) {
		if (root == null)
			return 0;
		return 1 + Math.min(minDepth(root.left), minDepth(root.right));
	}

	private int maxDepth(BTreeNode<K,V> root) {
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
	
	private boolean isLess(BTreeNode<K, V> root, K key, boolean less) {
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
		BTreeNode<K,V> prev = null;
		
		return this.enumInOrder(new BTreeNodeEnum<K, V>()   {
			@Override
			public boolean enumClb(K key, V value, int height, Object context) {
				// TODO Auto-generated method stub
				BTreeNode<K,V> prev = (BTreeNode<K,V>)context;
				if (prev != null && prev.key.compareTo(key) > 0)
					return false;
				return true;
			}
		}, prev);
	}
	
	private boolean isKeyInRange(BTreeNode<K, V> root, K min, K max) {
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
	
	public static void main(String args[]) {
		SecureRandom rnd = new SecureRandom();
		
		BST<Integer, String> t = new BST<Integer, String>();
		for (int i = 0; i < 10; i++) {
			t.insert(rnd.nextInt(), "Petya");
		}
		
		t.enumInOrder(new BTreeNodeEnum<Integer, String>() {

			@Override
			public boolean enumClb(Integer key, String value, int height, Object context) {
				// TODO Auto-generated method stub
				System.out.println("Key=" + key + " Value=" + value + " Height=" + height);
				return true;
			}
		}, null);
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
		System.out.println("isBST=" + t.isBST() + " isBST2=" + t.isBST2() + " isBST3=" + t.isBST3(Integer.MIN_VALUE, Integer.MAX_VALUE));
	}
}
