using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class ConstraintElement : HsfElement
    {
        public double Value;
        public Int64 SubId;
        public ConstraintType Type;
        public StatevarElement Statevar;

        public ConstraintElement()
        {
            ElementType = "CONSTRAINT";
            Value = 0.0;
            SubId = 0;
            Type = ConstraintType.IGNORE;
            Statevar = new StatevarElement();
        }

        public override string ToXml()
        {
            // Serialize object to xml string
            string toReturn = "<" + ElementType + " value=\"" + Value + "\" subId=\"" + SubId + "\"";
            toReturn += " type=\"" + Enum.GetName(typeof(ConstraintType), Type) + "\">";
            toReturn += Statevar.ToXml();
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserialize object from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT value, subId, type FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load values
            try
            {
                Value = Convert.ToDouble(results[0]["value"]);
            }
            catch (FormatException)
            {
                Value = 0.0;
            }
            try
            {
                SubId = Convert.ToInt64(results[0]["subId"]);
            }
            catch (FormatException)
            {
                SubId = 0;
            }

            // Load type enum
            Array posValues = Enum.GetValues(typeof(ConstraintType));
            foreach (ConstraintType v in posValues)
            {
                if (Enum.GetName(typeof(ConstraintType), v) == results[0]["type"].ToString())
                {
                    Type = v;
                }
            }

            // Load statevar
            results = parser.Query("SELECT _innerXml FROM " + ElementType);
            Statevar.FromXml(results[0]["_innerXml"].ToString());
        }
    }
}
