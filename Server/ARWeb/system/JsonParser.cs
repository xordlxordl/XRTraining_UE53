using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Json;

public class JsonParser
{
    public JsonParser()
    {
    
    }
    public string Parse( string str, string key )
    {
        int i = str.IndexOf( key );
        string ResultStr = "0";
        if( i < 0 )
            return ResultStr;

        JsonTextParser parser = new JsonTextParser();
        JsonObject obj = parser.Parse( str );
        JsonObjectCollection col = ( JsonObjectCollection )obj;
        if( col[ key ] == null )
            return ResultStr;

        String value = (String)col[ key ].GetValue();
        if(  value.Length <= 0 )
            return ResultStr; 

        return (String)col[ key ].GetValue();
    }

    public string GetString(string str, string key)
    {
        

        JsonTextParser parser = new JsonTextParser();
        JsonObject obj = parser.Parse(str);
        JsonObjectCollection col = (JsonObjectCollection)obj;

        return (String)col[key].GetValue();
    }
    public JsonArrayCollection GetArray( string str, string key )
    {
        JsonTextParser parser = new JsonTextParser();
        JsonObject obj = parser.Parse(str);
        JsonObjectCollection col = (JsonObjectCollection)obj;

        return (JsonArrayCollection)col[key];
    }
    public string GetInt(string str, string key)
    {
        int i = str.IndexOf(key);


        JsonTextParser parser = new JsonTextParser();
        JsonObject obj = parser.Parse(str);

        JsonObjectCollection col = (JsonObjectCollection)obj;


        object objdf = col[key].GetValue();
        string asdf = objdf.ToString();


        return asdf;


    }
}
