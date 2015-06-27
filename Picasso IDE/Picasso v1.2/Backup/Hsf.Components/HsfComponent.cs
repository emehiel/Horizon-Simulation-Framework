using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Hsf.Elements;

namespace Hsf.Components
{
    public abstract class HsfComponent : HsfElement
    {
        public void ToFile(string filename)
        {
            // Write xml-formatted string to file
            FileStream file = new FileStream(filename, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(file);
            sw.Write(ToXml());
            sw.Close();
            file.Close();
        }

        public void FromFile(string filename)
        {
            // Loads component and child elements from given file
            FileStream file = new FileStream(filename, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(file);
            FromXml(sr.ReadToEnd());
            sr.Close();
            file.Close();
        }
    }

    class HsfComponentException : ApplicationException
    {
        // Thrown when a component method fails
        public HsfComponentException() { }
        public HsfComponentException(string mess) : base(mess) { }
        public HsfComponentException(string mess, Exception inn) : base(mess, inn) { }
        protected HsfComponentException(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext content) { }

        public string GetReport()
        {
            string toReturn = "An HsfComponent has failed: " + Message;
            return toReturn;
        }
    }
}
