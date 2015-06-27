using System;
using System.Collections.Generic;
using System.Windows.Forms;
using PicassoGUI.Util;
using System.Xml;
using Hsf.Utility;

namespace TargetViewer
{
    public partial class TargetViewer : UserControl
    {
        public TargetViewer()
        {
            InitializeComponent();

            this.navControl.MyListView = this.targetListView;
        }

        public void ImportTargets()
        {
            this.importTargetFileDialog.ShowDialog();
        }
        public void ExportTargets()
        {
            this.exportTargetFileDialog.ShowDialog();
        }
        public void WriteTargets(String filename)
        {
            this.WriteTargetDeck(filename);
        }

        private void listView1_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            ((Target)e.Item.Tag).Selected = !((Target)e.Item.Tag).Selected;
            //this.Targets[e.ItemIndex].Selected = !this.Targets[e.ItemIndex].Selected;
            this.targetPropertyGrid.SelectedObject = e.Item.Tag;
           // this.targetPropertyGrid.SelectedObject = this.Targets[e.ItemIndex];
            this.navControl.Invalidate();
        }

        private void targetPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[0].Text = ((Target)this.targetListView.SelectedItems[0].Tag).Name;
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[1].Text = ((Target)this.targetListView.SelectedItems[0].Tag).Position.ToString();
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[2].Text = ((Target)this.targetListView.SelectedItems[0].Tag).TargetType.ToString();
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[3].Text = ((Target)this.targetListView.SelectedItems[0].Tag).TaskType.ToString();
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[4].Text = ((Target)this.targetListView.SelectedItems[0].Tag).MaxTimes.ToString();
            this.targetListView.Items[this.targetListView.SelectedIndices[0]].SubItems[5].Text = ((Target)this.targetListView.SelectedItems[0].Tag).Value.ToString();

            this.targetListView.RedrawItems(0, this.targetListView.Items.Count - 1, true);
        }

        private void importTargetFileDialog_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            XmlTextReader reader = new XmlTextReader(this.importTargetFileDialog.FileName);

            reader.WhitespaceHandling = WhitespaceHandling.None;
            XmlDocument xmlDoc = new XmlDocument();
            //Load the file into the XmlDocument
            xmlDoc.Load(reader);
            //Close off the connection to the file.
            reader.Close();

            XmlNode xnod = xmlDoc.DocumentElement;

            XmlNodeList nodes = xmlDoc.ChildNodes[0].ChildNodes;

            foreach (XmlNode node in nodes)
            {
                Target t = new Target(node);
                //this.Targets.Add(t);
                ListViewItem item = new ListViewItem();
                item.Tag = t;
                item.Text = ((Target)item.Tag).Name;
                item.SubItems.Add(((Target)item.Tag).Position.ToString());
                item.SubItems.Add(((Target)item.Tag).TargetType.ToString());
                item.SubItems.Add(((Target)item.Tag).TaskType.ToString());
                item.SubItems.Add(((Target)item.Tag).MaxTimes.ToString());
                item.SubItems.Add(((Target)item.Tag).Value.ToString());

                if (t.TargetType == TargetType.FacilityTarget)
                    item.StateImageIndex = 0;
                else if (t.TargetType == TargetType.LocationTarget)
                    item.StateImageIndex = 1;
                
                this.targetListView.Items.Add(item);
            }

            this.targetListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
        }

        private void exportTargetFileDialog_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            this.WriteTargetDeck(this.exportTargetFileDialog.FileName);
        }

        private void WriteTargetDeck(String filename)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;
            settings.NewLineOnAttributes = true;
            settings.OmitXmlDeclaration = true;

            XmlWriter writer = XmlWriter.Create(filename, settings);

            writer.WriteStartElement("TARGETDECK");

            //foreach (Target t in this.Targets)
            foreach(ListViewItem item in this.navControl.MyListView.Items)
            {
                Target t = (Target)item.Tag;
                writer.WriteStartElement("TARGET");
                writer.WriteAttributeString("TargetName", t.Name);
                writer.WriteAttributeString("TargetType", t.TargetType.ToString());
                writer.WriteAttributeString("TaskType", t.TaskType.ToString());
                writer.WriteAttributeString("MaxTimes", t.MaxTimes.ToString());
                writer.WriteAttributeString("Value", t.Value.ToString());
                writer.WriteStartElement("POSITION");
                writer.WriteAttributeString("PositionType", t.Position.Type.ToString());
                writer.WriteAttributeString("ICs", "Matrix(3,1,{" + t.Position.ToString() + "})");
                writer.WriteEndElement(); //POSITION

                writer.WriteEndElement(); //TARGET
            }
            writer.WriteEndElement(); //TARGETDECK

            writer.Close();
        }
    }
}
