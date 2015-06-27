namespace Picasso.Forms
{
    partial class TargetdeckForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TargetdeckForm));
            this.mapStatus = new System.Windows.Forms.StatusStrip();
            this.cursorStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.worldMap = new Picasso.Controls.WorldMap();
            this.zoomInButton = new System.Windows.Forms.PictureBox();
            this.zoomOutButton = new System.Windows.Forms.PictureBox();
            this.mapStatus.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.worldMap)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.zoomInButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.zoomOutButton)).BeginInit();
            this.SuspendLayout();
            // 
            // mapStatus
            // 
            this.mapStatus.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cursorStatus});
            this.mapStatus.Location = new System.Drawing.Point(0, 328);
            this.mapStatus.Name = "mapStatus";
            this.mapStatus.Size = new System.Drawing.Size(633, 22);
            this.mapStatus.TabIndex = 1;
            this.mapStatus.Text = "Cursor Position:";
            // 
            // cursorStatus
            // 
            this.cursorStatus.Name = "cursorStatus";
            this.cursorStatus.Size = new System.Drawing.Size(83, 17);
            this.cursorStatus.Text = "Cursor Position:";
            // 
            // worldMap
            // 
            this.worldMap.Cursor = System.Windows.Forms.Cursors.Hand;
            this.worldMap.Image = ((System.Drawing.Image)(resources.GetObject("worldMap.Image")));
            this.worldMap.InitialImage = null;
            this.worldMap.Location = new System.Drawing.Point(0, 0);
            this.worldMap.Name = "worldMap";
            this.worldMap.Size = new System.Drawing.Size(2048, 1024);
            this.worldMap.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.worldMap.TabIndex = 2;
            this.worldMap.TabStop = false;
            this.worldMap.ZoomFactor = 1;
            this.worldMap.MouseMove += new System.Windows.Forms.MouseEventHandler(this.worldMap_MouseMove_1);
            this.worldMap.MouseUp += new System.Windows.Forms.MouseEventHandler(this.worldMap_MouseUp);
            // 
            // zoomInButton
            // 
            this.zoomInButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.zoomInButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_In;
            this.zoomInButton.Location = new System.Drawing.Point(12, 248);
            this.zoomInButton.Name = "zoomInButton";
            this.zoomInButton.Size = new System.Drawing.Size(64, 32);
            this.zoomInButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.zoomInButton.TabIndex = 3;
            this.zoomInButton.TabStop = false;
            this.zoomInButton.MouseLeave += new System.EventHandler(this.zoomInButton_MouseLeave);
            this.zoomInButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.zoomInButton_MouseClick);
            this.zoomInButton.MouseEnter += new System.EventHandler(this.zoomInButton_MouseEnter);
            // 
            // zoomOutButton
            // 
            this.zoomOutButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.zoomOutButton.Image = global::Picasso.Forms.Properties.Resources.Zoom_Out;
            this.zoomOutButton.Location = new System.Drawing.Point(12, 281);
            this.zoomOutButton.Name = "zoomOutButton";
            this.zoomOutButton.Size = new System.Drawing.Size(64, 32);
            this.zoomOutButton.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.zoomOutButton.TabIndex = 4;
            this.zoomOutButton.TabStop = false;
            this.zoomOutButton.MouseLeave += new System.EventHandler(this.zoomOutButton_MouseLeave);
            this.zoomOutButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.zoomOutButton_MouseClick);
            this.zoomOutButton.MouseEnter += new System.EventHandler(this.zoomOutButton_MouseEnter);
            // 
            // TargetdeckForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(633, 350);
            this.Controls.Add(this.zoomOutButton);
            this.Controls.Add(this.zoomInButton);
            this.Controls.Add(this.mapStatus);
            this.Controls.Add(this.worldMap);
            this.Name = "TargetdeckForm";
            this.Text = "TargetdeckForm";
            this.SizeChanged += new System.EventHandler(this.TargetdeckForm_SizeChanged);
            this.Resize += new System.EventHandler(this.TargetdeckForm_Resize);
            this.ResizeEnd += new System.EventHandler(this.TargetdeckForm_ResizeEnd);
            this.mapStatus.ResumeLayout(false);
            this.mapStatus.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.worldMap)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.zoomInButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.zoomOutButton)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip mapStatus;
        private System.Windows.Forms.ToolStripStatusLabel cursorStatus;
        private Picasso.Controls.WorldMap worldMap;
        private System.Windows.Forms.PictureBox zoomInButton;
        private System.Windows.Forms.PictureBox zoomOutButton;
    }
}