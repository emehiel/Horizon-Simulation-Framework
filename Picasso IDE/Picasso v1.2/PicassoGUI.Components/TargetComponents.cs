using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.ComponentModel;
using System.Xml.Serialization;
using MementoModel;

namespace PicassoGUI.Components
{
    public enum TargetType { FacilityTarget, LocationTarget };
    public enum TaskType { CommTask, ImagingTask };

    [XmlType("TARGET")]
    public class TargetComponent : PicassoNode, IPicassoComponent, IMCloneable
    {
        private String _targetName;
        private TargetType _type;
        private TaskType _task;
        private Int32 _maxTimes;
        private Int32 _value;

        [XmlElement("POSITION")]
        public Position TargetPosition;

        [XmlAttribute("TargetName")]
        public String TargetName
        {
            get { return _targetName; }
            set { _targetName = value; }
        }

        [XmlAttribute("TargetType")]
        public TargetType Type
        {
            get { return _type; }
            set { _type = value; }
        }

        [XmlAttribute("TaskType")]
        public TaskType Task
        {
            get { return _task; }
            set { _task = value; }
        }

        [XmlAttribute("MaxTimes")]
        public Int32 MaxTimes
        {
            get { return _maxTimes; }
            set { _maxTimes = value; }
        }

        [XmlAttribute("Value")]
        public Int32 Value
        {
            get { return _value; }
            set { _value = value; }
        }

        public TargetComponent()
        {
            _targetName = "Unknown Target";
            _type = TargetType.FacilityTarget;
            _task = TaskType.CommTask;
            _maxTimes = 1000;
            _value = 1;
            TargetPosition = new Position();
        }

        public TargetComponent(double latitude, double longitude)
        {
            _targetName = "Unknown Target";
            _type = TargetType.FacilityTarget;
            _task = TaskType.CommTask;
            _maxTimes = 1000;
            _value = 1;
            TargetPosition = new Position(latitude, longitude);
        }

        public TargetComponent(XmlElement el)
        {
            // Attempts to initialize a target from the given xml element
            _targetName = el.GetAttribute("TargetName");
            _type = el.GetAttribute("TargetType") == "LocationTarget" ? TargetType.LocationTarget : TargetType.FacilityTarget;
            _task = el.GetAttribute("TaskType") == "ImagingTask" ? TaskType.ImagingTask : TaskType.CommTask;
            _maxTimes = Convert.ToInt32(el.GetAttribute("MaxTimes"));
            _value = Convert.ToInt32(el.GetAttribute("Value"));
            XmlNodeList nodes = el.GetElementsByTagName("POSITION");
            TargetPosition = new Position((XmlElement)nodes[0]);
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
            TargetForm childForm = new TargetForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For TargetComponent, headNode is TargetdeckComponent node; create new node with
            // no children. First, check for node that already has this object
            TreeNodeCollection siblingNodes = headNode.Nodes;
            foreach (TreeNode n in siblingNodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to given Scenario node
            TreeNode newNode = new TreeNode(TargetName);
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // No child components for recursion
            return true;
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            TargetComponent copy = new TargetComponent();
            copy._maxTimes = _maxTimes;
            copy._targetName = _targetName;
            copy._task = _task;
            copy._type = _type;
            copy._value = _value;
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            if (value.GetType() == typeof(TargetComponent))
            {
                TargetComponent v = (TargetComponent)value;
                _maxTimes = v._maxTimes;
                _targetName = v._targetName;
                _task = v._task;
                _type = v._type;
                _value = v._value;
                TargetPosition = (Position)(v.TargetPosition.Clone());
            }
        }
    }

    [XmlType("TARGETDECK"), Serializable]
    public class TargetdeckComponent : PicassoNode, IPicassoComponent, IMCloneable
    {
        public List<TargetComponent> Targets;

        public TargetdeckComponent()
        {
            Targets = new List<TargetComponent>();
        }

        public override bool Deserialize(string filename)
        {
            // Clears targetdeck, then imports all targets from given file
            Targets.Clear();
            XmlDocument xDoc = new XmlDocument();
            xDoc.Load(filename);
            XmlNodeList targets = xDoc.GetElementsByTagName("TARGET");
            foreach (XmlNode node in targets)
            {
                TargetComponent newTarget = new TargetComponent((XmlElement)node);
                Targets.Add(newTarget);
            }
            return true;
        }

        public bool FillProperties(PropertyGrid targetGrid)
        {
            // Fills given property grid. For TargetDeck, however, switch to table.
            targetGrid.SelectedObject = this;
            return true;
        }

        public bool FillWindow(Form parentWindow)
        {
            // Fills given (floating) window with appropriate content
            TargetdeckForm childForm = new TargetdeckForm(this, parentWindow);
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For TargetdeckComponent, headNode is ScenarioComponent node; create new node and add children (targets) underneath
            // First, check for node that already has this object
            MessageBox.Show("Attempting to populate tree with targetdeck node from " + headNode.Tag.ToString());
            TreeNodeCollection siblingNodes = headNode.Nodes;
            foreach (TreeNode n in siblingNodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to given Scenario node
            TreeNode newNode = new TreeNode("Target Deck");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // Recurse populateTree() on children (all targets)
            foreach (TargetComponent thisTarget in Targets)
            {
                thisTarget.PopulateTree(targetTree, newNode);
            }
            return true;
        }

        public void AddTarget(TargetComponent newTarget)
        {
            string targetName = "Target " + (Targets.Count + 1).ToString();
            newTarget.TargetName = targetName;
            Targets.Add(newTarget);
            TreeNode newTargetNode = new TreeNode(targetName);
            newTargetNode.Tag = newTarget;
            if (hNode != null) hNode.Nodes.Add(newTargetNode);
        }

        public void AddRandomTarget(TargetType type)
        {
            Int32 seed = Convert.ToInt32(DateTime.Now.Ticks % 0x7fffffff);
            Random generator = new Random(seed);
            double longitude = generator.NextDouble() * 360.0 - 180.0;
            double latitude = generator.NextDouble() * 180.0 - 90.0;
            TargetComponent newTarget = new TargetComponent(latitude, longitude);
            newTarget.Type = type;
            AddTarget(newTarget);
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            TargetdeckComponent copy = new TargetdeckComponent();
            foreach (TargetComponent t in Targets)
            {
                copy.AddTarget(t);
            }
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            if (value.GetType() == typeof(TargetdeckComponent))
            {
                TargetdeckComponent v = (TargetdeckComponent)value;
                Targets.Clear();
                foreach (TargetComponent t in v.Targets)
                {
                    AddTarget(t);
                }
            }
        }
    }
}