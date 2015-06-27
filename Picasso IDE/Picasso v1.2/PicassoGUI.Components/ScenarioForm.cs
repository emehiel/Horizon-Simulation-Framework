using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MementoModel;

namespace PicassoGUI.Components
{
    public partial class ScenarioForm : Form, IMForm
    {
        public ScenarioComponent hComponent;
        private MementoManager _mManager;

        public ScenarioForm(ScenarioComponent targetComponent, Form parentWindow)
        {
            InitializeComponent();
            MdiParent = parentWindow;
            Tag = targetComponent;
            hComponent = targetComponent;
            Text = hComponent.ScenarioName + " (Scenario)";
            _mManager = new MementoManager((IMForm)MdiParent);
            RefreshValues();
        }

        public void RefreshValues()
        {
            SimStartJDControl.Text = hComponent.SimParams.SimStartJD.ToString();
            SimStartControl.Text = hComponent.SimParams.SimStart.ToString();
            SimEndControl.Text = hComponent.SimParams.SimEnd.ToString();
            MaxSchedulesControl.Text = hComponent.SchParams.MaxSchedules.ToString();
            CropToControl.Text = hComponent.SchParams.CropTo.ToString();
            SimStepControl.Text = hComponent.SchParams.SimStep.ToString();
        }

        /* --- Events --- */
        // Note: All events that change values should register event with memento manager
        private void SimStartJDControl_TextChanged(object sender, EventArgs e)
        {
            try
            {
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SimParams.SimStartJD = Convert.ToDouble(SimStartJDControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
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
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SimParams.SimStart = Convert.ToDouble(SimStartControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
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
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SimParams.SimEnd = Convert.ToDouble(SimEndControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
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
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SchParams.MaxSchedules = Convert.ToInt32(MaxSchedulesControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
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
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SchParams.CropTo = Convert.ToInt32(CropToControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
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
                ScenarioComponent before = (ScenarioComponent)hComponent.Clone();
                hComponent.SchParams.SimStep = Convert.ToDouble(SimStepControl.Text);
                _mManager.RegisterEvent(before, hComponent, hComponent, "Reference Time Change");
            }
            catch (FormatException)
            {
                // Text box value not a double; ignore change
                MessageBox.Show("Text box value not in correct format");
            }
        }

        public void RefreshMementoMenus()
        {
            // Should update undo / redo menus in form's menu strip
            //Component forms have no menus (handled by main form)
        }

        public MementoManager GetMementoManager()
        {
            // Returns the MementoManager belonging to the form
            return _mManager;
        }
    }
}
