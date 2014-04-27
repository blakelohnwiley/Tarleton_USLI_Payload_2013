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
public class UV extends JPanel{

    //private XYSeries series;
    private TimeSeries series;
    private TimeSeriesCollection dataset;
    
    private JFreeChart chart;
    
    private ChartPanel chartPanel;
    
    private Integer x = -500;
    
    
    
    public UV() {
        
             
        
        series = new TimeSeries("Time", Second.class);
        
        dataset = new TimeSeriesCollection(series);
       // dataset.addSeries(series);
        chart = ChartFactory.createTimeSeriesChart(
            "UV",
            "Time", 
            "UV",
            dataset,
            true,
            true,
            false
        );
        chart.setBorderVisible(true);
        XYPlot plot = chart.getXYPlot();
        plot.getRangeAxis().setLowerMargin(0.0);
        plot.getRangeAxis().setUpperMargin(0.0);
        
        plot.getRangeAxis().setRange(-10.0,200.0);
        
        DateAxis axis = (DateAxis) plot.getDomainAxis();
        axis.setDateFormatOverride(new SimpleDateFormat("hh:mm:ss"));
        
        // add the chart to a panel...
        chartPanel = new ChartPanel(chart);
        chartPanel.setPreferredSize(new java.awt.Dimension(450, 250));
        
        chartPanel.setMouseWheelEnabled(true);
        
        this.setLayout(new FlowLayout());
        this.add(chartPanel);
       
    }
    private float UV1;
    private float UV2;
    private float UV3;
    private float UV4;
    
    private float maxUV;
    
    
    @Override
    public void paintComponent(Graphics g){
        UV1 = TwoWaySerialComm.UV1;
        UV2 = TwoWaySerialComm.UV2;
        UV3 = TwoWaySerialComm.UV3;
        UV4 = TwoWaySerialComm.UV4;
        
        maxUV = max(UV1,UV2,UV3,UV4);
        
        series.addOrUpdate(new Second(new Date()), maxUV);
       
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
