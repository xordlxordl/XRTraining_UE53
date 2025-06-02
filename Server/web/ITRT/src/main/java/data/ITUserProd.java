package data;

public class ITUserProd
{
	int puid;
	int userid;
	int pid;
	int oid;
	
	String loginid;
	String prodname;
	String orgname;
	String createdat;
	String expiredat;
	String memo;
	
	public String getMemo()
	{
		return memo;
	}
	public void setMemo(String memo)
	{
		this.memo = memo;
	}
	public int getPuid()
	{
		return puid;
	}
	public void setPuid(int puid)
	{
		this.puid = puid;
	}
	
	public int getUserid()
	{
		return userid;
	}
	public void setUserid(int userid)
	{
		this.userid = userid;
	}
	public int getPid()
	{
		return pid;
	}
	
	public void setPid(int pid)
	{
		this.pid = pid;
	}
	public int getOid()
	{
		return oid;
	}
	
	public void setOid(int oid)
	{
		this.oid = oid;
	}
	public String getLoginid()
	{
		return loginid;
	}
	public void setLoginid(String loginid)
	{
		this.loginid = loginid;
	}
	
	public String getProdname()
	{
		return prodname;
	}
	public void setProdname(String prodname)
	{
		this.prodname = prodname;
	}
	
	public String getOrgname()
	{
		return orgname;
	}
	public void setOrgname(String orgname)
	{
		this.orgname = orgname;
	}
	
	public String getCreatedat()
	{
		return createdat;
	}
	public void setCreatedat(String createdat)
	{
		this.createdat = createdat;
	}
	public String getExpiredat()
	{
		return expiredat;
	}
	public void setExpiredat(String expiredat)
	{
		this.expiredat = expiredat;
	}
	
	
	
}
