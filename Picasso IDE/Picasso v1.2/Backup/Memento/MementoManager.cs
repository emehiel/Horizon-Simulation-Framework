using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MementoModel
{
    public class MementoManager
    {
        // Contains stacks, undo / redo call logic, handle to primary IMForm for menu refresh, and event creation
        private Stack<Memento> _undoStack;
        private Stack<Memento> _redoStack;
        private IMForm _mainForm;

        public MementoManager(IMForm mainForm)
        {
            // Creates new MementoManager with blank stacks and assigned main form reference
            _undoStack = new Stack<Memento>();
            _redoStack = new Stack<Memento>();
            _mainForm = mainForm;
        }

        public void Undo()
        {
            // Executes undo call, including menu refreshing call to form handle
            if (_undoStack.Count > 0)
            {
                Memento currentEvent = _undoStack.Pop();
                currentEvent.Undo();
                _redoStack.Push(currentEvent);
                _mainForm.RefreshMementoMenus();

                /* This next line is not applicable for Picasso, because the forms with values and the main form (with
                 * menus) are never the same. In other applications, though, it would be necessary. */
                _mainForm.RefreshValues();
            }
        }

        public void Redo()
        {
            // Executes redo call, including menu refreshing call to form handle
            if (_redoStack.Count > 0)
            {
                Memento currentEvent = _redoStack.Pop();
                currentEvent.Redo();
                _undoStack.Push(currentEvent);
                _mainForm.RefreshMementoMenus();

                /* This next line is not applicable for Picasso, because the forms with values and the main form (with
                 * menus) are never the same. In other applications, though, it would be necessary. */
                _mainForm.RefreshValues();
            }
        }

        public void RegisterEvent(object before, object after, object handle, string desc)
        {
            // New event is filed with before, after, and handle of relevant object, and description of event. Note
            // objects are cloned in Memento creation.
            Memento newEvent;
            try
            {
                newEvent = new Memento((IMCloneable)before, (IMCloneable)after, (IMCloneable)handle, desc);
                _redoStack.Clear();
                _undoStack.Push(newEvent);
                _mainForm.RefreshValues();
                _mainForm.RefreshMementoMenus();
            }
            catch (InvalidCastException)
            {
                MessageBox.Show("Unable to create event from " + handle.GetType().ToString() + "; not IMClonable");
            }
        }

        public string GetUndoDesc()
        {
            // Returns the string describing the top undo event
            Memento topUndoEvent = _undoStack.Pop();
            string toReturn = topUndoEvent.Description;
            _undoStack.Push(topUndoEvent);
            return toReturn;
        }

        public string GetRedoDesc()
        {
            // Returns the string describing the top undo event
            Memento topRedoEvent = _redoStack.Pop();
            string toReturn = topRedoEvent.Description;
            _redoStack.Push(topRedoEvent);
            return toReturn;
        }

        public bool hasUndo()
        {
            // Returns true if the undo stack has at least one event
            return _undoStack.Count > 0;
        }

        public bool hasRedo()
        {
            // Returns true if the redo stack has at least one event
            return _redoStack.Count > 0;
        }
    }
}
