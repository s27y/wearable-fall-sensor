package util;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class DataFile {
	FileInputStream fstream;
	BufferedReader br;
	ArrayList<String[]> dataList;
	
	
	public ArrayList<String[]> getDataList() {
		return dataList;
	}

	public DataFile(String url) {
		dataList = new ArrayList<String[]>();
		try {
			fstream = new FileInputStream(url);
			br = new BufferedReader(new InputStreamReader(fstream));
			readFile();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void readFile()
	{
		try {
			String strLine;
			//Read File Line By Line
			while ((strLine = br.readLine()) != null)   {
			  // Print the content on the console
			  System.out.println (strLine);
			  dataList.add(strLine.split(","));
			}

			//Close the input stream
			br.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args)
	{
		new DataFile("data.txt").readFile();
	}
	
	public String [] splitData(String lineStr,String c)
	{
		String [] dataArray  = lineStr.split(c);

		return dataArray;
	}
}
