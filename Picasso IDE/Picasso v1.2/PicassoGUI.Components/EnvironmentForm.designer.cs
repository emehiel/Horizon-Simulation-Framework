namespace PicassoGUI.Components
{
    partial class EnvironmentForm
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
            this.sunBox = new System.Windows.Forms.GroupBox();
            this.constSunCheck = new System.Windows.Forms.CheckBox();
            this.sunBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // sunBox
            // 
            this.sunBox.Controls.Add(this.constSunCheck);
            this.sunBox.Location = new System.Drawing.Point(12, 12);
            this.sunBox.Name = "sunBox";
            this.sunBox.Size = new System.Drawing.Size(318, 50);
            this.sunBox.TabIndex = 0;
            this.sunBox.TabStop = false;
            this.sunBox.Text = "Sun";
            // 
            // constSunCheck
            // 
            this.constSunCheck.AutoSize = true;
            this.constSunCheck.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.constSunCheck.Location = new System.Drawing.Point(100, 19);
            this.constSunCheck.Name = "constSunCheck";
            this.constSunCheck.Size = new System.Drawing.Size(124, 17);
            this.constSunCheck.TabIndex = 0;
            this.constSunCheck.Text = "Constant Sun Vector";
            this.constSunCheck.UseVisualStyleBackColor = true;
            // 
            // EnvironmentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 76);
            this.Controls.Add(this.sunBox);
            this.Name = "EnvironmentForm";
            this.Text = "EnvironmentForm";
            this.sunBox.ResumeLayout(false);
            this.sunBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox sunBox;
        private System.Windows.Forms.CheckBox constSunCheck;
    }
}