namespace Picasso.Results
{
    partial class GroundTrack
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
            this.slideBar1 = new Picasso.Controls.SlideBar();
            this.speedButton = new TransparencyDemo.TransparentButton();
            this.timeLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.speedButton)).BeginInit();
            this.SuspendLayout();
            // 
            // slideBar1
            // 
            this.slideBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.slideBar1.BackColor = System.Drawing.Color.Transparent;
            this.slideBar1.Location = new System.Drawing.Point(50, 240);
            this.slideBar1.Name = "slideBar1";
            this.slideBar1.Size = new System.Drawing.Size(481, 17);
            this.slideBar1.TabIndex = 1;
            this.slideBar1.Value = 0.1F;
            // 
            // speedButton
            // 
            this.speedButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.speedButton.BackColor = System.Drawing.Color.Transparent;
            this.speedButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.speedButton.Image = global::Picasso.Results.Properties.Resources.speed0x;
            this.speedButton.Location = new System.Drawing.Point(12, 241);
            this.speedButton.Name = "speedButton";
            this.speedButton.OverImage = null;
            this.speedButton.Size = new System.Drawing.Size(32, 16);
            this.speedButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.speedButton.TabIndex = 0;
            this.speedButton.TabStop = false;
            this.speedButton.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            this.speedButton.Click += new System.EventHandler(this.speedButton_Click);
            // 
            // timeLabel
            // 
            this.timeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.timeLabel.AutoSize = true;
            this.timeLabel.BackColor = System.Drawing.Color.Transparent;
            this.timeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.timeLabel.ForeColor = System.Drawing.Color.LightGreen;
            this.timeLabel.Location = new System.Drawing.Point(537, 240);
            this.timeLabel.Name = "timeLabel";
            this.timeLabel.Size = new System.Drawing.Size(43, 17);
            this.timeLabel.TabIndex = 2;
            this.timeLabel.Text = "Time";
            // 
            // GroundTrack
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::Picasso.Results.Properties.Resources.land_shallow_topo_1024;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(592, 269);
            this.Controls.Add(this.timeLabel);
            this.Controls.Add(this.slideBar1);
            this.Controls.Add(this.speedButton);
            this.Name = "GroundTrack";
            this.Text = "GroundTrack";
            ((System.ComponentModel.ISupportInitialize)(this.speedButton)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TransparencyDemo.TransparentButton speedButton;
        private Picasso.Controls.SlideBar slideBar1;
        private System.Windows.Forms.Label timeLabel;
    }
}