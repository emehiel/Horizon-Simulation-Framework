using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;
using Hsf.Elements;
using Hsf.Components;
using Picasso.Utility;
using MementoModel;

namespace Picasso.Forms
{
    public partial class ModelForm : PicassoSubform, IMForm
    {
        private MementoManager _mManager;
        public ModelComponent Model;
        public override HsfElement Element
        {
            get
            {
                return Model;
            }
        }

        public ModelForm(Form parentWindow, TreeNode node)
        {
            // Creates a new model form for a new model component
            Model = new ModelComponent();
            Node = node;
            MdiParent = parentWindow;
            Initialize();
        }

        public ModelForm(ModelComponent targetComponent, Form parentWindow, TreeNode node)
        {
            // Creates a new model form for a given model component
            Model = targetComponent;
            Node = node;
            MdiParent = parentWindow;
            Initialize();
        }

        private void Initialize()
        {
            _mManager = new MementoManager((IMForm)MdiParent);
            InitializeComponent();
            Tag = Model;
            RefreshValues();
            typeComboBox.DataSource = Enum.GetValues(typeof(ScheduleEvaluatorType));
            Text = "Model Settings";
        }

        private void isScripted_CheckedChanged(object sender, EventArgs e)
        {
            // First, disable / enable scripting options based on value
            if (isScripted.Checked)
            {
                luaFilesAddButton.Enabled = true;
                luaFilesAddTextbox.Enabled = true;
                luaFilesLabel.Enabled = true;
                luaFilesListbox.Enabled = true;
                luaFilesRemoveButton.Enabled = true;
            }
            else
            {
                luaFilesAddButton.Enabled = false;
                luaFilesAddTextbox.Enabled = false;
                luaFilesLabel.Enabled = false;
                luaFilesListbox.Enabled = false;
                luaFilesRemoveButton.Enabled = false;
            }

            // Then, write and register changes
            ModelComponent before = (ModelComponent)Model.Clone();
            Model.Lua.IsScriptingEnabled = isScripted.Checked;
            ModelComponent after = (ModelComponent)Model.Clone();
            _mManager.RegisterEvent(before, after, Model, "Scripted");
        }

        private void typeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // First, disable / enable script function based on selection
            ScheduleEvaluatorType current = (ScheduleEvaluatorType)(typeComboBox.SelectedItem);
            if (current != ScheduleEvaluatorType.scripted)
            {
                evalFunctionTextbox.Enabled = false;
            }
            else
            {
                evalFunctionTextbox.Enabled = true;
            }

            // Then, write changes and register event
            ModelComponent before = (ModelComponent)Model.Clone();
            Model.ScheduleEvaluator.Type = current;
            ModelComponent after = (ModelComponent)Model.Clone();
            if (_mManager != null && before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Model, "Change Type"); }
        }

        private void evalFunctionTextbox_TextChanged(object sender, EventArgs e)
        {
            // Change evaluation function and register event
            ModelComponent before = (ModelComponent)Model.Clone();
            Model.ScheduleEvaluator.EvaulateLuaFcn = evalFunctionTextbox.Text;
            ModelComponent after = (ModelComponent)Model.Clone();
            if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Model, "Function Change"); }
        }

        private void luaFilesAddButton_Click(object sender, EventArgs e)
        {
            // Check for pre-existing script
            foreach (LuaFileElement script in Model.Lua.Files)
            {
                if (script.Src == luaFilesAddTextbox.Text)
                {
                    MessageBox.Show("That script has already been entered.");
                    luaFilesAddTextbox.Text = "";
                    return;
                }
            }

            // Add script name to list control and handle, and empty box; register event
            ModelComponent before = (ModelComponent)Model.Clone();
            string newItem = luaFilesAddTextbox.Text;
            luaFilesListbox.Items.Add(newItem);
            luaFilesAddTextbox.Text = "";
            LuaFileElement newScript = new LuaFileElement();
            newScript.Src = newItem;
            Model.Lua.Files.Add(newScript);
            ModelComponent after = (ModelComponent)Model.Clone();
            _mManager.RegisterEvent(before, after, Model, "Add Script");
        }

        private void luaFilesRemoveButton_Click(object sender, EventArgs e)
        {
            // Remove given file and register change
            ModelComponent before = (ModelComponent)Model.Clone();
            string selected = (string)(luaFilesListbox.SelectedItem);
            luaFilesListbox.Items.Remove(selected);
            Model.Lua.RemoveScriptByName(selected);
            ModelComponent after = (ModelComponent)Model.Clone();
            _mManager.RegisterEvent(before, after, Model, "Remove Script");
        }

        #region Memento

        public override void RefreshValues()
        {
            // Called after event / undo / redo to update fields after component change
            isScripted.Checked = Model.Lua.IsScriptingEnabled;
            typeComboBox.SelectedItem = Model.ScheduleEvaluator.Type;
            evalFunctionTextbox.Text = Model.ScheduleEvaluator.EvaulateLuaFcn;

            // If scripting is disabled, scripting options should be disabled
            if (!Model.Lua.IsScriptingEnabled)
            {
                luaFilesAddButton.Enabled = false;
                luaFilesAddTextbox.Enabled = false;
                luaFilesLabel.Enabled = false;
                luaFilesListbox.Enabled = false;
                luaFilesRemoveButton.Enabled = false;
            }

            // If scheduler is not scripted, scripting function should be disabled
            if (Model.ScheduleEvaluator.Type != ScheduleEvaluatorType.scripted)
            {
                evalFunctionTextbox.Enabled = false;
            }

            // Load list of scripts into listbox
            luaFilesListbox.Items.Clear();
            foreach (LuaFileElement file in Model.Lua.Files)
            {
                luaFilesListbox.Items.Add(file.Src);
            }
        }

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip according to this manager
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form
            return _mManager;
        }

        #endregion
    }
}
