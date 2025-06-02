

package db;

import data.arResult;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.sql.Types;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;




import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class arQuery
{

	private static String	driver = "org.mariadb.jdbc.Driver";
	private static String	pooldriver = "org.apache.commons.dbcp.PoolingDriver";

	private static String	str_connect = "jdbc:apache:commons:dbcp:/arts_db";
	

	// 
	private static boolean	bErrorLogDBException			= false;
	private static long		reTryErrorLogDBExceptionMillis	= 1000 * 60 * 10;									// 10분 
	private static long		reTryErrorLogDBExcute			= 0L;
	
	
	public static void configure()
	{
		
	}

	public static JSONArray selectError( String alias, Object... params )
	{
		
		JSONArray result = ( JSONArray ) executeError( true, alias, params );
		return result;
	}

	public static JSONArray select( String alias, Object... params )
	{

		JSONArray result = ( JSONArray ) execute( true, alias, params );
		return result;
	}

	public static JSONObject select1st( String alias, Object... params )
	{
		JSONArray result = select( alias, params );
		if( result == null )
			return null;

		JSONObject first = new JSONObject();
		if( result.size() == 0 )
			return first;

		first = ( JSONObject ) result.get( 0 );
		return first;
	}

	public static int update( String alias, Object... params )
	{
		Object result = execute( false, alias, params );
		if( result == null )
		{
			return 0;
		}
		return ( int ) result;
	}

	private static String decideUrl()
	{
		return str_connect;
	}

	private static Object executeError( Boolean doGetResult, String alias, Object... params )
	{
		long nowTime = DateUtils.NowTimeMillis();

		if( true == bErrorLogDBException )
		{
			if( nowTime > reTryErrorLogDBExcute )
			{
				bErrorLogDBException = false;
				reTryErrorLogDBExcute = 0L;
			
			}
			else
			{
				return null;
			}
		}

		String query = makeQueryString( alias, params );
		String url = decideUrl();
		if( url == null )
		{
			return null;
		}

		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;

		long bindTime = -1;
		long execTime = -1;
		long fetchTime = -1;
		long finishTime = -1;
		long finalTime = -1;
		long connDelay = -1;

		for( int i = 0; i < 1; i++ )
		{

			long startAt = System.currentTimeMillis();
			boolean bFailed = false;

			try
			{
				// connect
				Class.forName( driver );
				Class.forName( pooldriver );

				long connStartAt = System.currentTimeMillis();
				DriverManager.setLoginTimeout( 1 );
				conn = DriverManager.getConnection( url );
				long connEndAt = System.currentTimeMillis();
				connDelay = connEndAt - connStartAt;
				if( connDelay >= 1000 )
				{
				
				}
				DriverManager.setLoginTimeout( 60 );

				// prepare
				pstmt = conn.prepareStatement( query );
				pstmt.setQueryTimeout( 60 );
				// bind
				int argn = 1;
				for( Object o : params )
				{
					if( o == null )
						pstmt.setString( argn++, null );
					else if( o.getClass() == String.class )
						pstmt.setString( argn++, ( String ) o );
					else if( o.getClass() == Integer.class )
						pstmt.setInt( argn++, ( int ) o );
					else if( o.getClass() == Long.class )
						pstmt.setLong( argn++, ( Long ) o );
					else if( o.getClass() == Float.class )
						pstmt.setFloat( argn++, ( Float ) o );
					else
					{
						// Log.err("invalid class type: %s", o.getClass().toString());
						return null;
					}
				}

				bindTime = DateUtils.NowTimeMillis() - nowTime;
				// execute
				if( doGetResult )
				{
					rs = pstmt.executeQuery();
					JSONArray result = convertResultSetToJSON( rs );
					execTime = DateUtils.NowTimeMillis() - nowTime;
					return result;
				}

				int result = pstmt.executeUpdate();
				/*
				 * if(Config.getInt("jtds", 0) == 1 && result == 0){ result = -1; }
				 */
				fetchTime = DateUtils.NowTimeMillis() - nowTime;
				return result;

			}
			catch( Exception e )
			{
				StringBuilder builder = new StringBuilder();
				for( Object o : params )
				{
					if( o == null )
						builder.append( "null" );
					else if( o.getClass() == String.class )
						builder.append( ( String ) o );
					else if( o.getClass() == Integer.class )
						builder.append( String.valueOf( ( Integer ) o ) );
					else if( o.getClass() == Long.class )
						builder.append( String.valueOf( ( Long ) o ) );
					else if( o.getClass() == Float.class )
						builder.append( String.valueOf( ( Float ) o ) );

					builder.append( ", " );
				}
				
				bFailed = true;
			}
			finally
			{
				finalTime = DateUtils.NowTimeMillis() - nowTime;
				if( rs != null )
				{
					try
					{
						rs.close();
					}
					catch( SQLException e )
					{
					}
					rs = null;
				}
				if( pstmt != null )
				{
					try
					{
						pstmt.close();
					}
					catch( SQLException e )
					{
					}
					pstmt = null;
				}
				if( conn != null )
				{
					try
					{
						conn.close();
					}
					catch( SQLException e )
					{
					}
					conn = null;
				}
				finishTime = DateUtils.NowTimeMillis() - nowTime;

				

				if( finishTime > 1000 )
				{
				
				}
			}

			if( bFailed )
			{
				bErrorLogDBException = true;
				reTryErrorLogDBExcute = System.currentTimeMillis();
				reTryErrorLogDBExcute += reTryErrorLogDBExceptionMillis;
			}

		}

		return null;
	}

	private static Object execute( Boolean doGetResult, String alias, Object... params )
	{
		String query = makeQueryString( alias, params );
		String url = decideUrl();
		if( url == null )
		{
			return null;
		}

		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;

		long nowTime = DateUtils.NowTimeMillis();
		long bindTime = -1;
		long execTime = -1;
		long coverJsonTime = -1;
		long fetchTime = -1;
		long finishTime = -1;
		long finalTime = -1;
		long connDelay = -1;

		for( int i = 0; i < 1; i++ )
		{

			long startAt = System.currentTimeMillis();
			boolean bFailed = false;

			try
			{
				// connect
				Class.forName( driver );
				Class.forName( pooldriver );

				long connStartAt = System.currentTimeMillis();
				// DriverManager.setLoginTimeout(1);
				conn = DriverManager.getConnection( url );
				long connEndAt = System.currentTimeMillis();
				connDelay = connEndAt - connStartAt;
				if( connDelay >= 1000 )
				{
					// Log.log(Level.INFO, "(%s)SLOW GET DB CONNECTION %d. ", url.split("/")[1], connDelay);
				}
				DriverManager.setLoginTimeout( 60 );

				// prepare
				pstmt = conn.prepareStatement( query );
				pstmt.setQueryTimeout( 60 );
				// bind
				int argn = 1;
				for( Object o : params )
				{
					if( o == null )
						pstmt.setString( argn++, null );
					else if( o.getClass() == String.class )
						pstmt.setString( argn++, ( String ) o );
					else if( o.getClass() == Integer.class )
						pstmt.setInt( argn++, ( int ) o );
					else if( o.getClass() == Long.class )
						pstmt.setLong( argn++, ( Long ) o );
					else if( o.getClass() == Float.class )
						pstmt.setFloat( argn++, ( Float ) o );
					else
					{
						// Log.err("invalid class type: %s", o.getClass().toString());
						return null;
					}
				}

				bindTime = DateUtils.NowTimeMillis() - nowTime;
				// execute
				if( doGetResult )
				{
					rs = pstmt.executeQuery();
					execTime = DateUtils.NowTimeMillis() - nowTime;
					JSONArray result = convertResultSetToJSON( rs );
					coverJsonTime = DateUtils.NowTimeMillis() - nowTime;
					return result;
				}

				int result = pstmt.executeUpdate();
				/*
				 * if(Config.getInt("jtds", 0) == 1 && result == 0){ result = -1; }
				 */
				fetchTime = DateUtils.NowTimeMillis() - nowTime;
				return result;

			}
			catch( Exception e )
			{
				// Log.log(Level.ERROR, "query=[%s]", query);
				// Log.log(Level.ERROR, "url=[%s]", url);
				// 2019-07-02 dhjin, sp 오류 시 로그 강화
				StringBuilder builder = new StringBuilder();
				for( Object o : params )
				{
					if( o == null )
						builder.append( "null" );
					else if( o.getClass() == String.class )
						builder.append( ( String ) o );
					else if( o.getClass() == Integer.class )
						builder.append( String.valueOf( ( Integer ) o ) );
					else if( o.getClass() == Long.class )
						builder.append( String.valueOf( ( Long ) o ) );
					else if( o.getClass() == Float.class )
						builder.append( String.valueOf( ( Float ) o ) );

					builder.append( ", " );
				}
				
				bFailed = true;
			}
			finally
			{
				finalTime = DateUtils.NowTimeMillis() - nowTime;
				if( rs != null )
				{
					try
					{
						rs.close();
					}
					catch( SQLException e )
					{
					}
					rs = null;
				}
				if( pstmt != null )
				{
					try
					{
						pstmt.close();
					}
					catch( SQLException e )
					{
					}
					pstmt = null;
				}
				if( conn != null )
				{
					try
					{
						conn.close();
					}
					catch( SQLException e )
					{
					}
					conn = null;
				}
				finishTime = DateUtils.NowTimeMillis() - nowTime;

			

				//if (finishTime > 1000) {
				//if (execTime > 450) {
				if( execTime - bindTime > 5000 )
				{
				}

				if( connDelay > 10000 )
				{
				}

				long endAt = System.currentTimeMillis();
				if( ( ( endAt - startAt ) ) > 1000 )
				{
					// Log.log(Level.INFO, "[query]%s/%s %d millisec", url.split("/")[1], alias, endAt - startAt);
				}
			}

			if( bFailed )
			{
				// Log.log(Level.ERROR, "Retry %d query %s", i, alias);
				try
				{
					Thread.sleep( 100 );
				}
				catch( InterruptedException e )
				{
				}
			}

		}

		return null;
	}

	public static boolean delete( String tableName )
	{
		String query = makeDeleteQueryString( tableName );
		int checkLine = 0;
		return executeQuery( query, checkLine );
	}

	public static boolean insert( String tableName, List< String > paramName, List< String > paramValue )
	{
		String query = makeInsertQueryString( tableName, paramName, paramValue );
		int checkLine = 1;
		return executeQuery( query, checkLine );
	}

	public static boolean executeQuery( String query, int checkLine )
	{
		String url = decideUrl();
		if( url == null )
		{
			
			return false;
		}

		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;

		long nowTime = DateUtils.NowTimeMillis();
		long bindTime = -1;
		long execTime = -1;
		long fetchTime = -1;
		long finishTime = -1;
		long finalTime = -1;

		boolean isSuccess = true;

		try
		{
			// connect
			Class.forName( driver );
			Class.forName( pooldriver );
			
			long connStartAt = System.currentTimeMillis();
			conn = DriverManager.getConnection( url );
			long connEndAt = System.currentTimeMillis();
			long connDelay = connEndAt - connStartAt;
			if( connDelay >= 1000 )
			{
				// Log.log(Level.INFO, "(%s)SLOW GET DB CONNECTION %d. ", url.split("/")[1], connDelay);
			}
			DriverManager.setLoginTimeout( 60 );

			// prepare
			pstmt = conn.prepareStatement( query );
			pstmt.setQueryTimeout( 60 );
			// bind
			bindTime = DateUtils.NowTimeMillis() - nowTime;
			// execute
			int result = pstmt.executeUpdate();

			if( checkLine > 0 )
			{
				if( result != checkLine )
					isSuccess = false;
			}
			fetchTime = DateUtils.NowTimeMillis() - nowTime;
		}
		catch( Exception e )
		{
			// 2019-07-02 dhjin, sp 오류 시 로그 강화
			isSuccess = false;
		}
		finally
		{
			finalTime = DateUtils.NowTimeMillis() - nowTime;
			if( rs != null )
			{
				try
				{
					rs.close();
				}
				catch( SQLException e )
				{
				}
				rs = null;
			}
			if( pstmt != null )
			{
				try
				{
					pstmt.close();
				}
				catch( SQLException e )
				{
				}
				pstmt = null;
			}
			if( conn != null )
			{
				try
				{
					conn.close();
				}
				catch( SQLException e )
				{
				}
				conn = null;
			}
			finishTime = DateUtils.NowTimeMillis() - nowTime;

			if( finishTime > 1000 )
			{
			}
		}

		return isSuccess;
	}
	
	public static arResult ExecQuery( String query )
	{
		arResult iResult = new arResult();
		
		iResult.setnResult(1);
		iResult.setStrError("ok");
		
		String url = decideUrl();
		if( url == null )
		{
			iResult.setnResult(0);
			iResult.setStrError("url == null");
			return iResult;
		}

		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;


		try
		{
			// connect
			Class.forName( driver );
			Class.forName( pooldriver );
			
			long connStartAt = System.currentTimeMillis();
			conn = DriverManager.getConnection( url );
			long connEndAt = System.currentTimeMillis();
			long connDelay = connEndAt - connStartAt;

			DriverManager.setLoginTimeout( 60 );

			// prepare
			pstmt = conn.prepareStatement( query );
			pstmt.setQueryTimeout( 60 );
			// bind
			
			// execute
			int result = pstmt.executeUpdate();


			
		}
		catch( Exception e )
		{
			iResult.setnResult(0);
			iResult.setStrError(e.getMessage());
			 
		}
		finally
		{
			
			if( rs != null )
			{
				try
				{
					rs.close();
				}
				catch( SQLException e )
				{
				}
				rs = null;
			}
			if( pstmt != null )
			{
				try
				{
					pstmt.close();
				}
				catch( SQLException e )
				{
				}
				pstmt = null;
			}
			if( conn != null )
			{
				try
				{
					conn.close();
				}
				catch( SQLException e )
				{
				}
				conn = null;
			}
		}

		return iResult;
	}
	
	@SuppressWarnings( "unchecked" )
	private static JSONArray convertResultSetToJSON( ResultSet rs )
	{
		try
		{
			JSONArray result = new JSONArray();
			ResultSetMetaData rsmd = rs.getMetaData();

			while( rs.next() )
			{
				JSONObject row = new JSONObject();
				int colCount = rsmd.getColumnCount();
				for( int col = 1; col <= colCount; ++col )
				{
					String colname = rsmd.getColumnName( col );
					String label = rsmd.getColumnLabel( col );
					label = label.toLowerCase();
					int type = rsmd.getColumnType( col );

					switch( type )
					{
					case Types.CHAR:
					case Types.VARCHAR:
					case Types.LONGVARCHAR:
					case Types.NCHAR:
					case Types.NVARCHAR:
					case Types.LONGNVARCHAR:
					case Types.CLOB:
						String str = rs.getString( label );
						if( str != null )
							row.put( label, str.trim() );
						else
							row.put( label, null );
						break;
					case Types.TIMESTAMP:
						String strTime = rs.getString( label );
						// int ts = 0;
						long ts = 0;
						// long ts_m = 0;
						if( strTime != null )
						{
							// ts = dateToTimestamp(strTime.trim());
							ts = dateToLong( strTime.trim() );
							// ts_m = dateToMilliLong(strTime.trim());
						}
						row.put( label, ts );
						// row.put(label+"_m", ts_m);
						break;
					case Types.FLOAT:
						row.put( label, rs.getFloat( label ) );
						break;
					case Types.DOUBLE:
						row.put( label, rs.getFloat( label ) );
						break;
					case Types.BIGINT:
						row.put( label, rs.getLong( label ) );
						break;
					case Types.DATE:
						row.put( label, rs.getDate( label ) );
						break;
					case Types.BIT:
					case Types.BOOLEAN:
						row.put( label, rs.getBoolean( label ) );
						break;
					default:
						row.put( label, rs.getInt( label ) );
					}
				}
				result.add( row );
			}
			return result;
		}
		catch( SQLException e )
		{
			// Log.stackTrace(e);
			return null;
		}
	}

	@SuppressWarnings( "unused" )
	private static int dateToTimestamp( String dateStr )
	{
		if( dateStr == null )
			return 0;

		final int koreaGMT = 9;

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		try
		{
			Date date = ( Date ) formatter.parse( dateStr );
			java.sql.Timestamp ts = new Timestamp( date.getTime() );
			Long ret = ts.getTime() / 1000;
			return ret.intValue() + ( koreaGMT * 3600 );
		}
		catch( ParseException e )
		{
			// Log.stackTrace(e);
		}

		return 0;
	}

	private static long dateToLong( String dateStr )
	{
		if( dateStr == null )
			return 0;

		final int koreaGMT = 9;

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		try
		{
			Date date = ( Date ) formatter.parse( dateStr );
			java.sql.Timestamp ts = new Timestamp( date.getTime() );
			Long ret = ts.getTime() / 1000;
			return ret + ( koreaGMT * 3600 );
		}
		catch( ParseException e )
		{
			// Log.stackTrace(e);
		}

		return 0;
	}

	@SuppressWarnings( "unused" )
	private static long dateToMilliLong( String dateStr )
	{
		if( dateStr == null )
			return 0;

		final int koreaGMT = 9;

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss.SSS" );
		try
		{
			Date date = ( Date ) formatter.parse( dateStr );
			java.sql.Timestamp ts = new Timestamp( date.getTime() );
			Long ret = ts.getTime();
			return ret + ( koreaGMT * 3600000 );
		}
		catch( ParseException e )
		{
			// Log.stackTrace(e);
		}

		return 0;
	}

	private static String makeQueryString( String alias, Object... params )
	{
		String query = String.format( "call %s (", alias );
		StringBuffer queryStr = new StringBuffer();
		queryStr.append( query );

		int paramLen = params.length;
		if( paramLen > 0 )
		{
			queryStr.append( "?" );
			// query += " ?";
			for( int i = 1; i < paramLen; ++i )
			{
				// query += ", ?";
				queryStr.append( ", ?" );
			}
		}
		// query += ";";
		// return query;
		queryStr.append( " );" );
		return queryStr.toString();
	}

	private static String makeInsertQueryString( String tableName, List< String > paramName, List< String > paramValue )
	{
		String query = String.format( "insert into %s (", tableName );
		StringBuffer queryStr = new StringBuffer();
		queryStr.append( query );

		for( int i = 0; i < paramName.size(); i++ )
		{
			String str = paramName.get( i );
			if( i == 0 )
			{
				queryStr.append( str );
			}
			else
			{
				queryStr.append( "," );
				queryStr.append( str );
			}
		}
		queryStr.append( ") values (" );
		for( int i = 0; i < paramValue.size(); i++ )
		{
			String str = paramValue.get( i );
			if( i == 0 )
			{
				queryStr.append( str );
			}
			else
			{
				queryStr.append( "," );
				queryStr.append( str );
			}
		}

		queryStr.append( ");" );
		return queryStr.toString();
	}

	private static String makeDeleteQueryString( String tableName )
	{
		return String.format( "delete from %s", tableName );
	}

	public static String Synchronization( int dB_ID_DDR2 )
	{
		// TODO Auto-generated method stub
		return String.format( "EXEC P_SyncGmToolBaseData" );
	}
}
