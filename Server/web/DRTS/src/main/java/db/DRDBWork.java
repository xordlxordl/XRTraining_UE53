package db;
import data.*;


import java.util.ArrayList;

import org.json.simple.*;


public class DRDBWork 
{
	public DRDBWork()
	{
		
	}
	
	public DRAdmin login( String strID, String strPW, String strIP )
	{
		try
		{
			DRAdmin info = new DRAdmin();
			JSONObject JsonResult = DRQuery.select1st( "p_Login", strID, strPW, strIP );
			
			Integer nResult = (Integer)JsonResult.get("result_");
			info.setResult(nResult);
			String name = "";
			//Integer nGrade = 100;
			
			if( nResult == 1 )
			{
				name = (String)JsonResult.get("admin_name");
			}

			info.setDrpass(name);
			
			
			//info.setAdmingrade(nGrade);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	

	public int DR_JoinUser( DRUser user )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = DRQuery.select1st( "spAddTrainee", user.getSzname(), 
																	   user.getNsex(), 
																	   user.getBirthdate(), 
																	   user.getNsi(), 
																	   user.getNgun(),
																	   user.getNdong(),
																	   user.getBsick(),
																	   user.getSzsick(),
																	   user.getBdrug(),
																	   user.getSzdrug(),
																	   user.getCareeryear(),
																	   user.getCareermonth(),
																	   user.getBdrive(),
																	   user.getBcrash(),
																	   user.getBcrashobject(),
																	   user.getBcrashhuman(),
																	   user.getBcrashonly()
																	   );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			
			if( nResult == 1 )
			{
				return 1;
			}
			
			return 0;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int UpdateTrainee( DRUser user )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = DRQuery.select1st( "spUpdateTrainee", user.getUid(), 
																		  user.getSzname(),
																		  user.getNsex(), 
																		  user.getBirthdate(), 
																		  user.getNsi(), 
																		  user.getNgun(),
																		  user.getNdong(),
																		  user.getBsick(),
																		  user.getSzsick(),
																		  user.getBdrug(),
																		  user.getSzdrug(),
																		  user.getCareeryear(),
																		  user.getCareermonth(),
																		  user.getBdrive(),
																		  user.getBcrash(),
																		  user.getBcrashobject(),
																		  user.getBcrashhuman(),
																		  user.getBcrashonly() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			
			if( nResult == 1 )
			{
				return 1;
			}
			
			return 0;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ArrayList<DRUser> GetTraineeList()
	{
		try
		{
			
			
			ArrayList<DRUser> list = new ArrayList<DRUser>();
			JSONArray JsonResult = DRQuery.select("spGetTraineeList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				DRUser info = new DRUser();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = ((Long)obj.get("id")).intValue();
				info.setUid( uid );
				
				String username = (String)obj.get("szname");
				info.setSzname(username);
				
				Integer nsex = (Integer)obj.get("nsex");
				info.setNsex(nsex);
				
				String szsex = (String)obj.get("szsex");
				info.setSzSex(szsex);
				
				
				String birthdate = String.valueOf( obj.get("birthdate") );
				info.setBirthdate(birthdate);
				
				Integer nsi = (Integer)obj.get("nsi");
				info.setNsi(nsi);
				
				Integer ngun = (Integer)obj.get("ngun");
				info.setNgun(ngun);
				
				Integer ndong = (Integer)obj.get("ndong");
				info.setNdong(ndong);
				
				String szaddr = (String)obj.get("szaddr");
				info.setSzaddr(szaddr);
				
				Integer bsick = (Integer)obj.get("bsick");
				info.setBsick(bsick);
				
				String szsick = (String)obj.get("szsick");
				info.setSzsick(szsick);
				
				Integer bdrug = (Integer)obj.get("bdrug");
				info.setBdrug(bdrug);
				
				String szdrug = (String)obj.get("szdrug");
				info.setSzdrug(szdrug);
				
				Integer careeryear = (Integer)obj.get("careeryear");
				info.setCareeryear(careeryear);
				
				Integer careermonth = (Integer)obj.get("careermonth");
				info.setCareermonth(careermonth);
				
				Integer bdrive = (Integer)obj.get("bdrive");
				info.setBdrive(bdrive);
				
				Integer bcrash = (Integer)obj.get("bcrash");
				info.setBcrash(bcrash);
				
				Integer bcrashobject = (Integer)obj.get("bcrashobject");
				info.setBcrashobject(bcrashobject);
				
				Integer bcrashhuman = (Integer)obj.get("bcrashhuman");
				info.setBcrashhuman(bcrashhuman);
				
				Integer bcrashonly = (Integer)obj.get("bcrashonly");
				info.setBcrashonly(bcrashonly);
				
				String ndatetime = (String)obj.get("ndatetime");
				info.setRegidate(ndatetime);
				
				//Long ndatetime = ((Long)obj.get("ndatetime"));
				//java.sql.Timestamp ts = new java.sql.Timestamp( ndatetime ); 
				//info.setRegidate(ts.toString().substring(0,16));
				
				
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
	
	
	
	public DRUser GetTraineeData( String i_uid )
	{
		try
		{
			
			JSONObject obj = DRQuery.select1st( "spGetTraineeData", i_uid );
		
			DRUser info = new DRUser();
			
			
			Integer uid = ((Long)obj.get("id")).intValue();
			info.setUid( uid );
			
			String username = (String)obj.get("szname");
			info.setSzname(username);
			
			Integer nsex = (Integer)obj.get("nsex");
			info.setNsex(nsex);
			
			String szsex = (String)obj.get("szsex");
			info.setSzSex(szsex);
			
			String birthdate = String.valueOf(obj.get("birthdate")) ;
			info.setBirthdate(birthdate);
			
			Integer nsi = (Integer)obj.get("nsi");
			info.setNsi(nsi);
			
			Integer ngun = (Integer)obj.get("ngun");
			info.setNgun(ngun);
			
			Integer ndong = (Integer)obj.get("ndong");
			info.setNdong(ndong);
			
			String szaddr = (String)obj.get("szaddr");
			info.setSzaddr(szaddr);
			
			Integer bsick = (Integer)obj.get("bsick");
			info.setBsick(bsick);
			
			String szsick = (String)obj.get("szsick");
			info.setSzsick(szsick);
			
			Integer bdrug = (Integer)obj.get("bdrug");
			info.setBdrug(bdrug);
			
			String szdrug = (String)obj.get("szdrug");
			info.setSzdrug(szdrug);
			
			Integer careeryear = (Integer)obj.get("careeryear");
			info.setCareeryear(careeryear);
			
			Integer careermonth = (Integer)obj.get("careermonth");
			info.setCareermonth(careermonth);
			
			Integer bdrive = (Integer)obj.get("bdrive");
			info.setBdrive(bdrive);
			
			Integer bcrash = (Integer)obj.get("bcrash");
			info.setBcrash(bcrash);
			
			Integer bcrashobject = (Integer)obj.get("bcrashobject");
			info.setBcrashobject(bcrashobject);
			
			Integer bcrashhuman = (Integer)obj.get("bcrashhuman");
			info.setBcrashhuman(bcrashhuman);
			
			Integer bcrashonly = (Integer)obj.get("bcrashonly");
			info.setBcrashonly(bcrashonly);
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	
	public Integer DeleteTrainee( String uid )
	{
		try
		{
			
			JSONObject JsonResult = DRQuery.select1st( "spDeleteTrainee", uid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}

	
	
	public ArrayList<DRJuso> getJuso1()
	{
		try
		{
			
			ArrayList<DRJuso> list = new ArrayList<DRJuso>();
			JSONArray JsonResult = DRQuery.select( "spGetJuso1");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				DRJuso info = new DRJuso();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				
				String name = (String)obj.get("szname");
				
				
				info.setName(name);
				
				
				
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

	public ArrayList<DRJuso> getJuso2( int sido )
	{
		try
		{
			
			ArrayList<DRJuso> list = new ArrayList<DRJuso>();
			JSONArray JsonResult = DRQuery.select( "spGetJuso2", sido);
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				DRJuso info = new DRJuso();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				int nid = (Integer)obj.get("nid");
				info.setNid(nid);
				
				String name = (String)obj.get("szname");
				info.setName(name);
				
				
				
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
	
	public ArrayList<DRJuso> getJuso3( int gu )
	{
		try
		{
			
			ArrayList<DRJuso> list = new ArrayList<DRJuso>();
			JSONArray JsonResult = DRQuery.select( "spGetJuso3", gu);
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				DRJuso info = new DRJuso();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				int nid = (Integer)obj.get("nid");
				info.setNid(nid);
				
				String name = (String)obj.get("szname");
				info.setName(name);
				
				
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
	
	public int SetOption( DROption opt )
	{
		try
		{
						
			if( opt == null )
				return 0;
			
			JSONObject JsonResult = DRQuery.select1st( "spSetSimOption", 
														opt.getUid(), 
														opt.getMode(), 
														opt.getTimezone(), 
														opt.getWeather(), 
														opt.getAmount(), 
														opt.getWindspeed(), 
														opt.getWinddir() == null ? 0 : opt.getWinddir() , 
														opt.getbAEBS() );
			
			if( JsonResult == null )
			{
				return 2;
			}
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			
			if( nResult == 1 )
			{
				return 1;
			}
			
			return 0;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}

	public DROption GetOption( String i_uid )
	{
		try
		{
			DROption info = new DROption();
		
			
			JSONObject obj = DRQuery.select1st( "spGetOption", i_uid ); 
														
			
			
			Integer uid = ((Long)obj.get("uid")).intValue();
			info.setUid( uid );
			
			Integer mode = (Integer)obj.get("mode");
			info.setMode(mode);
			
			
			Integer timezone = (Integer)obj.get("time");
			info.setTimezone(timezone);
			
			Integer weather = (Integer)obj.get("weather");
			info.setWeather(weather);
			
			Integer amount = (Integer)obj.get("amount");
			info.setAmount(amount);
			
			Integer windspeed = (Integer)obj.get("windspeed");
			info.setWindspeed(windspeed);
			
			Integer winddir = (Integer)obj.get("winddir");
			if( winddir == null )
				winddir = 0;
			info.setWinddir(winddir.toString() + "°");
			
			Integer baebs = (Integer)obj.get("baebs");
			info.setbAEBS(baebs);
			
			return info;
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}

	
	
}
