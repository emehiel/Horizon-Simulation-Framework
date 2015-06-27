using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class DependencyFunctionElement : HsfElement
    {
        public bool IsScripted;
        public HsfType Type;
        public string Key;
        public string CallKey;
        public bool Enabled;

        public DependencyFunctionElement()
        {
            Enabled = false;
            ElementType = "DEPENDENCY_FCN";
            IsScripted = false;
            Type = HsfType.Bool;
            Key = "unknown_key";
            CallKey = "unknown_call_key";
        }

        public override string ToXml()
        {
            // Serializes object to xml string
            string toReturn = "<" + ElementType + " scripted=\"";
            toReturn += IsScripted ? "True" : "False";
            toReturn += "\" type=\"" + Enum.GetName(typeof(HsfType), Type) + "\" key=\"" + Key + "\" callKey=\"" + CallKey + "\"/>";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes object from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT scripted, type, key, callKey FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load type enum
            Array posValues = Enum.GetValues(typeof(HsfType));
            foreach (HsfType v in posValues)
            {
                if (Enum.GetName(typeof(HsfType), v) == results[0]["type"].ToString())
                {
                    Type = v;
                }
            }

            // Load flag
            IsScripted = results[0]["scripted"].ToString() == "True" ? true : false;

            // Load strings
            Key = results[0]["key"].ToString();
            CallKey = results[0]["callKey"].ToString();
        }
    }
}
