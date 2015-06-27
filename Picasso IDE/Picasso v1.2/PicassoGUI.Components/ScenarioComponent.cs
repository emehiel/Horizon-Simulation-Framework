/*  Brian Kirkpatrick
 *  ScenarioComponent definition and parts
 *  Last updated 7/21/2009
 *  */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.XPath;
using MementoModel;

namespace PicassoGUI.Components
{
    [XmlType("SIMULATION_PARAMETERS"), Serializable]
    public class SimulationParameters : PicassoNode
    {
        private double _simStartJD;
        private double _simStart;
        private double _simEnd;

        [XmlAttribute("SimStartJD")]
        public double SimStartJD
        {
            get { return _simStartJD; }
            set { _simStartJD = value; }
        }

        [XmlAttribute("SimStart")]
        public double SimStart
        {
            get { return _simStart; }
            set { _simStart = value; }
        }

        [XmlAttribute("SimEnd")]
        public double SimEnd
        {
            get { return _simEnd; }
            set { _simEnd = value; }
        }

        public SimulationParameters()
        {
            _simStartJD = 0.0;
            _simStart = 0.0;
            _simEnd = 1.0;
        }
    }

    [XmlType("SCHEDULER_PARAMETERS")]
    public class SchedulerParameters : PicassoNode
    {
        private Int32 _maxSchedules;
        private Int32 _cropTo;
        private double _simStep;

        [XmlAttribute("MaxSchedules")]
        public Int32 MaxSchedules
        {
            get { return _maxSchedules; }
            set { _maxSchedules = value; }
        }

        [XmlAttribute("CropTo")]
        public Int32 CropTo
        {
            get { return _cropTo; }
            set { _cropTo = value; }
        }

        [XmlAttribute("SimStep")]
        public double SimStep
        {
            get { return _simStep; }
            set { _simStep = value; }
        }

        public SchedulerParameters()
        {
            _maxSchedules = 1;
            _cropTo = 1;
            _simStep = 0.1;
        }
    }

    [DefaultPropertyAttribute("ScenarioName"), XmlType("SCENARIO")]
    public class ScenarioComponent : PicassoNode, IPicassoComponent, IMCloneable
    {
        private String _scenarioName;
        [XmlAttribute("scenarioName")]
        public String ScenarioName
        {
            get { return _scenarioName; }
            set { _scenarioName = value; }
        }

        [XmlElement("SIMULATION_PARAMETERS")]
        public SimulationParameters SimParams;

        [XmlElement("SCHEDULING_PARAMETERS")]
        public SchedulerParameters SchParams;

        [XmlIgnore]
        public TargetdeckComponent TargetDeck;

        [XmlIgnore]
        public EnvironmentComponent Environment;

        [XmlIgnore]
        public ModelComponent Model;

        [XmlIgnore]
        public ResultsComponent Results;

        public ScenarioComponent()
        {
            _scenarioName = "Unknown Scenario";
            SimParams = new SimulationParameters();
            SchParams = new SchedulerParameters();
            TargetDeck = new TargetdeckComponent();
            Environment = new EnvironmentComponent();
            Model = new ModelComponent();
            Results = new ResultsComponent();
        }

        public ScenarioComponent(string name)
        {
            _scenarioName = name;
            SimParams = new SimulationParameters();
            SchParams = new SchedulerParameters();
            TargetDeck = new TargetdeckComponent();
            Environment = new EnvironmentComponent();
            Model = new ModelComponent();
            Results = new ResultsComponent();
        }

        public bool FillProperties(PropertyGrid targetGrid)
        {
            // Fills given property grid
            targetGrid.SelectedObject = this;
            return true;
        }

        public bool FillWindow(Form parentWindow)
        {
            // Fills given (floating) window with appropriate content
            ScenarioForm childForm = new ScenarioForm(this, parentWindow);
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For ScenarioComponent, adds Scenario to head nodes of tree [with subsequent child
            // nodes added recursively] First, check for node that already has this object.
            TreeNodeCollection rootNodes = targetTree.Nodes;
            TreeNode currNode = null;
            foreach (TreeNode n in rootNodes)
            {
                if (n.Tag == this)
                {
                    currNode = n;
                }
            }

            if (currNode == null)
            {
                // Create new node and add to root of tree
                currNode = new TreeNode(ScenarioName);
                currNode.Tag = this;
                targetTree.Nodes.Add(currNode);
                hNode = currNode;
            }

            // Recurse populateTree() on children (model, environment, targetDeck, results) with this tree, and newNode
            Model.PopulateTree(targetTree, currNode);
            Environment.PopulateTree(targetTree, currNode);
            TargetDeck.PopulateTree(targetTree, currNode);
            Results.PopulateTree(targetTree, currNode);

            return true;
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            ScenarioComponent copy = new ScenarioComponent();
            copy._scenarioName = _scenarioName;
            copy.SimParams.SimEnd = SimParams.SimEnd;
            copy.SimParams.SimStart = SimParams.SimStart;
            copy.SimParams.SimStartJD = SimParams.SimStartJD;
            copy.SchParams = SchParams;
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            if (value.GetType() == typeof(ScenarioComponent))
            {
                ScenarioComponent v = (ScenarioComponent)value;
                _scenarioName = v._scenarioName;
                SimParams.SimEnd = v.SimParams.SimEnd;
                SimParams.SimStart = v.SimParams.SimStart;
                SimParams.SimStartJD = v.SimParams.SimStartJD;
                SchParams = v.SchParams;
            }
        }

        public override bool Deserialize(string filename)
        {
            // Attempts to deserialize this object's values from given file
            StreamReader sr = new StreamReader(filename);
            string content = sr.ReadToEnd();
            sr.Close();
            Match match;
            string value;

            // Get scenario name
            match = Regex.Match(content, "scenarioName=\"([A-Za-z0-9]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                _scenarioName = value;
            }

            // Get sim start jd
            match = Regex.Match(content, "SimStartJD=\"([0-9\\.]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SimParams.SimStartJD = Convert.ToDouble(value);
            }

            // Get sim start
            match = Regex.Match(content, "SimStart=\"([0-9\\.]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SimParams.SimStart = Convert.ToDouble(value);
            }

            // Get sim end
            match = Regex.Match(content, "SimEnd=\"([0-9\\.]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SimParams.SimEnd = Convert.ToDouble(value);
            }

            // Get max schedules
            match = Regex.Match(content, "MaxSchedules=\"([0-9]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SchParams.MaxSchedules = (int)(Convert.ToDouble(value));
            }

            // Get crop to
            match = Regex.Match(content, "CropTo=\"([0-9\\.]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SchParams.CropTo = (int)(Convert.ToDouble(value));
            }

            // Get sim step
            match = Regex.Match(content, "SimStep=\"([0-9\\.]+)\"");
            if (match.Success)
            {
                value = match.Groups[1].Value;
                SchParams.SimStep = Convert.ToDouble(value);
            }
            return true;
        }
    }
}
