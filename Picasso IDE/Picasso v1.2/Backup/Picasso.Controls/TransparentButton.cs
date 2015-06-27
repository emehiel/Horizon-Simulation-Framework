using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TransparencyDemo
{
    public class TransparentButton : PictureBox
    {
        private System.ComponentModel.IContainer components = null;
        private Bitmap _onImage;
        private Bitmap _offImage;
        private Color _transparentColor;

        public Color TransparentColor
        {
            get { return _transparentColor; }
            set
            {
                // Set color and update images
                _transparentColor = value;
                Bitmap bmp = (Bitmap)Image;
                bmp.MakeTransparent(_transparentColor);
                Image = bmp;
                _offImage.MakeTransparent(_transparentColor);
                if (_onImage != null) { _onImage.MakeTransparent(_transparentColor); }
            }
        }

        public Image OverImage
        {
            get { return _onImage; }
            set
            {
                if (value == null)
                {
                    _onImage = null;
                }
                else
                {
                    _onImage = (Bitmap)value;
                    _onImage.MakeTransparent(_transparentColor);
                }
            }
        }

        new public Image Image
        {
            get { return base.Image; }
            set
            {
                Bitmap bmp = (Bitmap)value;
                bmp.MakeTransparent(_transparentColor);
                base.Image = bmp;

                // Only set off image if we are not swapping
                if (bmp != _onImage)
                {
                    _offImage = bmp;
                }
            }
        }

        public TransparentButton()
        {
            // For transparency, parent control must specify self as button's Parent on construction
            _transparentColor = Color.FromArgb(255, 0, 255); // Traditional default: pink
            SizeMode = PictureBoxSizeMode.AutoSize;
            BackColor = Color.Transparent;
            InitializeComponent();
            DoubleBuffered = true;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            this.Cursor = System.Windows.Forms.Cursors.Hand;
            this.MouseLeave += new System.EventHandler(this.TransparentButton_MouseLeave);
            this.MouseEnter += new System.EventHandler(this.TransparentButton_MouseEnter);
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
        }

        public void On()
        {
            if (_onImage != null)
            {
                Image = _onImage;
            }
        }

        public void Off()
        {
            Image = _offImage;
        }

        private void TransparentButton_MouseEnter(object sender, EventArgs e)
        {
            if (_onImage != null && Enabled)
            {
                Image = _onImage;
            }
        }

        private void TransparentButton_MouseLeave(object sender, EventArgs e)
        {
            Image = _offImage;
        }
    }
}