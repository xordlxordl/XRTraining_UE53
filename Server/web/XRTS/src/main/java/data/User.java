package data;

public class User 
{
	private String userid;			//아이디
    private String username;		//이름
    private String usernumber;		//관리자, 관제자, 훈련자 등 번호 (신분증번호, 군번 등이 될수 도 있음 )
    private String userpassword;	//비번
    private String userxrmachine;
    
	private int usergrade;			//등급 0: 슈퍼, 1:관리자, 2:관제자, 100:훈련자
	private int uid;
	private int result;
    
	
	
    public String getUserxrmachine()
	{
		return userxrmachine;
	}
	public void setUserxrmachine(String userxrmachine)
	{
		this.userxrmachine = userxrmachine;
	}
	public int getResult()
	{
		return result;
	}
	public void setResult(int result)
	{
		this.result = result;
	}
	public int getUid()
	{
		return uid;
	}
	public void setUid(int uid)
	{
		this.uid = uid;
	}
	public String getUsernumber()
	{
		return usernumber;
	}
	public void setUsernumber(String usernumber)
	{
		this.usernumber = usernumber;
	}
	public String getUserid()
	{
		return userid;
	}
	public void setUserid(String userid)
	{
		this.userid = userid;
	}
	
	public String getUsername()
	{
		return username;
	}
	public void setUsername(String username)
	{
		this.username = username;
	}
	public String getUserpassword()
	{
		return userpassword;
	}
	public void setUserpassword(String userpassword)
	{
		this.userpassword = userpassword;
	}
	public int getUsergrade()
	{
		return usergrade;
	}
	public void setUsergrade(int usergrade)
	{
		this.usergrade = usergrade;
	}

	
}
