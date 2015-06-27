namespace Picasso.Controls
{
    partial class WorldMap
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
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // WorldMap
            // 
            this.Cursor = System.Windows.Forms.Cursors.Hand;
            this.Image = global::Picasso.Controls.Properties.Resources.land_shallow_topo_2048;
            this.InitialImage = null;
            this.Size = new System.Drawing.Size(633, 350);
            this.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.WorldMap_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.WorldMap_MouseDown);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.WorldMap_MouseUp);
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
    }
}
