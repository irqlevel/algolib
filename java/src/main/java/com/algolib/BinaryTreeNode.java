package com.algolib;

import java.util.Map;

public class BinaryTreeNode<K extends Comparable<K>, V> {
	public K key = null;
	public V value = null;
	public BinaryTreeNode<K, V> left = null, right = null;
	
	BinaryTreeNode() {
		
	}
	
	BinaryTreeNode(K key, V value) {
		this.key = key;
		this.value = value;
	}
	
	public void genChilds(Map<K, V> map) {
		Rng rng = new Rng();
		KeyValue<K, V> kv = Helper.popMap(map);
		if (kv == null)
			return;
		
		if (rng.nextBoolean()) {
			if (rng.nextBoolean()) {
				if (left == null) {
					left = new BinaryTreeNode<K, V>(kv.key, kv.value);
					kv = null;
				}
				if (right == null) {
					if (kv == null)
						kv = Helper.popMap(map);
					if (kv != null) {
						right = new BinaryTreeNode<K, V>(kv.key, kv.value);
						kv = null;
					}
				}
			} else {
				if (right == null) {
					right = new BinaryTreeNode<K, V>(kv.key, kv.value);
					kv = null;
				}
				
				if (left == null) {
					if (kv == null)
						kv = Helper.popMap(map);
					if (kv != null) {
						left = new BinaryTreeNode<K, V>(kv.key, kv.value);
						kv = null;
					}
				}
			}
		} else {
			if (rng.nextBoolean()) {
				if (left == null) {
					left = new BinaryTreeNode<K, V>(kv.key, kv.value);
					kv = null;
				}
			} else {
				if (right == null) {
					right = new BinaryTreeNode<K, V>(kv.key, kv.value);
					kv = null;
				}
			}
		}
		
		if (kv != null)
			Helper.pushMap(map, kv);
		
		if (left != null)
			left.genChilds(map);
		if (right != null)
			right.genChilds(map);
	}
}
