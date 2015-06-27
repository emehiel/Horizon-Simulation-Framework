namespace PicassoGUI.Components
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
            this.mapStatus = new System.Windows.Forms.StatusStrip();
            this.cursorStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.worldMap = new System.Windows.Forms.PictureBox();
            this.mapStatus.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.worldMap)).BeginInit();
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
            this.worldMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.worldMap.Image = global::PicassoGUI.Components.Properties.Resources.land_shallow_topo_2048;
            this.worldMap.Location = new System.Drawing.Point(0, 0);
            this.worldMap.Name = "worldMap";
            this.worldMap.Size = new System.Drawing.Size(633, 350);
            this.worldMap.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.worldMap.TabIndex = 0;
            this.worldMap.TabStop = false;
            this.worldMap.MouseMove += new System.Windows.Forms.MouseEventHandler(this.worldMap_MouseMove);
            this.worldMap.MouseClick += new System.Windows.Forms.MouseEventHandler(this.worldMap_MouseClick);
            // 
            // TargetdeckForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(633, 350);
            this.Controls.Add(this.mapStatus);
            this.Controls.Add(this.worldMap);
            this.Name = "TargetdeckForm";
            this.Text = "TargetdeckForm";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.TargetdeckForm_MouseUp);
            this.SizeChanged += new System.EventHandler(this.TargetdeckForm_SizeChanged);
            this.Resize += new System.EventHandler(this.TargetdeckForm_Resize);
            this.ResizeEnd += new System.EventHandler(this.TargetdeckForm_ResizeEnd);
            this.mapStatus.ResumeLayout(false);
            this.mapStatus.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.worldMap)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox worldMap;
        private System.Windows.Forms.StatusStrip mapStatus;
        private System.Windows.Forms.ToolStripStatusLabel cursorStatus;
    }
}