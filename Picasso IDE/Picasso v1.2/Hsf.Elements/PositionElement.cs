using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class PositionElement : HsfElement
    {
        public PositionType PositionType;
        public HsfMatrix Ics;
        public EomsElement Eoms;

        public PositionElement()
        {
            ElementType = "POSITION";
            PositionType = new PositionType();
            Ics = new HsfMatrix(3, 1);
            Eoms = new EomsElement();
        }

        public override string ToXml()
        {
            // Converts this elements into an Xml-formatted string
            string toReturn = "<" + ElementType + " PositionType=\"" + Enum.GetName(typeof(PositionType), PositionType) + "\" ICs=\"" + Ics.ToString() + "\">";
            if (Eoms.Type != EomsType.None) { toReturn += Eoms.ToXml(); }
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Attempts to load a position from the given xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT PositionType, ICs, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load enums
            Array posValues = Enum.GetValues(typeof(PositionType));
            foreach (PositionType v in posValues)
            {
                if (Enum.GetName(typeof(PositionType), v) == results[0]["PositionType"].ToString())
                {
                    PositionType = v;
                }
            }

            // Load children
            Ics.FromString(results[0]["ICs"].ToString());

            // Check for EOMs specification
            if (results[0]["_innerXml"].ToString() != "")
            {
                Eoms.FromXml(results[0]["_innerXml"].ToString());
            }
            else
            {
                Eoms.Type = EomsType.None;
            }
        }
    }
}
