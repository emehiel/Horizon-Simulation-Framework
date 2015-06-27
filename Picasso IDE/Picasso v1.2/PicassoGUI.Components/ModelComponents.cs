/*  Brian Kirkpatrick
 *  Model component definition and parts
 *  Created 7/21/2009
 *  */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace PicassoGUI.Components
{
    public enum HorizonDataType
    {
        Double,
        Matrix,
        Bool
    }

    public enum HorizonConstraintType
    {
        FAIL_IF_HIGHER,
        FAIL_IF_LOWER
    }

    public enum HorizonScheduleType
    {
        TargetValueScheduleEvaluator,
        scripted
    }

    [XmlType("SCHEDULE_EVALUATOR")]
    public class ScheduleEvaluator : PicassoNode
    {
        [XmlAttribute("type")]
        public HorizonScheduleType Type;

        [XmlAttribute("evaluateLUAFcn")]
        public string LuaFunctionName;

        public ScheduleEvaluator()
        {
            Type = HorizonScheduleType.TargetValueScheduleEvaluator;
            LuaFunctionName = "";
        }
    }

    [XmlType("IC")]
    public class InitialCondition : PicassoNode
    {
        [XmlAttribute("type")]
        public HorizonDataType Ic;

        [XmlAttribute("key")]
        public string Key;

        [XmlAttribute("value")]
        public string Value;

        public InitialCondition()
        {
            Ic = HorizonDataType.Double;
            Key = "unknown";
            Value = "0.0";
        }
    }

    [XmlType("DEPENDENCY_FCN")]
    public class DependencyFunction : PicassoNode
    {
        [XmlAttribute("scripted")]
        public bool Scripted;

        [XmlAttribute("type")]
        public HorizonDataType Type;

        [XmlAttribute("key")]
        public string Key;

        [XmlAttribute("callKey")]
        public string CallKey;

        public DependencyFunction()
        {
            Scripted = false;
            Type = HorizonDataType.Double;
            Key = "unknown_dependency_function";
            CallKey = "unknown_dependency_callkey";
        }
    }

    [XmlType("DEPENDENCY")]
    public class Dependency : PicassoNode
    {
        [XmlAttribute("SubId")]
        public int SubId;

        public Dependency()
        {
            SubId = 0;
        }
    }

    [XmlType("SUBSYSTEM")]
    public class Subsystem : PicassoNode, IPicassoComponent
    {
        [XmlAttribute("Type")]
        public string Type;

        [XmlAttribute("SubId")]
        public int SubId;

        [XmlAttribute("subsystemName")]
        public string SubsystemName;

        [XmlAttribute("initLUAFcn")]
        public string InitialLuaFunction;

        [XmlAttribute("canPerformLUAFcn")]
        public string CanPerformLuaFunction;

        [XmlAttribute("canExtendLUAFcn")]
        public string CanExtendLuaFunction;

        public List<InitialCondition> InitialConditions;
        public List<Dependency> Dependencies;
        public DependencyFunction DependFcn;

        public Subsystem()
        {
            Type = "Unknown Subsystem Type";
            SubId = 0;
            SubsystemName = "Unknown Subsystem Name";
            InitialConditions = new List<InitialCondition>();
            Dependencies = new List<Dependency>();
            DependFcn = new DependencyFunction();

            InitialLuaFunction = "";
            CanPerformLuaFunction = "";
            CanExtendLuaFunction = "";
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
            /*ScenarioForm childForm = new ScenarioForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();*/
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For ScenarioComponent, adds Scenario to head nodes of tree [with subsequent child
            // nodes added recursively] First, check for node that already has this object.
            TreeNodeCollection rootNodes = targetTree.Nodes;
            foreach (TreeNode n in rootNodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode("Subsystem " + SubId.ToString() + " (" + Type.ToString() + ")");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);

            // No child nodes to recurse to
            return true;
        }
    }

    [XmlType("CONSTRAINT")]
    public class Constraint : PicassoNode, IPicassoComponent
    {
        [XmlAttribute("value")]
        public double Value;

        [XmlAttribute("subId")]
        public int SubId;

        [XmlAttribute("type")]
        public HorizonConstraintType Type;

        public Constraint()
        {
            Value = 0.0;
            SubId = 0;
            Type = HorizonConstraintType.FAIL_IF_HIGHER;
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
            /*ScenarioForm childForm = new ScenarioForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();*/
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For ScenarioComponent, adds Scenario to head nodes of tree [with subsequent child
            // nodes added recursively] First, check for node that already has this object.
            TreeNodeCollection rootNodes = targetTree.Nodes;
            foreach (TreeNode n in rootNodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode("Subsystem " + SubId.ToString() + " Constraint (" + Type.ToString() + ")");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);

            // No child nodes to recurse to
            return true;
        }
    }

    [XmlType("ASSET")]
    public class AssetComponent : PicassoNode, IPicassoComponent
    {
        [XmlAttribute("POSITION")]
        public Position AssetPosition;

        public List<Subsystem> Subsystems;
        public List<Constraint> Constraints;
        public string AssetName;

        public AssetComponent()
        {
            AssetPosition = new Position();
            Subsystems = new List<Subsystem>();
            Constraints = new List<Constraint>();
            AssetName = "Unnamed Asset";
        }

        public AssetComponent(string name)
        {
            AssetPosition = new Position();
            Subsystems = new List<Subsystem>();
            Constraints = new List<Constraint>();
            AssetName = name;
        }

        public AssetComponent(XmlDocument source)
        {
            // Create new asset component from xml source (imported)
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
            AssetForm childForm = new AssetForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For ScenarioComponent, adds Scenario to head nodes of tree [with subsequent child
            // nodes added recursively] First, check for node that already has this object.
            TreeNodeCollection rootNodes = targetTree.Nodes;
            foreach (TreeNode n in rootNodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode(AssetName);
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);

            // Recurse populateTree() on children (subsystems, constraints)
            foreach (Subsystem subsys in Subsystems)
            {
                subsys.PopulateTree(targetTree, newNode);
            }
            foreach (Constraint cons in Constraints)
            {
                cons.PopulateTree(targetTree, newNode);
            }

            return true;
        }
    }

    [XmlType("LUA_FILE")]
    public class LuaScript : PicassoNode
    {
        [XmlAttribute("src")]
        public string Src;

        public LuaScript(string source)
        {
            Src = source;
        }
    }

    [XmlType("LUA")]
    public class ScriptingSettings : PicassoNode
    {
        [XmlAttribute("enableScripting")]
        public bool EnableScripting;

        public List<LuaScript> Scripts;

        public ScriptingSettings()
        {
            EnableScripting = false;
            Scripts = new List<LuaScript>();
        }

        public void RemoveScript(string scriptName)
        {
            int i = 0;
            List<int> toRemove = new List<int>();
            foreach (LuaScript script in Scripts)
            {
                if (script.Src == scriptName)
                {
                    toRemove.Add(i);
                }
                i++;
            }
            foreach (int ndx in toRemove)
            {
                Scripts.RemoveRange(ndx, 1);
            }
        }
    }

    [XmlType("MODEL")]
    public class ModelComponent : PicassoNode, IPicassoComponent
    {
        [XmlElement("ENVIRONMENT")]
        public EnvironmentComponent hEnvironment; // Handle to environment component from Scenario

        [XmlElement("SCHEDULE_EVALUATOR")]
        public ScheduleEvaluator SchEvaluator;

        [XmlElement("LUA")]
        public ScriptingSettings LuaSettings;

        public List<AssetComponent> Assets;

        public ModelComponent()
        {
            hEnvironment = null;
            Assets = new List<AssetComponent>();
            LuaSettings = new ScriptingSettings();
            SchEvaluator = new ScheduleEvaluator();
        }

        public ModelComponent(EnvironmentComponent targetEnvironment)
        {
            hEnvironment = targetEnvironment;
            Assets = new List<AssetComponent>();
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
            ModelForm childForm = new ModelForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // For ScenarioComponent, adds Scenario to head nodes of tree [with subsequent child
            // nodes added recursively] First, check for node that already has this object.
            foreach (TreeNode n in headNode.Nodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to head node
            TreeNode newNode = new TreeNode("System Model");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // Recurse populateTree() on children (assets)
            foreach (AssetComponent asset in Assets)
            {
                asset.PopulateTree(targetTree, newNode);
            }
            return true;
        }
    }
}
