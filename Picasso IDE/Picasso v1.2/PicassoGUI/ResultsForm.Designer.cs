namespace PicassoGUI
{
    partial class ResultsForm
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
            this.dxViewer = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.dxViewer)).BeginInit();
            this.SuspendLayout();
            // 
            // dxViewer
            // 
            this.dxViewer.BackColor = System.Drawing.Color.Black;
            this.dxViewer.Location = new System.Drawing.Point(12, 12);
            this.dxViewer.Name = "dxViewer";
            this.dxViewer.Size = new System.Drawing.Size(268, 245);
            this.dxViewer.TabIndex = 0;
            this.dxViewer.TabStop = false;
            // 
            // ResultsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 269);
            this.Controls.Add(this.dxViewer);
            this.Name = "ResultsForm";
            this.Text = "ResultsForm";
            ((System.ComponentModel.ISupportInitialize)(this.dxViewer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox dxViewer;
    }
}