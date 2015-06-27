using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Controls
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

        public TargetIcon()
        {
            Cursor = Cursors.Hand;
            InitializeComponent();
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
