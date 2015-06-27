﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicassoGUI.Forms
{
    public partial class PicassoSettings : Form
    {
        private string _initRunner;
        private string _initWd;
        private string _initArgs;
        private bool _initAuto;

        public string Runner
        {
            get { return runnerTextbox.Text; }
        }

        public string WorkingDirectory
        {
            get { return wdTextbox.Text; }
        }

        public string Arguments
        {
            get { return argsTextbox.Text; }
        }

        public bool AutoArgs
        {
            get { return generatedCheckbox.Checked; }
        }

        public PicassoSettings()
        {
            InitializeComponent();
            runnerTextbox.Text = "C:\\Horizon\\horizon.exe";
            wdTextbox.Text = "C:\\Horizon\\";
            argsTextbox.Text = "io/DSAC.xml io/300targets.xml io";
            generatedCheckbox.Checked = false;
            _initRunner = runnerTextbox.Text;
            _initWd = wdTextbox.Text;
            _initArgs = argsTextbox.Text;
            _initAuto = generatedCheckbox.Checked;
        }

        public PicassoSettings(string runner, string wd, string args)
        {
            InitializeComponent();
            runnerTextbox.Text = runner;
            wdTextbox.Text = wd;
            argsTextbox.Text = args;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Close dialog, but do not destroy
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
            runnerTextbox.Text = _initRunner;
            wdTextbox.Text = _initWd;
            argsTextbox.Text = _initArgs;
            generatedCheckbox.Checked = _initAuto;
        }
}
}
