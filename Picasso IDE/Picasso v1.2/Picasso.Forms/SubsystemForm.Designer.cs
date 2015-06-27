using Picasso.Controls;

namespace Picasso.Forms
{
    partial class SubsystemForm
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

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Scripted Functions");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("State Variables");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Subsystem Parameters");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Subsystem", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3});
            this.subTreeContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.BottomToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.TopToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.RightToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.LeftToolStripPanel = new System.Windows.Forms.ToolStripPanel();
            this.ContentPanel = new System.Windows.Forms.ToolStripContentPanel();
            this.mainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.lhsSplitContainer = new System.Windows.Forms.SplitContainer();
            this.subsystemTreeView = new System.Windows.Forms.TreeView();
            this.propertyGrid = new System.Windows.Forms.PropertyGrid();
            this.scriptingEditor = new Picasso.Controls.ScriptingEditor();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.scriptingTrafficStatus = new Picasso.Controls.TrafficStatus();
            this.removeNodeButton = new TransparencyDemo.TransparentButton();
            this.addNodeButton = new TransparencyDemo.TransparentButton();
            this.subTreeContextMenuStrip.SuspendLayout();
            this.mainSplitContainer.Panel1.SuspendLayout();
            this.mainSplitContainer.Panel2.SuspendLayout();
            this.mainSplitContainer.SuspendLayout();
            this.lhsSplitContainer.Panel1.SuspendLayout();
            this.lhsSplitContainer.Panel2.SuspendLayout();
            this.lhsSplitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.removeNodeButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.addNodeButton)).BeginInit();
            this.SuspendLayout();
            // 
            // subTreeContextMenuStrip
            // 
            this.subTreeContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.deleteToolStripMenuItem});
            this.subTreeContextMenuStrip.Name = "contextMenuStrip1";
            this.subTreeContextMenuStrip.Size = new System.Drawing.Size(129, 48);
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(128, 22);
            this.newToolStripMenuItem.Text = "New...";
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(128, 22);
            this.deleteToolStripMenuItem.Text = "Delete...";
            // 
            // BottomToolStripPanel
            // 
            this.BottomToolStripPanel.Location = new System.Drawing.Point(0, 0);
            this.BottomToolStripPanel.Name = "BottomToolStripPanel";
            this.BottomToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.BottomToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            this.BottomToolStripPanel.Size = new System.Drawing.Size(0, 0);
            // 
            // TopToolStripPanel
            // 
            this.TopToolStripPanel.Location = new System.Drawing.Point(0, 0);
            this.TopToolStripPanel.Name = "TopToolStripPanel";
            this.TopToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.TopToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            this.TopToolStripPanel.Size = new System.Drawing.Size(0, 0);
            // 
            // RightToolStripPanel
            // 
            this.RightToolStripPanel.Location = new System.Drawing.Point(0, 0);
            this.RightToolStripPanel.Name = "RightToolStripPanel";
            this.RightToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.RightToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            this.RightToolStripPanel.Size = new System.Drawing.Size(0, 0);
            // 
            // LeftToolStripPanel
            // 
            this.LeftToolStripPanel.Location = new System.Drawing.Point(0, 0);
            this.LeftToolStripPanel.Name = "LeftToolStripPanel";
            this.LeftToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            this.LeftToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
            this.LeftToolStripPanel.Size = new System.Drawing.Size(0, 0);
            // 
            // ContentPanel
            // 
            this.ContentPanel.AutoScroll = true;
            this.ContentPanel.Size = new System.Drawing.Size(816, 494);
            // 
            // mainSplitContainer
            // 
            this.mainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitContainer.Location = new System.Drawing.Point(0, 22);
            this.mainSplitContainer.Name = "mainSplitContainer";
            // 
            // mainSplitContainer.Panel1
            // 
            this.mainSplitContainer.Panel1.Controls.Add(this.lhsSplitContainer);
            // 
            // mainSplitContainer.Panel2
            // 
            this.mainSplitContainer.Panel2.Controls.Add(this.scriptingEditor);
            this.mainSplitContainer.Size = new System.Drawing.Size(592, 347);
            this.mainSplitContainer.SplitterDistance = 178;
            this.mainSplitContainer.TabIndex = 3;
            // 
            // lhsSplitContainer
            // 
            this.lhsSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lhsSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.lhsSplitContainer.Name = "lhsSplitContainer";
            this.lhsSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // lhsSplitContainer.Panel1
            // 
            this.lhsSplitContainer.Panel1.Controls.Add(this.subsystemTreeView);
            // 
            // lhsSplitContainer.Panel2
            // 
            this.lhsSplitContainer.Panel2.Controls.Add(this.propertyGrid);
            this.lhsSplitContainer.Size = new System.Drawing.Size(178, 347);
            this.lhsSplitContainer.SplitterDistance = 164;
            this.lhsSplitContainer.TabIndex = 0;
            // 
            // subsystemTreeView
            // 
            this.subsystemTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.subsystemTreeView.Location = new System.Drawing.Point(0, 0);
            this.subsystemTreeView.Name = "subsystemTreeView";
            treeNode1.Name = "scriptingNode";
            treeNode1.Text = "Scripted Functions";
            treeNode2.ContextMenuStrip = this.subTreeContextMenuStrip;
            treeNode2.Name = "stateVarNode";
            treeNode2.Text = "State Variables";
            treeNode3.ContextMenuStrip = this.subTreeContextMenuStrip;
            treeNode3.Name = "parametersNode";
            treeNode3.Text = "Subsystem Parameters";
            treeNode4.Name = "subsystemNode";
            treeNode4.Text = "Subsystem";
            this.subsystemTreeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode4});
            this.subsystemTreeView.Size = new System.Drawing.Size(178, 164);
            this.subsystemTreeView.TabIndex = 0;
            this.subsystemTreeView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.subsystemTreeView_MouseUp);
            // 
            // propertyGrid
            // 
            this.propertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid.Location = new System.Drawing.Point(0, 0);
            this.propertyGrid.Name = "propertyGrid";
            this.propertyGrid.Size = new System.Drawing.Size(178, 179);
            this.propertyGrid.TabIndex = 1;
            // 
            // scriptingEditor
            // 
            this.scriptingEditor.CursorPosition = new System.Drawing.Point(0, 0);
            this.scriptingEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scriptingEditor.Lines = new string[0];
            this.scriptingEditor.Location = new System.Drawing.Point(0, 0);
            this.scriptingEditor.Name = "scriptingEditor";
            this.scriptingEditor.Size = new System.Drawing.Size(410, 347);
            this.scriptingEditor.StatusText = "Cursor Position: Row 0, Col 0";
            this.scriptingEditor.TabIndex = 23;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(592, 22);
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // scriptingTrafficStatus
            // 
            this.scriptingTrafficStatus.Location = new System.Drawing.Point(184, 3);
            this.scriptingTrafficStatus.Name = "scriptingTrafficStatus";
            this.scriptingTrafficStatus.Size = new System.Drawing.Size(61, 16);
            this.scriptingTrafficStatus.TabIndex = 22;
            // 
            // removeNodeButton
            // 
            this.removeNodeButton.BackColor = System.Drawing.Color.Transparent;
            this.removeNodeButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.removeNodeButton.Image = global::Picasso.Forms.Properties.Resources.cyanButtonOff;
            this.removeNodeButton.Location = new System.Drawing.Point(162, 3);
            this.removeNodeButton.Name = "removeNodeButton";
            this.removeNodeButton.OverImage = global::Picasso.Forms.Properties.Resources.cyanButtonOn;
            this.removeNodeButton.Size = new System.Drawing.Size(16, 16);
            this.removeNodeButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.removeNodeButton.TabIndex = 0;
            this.removeNodeButton.TabStop = false;
            this.removeNodeButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            // 
            // addNodeButton
            // 
            this.addNodeButton.BackColor = System.Drawing.Color.Transparent;
            this.addNodeButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.addNodeButton.Image = global::Picasso.Forms.Properties.Resources.blueButtonOff;
            this.addNodeButton.Location = new System.Drawing.Point(140, 3);
            this.addNodeButton.Name = "addNodeButton";
            this.addNodeButton.OverImage = global::Picasso.Forms.Properties.Resources.blueButtonOn;
            this.addNodeButton.Size = new System.Drawing.Size(16, 16);
            this.addNodeButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.addNodeButton.TabIndex = 21;
            this.addNodeButton.TabStop = false;
            this.addNodeButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            this.addNodeButton.Click += new System.EventHandler(this.addNodeButton_Click);
            // 
            // SubsystemForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(592, 369);
            this.Controls.Add(this.scriptingTrafficStatus);
            this.Controls.Add(this.removeNodeButton);
            this.Controls.Add(this.addNodeButton);
            this.Controls.Add(this.mainSplitContainer);
            this.Controls.Add(this.pictureBox1);
            this.Name = "SubsystemForm";
            this.Text = "Subsystem Design";
            this.subTreeContextMenuStrip.ResumeLayout(false);
            this.mainSplitContainer.Panel1.ResumeLayout(false);
            this.mainSplitContainer.Panel2.ResumeLayout(false);
            this.mainSplitContainer.ResumeLayout(false);
            this.lhsSplitContainer.Panel1.ResumeLayout(false);
            this.lhsSplitContainer.Panel2.ResumeLayout(false);
            this.lhsSplitContainer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.removeNodeButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.addNodeButton)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip subTreeContextMenuStrip;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripPanel BottomToolStripPanel;
        private System.Windows.Forms.ToolStripPanel TopToolStripPanel;
        private System.Windows.Forms.ToolStripPanel RightToolStripPanel;
        private System.Windows.Forms.ToolStripPanel LeftToolStripPanel;
        private System.Windows.Forms.ToolStripContentPanel ContentPanel;
        private System.Windows.Forms.SplitContainer mainSplitContainer;
        private System.Windows.Forms.SplitContainer lhsSplitContainer;
        private System.Windows.Forms.TreeView subsystemTreeView;
        private System.Windows.Forms.PropertyGrid propertyGrid;
        private TransparencyDemo.TransparentButton addNodeButton;
        private System.Windows.Forms.PictureBox pictureBox1;
        private TransparencyDemo.TransparentButton removeNodeButton;
        private TrafficStatus scriptingTrafficStatus;
        private ScriptingEditor scriptingEditor;
    }
}
