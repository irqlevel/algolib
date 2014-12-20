package algolib;

public class CountMinSketch {
	private int arr[][] = null;
	private HashFuncInt hashes[] = null;
	
	public CountMinSketch(HashFuncInt hashes[], int arrBits) throws Exception {
		if (hashes == null || hashes.length < 1 ||
				arrBits < 0 || arrBits > 30)
			throw new Exception("Invalid parameters");
		
		this.hashes = hashes;
		arr = new int[this.hashes.length][];
		for (int i = 0; i < this.hashes.length; i++) {
			arr[i] = new int[1 << arrBits];
			for (int j = 0; j < arr[i].length; j++)
				arr[i][j] = 0;
		}
	}
	
	public void add(int value) {
		for (int i = 0; i < this.hashes.length; i++) {
			int pos = this.hashes[i].hash(value) & (arr[i].length - 1);
			arr[i][pos]+= 1;
			//System.out.println("add " + i + " " + pos);
		}
	}
	
	public int estimate(int value) {		
		int [] counts = new int[this.hashes.length];
		for (int i = 0; i < counts.length; i++) {
			int pos = this.hashes[i].hash(value) & (arr[i].length - 1);
			counts[i] = arr[i][pos];
		}
		return ArrayHelper.minIntArr(counts);
	}
	
	public static void main(String [] args) throws Exception {
		
		HashFuncInt hashes[] = new HashFuncInt[]{
			new HashFuncInt() {

				@Override
				public int hash(int key) {
					// TODO Auto-generated method stub
					return HashFuncs.hash32shift(key);
				}
			},
			new HashFuncInt() {

				@Override
				public int hash(int key) {
					// TODO Auto-generated method stub
					return HashFuncs.hash32shiftmult(key);
				}
			},
			new HashFuncInt() {

				@Override
				public int hash(int key) {
					// TODO Auto-generated method stub
					return HashFuncs.hashJenkins(key);
				}
			},
		};
		
		CountMinSketch cms = new CountMinSketch(hashes, 11);
		for (int i = Short.MIN_VALUE; i < Short.MAX_VALUE; i++)
			cms.add(i);
		
		for (int i = 1; i < 10; i++)
			System.out.println("estimate[" + i + "]=" + cms.estimate(i));
		System.out.println(cms.estimate(12));
	}
}
