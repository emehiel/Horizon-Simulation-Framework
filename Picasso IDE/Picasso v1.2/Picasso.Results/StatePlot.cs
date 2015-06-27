using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Results
{
    public delegate void CloseStatePlot(StatePlot target);

    public partial class StatePlot : UserControl
    {
        private List<double> _times;
        private List<double> _values;
        private string _stateName;
        private double _minT;
        private double _maxT;
        private double _minV;
        private double _maxV;
        private InterpolationMethod _mode;

        public bool Debug;
        public int CursorPosition;
        public CloseStatePlot Close;
        public string StateName
        {
            get { return _stateName; }
            set
            {
                _stateName = value;
                stateNameLabel.Text = _stateName;
            }
        }

        public StatePlot()
        {
            InitializeComponent();
            _times = new List<double>();
            _values = new List<double>();
            Close = DoNothing;
            _minT = 0.0;
            _maxT = 0.0;
            _minV = 0.0;
            _maxV = 0.0;
            CursorPosition = -1;
            Debug = false;
            _mode = InterpolationMethod.Cardinal;
            DoubleBuffered = true;
        }

        public void SetDomain(double lowerBounds, double upperBounds)
        {
            // Sets the domain to the given bounds
            if (lowerBounds >= upperBounds) return;
            _minT = lowerBounds;
            _maxT = upperBounds;

            // Must re-adjust Range for given Domain
            bool firstPointFound = false;
            for (int i = 0; i < _times.Count; i++)
            {
                if (_times[i] >= _minT && _times[i] <= _maxT)
                {
                    if (firstPointFound)
                    {
                        _maxV = _values[i];
                        _minV = _values[i];
                        firstPointFound = true;
                    }
                    else
                    {
                        if (_values[i] > _maxV)
                        {
                            _maxV = _values[i];
                        }
                        if (_values[i] < _minV)
                        {
                            _minV = _values[i];
                        }
                    }
                }
            }
        }

        public void AddPoint(double t, double v)
        {
            // Adds point (t,v) to list, and scales domain to include new point
            _times.Add(t);
            _values.Add(v);
            Sort();
            if (t > _maxT) { _maxT = t; }
            if (t < _minT) { _minT = t; }
            if (v > _maxV) { _maxV = v; }
            if (v < _minV) { _minV = v; }
        }

        public void AddPoints(double[] t, double[] v)
        {
            for (int i = 0; i < t.Length; i++)
            {
                _times.Add(t[i]);
                _values.Add(v[i]);
                if (t[i] > _maxT) { _maxT = t[i]; }
                if (t[i] < _minT) { _minT = t[i]; }
                if (v[i] > _maxV) { _maxV = v[i]; }
                if (v[i] < _minV) { _minV = v[i]; }
            }
            Sort();
            Update();
        }

        public void Sort()
        {
            // Bubble-sorts data points by time
            bool _isSorted = true;
            double tmp = 0.0;
            do
            {
                _isSorted = true;
                for (int i = 0; i < _times.Count - 1; i++)
                {
                    if (_times[i] > _times[i + 1])
                    {
                        tmp = _times[i];
                        _times[i] = _times[i + 1];
                        _times[i + 1] = tmp;
                        tmp = _values[i];
                        _values[i] = _values[i + 1];
                        _values[i + 1] = tmp;
                        _isSorted = false;
                    }
                }
            } while (!_isSorted);
        }

        private void closeButton_MouseEnter(object sender, EventArgs e)
        {
            // Change image to 'over'
            closeButton.Image = global::Picasso.Results.Properties.Resources.Close_Button_Over;
        }

        private void closeButton_MouseLeave(object sender, EventArgs e)
        {
            // Change back image
            closeButton.Image = global::Picasso.Results.Properties.Resources.Close_Button;
        }

        private void closeButton_MouseClick(object sender, MouseEventArgs e)
        {
            // Close; parent component will have to intercept close delegate
            Close(this);
        }

        private void DoNothing(StatePlot plot)
        {
            // Control must be closed by parent; this method is blank
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Draw plot
            e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
            List<Point> points = new List<Point>();
            double highest = _values[0];
            double lowest = _values[0];
            int currX = 0; int currY = 0; int prevX = 0; int prevY = 0;
            for (int i = 0; i < _times.Count; i++)
            {
                if (_times[i] >= _minT && _times[i] <= _maxT)
                {
                    prevX = currX;
                    prevY = currY;
                    currX = (int)(Width * (_times[i] - _minT) / (_maxT - _minT));
                    currY = Height - (int)(Height * (_values[i] - _minV) / (_maxV - _minV));
                    if (_mode == InterpolationMethod.Linear && i > 0 && _times[i-1] > _minT)
                    {
                        e.Graphics.DrawLine(Pens.Yellow, new Point(prevX, prevY), new Point(currX, currY));
                    }
                    else if (_mode == InterpolationMethod.Cardinal)
                    {
                        points.Add(new Point(currX, currY));
                    }
                }
            }
            if (_mode == InterpolationMethod.Cardinal)
            {
                e.Graphics.DrawCurve(Pens.Yellow, points.ToArray(), 0.5f);
            }

            // Draw cursor highlight, if position is known
            if (Debug)
            {
                Debug = true;
            }
            if (CursorPosition >= 0)
            {
                Point upperLeft = new Point(CursorPosition - 1, 0);
                Point upperRight = new Point(CursorPosition + 1, 0);
                Point lowerLeft = new Point(CursorPosition - 1, Height);
                Point lowerRight= new Point(CursorPosition + 1, Height);
                e.Graphics.DrawLine(Pens.White, upperLeft, lowerLeft);
                e.Graphics.DrawLine(Pens.White, upperRight, lowerRight);
            }
            base.OnPaint(e);
        }

        private void interpolationButton_MouseClick(object sender, MouseEventArgs e)
        {
            // Change interpolation method
            if (_mode == InterpolationMethod.Cardinal)
            {
                _mode = InterpolationMethod.Linear;
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonLineOver;
            }
            else
            {
                _mode = InterpolationMethod.Cardinal;
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonSplineOver;
            }
            Refresh();
        }

        private void interpolationButton_MouseEnter(object sender, EventArgs e)
        {
            // Switch to over image
            if (_mode == InterpolationMethod.Cardinal)
            {
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonSplineOver;
            }
            else
            {
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonLineOver;
            }
        }

        private void interpolationButton_MouseLeave(object sender, EventArgs e)
        {
            // Switch back to normal image
            if (_mode == InterpolationMethod.Cardinal)
            {
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonSpline;
            }
            else
            {
                interpolationButton.Image = global::Picasso.Results.Properties.Resources.interpButtonLine;
            }
        }
    }

    public enum InterpolationMethod
    {
        Linear,
        Cardinal
    }
}
