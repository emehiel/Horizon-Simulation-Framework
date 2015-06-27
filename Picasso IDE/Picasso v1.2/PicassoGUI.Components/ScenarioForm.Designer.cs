namespace PicassoGUI.Components
{
    partial class ScenarioForm
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
            this.SimStartJDControl = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.SimStartControl = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SimEndControl = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.SimStepControl = new System.Windows.Forms.TextBox();
            this.CropToControl = new System.Windows.Forms.TextBox();
            this.MaxSchedulesControl = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // SimStartJDControl
            // 
            this.SimStartJDControl.Location = new System.Drawing.Point(160, 22);
            this.SimStartJDControl.Name = "SimStartJDControl";
            this.SimStartJDControl.Size = new System.Drawing.Size(100, 20);
            this.SimStartJDControl.TabIndex = 1;
            this.SimStartJDControl.TextChanged += new System.EventHandler(this.SimStartJDControl_TextChanged);
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(49, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(105, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Reference Time (JD)";
            // 
            // SimStartControl
            // 
            this.SimStartControl.Location = new System.Drawing.Point(160, 48);
            this.SimStartControl.Name = "SimStartControl";
            this.SimStartControl.Size = new System.Drawing.Size(100, 20);
            this.SimStartControl.TabIndex = 3;
            this.SimStartControl.TextChanged += new System.EventHandler(this.SimStartControl_TextChanged);
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(99, 51);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Start Time";
            // 
            // SimEndControl
            // 
            this.SimEndControl.Location = new System.Drawing.Point(160, 74);
            this.SimEndControl.Name = "SimEndControl";
            this.SimEndControl.Size = new System.Drawing.Size(100, 20);
            this.SimEndControl.TabIndex = 5;
            this.SimEndControl.TextChanged += new System.EventHandler(this.SimEndControl_TextChanged);
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(102, 77);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "End Time";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.SimEndControl);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.SimStartJDControl);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.SimStartControl);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(317, 115);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Simulation Parameters";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.SimStepControl);
            this.groupBox2.Controls.Add(this.CropToControl);
            this.groupBox2.Controls.Add(this.MaxSchedulesControl);
            this.groupBox2.Location = new System.Drawing.Point(12, 133);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(317, 110);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Scheduler Parameters";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(102, 74);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(52, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Step Size";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(95, 48);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Crop Value";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(50, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Maximum Schedules";
            // 
            // SimStepControl
            // 
            this.SimStepControl.Location = new System.Drawing.Point(160, 71);
            this.SimStepControl.Name = "SimStepControl";
            this.SimStepControl.Size = new System.Drawing.Size(100, 20);
            this.SimStepControl.TabIndex = 2;
            this.SimStepControl.TextChanged += new System.EventHandler(this.SimStepControl_TextChanged);
            // 
            // CropToControl
            // 
            this.CropToControl.Location = new System.Drawing.Point(160, 45);
            this.CropToControl.Name = "CropToControl";
            this.CropToControl.Size = new System.Drawing.Size(100, 20);
            this.CropToControl.TabIndex = 1;
            this.CropToControl.TextChanged += new System.EventHandler(this.CropToControl_TextChanged);
            // 
            // MaxSchedulesControl
            // 
            this.MaxSchedulesControl.Location = new System.Drawing.Point(160, 19);
            this.MaxSchedulesControl.Name = "MaxSchedulesControl";
            this.MaxSchedulesControl.Size = new System.Drawing.Size(100, 20);
            this.MaxSchedulesControl.TabIndex = 0;
            this.MaxSchedulesControl.TextChanged += new System.EventHandler(this.MaxSchedulesControl_TextChanged);
            // 
            // ScenarioForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 259);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "ScenarioForm";
            this.Text = "ScenarioForm";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox SimStartJDControl;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox SimStartControl;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox SimEndControl;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox MaxSchedulesControl;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox SimStepControl;
        private System.Windows.Forms.TextBox CropToControl;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
    }
}