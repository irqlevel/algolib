package algolib;

public class Stack {
	public Object data = null;
	public Stack next = null;
	
	public Stack() {
	}
	
	public Stack(Object data) {
		this.data = data;
	}
	public void push(Stack node) {
		node.next = this.next;
		this.next = node;
	}
	
	public Stack pop() {
		Stack first = this.next;
		if (first != null) {
			this.next = first.next;
		}
		return first;
	}
	
	public static void main(String args[]) {
		Stack head = new Stack();
		head.push(new Stack("A"));
		head.push(new Stack("B"));
		head.push(new Stack("C"));
		Stack node;
		while ((node = head.pop()) != null) {
			System.out.println((String)node.data);
		}
	}
}
