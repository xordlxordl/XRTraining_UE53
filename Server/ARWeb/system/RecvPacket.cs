using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;
using System.Data.Odbc;
using System.Text;


public abstract class RecvPacket
{
    public RecvPacket()
    {
    
    }

    //public abstract byte[] Recv( String str );
    public abstract byte[] Recv( String str, int protocol );

	public int CheckServer()
	{
		StringBuilder query = new StringBuilder();

		query.Append("EXEC spCheckServer");

		DB db = new DB();
        OdbcDataReader reader = db.Exec(query.ToString());

		while(reader.Read())
		{
			string Result = reader["Result"].ToString();
			reader.Close();
			db.Close();

			if(Result.Equals("1") == false)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

		return 0;
	}

    public int CheckOTP( String str )
    {
        JsonParser recv = new JsonParser();

        String OTP = recv.Parse(str, "OTP");
        String UID = recv.Parse(str, "UID");
        
        if( UID.Equals("0") )
        {
            return 1;
        }
        

        StringBuilder query = new StringBuilder();

        query.Append("CALL spCheckOTP ");
        query.Append(UID + ",");
        query.Append("'" + OTP + "'");

        DB db = new DB();
        OdbcDataReader reader = db.Exec(query.ToString());


        String strProtocol = recv.Parse( str, "protocol" );
        int Protocol = Convert.ToInt32( strProtocol );


        if( Protocol <= (int)enWEBProtocol.webLogin )
        {
            return 1;
        }
        else
        {
            while( reader.Read() )
            {
                string Result = reader["Result"].ToString();
                reader.Close();
                db.Close();

                if(Result.Equals("1") == false)
                {
                    if( Result.Equals("-1") == true ) 
                        return -1;
                    else
                        return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        return 0;
    }

}
