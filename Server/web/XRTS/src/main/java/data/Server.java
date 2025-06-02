package data;

public class Server
{
	private int nid;
    private int mapid;	//시나리오(맵) ID
    private String name;
    private String ip;
	private int port;
	private int bon;
	
	public int getNid()
	{
		return nid;
	}
	public void setNid(int nid)
	{
		this.nid = nid;
	}
	public int getMapid()
	{
		return mapid;
	}
	public void setMapid(int mapid)
	{
		this.mapid = mapid;
	}
	public String getName()
	{
		return name;
	}
	public void setName(String name)
	{
		this.name = name;
	}
	public String getIp()
	{
		return ip;
	}
	public void setIp(String ip)
	{
		this.ip = ip;
	}
	public int getPort()
	{
		return port;
	}
	public void setPort(int port)
	{
		this.port = port;
	}
	public int getBon()
	{
		return bon;
	}
	public void setBon(int bon)
	{
		this.bon = bon;
	}
	
	

    
}
