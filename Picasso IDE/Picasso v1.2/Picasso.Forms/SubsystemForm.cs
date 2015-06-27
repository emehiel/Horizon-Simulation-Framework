using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using MementoModel;
using Hsf.Elements;
using Hsf.Scripting;
using Picasso.Controls;
using Picasso.Dialogs;

namespace Picasso.Forms
{
    public partial class SubsystemForm : PicassoSubform
    {
        public SubsystemElement Subsystem;
        public SubsystemScript Script;

        public TreeNode SubsystemNode
        {
            get
            {
                TreeNodeCollection rootNodes = subsystemTreeView.Nodes;
                return rootNodes[0];
            }
        }

        public TreeNode FunctionNode
        {
            get { return SubsystemNode.Nodes[0]; }
        }

        public TreeNode StatesNode
        {
            get { return SubsystemNode.Nodes[1]; }
        }

        public TreeNode ParametersNode
        {
            get { return SubsystemNode.Nodes[2]; }
        }

        public SubsystemForm(SubsystemElement subsystem, Form parentForm, TreeNode node)
        {
            MdiParent = parentForm;
            node.Tag = this;
            this.Subsystem = subsystem;
            Script = new SubsystemScript();
            InitializeComponent();
            RefreshValues();
        }

        public override HsfElement Element
        {
            get
            {
                return Subsystem;
            }
        }

        public override void RefreshValues()
        {
            // Refresh abstracted-script content from subsystem element
            Script.LoadFromElements(Subsystem);

            // Reset grid and textbox
            propertyGrid.SelectedObject = null;

            // Populate function nodes
            FunctionNode.Nodes.Clear();
            foreach (ScriptedFunction func in Script.Functions)
            {
                TreeNode newNode = new TreeNode(func.Header);
                newNode.Tag = func;
                if (func.IsRequired) { newNode.ForeColor = Color.Purple; }
                FunctionNode.Nodes.Add(newNode);
            }

            // Populate state variable nodes
            StatesNode.Nodes.Clear();
            foreach (StateVariable state in Script.States)
            {
                TreeNode newNode = new TreeNode(state.Key);
                newNode.Tag = state;
                StatesNode.Nodes.Add(newNode);
            }

            // Populate parameter nodes
            ParametersNode.Nodes.Clear();
            foreach (SubsystemParameter param in Script.Parameters)
            {
                TreeNode newNode = new TreeNode(param.Name);
                newNode.Tag = param;
                ParametersNode.Nodes.Add(newNode);
            }

            // Make sure all nodes are visible
            SubsystemNode.ExpandAll();
        }

        private void addNodeButton_Click(object sender, EventArgs e)
        {
            // Check for location
            TreeNode currentNode = subsystemTreeView.SelectedNode;
            if (currentNode == SubsystemNode)
            {
                // Do nothing to add to head node
                return;
            }
            if (currentNode.Parent != SubsystemNode)
            {
                // Shift to right level
                currentNode = currentNode.Parent;
            }
            if (currentNode.Text == "Scripted Functions")
            {
                // Create new function with name from textbox dialog
                ScriptedFunction newFunction = new ScriptedFunction();
                TextboxDialog dialog = new TextboxDialog("Create new function", "New function name");
                dialog.ShowDialog();
                if (dialog.Cancelled)
                {
                    return;
                }
                newFunction.Name = dialog.StringValue;

                // Add function node
                TreeNode newNode = new TreeNode(newFunction.Name + "()");
                newNode.Tag = newFunction;
                currentNode.Nodes.Add(newNode);
                currentNode.Expand();

                // Add function text at bottom, set cursor position to function parameters
                string header = "function " + newFunction.Name + "()";
                scriptingEditor.InsertLine("", -1);
                scriptingEditor.InsertLine(header, -1);
                scriptingEditor.InsertLine("end", -1);
                scriptingEditor.Focus();
                int focusCol = header.Length - 1;
                int focusRow = scriptingEditor.Lines.Length - 2;
                scriptingEditor.CursorPosition = new Point(focusCol, focusRow);

                // Add function to subsystem script
                Script.Functions.Add(newFunction);
            }
            if (currentNode.Text == "State Variables")
            {
                // Create new variable with name from textbox dialog
                StateVariable newVar = new StateVariable();
                TextboxDialog dialog = new TextboxDialog("Create new state variable", "New variable name");
                dialog.ShowDialog();
                if (dialog.Cancelled)
                {
                    return;
                }
                newVar.Name = dialog.StringValue;

                // Add variable node
                TreeNode newNode = new TreeNode(newVar.Name);
                newNode.Tag = newVar;
                currentNode.Nodes.Add(newNode);
                currentNode.Expand();

                // Add variable to global symbols
                ScriptingSymbol varSymb = new ScriptingSymbol();
                varSymb.Enabled = true;
                varSymb.Name = newVar.Name;
                varSymb.Type = LuaSymbolType.Global;
                scriptingEditor.ScriptingSymbolListbox.Symbols.Add(varSymb);
                scriptingEditor.ScriptingSymbolListbox.SortSymbols();

                // Add function to subsystem script
                Script.States.Add(newVar);
            }
            if (currentNode.Text == "Subsystem Parameters")
            {
                // Create new parameter with name from textbox dialog
                SubsystemParameter newParam = new SubsystemParameter();
                TextboxDialog dialog = new TextboxDialog("Create new parameter", "New parameter name");
                dialog.ShowDialog();
                if (dialog.Cancelled)
                {
                    return;
                }
                newParam.Name = dialog.StringValue;

                // Add parameter node
                TreeNode newNode = new TreeNode(newParam.Name);
                newNode.Tag = newParam;
                currentNode.Nodes.Add(newNode);
                currentNode.Expand();

                // Add parameter to global symbols
                ScriptingSymbol varSymb = new ScriptingSymbol();
                varSymb.Enabled = true;
                varSymb.Name = newParam.Name;
                varSymb.Type = LuaSymbolType.Global;
                scriptingEditor.ScriptingSymbolListbox.Symbols.Add(varSymb);
                scriptingEditor.ScriptingSymbolListbox.SortSymbols();

                // Add parameter to subsystem script
                Script.Parameters.Add(newParam);
            }
        }

        private void UpdateIcons()
        {
            // Sets status of icons depending on node selection
            TreeNode node = subsystemTreeView.SelectedNode;
            if (node == null || node == SubsystemNode)
            {
                addNodeButton.Enabled = false;
                removeNodeButton.Enabled = false;
            }
            else
            {
                addNodeButton.Enabled = true;
                if (node.Parent == FunctionNode)
                {
                    ScriptedFunction func = (ScriptedFunction)node.Tag;
                    removeNodeButton.Enabled = !func.IsRequired;
                }
                else
                {
                    removeNodeButton.Enabled = true;
                }
            }
        }

        private void subsystemTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            // Update icons
            Point p = new Point(e.X, e.Y);
            TreeNode node = subsystemTreeView.GetNodeAt(p);
            subsystemTreeView.SelectedNode = node;
            UpdateIcons();
            if (node.Tag != null)
            {
                propertyGrid.SelectedObject = node.Tag;
            }
        }
    }
}
