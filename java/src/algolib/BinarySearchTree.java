package algolib;


class BSTNode<K extends Comparable<K>, V> {
	public K key = null;
	public V value = null;
	public BSTNode<K, V> left = null, right = null;
}

interface BSTEnumerator<K extends Comparable<K>, V> {
	public void enumCallback(K key, V value, int height);
}

public class BinarySearchTree<K extends Comparable<K>, V> {
	private  BSTNode<K, V> root = null;

	public boolean insert(K key, V value) {
		BSTNode<K, V> curr = root, prev = null;
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
		
		BSTNode<K, V> node = new BSTNode<K, V>();
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
		BSTNode<K, V> curr = root;
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
	
	private void enumInOrder(BSTNode<K, V> root, BSTEnumerator<K, V> enumerator, int height) {
		BSTNode<K, V> curr = root;
		if (curr.left != null)
			enumInOrder(curr.left, enumerator, height+1);
		enumerator.enumCallback(curr.key, curr.value, height);
		if (curr.right != null)
			enumInOrder(curr.right, enumerator, height+1);
	}
	
	public void enumInOrder(BSTEnumerator<K, V> enumerator) {
		enumInOrder(root, enumerator, 0);
	}
	
	private int minDepth(BSTNode<K,V> root) {
		if (root == null)
			return 0;
		return 1 + Math.min(minDepth(root.left), minDepth(root.right));
	}

	private int maxDepth(BSTNode<K,V> root) {
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
	
	public static void main(String args[]) {
		BinarySearchTree<Integer, String> t = new BinarySearchTree<Integer, String>();
		t.insert(166, "Vasya2");
		t.insert(1, "Petya");
		t.insert(2, "Vasya");
		t.insert(3, "Vasya2");
		t.insert(1, "Vasya2");
		t.insert(13, "Vasya2");
		t.insert(166, "Kolya");
		t.insert(16, "Vasya2");
		t.insert(-2, "Vasya2");
		t.insert(-9, "Vasya2");
		t.insert(66, "Vasya2");
		System.out.println(t.search(2));
		System.out.println(t.search(3));
		System.out.println(t.search(32));
		
		t.enumInOrder(new BSTEnumerator<Integer, String>() {

			@Override
			public void enumCallback(Integer key, String value, int height) {
				// TODO Auto-generated method stub
				System.out.println("Key=" + key + " Value=" + value + " Height=" + height);
			}
		});
		
		System.out.println("balanced=" + t.isBalanced() + " min=" + t.minDepth() + " max=" + t.maxDepth());
	}
}
