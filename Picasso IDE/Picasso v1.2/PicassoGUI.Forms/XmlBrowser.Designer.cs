namespace PicassoGUI.Forms
{
    partial class XmlBrowser
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
            this.xmlWindow = new System.Windows.Forms.WebBrowser();
            this.SuspendLayout();
            // 
            // xmlWindow
            // 
            this.xmlWindow.Dock = System.Windows.Forms.DockStyle.Fill;
            this.xmlWindow.Location = new System.Drawing.Point(0, 0);
            this.xmlWindow.MinimumSize = new System.Drawing.Size(20, 20);
            this.xmlWindow.Name = "xmlWindow";
            this.xmlWindow.Size = new System.Drawing.Size(574, 340);
            this.xmlWindow.TabIndex = 0;
            // 
            // XmlBrowser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(574, 340);
            this.Controls.Add(this.xmlWindow);
            this.Name = "XmlBrowser";
            this.Text = "XmlBrowser";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser xmlWindow;
    }
}