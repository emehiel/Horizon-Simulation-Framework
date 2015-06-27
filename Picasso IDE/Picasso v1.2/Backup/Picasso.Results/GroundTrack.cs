using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;
using Picasso.Forms;
using Picasso.Utility;

namespace Picasso.Results
{
    public partial class GroundTrack : PicassoSubform
    {
        private int speed;
        private float _currTime;
        private DateTime lastRender;
        public float StartTime;
        public float EndTime;
        public int satellitePointRadius;
        public List<stateHistory> histories;
        
        public float CurrTime
        {
            get { return _currTime; }
            set
            {
                if (value < StartTime)
                {
                    _currTime = StartTime;
                }
                else if (value < _currTime)
                {
                    _currTime = EndTime;
                }
                else
                {
                    _currTime = value;
                    slideBar1.Value = (_currTime - StartTime) / (EndTime - StartTime);
                }
            }
        }

        public override Hsf.Elements.HsfElement Element
        {
            get
            {
                return new ResultsElement();
            }
        }

        public GroundTrack(Form parent)
        {
            InitializeComponent();
            speed = 0;
            MdiParent = parent;
            histories = new List<stateHistory>();
            StartTime = 0.0f;
            EndTime = 100.0f;
            CurrTime = 50.0f;
            satellitePointRadius = 4;
            lastRender = DateTime.Now;

            // Reduce flickering - double buffer
            DoubleBuffered = true;

            // Create sample histories
            stateHistory h1 = new stateHistory();
            stateHistory h2 = new stateHistory();
            h1.name = "Test 1";
            h2.name = "Test 2";
            h2.color = MagicColors.RandomColor(h1.color, 0.25f);
            for (float t = 0.0f; t < 100.0; t += 1.0f)
            {
                h1.addPoint(t, 15.0f * (float)(Math.Sin(t / 50.0f)) + 10.0f, t + 200.0f, 500.0f);
                h2.addPoint(t, 7.0f * (float)(Math.Cos(t / 20.0f)) - 25.0f, t - 120.0f, 200.0f);
            }
            histories.Add(h1);
            histories.Add(h2);
        }

        private void speedButton_Click(object sender, EventArgs e)
        {
            // Rotate speed
            speed = speed > 0 ? speed * 2 : 1;
            if (speed > 4) speed = 0;
            if (CurrTime == EndTime)
            {
                speed = 1;
                CurrTime = StartTime;
            }
            updateButton();
        }

        private void updateButton()
        {
            // Changes button image depending on speed
            switch (speed)
            {
                case 0:
                    this.speedButton.Image = global::Picasso.Results.Properties.Resources.speed0x;
                    break;
                case 1:
                    this.speedButton.Image = global::Picasso.Results.Properties.Resources.speed1x;
                    break;
                case 2:
                    this.speedButton.Image = global::Picasso.Results.Properties.Resources.speed2x;
                    break;
                case 4:
                    this.speedButton.Image = global::Picasso.Results.Properties.Resources.speed4x;
                    break;
            }
            speedButton.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            for (int j = 0; j < histories.Count; j++)
            {
                stateHistory h = histories[j];
                Pen pen = new Pen(h.color);
                List<Point> points = new List<Point>();
                int xOffset = 0;
                for (int i = 0; i < h.history.Count; i++)
                {
                    statePoint p = h.history[i];
                    int x = Conversion.Longitude2Pixel(p.longitude, Width) + xOffset;
                    int y = Conversion.Latitude2Pixel(p.latitude, Height);
                    points.Add(new Point(x, y));
                    if (x > Width)
                    {
                        // Draw current points, start new set
                        if (points.Count > 1)
                        {
                            e.Graphics.DrawLines(pen, points.ToArray());
                        }
                        points.Clear();
                        xOffset -= Width;
                    }
                    if (i != h.history.Count - 1 && p.t < CurrTime && CurrTime <= h.history[i + 1].t)
                    {
                        // Draw point for current satellite position
                        int sx = Conversion.Longitude2Pixel(p.longitude, Width) + xOffset;
                        int sy = Conversion.Latitude2Pixel(p.latitude, Height);
                        e.Graphics.DrawEllipse(pen, new Rectangle(sx - satellitePointRadius, sy - satellitePointRadius, 2*satellitePointRadius, 2*satellitePointRadius));
                    }
                }
                if (points.Count > 1)
                {
                    e.Graphics.DrawLines(pen, points.ToArray());
                }

                // Draw label
                e.Graphics.DrawString(h.name, Font, pen.Brush, new Point(Width - 100, 20 * (j + 1)));
            }

            // Update 
            TimeSpan diff = DateTime.Now - lastRender;
            float dt = (float)(diff.Seconds) + (float)(diff.Milliseconds) / 1000.0f;
            CurrTime = CurrTime + dt * speed;
            if (CurrTime == EndTime)
            {
                // Pause at end
                speed = 0;
                updateButton();
            }
            lastRender = DateTime.Now;
            timeLabel.Text = CurrTime.ToString();

            // Force animation update
            if (speed > 0)
            {
                Invalidate(true);
                timeLabel.Invalidate();
                slideBar1.Invalidate();
            }
            base.OnPaint(e);
        }
    }
}
