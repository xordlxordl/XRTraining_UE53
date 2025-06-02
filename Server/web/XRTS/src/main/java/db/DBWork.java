package db;
import data.*;

import java.util.ArrayList;

import org.json.simple.*;


public class DBWork 
{
	public DBWork()
	{
		
	}
	
	public CAdmin login( String strID, String strPW )
	{
		try
		{
			CAdmin info = new CAdmin();
			JSONObject JsonResult = Query.select1st( "spLogin", strID, strPW );
			
			Integer nResult = (Integer)JsonResult.get("result");
			Integer nGrade = 100;
			
			if( nResult == 1 )
				nGrade = (Integer)JsonResult.get("grade");
			
			info.setResult(nResult);
			info.setAdmingrade(nGrade);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public User joinAdmin( String strMaker, String strID, String strXRNum, String strName, String strPW )
	{
		try
		{
			if( strName == null)
				return null;
			if( strXRNum == null )
				return null;
			if( strID == null )
				return null;
			
			if( strName.length() < 1 )
				return null;
			if( strXRNum.length() < 1 )
				return null;
			if( strID.length() < 1 )
				return null;
			
			
			User info = new User();
			JSONObject JsonResult = Query.select1st( "spCreateAdmin",strMaker, strID,strXRNum,strName, strPW, 1 );
			
			Integer nResult = (Integer)JsonResult.get("result");
			Integer uid = 0;
			
			//DB에서 넘겨 주는 LAST_INSERT_ID()이 값이 Long 인듯 Integer로 받을때 뻑남 그래서 Long 클레스에서 int로 형변환
			if( nResult == 1 )
				uid = ((Long)JsonResult.get("uid")).intValue();
			
			info.setResult(nResult);
			info.setUid(uid);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public User joinTrainer( String strMaker, String strID, String strXRNum, String strName, String strPW )
	{
		try
		{
			
			if( strName == null)
				return null;
			if( strXRNum == null )
				return null;
			if( strID == null )
				return null;
			
			if( strName.length() < 1 )
				return null;
			if( strXRNum.length() < 1 )
				return null;
			if( strID.length() < 1 )
				return null;
			
			User info = new User();
			JSONObject JsonResult = Query.select1st( "spCreateAdmin",strMaker, strID,strXRNum,strName, strPW, 2 );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			Integer uid = 0;
			
			if( nResult == 1 )
				uid = ((Long)JsonResult.get("uid")).intValue();
			
			info.setResult(nResult);
			info.setUid(uid);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public User joinUser( String strXRNum, String strName )
	{
		try
		{
			if( strName == null)
				return null;
			if( strXRNum == null )
				return null;
			
			if( strName.length() < 1 )
				return null;
			
			if( strXRNum.length() < 1 )
				return null;
			
			User info = new User();
			JSONObject JsonResult = Query.select1st( "spCreateUser",strXRNum,strName );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			Integer uid = 0;
			
			if( nResult == 1 )
				uid = ((Long)JsonResult.get("uid")).intValue();
			
			info.setResult(nResult);
			info.setUid(uid);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public ArrayList<User> getAdminList()
	{
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetAdminList", 1 );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String userid = (String)obj.get("szuserid");
				info.setUserid( userid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	public ArrayList<User> getAdminListLimit( int start )
	{
		int record_start_no = start - 1;
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetAdminListLimit", 1, record_start_no, 10 );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String userid = (String)obj.get("szuserid");
				info.setUserid( userid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}

	public User getAdminData( int i_uid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetAdminData", i_uid );
		
			User info = new User();
			
			Integer uid = (Integer)JsonResult.get("uid");
			info.setUid( uid );
			
			String userid = (String)JsonResult.get("szuserid");
			info.setUserid( userid );
			
			String usernumber = (String)JsonResult.get("szxrnumber");
			info.setUsernumber( usernumber );
			
			String username = (String)JsonResult.get("szname");
			info.setUsername( username );
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public ArrayList<User> getTrainerList()
	{
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetAdminList", 2 );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String userid = (String)obj.get("szuserid");
				info.setUserid( userid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	public ArrayList<User> getTrainerListLimit( int start )
	{
		int record_start_no = start - 1; 
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetAdminListLimit", 2, record_start_no, 10 );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String userid = (String)obj.get("szuserid");
				info.setUserid( userid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	//Admin 과 같은 내용이지만 일관된 인터페이스를 위해 관제자도 만들어 준다
	public User getTrainerData( int i_uid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetAdminData", i_uid );
		
			User info = new User();
			
			Integer uid = (Integer)JsonResult.get("uid");
			info.setUid( uid );
			
			String userid = (String)JsonResult.get("szuserid");
			info.setUserid( userid );
			
			String usernumber = (String)JsonResult.get("szxrnumber");
			info.setUsernumber( usernumber );
			
			String username = (String)JsonResult.get("szname");
			info.setUsername( username );
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public ArrayList<User> getUserList()
	{
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetUserList" );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				String userxrmachine = (String)obj.get("szxrmachine");
				if( userxrmachine == null )
					info.setUserxrmachine( "" );
				else
					info.setUserxrmachine( userxrmachine );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	public ArrayList<User> getUserListLimit( int start )
	{
		int record_start_no = start - 1; 
		try
		{
			
			ArrayList<User> list = new ArrayList<User>();
			JSONArray JsonResult = Query.select( "spGetUserListLimit", record_start_no, 10 );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				User info = new User();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = (Integer)obj.get("uid");
				info.setUid( uid );
				
				String usernumber = (String)obj.get("szxrnumber");
				info.setUsernumber( usernumber );
				
				String username = (String)obj.get("szname");
				info.setUsername( username );
				
				String userxrmachine = (String)obj.get("szxrmachine");
				if( userxrmachine == null )
					info.setUserxrmachine( "" );
				else
					info.setUserxrmachine( userxrmachine );
				
				list.add( info );
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	public User getUserData( int i_uid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetUserData", i_uid );
		
			User info = new User();
			
			Integer uid = (Integer)JsonResult.get("uid");
			info.setUid( uid );
			
			String usernumber = (String)JsonResult.get("szxrnumber");
			info.setUsernumber( usernumber );
			
			String username = (String)JsonResult.get("szname");
			info.setUsername( username );
			
			String userxrmachine = (String)JsonResult.get("szxrmachine");
			if( userxrmachine == null )
				info.setUserxrmachine( "" );
			else
				info.setUserxrmachine( userxrmachine );
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public Integer GetAdminCount()
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetAdminCount", 1 );
			
			Integer nResult = ((Long)JsonResult.get("ncount")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer GetTrainerCount()
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetAdminCount", 2 );
			
			Integer nResult = ((Long)JsonResult.get("ncount")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer GetUserCount()
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spGetUserCount");
			
			Integer nResult = ((Long)JsonResult.get("ncount")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateAdmin( int uid, String id, String num, String name, String pass )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spUpdateAdmin", uid, id, num, name, pass );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateAdminWithOutPass( int uid, String id, String num, String name )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spUpdateAdminWithOutPass", uid, id, num, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer DeleteAdmin( int uid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteAdmin", uid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateTrainer( int uid, String id, String num, String name, String pass )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spUpdateAdmin", uid, id, num, name, pass );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateTrainerWithOutPass( int uid, String id, String num, String name )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spUpdateAdminWithOutPass", uid, id, num, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateUser( int uid, String num, String name )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spUpdateUser", uid, num, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer DeleteUser( int uid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteUser", uid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}

	public Integer CreateXRMashine( String name )
	{
		try
		{
			
			if( name == null)
				return 0;
			if( name.length() < 1 )
				return 0;
			
			JSONObject JsonResult = Query.select1st( "spCreateXRMashine", name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}

	public ArrayList<XRMachine> GetXRMachine()
	{
		try
		{
			
			ArrayList<XRMachine> list = new ArrayList<XRMachine>();
			JSONArray JsonResult = Query.select( "spGetXRList" );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				XRMachine info = new XRMachine();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer nid = (Integer)obj.get("nid");
				info.setNid( nid );
				
				String name = (String)obj.get("szname");
				info.setName(name);
				
				Integer uid = (Integer)obj.get("uid");
				
				if( uid == null )
					info.setUid( 0 );
				else
					info.setUid( uid );
				
				list.add(info);
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public Integer UpdateXR( int nid, String name )
	{
		try
		{
			if( name == null )
				return -99;
			if( name.length() < 1 )
				return -99;
			
			JSONObject JsonResult = Query.select1st( "spUpdateXR", nid, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer DeleteXR( int nid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteXRMashine", nid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer CreateServer( int nid, int mapid, String name, String ip, int port )
	{
		try
		{
			if( name == null)
				return -99;
			if( name.length() < 1 )
				return -99;
			
			if( mapid == 0)
				return -99;
			
			if( ip == null)
				return -99;
			if( ip.length() < 1 )
				return -99;
			
			if( port == 0)
				return -99;
			
			
			JSONObject JsonResult = Query.select1st( "spCreateServer", nid, mapid, name, ip, port );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer UpdateServer( int nid, int mapid, String name, String ip, int port )
	{
		try
		{
			if( name == null)
				return -99;
			if( name.length() < 1 )
				return -99;
			
			if( mapid == 0)
				return -99;
			
			if( ip == null)
				return -99;
			if( ip.length() < 1 )
				return -99;
			
			if( port == 0)
				return -99;
			
			
			JSONObject JsonResult = Query.select1st( "spUpdateServer", nid, mapid, name, ip, port );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ArrayList<Server> GetServerList()
	{
		try
		{
			
			ArrayList<Server> list = new ArrayList<Server>();
			JSONArray JsonResult = Query.select( "spGetServerList" );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				Server info = new Server();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer nid = (Integer)obj.get("nid");
				info.setNid( nid );
				
				Integer mapid = (Integer)obj.get("mapid");
				info.setMapid( mapid );
				
				String name = (String)obj.get("szname");
				info.setName(name);
				
				String ip = (String)obj.get("ip");
				info.setIp( ip );
				
				Integer port = (Integer)obj.get("port");
				info.setPort( port );
				
				Integer bon = (Integer)obj.get("bon");
				info.setBon( bon );
				
				list.add(info);
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;

	}
	
	public Integer DeleteServer( int nid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteServer", nid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	

	public Integer CreateMap( int nid, String name )
	{
		try
		{
			if( nid < 1 )
				return -99;
			if( name == null)
				return -99;
			if( name.length() < 1 )
				return -99;
			
			JSONObject JsonResult = Query.select1st( "spCreateMap", nid, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	public ArrayList<CMap> GetMapList()
	{
		try
		{
			
			ArrayList<CMap> list = new ArrayList<CMap>();
			JSONArray JsonResult = Query.select( "spGetMapList" );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				CMap info = new CMap();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer nid = (Integer)obj.get("nid");
				info.setNid( nid );

				String name = (String)obj.get("szname");
				info.setName(name);
				
				list.add(info);
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;

	}
	
	public Integer UpdateMap( int nid, String name )
	{
		try
		{
			if( name == null )
				return -99;
			if( name.length() < 1 )
				return -99;
			
			JSONObject JsonResult = Query.select1st( "spUpdateMap", nid, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer DeleteMap( int nid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteMap", nid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer CreateTraining( String name )
	{
		try
		{
			
			if( name == null)
				return 0;
			if( name.length() < 1 )
				return 0;
			
			JSONObject JsonResult = Query.select1st( "spCreateTraining", name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ArrayList<Training> GetTrainingList()
	{
		try
		{
			
			ArrayList<Training> list = new ArrayList<Training>();
			JSONArray JsonResult = Query.select( "spGetTrainingList" );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				Training info = new Training();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer nid = (Integer)obj.get("nid");
				info.setNid( nid );

				String name = (String)obj.get("szname");
				info.setName(name);
				
				list.add(info);
			}
			
			return list;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;

	}
	
	public Integer UpdateTraining( int nid, String name )
	{
		try
		{
			if( name == null )
				return -99;
			if( name.length() < 1 )
				return -99;
			
			JSONObject JsonResult = Query.select1st( "spUpdateTraining", nid, name );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer DeleteTraining( int nid )
	{
		try
		{
			
			JSONObject JsonResult = Query.select1st( "spDeleteTraining", nid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
}
