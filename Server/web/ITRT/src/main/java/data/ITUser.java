package data;

public class ITUser
{
	private int uid;	//유저인덱스
	private int oid;	//조직ID
	private String loginid;		//Account
	private String orgname;		//org 이름
	private String pass;
    
	public int getUid()
	{
		return uid;
	}
	public void setUid(int uid)
	{
		this.uid = uid;
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
	
	
	public String getOrgname()
	{
		return orgname;
	}
	public void setOrgname(String orgname)
	{
		this.orgname = orgname;
	}
	public String getPass()
	{
		return pass;
	}
	public void setPass(String pass)
	{
		this.pass = pass;
	}
    
}
