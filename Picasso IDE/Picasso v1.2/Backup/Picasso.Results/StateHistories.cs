using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Picasso.Forms;

namespace Picasso.Results
{
    public partial class StateHistories : PicassoSubform
    {
        private int _plotBuffer;    // Amount of space between each plot and between plots and listbox, in px
        private int _plotCount;     // Number of plots currently being displayed
        private int _plotHeight;    // Desired height of each plot
        private List<StatePlot> _hiddenStates; // States not being displayed; listed in hiddenStateListBox
        private List<StatePlot> _shownStates; // States being displayed; also referenced in Controls collection

        public override Hsf.Elements.HsfElement Element
        {
            get
            {
                return new ResultsElement();
            }
        }

        public StateHistories(Form parent)
        {
            InitializeComponent();
            _plotCount = 0;
            _plotBuffer = 25;
            _plotHeight = 100;
            _hiddenStates = new List<StatePlot>();
            _shownStates = new List<StatePlot>();
            MdiParent = parent;
        }

        public void LoadFromXmlFile(string filename)
        {
            // Displays plots defined in the given file, formatted by Horizon subsystem
        }

        public void LoadFromRawFile(string filename)
        {
            // Displays plots defined in the given file, space-delimited with one row per variable
            StreamReader sr = new StreamReader(filename);
            string timeLine = sr.ReadLine();

            // Load time values for reference
            List<double> timeValues = new List<double>();
            string[] timeStrings = timeLine.Split('\t');
            if (timeStrings[0] != "Time")
            {
                MessageBox.Show("Unable to read raw data from " + filename + "; no time values given");
                return;
            }
            foreach (string timeString in timeStrings)
            {
                if (timeString != "Time")
                {
                    timeValues.Add(Convert.ToDouble(timeString.Trim()));
                }
            }

            // Load remaining values
            string line = "";
            while (!sr.EndOfStream)
            {
                line = sr.ReadLine();
                string[] values = line.Split('\t');
                string name = values[0];
                StatePlot newPlot = new StatePlot();
                newPlot.StateName = name;
                int ndx = 0;
                foreach (string value in values)
                {
                    if (value != name)
                    {
                        newPlot.AddPoint(timeValues[ndx], Convert.ToDouble(value.Trim()));
                        ndx++;
                    }
                }
                ShowPlot(newPlot);
            }
            sr.Close();
        }

        public void ShowPlot(StatePlot plot)
        {
            // Adds plot to display list
            Point newLocation = new Point(_plotBuffer, _plotCount * (_plotHeight + _plotBuffer) + _plotBuffer);
            _plotCount++;
            plot.Location = newLocation;
            Controls.Add(plot);
            plot.Parent = formContainer.Panel2;
            plot.Show();
            _shownStates.Add(plot);

            // Make sure events are tied
            plot.Close = ClosePlot;
            plot.MouseMove += UpdateCursorHighlights;
            plot.MouseLeave += CancelCursorHighlights;

            // Remove plot from display list
            _hiddenStates.Remove(plot);
            hiddenStateListBox.Items.Remove(plot.StateName);
        }

        public void ClosePlot(StatePlot plot)
        {
            // Close given state plot, re-add to list of hidden states, and re-order current plots
            _shownStates.Remove(plot);
            _hiddenStates.Add(plot);
            hiddenStateListBox.Items.Add(plot.StateName);
            formContainer.Panel2.Controls.Remove(plot);
            _plotCount = 0;
            foreach (Control c in formContainer.Panel2.Controls)
            {
                if (c.GetType() == typeof(StatePlot))
                {
                    Point newLocation = new Point(_plotBuffer, _plotCount * (_plotHeight + _plotBuffer) + _plotBuffer);
                    c.Location = newLocation;
                    _plotCount++;
                }
            }
        }

        private void hiddenStateListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Removes state name from hidden list, finds state with given name, and adds to display list
            string toAdd = (string)(hiddenStateListBox.SelectedItem);
            hiddenStateListBox.Items.Remove(toAdd);
            foreach (StatePlot sp in _hiddenStates)
            {
                if (sp.StateName == toAdd)
                {
                    ShowPlot(sp);
                    break;
                }
            }
        }

        private void UpdateCursorHighlights(object sender, MouseEventArgs e)
        {
            // Sends new cursor position (x offset from LHS of plots) to each shown plot
            foreach (StatePlot sp in _shownStates)
            {
                sp.CursorPosition = e.X;     // Mouse location is in control frame
                sp.Debug = true;
                sp.Invalidate();
            }
        }

        private void CancelCursorHighlights(object sender, EventArgs e)
        {
            // Resets CursorPosition in all shown forms so it is not displayed
            foreach (StatePlot sp in _shownStates)
            {
                sp.CursorPosition = -1;
                sp.Invalidate();
            }
        }

        private void openButton_Click(object sender, EventArgs e)
        {
            // Open given file and try to read data
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Xml data (.xml)|*.xml|Raw data (.txt)|*.txt|All files (*.*)|*.*";
            dialog.Title = "Select file with state data";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                if (dialog.FileName.Contains(".xml"))
                {
                    LoadFromXmlFile(dialog.FileName);
                }
                else
                {
                    LoadFromRawFile(dialog.FileName);
                }
            }
        }
    }
}
