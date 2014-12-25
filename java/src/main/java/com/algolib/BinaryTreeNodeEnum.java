package com.algolib;

public interface BinaryTreeNodeEnum<K extends Comparable<K>, V> {
	public boolean enumClb(BinaryTreeNode<K,V> node, int height, Object context);
}
