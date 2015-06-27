using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using Hsf.Elements;
using Hsf.Utility;
using MementoModel;

namespace Hsf.Components
{
    public class ScenarioComponent : HsfComponent, IMCloneable
    {
        // Component information
        private string _scenarioName;
        public SimulationParametersElement SimParams;
        public SchedulerParametersElement SchParams;

        // Property accessors
        public string ScenarioName
        {
            get { return _scenarioName; }
            set { _scenarioName = value; }
        }

/*        [CategoryAttribute("Simulation Parameters")]
        public double SimStartJd
        {
            get { return SimParams.SimStartJd; }
            set { SimParams.SimStartJd = value; }
        }

        [CategoryAttribute("Simulation Parameters")]
        public double SimStart
        {
            get { return SimParams.SimStart; }
            set { SimParams.SimStart = value; }
        }

        [CategoryAttribute("Simulation Parameters")]
        public double SimEnd
        {
            get { return SimParams.SimEnd; }
            set { SimParams.SimEnd = value; }
        }

        [CategoryAttribute("Scheduler Parameters")]
        public double MaxSchedules
        {
            get { return SchParams.MaxSchedules; }
            set { SchParams.MaxSchedules = value; }
        }

        [CategoryAttribute("Scheduler Parameters")]
        public double CropTo
        {
            get { return SchParams.CropTo; }
            set { SchParams.CropTo = value; }
        }

        [CategoryAttribute("Scheduler Parameters")]
        public double SimStep
        {
            get { return SchParams.SimStep; }
            set { SchParams.SimStep = value; }
        } */

        // Child components
        public TargetdeckComponent Targetdeck;
        public EnvironmentComponent Environment;
        public ModelComponent Model;

        public ScenarioComponent()
        {
            ElementType = "SCENARIO";
            ScenarioName = "Unknown Scenario";
            SimParams = new SimulationParametersElement();
            SchParams = new SchedulerParametersElement();
            Targetdeck = new TargetdeckComponent();
            Environment = new EnvironmentComponent();
            Model = new ModelComponent();
        }

        public override string ToXml()
        {
            // Writes component and child elements to xml-formatted string
            string toReturn = "<" + ElementType + " scenarioName=\"" + ScenarioName + "\">";
            toReturn += SimParams.ToXml();
            toReturn += SchParams.ToXml();
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Loads component and child elements from raw string data
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT scenarioName, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfComponentException("Unable to parse " + ElementType + " from file");
            }

            // Get values and pass to child elements
            ScenarioName = results[0]["scenarioName"].ToString();
            SimParams.FromXml(results[0]["_innerXml"].ToString());
            SchParams.FromXml(results[0]["_innerXml"].ToString());
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            ScenarioComponent clone = new ScenarioComponent();
            clone.FromXml(ToXml());
            return clone;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            FromXml(((ScenarioComponent)value).ToXml());
        }
    }
}
