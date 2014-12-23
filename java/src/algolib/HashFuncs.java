package algolib;

public class HashFuncs {
	public static int hash32shift(int key)
	{
		key = ~key + (key << 15); // key = (key << 15) - key - 1;
  		key = key ^ (key >>> 12);
  		key = key + (key << 2);
  		key = key ^ (key >>> 4);
  		key = key * 2057; // key = (key + (key << 3)) + (key << 11);	
  		key = key ^ (key >>> 16);
  		return key;
	}
	
	public static int hashJenkins(int a)
	{
	   a = (a+0x7ed55d16) + (a<<12);
	   a = (a^0xc761c23c) ^ (a>>19);
	   a = (a+0x165667b1) + (a<<5);
	   a = (a+0xd3a2646c) ^ (a<<9);
	   a = (a+0xfd7046c5) + (a<<3);
	   a = (a^0xb55a4f09) ^ (a>>16);
	   return a;
	}
	
	public static int hash32shiftmult(int key)
	{
		int c2=0x27d4eb2d; // a prime or an odd constant	
		key = (key ^ 61) ^ (key >>> 16);
		key = key + (key << 3);
		key = key ^ (key >>> 4);
		key = key * c2;
		key = key ^ (key >>> 15);
		return key;
	}
}
