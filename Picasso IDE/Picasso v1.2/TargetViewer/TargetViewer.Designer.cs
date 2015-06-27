namespace TargetViewer
{
    partial class TargetViewer
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TargetViewer));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.navControl = new NavControl.NavControl();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.targetListView = new System.Windows.Forms.ListView();
            this.nameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.positionColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.targetTypeColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.taskTypeColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.maxTimesColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.valueColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.targetIconImageList = new System.Windows.Forms.ImageList(this.components);
            this.targetPropertyGrid = new System.Windows.Forms.PropertyGrid();
            this.importTargetFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.exportTargetFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.navControl);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1024, 712);
            this.splitContainer1.SplitterDistance = 498;
            this.splitContainer1.TabIndex = 0;
            // 
            // navControl
            // 
            this.navControl.BackColor = System.Drawing.SystemColors.Control;
            this.navControl.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.navControl.Center = new System.Drawing.Point(1024, 512);
            this.navControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.navControl.Location = new System.Drawing.Point(0, 0);
            this.navControl.Name = "navControl";
            this.navControl.Size = new System.Drawing.Size(1024, 498);
            this.navControl.TabIndex = 0;
            this.navControl.Zoom = 1;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.targetListView);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.targetPropertyGrid);
            this.splitContainer2.Size = new System.Drawing.Size(1024, 210);
            this.splitContainer2.SplitterDistance = 610;
            this.splitContainer2.TabIndex = 1;
            // 
            // targetListView
            // 
            this.targetListView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.targetListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.nameColumnHeader,
            this.positionColumnHeader,
            this.targetTypeColumnHeader,
            this.taskTypeColumnHeader,
            this.maxTimesColumnHeader,
            this.valueColumnHeader});
            this.targetListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.targetListView.FullRowSelect = true;
            this.targetListView.GridLines = true;
            this.targetListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.targetListView.Location = new System.Drawing.Point(0, 0);
            this.targetListView.Name = "targetListView";
            this.targetListView.Size = new System.Drawing.Size(610, 210);
            this.targetListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.targetListView.StateImageList = this.targetIconImageList;
            this.targetListView.TabIndex = 0;
            this.targetListView.UseCompatibleStateImageBehavior = false;
            this.targetListView.View = System.Windows.Forms.View.Details;
            this.targetListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listView1_ItemSelectionChanged);
            // 
            // nameColumnHeader
            // 
            this.nameColumnHeader.Text = "Name";
            // 
            // positionColumnHeader
            // 
            this.positionColumnHeader.Text = "Position";
            // 
            // targetTypeColumnHeader
            // 
            this.targetTypeColumnHeader.Text = "Target Type";
            // 
            // taskTypeColumnHeader
            // 
            this.taskTypeColumnHeader.Text = "Task Type";
            // 
            // maxTimesColumnHeader
            // 
            this.maxTimesColumnHeader.Text = "Max #";
            // 
            // valueColumnHeader
            // 
            this.valueColumnHeader.Text = "Value";
            // 
            // targetIconImageList
            // 
            this.targetIconImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("targetIconImageList.ImageStream")));
            this.targetIconImageList.TransparentColor = System.Drawing.Color.Blue;
            this.targetIconImageList.Images.SetKeyName(0, "ground.bmp");
            this.targetIconImageList.Images.SetKeyName(1, "target.bmp");
            this.targetIconImageList.Images.SetKeyName(2, "target_selected.bmp");
            // 
            // targetPropertyGrid
            // 
            this.targetPropertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.targetPropertyGrid.Location = new System.Drawing.Point(0, 0);
            this.targetPropertyGrid.Name = "targetPropertyGrid";
            this.targetPropertyGrid.Size = new System.Drawing.Size(410, 210);
            this.targetPropertyGrid.TabIndex = 0;
            this.targetPropertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.targetPropertyGrid_PropertyValueChanged);
            // 
            // importTargetFileDialog
            // 
            this.importTargetFileDialog.DefaultExt = "xml";
            this.importTargetFileDialog.FileName = "targs";
            this.importTargetFileDialog.Title = "Import Target Deck";
            this.importTargetFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.importTargetFileDialog_FileOk);
            // 
            // exportTargetFileDialog
            // 
            this.exportTargetFileDialog.DefaultExt = "xml";
            this.exportTargetFileDialog.FileName = "targs";
            this.exportTargetFileDialog.Title = "Export Target Deck";
            this.exportTargetFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.exportTargetFileDialog_FileOk);
            // 
            // TargetViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "TargetViewer";
            this.Size = new System.Drawing.Size(1024, 712);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListView targetListView;
        private System.Windows.Forms.ColumnHeader nameColumnHeader;
        private System.Windows.Forms.ColumnHeader positionColumnHeader;
        private System.Windows.Forms.ColumnHeader targetTypeColumnHeader;
        private System.Windows.Forms.ColumnHeader taskTypeColumnHeader;
        private System.Windows.Forms.ColumnHeader maxTimesColumnHeader;
        private System.Windows.Forms.ColumnHeader valueColumnHeader;
        private System.Windows.Forms.OpenFileDialog importTargetFileDialog;
        private System.Windows.Forms.ImageList targetIconImageList;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.PropertyGrid targetPropertyGrid;
        private NavControl.NavControl navControl;
        private System.Windows.Forms.SaveFileDialog exportTargetFileDialog;
    }
}
