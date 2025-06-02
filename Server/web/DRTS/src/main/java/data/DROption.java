package data;

public class DROption
{
	private int uid;
	private int mode;
	
	private int timezone;
	private int weather;
	private int amount;
	private int windspeed;
	private String winddir;
	private int bAEBS;
	
	

	public int getUid()
	{
		return uid;
	}
	public void setUid(int uid)
	{
		this.uid = uid;
	}
	

	
	public int getMode()
	{
		return mode;
	}
	public void setMode(int mode)
	{
		this.mode = mode;
	}
	
	public int getTimezone()
	{
		return timezone;
	}
	public void setTimezone(int timezone)
	{
		this.timezone = timezone;
	}
	
	public int getWeather()
	{
		return weather;
	}
	public void setWeather(int weather)
	{
		this.weather = weather;
	}
	
	public int getAmount()
	{
		return amount;
	}
	public void setAmount(int amount)
	{
		this.amount = amount;
	}
	
	public int getWindspeed()
	{
		return windspeed;
	}
	public void setWindspeed(int windspeed)
	{
		this.windspeed = windspeed;
	}
	
	public String getWinddir()
	{
		if( winddir == null )
			return "0";
		return winddir;
	}
	public void setWinddir(String winddir)
	{
		if( winddir == null)
			this.winddir ="0";
		else
			this.winddir =  winddir.replaceAll("[^0-9]","");
	}
	
	public int getbAEBS()
	{
		return bAEBS;
	}
	public void setbAEBS(int bAEBS)
	{
		this.bAEBS = bAEBS;
	}
	
	
}
