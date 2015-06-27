using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class LuaFileElement : HsfElement
    {
        public string Src;

        public LuaFileElement()
        {
            ElementType = "LUA_FILE";
        }

        public override string ToXml()
        {
            // Serializes element to xml string
            string toReturn = "<" + ElementType + " src=\"" + Src + "\"/>";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes element from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT src FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load src!
            Src = results[0]["src"].ToString();
        }
    }
}
