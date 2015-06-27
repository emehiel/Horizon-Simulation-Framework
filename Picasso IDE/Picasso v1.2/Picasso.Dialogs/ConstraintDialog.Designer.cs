namespace Picasso.Dialogs
{
    partial class ConstraintDialog
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
            this.parametersBox = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.valueLabel = new System.Windows.Forms.Label();
            this.typeCombo = new System.Windows.Forms.ComboBox();
            this.valueTextbox = new System.Windows.Forms.TextBox();
            this.variableGroup = new System.Windows.Forms.GroupBox();
            this.datatypeLabel = new System.Windows.Forms.Label();
            this.nameLabel = new System.Windows.Forms.Label();
            this.datatypeCombo = new System.Windows.Forms.ComboBox();
            this.nameTextbox = new System.Windows.Forms.TextBox();
            this.okButton = new System.Windows.Forms.Button();
            this.parametersBox.SuspendLayout();
            this.variableGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // parametersBox
            // 
            this.parametersBox.Controls.Add(this.label1);
            this.parametersBox.Controls.Add(this.valueLabel);
            this.parametersBox.Controls.Add(this.typeCombo);
            this.parametersBox.Controls.Add(this.valueTextbox);
            this.parametersBox.Location = new System.Drawing.Point(12, 12);
            this.parametersBox.Name = "parametersBox";
            this.parametersBox.Size = new System.Drawing.Size(212, 83);
            this.parametersBox.TabIndex = 0;
            this.parametersBox.TabStop = false;
            this.parametersBox.Text = "Parameters";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(33, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Type";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // valueLabel
            // 
            this.valueLabel.AutoSize = true;
            this.valueLabel.Location = new System.Drawing.Point(30, 22);
            this.valueLabel.Name = "valueLabel";
            this.valueLabel.Size = new System.Drawing.Size(34, 13);
            this.valueLabel.TabIndex = 2;
            this.valueLabel.Text = "Value";
            this.valueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // typeCombo
            // 
            this.typeCombo.FormattingEnabled = true;
            this.typeCombo.Location = new System.Drawing.Point(70, 50);
            this.typeCombo.Name = "typeCombo";
            this.typeCombo.Size = new System.Drawing.Size(101, 21);
            this.typeCombo.TabIndex = 1;
            // 
            // valueTextbox
            // 
            this.valueTextbox.Location = new System.Drawing.Point(70, 19);
            this.valueTextbox.Name = "valueTextbox";
            this.valueTextbox.Size = new System.Drawing.Size(101, 20);
            this.valueTextbox.TabIndex = 0;
            // 
            // variableGroup
            // 
            this.variableGroup.Controls.Add(this.datatypeLabel);
            this.variableGroup.Controls.Add(this.nameLabel);
            this.variableGroup.Controls.Add(this.datatypeCombo);
            this.variableGroup.Controls.Add(this.nameTextbox);
            this.variableGroup.Location = new System.Drawing.Point(12, 101);
            this.variableGroup.Name = "variableGroup";
            this.variableGroup.Size = new System.Drawing.Size(212, 88);
            this.variableGroup.TabIndex = 1;
            this.variableGroup.TabStop = false;
            this.variableGroup.Text = "Variable";
            // 
            // datatypeLabel
            // 
            this.datatypeLabel.AutoSize = true;
            this.datatypeLabel.Location = new System.Drawing.Point(7, 55);
            this.datatypeLabel.Name = "datatypeLabel";
            this.datatypeLabel.Size = new System.Drawing.Size(57, 13);
            this.datatypeLabel.TabIndex = 3;
            this.datatypeLabel.Text = "Data Type";
            this.datatypeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // nameLabel
            // 
            this.nameLabel.AutoSize = true;
            this.nameLabel.Location = new System.Drawing.Point(29, 22);
            this.nameLabel.Name = "nameLabel";
            this.nameLabel.Size = new System.Drawing.Size(35, 13);
            this.nameLabel.TabIndex = 2;
            this.nameLabel.Text = "Name";
            this.nameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // datatypeCombo
            // 
            this.datatypeCombo.FormattingEnabled = true;
            this.datatypeCombo.Location = new System.Drawing.Point(70, 52);
            this.datatypeCombo.Name = "datatypeCombo";
            this.datatypeCombo.Size = new System.Drawing.Size(101, 21);
            this.datatypeCombo.TabIndex = 1;
            // 
            // nameTextbox
            // 
            this.nameTextbox.Location = new System.Drawing.Point(70, 19);
            this.nameTextbox.Name = "nameTextbox";
            this.nameTextbox.Size = new System.Drawing.Size(101, 20);
            this.nameTextbox.TabIndex = 0;
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(149, 199);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // ConstraintDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 231);
            this.ControlBox = false;
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.variableGroup);
            this.Controls.Add(this.parametersBox);
            this.Name = "ConstraintDialog";
            this.Text = "ConstraintForm";
            this.parametersBox.ResumeLayout(false);
            this.parametersBox.PerformLayout();
            this.variableGroup.ResumeLayout(false);
            this.variableGroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox parametersBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label valueLabel;
        private System.Windows.Forms.ComboBox typeCombo;
        private System.Windows.Forms.TextBox valueTextbox;
        private System.Windows.Forms.GroupBox variableGroup;
        private System.Windows.Forms.Label datatypeLabel;
        private System.Windows.Forms.Label nameLabel;
        private System.Windows.Forms.ComboBox datatypeCombo;
        private System.Windows.Forms.TextBox nameTextbox;
        private System.Windows.Forms.Button okButton;

    }
}