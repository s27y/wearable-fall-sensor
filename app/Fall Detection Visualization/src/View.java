import java.awt.BorderLayout;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JPanel;

public class View extends JPanel implements ActionListener
    {
    	private JPanel controlPnl = new JPanel();
    	private ImageCanvas imageCanvas = new ImageCanvas();
    	
    	private JButton btn = new JButton("load red ");
    	private JButton btn2 = new JButton("rotate ");
        public View()
        {
            super();
            setLayout(new BorderLayout());
            add("Center", this.imageCanvas);
            add("South", this.controlPnl);
            this.controlPnl.add(btn);
            this.controlPnl.add(btn2);
            this.btn.addActionListener(this);
            this.btn2.addActionListener(this);
            
        }

        @Override
        protected void paintComponent(Graphics g)
        {
            super.paintComponent(g);
            g.drawString("Java Application Demo", 350, 350);
        }

		@Override
		public void actionPerformed(ActionEvent e) {
			Object source = e.getSource();
			
			if(source == this.btn)
			{

			}
			else if(source == this.btn2)
			{
			}
			
			
			this.imageCanvas.repaint();
		}
		
		public void updateImage(String str)
		{
			String [] dataArray  = str.split(",");
			this.imageCanvas.getImage()[0].setRotationInDegreee(Float.parseFloat(dataArray[1])*90f);
			this.imageCanvas.getImage()[1].setRotationInDegreee(Float.parseFloat(dataArray[2])*90f);
			this.imageCanvas.getImage()[2].setRotationInDegreee(Float.parseFloat(dataArray[3])*90f);

			this.imageCanvas.repaint();
		}
    }