/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package twowayserialcomm;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.swing.JPanel;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.time.Second;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;

/**
 *
 * @author John
 */
public class altitudeGraph extends JPanel{

    //private XYSeries series;
    private TimeSeries series;
    private TimeSeriesCollection dataset;
    
    private JFreeChart chart;
    
    private ChartPanel chartPanel;
    
    private Integer x = -500;
    
    public altitudeGraph() {
        
        
        series = new TimeSeries("Altitude", Second.class);
        
        dataset = new TimeSeriesCollection(series);
       // dataset.addSeries(series);
        chart = ChartFactory.createTimeSeriesChart(
            "Altitude",
            "Time", 
            "Height",
            dataset,
            true,
            true,
            false
        );
        chart.setBorderVisible(true);
        XYPlot plot = chart.getXYPlot();
        plot.getRangeAxis().setLowerMargin(0.0);
        plot.getRangeAxis().setUpperMargin(0.0);
        
        plot.getRangeAxis().setRange(-10.0,6000.0);
        DateAxis axis = (DateAxis) plot.getDomainAxis();
        axis.setDateFormatOverride(new SimpleDateFormat("hh:mm:ss"));
        
        // add the chart to a panel...
        chartPanel = new ChartPanel(chart);
        chartPanel.setPreferredSize(new java.awt.Dimension(450, 250));
        
        chartPanel.setMouseWheelEnabled(true);
        chartPanel.setBackground(new Color(0.598f,0.312f,0.769f));
        this.setBackground(new Color(0.598f,0.312f,0.769f));
        this.setLayout(new FlowLayout());
        this.add(chartPanel);
       
    }
    private float altitude;
    @Override
    public void paintComponent(Graphics g){
       // series.add(x,x*x); 
        if(TwoWaySerialComm.altitude != 0){
            altitude = (float)TwoWaySerialComm.altitude/10;
            series.addOrUpdate(new Second(new Date()),altitude);
        }
        //dataset.addSeries(series);
        
        
        this.repaint();
    }
    
    
}
