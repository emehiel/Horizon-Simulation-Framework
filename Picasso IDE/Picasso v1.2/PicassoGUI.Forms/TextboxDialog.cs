using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicassoGUI.Forms
{
    public partial class TextboxDialog : Form
    {
        public string StringValue
        {
            get { return dialogTextbox.Text; }
        }

        public TextboxDialog(string Name, string Query)
        {
            InitializeComponent();
            Text = Name;
            dialogLabel.Text = Query;
        }

        private void returnButton_Click(object sender, EventArgs e)
        {
            if (dialogTextbox.Text != "")
            {
                Close();
            }
        }
    }
}
