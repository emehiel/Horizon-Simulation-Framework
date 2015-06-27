using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class SchedulerParametersElement : HsfElement
    {
        public double MaxSchedules;
        public double CropTo;
        public double SimStep;

        public SchedulerParametersElement()
        {
            ElementType = "SCHEDULER_PARAMETERS";
            MaxSchedules = 1;
            CropTo = 1;
            SimStep = 10.0;
        }

        public override string ToXml()
        {
            // Writes this element to a string in Xml format
            string toReturn = "<" + ElementType + " MaxSchedules=\"" + MaxSchedules + "\" NumCrop=\"" + CropTo + "\" SimStep=\"" + SimStep + "\"></" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Loads values of this element from given Xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT MaxSchedules, NumCrop, SimStep FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            MaxSchedules = Convert.ToDouble(results[0]["MaxSchedules"]);
            CropTo = Convert.ToDouble(results[0]["NumCrop"]);
            SimStep = Convert.ToDouble(results[0]["SimStep"]);
        }
    }
}
