namespace Picasso.Controls
{
    partial class SubsystemIcon
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
            this.Title = new System.Windows.Forms.Label();
            this.addConstraintButton = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // Title
            // 
            this.Title.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Title.BackColor = System.Drawing.Color.Transparent;
            this.Title.Location = new System.Drawing.Point(1, 1);
            this.Title.Name = "Title";
            this.Title.Padding = new System.Windows.Forms.Padding(2);
            this.Title.Size = new System.Drawing.Size(98, 58);
            this.Title.TabIndex = 0;
            this.Title.Text = "label1";
            this.Title.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // addConstraintButton
            // 
            this.addConstraintButton.AutoSize = true;
            this.addConstraintButton.BackColor = System.Drawing.Color.Transparent;
            this.addConstraintButton.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.addConstraintButton.Location = new System.Drawing.Point(75, 43);
            this.addConstraintButton.Name = "addConstraintButton";
            this.addConstraintButton.Size = new System.Drawing.Size(25, 13);
            this.addConstraintButton.TabIndex = 1;
            this.addConstraintButton.Text = "[+]";
            // 
            // SubsystemIcon
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.addConstraintButton);
            this.Controls.Add(this.Title);
            this.Name = "SubsystemIcon";
            this.Padding = new System.Windows.Forms.Padding(1);
            this.Size = new System.Drawing.Size(100, 60);
            this.MouseLeave += new System.EventHandler(this.SubsystemIcon_MouseLeave);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.SubsystemIcon_MouseMove);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.SubsystemIcon_MouseDoubleClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.SubsystemIcon_MouseDown);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.SubsystemIcon_MouseUp);
            this.MouseEnter += new System.EventHandler(this.SubsystemIcon_MouseEnter);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Title;
        private System.Windows.Forms.Label addConstraintButton;

    }
}
