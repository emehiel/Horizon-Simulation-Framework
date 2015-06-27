/*  Brian Kirkpatrick
 *  Environment Component for Picasso / Horizon
 *  Added 7/18/2009
 *  */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.ComponentModel;
using System.Xml.Serialization;

namespace PicassoGUI.Components
{
    [XmlType("SUN")]
    public class SunEnvironment : PicassoNode
    {
        [XmlElement("isSunVecConstant")]
        public bool IsSunVecConstant;

        public SunEnvironment()
        {
            IsSunVecConstant = true;
        }
    }

    [XmlType("ENVIRONMENT")]
    public class EnvironmentComponent: PicassoNode, IPicassoComponent
    {
        public SunEnvironment Sun;

        public EnvironmentComponent()
        {
            Sun = new SunEnvironment();
        }

        public override bool Deserialize(string filename)
        {
            XmlDocument xDoc = new XmlDocument();
            xDoc.Load(filename);
            XmlNodeList nodes = xDoc.GetElementsByTagName("ENVIRONMENT");
            Sun.IsSunVecConstant = ((XmlElement)(nodes[0])).GetAttribute("isSunVecConstant") == "True" ? true : false;
            return true;
        }

        public bool FillProperties(PropertyGrid targetGrid)
        {
            // Fills given property grid (no properties for enviornment, yet)
            targetGrid.SelectedObject = this;
            return true;
        }

        public bool FillWindow(Form parentWindow)
        {
            // Fills given (floating) window with appropriate content
            EnvironmentForm childForm = new EnvironmentForm(this);
            childForm.MdiParent = parentWindow;
            childForm.Show();
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children.
            // No child nodes visible for environment components, but avoid duplication of node.
            foreach (TreeNode n in headNode.Nodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode("Environment");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // Recursion not necessary
            return true;
        }
    }
}
