namespace Picasso.Forms
{
    partial class TargetForm
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
            this.positionGroup = new System.Windows.Forms.GroupBox();
            this.pzLabel = new System.Windows.Forms.Label();
            this.pyLabel = new System.Windows.Forms.Label();
            this.pxLabel = new System.Windows.Forms.Label();
            this.vzLabel = new System.Windows.Forms.Label();
            this.vyLabel = new System.Windows.Forms.Label();
            this.vxLabel = new System.Windows.Forms.Label();
            this.vyBox = new System.Windows.Forms.TextBox();
            this.vzBox = new System.Windows.Forms.TextBox();
            this.pxBox = new System.Windows.Forms.TextBox();
            this.pyBox = new System.Windows.Forms.TextBox();
            this.pzBox = new System.Windows.Forms.TextBox();
            this.vxBox = new System.Windows.Forms.TextBox();
            this.zLabel = new System.Windows.Forms.Label();
            this.yLabel = new System.Windows.Forms.Label();
            this.xLabel = new System.Windows.Forms.Label();
            this.altBox = new System.Windows.Forms.TextBox();
            this.latBox = new System.Windows.Forms.TextBox();
            this.longBox = new System.Windows.Forms.TextBox();
            this.typeComboBox = new System.Windows.Forms.ComboBox();
            this.positionTypeLabel = new System.Windows.Forms.Label();
            this.positionGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // positionGroup
            // 
            this.positionGroup.Controls.Add(this.pzLabel);
            this.positionGroup.Controls.Add(this.pyLabel);
            this.positionGroup.Controls.Add(this.pxLabel);
            this.positionGroup.Controls.Add(this.vzLabel);
            this.positionGroup.Controls.Add(this.vyLabel);
            this.positionGroup.Controls.Add(this.vxLabel);
            this.positionGroup.Controls.Add(this.vyBox);
            this.positionGroup.Controls.Add(this.vzBox);
            this.positionGroup.Controls.Add(this.pxBox);
            this.positionGroup.Controls.Add(this.pyBox);
            this.positionGroup.Controls.Add(this.pzBox);
            this.positionGroup.Controls.Add(this.vxBox);
            this.positionGroup.Controls.Add(this.zLabel);
            this.positionGroup.Controls.Add(this.yLabel);
            this.positionGroup.Controls.Add(this.xLabel);
            this.positionGroup.Controls.Add(this.altBox);
            this.positionGroup.Controls.Add(this.latBox);
            this.positionGroup.Controls.Add(this.longBox);
            this.positionGroup.Controls.Add(this.typeComboBox);
            this.positionGroup.Controls.Add(this.positionTypeLabel);
            this.positionGroup.Location = new System.Drawing.Point(12, 12);
            this.positionGroup.Name = "positionGroup";
            this.positionGroup.Size = new System.Drawing.Size(359, 134);
            this.positionGroup.TabIndex = 0;
            this.positionGroup.TabStop = false;
            this.positionGroup.Text = "Target Position";
            // 
            // pzLabel
            // 
            this.pzLabel.AutoSize = true;
            this.pzLabel.Location = new System.Drawing.Point(14, 104);
            this.pzLabel.Name = "pzLabel";
            this.pzLabel.Size = new System.Drawing.Size(54, 13);
            this.pzLabel.TabIndex = 18;
            this.pzLabel.Text = "Position Z";
            this.pzLabel.Visible = false;
            // 
            // pyLabel
            // 
            this.pyLabel.AutoSize = true;
            this.pyLabel.Location = new System.Drawing.Point(14, 78);
            this.pyLabel.Name = "pyLabel";
            this.pyLabel.Size = new System.Drawing.Size(54, 13);
            this.pyLabel.TabIndex = 17;
            this.pyLabel.Text = "Position Y";
            this.pyLabel.Visible = false;
            // 
            // pxLabel
            // 
            this.pxLabel.AutoSize = true;
            this.pxLabel.Location = new System.Drawing.Point(14, 52);
            this.pxLabel.Name = "pxLabel";
            this.pxLabel.Size = new System.Drawing.Size(54, 13);
            this.pxLabel.TabIndex = 16;
            this.pxLabel.Text = "Position X";
            this.pxLabel.Visible = false;
            // 
            // vzLabel
            // 
            this.vzLabel.AutoSize = true;
            this.vzLabel.Location = new System.Drawing.Point(193, 104);
            this.vzLabel.Name = "vzLabel";
            this.vzLabel.Size = new System.Drawing.Size(54, 13);
            this.vzLabel.TabIndex = 15;
            this.vzLabel.Text = "Velocity Z";
            this.vzLabel.Visible = false;
            // 
            // vyLabel
            // 
            this.vyLabel.AutoSize = true;
            this.vyLabel.Location = new System.Drawing.Point(193, 78);
            this.vyLabel.Name = "vyLabel";
            this.vyLabel.Size = new System.Drawing.Size(54, 13);
            this.vyLabel.TabIndex = 14;
            this.vyLabel.Text = "Velocity Y";
            this.vyLabel.Visible = false;
            // 
            // vxLabel
            // 
            this.vxLabel.AutoSize = true;
            this.vxLabel.Location = new System.Drawing.Point(193, 52);
            this.vxLabel.Name = "vxLabel";
            this.vxLabel.Size = new System.Drawing.Size(54, 13);
            this.vxLabel.TabIndex = 13;
            this.vxLabel.Text = "Velocity X";
            this.vxLabel.Visible = false;
            // 
            // vyBox
            // 
            this.vyBox.Location = new System.Drawing.Point(253, 75);
            this.vyBox.Name = "vyBox";
            this.vyBox.Size = new System.Drawing.Size(100, 20);
            this.vyBox.TabIndex = 12;
            this.vyBox.Visible = false;
            this.vyBox.TextChanged += new System.EventHandler(this.vyBox_TextChanged);
            // 
            // vzBox
            // 
            this.vzBox.Location = new System.Drawing.Point(253, 101);
            this.vzBox.Name = "vzBox";
            this.vzBox.Size = new System.Drawing.Size(100, 20);
            this.vzBox.TabIndex = 11;
            this.vzBox.Visible = false;
            this.vzBox.TextChanged += new System.EventHandler(this.vzBox_TextChanged);
            // 
            // pxBox
            // 
            this.pxBox.Location = new System.Drawing.Point(74, 49);
            this.pxBox.Name = "pxBox";
            this.pxBox.Size = new System.Drawing.Size(100, 20);
            this.pxBox.TabIndex = 10;
            this.pxBox.Visible = false;
            this.pxBox.TextChanged += new System.EventHandler(this.pxBox_TextChanged);
            // 
            // pyBox
            // 
            this.pyBox.Location = new System.Drawing.Point(74, 75);
            this.pyBox.Name = "pyBox";
            this.pyBox.Size = new System.Drawing.Size(100, 20);
            this.pyBox.TabIndex = 9;
            this.pyBox.Visible = false;
            this.pyBox.TextChanged += new System.EventHandler(this.pyBox_TextChanged);
            // 
            // pzBox
            // 
            this.pzBox.Location = new System.Drawing.Point(74, 101);
            this.pzBox.Name = "pzBox";
            this.pzBox.Size = new System.Drawing.Size(100, 20);
            this.pzBox.TabIndex = 8;
            this.pzBox.Visible = false;
            this.pzBox.TextChanged += new System.EventHandler(this.pzBox_TextChanged);
            // 
            // vxBox
            // 
            this.vxBox.Location = new System.Drawing.Point(253, 49);
            this.vxBox.Name = "vxBox";
            this.vxBox.Size = new System.Drawing.Size(100, 20);
            this.vxBox.TabIndex = 1;
            this.vxBox.Visible = false;
            this.vxBox.TextChanged += new System.EventHandler(this.vxBox_TextChanged);
            // 
            // zLabel
            // 
            this.zLabel.AutoSize = true;
            this.zLabel.Location = new System.Drawing.Point(100, 104);
            this.zLabel.Name = "zLabel";
            this.zLabel.Size = new System.Drawing.Size(42, 13);
            this.zLabel.TabIndex = 7;
            this.zLabel.Text = "Altitude";
            // 
            // yLabel
            // 
            this.yLabel.AutoSize = true;
            this.yLabel.Location = new System.Drawing.Point(97, 78);
            this.yLabel.Name = "yLabel";
            this.yLabel.Size = new System.Drawing.Size(45, 13);
            this.yLabel.TabIndex = 6;
            this.yLabel.Text = "Latitude";
            // 
            // xLabel
            // 
            this.xLabel.AutoSize = true;
            this.xLabel.Location = new System.Drawing.Point(88, 52);
            this.xLabel.Name = "xLabel";
            this.xLabel.Size = new System.Drawing.Size(54, 13);
            this.xLabel.TabIndex = 5;
            this.xLabel.Text = "Longitude";
            // 
            // altBox
            // 
            this.altBox.Location = new System.Drawing.Point(148, 101);
            this.altBox.Name = "altBox";
            this.altBox.Size = new System.Drawing.Size(100, 20);
            this.altBox.TabIndex = 4;
            // 
            // latBox
            // 
            this.latBox.Location = new System.Drawing.Point(148, 75);
            this.latBox.Name = "latBox";
            this.latBox.Size = new System.Drawing.Size(100, 20);
            this.latBox.TabIndex = 3;
            // 
            // longBox
            // 
            this.longBox.Location = new System.Drawing.Point(148, 49);
            this.longBox.Name = "longBox";
            this.longBox.Size = new System.Drawing.Size(100, 20);
            this.longBox.TabIndex = 2;
            this.longBox.TextChanged += new System.EventHandler(this.xBox_TextChanged);
            // 
            // typeComboBox
            // 
            this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeComboBox.FormattingEnabled = true;
            this.typeComboBox.Location = new System.Drawing.Point(148, 22);
            this.typeComboBox.Name = "typeComboBox";
            this.typeComboBox.Size = new System.Drawing.Size(121, 21);
            this.typeComboBox.TabIndex = 1;
            this.typeComboBox.SelectedIndexChanged += new System.EventHandler(this.typeComboBox_SelectedIndexChanged);
            // 
            // positionTypeLabel
            // 
            this.positionTypeLabel.AutoSize = true;
            this.positionTypeLabel.Location = new System.Drawing.Point(71, 25);
            this.positionTypeLabel.Name = "positionTypeLabel";
            this.positionTypeLabel.Size = new System.Drawing.Size(71, 13);
            this.positionTypeLabel.TabIndex = 0;
            this.positionTypeLabel.Text = "Position Type";
            // 
            // TargetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(383, 158);
            this.Controls.Add(this.positionGroup);
            this.Name = "TargetForm";
            this.Text = "TargetForm";
            this.positionGroup.ResumeLayout(false);
            this.positionGroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox positionGroup;
        private System.Windows.Forms.Label positionTypeLabel;
        private System.Windows.Forms.ComboBox typeComboBox;
        private System.Windows.Forms.Label zLabel;
        private System.Windows.Forms.Label yLabel;
        private System.Windows.Forms.Label xLabel;
        private System.Windows.Forms.TextBox altBox;
        private System.Windows.Forms.TextBox latBox;
        private System.Windows.Forms.TextBox longBox;
        private System.Windows.Forms.Label vzLabel;
        private System.Windows.Forms.Label vyLabel;
        private System.Windows.Forms.Label vxLabel;
        private System.Windows.Forms.TextBox vyBox;
        private System.Windows.Forms.TextBox vzBox;
        private System.Windows.Forms.TextBox pxBox;
        private System.Windows.Forms.TextBox pyBox;
        private System.Windows.Forms.TextBox pzBox;
        private System.Windows.Forms.TextBox vxBox;
        private System.Windows.Forms.Label pzLabel;
        private System.Windows.Forms.Label pyLabel;
        private System.Windows.Forms.Label pxLabel;
    }
}