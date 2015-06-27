using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Dialogs
{
    public partial class TextboxDialog : Form
    {
        private bool _cancelled;
        public bool Cancelled
        {
            get { return _cancelled; }
        }

        public string StringValue
        {
            get { return dialogTextbox.Text; }
        }

        public TextboxDialog(string Name, string Query)
        {
            InitializeComponent();
            Text = Name;
            dialogLabel.Text = Query;
            _cancelled = true;
        }

        private void returnButton_Click(object sender, EventArgs e)
        {
            if (dialogTextbox.Text != "")
            {
                _cancelled = false;
                Close();
            }
            else
            {
                MessageBox.Show("The textbox is empty!");
            }
        }
    }
}
