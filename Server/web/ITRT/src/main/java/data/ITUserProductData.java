package data;

public class ITUserProductData
{
	int puid;
	int uid;
	int pid;
	
	String prodname;
	String createat;
	String expiredat;
	String memo;
	
	public int getPuid()
	{
		return puid;
	}
	public void setPuid(int puid)
	{
		this.puid = puid;
	}
	public int getUid()
	{
		return uid;
	}
	public void setUid(int uid)
	{
		this.uid = uid;
	}
	public int getPid()
	{
		return pid;
	}
	public void setPid(int pid)
	{
		this.pid = pid;
	}
	
	public String getProdname()
	{
		return prodname;
	}
	public void setProdname(String prodname)
	{
		this.prodname = prodname;
	}
	
	public String getCreateat()
	{
		return createat;
	}
	public void setCreateat(String createat)
	{
		this.createat = createat;
	}
	
	public String getExpiredat()
	{
		return expiredat;
	}
	public void setExpiredat(String expiredat)
	{
		this.expiredat = expiredat;
	}
	
	public String getMemo()
	{
		return memo;
	}
	public void setMemo(String memo)
	{
		this.memo = memo;
	}


	
}
