using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Hsf.Elements;
using Hsf.Utility;

namespace Hsf.Components
{
    public class Scenario : HsfComponent
    {
        public string ScenarioName;
        public SimulationParameters SimulationParameters;
        public SchedulerParameters SchedulerParameters;

        public Scenario()
        {
            ElementType = "SCENARIO";
            ScenarioName = "Unknown Scenario";
            SimulationParameters = new SimulationParameters();
            SchedulerParameters = new SchedulerParameters();
        }

        public override void ToFile(string filename)
        {
            // Writes component and child elements to given file
            string toWrite = "<" + ElementType + " scenarioName=\"" + ScenarioName + "\">";
            toWrite += SimulationParameters.ToXml();
            toWrite += SchedulerParameters.ToXml();
            toWrite += "</" + ElementType + ">";

            // Write string to file
            FileStream file = new FileStream(filename, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(file);
            sw.Write(toWrite);
            sw.Close();
            file.Close();
        }

        public override void FromFile(string filename)
        {
            // Loads component and child elements from given file
            XqlParser parser = new XqlParser(filename);
            List<Hashtable> results = parser.Query("SELECT scenarioName, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfComponentException("Unable to parse " + ElementType + " from file " + filename);
            }

            // Get values and pass to child elements
            ScenarioName = results[0]["scenarioName"].ToString();
            SimulationParameters.FromXml(results[0]["_innerXml"].ToString());
            SchedulerParameters.FromXml(results[0]["_innerXml"].ToString());
        }
    }
}
