using System;
using System.Net.Json;
using System.Net.Sockets;
using System.Web.UI.WebControls;
using CFileLog;

namespace ARWeb.ARTS
{
    public partial class logmain : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            FileLog log = new FileLog();

            String param = Request.Form[0];
            String sIP = Request.UserHostAddress;

            JsonObjectCollection send = new JsonObjectCollection();

            String strProtocol = String.Empty;
            try
            {
                String packet = param.ToLower();

                //log.Log(sIP.ToString(), this.ToString(), "packet=" + packet);


                JsonParser json = new JsonParser();

                strProtocol = json.Parse(packet, "protocol");

                int Protocol = Convert.ToInt32(strProtocol);

                RecvLog RecvObj = Parse((enLogProtocol)Protocol);

                RecvObj.Recv(packet, Protocol);

                
            }
            catch (System.Exception ex)
            {
                log.Log(ex.ToString());
                send.Add(new JsonStringValue("Protocol", strProtocol));
                Response.BinaryWrite(SendPacket.Send((int)enWEBProtocol.webError, send.ToString()));
            }
        }
        public RecvLog Parse(enLogProtocol Protocol)
        {
            RecvLog packet;
            switch (Protocol)
            {
                case enLogProtocol.LOG_FIRE:            packet = new LOGFIRE();             break;
                case enLogProtocol.LOG_HIT:             packet = new LOGHIT();              break;
                case enLogProtocol.LOG_CHALLENGE:       packet = new LOGCHALLENGE();        break;
                case enLogProtocol.LOG_SURRENDER:       packet = new LOGSURRENDER();        break;
                case enLogProtocol.LOG_BIND:            packet = new LOGBIND();             break;
                case enLogProtocol.LOG_COMMANDMOVE:     packet = new LOGCOMMANDMOVE();      break;
                case enLogProtocol.LOG_BREIFSITUATION:  packet = new LOGBREIFSITUATION();   break;

                case enLogProtocol.LOG_CALL:            packet = new LOGCALL();             break;
                case enLogProtocol.LOG_BADFIRE:         packet = new LOGBADFIRE();          break;

                case enLogProtocol.LOG_TR_HMD:          packet = new LOGTRHMD();            break;
                case enLogProtocol.LOG_TR_GUN:          packet = new LOGTRGUN();            break;
                case enLogProtocol.LOG_DISTANCE:        packet = new LOGDISTANCE();         break;

                case enLogProtocol.LOG_MOTION:          packet = new LOGMOTION(); break;
                default: packet = new DefaultLog();break;

            }
            return packet;
            
        }
    }
}
