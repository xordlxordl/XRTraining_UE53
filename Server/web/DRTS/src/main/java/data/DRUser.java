package data;

import java.util.Calendar;

public class DRUser 
{
	private int uid;
    private String szname;		//이름
    
    private int nsex;
    private String szSex;
    
    private String birthdate;
    
    private int nsi;
    private int ngun;
    private int ndong;
    private String szaddr;
    
    private int bsick;
    private	String szsick;
    private int bdrug;
    private	String szdrug;
    

    
    private int  careeryear;
    private int  careermonth;

    private int  bdrive;
    private int  bcrash;
    private int  bcrashobject;
    private int  bcrashhuman;
    private int  bcrashonly;
    
    private String regidate;
 	
	private int result;

	
	public String GetCareer()
	{
		String strCareer = "없음";
		{
			if( careermonth == 0 )
				strCareer =  careeryear + "년 ";
			else
				strCareer =  careeryear + "년 " + careermonth + "개월";
		}
		
		return strCareer;
			
	}
    
	public int GetAge()
	{
		int age = 0;
		
		Calendar current = Calendar.getInstance();
		 int currentYear  = current.get(Calendar.YEAR);
         int currentMonth = current.get(Calendar.MONTH) + 1;
         int currentDay   = current.get(Calendar.DAY_OF_MONTH);
         
         Integer birthYear = Integer.parseInt( birthdate.substring( 0, 4 ) );
         Integer birthMonth = Integer.parseInt( birthdate.substring( 5, 7 ) );
         Integer birthDay = Integer.parseInt( birthdate.substring( 8,10 ) );
         age = currentYear - birthYear; 
         
         if (birthMonth * 100 + birthDay > currentMonth * 100 + currentDay) 
             age--;
         
		return age;
		
	}

	public int getUid()
	{
		return uid;
	}

	public void setUid(int uid)
	{
		this.uid = uid;
	}

	public String getSzname()
	{
		return szname;
	}

	public void setSzname(String szname)
	{
		this.szname = szname;
	}

	public int getNsex()
	{
		return nsex;
	}

	public void setNsex(int nsex)
	{
		this.nsex = nsex;
	}
	

	public String getSzSex()
	{
		return szSex;
	}

	public void setSzSex(String szSex)
	{
		this.szSex = szSex;
	}

	public String getBirthdate()
	{
		return birthdate;
	}

	public void setBirthdate(String birthdate)
	{
		this.birthdate = birthdate;
	}

	public int getNsi()
	{
		return nsi;
	}

	public void setNsi(int nsi)
	{
		this.nsi = nsi;
	}

	public int getNgun()
	{
		return ngun;
	}

	public void setNgun(int ngun)
	{
		this.ngun = ngun;
	}

	public int getNdong()
	{
		return ndong;
	}

	public void setNdong(int ndong)
	{
		this.ndong = ndong;
	}

	public String getSzaddr()
	{
		return szaddr;
	}

	public void setSzaddr(String szaddr)
	{
		this.szaddr = szaddr;
	}

	public int getBsick()
	{
		return bsick;
	}

	public void setBsick(int bsick)
	{
		this.bsick = bsick;
	}

	public String getSzsick()
	{
		return szsick;
	}

	public void setSzsick(String szsick)
	{
		this.szsick = szsick;
	}

	public int getBdrug()
	{
		return bdrug;
	}

	public void setBdrug(int bdrug)
	{
		this.bdrug = bdrug;
	}

	public String getSzdrug()
	{
		return szdrug;
	}

	public void setSzdrug(String szdrug)
	{
		this.szdrug = szdrug;
	}

	
	public int getCareeryear()
	{
		return careeryear;
	}

	public void setCareeryear(int careeryear)
	{
		this.careeryear = careeryear;
	}

	public int getCareermonth()
	{
		return careermonth;
	}

	public void setCareermonth(int careermonth)
	{
		this.careermonth = careermonth;
	}

	public int getBdrive()
	{
		return bdrive;
	}

	public void setBdrive(int bdrive)
	{
		this.bdrive = bdrive;
	}

	public int getBcrash()
	{
		return bcrash;
	}

	public void setBcrash(int bcrash)
	{
		this.bcrash = bcrash;
	}

	public int getBcrashobject()
	{
		return bcrashobject;
	}

	public void setBcrashobject(int bcrashobject)
	{
		this.bcrashobject = bcrashobject;
	}

	public int getBcrashhuman()
	{
		return bcrashhuman;
	}

	public void setBcrashhuman(int bcrashhuman)
	{
		this.bcrashhuman = bcrashhuman;
	}

	public int getBcrashonly()
	{
		return bcrashonly;
	}

	public void setBcrashonly(int bcrashonly)
	{
		this.bcrashonly = bcrashonly;
	}

	public String getRegidate()
	{
		return regidate;
	}

	public void setRegidate(String regidate)
	{
		this.regidate = regidate;
	}

	public int getResult()
	{
		return result;
	}

	public void setResult(int result)
	{
		this.result = result;
	}
	
	
}

