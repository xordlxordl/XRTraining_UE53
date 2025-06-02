package data;

public class arMachine
{
	int nID;
	String name;
	String BPC;
	String HMD;
	String UTT1;
	String UTT2;
	String UTT3;
	String UTT4;
	
	public int getnID()
	{
		return nID;
	}
	public void setnID(int nID)
	{
		this.nID = nID;
	}
	
	public String getName()
	{
		return name;
	}
	public void setName(String name)
	{
		this.name = name;
	}
	
	public String getBPC()
	{
		return ( BPC == null ) ? "":BPC;
	}
	public void setBPC(String bPC)
	{
		BPC = bPC;
	}
	
	public String getHMD()
	{
		return ( HMD == null ) ? "":HMD;
	}
	public void setHMD(String hMD)
	{
		HMD = hMD;
	}
	
	public String getUTT1()
	{
		return ( UTT1 == null ) ? "":UTT1;
	
	}
	public void setUTT1(String uTT1)
	{
		UTT1 = uTT1;
	}
	public String getUTT2()
	{
		return ( UTT2 == null ) ? "":UTT2;
		
	}
	public void setUTT2(String uTT2)
	{
		UTT2 = uTT2;
	}
	public String getUTT3()
	{
		return ( UTT3 == null ) ? "":UTT3;
		
	}
	public void setUTT3(String uTT3)
	{
		UTT3 = uTT3;
	}
	public String getUTT4()
	{
		return ( UTT4 == null ) ? "":UTT4;
		
	}
	public void setUTT4(String uTT4)
	{
		UTT4 = uTT4;
	}
	

	
	
}

