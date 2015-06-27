using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Results
{
    public partial class TestDriver : Form
    {
        public TestDriver()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StateHistories form = new StateHistories();
            form.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ModelView form = new ModelView();
            form.Show();
        }
    }
}
