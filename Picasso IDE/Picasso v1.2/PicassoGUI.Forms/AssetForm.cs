using System;
using System.Windows.Forms;

namespace PicassoGUI
{
    public partial class AssetForm : Form
    {
        public String AssetName
        {
            get { return this.assetNameTextBox.Text; }
        }
        private Guid assetID;
        public Guid AssetID
        {
            get { return this.assetID; }
        }

        public AssetForm()
        {
            InitializeComponent();
            this.assetNameTextBox.Text = "New Asset";
            this.assetIDTextBox.Enabled = false;
            this.assetID = Guid.NewGuid();
            this.assetIDTextBox.Text = this.AssetID.ToString();
        }
    }
}
