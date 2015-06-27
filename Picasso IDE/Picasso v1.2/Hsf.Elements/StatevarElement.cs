using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class StatevarElement : HsfElement
    {
        public HsfType Type;
        public string Key;

        public StatevarElement()
        {
            ElementType = "STATEVAR";
        }

        public override string ToXml()
        {
            // Serialize object to xml string
            string toReturn = "<" + ElementType + " type=\"" + Enum.GetName(typeof(HsfType), Type) + "\" key=\"" + Key + "\"/>";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserialize object from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT type, key, type FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load values
            Key = results[0]["key"].ToString();

            // Load type enum
            Array posValues = Enum.GetValues(typeof(HsfType));
            foreach (HsfType v in posValues)
            {
                if (Enum.GetName(typeof(HsfType), v) == results[0]["type"].ToString())
                {
                    Type = v;
                }
            }
        }
    }
}
