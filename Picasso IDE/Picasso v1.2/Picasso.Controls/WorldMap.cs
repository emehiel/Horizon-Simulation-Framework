using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;

namespace Picasso.Controls
{
    public partial class WorldMap : PictureBox
    {
        private bool _isDragging;
        private double _zoomFactor;     // Ranges from 1 (view whole map) to _zoomLimit
        private Cursor _previousCursor;
        private Point _previousPoint;
        private Image _originalMap;
        private double _zoomLimit;      // Upper limit of zoom factor

        public double ZoomFactor
        {
            get { return _zoomFactor; }
            set
            {
                // Check and set zoom factor
                if (value < 1.0) { value = 1.0; }
                if (value > _zoomLimit) { value = _zoomLimit; }
                _zoomFactor = value;

                // Check viewable area
                Form parent = FindForm();
                int newWidth = _originalMap.Width;
                int newHeight = _originalMap.Height;
                if (parent != null)
                {
                    newWidth = (int)(parent.Width * _zoomFactor);
                    newHeight = (int)(parent.Height * _zoomFactor);
                }

                // Re-sample image from original
                Bitmap newImage = new Bitmap(newWidth, newHeight);
                using (Graphics graphics = Graphics.FromImage(newImage))
                {
                    graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.Default;
                    graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;
                    graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;
                    graphics.DrawImage(_originalMap, 0, 0, newImage.Width, newImage.Height);
                }
                Image = (Image)newImage;

                // Make sure user isn't viewing past edges (bottom and right)
                if (parent != null)
                {
                    Point newLocation = Location;
                    if (-1.0 * Location.X + parent.Width > Width)
                    {
                        newLocation.X = -1 * (Width - parent.Width);
                    }
                    if (-1.0 * Location.Y + parent.Height > Height)
                    {
                        newLocation.Y = -1 * (Height - parent.Height);
                    }
                    Location = newLocation;
                }

                Refresh();
            }
        }

        public WorldMap()
        {
            InitializeComponent();
            _zoomLimit = 10.0;
            _originalMap = global::Picasso.Controls.Properties.Resources.land_shallow_topo_2048;
            ZoomFactor = 1.0;
        }

        private void WorldMap_MouseDown(object sender, MouseEventArgs e)
        {
            // Start dragging
            if (e.Button == MouseButtons.Left)
            {
                _isDragging = true;
                _previousCursor = Cursor;
                Cursor = Cursors.SizeAll;
                _previousPoint = new Point(e.X, e.Y);
            }
        }

        private void WorldMap_MouseUp(object sender, MouseEventArgs e)
        {
            // Stop dragging, register new offset
            if (e.Button == MouseButtons.Left)
            {
                _isDragging = false;
                Cursor = _previousCursor;
            }
        }

        private void WorldMap_MouseMove(object sender, MouseEventArgs e)
        {
            // Move map image
            if (_isDragging)
            {
                Form parentForm = FindForm();
                Point formDimensions = new Point(parentForm.Width, parentForm.Height);
                Point newLocation = new Point(Location.X + (e.X - _previousPoint.X), Location.Y + (e.Y - _previousPoint.Y));
                if (newLocation.X >= 0) { newLocation.X = 0; }
                if (newLocation.Y >= 0) { newLocation.Y = 0; }
                if (newLocation.X <= -1 * Width + formDimensions.X) { newLocation.X = -1 * Width + formDimensions.X; }
                if (newLocation.Y <= -1 * Height + formDimensions.Y) { newLocation.Y = -1 * Height + formDimensions.Y; }
                Location = newLocation;
            }
        }
    }
}
