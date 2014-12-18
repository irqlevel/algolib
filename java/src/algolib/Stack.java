package algolib;

public class Stack<T> {
	public SListEntry<T> head = null;
	
	public void push(T v) {
		SListEntry<T> node = new SListEntry<T>(v);
		node.next = this.head;
		this.head = node;
	}
	
	public T pop() {
		SListEntry<T> first = head;
		if (first == null)
			return null;
		
		this.head = first.next;
		return first.value;
	}
	
	public T peek() {
		if (head == null)
			return null;
		return head.value;
	}
	
	public static void main(String args[]) {
		Stack<String> s = new Stack<String>();
		s.push("1");
		s.push("2");
		s.push("3");
		s.push("4");
		s.push("5");
		String v;
		while ((v = s.pop()) != null)
			System.out.println(v);
	}
}
