import java.awt.Dimension;
import java.awt.image.BufferedImage;


public class MyBufferedImage extends BufferedImage{
	
	private Dimension location;
	private float rotationInDegreee=0;

	public MyBufferedImage(int arg0, int arg1, int arg2, Dimension aLocation) {
		super(arg0, arg1, arg2);
		this.location = aLocation;
	}
	
	public Dimension getLocation() {
		return location;
	}

	public void setLocation(Dimension location) {
		this.location = location;
	}

	public float getRotationInDegreee() {
		return rotationInDegreee;
	}

	public void setRotationInDegreee(float rotationInDegreee) {
		this.rotationInDegreee = rotationInDegreee;
	}
	
	
}
