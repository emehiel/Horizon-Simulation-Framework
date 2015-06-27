using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Forms
{
    public partial class SimpleBrowser : Form
    {
        private bool _persistent;
        public bool Persistent
        {
            set { _persistent = value; }
        }
        public string Url
        {
            get { return xmlWindow.Url.AbsoluteUri; }
            set { xmlWindow.Navigate(value); }
        }

        public SimpleBrowser(string Url, bool showWindow)
        {
            InitializeComponent();
            xmlWindow.Navigate(Url);
            Visible = showWindow;
        }

        public void GoTo(string Url)
        {
            xmlWindow.Navigate(Url);
        }

        private void SimpleBrowser_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_persistent && e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                Hide();
            }
        }
    }
}
