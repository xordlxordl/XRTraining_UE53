
package db;
import data.*;


import java.util.ArrayList;

import org.json.simple.*;


public class ITDBWork 
{
	public ITDBWork()
	{
		
	}
	
	

	public int AddUser( ITUser user )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spAddUser", user.getOid(), 
																	user.getLoginid(), 
																	user.getPass() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			
			if( nResult == 1 )
			{
				return 1;
			}
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int UpdateUser( ITUser user, int bPass )
	{
		try
		{
						
			if( user == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spUpdateUser", user.getUid(), 
																	   user.getOid(), 
																	   user.getLoginid(),
																	   user.getPass(),
																	   bPass );
																	   
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			
			if( nResult == 1 )
			{
				return 1;
			}
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ArrayList<ITUser> GetUserList()
	{
		try
		{
			
			ArrayList<ITUser> list = new ArrayList<ITUser>();
			JSONArray JsonResult = ITQuery.select("spGetUserList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITUser info = new ITUser();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = ((Long)obj.get("id")).intValue();
				info.setUid( uid );
				
				String loginid = (String)obj.get("login_id");
				info.setLoginid(loginid);
				
				
				Integer oid = ((Long)obj.get("organization_id")).intValue();
				info.setOid(oid);
				
				String orgname = (String)obj.get("name");
				info.setOrgname(orgname);
								
				
				
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
	
	
	
	
	public ITUser GetUserData( String i_uid )
	{
		try
		{
			
			JSONObject obj = ITQuery.select1st( "spGetUserData", i_uid );
		
			ITUser info = new ITUser();
			
			
			Integer uid = ((Long)obj.get("id")).intValue();
			info.setUid( uid );
			
			Integer oid = ((Long)obj.get("organization_id")).intValue();
			info.setOid(oid);
			
			String loginid = (String)obj.get("login_id");
			info.setLoginid(loginid);
			
			String orgname = (String)obj.get("name");
			info.setOrgname(orgname);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	
	public Integer DeleteUser( String uid )
	{
		try
		{
			
			JSONObject JsonResult = ITQuery.select1st( "spDeleteUser", uid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ArrayList<ITUser> GetDelUserList()
	{
		try
		{
			
			ArrayList<ITUser> list = new ArrayList<ITUser>();
			JSONArray JsonResult = ITQuery.select("spGetDelUserList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITUser info = new ITUser();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer uid = ((Long)obj.get("id")).intValue();
				info.setUid( uid );
				
				String loginid = (String)obj.get("login_id");
				info.setLoginid(loginid);
				
				
				Integer oid = ((Long)obj.get("organization_id")).intValue();
				info.setOid(oid);
				
				String orgname = (String)obj.get("name");
				info.setOrgname(orgname);
								
				
				
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
	
	
	public Integer RepairUser( String uid )
	{
		try
		{
			
			JSONObject JsonResult = ITQuery.select1st( "spRepairUser", uid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int AddOrg( ITOrg org )
	{
		try
		{
						
			if( org == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spAddORG", org.getType(), org.getName(), org.getMemo() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int UpdateOrg( ITOrg org )
	{
		try
		{
						
			if( org == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spUpdateOrg", org.getOid(), org.getType(), org.getName(), org.getMemo() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public ITOrg GetOrgData( String i_oid )
	{
		try
		{
			
			JSONObject obj = ITQuery.select1st("spGetOrgData", i_oid );
			
			ITOrg info = new ITOrg();
				
				
			Integer oid = ((Long)obj.get("id")).intValue();
			info.setOid( oid );
				
			Integer type = ((Integer)obj.get("type")).intValue();
			info.setType(type);
				
			String typename = (String)obj.get("typename");
			info.setTypename(typename);
				
			String name = (String)obj.get("name");
			info.setName(name);
				
			String memo = (String)obj.get("memo");
			if( memo == null)
				info.setMemo("");
			else
				info.setMemo(memo);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	public ArrayList<ITOrg> GetOrgList()
	{
		try
		{
			
			ArrayList<ITOrg> list = new ArrayList<ITOrg>();
			JSONArray JsonResult = ITQuery.select("spGetOrgList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITOrg info = new ITOrg();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer oid = ((Long)obj.get("id")).intValue();
				info.setOid( oid );
				
				Integer type = ((Integer)obj.get("type")).intValue();
				info.setType(type);
				
				String typename = (String)obj.get("typename");
				info.setTypename(typename);
				
				String name = (String)obj.get("name");
				info.setName(name);
				
				String memo = (String)obj.get("memo");
				if( memo == null)
					info.setMemo("");
				else
					info.setMemo(memo);
				
				
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
	
	public ArrayList<ITOrg> GetDelOrgList()
	{
		try
		{
			
			ArrayList<ITOrg> list = new ArrayList<ITOrg>();
			JSONArray JsonResult = ITQuery.select("spGetDelOrgList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITOrg info = new ITOrg();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer oid = ((Long)obj.get("id")).intValue();
				info.setOid( oid );
				
				Integer type = ((Integer)obj.get("type")).intValue();
				info.setType(type);
				
				String typename = (String)obj.get("typename");
				info.setTypename(typename);
				
				String name = (String)obj.get("name");
				info.setName(name);
				
				String memo = (String)obj.get("memo");
				if( memo == null)
					info.setMemo("");
				else
					info.setMemo(memo);
				
				
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
	
	public Integer DeleteOrg( String oid )
	{
		try
		{
			
			JSONObject JsonResult = ITQuery.select1st( "spDeleteOrg", oid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public Integer RepairOrg( String oid )
	{
		try
		{
			
			JSONObject JsonResult = ITQuery.select1st( "spRepairOrg", oid );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	
	public ArrayList<ITProd> GetProdList()
	{
		try
		{
			
			ArrayList<ITProd> list = new ArrayList<ITProd>();
			JSONArray JsonResult = ITQuery.select("spGetProdList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITProd info = new ITProd();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				Integer pid = ((Long)obj.get("id")).intValue();
				info.setPid( pid );
				
				
				String name  = (String)obj.get("name");
				info.setName(name);
				
				//String strdate = String.valueOf(obj.get("strdate")) ;

				String strdate = (String)obj.get("strdate");
				info.setCreatetime(strdate);
				
				String desc = (String)obj.get("description");
				if( desc == null )
					info.setDesc("");
				else
					info.setDesc(desc);
				
				
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
	
	public ITProd GetProdData( String i_pid )
	{
		try
		{
			
			
			JSONObject obj = ITQuery.select1st("spGetProdData", i_pid);
			
			
			ITProd info = new ITProd();
			
			
			Integer pid = ((Long)obj.get("id")).intValue();
			info.setPid( pid );
			
			
			String name  = (String)obj.get("name");
			info.setName(name);
			
			//String strdate = String.valueOf(obj.get("strdate")) ;

			String strdate = (String)obj.get("strdate");
			info.setCreatetime(strdate);
			
			String desc = (String)obj.get("description");
			if( desc == null )
				info.setDesc("");
			else
				info.setDesc(desc);
				
				
			
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	
	public int AddProd( ITProd prod )
	{
		try
		{
						
			if( prod == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spAddProd",  prod.getName(), prod.getDesc() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int UpdateProd( ITProd prod )
	{
		try
		{
						
			if( prod == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spUpdateProd", prod.getPid(), prod.getName(), prod.getDesc() );
			
			Integer nResult = ((Integer)JsonResult.get("result")).intValue();
			
			return nResult;
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	
	public ITResult AddUserProd( ITUserProductData user )
	{
		try
		{
						
			
			JSONObject obj = ITQuery.select1st( "spAddUserProd", user.getUid(), 
																		user.getPid(), 
																		user.getCreateat(),
																		user.getExpiredat(),
																		user.getMemo());
			
			ITResult info = new ITResult();
			Integer result = ((Integer)obj.get("result")).intValue();
			
			info.setResult( result );
			if( result == 1 )
			{
				String loginid  = (String)obj.get("loginid");
				info.setLoginid(loginid);
				
				String prodname = (String)obj.get("prodname");
				info.setProdname(prodname);
				
			}
			
			return info;
			
			
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
	}
	
	public ArrayList<ITUserProd> GetUserProdList()
	{
		try
		{
			
			ArrayList<ITUserProd> list = new ArrayList<ITUserProd>();
			JSONArray JsonResult = ITQuery.select("spGetUserProdList");
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITUserProd info = new ITUserProd();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				//Integer puid = ((Long)obj.get("puid")).intValue();
				Long puid = (Long)obj.get("puid");
				info.setPuid( (Integer)puid.intValue() );
				
				Integer uid = ((Long)obj.get("user_id")).intValue();
				info.setUserid( uid );
				
				Integer pid = ((Long)obj.get("product_id")).intValue();
				info.setPid( pid );
				
				Integer oid = ((Long)obj.get("organization_id")).intValue();
				info.setOid( oid );
				
				String memo  = (String)obj.get("memo");
				if( memo == null)
					memo ="";
				info.setMemo( memo );
				
				String login_id  = (String)obj.get("login_id");
				info.setLoginid(login_id);
				

				String productname = (String)obj.get("productname");
				info.setProdname(productname);
				
				String orgname = (String)obj.get("orgname");
				info.setOrgname(orgname);
				
				String createdat = (String)obj.get("strcreatedat");
				info.setCreatedat(createdat);
				
				String expiredat = (String)obj.get("strexpiredat");
				if( expiredat == null )
					expiredat = "";
				info.setExpiredat(expiredat);
				
				
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
	
	public ArrayList<ITUserProductData> GetUserProdDataList( String i_uid )
	{
		try
		{
			
			ArrayList<ITUserProductData> list = new ArrayList<ITUserProductData>();
			JSONArray JsonResult = ITQuery.select("spGetUserProd", i_uid );
			
			int i;
			for( i = 0; i < JsonResult.size(); ++i )
			{
				ITUserProductData info = new ITUserProductData();
				JSONObject obj = (JSONObject)JsonResult.get(i);
				
				//Integer puid = ((Long)obj.get("puid")).intValue();
				Long puid = (Long)obj.get("id");
				info.setPuid( (Integer)puid.intValue() );
				
				Integer uid = ((Long)obj.get("user_id")).intValue();
				info.setUid( uid );
				
				Integer pid = ((Long)obj.get("product_id")).intValue();
				info.setPid( pid );
				
				
				String name  = (String)obj.get("name");
				info.setProdname(name);
				
				String created_at  = (String)obj.get("strcreatedat");
				info.setCreateat( created_at );
				

				String expired_at = (String)obj.get("strexpiredat");
				if( expired_at == null)
					expired_at = "";
				
				info.setExpiredat( expired_at );
				
				String memo = (String)obj.get("memo");
				if( memo == null )
					memo = "";
				
				info.setMemo(memo);
				
				
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
	
	public ITUserProductData GetUserProdToIndex( String i )
	{
		try
		{
			
			ITUserProductData info = new ITUserProductData();
			JSONObject obj = ITQuery.select1st("spGetUserProdToIndex", i );
				
				//Integer puid = ((Long)obj.get("puid")).intValue();
			Long puid = (Long)obj.get("id");
			info.setPuid( (Integer)puid.intValue() );
			
			Integer uid = ((Long)obj.get("user_id")).intValue();
			info.setUid( uid );
			
			Integer pid = ((Long)obj.get("product_id")).intValue();
			info.setPid( pid );
			
			String created_at  = (String)obj.get("strcreatedat");
			info.setCreateat( created_at );
			

			String expired_at = (String)obj.get("strexpiredat");
			if( expired_at == null)
				expired_at = "";
			
			info.setExpiredat( expired_at );
			
			
			String memo = (String)obj.get("memo");
			if( memo == null )
				memo = "";
			
			info.setMemo(memo);
			
			return info;
			
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		return null;
		
	}
	
	
	public int UpdateUserProd( ITUserProductData userprod )
	{
		try
		{
						
			if( userprod == null )
				return 0;
			
			JSONObject JsonResult = ITQuery.select1st( "spUpdateUserProd", 
														userprod.getPuid(), 
														userprod.getUid(), 
														userprod.getPid(), 
														userprod.getCreateat(), 
														userprod.getExpiredat(), 
														userprod.getMemo() );
			
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
