<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
    
<%@ page import="java.util.*" %>
<%@ page import="java.io.PrintWriter" %>
<%@ page import="javax.sql.*" %>
<%@ page import="org.json.simple.*" %>
<%@ page import="db.*" %>
<%@ page import="data.*" %>




<%

String sido = request.getParameter("sido");

try
{

	JSONArray ResultArray = DRQuery.select( "spGetJuso2", sido );
	
	out.print( ResultArray );

}
catch(Exception e) 
{
	
	e.printStackTrace();
}
%>
