package algolib;

public class SListEntry<T> {
	public SListEntry<T> next = null;
	public T value = null;
	
	SListEntry(T value) {
		this.value = value;
	}
}
