using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Security.Cryptography;
using System.Xml;
using Zeptomoby.OrbitTools;

namespace Hsf.Utility
{
    public class ImportUtility
    {
        private string _celesUrl;
        private string _poscUrl;
        private string _prevCategory;
        private List<string> _importsEnabled;
        public List<string> ImportsEnabled
        {
            get { return _importsEnabled; }
        }

        public ImportUtility() {
            _importsEnabled = new List<string>();
            _importsEnabled.Add("POSC");
            _celesUrl = "http://celestrak.com/NORAD/elements/";
            _poscUrl = "http://tythos.net/horizon/posc/";
        }

        public List<string> SearchCeles(string category, string satellite)
        {
            // Returns a list of all matching satellite names retrieved from given category
            List<string> toReturn = new List<string>();
            string url = _celesUrl + category.ToLower() + ".txt";
            _prevCategory = category;

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
            string result = sb.ToString();

            // Check for valid matches
            MatchCollection matches = Regex.Matches(result, "^([A-Za-z0-9\\s_\\(\\)-]*" + satellite + "[A-Za-z0-9\\s_\\(\\)-]*)$", RegexOptions.Multiline | RegexOptions.IgnoreCase);
            foreach (Match match in matches)
            {
                toReturn.Add(match.Groups[1].ToString().Trim());
            }
            return toReturn;
        }

        public List<string> SearchPosc(string email, string search)
        {
            // For now, set IP at 66.171.219.148
            string ip = "66.171.219.148";
            List<string> toReturn = new List<string>();

            // Create api key from hash of email and ip
            string apiKey = hashMd5(email, ip);

            // Create url from email, api key, and object search
            string url = _poscUrl + "poscSearch.php?email=" + email + "&apiKey=" + apiKey + "&obj=" + search;

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
            string result = sb.ToString();
            Regex pattern = new Regex("^Error: ");
            if (pattern.IsMatch(result))
            {
                // Error has occured; abort with message
                //MessageBox.Show("Unable to import asset from POSC:\n" + result);
                //toReturn = new AssetComponent(o);
                return toReturn;
            }

            // Parse and return results
            string[] splitter = {","};
            string[] names = result.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            foreach (string name in names)
            {
                toReturn.Add(name);
            }
            return toReturn;
        }

        public Hashtable ImportFromCeles(string name)
        {
            // Imports (and returns as hashtable) specific entry (by name) from Celestrak, using the most recent library
            Hashtable toReturn = new Hashtable();
            string url = _celesUrl + _prevCategory.ToLower() + ".txt";

            // Make http request
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream resStream = response.GetResponseStream();

            // Find matching line and read next two lines (TLEs)
            StreamReader sr = new StreamReader(resStream);
            bool found = false; string line = "";
            while (!found)
            {
                line = sr.ReadLine();
                if (line.Trim() == name)
                {
                    found = true;
                }
            }
            string tle1 = sr.ReadLine();
            string tle2 = sr.ReadLine();

            // Convert TLEs to COEs
            Tle tle = new Tle(name, tle1.Trim(), tle2.Trim());
            Orbit orbit = new Orbit(tle);
            double timePastEpoch = 0;
            Eci eci = orbit.getPosition(timePastEpoch);
            toReturn.Add("Name", name + " (" + tle1.Substring(2, 4) + ")");
            toReturn.Add("Position", "Matrix(6,1,{" + eci.Position.X + ", " + eci.Position.Y + ", " + eci.Position.Z + ", " + eci.Velocity.X + ", " + eci.Velocity.Y + ", " + eci.Velocity.Z + "})");
            return toReturn;
        }

        public Hashtable ImportFromPosc(string name, string email)
        {
            // Imports (and returns as hashtable) specific entry (by name) from Posc
            Hashtable toReturn = new Hashtable();
            string ip = "66.171.219.148";

            // Create api key from hash of email and ip
            string apiKey = hashMd5(email, ip);

            // Create url from email, api key, and object search
            string url = _poscUrl + "poscImport.php?email=" + email + "&apiKey=" + apiKey + "&obj=" + name;

            // Make http request
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream resStream = response.GetResponseStream();

            // Read keys and values from lines
            StreamReader sr = new StreamReader(resStream);
            string keyString = sr.ReadLine().Replace("<br/>", "");
            string valueString = sr.ReadLine().Replace("<br/>", "");

            // Parse keys and values into hash
            string[] keys = keyString.Split(',');
            string[] values = valueString.Split(',');
            int ndx = 0; Hashtable results = new Hashtable();
            foreach (string key in keys)
            {
                results.Add(key, values[ndx]);
                ndx++;
            }
            toReturn.Add("Name", name);
            toReturn.Add("Position", "Matrix(6,1,{" + results["posX"] + "," + results["posY"] + "," + results["posZ"] + "," + results["velX"] + "," + results["velY"] + "," + results["velZ"] + "})");
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