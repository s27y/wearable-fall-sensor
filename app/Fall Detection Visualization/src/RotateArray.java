
public class RotateArray {
	private float [] rotateArrayOld = new float[3];
	private float [] rotateArrayNew = new float[3];
	
	
	public RotateArray() {
		super();
		rotateArrayOld[0] = 0f;
		rotateArrayOld[1] = 0f;
		rotateArrayOld[2] = 0f;
	}

	public synchronized void updateOld(float [] f) {
		rotateArrayOld = f;
    }

    public synchronized void updateNew(float [] f) {
    	rotateArrayNew = f;
    }

    public synchronized float [] getRotateArrayOld() {
        return this.rotateArrayOld;
    }
    
    public synchronized float [] getRotateArrayNew() {
        return this.rotateArrayNew;
    }
    
    public synchronized float [] getDifference() {
    	float []fNew = this.getRotateArrayNew();
    	float []fOld = this.getRotateArrayOld();
        float newX = fNew[0] - fOld[0];
        float newY = fNew[1] - fOld[1];
        float newZ = fNew[2] - fOld[2];
        
        float[] diff = {newX, newY, newZ};
        this.updateOld(fNew);
    	return diff;
    }
}
