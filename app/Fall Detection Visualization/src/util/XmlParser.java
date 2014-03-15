package util;

import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Logger;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class XmlParser {
	
	private static Logger LOGGER = Logger.getLogger("InfoLogging");
	
	SAXParserFactory factory = SAXParserFactory.newInstance();
	SAXParser saxParser;
	ArrayList<String> columnName= new ArrayList<String>();
	DefaultHandler handler = new DefaultHandler() {
		public void startElement(String uri, String localName, String qName,
				Attributes attributes) throws SAXException {

			LOGGER.info("Start Element :" + qName);

			if (qName.equalsIgnoreCase("columnName")) {
				isColumnName = true;
			}
		}
		
		public void endElement(String uri, String localName,
				String qName) throws SAXException {
			LOGGER.info("End Element :" + qName);
			}
		
		public void characters(char ch[], int start, int length) throws SAXException {
			 
			if (isColumnName) {
				LOGGER.info("Column name: " + new String(ch, start, length));
				columnName.add(new String(ch, start, length));
				isColumnName = false;
			} 
		}
		};
	

	boolean isColumnName = false;

	public ArrayList<String> loadXML(String url) {
		try {
			saxParser = factory.newSAXParser();
			saxParser.parse(url, handler);
		} catch (ParserConfigurationException | SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return this.columnName;
	}
	
	public static void main(String[] arg)
	{
	}
	
}
