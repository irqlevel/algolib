package reverse;

public class ReverseString {

	public static void reverseInPlace(char str[])
	{
		char tmp;

		for (int i = 0; i < str.length/2 ; i++) {
			tmp = str[i];
			str[i] = str[str.length-1-i];
			str[str.length-1-i] = tmp;			
		}
	}
	
	public static void main(String args[])
	{
		char str[] = new String("Hello world").toCharArray();
		
		reverseInPlace(str);
		
		System.out.println(new String(str));
	}
}
