/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package twowayserialcomm;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.TextArea;
import javax.swing.JPanel;

/**
 *
 * @author John
 */
public class southPanel extends JPanel{
    
    public static Button activateButton = new Button("Activate");
    public static Button stopButton = new Button("Stop");
    public static Button speedButton = new Button("Slow");
    JPanel emptyPanel = new JPanel();
    JPanel emptyPanel1 = new JPanel();
    JPanel emptyPanel2 = new JPanel();
    JPanel emptyPanel3 = new JPanel();
    JPanel emptyPanel4 = new JPanel();
    JPanel emptyPanel5 = new JPanel();
    JPanel emptyPanel6 = new JPanel();
    JPanel emptyPanel7 = new JPanel();
    JPanel emptyPanel8 = new JPanel();
    JPanel emptyPanel9 = new JPanel();
    JPanel emptyPanel10 = new JPanel();
    JPanel emptyPanel11 = new JPanel();
    JPanel emptyPanel12 = new JPanel();
    
    
    southPanel(TextArea textArea){
        super();
        this.setLayout(new BorderLayout());
        
        JPanel buttonPanel = new JPanel(null);
        
        
        buttonPanel.setLayout(new GridLayout(5,3));
        
        //activateButton.setBackground(Color.green);
        speedButton.setBackground(Color.orange);
        stopButton.setBackground(Color.red);
        
        //buttonPanel.add(emptyPanel);
        //buttonPanel.add(activateButton);
        buttonPanel.add(emptyPanel1);
        buttonPanel.add(emptyPanel2);
        buttonPanel.add(emptyPanel3);
        buttonPanel.add(emptyPanel4);
        buttonPanel.add(speedButton);        
        buttonPanel.add(emptyPanel5);
        buttonPanel.add(emptyPanel6);
        buttonPanel.add(emptyPanel7);
        buttonPanel.add(emptyPanel8);
        buttonPanel.add(emptyPanel);
        buttonPanel.add(stopButton);
        buttonPanel.add(emptyPanel9);
        buttonPanel.add(emptyPanel10);
        buttonPanel.add(emptyPanel11);
        buttonPanel.add(emptyPanel12);
        this.add(buttonPanel,BorderLayout.EAST);
        this.add(textArea, BorderLayout.CENTER);
    }
    
}
