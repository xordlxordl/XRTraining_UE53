package data;

public class CAdmin
{
	private String adminid;			//아이디
    private String adminpass;	//비번
    private int admingrade;
    private int result;
    
	public int getResult()
	{
		return result;
	}
	public void setResult(int result)
	{
		this.result = result;
	}
	public String getAdminid()
	{
		return adminid;
	}
	public void setAdminid(String adminid)
	{
		this.adminid = adminid;
	}
	public String getAdminpass()
	{
		return adminpass;
	}
	public void setAdminpass(String adminpass)
	{
		this.adminpass = adminpass;
	}
	public int getAdmingrade()
	{
		return admingrade;
	}
	public void setAdmingrade(int admingrade)
	{
		this.admingrade = admingrade;
	}
    
    
    
}
