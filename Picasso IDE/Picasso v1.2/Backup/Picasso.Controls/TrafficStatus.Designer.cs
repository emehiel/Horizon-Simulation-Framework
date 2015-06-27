namespace Picasso.Controls
{
    partial class TrafficStatus
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
            this.stopButton = new TransparencyDemo.TransparentButton();
            this.yieldButton = new TransparencyDemo.TransparentButton();
            this.goButton = new TransparencyDemo.TransparentButton();
            ((System.ComponentModel.ISupportInitialize)(this.stopButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.yieldButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.goButton)).BeginInit();
            this.SuspendLayout();
            // 
            // stopButton
            // 
            this.stopButton.BackColor = System.Drawing.Color.Transparent;
            this.stopButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.stopButton.Enabled = false;
            this.stopButton.Image = global::Picasso.Controls.Properties.Resources.redButtonOff;
            this.stopButton.Location = new System.Drawing.Point(44, 0);
            this.stopButton.Name = "stopButton";
            this.stopButton.OverImage = global::Picasso.Controls.Properties.Resources.redButtonOn;
            this.stopButton.Size = new System.Drawing.Size(16, 16);
            this.stopButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.stopButton.TabIndex = 2;
            this.stopButton.TabStop = false;
            this.stopButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            // 
            // yieldButton
            // 
            this.yieldButton.BackColor = System.Drawing.Color.Transparent;
            this.yieldButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.yieldButton.Enabled = false;
            this.yieldButton.Image = global::Picasso.Controls.Properties.Resources.yellowButtonOff;
            this.yieldButton.Location = new System.Drawing.Point(22, 0);
            this.yieldButton.Name = "yieldButton";
            this.yieldButton.OverImage = global::Picasso.Controls.Properties.Resources.yellowButtonOn;
            this.yieldButton.Size = new System.Drawing.Size(16, 16);
            this.yieldButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.yieldButton.TabIndex = 1;
            this.yieldButton.TabStop = false;
            this.yieldButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            // 
            // goButton
            // 
            this.goButton.BackColor = System.Drawing.Color.Transparent;
            this.goButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.goButton.Enabled = false;
            this.goButton.Image = global::Picasso.Controls.Properties.Resources.greenButtonOff;
            this.goButton.Location = new System.Drawing.Point(0, 0);
            this.goButton.Name = "goButton";
            this.goButton.OverImage = global::Picasso.Controls.Properties.Resources.greenButtonOn;
            this.goButton.Size = new System.Drawing.Size(16, 16);
            this.goButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.goButton.TabIndex = 0;
            this.goButton.TabStop = false;
            this.goButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            // 
            // TrafficStatus
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.yieldButton);
            this.Controls.Add(this.goButton);
            this.Name = "TrafficStatus";
            this.Size = new System.Drawing.Size(61, 16);
            ((System.ComponentModel.ISupportInitialize)(this.stopButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.yieldButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.goButton)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TransparencyDemo.TransparentButton goButton;
        private TransparencyDemo.TransparentButton yieldButton;
        private TransparencyDemo.TransparentButton stopButton;
    }
}
