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
    public class ProjectedMapComponent : PicassoNode, IPicassoComponent
    {
        // Encapsulates logic for projected map
        public ProjectedMapComponent()
        {
        }

        public bool FillProperties(PropertyGrid targetGrid)
        {
            // Fills given property grid
            return true;
        }

        public bool FillWindow(Form parentWindow)
        {
            // Fills given (floating) window with appropriate content
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // No child nodes for projected view components, but avoid duplication of node.
            foreach (TreeNode n in headNode.Nodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode("Projected Map");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // No children = no recursion
            return true;
        }
    }

    public class ResultsComponent : PicassoNode, IPicassoComponent
    {
        // Encapsulates the results from a Horizon use case. NOT an actual IPicassoComponent!
        private string _source;
        private bool _isEnabled;

        // View components
        public ProjectedMapComponent projectedView;

        public ResultsComponent()
        {
            _source = "";
            _isEnabled = false;
            projectedView = new ProjectedMapComponent();
        }

        public ResultsComponent(string src)
        {
            _source = src;
            _isEnabled = false;
        }

        public void Disable()
        {
            // Set status, disable nodes, hide windows
            _isEnabled = false;
            if (hNode != null)
            {
                hNode.ForeColor = System.Drawing.Color.LightGray;
                hNode.Nodes.Clear();
            }
        }

        public void Enable()
        {
            // Set status, enable nodes and window links (?)
            TreeView blankView = new TreeView();
            _isEnabled = true;
            hNode.ForeColor = System.Drawing.Color.Black;
            projectedView.PopulateTree(blankView, hNode);
            hNode.Expand();
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
            if (_isEnabled)
            {
                // No window for general results?
            }
            else
            {
                MessageBox.Show("Results have not yet been calculated for this scenario.");
            }
            return true;
        }

        public bool PopulateTree(TreeView targetTree, TreeNode headNode)
        {
            // Populates tree view with name and children
            // No child nodes initially visible for results components, but avoid duplication of node.
            foreach (TreeNode n in headNode.Nodes)
            {
                if (n.Tag == this) return false;
            }

            // Create new node and add to root of tree
            TreeNode newNode = new TreeNode("Results");
            newNode.Tag = this;
            headNode.Nodes.Add(newNode);
            hNode = newNode;

            // Recurse to children
            if (_isEnabled)
            {
                projectedView.PopulateTree(targetTree, newNode);
            }
            else
            {
                newNode.ForeColor = System.Drawing.Color.LightGray;
            }

            return true;
        }
    }
}
