using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicassoGUI.Components
{
    public partial class TargetIcon : PictureBox
    {
        private Cursor _currentCursor;
        private Point _cursorOffset;
        private bool _isMoving;

        public bool IsMoving
        {
            get { return _isMoving; }
        }

        public TargetIcon(TargetComponent h)
        {
            Cursor = Cursors.Hand;
            InitializeComponent();
            Tag = h;
        }

        private void TargetIcon_MouseDown(object sender, MouseEventArgs e)
        {
            _currentCursor = Cursor;
            Cursor = Cursors.SizeAll;
            _cursorOffset = e.Location;
            _isMoving = true;
        }

        private void TargetIcon_MouseUp(object sender, MouseEventArgs e)
        {
            Cursor = _currentCursor;
            _isMoving = false;
            if (Tag.GetType() == typeof(TargetComponent))
            {
                double latitude = Convert.ToDouble(this.Location.Y) / this.Parent.Height * 180.0 - 90.0;
                double longitude = Convert.ToDouble(this.Location.X) / this.Parent.Width * 360.0 - 180.0;
                ((TargetComponent)Tag).TargetPosition = new Position(latitude, longitude);
            }
        }

        private void TargetIcon_MouseMove(object sender, MouseEventArgs e)
        {
            if (_isMoving)
            {
                Point clientPosition = Parent.PointToClient(Cursor.Position);
                Point adjustedLocation = new Point(clientPosition.X - _cursorOffset.X, clientPosition.Y - _cursorOffset.Y);
                Location = adjustedLocation;
            }
        }
    }
}
