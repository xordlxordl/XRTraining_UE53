package db;


import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import java.net.URL;

import java.util.Properties;


public class ResourceLoader
{

	public static Properties loadProperties( String fileClassPath )
	{

		if( fileClassPath.startsWith( "classpath:" ) )
		{
			fileClassPath = fileClassPath.substring( "classpath:".length() );
		}

		Properties properties = new Properties();
		URL dataSourceUrl = ResourceLoader.class.getClassLoader().getResource( fileClassPath );
		InputStream inputStream = null;

		try
		{
			inputStream = dataSourceUrl.openStream();
			properties.loadFromXML( inputStream );
		}
		catch( IOException e )
		{
			
		}
		finally
		{
			try
			{
				inputStream.close();
			}
			catch( IOException ignored )
			{
				
			}
		}

		return properties;
	}

	public static Properties loadPropertiesXMLAbsolutePath( String filePath )
	{
		Properties properties = new Properties();
 
		FileInputStream inputStream = null;
		try
		{
			inputStream = new FileInputStream( filePath );
			properties.loadFromXML( inputStream );
		}
		catch( IOException e )
		{
			return null;
		}
		finally
		{
			try
			{
				if( null == inputStream )
				{
					return null;
				}
				inputStream.close();
			}
			catch( IOException ignored )
			{
				
			}
		}

		return properties;
	}

}
