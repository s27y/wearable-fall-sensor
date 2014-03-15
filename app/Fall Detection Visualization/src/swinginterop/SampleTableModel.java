package swinginterop;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.chart.LineChart;

import javax.swing.table.AbstractTableModel;

import util.DataFile;
import util.XmlParser;

/**
 * SampleTableModel
 */
public class SampleTableModel extends AbstractTableModel {
	private static ObservableList<LineChart.Series> bcData;

	private ArrayList<String> names;
	
	public SampleTableModel() {
		super();
		names = new XmlParser().loadXML("config/config.xml");
		
		getData("data.txt");
		System.out.println(names.size());
		//names = new ArrayList<String>();
		//names.add("x");
		//names.add("y");
		//names.add("z");
	}

	/**private Object[][] data = {
			{ new Double(567), new Double(956), new Double(1154) },
			{ new Double(1292), new Double(1665), new Double(1927) },
			{ new Double(1292), new Double(2559), new Double(2774) } };
			*/
	private ArrayList<String[]> dataList;
	private ArrayList<float[]> floatList = new  ArrayList<float[]>();
	
	public ObservableList<LineChart.Series> getLineChartData() {
		if (bcData == null) {
			bcData = FXCollections.<LineChart.Series> observableArrayList();
			for (int column = 1; column < getColumnCount(); column++) {
			//for (int row = 0; row < getRowCount(); row++) {
				ObservableList<LineChart.Data> series = FXCollections
						.<LineChart.Data> observableArrayList();
				for (int row = 0; row < getRowCount(); row++) {
				//for (int column = 0; column < getColumnCount(); column++) {
					series.add(new LineChart.Data(dataList.get(row)[0],
							getValueAt(row, column)));
					
				}
				bcData.add(new LineChart.Series(series));
			}
		}
		return bcData;
	}
	
	public void getData(String url)
	{
		dataList = new DataFile("data.txt").getDataList();
		for(String[] strArray : dataList)
		{
			float[] floatArray = new float[strArray.length];
			for(int i = 0; i < strArray.length; i++) {
				if(Float.parseFloat(strArray[i])<1.1f)
				floatArray[i] = Float.parseFloat(strArray[i]);
				else
					floatArray[i] = 0;
			}
			floatList.add(floatArray);
		}
	}
	

	public double getTickUnit() {
		return 1000;
	}

	public List getColumnNames() {
		return Arrays.asList(names);
	}

	@Override
	public int getRowCount() {
		return floatList.size();
	}

	@Override
	public int getColumnCount() {
		return names.size();
	}

	@Override
	public Object getValueAt(int row, int column) {
		return floatList.get(row)[column];
	}

	@Override
	public String getColumnName(int column) {
		return names.get(column);
	}
	
	public String getRowName(int row) {
		return names.get(row);
	}

	@Override
	public Class getColumnClass(int column) {
		return getValueAt(0, column).getClass();
	}

	@Override
	public boolean isCellEditable(int row, int column) {
		return true;
	}

	@Override
	public void setValueAt(Object value, int row, int column) {
		try{
			floatList.get(row)[column] = (float) value;
		}catch(NumberFormatException e)
		{
			
		}
		

		fireTableCellUpdated(row, column);
	}

}