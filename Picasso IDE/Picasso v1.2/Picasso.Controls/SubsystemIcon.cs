using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MementoModel;
using Hsf.Utility;
using Hsf.Elements;
using Picasso.Dialogs;
using Picasso.Utility;

namespace Picasso.Controls
{
    public partial class SubsystemIcon : UserControl, IMCloneable
    {
        public SubsystemElement subsystem;

        private Color _mainColor;
        private Color _lightColor;
        private Color _darkColor;
        private Point _cursorOffset;
        private bool _isMoving;
        private bool _isOver;
        private Cursor _previousCursor;
        private Cursor _overCursorSetting;
        private Cursor _moveCursorSetting;
        private Point _previousLocation;
        private bool _isOverLeft = false;
        private bool _isOverRight = false;
        private int connectorRadius = 8;
        private int _independentCount = 0;
        private int _dependentCount = 0;
        private int borderThickness = 2;
        private List<Label> constraintLabels;
        private int initHeight;
        private int colorThreshold = 40;

        public Color Shade
        {
            get { return _mainColor; }
            set
            {
                if (colorThreshold < value.R && value.R < 255 - colorThreshold && colorThreshold < value.G && value.G < 255 - colorThreshold && colorThreshold < value.B && value.B < 255 - colorThreshold)
                {
                    _mainColor = value;
                    _lightColor = Color.FromArgb(_mainColor.R + colorThreshold, _mainColor.G + colorThreshold, _mainColor.B + colorThreshold);
                    _darkColor = Color.FromArgb(_mainColor.R - colorThreshold, _mainColor.G - colorThreshold, _mainColor.B - colorThreshold);
                    BackColor = _mainColor;
                    Title.ForeColor = _darkColor;
                    addConstraintButton.ForeColor = _darkColor;
                }
            }
        }

        public bool IsMoving
        {
            get { return _isMoving; }
        }

        public int IndependentCount
        {
            get { return _independentCount; }
            set
            {
                if (value >= 0) { _independentCount = value; }
                if (_independentCount == 0) { Refresh(); }
            }
        }

        public int DependentCount
        {
            get { return _dependentCount; }
            set
            {
                if (value >= 0) { _dependentCount = value; }
                if (_dependentCount == 0) { Refresh(); }
            }
        }

        public bool IsOverLeft
        {
            get { return _isOverLeft; }
            set
            {
                _isOverLeft = value;
                if (_isOverLeft)
                {
                    Cursor = Cursors.Cross;
                }
                else if (!_isOverRight)
                {
                    Cursor = Cursors.Hand;
                }
            }
        }

        public bool IsOverRight
        {
            get { return _isOverRight; }
            set
            {
                _isOverRight = value;
                if (_isOverRight)
                {
                    Cursor = Cursors.Cross;
                }
                else if (!_isOverLeft)
                {
                    Cursor = Cursors.Hand;
                }
            }
        }

        public Point PreviousLocation
        {
            get { return _previousLocation; }
        }

        public SubsystemIcon()
        {
            InitializeComponent();
            constraintLabels = new List<Label>();

            // Set colors
            _mainColor = MagicColors.RandomColor(colorThreshold);
            _lightColor = Color.FromArgb(_mainColor.R + colorThreshold, _mainColor.G + colorThreshold, _mainColor.B + colorThreshold);
            _darkColor = Color.FromArgb(_mainColor.R - colorThreshold, _mainColor.G - colorThreshold, _mainColor.B - colorThreshold);
            BackColor = _mainColor;
            Title.ForeColor = _darkColor;
            addConstraintButton.ForeColor = _darkColor;

            // Set cursors
            _overCursorSetting = Cursors.Hand;
            _moveCursorSetting = Cursors.SizeAll;
            initHeight = Height;

            // Set control transparencies
            Title.MouseEnter += new EventHandler(SubsystemIcon_MouseEnter);
            Title.MouseLeave += new EventHandler(SubsystemIcon_MouseLeave);
            Title.MouseDown += new MouseEventHandler(Title_MouseDown);
            Title.MouseMove += new MouseEventHandler(SubsystemIcon_MouseMove);
            Title.MouseUp += new MouseEventHandler(SubsystemIcon_MouseUp);
            Title.MouseDoubleClick += new MouseEventHandler(SubsystemIcon_MouseDoubleClick);
            addConstraintButton.MouseClick += new MouseEventHandler(addConstraintButton_MouseClick);
        }

        void addConstraintButton_MouseClick(object sender, MouseEventArgs e)
        {
            // Pop up constraint dialog
            ConstraintDialog diag = new ConstraintDialog();
            diag.ShowDialog();
            
            // Add constraint and display as label
            Label newConstraint = new Label();
            newConstraint.Text = diag.VariableName;
            newConstraint.Parent = this;
            newConstraint.Width = Width;
            newConstraint.Location = new Point(borderThickness, Height);
            newConstraint.BackColor = Color.Transparent;
            newConstraint.ForeColor = _darkColor;
            newConstraint.BringToFront();
            newConstraint.MouseEnter += new EventHandler(newConstraint_MouseEnter);
            newConstraint.MouseLeave += new EventHandler(newConstraint_MouseLeave);
            newConstraint.MouseClick += new MouseEventHandler(newConstraint_MouseClick);
            constraintLabels.Add(newConstraint);
            Height += newConstraint.Height;
            Refresh();
        }

        void newConstraint_MouseClick(object sender, MouseEventArgs e)
        {
            // Remove this constraint when clicked (redraw icon)
            try
            {
                Label cLabel = (Label)sender;
                int ndx = -1;
                int constHeight = constraintLabels[0].Height;
                for (int i = 0; i < constraintLabels.Count; i++)
                {
                    if (constraintLabels[i] == cLabel)
                    {
                        ndx = i;
                    }
                }
                if (ndx != -1)
                {
                    constraintLabels.RemoveAt(ndx);
                    Height -= constHeight;
                }
                Refresh();
            }
            catch (InvalidCastException)
            {
                // Sender wasn't a label
            }
        }

        void newConstraint_MouseLeave(object sender, EventArgs e)
        {
            try
            {
                Label constraint = (Label)sender;
                constraint.ForeColor = _darkColor;
            }
            catch (InvalidCastException)
            {
                // Somehow called by a non-label
            }
        }

        void newConstraint_MouseEnter(object sender, EventArgs e)
        {
            try
            {
                Label constraint = (Label)sender;
                constraint.ForeColor = _lightColor;
            }
            catch (InvalidCastException)
            {
                // Somehow called by a non-label
            }
        }

        void Title_MouseDown(object sender, MouseEventArgs e)
        {
            // Notify subsystem and canvas
            OnMouseDown(e);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Set rendering objects
            Graphics g = e.Graphics;
            Pen backgroundPen = new Pen(_mainColor);
            Pen borderPen = new Pen(_darkColor);
            int offset = borderThickness / 2;
            if (_isOver) { borderPen.Color = _lightColor; }
            borderPen.Width = borderThickness;

            // Update title
            Title.Text = subsystem.Name;

            // Draw rectangle
            g.DrawRectangle(backgroundPen, 0, 0, Width, Height);

            // Draw borders
            g.DrawLine(borderPen, offset, offset, Width - offset, offset);
            g.DrawLine(borderPen, Width - offset, 0, Width - offset, initHeight / 2 - connectorRadius);
            g.DrawEllipse(borderPen, Width - connectorRadius - offset, initHeight / 2 - connectorRadius, connectorRadius * 2, connectorRadius * 2);
            g.DrawLine(borderPen, Width - offset, initHeight / 2 + connectorRadius, Width - offset, Height - offset);
            g.DrawLine(borderPen, Width - offset, Height - offset, offset, Height - offset);
            g.DrawLine(borderPen, offset, Height - offset, offset, initHeight / 2 + connectorRadius);
            g.DrawEllipse(borderPen, -connectorRadius, initHeight / 2 - connectorRadius, connectorRadius * 2, connectorRadius * 2);
            g.DrawLine(borderPen, offset, initHeight / 2 - connectorRadius, offset, offset);
        }

        private void SubsystemIcon_MouseEnter(object sender, EventArgs e)
        {
            // Change border color
            Title.ForeColor = _lightColor;
            addConstraintButton.ForeColor = _lightColor;
            _previousCursor = Cursor;
            Cursor = _overCursorSetting;
            _isOver = true;
            Refresh();
        }

        private void SubsystemIcon_MouseLeave(object sender, EventArgs e)
        {
            // Change border color
            Title.ForeColor = _darkColor;
            addConstraintButton.ForeColor = _darkColor;
            Cursor = _previousCursor;
            IsOverLeft = false;
            IsOverRight = false;
            _isOver = false;
            Refresh();
        }

        private void SubsystemIcon_MouseDown(object sender, MouseEventArgs e)
        {
            if (!IsOverLeft && !IsOverRight)
            {
                // Begin move
                Cursor = _moveCursorSetting;
                _cursorOffset = e.Location;
                _isMoving = true;
                _previousLocation = Location;
            }
        }

        private void SubsystemIcon_MouseUp(object sender, MouseEventArgs e)
        {
            // Mouse is no longer pressed; end move
            Cursor = _overCursorSetting;
            _isMoving = false;
            try
            {
                IMessageRouter mdiChild = (IMessageRouter)FindForm();
                mdiChild.OnControlMove(this, e);
            }
            catch (InvalidCastException)
            {
                // Couldn't find parent form with appropriate message router
            }
        }

        private void SubsystemIcon_MouseMove(object sender, MouseEventArgs e)
        {
            // Mouse is still pressed; continue move
            if (_isMoving)
            {
                Point clientPosition = Parent.PointToClient(Cursor.Position);
                Point adjustedLocation = new Point(clientPosition.X - _cursorOffset.X, clientPosition.Y - _cursorOffset.Y);
                Location = adjustedLocation;
            }

            // Check connection mouseover
            if (e.X < Width * 0.1 && e.Y < Height * 0.7 && Height * 0.3 < e.Y)
            {
                IsOverLeft = true;
            }
            else
            {
                IsOverLeft = false;
            }
            if (e.X > Width * 0.9 && e.Y < Height * 0.7 && Height * 0.3 < e.Y)
            {
                IsOverRight = true;
            }
            else
            {
                IsOverRight = false;
            }
            Refresh();
        }

        private void SubsystemIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Pass double-click to message-routing form, using this subsystem
            try
            {
                IMessageRouter mdiChild = (IMessageRouter)FindForm();
                mdiChild.OnDoubleClick(this, e);
            }
            catch (InvalidCastException)
            {
                // Couldn't find appropriate parent form, or parent form was not a message router
            }
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object's relevant values
            SubsystemIcon copy = new SubsystemIcon();
            copy.Location = new Point(Location.X, Location.Y);
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            SubsystemIcon copy = (SubsystemIcon)value;
            Location = new Point(copy.Location.X, copy.Location.Y);
        }
    }
}
