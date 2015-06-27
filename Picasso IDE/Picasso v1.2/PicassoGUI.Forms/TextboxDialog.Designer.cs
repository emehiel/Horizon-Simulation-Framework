namespace PicassoGUI.Forms
{
    partial class TextboxDialog
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
            this.dialogLabel = new System.Windows.Forms.Label();
            this.dialogTextbox = new System.Windows.Forms.TextBox();
            this.returnButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // dialogLabel
            // 
            this.dialogLabel.AutoSize = true;
            this.dialogLabel.Location = new System.Drawing.Point(12, 9);
            this.dialogLabel.Name = "dialogLabel";
            this.dialogLabel.Size = new System.Drawing.Size(59, 13);
            this.dialogLabel.TabIndex = 0;
            this.dialogLabel.Text = "Enter Text:";
            // 
            // dialogTextbox
            // 
            this.dialogTextbox.Location = new System.Drawing.Point(129, 6);
            this.dialogTextbox.Name = "dialogTextbox";
            this.dialogTextbox.Size = new System.Drawing.Size(120, 20);
            this.dialogTextbox.TabIndex = 1;
            // 
            // returnButton
            // 
            this.returnButton.Location = new System.Drawing.Point(255, 4);
            this.returnButton.Name = "returnButton";
            this.returnButton.Size = new System.Drawing.Size(75, 23);
            this.returnButton.TabIndex = 2;
            this.returnButton.Text = "Go";
            this.returnButton.UseVisualStyleBackColor = true;
            this.returnButton.Click += new System.EventHandler(this.returnButton_Click);
            // 
            // TextboxDialog
            // 
            this.AcceptButton = this.returnButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 34);
            this.Controls.Add(this.returnButton);
            this.Controls.Add(this.dialogTextbox);
            this.Controls.Add(this.dialogLabel);
            this.Name = "TextboxDialog";
            this.Text = "TextboxDialog";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label dialogLabel;
        private System.Windows.Forms.TextBox dialogTextbox;
        private System.Windows.Forms.Button returnButton;
    }
}