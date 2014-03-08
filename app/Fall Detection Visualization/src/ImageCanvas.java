import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;


public class ImageCanvas extends JPanel{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	int width = 300;
	int height = 300;
	
	
	private float rotationInDegreee=0;
	
	public float getRotationInDegreee() {
		return rotationInDegreee;
	}

	public void setRotationInDegreee(float rotationInDegreee) {
		this.rotationInDegreee = rotationInDegreee;
	}

	private BufferedImage mBufferedImage;
	public ImageCanvas() {
		super();
		this.setBackground(Color.white);
		mBufferedImage = new BufferedImage(5, 50,
				BufferedImage.TYPE_INT_RGB);
	}

	@Override
	public void paintComponent(Graphics g) {
		final Graphics2D g2 = (Graphics2D) g;
		
		//back ground image
		BufferedImage image = new BufferedImage(width, height,
				BufferedImage.TYPE_INT_RGB);
		Graphics2D blankG = image.createGraphics();
		blankG.setColor(Color.WHITE);
		blankG.fillRect(0, 0, width, height);
		g.drawImage(image, 0, 0, this);
		
		
		//mBufferedImage
		blankG = mBufferedImage.createGraphics();
		blankG.setColor(Color.red);
		blankG.fillRect(0, 0, mBufferedImage.getWidth(), mBufferedImage.getHeight());
		blankG.drawImage(mBufferedImage, 0, 0, this);
		
        final AffineTransform affineTransform = new AffineTransform();
        
        int centreX = mBufferedImage.getWidth() / 2;
		int centreY = mBufferedImage.getHeight() / 2;
		
        affineTransform.translate(145+centreX, 
        		125 +centreY);
        // transforms are applied in reverse order
        affineTransform.rotate(Math.toRadians(rotationInDegreee));
        
		// translate the center of the image to the top
		affineTransform.translate(centreX, 0);
		
		g2.drawImage(mBufferedImage,affineTransform, this);
		
		this.revalidate();
	}
	
	public void rotateImage()
	{
		

	}

	public BufferedImage getImage() {
		return mBufferedImage;
	}

	public void setImage(BufferedImage image) {
		this.mBufferedImage = image;
	}
}
