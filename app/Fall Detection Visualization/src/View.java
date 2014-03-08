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
				this.imageCanvas.setRotationInDegreee(this.imageCanvas.getRotationInDegreee()+10);
			}
			
			
			this.imageCanvas.repaint();
		}
		
		public void updateImage(float deg)
		{
			this.imageCanvas.setRotationInDegreee(deg);
			this.imageCanvas.repaint();
		}
    }