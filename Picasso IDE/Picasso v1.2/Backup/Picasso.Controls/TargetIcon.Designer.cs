﻿namespace Picasso.Controls
{
    partial class TargetIcon
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
            // TargetIcon
            // 
            this.Image = global::Picasso.Controls.Properties.Resources.target;
            this.InitialImage = null;
            this.Size = new System.Drawing.Size(15, 15);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.TargetIcon_MouseMove);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.TargetIcon_MouseDoubleClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.TargetIcon_MouseDown);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.TargetIcon_MouseUp);
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
    }
}