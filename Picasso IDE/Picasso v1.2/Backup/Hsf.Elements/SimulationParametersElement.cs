using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class SimulationParametersElement : HsfElement
    {
        public double SimStartJd;
        public double SimStart;
        public double SimEnd;

        public SimulationParametersElement()
        {
            ElementType = "SIMULATION_PARAMETERS";
            SimStartJd = 0.0;
            SimStart = 0.0;
            SimEnd = 1.0;
        }

        public override string ToXml()
        {
            // Writes this element to a string in Xml format
            string toReturn = "<" + ElementType + " SimStartJD=\"" + SimStartJd + "\" SimStart=\"" + SimStart + "\" SimEnd=\"" + SimEnd + "\"></" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Loads values of this element from given Xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT SimStartJD, SimStart, SimEnd FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            SimStartJd = Convert.ToDouble(results[0]["SimStartJD"]);
            SimStart = Convert.ToDouble(results[0]["SimStart"]);
            SimEnd = Convert.ToDouble(results[0]["SimEnd"]);
        }
    }
}
