package db;

import java.util.ArrayList;
import org.json.simple.*;

import data.*;

public class arDBWork
{
	public arDBWork()
	{
		
	}
	
	public arAdmin spLogin( String strID, String strPass )
	{
		try
		{
			
			
			arAdmin info = new arAdmin();
			
			if( strPass == null )
			{
				info.setResult(-4);
				return info;
			}
			
			JSONObject JsonResult = arQuery.select1st( "spLogin", strID, strPass );
			
			Integer nResult = ( Integer )JsonResult.get("result");
			info.setResult( nResult );
			
			if( nResult == 1 )
			{
				int uid = ( Integer )JsonResult.get("uid");
				info.setUid( uid );
				
				String account = ( String )JsonResult.get("account");
				info.setAccount( account );
				
				int nGrade = ( Integer )JsonResult.get("ngrade");
				info.setnGrade( nGrade );
				
			}
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
//---------
//관제자
//---------	
	public int spAddTrainer( arAdmin admin )
	{
		try
		{
						
			if( admin == null )
				return 0;
			
			JSONObject JsonResult = arQuery.select1st( "spAddTrainer", 
														admin.getAccount(), 
														admin.getPass(),
														admin.getNum(),
														admin.getName(),
														admin.getGroup()
														);
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return -100;
	}
	
	public ArrayList< arAdmin > spGetTrainerList()
	{
		try
		{
			
			ArrayList< arAdmin > list = new ArrayList< arAdmin >();
			JSONArray JsonResult = arQuery.select( "spGetTrainerList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arAdmin info = new arAdmin();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				int uid = ( Integer )obj.get("uid");
				info.setUid( uid );
				
				String account = ( String )obj.get("account");
				info.setAccount( account );
				
				String num = ( String )obj.get("num");
				info.setNum( num );
				
				String name = ( String )obj.get("name");
				info.setName( name );
				
				String group = ( String )obj.get("ngroup");
				info.setGroup( group );
				
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
	
	public arAdmin spGetTrainerData( String i_uid )
	{
		try
		{
			
			JSONObject obj = arQuery.select1st( "spGetTrainerData", i_uid );

			arAdmin info = new arAdmin();
				
			int uid = ( Integer )obj.get("uid");
			info.setUid(uid);
				
			String account = ( String )obj.get("account");
			info.setAccount( account );
			
			String num = ( String )obj.get("num");
			info.setNum( num );
			
			String name = ( String )obj.get("name");
			info.setName( name );
			
			String group = ( String )obj.get("ngroup");
			info.setGroup( group );;
				
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public int spUpdateTrainer( arAdmin admin )
	{
		try
		{
						
			if( admin == null )
				return 0;
			
			JSONObject JsonResult = arQuery.select1st( "spUpdateTrainer", admin.getUid(), admin.getGroup(), admin.getPass() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return -100;
	}
	
	
//------------------
// 훈련자
//------------------
	public arResult InsertUserQuery( String query )
	{
		try
		{
			//INSERT INTO arts.tb_user
			//(UID, name, num, nGroup, height, nDateTime)
			//VALUES(0, NULL, NULL, NULL, 0, NULL);
			
			String str = "INSERT INTO arts.tb_user ( name, num, height, nDateTime) VALUES";
			
			StringBuffer queryString = new StringBuffer();
			String[] buffer = query.split(",");
			
			queryString.append( str );
			
			int fullsize = buffer.length / 3;
			
			int i;
			for( i = 0; i < fullsize; ++i )
			{
				int c = i * 3;
				
				queryString.append( "(" );
				
				String str1 = buffer[ c ];
				
				String str2 = buffer[ c + 1 ];
				String str3 = buffer[ c + 2 ];
				
				if( str2.length() < 8 )
				{
					arResult Result = new arResult();
					Result.setnResult(-1);
					Result.setStrError( str1 + " " + str2 + " 군번 오류 ");
					return Result;
					
				}
				
				queryString.append( "'" + str1 + "'," );
				queryString.append( "'" + str2 + "'," );
				queryString.append( str3 + ", now() )" );
				
				if( i < fullsize - 1 )
					queryString.append( "," );
				else
					queryString.append( ";" );
					
				
			}
			
			
			arResult iResult = arQuery.ExecQuery( queryString.toString() );
			
			
			if( iResult.getnResult() == 1 )
				iResult.setnResult( fullsize );
			
			return iResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public int spAddUser( arUser user )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = arQuery.select1st( "spAddUser", user.getUsername(), user.getUsernum(), user.getUsergroup(), user.getHeight() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return -100;
	}
	
	public ArrayList< arUser > spGetUserFindList( String i_name )
	{
		try
		{
			
			ArrayList< arUser > list = new ArrayList< arUser >();
			JSONArray JsonResult = arQuery.select( "spGetUserFindList", i_name);
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arUser info = new arUser();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				int uid = ( Integer )obj.get("uid");
				info.setUid(uid);
				
				String name = ( String )obj.get("name");
				info.setUsername( name );
				
				String num = ( String )obj.get("num");
				info.setUsernum( num );
				
				String ngroup = ( String )obj.get("ngroup");
				info.setUsergroup( ngroup );
				
				
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
	
	public ArrayList< arUser > spGetUserList()
	{
		try
		{
			
			ArrayList< arUser > list = new ArrayList< arUser >();
			JSONArray JsonResult = arQuery.select( "spGetUserList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arUser info = new arUser();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				int uid = ( Integer )obj.get("uid");
				info.setUid(uid);
				
				String name = ( String )obj.get("name");
				info.setUsername( name );
				
				String num = ( String )obj.get("num");
				info.setUsernum( num );
				
				String ngroup = ( String )obj.get("ngroup");
				info.setUsergroup( ngroup );
				
				int height = ( Integer )obj.get("height");
				info.setHeight( height );
				
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
	
	public arUser spGetUserData( String i_uid )
	{
		try
		{
			JSONObject obj = arQuery.select1st( "spGetUserData", i_uid );

			arUser info = new arUser();
				
			int uid = ( Integer )obj.get("uid");
			info.setUid(uid);
				
			String name = ( String )obj.get("name");
			info.setUsername( name );
				
			String num = ( String )obj.get("num");
			info.setUsernum( num );
				
			String ngroup = ( String )obj.get("ngroup");
			info.setUsergroup( ngroup );
			
			int height = ( Integer )obj.get("height");
			info.setHeight( height );
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public int spUpdateUser( arUser user )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = arQuery.select1st( "spUpdateUser", user.getUid(), 
																	   user.getUsername(), 
																	   user.getUsernum(), 
																	   user.getUsergroup(),
																	   user.getHeight());
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return -100;
	}

//-------------------------------------------------------------------------------------------------	
//장비
//-------------------------------------------------------------------------------------------------
	
	public int spUpdateMachine( arMachine data )
	{
		try
		{
						
			if( data == null )
				return 0;
			
			if( data.getBPC() == null )
				return -200;
			if( data.getBPC().length() < 1 )
				return -200;
			if( data.getHMD() == null)
				return -200;
			if( data.getHMD().length() < 1 )
				return -200;
			
			if( data.getUTT1() == null )
				return -200;
			if( data.getUTT1().length() < 1 )
				return -200;
			
			if( data.getUTT2() == null )
				return -200;
			if( data.getUTT2().length() < 1 )
				return -200;
			
			if( data.getUTT3() == null )
				return -200;
			if( data.getUTT3().length() < 1 )
				return -200;
			
			if( data.getUTT4() == null )
				return -200;
			if( data.getUTT4().length() < 1 )
				return -200;
			
			JSONObject JsonResult = arQuery.select1st( "spUpdateMachine", data.getnID(), 
																		  data.getBPC(), 
																		  data.getHMD(), 
																		  data.getUTT1(),
																		  data.getUTT2(),
																		  data.getUTT3(),
																		  data.getUTT4() ); 
					
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return -100;
	}
	
	public arMachine spGetMachineData( String i_nid )
	{
		try
		{
			
			JSONObject obj = arQuery.select1st( "spGetMachineData", i_nid );
			
			arMachine info = new arMachine();
			
			Integer nid = ( Integer )obj.get("nid");
			info.setnID( nid );
			
			String name = ( String )obj.get("name");
			info.setName(name);
			
			String bpc = ( String )obj.get("bpc");
			info.setBPC(bpc);
			
			String hmd = ( String )obj.get("hmd");
			info.setHMD(hmd);
			
			String utt1 = ( String )obj.get("utt1");
			info.setUTT1(utt1);
			
			String utt2 = ( String )obj.get("utt2");
			info.setUTT2(utt2);
			
			String utt3 = ( String )obj.get("utt3");
			info.setUTT3(utt3);
			
			String utt4 = ( String )obj.get("utt4");
			info.setUTT4(utt4);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public ArrayList<arMachine> spGetMachineList()
	{
		try
		{
			
			ArrayList< arMachine > list = new ArrayList< arMachine >();
			JSONArray JsonResult = arQuery.select( "spGetMachineList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arMachine info = new arMachine();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("nid");
				info.setnID( nid );
				
				String name = ( String )obj.get("name");
				info.setName(name);
				
				String bpc = ( String )obj.get("bpc");
				info.setBPC(bpc);
				
				String hmd = ( String )obj.get("hmd");
				info.setHMD(hmd);
				
				String utt1 = ( String )obj.get("utt1");
				info.setUTT1(utt1);
				
				String utt2 = ( String )obj.get("utt2");
				info.setUTT2(utt2);
				
				String utt3 = ( String )obj.get("utt3");
				info.setUTT3(utt3);
				
				String utt4 = ( String )obj.get("utt4");
				info.setUTT4(utt4);
				
				
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
	
	public ArrayList< arTrData > spGetTrDataList()
	{
		try
		{
			
			ArrayList< arTrData > list = new ArrayList< arTrData >();
			JSONArray JsonResult = arQuery.select( "spGetTrDataList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arTrData info = new arTrData();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer tid = ( Integer )obj.get("tid");
				info.setTID( tid );
				
				String nstarttime = ( String )obj.get("nstarttime");
				info.setStartTime( nstarttime );
				
				String nendtime = ( String )obj.get("nendtime");
				info.setEndTime( nendtime );
				
				String PlayTime = ( String )obj.get("playtime");
				info.setPlayTime(PlayTime);
				
				String name = ( String )obj.get("name");
				info.setTrName( name );
				
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
	
	public ArrayList< arTrData > spGetUIDfromTrList( String uid )
	{
		try
		{
			
			ArrayList< arTrData > list = new ArrayList< arTrData >();
			JSONArray JsonResult = arQuery.select( "spGetUIDfromTrList", uid );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arTrData info = new arTrData();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer tid = ( Integer )obj.get("tid");
				info.setTID( tid );
				
				String nstarttime = ( String )obj.get("nstarttime");
				info.setStartTime( nstarttime );
				
				String nendtime = ( String )obj.get("nendtime");
				info.setEndTime( nendtime );
				
				String PlayTime = ( String )obj.get("playtime");
				info.setPlayTime(PlayTime);
				
				String name = ( String )obj.get("name");
				info.setTrName( name );
				
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
	
	public arTrTitle spGetTrTitle( String tid )
	{
		try
		{
			
			JSONObject obj = arQuery.select1st( "spGetTrTitle", tid );
			
			arTrTitle info = new arTrTitle();
			
			Integer TID = ( Integer )obj.get("tid");
			info.setTID(TID);
			
			String nstarttime = ( String )obj.get("nstarttime");
			info.setStartTime( nstarttime );
			
			String title = ( String )obj.get("tname");
			info.setTitle(title);
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	//arTrUserData
	
	public ArrayList< arTrUserData > spGetTRUserList( String i_TID )
	{
		try
		{
			
			ArrayList< arTrUserData > list = new ArrayList< arTrUserData >();
			JSONArray JsonResult = arQuery.select( "spGetTrUserList", i_TID);
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arTrUserData info = new arTrUserData();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				int nID = ( Integer )obj.get("i");
				info.setnID(nID);
				
				int TID = ( Integer )obj.get("tid");
				info.setTID(TID);
				
				int UID = ( Integer )obj.get("uid");
				info.setUID(UID);
				
				
				String job = ( String )obj.get("job");
				info.setJob(job);
				
				String mname = ( String )obj.get("mname");
				info.setMname( mname );
				
				String name = ( String )obj.get("name");
				info.setName( name );
				
				
				String num = ( String )obj.get("num");
				info.setNum( num );
				
				
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
	
	
	public arLogDetail spGetUserLogDetail( String tid, String uid )
	{
		try
		{
			
			JSONObject obj = arQuery.select1st( "spGetUserLogDetail", tid, uid );
			
			arLogDetail info = new arLogDetail();
			
			Integer firecount = ( Integer )obj.get("firecount");
			info.setFireCount( firecount );
			
			Integer hitcount = ( Integer )obj.get("hitcount");
			info.setHitCount( hitcount );
			
			Integer challengecount = ( Integer )obj.get("challengecount");
			info.setChallengeCount( challengecount );
			
			Integer surrendercount = ( Integer )obj.get("surrendercount");
			info.setSurrenderCount( surrendercount );
			
			Integer bindcount = ( Integer )obj.get("bindcount");
			info.setBindCount(bindcount);
			
			Integer commandmovecount = ( Integer )obj.get("commandmovecount");
			info.setCommandMoveCount( commandmovecount );
			
			Integer breifcount = ( Integer )obj.get("breifcount");
			info.setBreifCount( breifcount );
			
			Integer callcount = ( Integer )obj.get("callcount");
			info.setCallCount( callcount );
			
			Integer eodcount = ( Integer )obj.get("eodcount");
			info.setEodCount( eodcount );
			
			Integer badfirecount = ( Integer )obj.get("badfirecount");
			info.setBadFireCount( badfirecount );
			
			String playtime = ( String )obj.get("playtime");
			info.setPlayTime( playtime );
			
			String nstarttime = ( String )obj.get("nstarttime");
			info.setnStartTime( nstarttime );
			
			String nendtime = ( String )obj.get("nendtime");
			info.setnEndTime( nendtime );
			
			String tname = ( String )obj.get("tname");
			info.setTName( tname );
			
			String job = ( String )obj.get("job");
			info.setJob( job );
			
			String mname = ( String )obj.get("mname");
			info.setMName( mname );
			
			String uname = ( String )obj.get("uname");
			info.setUName( uname );
			
			String num = ( String )obj.get("num");
			info.setNum( num );
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------	
	
	public ArrayList< arRole > spGetRoleList()
	{
		try
		{
			
			ArrayList< arRole > list = new ArrayList< arRole >();
			JSONArray JsonResult = arQuery.select( "spGetRoleList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arRole info = new arRole();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer rid = ( Integer )obj.get("rid");
				info.setRID( rid );
				
				String rname = ( String )obj.get("rname");
				info.setRName( rname );
				
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
	
	
//-------------------------------------------------------------------------------------------------
//파일
//-------------------------------------------------------------------------------------------------
	
	public ArrayList< arFile > spGetFBXList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetFBXList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetMCTList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetMCTList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetPNGList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetPNGList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetSCTList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetSCTList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetTCTList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetTCTList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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

	public ArrayList< arFile > spGetWAVList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetWAVList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetWCTList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetWCTList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList< arFile > spGetAllFileList()
	{
		try
		{
			
			ArrayList< arFile > list = new ArrayList< arFile >();
			JSONArray JsonResult = arQuery.select( "spGetAllFileList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arFile info = new arFile();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("i");
				info.setNid( nid );
				
				String filename = ( String )obj.get("filename");
				info.setFilename( filename );
				
				
				Integer ver = ( Integer )obj.get("ver");
				info.setVer(ver);
				
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
	
	public ArrayList<arMachine> webGetMCList()
	{
		try
		{
			
			ArrayList< arMachine > list = new ArrayList< arMachine >();
			JSONArray JsonResult = arQuery.select( "webGetMCList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arMachine info = new arMachine();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer nid = ( Integer )obj.get("nid");
				info.setnID( nid );
				
				String name = ( String )obj.get("name");
				info.setName(name);
				
				String bpc = ( String )obj.get("bpc");
				info.setBPC(bpc);
				
				String hmd = ( String )obj.get("hmd");
				info.setHMD(hmd);
				
				String utt1 = ( String )obj.get("utt1");
				info.setUTT1(utt1);
				
				String utt2 = ( String )obj.get("utt2");
				info.setUTT2(utt2);
				
				String utt3 = ( String )obj.get("utt3");
				info.setUTT3(utt3);
				
				String utt4 = ( String )obj.get("utt4");
				info.setUTT4(utt4);
				
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
	
	public ArrayList< arTrData > webGetTrDataListDesc()
	{
		try
		{
			
			ArrayList< arTrData > list = new ArrayList< arTrData >();
			JSONArray JsonResult = arQuery.select( "webGetTrDataListDesc");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				arTrData info = new arTrData();
				JSONObject obj = ( JSONObject )JsonResult.get(i);
				
				Integer tid = ( Integer )obj.get("tid");
				info.setTID( tid );
				
				String nstarttime = ( String )obj.get("nstarttime");
				info.setStartTime( nstarttime );
				
				String nendtime = ( String )obj.get("nendtime");
				info.setEndTime( nendtime );
				
				String PlayTime = ( String )obj.get("playtime");
				info.setPlayTime(PlayTime);
				
				String name = ( String )obj.get("name");
				info.setTrName( name );
				
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
	public JSONArray webGetRandmonData()
	{
		return arQuery.select( "webGetRandomData");
	}
	public JSONArray webGetMonthUserData()
	{
		return arQuery.select( "webGetMonthUserData");
	}
	public JSONArray webGetTrainingPer()
	{
		return arQuery.select( "webGetTrainingPer");
	}
	public JSONArray webGetTrainingCount()
	{
		return arQuery.select( "webGetTrainingCount");
	}
	public JSONArray webGetTrainingTime()
	{
		return arQuery.select( "webGetTrainingTime");
	}
		
}
