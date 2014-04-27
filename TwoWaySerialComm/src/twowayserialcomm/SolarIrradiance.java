/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package twowayserialcomm;

import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Graphics;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.swing.JFrame;
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
public class SolarIrradiance extends JPanel{
   
    //private XYSeries series;
    private TimeSeries series;
    private TimeSeriesCollection dataset;
    
    private JFreeChart chart;
    
    private ChartPanel chartPanel;
    
    private Integer x = -500;
    
    
    
    public SolarIrradiance() {
        
             
        
        series = new TimeSeries("Time", Second.class);
        
        dataset = new TimeSeriesCollection(series);
       // dataset.addSeries(series);
        chart = ChartFactory.createTimeSeriesChart(
            "Solar Irradiance",
            "Time", 
            "W/(m^2)",
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
        
        this.setLayout(new FlowLayout());
        this.add(chartPanel);
       
    }
    private float SI1;
    private float SI2;
    private float SI3;
    private float SI4;    
    
    private float maxSI;
        
    @Override
    public void paintComponent(Graphics g){
        SI1 = TwoWaySerialComm.solarIrr1;
        SI2 = TwoWaySerialComm.solarIrr2;
        SI3 = TwoWaySerialComm.solarIrr3;
        SI4 = TwoWaySerialComm.solarIrr4;
        
        maxSI = max(SI1,SI2,SI3,SI4);
        
        series.addOrUpdate(new Second(new Date()), maxSI);
       
        this.repaint();
    }
     private float max(float f1,float f2, float f3, float f4){
         float fArray[] = {f1,f2,f3,f4};
         float max = 0;
         for(int i = 0; i < 4; i++){
             if(fArray[i]>max){
                 max = fArray[i];
             }
         }
         return max;
     }
    
}
