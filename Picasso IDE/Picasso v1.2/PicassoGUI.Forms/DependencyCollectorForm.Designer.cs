namespace PicassoGUI
{
    partial class DependencyCollectorForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.depComTextBox = new System.Windows.Forms.TextBox();
            this.depFcnCheckBox = new System.Windows.Forms.CheckBox();
            this.depFcnRichTextBox = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(129, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Dependency Combination";
            // 
            // depComTextBox
            // 
            this.depComTextBox.Location = new System.Drawing.Point(12, 29);
            this.depComTextBox.Name = "depComTextBox";
            this.depComTextBox.Size = new System.Drawing.Size(126, 20);
            this.depComTextBox.TabIndex = 1;
            this.depComTextBox.Text = "++";
            this.depComTextBox.TextChanged += new System.EventHandler(this.depComTextBox_TextChanged);
            // 
            // depFcnCheckBox
            // 
            this.depFcnCheckBox.AutoSize = true;
            this.depFcnCheckBox.Location = new System.Drawing.Point(12, 68);
            this.depFcnCheckBox.Name = "depFcnCheckBox";
            this.depFcnCheckBox.Size = new System.Drawing.Size(252, 17);
            this.depFcnCheckBox.TabIndex = 2;
            this.depFcnCheckBox.Text = "Use Custom Dependency Combination Function";
            this.depFcnCheckBox.UseVisualStyleBackColor = true;
            // 
            // depFcnRichTextBox
            // 
            this.depFcnRichTextBox.ImeMode = System.Windows.Forms.ImeMode.On;
            this.depFcnRichTextBox.Location = new System.Drawing.Point(12, 91);
            this.depFcnRichTextBox.Name = "depFcnRichTextBox";
            this.depFcnRichTextBox.Size = new System.Drawing.Size(398, 152);
            this.depFcnRichTextBox.TabIndex = 3;
            this.depFcnRichTextBox.Text = "";
            // 
            // DependencyCollectorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(426, 255);
            this.Controls.Add(this.depFcnRichTextBox);
            this.Controls.Add(this.depFcnCheckBox);
            this.Controls.Add(this.depComTextBox);
            this.Controls.Add(this.label1);
            this.Name = "DependencyCollectorForm";
            this.Text = "DependencyCollectorForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox depComTextBox;
        private System.Windows.Forms.CheckBox depFcnCheckBox;
        private System.Windows.Forms.RichTextBox depFcnRichTextBox;
    }
}