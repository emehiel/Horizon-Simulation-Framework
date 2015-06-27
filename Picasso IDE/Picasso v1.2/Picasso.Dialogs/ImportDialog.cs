using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;
using Hsf.Elements;
using Picasso.Utility;

namespace Picasso.Dialogs
{
    public partial class ImportDialog : Form
    {
        private ImportUtility _importUt;
        private AssetElement _asset;
        private bool _cancelled;

        public bool Cancelled
        {
            get { return _cancelled; }
        }

        public AssetElement Asset
        {
            get { return _asset; }
        }

        public ImportDialog()
        {
            InitializeComponent();
            comboBox1.DataSource = Enum.GetValues(typeof(ImportQuerySource));
            _importUt = new ImportUtility();
            _cancelled = false;
            CancelButton = cancelButton;
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
            if (textBox1.Text == "" || textBox2.Text == "")
            {
                MessageBox.Show("Enter terms for both search criteria");
                return;
            }
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

        private void importButton_Click(object sender, EventArgs e)
        {
            // Create a new asset from current import selection and assign to 
            Hashtable source;
            string name = (string)listBox1.SelectedItem;
            if (name == null)
            {
                return;
            }
            if (comboBox1.Text == ImportQuerySource.CelesTrak.ToString())
            {
                source = _importUt.ImportFromCeles(name);
            }
            else
            {
                source = _importUt.ImportFromPosc(name, textBox1.Text);
            }

            // Read source ht values and convert into asset
            _asset = new AssetElement();
            _asset.Name = source["Name"].ToString();
            _asset.Position.Ics.FromString(source["Position"].ToString());
            _asset.Position.PositionType = PositionType.DynamicECI;
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            _cancelled = true;
            Close();
        }
    }

    public enum ImportQuerySource
    {
        POSC,
        CelesTrak
    }
}
