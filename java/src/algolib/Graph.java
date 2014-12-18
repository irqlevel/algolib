package algolib;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

class GraphEnumStat {
	public int numNodes = 0;
}

public class Graph<ID> {
	private Map<ID, GraphNode<ID>> nodes = new HashMap<ID, GraphNode<ID>>();
	
	public Graph() {
		
	}
	
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
	
	public void searchPrepare() {
		GraphNode<ID> node;
		for (ID id : nodes.keySet()) {
			node = this.getNode(id);
			node.color = GraphNode.UNVISITED;
		}
	}
	
	public boolean bfs(GraphNodeEnum<ID> enumerator, Object context) {
		if (nodes.size() == 0)
			return false;

		GraphNode<ID> node, start;
		start = this.selectUnvisited();
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
	
	private GraphNode<ID> selectUnvisited() {
		GraphNode<ID> node, start = null;
		for (ID id : nodes.keySet()) {
			node = nodes.get(id);
			if (node.color == GraphNode.UNVISITED) {
				start = node;
				break;
			}
		}
		return start;
	}
	
	private boolean dfsRecInternal(GraphNode<ID> start, GraphNodeEnum<ID> enumerator, Object context)
	{
		start.color = GraphNode.VISITING;
		for (ID id : start.sibIds.keySet()) {
			GraphNode<ID> node = this.getNode(id);
			if (node.color == GraphNode.UNVISITED)
				if (!dfsRecInternal(node, enumerator, context))
					return false;
		}
		
		start.color = GraphNode.VISITED;
		if (!enumerator.enumClb(start, context))
			return false;
		
		return true;
	}
	
	public boolean dfsRec(GraphNodeEnum<ID> enumerator, Object context) {
		if (nodes.size() == 0)
			return false;

		GraphNode<ID> start = this.selectUnvisited();
		if (start == null)
			return false;
		
		return dfsRecInternal(start, enumerator, context);
	}
	
	public boolean dfsIter(GraphNodeEnum<ID> enumerator, Object context) {
		if (nodes.size() == 0)
			return false;

		GraphNode<ID> curr = this.selectUnvisited();
		if (curr == null)
			return false;
		
		Stack<GraphNode<ID>> stack = new Stack<GraphNode<ID>>();
		curr.color = GraphNode.VISITING;
		stack.push(curr);
		
		while ((curr = stack.peek()) != null) {
			int found = 0;
			Queue<GraphNode<ID>> q = new Queue<GraphNode<ID>>();
			GraphNode<ID> sib;
			for (ID id : curr.sibIds.keySet()) {
				sib = this.getNode(id);
				if (sib.color == GraphNode.UNVISITED) {
					sib.color = GraphNode.VISITING;
					q.push(sib);
					found++;
				}
			}
			
			if (found > 0) {
				while ((sib = q.deque()) != null)
					stack.push(sib);
			} else {
				stack.pop();
				curr.color = GraphNode.VISITED;
				if (!enumerator.enumClb(curr, context))
					return false;
			}
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
	
	public static Graph<Integer> generate(int numNodes, double p) {
		if (numNodes <= 0)
			return null;
		
		if (p < 0 || p >= 1)
			return null;
		
		Rng rng = new Rng();
		int [][]m = new int[numNodes][numNodes];
		for (int i = 0; i < m.length; i++) {		
			for (int j = 0; j < i; j++) {
				if (rng.nextDouble() < p)
					m[i][j] = 1;
				else
					m[i][j] = 0;
			}
		}
		
		for (int i = 0; i < m.length; i++)	
			for (int j = 0; j < m[i].length; j++)
				if (m[i][j] > 0)
					System.out.println("[" + i + "][" + j + "]=" + m[i][j]);
		
		return Graph.generate(m);
	}
	
	public void addNode(GraphNode<ID> n) {
		nodes.put(n.id, n);
	}
	
	public void addLink(ID fromId, ID toId) {
		GraphNode<ID> from = getNode(fromId);
		if (from == null) {
			from = new GraphNode<ID>(fromId);
			addNode(from);
		}
		
		GraphNode<ID> to = getNode(toId);
		if (to == null) {
			to = new GraphNode<ID>(toId);
			addNode(to);
		}
		
		from.connect(to.id);
	}
	
	public static int numClustersByBfs(Graph<Integer> g) {
		GraphEnumStat st = new GraphEnumStat();
		
		g.searchPrepare();
		int clusters = 0;
		while (true) {
			st.numNodes = 0;
			if (!g.bfs(new GraphNodeEnum<Integer>() {
				
				@Override
				public boolean enumClb(GraphNode<Integer> node, Object context) {
					// TODO Auto-generated method stub
					GraphEnumStat st = (GraphEnumStat)context;
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
		return clusters;
	}
	
	public static int numClustersByDfsIter(Graph<Integer> g) {
		GraphEnumStat st = new GraphEnumStat();
		
		g.searchPrepare();
		int clusters = 0;
		while (true) {
			st.numNodes = 0;
			if (!g.dfsIter(new GraphNodeEnum<Integer>() {
				
				@Override
				public boolean enumClb(GraphNode<Integer> node, Object context) {
					// TODO Auto-generated method stub
					GraphEnumStat st = (GraphEnumStat)context;
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
		return clusters;
	}
	
	public static int numClustersByDfsRec(Graph<Integer> g) {
		GraphEnumStat st = new GraphEnumStat();
		
		g.searchPrepare();
		int clusters = 0;
		while (true) {
			st.numNodes = 0;
			if (!g.dfsRec(new GraphNodeEnum<Integer>() {
				
				@Override
				public boolean enumClb(GraphNode<Integer> node, Object context) {
					// TODO Auto-generated method stub
					GraphEnumStat st = (GraphEnumStat)context;
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
		return clusters;
	}
	
	public static void topSort() {
		Graph<Integer> g = new Graph<Integer>();
		
		g.addLink(0, 1);
		g.addLink(0, 2);
		g.addLink(1, 4);
		g.addLink(1, 6);
		g.addLink(2, 3);
		g.addLink(2, 5);
		g.addLink(3, 4);
		g.addLink(6, 5);
		
		g.searchPrepare();
		GraphEnumStat st = new GraphEnumStat();
		
		g.dfsIter(new GraphNodeEnum<Integer>() {
			
			@Override
			public boolean enumClb(GraphNode<Integer> node, Object context) {
				// TODO Auto-generated method stub
				GraphEnumStat st = (GraphEnumStat)context;
				System.out.println("node " + node.id);
				st.numNodes++;
				return true;
			}
			}, st);
	}
	
	public static void numClusters() {
		Graph<Integer> g = Graph.generate(100, 0.01);
		System.out.println("clusters=" + numClustersByBfs(g));
		System.out.println("clusters=" + numClustersByDfsRec(g));
		System.out.println("clusters=" + numClustersByDfsIter(g));
	}

	public static void main(String args[]) {
		numClusters();
		topSort();
	}
}
