package algolib;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

class GraphEnumStat {
	public int numNodes = 0;
}

public class Graph<ID> {
	private Map<ID, GraphNode<ID>> nodes = new HashMap<ID, GraphNode<ID>>();
	
	public Graph(ID ids[]) {
		for (ID id : ids)
			nodes.put(id, new GraphNode<ID>(id));
	}
	
	public GraphNode<ID> getNode(ID id) {
		return nodes.get(id);
	}
	
	public Set<ID> getIds() {
		return nodes.keySet();
	}
	
	public void bfsPrepare() {
		GraphNode<ID> node;
		for (ID id : nodes.keySet()) {
			node = this.getNode(id);
			node.color = GraphNode.UNVISITED;
		}
	}
	
	public boolean bfs(GraphNodeEnum<ID> enumerator, Object context) {
		if (nodes.size() == 0)
			return false;

		GraphNode<ID> node, start = null;
		for (ID id : nodes.keySet()) {
			node = nodes.get(id);
			if (node.color == GraphNode.UNVISITED) {
				start = node;
				break;
			}
		}
		
		if (start == null)
			return false;
		
		start.color = GraphNode.VISITING;
		Queue<GraphNode<ID>> visiting = new Queue<GraphNode<ID>>();
		visiting.enque(start);
		while ((node = visiting.deque()) != null) {
			for (ID id : node.sibIds.keySet()) {
				GraphNode<ID> sib = this.getNode(id);
				if (sib.color == GraphNode.UNVISITED) {
					sib.color = GraphNode.VISITING;
					visiting.enque(sib);
				}
			}
			node.color = GraphNode.VISITED;
			if (!enumerator.enumClb(node, context))
				return false;
		}
	
		return true;
	}
	
	public static Graph<Integer> generate(int adjMatrix[][]) {
		for (int i = 0 ; i < adjMatrix.length; i++)
			if (adjMatrix[i].length != adjMatrix.length)
				return null;
		
		Integer [] ids = new Integer[adjMatrix.length];
		for (int i = 0; i < ids.length; i++)
			ids[i] = i;
		
		Graph<Integer> g = new Graph<Integer>(ids);
		
		for (int i = 0; i < adjMatrix.length; i++) {
			GraphNode<Integer> node = g.getNode(i);
			for (int j = 0; j < adjMatrix[i].length; j++) {
				if (adjMatrix[i][j] != 0) {
					GraphNode<Integer> sib = g.getNode(j);
					node.connect(sib.id);
					sib.connect(node.id);
				}
			}
		}
		
		return g;
	}

	public static void main(String args[]) {
		Rng rng = new Rng();
		
		int [][]m = new int[100][100];
		for (int i = 0; i < m.length; i++) {		
			for (int j = 0; j < m[i].length; j++) {
				if (rng.nextDouble() < 0.01)
					m[i][j] = 1;
				else
					m[i][j] = 0;
			}
		}
		
		for (int i = 0; i < m.length; i++)	
			for (int j = 0; j < m[i].length; j++)
				if (m[i][j] > 0)
					System.out.println("[" + i + "][" + j + "]=" + m[i][j]);

		Graph<Integer> g = Graph.generate(m);
		GraphEnumStat st = new GraphEnumStat();
		
		g.bfsPrepare();
		int clusters = 0;
		while (true) {
			st.numNodes = 0;
			if (!g.bfs(new GraphNodeEnum<Integer>() {
				
				@Override
				public boolean enumClb(GraphNode<Integer> node, Object context) {
					// TODO Auto-generated method stub
					System.out.println("node " + node.id);
					st.numNodes++;
					return true;
				}
				},
			st
			))
				break;
			clusters++;
			System.out.println("numNodes=" + st.numNodes);
		}
		System.out.println("clusters=" + clusters);
	}
}
