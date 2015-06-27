using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Linq;
using PicassoGUI.Controls;

namespace PicassoGUI
{
    public partial class DependencyForm : Form
    {
        public Dependency Dependency { get; set; }

        //private String directory;

        public DependencyForm(Connector connector)
        {
            InitializeComponent();
            this.Dependency = connector.Dependency;
            //this.directory = this.Dependency.CustomLogicFileDirectory;

            //this.scriptedDepGroupBox.Enabled = this.Dependency.Scripted;

            this.depFunctionCheckBox.Checked = false;
            this.autoRichTextBox1.Enabled = !this.depFunctionCheckBox.Checked;
            this.depNameTextBox.Text = this.Dependency.Name;
            this.depPropertyGrid.SelectedObject = this.Dependency;

            if (this.Dependency.Scripted)
                this.autoRichTextBox1.Lines = this.Dependency.CustomDependency.Function;

            
            if (this.Dependency.CustomLogicFile != null)
            {
                this.depFunctionCheckBox.Checked = true;
                DirectoryInfo tempDir = new DirectoryInfo(this.Dependency.CustomLogicFileDirectory);
                this.depFcnComboBox.Items.AddRange(tempDir.GetFiles().ToArray());
                int i = this.depFcnComboBox.FindStringExact(this.Dependency.CustomLogicFile);
                this.depFcnComboBox.SelectedIndex = i;
                this.workingDirLabel.Text = this.Dependency.CustomLogicFileDirectory;
            }
            
            this.groupBox1.Enabled = this.depFunctionCheckBox.Checked;
        }
        private void depPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            if (e.ChangedItem.Label == "Scripted")
                this.IsScripted();
        }

        private void IsScripted()
        {
            bool temp = this.Dependency.Scripted;
            //this.scriptedDepGroupBox.Enabled = temp;
            //this.depPropertyGrid.SelectedObject = this.depListBox.SelectedItem;
        }

        private void depNameTextBox_TextChanged(object sender, EventArgs e)
        {
            this.Dependency.Name = this.depNameTextBox.Text;
            this.depPropertyGrid.SelectedObject = this.Dependency;
        }

        private void depFunctionCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            this.autoRichTextBox1.Enabled = !this.depFunctionCheckBox.Checked;
            this.groupBox1.Enabled = this.depFunctionCheckBox.Checked;

            if (!this.depFunctionCheckBox.Checked)
            {
                this.Dependency.CustomLogicFile = null;
                this.Dependency.CustomLogicFileDirectory = null;
                this.Dependency.CustomDependency.Function = this.autoRichTextBox1.Lines;
            }
            else if (this.depFunctionCheckBox.Checked)
            {
                if (this.depFcnComboBox.SelectedItem != null)
                    this.Dependency.CustomLogicFile = this.depFcnComboBox.SelectedItem.ToString();
                this.Dependency.CustomLogicFileDirectory = this.workingDirLabel.Text;
                this.Dependency.CustomDependency.Function = null;
            }
            else if (this.depFcnComboBox.SelectedItem != null)
            {
                this.Dependency.CustomLogicFile = this.depFcnComboBox.SelectedItem.ToString();
                this.Dependency.CustomLogicFileDirectory = this.workingDirLabel.Text;
            }

            this.depPropertyGrid.SelectedObject = this.Dependency;
        }

        private void depFcnOpenButton_Click(object sender, EventArgs e)
        {
            
            //TODO:  Set the default CustomLogicFIleDirectory to the project folder
            if (this.Dependency.CustomLogicFileDirectory != null)
                this.depFcnFolderBrowserDialog.SelectedPath = this.Dependency.CustomLogicFileDirectory;

            this.depFcnFolderBrowserDialog.ShowDialog();
            if (this.depFcnFolderBrowserDialog.SelectedPath != "")
            {
                this.depFcnComboBox.Items.Clear();

                this.Dependency.CustomLogicFileDirectory = this.depFcnFolderBrowserDialog.SelectedPath;

                DirectoryInfo tempDir = new DirectoryInfo(this.depFcnFolderBrowserDialog.SelectedPath);

                this.workingDirLabel.Text = this.Dependency.CustomLogicFileDirectory;
                this.depFcnComboBox.Items.AddRange(tempDir.GetFiles());
                this.depFcnComboBox.SelectedIndex = 0;
                this.Dependency.CustomLogicFile = (this.depFcnComboBox.SelectedItem.ToString());
                this.Dependency.CustomLogicFileDirectory = this.Dependency.CustomLogicFileDirectory;
                this.depPropertyGrid.SelectedObject = this.Dependency;
            }
            
        }

        private void depFcnComboBox_TextUpdate(object sender, EventArgs e)
        {
            //this.Dependency.CustomLogicFile = this.depFcnComboBox.SelectedText;
            this.depPropertyGrid.SelectedObject = this.Dependency;
        }

        private void depFcnComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //this.Dependency.CustomLogicFile = this.depFcnComboBox.SelectedItem.ToString();
            this.depPropertyGrid.SelectedObject = this.Dependency;
        }

        private void DependencyForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Dependency.CustomDependency.Function = this.autoRichTextBox1.Lines;
            // TODO:  Verfiy Data Here
        }
    }
}
