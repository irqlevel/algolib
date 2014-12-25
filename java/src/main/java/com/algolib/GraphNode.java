package com.algolib;

import java.util.Map;
import java.util.TreeMap;

public class GraphNode<ID> {
	public static final int UNVISITED = 0;
	public static final int VISITING = 1;
	public static final int VISITED = 2;
	public int color = UNVISITED;
	public ID id = null;

	public Map<ID, Object> sibIds = new TreeMap<ID, Object>(); 

	public GraphNode(ID id) {
		this.id = id;
		this.color = UNVISITED;
	}
	
	public void connect(ID id) {
		sibIds.put(id, null);
	}
}
