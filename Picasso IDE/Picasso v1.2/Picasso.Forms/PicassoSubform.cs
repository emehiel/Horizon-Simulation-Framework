using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Elements;
using MementoModel;

namespace Picasso.Forms
{
    public class PicassoSubform : Form
    {
        public TreeNode Node;

        public virtual HsfElement Element
        {
            get { throw new NotImplementedException("Element property not implemented by base PicassoSubform or derived class"); }
        }

        public virtual void RefreshValues()
        {
            return;
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // PicassoSubform
            // 
            this.ClientSize = new System.Drawing.Size(292, 269);
            this.Name = "PicassoSubform";
            this.ResumeLayout(false);
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                Hide();
            }
            else
            {
                base.OnFormClosing(e);
            }
        }

        protected override void OnEnter(EventArgs e)
        {
            RefreshValues();
            base.OnEnter(e);
        }
    }
}
