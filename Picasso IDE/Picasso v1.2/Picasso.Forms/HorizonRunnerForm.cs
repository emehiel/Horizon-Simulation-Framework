using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace Picasso.Forms
{
    public delegate void CallbackDelegate();

    public enum RunnerStatus
    {
        Stopped,
        Running,
        FinishedSuccess,
        FinishedCancelled
    };

    public partial class HorizonRunnerForm : Form
    {
        private StreamReader reader;
        private bool cancelled = false;
        public RunnerStatus Status;

        public CallbackDelegate OnSuccess;
        public CallbackDelegate OnFailure;

        public String FileName
        {
            get { return this.process1.StartInfo.FileName; }
            set { this.process1.StartInfo.FileName = value; }
        }
        public String WorkingDirectory
        {
            get { return this.process1.StartInfo.WorkingDirectory; }
            set { this.process1.StartInfo.WorkingDirectory = value; }
        }
        public String Arguments
        {
            get { return this.process1.StartInfo.Arguments; }
            set { this.process1.StartInfo.Arguments = value; }
        }

        public HorizonRunnerForm()
        {
            InitializeComponent();
            Status = RunnerStatus.Stopped;
            OnSuccess = new CallbackDelegate(DoNothing);
            OnFailure = new CallbackDelegate(DoNothing);
        }

        public void DoNothing()
        {
            return;
        }

        private void HorizonRunnerForm_Load(object sender, EventArgs e)
        {
            this.textBox1.Text = "Launching Horizon.  Please stand by..." + Environment.NewLine;
            this.continueButton.Enabled = false;
            this.cancelButton.Enabled = true;

            this.backgroundWorker1.RunWorkerAsync();
            Status = RunnerStatus.Stopped;
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.backgroundWorker1.CancelAsync();
            this.cancelButton.Enabled = false;
            this.taskLabel.Text = "Terminating Simulation...";
            this.progressBar1.Value = 100;
            Status = RunnerStatus.FinishedCancelled;
        }

        private void continueButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            String text = "";
            this.process1.StartInfo.FileName = this.FileName;
            this.process1.StartInfo.WorkingDirectory = this.WorkingDirectory;
            this.process1.StartInfo.Arguments = this.Arguments;
            this.process1.Start();

            this.reader = this.process1.StandardOutput;

            while (!this.process1.HasExited)
            {
                if (this.backgroundWorker1.CancellationPending)
                {
                    this.cancelled = true;
                    break;
                }
                text = reader.ReadLine();
                if (text != null)
                    this.backgroundWorker1.ReportProgress(20, text);                
            }
            this.process1.Close();
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            int s = 0;
            int l = 0;
            String text = e.UserState as String;
            Status = RunnerStatus.Running;

            if (text.Contains("Integrator State:"))
            {
                this.taskLabel.Text = "Propagating Assets...";
                s = "Integrator State:".Length;
                l = text.IndexOf("%") - s;
                this.progressBar1.Value = (int)Convert.ToDouble(text.Substring(s, l));
            }
            else if (text.Contains("Inserting Accesses:"))
            {
                this.taskLabel.Text = "Inserting Asset Tasks...";
                s = "Inserting Accesses:".Length;
                l = text.IndexOf("%") - s;
                this.progressBar1.Value = (int)Convert.ToDouble(text.Substring(s, l));
            }
            else if (text.Contains("Scheduler Status:"))
            {
                this.taskLabel.Text = "Scheduling Tasks...";
                s = "Scheduler Status:".Length;
                l = text.IndexOf("%") - s;
                this.progressBar1.Value = (int)Convert.ToDouble(text.Substring(s, l));
            }
            else if (text.Contains("DONE!"))
            {
                this.taskLabel.Text += "  Complete!";
            }
            else
                this.textBox1.AppendText(text + Environment.NewLine);

        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // Update labels and call proper delegates
            if (!this.cancelled && textBox1.Text.Contains("DONE!"))
            {
                this.taskLabel.Text = "Horizon Simulation Complete!";
                OnSuccess();
            }
            else if (this.cancelled)
            {
                this.taskLabel.Text = "Simulation Terminated per User Request!";
                OnFailure();
            }
            else
            {
                this.taskLabel.Text = "Simulation Failed";
                OnFailure();
            }

            // Disable buttons
            this.cancelButton.Enabled = false;
            this.continueButton.Enabled = true;

            // Update status
            Status = RunnerStatus.FinishedSuccess;
        }

    }
}
