using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PicassoGUI.Util;

namespace PicassoGUI.Forms
{
    public partial class ImportDialog : Form
    {
        private ImportQuerySource _source;
        private ImportUtility _importUt;

        public ImportDialog()
        {
            InitializeComponent();
            comboBox1.DataSource = Enum.GetValues(typeof(ImportQuerySource));
            _importUt = new ImportUtility();
        }

        private void comboBox1_SelectedValueChanged(object sender, EventArgs e)
        {
            // Update labels
            if (comboBox1.Text == ImportQuerySource.CelesTrak.ToString())
            {
                label2.Text = "Category";
                label3.Text = "Satellite";
            }
            else
            {
                label2.Text = "User Email";
                label3.Text = "Search Term";
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Get list of potential results from import utility
            List<string> results;
            listBox1.Items.Clear();
            if (comboBox1.Text == ImportQuerySource.CelesTrak.ToString())
            {
                results = _importUt.SearchCeles(textBox1.Text, textBox2.Text);
            }
            else
            {
                results = _importUt.SearchPosc(textBox1.Text, textBox2.Text);
            }
            foreach (string result in results)
            {
                listBox1.Items.Add(result);
            }
        }
    }

    public enum ImportQuerySource
    {
        POSC,
        CelesTrak
    }
}
