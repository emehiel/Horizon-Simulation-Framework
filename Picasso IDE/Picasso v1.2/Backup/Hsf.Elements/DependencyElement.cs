using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class DependencyElement : HsfElement
    {
        public Int64 SubId;

        public DependencyElement()
        {
            ElementType = "DEPENDENCY";
            SubId = 0;
        }

        public override string ToXml()
        {
            // Serializes element to xml string
            string toReturn = "<" + ElementType + " SubId=\"" + SubId + "\"/>";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes element from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT SubId FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Grab sub id
            SubId = Convert.ToInt64(results[0]["SubId"]);
        }
    }
}
