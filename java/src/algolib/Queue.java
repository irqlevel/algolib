package algolib;

public class Queue<T> {
	public SListEntry<T> head = null;
	public SListEntry<T> tail = null;
	public SListEntry<T> next = null;
	

	public void enque(T value) {
		SListEntry<T> node = new SListEntry<T>(value);
		if (this.tail != null)
			this.tail.next = node;
		this.tail = node;
		if (this.head == null)
			this.head = node;
	}
	
	public T deque() {
		SListEntry<T> first = this.head;
		if (first == null)
			return null;
		
		this.head = first.next;
		if (first == this.tail)
			this.tail = null;
		return first.value;
	}
	
	public static void main(String args[]) {
		Queue<String> q = new Queue<String>();
		q.enque("1");
		q.enque("2");
		q.enque("3");
		q.enque("4");
		q.enque("5");
		q.enque("6");
		String v;
		while ((v = q.deque()) != null) {
			System.out.println(v);
		}
	}
}
