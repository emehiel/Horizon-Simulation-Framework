using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Remoting;
using System.Windows.Forms;

namespace MementoModel
{
    public class Memento
    {
        public IMCloneable before;
        public IMCloneable after;
        public IMCloneable handle;
        private string _description;
        public string Description
        {
            get { return _description; }
        }

        public Memento(IMCloneable b, IMCloneable a, IMCloneable h, string d)
        {
            before = (IMCloneable)(b.Clone());
            after = (IMCloneable)(a.Clone());
            handle = h;
            _description = d;
        }

        public void Undo()
        {
            handle.Clone(before);
        }

        public void Redo()
        {
            handle.Clone(after);
        }
    }

    public interface IMCloneable
    {
        // An object whose property changes may be undone / redone (boxes, controls, fields, models, etc)
        object Clone();                 // Returns a deep (value-wise) copy of the object
        void Clone(object value);       // Writes a deep (value-wise) copy onto the object from value
    }

    public interface IMForm
    {
        // A form that contains one or more of the following: undo / redo menu options, a memento manager, and value-refresher
        void RefreshMementoMenus();         // Should update undo / redo menus in form's menu strip according to this manager
        MementoManager GetMementoManager(); // Returns the MementoManager belonging to the form
        void RefreshValues();               // Updates value visualizations that draw from clonable objects
    }
}
