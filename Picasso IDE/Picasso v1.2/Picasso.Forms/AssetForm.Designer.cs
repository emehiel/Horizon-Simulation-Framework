namespace Picasso.Forms
{
    partial class AssetForm
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
            this.x1Label = new System.Windows.Forms.Label();
            this.y1Label = new System.Windows.Forms.Label();
            this.z1Label = new System.Windows.Forms.Label();
            this.z2Label = new System.Windows.Forms.Label();
            this.y2Label = new System.Windows.Forms.Label();
            this.x2Label = new System.Windows.Forms.Label();
            this.z2Textbox = new System.Windows.Forms.TextBox();
            this.y2Textbox = new System.Windows.Forms.TextBox();
            this.x2Textbox = new System.Windows.Forms.TextBox();
            this.z1Textbox = new System.Windows.Forms.TextBox();
            this.y1Textbox = new System.Windows.Forms.TextBox();
            this.x1Textbox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.positionTypeComboBox = new System.Windows.Forms.ComboBox();
            this.eomGroup = new System.Windows.Forms.GroupBox();
            this.evalFcnLabel = new System.Windows.Forms.Label();
            this.initFcnLabel = new System.Windows.Forms.Label();
            this.evalFuncTextbox = new System.Windows.Forms.TextBox();
            this.initFuncTextbox = new System.Windows.Forms.TextBox();
            this.eomTypeLabel = new System.Windows.Forms.Label();
            this.eomTypeComboBox = new System.Windows.Forms.ComboBox();
            this.drawingCanvas1 = new Picasso.Controls.DrawingCanvas();
            this.positionGroup.SuspendLayout();
            this.eomGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // positionGroup
            // 
            this.positionGroup.Controls.Add(this.x1Label);
            this.positionGroup.Controls.Add(this.y1Label);
            this.positionGroup.Controls.Add(this.z1Label);
            this.positionGroup.Controls.Add(this.z2Label);
            this.positionGroup.Controls.Add(this.y2Label);
            this.positionGroup.Controls.Add(this.x2Label);
            this.positionGroup.Controls.Add(this.z2Textbox);
            this.positionGroup.Controls.Add(this.y2Textbox);
            this.positionGroup.Controls.Add(this.x2Textbox);
            this.positionGroup.Controls.Add(this.z1Textbox);
            this.positionGroup.Controls.Add(this.y1Textbox);
            this.positionGroup.Controls.Add(this.x1Textbox);
            this.positionGroup.Controls.Add(this.label1);
            this.positionGroup.Controls.Add(this.positionTypeComboBox);
            this.positionGroup.Location = new System.Drawing.Point(12, 12);
            this.positionGroup.Name = "positionGroup";
            this.positionGroup.Size = new System.Drawing.Size(350, 100);
            this.positionGroup.TabIndex = 0;
            this.positionGroup.TabStop = false;
            this.positionGroup.Text = "Asset Position";
            // 
            // x1Label
            // 
            this.x1Label.AutoSize = true;
            this.x1Label.Location = new System.Drawing.Point(7, 46);
            this.x1Label.Name = "x1Label";
            this.x1Label.Size = new System.Drawing.Size(20, 13);
            this.x1Label.TabIndex = 13;
            this.x1Label.Text = "X1";
            this.x1Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // y1Label
            // 
            this.y1Label.AutoSize = true;
            this.y1Label.Location = new System.Drawing.Point(132, 46);
            this.y1Label.Name = "y1Label";
            this.y1Label.Size = new System.Drawing.Size(20, 13);
            this.y1Label.TabIndex = 12;
            this.y1Label.Text = "Y1";
            this.y1Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // z1Label
            // 
            this.z1Label.AutoSize = true;
            this.z1Label.Location = new System.Drawing.Point(248, 46);
            this.z1Label.Name = "z1Label";
            this.z1Label.Size = new System.Drawing.Size(20, 13);
            this.z1Label.TabIndex = 11;
            this.z1Label.Text = "Z1";
            this.z1Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // z2Label
            // 
            this.z2Label.AutoSize = true;
            this.z2Label.Location = new System.Drawing.Point(248, 72);
            this.z2Label.Name = "z2Label";
            this.z2Label.Size = new System.Drawing.Size(20, 13);
            this.z2Label.TabIndex = 10;
            this.z2Label.Text = "Z2";
            this.z2Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // y2Label
            // 
            this.y2Label.AutoSize = true;
            this.y2Label.Location = new System.Drawing.Point(132, 72);
            this.y2Label.Name = "y2Label";
            this.y2Label.Size = new System.Drawing.Size(20, 13);
            this.y2Label.TabIndex = 9;
            this.y2Label.Text = "Y2";
            this.y2Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // x2Label
            // 
            this.x2Label.AutoSize = true;
            this.x2Label.Location = new System.Drawing.Point(6, 72);
            this.x2Label.Name = "x2Label";
            this.x2Label.Size = new System.Drawing.Size(20, 13);
            this.x2Label.TabIndex = 8;
            this.x2Label.Text = "X2";
            this.x2Label.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // z2Textbox
            // 
            this.z2Textbox.Location = new System.Drawing.Point(274, 69);
            this.z2Textbox.Name = "z2Textbox";
            this.z2Textbox.Size = new System.Drawing.Size(70, 20);
            this.z2Textbox.TabIndex = 7;
            this.z2Textbox.TextChanged += new System.EventHandler(this.z2Textbox_TextChanged);
            // 
            // y2Textbox
            // 
            this.y2Textbox.Location = new System.Drawing.Point(160, 69);
            this.y2Textbox.Name = "y2Textbox";
            this.y2Textbox.Size = new System.Drawing.Size(70, 20);
            this.y2Textbox.TabIndex = 6;
            this.y2Textbox.TextChanged += new System.EventHandler(this.y2Textbox_TextChanged);
            // 
            // x2Textbox
            // 
            this.x2Textbox.Location = new System.Drawing.Point(37, 69);
            this.x2Textbox.Name = "x2Textbox";
            this.x2Textbox.Size = new System.Drawing.Size(70, 20);
            this.x2Textbox.TabIndex = 5;
            this.x2Textbox.TextChanged += new System.EventHandler(this.x2Textbox_TextChanged);
            // 
            // z1Textbox
            // 
            this.z1Textbox.Location = new System.Drawing.Point(274, 43);
            this.z1Textbox.Name = "z1Textbox";
            this.z1Textbox.Size = new System.Drawing.Size(70, 20);
            this.z1Textbox.TabIndex = 4;
            this.z1Textbox.TextChanged += new System.EventHandler(this.z1Textbox_TextChanged);
            // 
            // y1Textbox
            // 
            this.y1Textbox.Location = new System.Drawing.Point(160, 43);
            this.y1Textbox.Name = "y1Textbox";
            this.y1Textbox.Size = new System.Drawing.Size(70, 20);
            this.y1Textbox.TabIndex = 3;
            this.y1Textbox.TextChanged += new System.EventHandler(this.y1Textbox_TextChanged);
            // 
            // x1Textbox
            // 
            this.x1Textbox.Location = new System.Drawing.Point(37, 43);
            this.x1Textbox.Name = "x1Textbox";
            this.x1Textbox.Size = new System.Drawing.Size(70, 20);
            this.x1Textbox.TabIndex = 2;
            this.x1Textbox.TextChanged += new System.EventHandler(this.x1Textbox_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(71, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Position Type";
            // 
            // positionTypeComboBox
            // 
            this.positionTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.positionTypeComboBox.FormattingEnabled = true;
            this.positionTypeComboBox.Location = new System.Drawing.Point(148, 16);
            this.positionTypeComboBox.Name = "positionTypeComboBox";
            this.positionTypeComboBox.Size = new System.Drawing.Size(132, 21);
            this.positionTypeComboBox.TabIndex = 0;
            this.positionTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.positionTypeComboBox_SelectedIndexChanged);
            // 
            // eomGroup
            // 
            this.eomGroup.Controls.Add(this.evalFcnLabel);
            this.eomGroup.Controls.Add(this.initFcnLabel);
            this.eomGroup.Controls.Add(this.evalFuncTextbox);
            this.eomGroup.Controls.Add(this.initFuncTextbox);
            this.eomGroup.Controls.Add(this.eomTypeLabel);
            this.eomGroup.Controls.Add(this.eomTypeComboBox);
            this.eomGroup.Location = new System.Drawing.Point(375, 12);
            this.eomGroup.Name = "eomGroup";
            this.eomGroup.Size = new System.Drawing.Size(263, 100);
            this.eomGroup.TabIndex = 1;
            this.eomGroup.TabStop = false;
            this.eomGroup.Text = "Equations of Motion";
            // 
            // evalFcnLabel
            // 
            this.evalFcnLabel.AutoSize = true;
            this.evalFcnLabel.Location = new System.Drawing.Point(13, 76);
            this.evalFcnLabel.Name = "evalFcnLabel";
            this.evalFcnLabel.Size = new System.Drawing.Size(101, 13);
            this.evalFcnLabel.TabIndex = 5;
            this.evalFcnLabel.Text = "Evaluation Function";
            // 
            // initFcnLabel
            // 
            this.initFcnLabel.AutoSize = true;
            this.initFcnLabel.Location = new System.Drawing.Point(9, 50);
            this.initFcnLabel.Name = "initFcnLabel";
            this.initFcnLabel.Size = new System.Drawing.Size(105, 13);
            this.initFcnLabel.TabIndex = 4;
            this.initFcnLabel.Text = "Initialization Function";
            // 
            // evalFuncTextbox
            // 
            this.evalFuncTextbox.Location = new System.Drawing.Point(120, 72);
            this.evalFuncTextbox.Name = "evalFuncTextbox";
            this.evalFuncTextbox.Size = new System.Drawing.Size(121, 20);
            this.evalFuncTextbox.TabIndex = 3;
            this.evalFuncTextbox.TextChanged += new System.EventHandler(this.evalFuncTextbox_TextChanged);
            // 
            // initFuncTextbox
            // 
            this.initFuncTextbox.Location = new System.Drawing.Point(120, 46);
            this.initFuncTextbox.Name = "initFuncTextbox";
            this.initFuncTextbox.Size = new System.Drawing.Size(121, 20);
            this.initFuncTextbox.TabIndex = 2;
            this.initFuncTextbox.TextChanged += new System.EventHandler(this.initFuncTextbox_TextChanged);
            // 
            // eomTypeLabel
            // 
            this.eomTypeLabel.AutoSize = true;
            this.eomTypeLabel.Location = new System.Drawing.Point(38, 24);
            this.eomTypeLabel.Name = "eomTypeLabel";
            this.eomTypeLabel.Size = new System.Drawing.Size(76, 13);
            this.eomTypeLabel.TabIndex = 1;
            this.eomTypeLabel.Text = "Equation Type";
            // 
            // eomTypeComboBox
            // 
            this.eomTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.eomTypeComboBox.FormattingEnabled = true;
            this.eomTypeComboBox.Location = new System.Drawing.Point(120, 19);
            this.eomTypeComboBox.Name = "eomTypeComboBox";
            this.eomTypeComboBox.Size = new System.Drawing.Size(121, 21);
            this.eomTypeComboBox.TabIndex = 0;
            this.eomTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.eomTypeComboBox_SelectedIndexChanged);
            // 
            // drawingCanvas1
            // 
            this.drawingCanvas1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.drawingCanvas1.AutoScroll = true;
            this.drawingCanvas1.BackColor = System.Drawing.Color.White;
            this.drawingCanvas1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.drawingCanvas1.ForeColor = System.Drawing.SystemColors.Control;
            this.drawingCanvas1.Location = new System.Drawing.Point(12, 118);
            this.drawingCanvas1.Name = "drawingCanvas1";
            this.drawingCanvas1.Size = new System.Drawing.Size(626, 255);
            this.drawingCanvas1.TabIndex = 2;
            // 
            // AssetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(650, 385);
            this.Controls.Add(this.drawingCanvas1);
            this.Controls.Add(this.eomGroup);
            this.Controls.Add(this.positionGroup);
            this.Name = "AssetForm";
            this.Text = "AssetForm";
            this.positionGroup.ResumeLayout(false);
            this.positionGroup.PerformLayout();
            this.eomGroup.ResumeLayout(false);
            this.eomGroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox positionGroup;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox positionTypeComboBox;
        private System.Windows.Forms.Label x2Label;
        private System.Windows.Forms.TextBox z2Textbox;
        private System.Windows.Forms.TextBox y2Textbox;
        private System.Windows.Forms.TextBox x2Textbox;
        private System.Windows.Forms.TextBox z1Textbox;
        private System.Windows.Forms.TextBox y1Textbox;
        private System.Windows.Forms.TextBox x1Textbox;
        private System.Windows.Forms.Label x1Label;
        private System.Windows.Forms.Label y1Label;
        private System.Windows.Forms.Label z1Label;
        private System.Windows.Forms.Label z2Label;
        private System.Windows.Forms.Label y2Label;
        private System.Windows.Forms.GroupBox eomGroup;
        private System.Windows.Forms.Label eomTypeLabel;
        private System.Windows.Forms.ComboBox eomTypeComboBox;
        private System.Windows.Forms.Label evalFcnLabel;
        private System.Windows.Forms.Label initFcnLabel;
        private System.Windows.Forms.TextBox evalFuncTextbox;
        private System.Windows.Forms.TextBox initFuncTextbox;
        private Picasso.Controls.DrawingCanvas drawingCanvas1;
    }
}