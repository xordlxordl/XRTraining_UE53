<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<% request.setCharacterEncoding("UTF-8"); %>
<%@ page import="java.io.*, java.util.*, java.lang.*"%>

<%

String filePath = "D:/sample/";
String fileName = "sample.csv";
String orgFilename = "sample.csv";
try 
{
	File file = new File(filePath + File.separator + fileName);
	if( file.isFile() ) 
	{
		byte b[] = new byte[(int) file.length()];
		fileName = new String( fileName.getBytes("euc-kr"), "8859_1" );
		orgFilename = java.net.URLEncoder.encode(orgFilename, "UTF-8");
		
		//response.setHeader("Content-Type", "text/csv; charset=MS949");
		
		response.setHeader("Content-Disposition", "attachment;filename=" + orgFilename);
		
		BufferedInputStream fin = new BufferedInputStream( new FileInputStream(file) );
		BufferedOutputStream outs = new BufferedOutputStream( response.getOutputStream() );
		
		int read = 0;
		while( ( read = fin.read( b ) ) != -1 ) 
		{
			outs.write(b, 0, read);
		}
		outs.close();
		fin.close();
	}
	else 
	{
		System.out.println("File does not exist!");
	}
} 
catch (IOException e) 
{
	System.out.println("download.jsp error!");
}


/*
  // Define filename and filepath
  String filename = "sample.csv"; 
  String filepath = "C:\\sample\\"; 
  
  // Set response to download file
  response.setContentType("APPLICATION/OCTET-STREAM"); 
  response.setHeader("Content-Disposition", "attachment; filename=\"" + filename + "\""); 

  // Input stream for file reading
  java.io.FileInputStream fileInputStream = new java.io.FileInputStream(filepath + filename);
          
  // Read file content and stream it to the response output stream

  int i; 
  while ((i=fileInputStream.read()) != -1) {
    out.write(i); 
  } 

  // Close the FileInputStream to release resources
  fileInputStream.close(); 
  */
%> 