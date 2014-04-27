/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package twowayserialcomm;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;

public class TwoWaySerialComm implements ActionListener {

    JFrame f;
    public static TextArea textArea;
    public static Panel graphingPanel;
    public static int humidity;
    public static int lux;
    public static int temperature;
    public static int pressure;
    public static int altitude;
    public static float solarIrr1;
    public static float solarIrr2;
    public static float solarIrr3;
    public static float solarIrr4;
    public static float UV1;
    public static float UV2;
    public static float UV3;
    public static float UV4;
    public static char command = '*';

    public TwoWaySerialComm() {
        super();
        southPanel sPanel;

        f = new JFrame();
        f.setUndecorated(true);
        GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
               
        if(gd.isFullScreenSupported()){            
            gd.setFullScreenWindow(f.getOwner());
        }
        f.setTitle("Tarleton USLI Ground Station");

        f.setLayout(new BorderLayout());
        f.setBackground(new Color(0.598f, 0.312f, 0.769f));

        graphingPanel = new Panel(new GridLayout(2, 3));

        graphingPanel.add(new humidityGraph());
        graphingPanel.add(new temperatureGraph());
        graphingPanel.add(new pressureGraph());
        graphingPanel.add(new altitudeGraph());
        graphingPanel.add(new SolarIrradiance());
        graphingPanel.add(new UV());
        //graphingPanel.add(new Gui(), BorderLayout.CENTER);
        graphingPanel.setBackground(new Color(0.598f, 0.312f, 0.769f));
        textArea = new TextArea("  ");
        f.add(sPanel = new southPanel(textArea), BorderLayout.SOUTH);

        southPanel.speedButton.addActionListener(this);
        southPanel.stopButton.addActionListener(this);
        southPanel.activateButton.addActionListener(this);

        f.add(graphingPanel, BorderLayout.CENTER);

        f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent windowevent) {
                try {
                    SerialReader.fout.close();
                    SerialReader.in.close();
                    SerialWriter.out.close();
                } catch (Exception ex) {
                    Logger.getLogger(TwoWaySerialComm.class.getName()).log(Level.SEVERE, null, ex);
                }

                System.exit(0);
            }
        });



        f.setBounds(GraphicsEnvironment.getLocalGraphicsEnvironment().getMaximumWindowBounds());
        f.setVisible(true);

    }

    void connect(String portName) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port is currently in use");
        } else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(19200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                InputStream in = serialPort.getInputStream();
                OutputStream out = serialPort.getOutputStream();

                (new Thread(new SerialReader(in))).start();
                (new Thread(new SerialWriter(out))).start();

            } else {
                System.out.println("Error: Only serial ports are handled.");
            }
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == southPanel.speedButton) {
            command = 'l';
            if(southPanel.speedButton.getBackground() == Color.orange){
                southPanel.speedButton.setBackground(Color.blue);
                southPanel.speedButton.setLabel("Fast");
            } else {
                southPanel.speedButton.setBackground(Color.orange);
                southPanel.speedButton.setLabel("Slow");
            }
        }
        if (e.getSource() == southPanel.stopButton) {
            command = 'd';
            if(southPanel.stopButton.getBackground() == Color.red){
                southPanel.stopButton.setBackground(Color.green);
                southPanel.stopButton.setLabel("Go");
            } else {
                southPanel.stopButton.setBackground(Color.red);
                southPanel.stopButton.setLabel("Stop");
            }
        }
        if (e.getSource() == southPanel.activateButton) {
            command = 'a';
        }
        //throw new UnsupportedOperationException("Not supported yet.");
    }

    /**
     *      */
    public static class SerialReader implements Runnable {

        public static InputStream in;
        String telemetryString = "";
        char telemetryChar = '*';
        String[] values;
        public static PrintStream fout;

        public SerialReader(InputStream in) {
            this.in = in;
        }

        public void run() {
            byte[] buffer = new byte[1024];
            int len = -1;
            try {
                this.fout = new PrintStream(new FileOutputStream("telemetryData.txt"));
            } catch (FileNotFoundException ex) {
                Logger.getLogger(TwoWaySerialComm.class.getName()).log(Level.SEVERE, null, ex);
            }

            try {

                while (true) {

                    telemetryChar = (char) this.in.read();
                    if (telemetryChar > 47 && telemetryChar < 58 || telemetryChar == '\n' || telemetryChar == 32 || telemetryChar == '-') {
                        telemetryString += telemetryChar;

                        if (telemetryChar == '\n') {
                            values = telemetryString.split(" ");

                            humidity = Integer.parseInt(values[1]);
                            lux = Integer.parseInt(values[2]);
                            temperature = Integer.parseInt(values[3]);
                            pressure = Integer.parseInt(values[4]);
                            altitude = Integer.parseInt(values[5]);

                            //6
                            //7
                            //8
                            //9
                            //10

                            solarIrr1 = (float) (Integer.parseInt(values[9]) / 100);
                            solarIrr2 = (float) (Integer.parseInt(values[10]) / 100);
                            solarIrr3 = (float) (Integer.parseInt(values[11]) / 100);
                            solarIrr4 = (float) (Integer.parseInt(values[12]) / 100);

                            UV1 = (float) (Integer.parseInt(values[13]) / 100);
                            UV2 = (float) (Integer.parseInt(values[14]) / 100);
                            UV3 = (float) (Integer.parseInt(values[15]) / 100);
                            UV4 = (float) (Integer.parseInt(values[16]) / 100);


                            textArea.append(telemetryString);
                            fout.append(telemetryString);
                            fout.append("/n");

                            telemetryString = "";
                        }
                    }
                }
            } catch (IOException e) {
                
                textArea.append(e.getMessage());
                e.printStackTrace();
            }
        }
    }

    /**
     *      */
    public static class SerialWriter implements Runnable {

        public static OutputStream out;

        public SerialWriter(OutputStream out) {
            this.out = out;
        }

        public void run() {
            while (true) {
                try {

                    if (command == 'a' || command == 'd' || command == 'l') {
                        this.out.write(command);
                        command = '*';
                    }
                } catch (IOException e) {
                    textArea.append(e.toString());
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {


        try {
            (new TwoWaySerialComm()).connect("COM17");

        } catch (Exception e) {
            // TODO Auto-generated catch block
            textArea.append(e.toString());
            e.printStackTrace();
        }

    }

}
