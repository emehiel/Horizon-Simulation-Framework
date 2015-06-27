using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class IcElement : HsfElement
    {
        public HsfType Type;
        public string Key;
        public string Value;

        public IcElement()
        {
            ElementType = "IC";
            Type = HsfType.Bool;
            Key = "unnamed_variable";
            Value = "0.0";
        }

        public override string ToXml()
        {
            // Serializes initial condition to xml string
            string toReturn = "<" + ElementType + " type=\"" + Enum.GetName(typeof(HsfType), Type) + "\" key=\"" + Key + "\" value=\"" + Value + "\"/>";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes initial condition from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT type, key, value FROM " + ElementType);
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

            // Load values
            Key = results[0]["key"].ToString();
            Value = results[0]["key"].ToString();
        }
    }
}
