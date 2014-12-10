package algolib;

public class PrintParens {

	public static void printParens(int l, int r, char []str, int pos) {
		if (l < 0 || r < l)
			return;
		if (l == 0 && r == 0) {
			System.out.println(str);
		} else {
			if (l > 0) {
				str[pos] = '(';
				printParens(l-1, r, str, pos+1);
			}
			if (r > l) {
				str[pos] = ')';
				printParens(l, r-1, str, pos+1);
			}
		}
	}
	public static void printParens(int count) {
		char str[] = new char[2*count];
		printParens(count, count, str, 0);
	}
	
	public static void main(String args[]) {
		printParens(3);
	}
	
}
