package algolib;

public class Queue {
	public Object data = null;
	public Queue head = null;
	public Queue tail = null;
	public Queue next = null;
	
	public Queue() {
	}
	
	public Queue(Object data) {
		this.data = data;
	}
	
	public void enque(Queue node) {
		if (this.tail != null)
			this.tail.next = node;
		this.tail = node;
		if (this.head == null)
			this.head = node;
	}
	
	public Queue deque() {
		Queue first = this.head;
		if (first != null)
			this.head = first.next;
		if (first == this.tail)
			this.tail = null;
		return first;
	}
	
	public static void main(String args[]) {
		Queue q = new Queue();
		q.enque(new Queue("A"));
		q.enque(new Queue("B"));
		q.enque(new Queue("E"));
		q.deque();
		q.enque(new Queue("C"));
		q.enque(new Queue("E"));
		q.deque();
		q.enque(new Queue("C"));
		q.deque();
		q.deque();
		Queue n;
		while ((n = q.deque()) != null) {
			System.out.println((String)n.data);
		}
	}
}
