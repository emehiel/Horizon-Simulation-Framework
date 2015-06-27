using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Elements;
using Picasso.Utility;

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

        public TargetIcon(TargetElement target)
        {
            Tag = target;
            Cursor = Cursors.Hand;
            InitializeComponent();
        }

        private void TargetIcon_MouseDown(object sender, MouseEventArgs e)
        {
            // Mouse has been pressed down; begin move
            _currentCursor = Cursor;
            Cursor = Cursors.SizeAll;
            _cursorOffset = e.Location;
            _isMoving = true;
        }

        private void TargetIcon_MouseUp(object sender, MouseEventArgs e)
        {
            // Mouse is no longer pressed; end move
            Cursor = _currentCursor;
            _isMoving = false;

            // Notify parent form of move
            try
            {
                ((IMessageRouter)FindForm()).OnControlMove(this, e);
            }
            catch (InvalidCastException)
            {
                // Parent isn't a message router; that's ok
            }
        }

        private void TargetIcon_MouseMove(object sender, MouseEventArgs e)
        {
            // Mouse is still pressed; continue move
            if (_isMoving)
            {
                Point clientPosition = Parent.PointToClient(Cursor.Position);
                Point adjustedLocation = new Point(clientPosition.X - _cursorOffset.X, clientPosition.Y - _cursorOffset.Y);
                Location = adjustedLocation;
            }
        }

        private void TargetIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Send double-click message to parent form
            try
            {
                ((IMessageRouter)FindForm()).OnDoubleClick(this, e);
            }
            catch (InvalidCastException)
            {
                // Parent form not a message router; control in unusual location, but that's ok
            }
        }
    }
}
