/*  Brian Kirkpatrick
 *  Base Class for Picasso Xml Handling and Interface for Piccaso Components
 *  Rewritten 7/17/2009
 *  */

using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.XPath;
using MementoModel;

namespace PicassoGUI.Components
{
    [XmlType("UNDEFINED_TYPE"), Serializable]
    public class PicassoNode
    {
        [XmlIgnore]
        public TreeNode hNode;

        public string GetXmlType()
        {
            // Returns the XmlType (node type) of relevant component class, or blank if no XmlType
            // has been defined
            Type myType = this.GetType();
            foreach (Attribute attr in Attribute.GetCustomAttributes(myType))
            {
                if (attr.GetType() == typeof(XmlTypeAttribute))
                {
                    return ((XmlTypeAttribute)attr).TypeName.ToString();
                }
            }
            return "";
        }

        public virtual bool Deserialize(string filename)
        {
            // Attempts to deserialize the relevant PIcasso componet from the given target node.
            // Loads child members from node attributes, child objects from child nodes. Returns
            // 'false' if no correct XmlType exists at or within given node.
            return true;
        }

        public virtual bool Serialize(XmlTextWriter targetWriter)
        {
            // Serializes this object into given stream, writing object as node, child members as
            // attributes, and child objects as sub-nodes. Can be overridden for special node
            // structure handling.
            XmlSerializer serializer = new XmlSerializer(this.GetType());
            serializer.Serialize(targetWriter, this);
            return true;
        }
    }

    public interface IPicassoComponent
    {
        bool FillProperties(PropertyGrid targetGrid);               // Fills given property grid
        bool FillWindow(Form parentWindow);                         // Fills given (floating) window with appropriate content
        bool PopulateTree(TreeView targetTree, TreeNode headNode);  // Populates tree view with name and children
    }
}
