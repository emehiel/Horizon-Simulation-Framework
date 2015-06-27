namespace PicassoGUI.Components
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
            this.typeComboBox = new System.Windows.Forms.ComboBox();
            this.positionTypeLabel = new System.Windows.Forms.Label();
            this.xBox = new System.Windows.Forms.TextBox();
            this.yBox = new System.Windows.Forms.TextBox();
            this.zBox = new System.Windows.Forms.TextBox();
            this.xLabel = new System.Windows.Forms.Label();
            this.yLabel = new System.Windows.Forms.Label();
            this.zLabel = new System.Windows.Forms.Label();
            this.positionGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // positionGroup
            // 
            this.positionGroup.Controls.Add(this.zLabel);
            this.positionGroup.Controls.Add(this.yLabel);
            this.positionGroup.Controls.Add(this.xLabel);
            this.positionGroup.Controls.Add(this.zBox);
            this.positionGroup.Controls.Add(this.yBox);
            this.positionGroup.Controls.Add(this.xBox);
            this.positionGroup.Controls.Add(this.typeComboBox);
            this.positionGroup.Controls.Add(this.positionTypeLabel);
            this.positionGroup.Location = new System.Drawing.Point(12, 12);
            this.positionGroup.Name = "positionGroup";
            this.positionGroup.Size = new System.Drawing.Size(318, 134);
            this.positionGroup.TabIndex = 0;
            this.positionGroup.TabStop = false;
            this.positionGroup.Text = "Target Position";
            // 
            // typeComboBox
            // 
            this.typeComboBox.FormattingEnabled = true;
            this.typeComboBox.Location = new System.Drawing.Point(148, 22);
            this.typeComboBox.Name = "typeComboBox";
            this.typeComboBox.Size = new System.Drawing.Size(121, 21);
            this.typeComboBox.TabIndex = 1;
            this.typeComboBox.SelectedValueChanged += new System.EventHandler(this.typeComboBox_SelectedValueChanged);
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
            // xBox
            // 
            this.xBox.Location = new System.Drawing.Point(148, 49);
            this.xBox.Name = "xBox";
            this.xBox.Size = new System.Drawing.Size(100, 20);
            this.xBox.TabIndex = 2;
            this.xBox.TextChanged += new System.EventHandler(this.xBox_TextChanged);
            // 
            // yBox
            // 
            this.yBox.Location = new System.Drawing.Point(148, 75);
            this.yBox.Name = "yBox";
            this.yBox.Size = new System.Drawing.Size(100, 20);
            this.yBox.TabIndex = 3;
            // 
            // zBox
            // 
            this.zBox.Location = new System.Drawing.Point(148, 101);
            this.zBox.Name = "zBox";
            this.zBox.Size = new System.Drawing.Size(100, 20);
            this.zBox.TabIndex = 4;
            // 
            // xLabel
            // 
            this.xLabel.AutoSize = true;
            this.xLabel.Location = new System.Drawing.Point(71, 52);
            this.xLabel.Name = "xLabel";
            this.xLabel.Size = new System.Drawing.Size(14, 13);
            this.xLabel.TabIndex = 5;
            this.xLabel.Text = "X";
            // 
            // yLabel
            // 
            this.yLabel.AutoSize = true;
            this.yLabel.Location = new System.Drawing.Point(71, 78);
            this.yLabel.Name = "yLabel";
            this.yLabel.Size = new System.Drawing.Size(14, 13);
            this.yLabel.TabIndex = 6;
            this.yLabel.Text = "Y";
            // 
            // zLabel
            // 
            this.zLabel.AutoSize = true;
            this.zLabel.Location = new System.Drawing.Point(71, 104);
            this.zLabel.Name = "zLabel";
            this.zLabel.Size = new System.Drawing.Size(14, 13);
            this.zLabel.TabIndex = 7;
            this.zLabel.Text = "Z";
            // 
            // TargetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 158);
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
        private System.Windows.Forms.TextBox zBox;
        private System.Windows.Forms.TextBox yBox;
        private System.Windows.Forms.TextBox xBox;
    }
}