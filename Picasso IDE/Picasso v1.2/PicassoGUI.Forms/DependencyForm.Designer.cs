namespace PicassoGUI
{
    partial class DependencyForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.subsystemNameLabel = new System.Windows.Forms.Label();
            this.subsystemNameTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.depPropertyGrid = new System.Windows.Forms.PropertyGrid();
            this.autoRichTextBox1 = new ARTB.AutoRichTextBox();
            this.depFunctionCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.workingDirLabel = new System.Windows.Forms.Label();
            this.depFcnComboBox = new System.Windows.Forms.ComboBox();
            this.depFcnOpenButton = new System.Windows.Forms.Button();
            this.depFcnFolderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.panel1 = new System.Windows.Forms.Panel();
            this.doneButton = new System.Windows.Forms.Button();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.depNameTextBox = new System.Windows.Forms.ToolStripTextBox();
            this.groupBox1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // subsystemNameLabel
            // 
            this.subsystemNameLabel.AutoSize = true;
            this.subsystemNameLabel.Location = new System.Drawing.Point(9, -22);
            this.subsystemNameLabel.Name = "subsystemNameLabel";
            this.subsystemNameLabel.Size = new System.Drawing.Size(89, 13);
            this.subsystemNameLabel.TabIndex = 14;
            this.subsystemNameLabel.Text = "Subsystem Name";
            // 
            // subsystemNameTextBox
            // 
            this.subsystemNameTextBox.Location = new System.Drawing.Point(104, -25);
            this.subsystemNameTextBox.Name = "subsystemNameTextBox";
            this.subsystemNameTextBox.Size = new System.Drawing.Size(100, 20);
            this.subsystemNameTextBox.TabIndex = 13;
            this.subsystemNameTextBox.Text = "New Subsystem";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 10);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(118, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Dependency Properties";
            // 
            // depPropertyGrid
            // 
            this.depPropertyGrid.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.depPropertyGrid.Location = new System.Drawing.Point(0, 30);
            this.depPropertyGrid.Name = "depPropertyGrid";
            this.depPropertyGrid.Size = new System.Drawing.Size(243, 385);
            this.depPropertyGrid.TabIndex = 5;
            this.depPropertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.depPropertyGrid_PropertyValueChanged);
            // 
            // autoRichTextBox1
            // 
            this.autoRichTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.autoRichTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.autoRichTextBox1.Lines = new string[0];
            this.autoRichTextBox1.Location = new System.Drawing.Point(0, 0);
            this.autoRichTextBox1.Name = "autoRichTextBox1";
            this.autoRichTextBox1.Size = new System.Drawing.Size(635, 415);
            this.autoRichTextBox1.TabIndex = 7;
            // 
            // depFunctionCheckBox
            // 
            this.depFunctionCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.depFunctionCheckBox.AutoSize = true;
            this.depFunctionCheckBox.Location = new System.Drawing.Point(3, 3);
            this.depFunctionCheckBox.Name = "depFunctionCheckBox";
            this.depFunctionCheckBox.Size = new System.Drawing.Size(215, 17);
            this.depFunctionCheckBox.TabIndex = 2;
            this.depFunctionCheckBox.Text = "Use Custom LUA Depencency Function";
            this.depFunctionCheckBox.UseVisualStyleBackColor = true;
            this.depFunctionCheckBox.CheckedChanged += new System.EventHandler(this.depFunctionCheckBox_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.workingDirLabel);
            this.groupBox1.Controls.Add(this.depFcnComboBox);
            this.groupBox1.Controls.Add(this.depFcnOpenButton);
            this.groupBox1.Location = new System.Drawing.Point(3, 26);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(262, 86);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Custom Dependency Function";
            // 
            // workingDirLabel
            // 
            this.workingDirLabel.AutoSize = true;
            this.workingDirLabel.Location = new System.Drawing.Point(6, 43);
            this.workingDirLabel.Name = "workingDirLabel";
            this.workingDirLabel.Size = new System.Drawing.Size(0, 13);
            this.workingDirLabel.TabIndex = 5;
            // 
            // depFcnComboBox
            // 
            this.depFcnComboBox.FormattingEnabled = true;
            this.depFcnComboBox.Location = new System.Drawing.Point(9, 20);
            this.depFcnComboBox.Name = "depFcnComboBox";
            this.depFcnComboBox.Size = new System.Drawing.Size(215, 21);
            this.depFcnComboBox.TabIndex = 3;
            this.depFcnComboBox.SelectionChangeCommitted += new System.EventHandler(this.depFcnComboBox_TextUpdate);
            this.depFcnComboBox.SelectedIndexChanged += new System.EventHandler(this.depFcnComboBox_SelectedIndexChanged);
            this.depFcnComboBox.TextUpdate += new System.EventHandler(this.depFcnComboBox_TextUpdate);
            // 
            // depFcnOpenButton
            // 
            this.depFcnOpenButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.depFcnOpenButton.Location = new System.Drawing.Point(227, 19);
            this.depFcnOpenButton.Name = "depFcnOpenButton";
            this.depFcnOpenButton.Size = new System.Drawing.Size(29, 21);
            this.depFcnOpenButton.TabIndex = 4;
            this.depFcnOpenButton.Text = "...";
            this.depFcnOpenButton.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.depFcnOpenButton.UseVisualStyleBackColor = true;
            this.depFcnOpenButton.Click += new System.EventHandler(this.depFcnOpenButton_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.depPropertyGrid);
            this.splitContainer1.Panel1.Controls.Add(this.label3);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.autoRichTextBox1);
            this.splitContainer1.Size = new System.Drawing.Size(882, 415);
            this.splitContainer1.SplitterDistance = 243;
            this.splitContainer1.TabIndex = 17;
            // 
            // toolStripContainer1
            // 
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.panel1);
            this.toolStripContainer1.ContentPanel.Controls.Add(this.splitContainer1);
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(882, 536);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.Size = new System.Drawing.Size(882, 561);
            this.toolStripContainer1.TabIndex = 18;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip1);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.doneButton);
            this.panel1.Controls.Add(this.depFunctionCheckBox);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 421);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(882, 115);
            this.panel1.TabIndex = 18;
            // 
            // doneButton
            // 
            this.doneButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.doneButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.doneButton.Location = new System.Drawing.Point(804, 80);
            this.doneButton.Name = "doneButton";
            this.doneButton.Size = new System.Drawing.Size(75, 23);
            this.doneButton.TabIndex = 0;
            this.doneButton.Text = "Done";
            this.doneButton.UseVisualStyleBackColor = true;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.depNameTextBox});
            this.toolStrip1.Location = new System.Drawing.Point(3, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(211, 25);
            this.toolStrip1.TabIndex = 0;
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(97, 22);
            this.toolStripLabel1.Text = "Dependency Name";
            // 
            // depNameTextBox
            // 
            this.depNameTextBox.Name = "depNameTextBox";
            this.depNameTextBox.Size = new System.Drawing.Size(100, 25);
            // 
            // DependencyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(882, 561);
            this.Controls.Add(this.toolStripContainer1);
            this.Controls.Add(this.subsystemNameLabel);
            this.Controls.Add(this.subsystemNameTextBox);
            this.Name = "DependencyForm";
            this.Text = "Dependency Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DependencyForm_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label subsystemNameLabel;
        private System.Windows.Forms.TextBox subsystemNameTextBox;
        private System.Windows.Forms.PropertyGrid depPropertyGrid;
        private System.Windows.Forms.CheckBox depFunctionCheckBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox depFcnComboBox;
        private System.Windows.Forms.Button depFcnOpenButton;
        private System.Windows.Forms.FolderBrowserDialog depFcnFolderBrowserDialog;
        private System.Windows.Forms.Label workingDirLabel;
        private System.Windows.Forms.GroupBox groupBox1;
        private ARTB.AutoRichTextBox autoRichTextBox1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox depNameTextBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button doneButton;
    }
}