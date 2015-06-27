namespace Picasso.Results
{
    partial class StatePlot
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.stateNameLabel = new System.Windows.Forms.Label();
            this.interpolationButton = new System.Windows.Forms.PictureBox();
            this.closeButton = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.interpolationButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.closeButton)).BeginInit();
            this.SuspendLayout();
            // 
            // stateNameLabel
            // 
            this.stateNameLabel.AutoSize = true;
            this.stateNameLabel.ForeColor = System.Drawing.Color.Yellow;
            this.stateNameLabel.Location = new System.Drawing.Point(3, 3);
            this.stateNameLabel.Name = "stateNameLabel";
            this.stateNameLabel.Size = new System.Drawing.Size(35, 13);
            this.stateNameLabel.TabIndex = 1;
            this.stateNameLabel.Text = "label1";
            // 
            // interpolationButton
            // 
            this.interpolationButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonSpline;
            this.interpolationButton.Location = new System.Drawing.Point(359, 3);
            this.interpolationButton.Name = "interpolationButton";
            this.interpolationButton.Size = new System.Drawing.Size(16, 16);
            this.interpolationButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.interpolationButton.TabIndex = 2;
            this.interpolationButton.TabStop = false;
            this.interpolationButton.MouseLeave += new System.EventHandler(this.interpolationButton_MouseLeave);
            this.interpolationButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.interpolationButton_MouseClick);
            this.interpolationButton.MouseEnter += new System.EventHandler(this.interpolationButton_MouseEnter);
            // 
            // closeButton
            // 
            this.closeButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.closeButton.Image = global::Picasso.Results.Properties.Resources.Close_Button;
            this.closeButton.Location = new System.Drawing.Point(381, 3);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(16, 16);
            this.closeButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.closeButton.TabIndex = 0;
            this.closeButton.TabStop = false;
            this.closeButton.MouseLeave += new System.EventHandler(this.closeButton_MouseLeave);
            this.closeButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.closeButton_MouseClick);
            this.closeButton.MouseEnter += new System.EventHandler(this.closeButton_MouseEnter);
            // 
            // StatePlot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.Controls.Add(this.interpolationButton);
            this.Controls.Add(this.stateNameLabel);
            this.Controls.Add(this.closeButton);
            this.Name = "StatePlot";
            this.Size = new System.Drawing.Size(400, 100);
            ((System.ComponentModel.ISupportInitialize)(this.interpolationButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.closeButton)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox closeButton;
        private System.Windows.Forms.Label stateNameLabel;
        private System.Windows.Forms.PictureBox interpolationButton;
    }
}
