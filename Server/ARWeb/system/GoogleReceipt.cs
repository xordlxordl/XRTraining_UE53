using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using PublicKeyConvert;
using System.Security.Cryptography;

namespace GoogleReceipt
{
    public class GoogleVerify
    {
        public static bool Verify(string message, string base64Signature, string xmlPublicKey)
        {
            
            RSACryptoServiceProvider provider = new RSACryptoServiceProvider();
            provider.FromXmlString( xmlPublicKey );

            
            byte[] signature = System.Convert.FromBase64String( base64Signature );
            SHA1Managed sha = new SHA1Managed();
            byte[] data = System.Text.Encoding.UTF8.GetBytes( message );

            return provider.VerifyData( data, CryptoConfig.MapNameToOID("SHA1"), signature );
        }
    }
}