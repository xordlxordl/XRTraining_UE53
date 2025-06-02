package db;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.TimeZone;


public class DateUtils
{
	

	public static GregorianCalendar ConvertStringToDate( String strDate )
	{

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd" );
		try
		{
			GregorianCalendar cal = new GregorianCalendar();
			cal.setTime( formatter.parse( strDate ) );
			return cal;
		}
		catch( ParseException e )
		{
		
		}
		return null;
	}

	public static GregorianCalendar ConvertStringToDateTime( String strDate )
	{

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		try
		{
			GregorianCalendar cal = new GregorianCalendar();
			cal.setTime( formatter.parse( strDate ) );
			return cal;
		}
		catch( ParseException e )
		{
			
		}
		return null;
	}

	public static GregorianCalendar ConvertTimeStringToDate( String strTime )
	{
		GregorianCalendar cal = new GregorianCalendar();

		String[] strSplitTime = strTime.split( ":" );

		cal.set( cal.get( Calendar.YEAR ), cal.get( Calendar.MONTH ), cal.get( Calendar.DAY_OF_MONTH ), Integer.parseInt( strSplitTime[0] ), Integer.parseInt( strSplitTime[1] ), Integer.parseInt( strSplitTime[2] ) );

		return cal;
	}

	public static String ConvertDateToString( GregorianCalendar cal )
	{
		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd" );
		formatter.setCalendar( cal );
		return formatter.format( cal.getTime() );
	}

	public static String ConvertTimeMillisToString( long timeMillis )
	{
		GregorianCalendar cal = ConvertTimeMillisToCalender( timeMillis );

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		formatter.setCalendar( cal );
		return formatter.format( cal.getTime() );
	}

	public static String ConvertTimeSecToString( long timeSecs )
	{
		GregorianCalendar cal = ConvertTimeSecToCalender( timeSecs );

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		formatter.setCalendar( cal );
		return formatter.format( cal.getTime() );
	}

	public static GregorianCalendar NowTime()
	{
		GregorianCalendar NowCalendar = new GregorianCalendar();
		NowCalendar.setTimeZone( TimeZone.getTimeZone( "Etc/GMT+9" ) );

		return NowCalendar;
	}

	public static GregorianCalendar NowDateWithOutTime()
	{
		GregorianCalendar NowCalendar = new GregorianCalendar();
		NowCalendar.setTimeZone( TimeZone.getTimeZone( "Etc/GMT+9" ) );
		NowCalendar.set( GregorianCalendar.HOUR, 0 );
		NowCalendar.set( GregorianCalendar.MINUTE, 0 );
		NowCalendar.set( GregorianCalendar.SECOND, 0 );
		NowCalendar.set( GregorianCalendar.MILLISECOND, 0 );
		return NowCalendar;
	}

	public static long NowTimeMillis()
	{
		return NowTime().getTimeInMillis();
	}

	public static long GetTimeDiffFromCurrent( long lParam )
	{
		long lNowTime = NowTimeMillis();

		return ( lParam - lNowTime ) / 1000;
	}

	public static long GetMillis( String strTime )
	{
		GregorianCalendar cal = ConvertTimeStringToDate( strTime );

		return cal.getTimeInMillis();
	}

	public static long GetDate2Millis( String strDate )
	{
		GregorianCalendar cal = ConvertStringToDateTime( strDate );
		return cal.getTimeInMillis();
	}

	// 시간 스트링에 AddSec을 더하던지 감소하던지 해서 다시 String으로 리턴.
	public static String GetDateStringAddSec( String strDate, long AddSec )
	{
		long tempMillis = GetDate2Millis( strDate ) + ( AddSec * 1000 );

		GregorianCalendar cal = new GregorianCalendar();
		cal.setTimeInMillis( tempMillis );

		DateFormat formatter = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
		formatter.setCalendar( cal );

		return formatter.format( cal.getTime() );
	}

	public static GregorianCalendar ConvertTimeSecToCalender( long timeSecs )
	{
		GregorianCalendar cal = new GregorianCalendar();
		cal.setTimeZone( TimeZone.getTimeZone( "Etc/GMT+9" ) );
		cal.setTimeInMillis( ConvertGmt9SecToGmt0Sec( timeSecs ) );
		return cal;
	}

	public static GregorianCalendar ConvertTimeMillisToCalender( long timeMillis )
	{
		GregorianCalendar cal = new GregorianCalendar();
		cal.setTimeZone( TimeZone.getTimeZone( "Etc/GMT+9" ) );
		cal.setTimeInMillis( ConvertGmt9ToGmt0( timeMillis ) );
		return cal;
	}

	private static long ConvertGmt9SecToGmt0Sec( long timeSec )
	{
		return ( timeSec - ( 9L * 3600L ) ) * 1000L;
	}

	private static long ConvertGmt9ToGmt0( long timeMillis )
	{
		return ( timeMillis - ( 9L * 3600L ) * 1000L );
	}

	// 시작시간(HH:mm:ss)과 종료시간(HH:mm:ss)안에 입력한 시간이 포함되는지 리턴. add by zihun 130309
	public static Boolean isValidDate( String strStartTime, String strEndTime, String strCheckDate )
	{
		if( true == strStartTime.isEmpty() || true == strEndTime.isEmpty() )
		{
			return false;
		}

		GregorianCalendar calStart = ConvertTimeStringToDate( strStartTime );
		GregorianCalendar calEnd = ConvertTimeStringToDate( strEndTime );
		GregorianCalendar calValue = ConvertStringToDateTime( strCheckDate );

		if( true == calStart.before( calValue ) && true == calEnd.after( calValue ) )
		{
			return true;
		}

		return false;
	}

	// 시작시간(yyyy-MM-dd HH:mm:ss)과 종료시간(yyyy-MM-dd HH:mm:ss)안에 입력한 시간이 포함되는지 리턴. add by zihun 130325
	public static Boolean isValidDate2( String strStartDate, String strEndDate, String strCheckDate )
	{
		if( true == strStartDate.isEmpty() || true == strEndDate.isEmpty() )
		{
			return false;
		}

		GregorianCalendar calStart = ConvertStringToDateTime( strStartDate );
		GregorianCalendar calEnd = ConvertStringToDateTime( strEndDate );
		GregorianCalendar calValue = ConvertStringToDateTime( strCheckDate );

		if( true == calStart.before( calValue ) && true == calEnd.after( calValue ) )
		{
			return true;
		}

		return false;
	}

	public static Boolean isInvalidDate( String strStartTime, String strEndTime, String strCheckDate )
	{
		if( true == strStartTime.isEmpty() || true == strEndTime.isEmpty() )
		{
			return false;
		}

		GregorianCalendar calStart = ConvertTimeStringToDate( strStartTime );
		GregorianCalendar calEnd = ConvertTimeStringToDate( strEndTime );
		GregorianCalendar calValue = ConvertStringToDateTime( strCheckDate );

		if( true == calStart.before( calValue ) && true == calEnd.after( calValue ) )
		{
			return false;
		}

		return true;
	}

	// return + : 현재 요일이 체크 요일보다 뒤 일 경우
	// return 0 : 현재 요일과 체크 요일이 같을 경우
	// return - : 현재 요일이 체크 요일보다 앞일 경우
	// 현재값        체크기준     현재값
	// -		  0		  +
	public static int isInvalidDay( Integer iCheckDay, String strCheckDate )
	{
		GregorianCalendar NowCalendar = new GregorianCalendar();
		NowCalendar = DateUtils.ConvertStringToDate( strCheckDate );
		Integer iCurWeekDay = NowCalendar.get( Calendar.DAY_OF_WEEK );

		return iCurWeekDay - iCheckDay;
	}

}
