import java.awt.Color;
import java.awt.Dimension;
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
	private MyBufferedImage[] mBufferedImage = new MyBufferedImage[3];
	
	
	public ImageCanvas() {
		super();
		this.setBackground(Color.white);
		mBufferedImage[0] = new MyBufferedImage(5, 50,
				BufferedImage.TYPE_INT_RGB,new Dimension(20,125));
		mBufferedImage[1] = new MyBufferedImage(5, 50,
				BufferedImage.TYPE_INT_RGB,new Dimension(100,125));
		mBufferedImage[2] = new MyBufferedImage(5, 50,
				BufferedImage.TYPE_INT_RGB,new Dimension(180,125));
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
		
		
		for(MyBufferedImage bi : mBufferedImage)
		{
			//mBufferedImage
			blankG = bi.createGraphics();
			blankG.setColor(Color.red);
			blankG.fillRect(0, 0, bi.getWidth(), bi.getHeight());
			blankG.drawImage(bi, 0, 0, this);
			
			final AffineTransform affineTransform = new AffineTransform();
	        
	        int centreX = bi.getWidth() / 2;
			int centreY = bi.getHeight() / 2;
			
	        affineTransform.translate(bi.getLocation().getWidth()+centreX, 
	        		bi.getLocation().getHeight()+centreY);
	        // transforms are applied in reverse order
	        affineTransform.rotate(Math.toRadians(bi.getRotationInDegreee()));
	        
			// translate the center of the image to the top
			affineTransform.translate(centreX, 0);
			
			g2.drawImage(bi,affineTransform, this);
		}
		
		
        
		
		this.revalidate();
	}
	
	public void rotateImage()
	{
		

	}

	public MyBufferedImage[] getImage() {
		return mBufferedImage;
	}

	public void setImage(MyBufferedImage image) {
		this.mBufferedImage[0] = image;
	}
}
