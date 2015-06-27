namespace Picasso.Results
{
    partial class ModelView
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
            this.glContext1 = new Picasso.Results.GlContext();
            this.SuspendLayout();
            // 
            // glContext1
            // 
            this.glContext1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.glContext1.Location = new System.Drawing.Point(0, 0);
            this.glContext1.Name = "glContext1";
            this.glContext1.Size = new System.Drawing.Size(492, 469);
            this.glContext1.TabIndex = 0;
            this.glContext1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseMove);
            this.glContext1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseDown);
            this.glContext1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseUp);
            // 
            // ModelView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(492, 469);
            this.Controls.Add(this.glContext1);
            this.Cursor = System.Windows.Forms.Cursors.NoMove2D;
            this.Name = "ModelView";
            this.Text = "ModelView";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseUp);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ModelView_MouseMove);
            this.ResumeLayout(false);

        }

        #endregion

        private GlContext glContext1;
    }
}