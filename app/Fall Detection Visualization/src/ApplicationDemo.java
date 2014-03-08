

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

// To turn an applet into an application, include the main() method
public class ApplicationDemo extends JApplet
{
    // To turn an applet into an application, include the main() method
    public static void main(String[] args)
    {
        // application title and dimensions
        final String title = "Application Title";
        final Dimension applicationFrameSize = new Dimension(640, 480);

        // make an application frame to hold the applet
        final JFrame applicationFrame = new JFrame(title);
        applicationFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        applicationFrame.setSize(applicationFrameSize);

        // place the applet inside the application's frame
        final JApplet applet = new ApplicationDemo();
        applicationFrame.setLayout(new BorderLayout());
        applicationFrame.getContentPane().add("Center", applet);
        //applet.init();
        View v = new View();
        applet.setContentPane(v);
        applicationFrame.setVisible(true);
        startSerialRead(v);
    }

    @Override
    public void init()
    {
        this.setContentPane(new View());
    }
    
    public static void startSerialRead(View v)
    {
    	
    	SerialTest main = new SerialTest();
    	main.v = v;
		main.initialize();
		Thread t=new Thread() {
			public void run() {
				//the following line will keep this app alive for 1000 seconds,
				//waiting for events to occur and responding to them (printing incoming messages to console).
				try {Thread.sleep(1000000);} catch (InterruptedException ie) {}
			}
		};
		t.start();
		System.out.println("Started");
    }

    
    
}