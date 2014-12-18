package algolib;

import java.util.HashMap;
import java.util.Map;

public class GraphNode<ID> {
	public static final int UNVISITED = 0;
	public static final int VISITING = 1;
	public static final int VISITED = 2;
	public int color = UNVISITED;
	public ID id = null;

	public Map<ID, Object> sibIds = new HashMap<ID, Object>(); 
	public GraphNode(ID value) {
		this.id = value;
		this.color = UNVISITED;
	}
	
	public void connect(ID id) {
		sibIds.put(id, null);
	}
}
