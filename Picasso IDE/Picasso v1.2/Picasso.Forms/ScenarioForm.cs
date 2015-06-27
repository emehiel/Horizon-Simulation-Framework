using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Elements;
using Hsf.Components;
using MementoModel;

namespace Picasso.Forms
{
    public partial class ScenarioForm : PicassoSubform, IMForm
    {
        private MementoManager _mManager;
        public ScenarioComponent Scenario;
        public bool IsSaved;
        public string FileTarget;
        public double Version;

        public override HsfElement Element
        {
            get { return Scenario; }
        }

        public ScenarioForm(Form parentWindow)
        {
            // Creates a new scenario form with a new scenario component
            InitializeComponent();
            Scenario = new ScenarioComponent();
            MdiParent = parentWindow;
            Tag = Scenario;
            Text = Scenario.ScenarioName + " (Scenario)";
            _mManager = new MementoManager((IMForm)parentWindow);
            RefreshValues();
            IsSaved = true;
            Version = 0.1;
            FileTarget = "";
        }

        public ScenarioForm(ScenarioComponent targetComponent, Form parentWindow)
        {
            // Creates a new scenario form based on the given scenario component
            InitializeComponent();
            MdiParent = parentWindow;
            Tag = targetComponent;
            Scenario = targetComponent;
            Text = Scenario.ScenarioName + " (Scenario)";
            _mManager = new MementoManager((IMForm)parentWindow);
            RefreshValues();
            IsSaved = true;
            FileTarget = "";
        }

        public override void RefreshValues()
        {
            // Updates form values from component values
            SimStartJDControl.Text = Scenario.SimParams.SimStartJd.ToString();
            SimStartControl.Text = Scenario.SimParams.SimStart.ToString();
            SimEndControl.Text = Scenario.SimParams.SimEnd.ToString();
            MaxSchedulesControl.Text = Scenario.SchParams.MaxSchedules.ToString();
            CropToControl.Text = Scenario.SchParams.CropTo.ToString();
            SimStepControl.Text = Scenario.SchParams.SimStep.ToString();
        }

        #region Events

        // Note: All events that change values should register event with memento manager
        private void SimStartJDControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SimParams.SimStartJd = Convert.ToDouble(SimStartJDControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Start Julian Date Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        private void SimStartControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SimParams.SimStart = Convert.ToDouble(SimStartControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Simulation Starttime Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        private void SimEndControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SimParams.SimEnd = Convert.ToDouble(SimEndControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Simulation Endtime Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        private void MaxSchedulesControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SchParams.MaxSchedules = Convert.ToInt32(MaxSchedulesControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Max Schedules Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        private void CropToControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SchParams.CropTo = Convert.ToInt32(CropToControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Crop To Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        private void SimStepControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)Scenario.Clone();
                Scenario.SchParams.SimStep = Convert.ToDouble(SimStepControl.Text);
                ScenarioComponent after = (ScenarioComponent)Scenario.Clone();
                if (before.ToXml() != after.ToXml()) { _mManager.RegisterEvent(before, after, Scenario, "Time Step Change"); }
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        #endregion

        #region Memento

        public void RefreshMementoMenus()
        {
            // No menus for subform to refresh
            return;
        }

        public MementoManager GetMementoManager()
        {
            // Allows access from other forms
            return _mManager;
        }

        #endregion

        private void ScenarioForm_Load(object sender, EventArgs e)
        {

        }
    }
}
