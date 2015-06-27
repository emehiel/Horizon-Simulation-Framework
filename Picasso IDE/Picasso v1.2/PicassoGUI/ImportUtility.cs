using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Security.Cryptography;
using System.Windows.Forms;
using System.Xml;
using PicassoGUI.Components;

namespace PicassoGUI
{
    public class ImportUtility
    {
        private List<string> _importsEnabled;
        public List<string> ImportsEnabled
        {
            get { return _importsEnabled; }
        }

        public ImportUtility() {
            _importsEnabled = new List<string>();
            _importsEnabled.Add("POSC");
        }

        public AssetComponent ImportFromPosc(string e, string i, string o, string c)
        {
            /* Attempts to import an asset from POSC. Provided values are:
             *  e - email of POSC contributor account
             *  i - ip address of POSC contributor account (will eventually be generated automatically)
             *  o - search phrase for object
             *  c - id of contributor account
             *  f - format of desired output
             */

            // Hash api key
            string apiKey = hashMd5(e, i);

            // Assemble URL
            string url = "http://tythos.net/horizon/posc/poscQuery.php?apiKey=" + apiKey + "&obj=" + o + "&cont_id=" + c + "&format=xml";

            // Make http request
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream resStream = response.GetResponseStream();

            // Read response into string
            StringBuilder sb = new StringBuilder();
            byte[] buffer = new byte[8192];
            string tmpString = null;
            int count = 0;
            do
            {
                count = resStream.Read(buffer, 0, buffer.Length);
                if (count != 0)
                {
                    tmpString = Encoding.ASCII.GetString(buffer, 0, count);
                    sb.Append(tmpString);
                }
            } while (count > 0);

            // Check for error
            AssetComponent toReturn;
            string result = sb.ToString();
            Regex pattern = new Regex("^Error: ");
            if (pattern.IsMatch(result))
            {
                // Error has occured; abort with message
                MessageBox.Show("Unable to import asset from POSC:\n" + result);
                toReturn = new AssetComponent(o);
                return toReturn;
            }

            // Create new asset from xml and return
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(new StringReader(result));
            toReturn = new AssetComponent(xmlDoc);
            return toReturn;
        }

        private string hashMd5(string s1, string s2)
        {
            // Compute hash (byte array)
            ASCIIEncoding asciiEncoding = new ASCIIEncoding();
            byte[] data = asciiEncoding.GetBytes(s1+s2);
            MD5 md5Gen = new MD5CryptoServiceProvider();
            byte[] result = md5Gen.ComputeHash(data);

            // Convert result back to string
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < result.Length; i++)
            {
                sb.Append(result[i].ToString("X2"));
            }
            string toReturn = sb.ToString();
            string originalData = asciiEncoding.GetString(data);
            return toReturn;
        }
    }
}