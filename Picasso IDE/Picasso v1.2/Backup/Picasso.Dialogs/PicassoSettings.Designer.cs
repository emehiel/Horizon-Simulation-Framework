namespace Picasso.Dialogs
{
    partial class PicassoSettings
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
            this.runnerTextbox = new System.Windows.Forms.TextBox();
            this.wdTextbox = new System.Windows.Forms.TextBox();
            this.argsTextbox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.generatedCheckbox = new System.Windows.Forms.CheckBox();
            this.authorTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // runnerTextbox
            // 
            this.runnerTextbox.Location = new System.Drawing.Point(110, 8);
            this.runnerTextbox.Name = "runnerTextbox";
            this.runnerTextbox.Size = new System.Drawing.Size(176, 20);
            this.runnerTextbox.TabIndex = 0;
            // 
            // wdTextbox
            // 
            this.wdTextbox.Location = new System.Drawing.Point(110, 34);
            this.wdTextbox.Name = "wdTextbox";
            this.wdTextbox.Size = new System.Drawing.Size(176, 20);
            this.wdTextbox.TabIndex = 1;
            // 
            // argsTextbox
            // 
            this.argsTextbox.Location = new System.Drawing.Point(110, 60);
            this.argsTextbox.Name = "argsTextbox";
            this.argsTextbox.Size = new System.Drawing.Size(176, 20);
            this.argsTextbox.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Horizon Runner";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Working Directory";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(47, 63);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Arguments";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(110, 135);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(81, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Save Settings";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(197, 135);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 7;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // generatedCheckbox
            // 
            this.generatedCheckbox.AutoSize = true;
            this.generatedCheckbox.Location = new System.Drawing.Point(110, 112);
            this.generatedCheckbox.Name = "generatedCheckbox";
            this.generatedCheckbox.Size = new System.Drawing.Size(129, 17);
            this.generatedCheckbox.TabIndex = 8;
            this.generatedCheckbox.Text = "Generated Arguments";
            this.generatedCheckbox.UseVisualStyleBackColor = true;
            // 
            // authorTextBox
            // 
            this.authorTextBox.Location = new System.Drawing.Point(110, 86);
            this.authorTextBox.Name = "authorTextBox";
            this.authorTextBox.Size = new System.Drawing.Size(176, 20);
            this.authorTextBox.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(66, 89);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Author";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // PicassoSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(298, 171);
            this.ControlBox = false;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.authorTextBox);
            this.Controls.Add(this.generatedCheckbox);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.argsTextbox);
            this.Controls.Add(this.wdTextbox);
            this.Controls.Add(this.runnerTextbox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "PicassoSettings";
            this.Text = "Picasso Settings";
            this.Shown += new System.EventHandler(this.PicassoSettings_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox runnerTextbox;
        private System.Windows.Forms.TextBox wdTextbox;
        private System.Windows.Forms.TextBox argsTextbox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.CheckBox generatedCheckbox;
        private System.Windows.Forms.TextBox authorTextBox;
        private System.Windows.Forms.Label label4;
    }
}