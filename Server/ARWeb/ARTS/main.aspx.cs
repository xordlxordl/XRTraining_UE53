using System;
using System.Net.Json;
using System.Web.UI.WebControls;
using CFileLog;

namespace ARWeb.ARTS
{
    public partial class main : System.Web.UI.Page
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

                log.Log(sIP.ToString(), this.ToString(), "packet=" + packet);


                JsonParser json = new JsonParser();

                strProtocol = json.Parse(packet, "protocol");
                String strOTP = json.Parse(packet, "otp");

                int Protocol = Convert.ToInt32(strProtocol);

                RecvPacket RecvObj = Parse((enWEBProtocol)Protocol);

                byte[] SendObj = RecvObj.Recv(packet, Protocol);

                Response.BinaryWrite(SendObj);
            }
            catch (System.Exception ex)
            {
                log.Log(ex.ToString());
                send.Add(new JsonStringValue("Protocol", strProtocol));
                Response.BinaryWrite(SendPacket.Send((int)enWEBProtocol.webError, send.ToString()));
            }
        }
        public RecvPacket Parse(enWEBProtocol Protocol)
        {
            RecvPacket packet;
            switch (Protocol)
            {
             
                case enWEBProtocol.webGetSCTList:       packet = new GetSCTList();      break;
                case enWEBProtocol.webGetMCTList:       packet = new GetMCTList();      break;
                case enWEBProtocol.webGetWCTList:       packet = new GetWCTList();      break;
                case enWEBProtocol.webGetTCTList:       packet = new GetTCTList();      break;
                case enWEBProtocol.webGetFBXList:       packet = new GetFBXList();      break;
                case enWEBProtocol.webGetPNGList:       packet = new GetPNGList();      break;
                case enWEBProtocol.webGetWAVList:       packet = new GetWAVList();      break;
                case enWEBProtocol.webGetFBXAnimList:   packet = new GetFBXAnimList();  break;
                case enWEBProtocol.webGetEffList:       packet = new GetEffList();      break;

                case enWEBProtocol.webGetAllFileList:   packet = new GetAllFileList();  break;

                case enWEBProtocol.webLogin:            packet = new Login();           break;

                case enWEBProtocol.webGetMachineList:   packet = new GetMachineList();  break;
                case enWEBProtocol.webGetUserList:      packet = new GetUserList();     break;
                case enWEBProtocol.webStart:            packet = new Start();           break;
                case enWEBProtocol.webFinish:           packet = new Result();          break;
                case enWEBProtocol.webEnterUser:        packet = new EnterUser();       break;
                case enWEBProtocol.webTrainingUserList: packet = new TrainingUserList();        break;
                case enWEBProtocol.webTrainingDetail:   packet = new TrainingDetail();  break;
                case enWEBProtocol.webGetMotionLog:     packet = new GetMotionLog();    break;

                case enWEBProtocol.webGetVoiceList:     packet = new GetVoiceList();    break;
                case enWEBProtocol.webGetObjectLog:     packet = new GetObjectLog();    break;
                case enWEBProtocol.webCheckFile:        packet = new CheckFile();       break;

                default: packet = new Default(); break;
            }
            return packet;
        }
    }
}
