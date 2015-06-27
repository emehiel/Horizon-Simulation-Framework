using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Hsf.Scripting;
using Picasso.Utility;

namespace Picasso.Controls
{
    public partial class ScriptingEditor : UserControl
    {
        private int lineHeight = 15;
        private int charWidth = 6;
        private Color textColor = Color.Black;
        private Color commentColor = Color.Green;
        private EditorState state;
        private EditorStatus _status = EditorStatus.Green;
        private float statusDelay = 2.0f; // Determines how long after most recent key press scripts are processed / saved / verified
        private DateTime lastTime = DateTime.Now; // How long (in seconds) since the last key press

        public EditorStatus Status
        {
            get { return _status; }
        }

        public string[] Lines
        {
            get { return editorTextbox.Lines; }
            set { editorTextbox.Lines = value; }
        }

        public Point CursorPosition
        {
            get
            {
                if (Lines.Length == 0) { return new Point(0, 0); }
                int position = 0;
                int line = 0;
                while (position + Lines[line].Length + 1 <= editorTextbox.SelectionStart)
                {
                    position += Lines[line].Length + 1; // Advance count by length of one line and EOL char
                    line++;
                }
                Point toReturn = new Point();
                toReturn.X = editorTextbox.SelectionStart - position; // Col
                toReturn.Y = line; // Row
                return toReturn;
            }
            set
            {
                int position = 0;
                for (int i = 0; i < value.Y; i++)
                {
                    position += Lines[i].Length + 1;
                }
                editorTextbox.SelectionStart = position + value.X;
                Point pos = CursorPosition;
                StatusText = "Cursor Position: Row " + pos.Y + ", Col " + pos.X;
            }
        }

        public string StatusText
        {
            get { return editorStatusLabel.Text; }
            set { editorStatusLabel.Text = value; }
        }

        public SymbolListbox ScriptingSymbolListbox
        {
            get { return scriptingSymbolListbox; }
        }

        public ScriptingEditor()
        {
            InitializeComponent();
            scriptingSymbolListbox.Visible = false;
            state = EditorState.Code;
            updateEditorStatus();
        }

        private void editorTextbox_KeyUp(object sender, KeyEventArgs e)
        {
            // Show cursor position in status bar for debugging
            Point pos = CursorPosition;
            //Status = "Cursor Position: Row " + pos.Y + ", Col " + pos.X;
        }

        public void InsertLine(string lineContent, int lineNum)
        {
            List<string> tmpLines = new List<string>(Lines);
            if (lineNum < 0)
            {
                tmpLines.Add(lineContent);
            }
            else
            {
                tmpLines.Insert(lineNum, lineContent);
            }
            Lines = tmpLines.ToArray();
        }

        public void SelectLine(int n)
        {
            // Selects the nth line
            CursorPosition = new Point(0, n);
            editorTextbox.SelectionLength = Lines[n].Length;
        }

        public void ShadeLine(int n)
        {
            // Checks each word in given line against keywords, assuming not a comment, and shades appropriately
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            const int WM_KEYDOWN = 0x100;
            const int WM_SYSKEYDOWN = 0x104;

            // Update timer
            lastTime = DateTime.Now;

            if ((msg.Msg == WM_KEYDOWN) || (msg.Msg == WM_SYSKEYDOWN))
            {
                if (keyData == Keys.Tab)
                {
                    if (scriptingSymbolListbox.Visible && scriptingSymbolListbox.SelectedItems.Count == 1)
                    {
                        // Insert current selection
                        int initPos = editorTextbox.SelectionStart;
                        string secondHalf = scriptingSymbolListbox.CompleteSelection(previousWord());
                        insertText(secondHalf, editorTextbox.SelectionStart);
                        shadePreviousWord(' ');
                        editorTextbox.SelectionStart = initPos + secondHalf.Length;
                        editorTextbox.SelectionLength = 0;
                    }
                    else
                    {
                        // Insert tab character
                        insertText("\t", editorTextbox.SelectionStart);
                        editorTextbox.SelectionLength = 0;
                    }
                    ResetBoxes();
                    return true;
                }
                if (keyData == Keys.Return)
                {
                    if (scriptingSymbolListbox.Visible && scriptingSymbolListbox.SelectedItems.Count == 1)
                    {
                        // Insert current selection
                        int initPos = editorTextbox.SelectionStart;
                        string secondHalf = scriptingSymbolListbox.CompleteSelection(previousWord());
                        insertText(secondHalf, editorTextbox.SelectionStart);
                        shadePreviousWord(' ');
                        editorTextbox.SelectionStart = initPos + secondHalf.Length;
                        editorTextbox.SelectionLength = 0;
                    }
                    else
                    {
                        string currentLine = Lines[CursorPosition.Y];
                        int currPos = editorTextbox.SelectionStart;
                        if (currentLine.Replace("\t", "") == "end" && state == EditorState.Code)
                        {
                            // After "end\n", remove one tab from line
                            SelectLine(CursorPosition.Y);
                            int indCount = indentationCount() - 1;
                            string newLine = "";
                            for (int i = 0; i < indCount; i++)
                            {
                                newLine += "\t";
                            }
                            newLine += editorTextbox.SelectedText.Replace("\t", "") + "\n";
                            editorTextbox.SelectedText = newLine;
                        }
                        else if (currentLine.Length > 0 && currentLine[currentLine.Length-1] == ')' && currentLine.Substring(0, 8) == "function" && state == EditorState.Code)
                        {
                            // After "function ...(...)\n", increment indentation from previous line
                            int indCount = indentationCount() + 1;
                            string newLine = "\n";
                            for (int i = 0; i < indCount; i++)
                            {
                                newLine += "\t";
                            }
                            insertText(newLine, editorTextbox.SelectionStart);
                        }
                        else
                        {
                            // For normal new line, match indentation from previous line
                            int initPos = editorTextbox.SelectionStart;
                            int indCount = indentationCount();
                            string newLine = "\n";
                            for (int i = 0; i < indCount; i++)
                            {
                                newLine += "\t";
                            }
                            insertText(newLine, editorTextbox.SelectionStart);
                            editorTextbox.SelectionStart = initPos + newLine.Length;
                            editorTextbox.SelectionLength = 0;
                        }
                        if (state == EditorState.SingleComment)
                        {
                            // End this comment with line
                            state = EditorState.Code;
                            editorTextbox.SelectionStart = currPos;
                            editorTextbox.SelectionColor = textColor;
                        }
                    }
                    ResetBoxes();
                    return true;
                }
                if (keyData == Keys.Back && state == EditorState.Code)
                {
                    // Normal backspace functionality is fine, but de-color previous word and hide boxes
                    int currPos = editorTextbox.SelectionStart;
                    selectPreviousWord();
                    editorTextbox.SelectionColor = textColor;
                    editorTextbox.SelectionStart = currPos;
                    ResetBoxes();
                }
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void updateEditorStatus()
        {
            // Updates editor status label, lights, and error messages (if time has passed since last keystroke)
            if (DateTime.Now.Subtract(lastTime).Milliseconds / 1000.0f > statusDelay)
            {
                // Try to interpret scripts
                StatusText = "Status: Interpreting...";
                bool valid = true;

                if (valid)
                {
                    // If valid, save
                    StatusText = "Status: Saved.";
                    _status = EditorStatus.Green;
                }
                else
                {
                    // Otherwise, highlight errors
                    StatusText = "Status: Errors Found";
                    _status = EditorStatus.Red;
                }
            }
            else
            {
                // Status label should instead report cursor position
                Point pos = CursorPosition;
                StatusText = "Cursor position: Row " + pos.Y + ", Col " + pos.X;
                _status = EditorStatus.Yellow;
            }
        }

        private int indentationCount()
        {
            // Calculates the degree of indentation ('\t' count) in the current line
            int currPos = editorTextbox.SelectionStart;
            string before = Lines[CursorPosition.Y];
            string after = before.Replace("\t", "");
            return before.Length - after.Length;
        }

        private void editorTextbox_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Adjust listbox status
            string key = e.KeyChar + "";
            bool flag = true;
            if (Regex.Match(key, "[a-zA-Z0-9_]").Success && state == EditorState.Code)
            {
                // Move listbox / suggestion box
                Point pos = new Point((CursorPosition.X + 1) * charWidth + 15, CursorPosition.Y * lineHeight + lineHeight / 2);
                if (parameterSuggestionBox.Visible)
                {
                    parameterSuggestionBox.Location = pos;
                }
                else
                {
                    scriptingSymbolListbox.Location = pos;
                }

                // Update selection
                string prevWord = previousWord() + key;
                scriptingSymbolListbox.HighlightBestGuess(prevWord);

                // Show listbox
                scriptingSymbolListbox.Visible = true;
            }
            else if (Regex.Match(key, "[-]").Success)
            {
                // Check for line comment beginning
                int currPos = editorTextbox.SelectionStart;
                editorTextbox.SelectionStart = editorTextbox.SelectionStart - 1;
                editorTextbox.SelectionLength = 1;
                if (editorTextbox.SelectedText == "-")
                {
                    state = EditorState.SingleComment;
                    editorTextbox.SelectionColor = commentColor;
                    ResetBoxes();
                }
                editorTextbox.SelectionStart = currPos;
                editorTextbox.SelectionLength = 0;
            }
            else if (Regex.Match(key, "[\\[]").Success)
            {
                // Check for range comment beginning
                int currPos = editorTextbox.SelectionStart;
                editorTextbox.SelectionStart = editorTextbox.SelectionStart - 3;
                editorTextbox.SelectionLength = 3;
                if (editorTextbox.SelectedText == "--[")
                {
                    state = EditorState.RangeComment;
                    editorTextbox.SelectionColor = commentColor;
                    ResetBoxes();
                }
                editorTextbox.SelectionStart = currPos;
                editorTextbox.SelectionLength = 0;
            }
            else if (Regex.Match(key, "[\\]]").Success)
            {
                // Check for range comment end
                int currPos = editorTextbox.SelectionStart;
                editorTextbox.SelectionStart = editorTextbox.SelectionStart - 3;
                editorTextbox.SelectionLength = 3;
                if (editorTextbox.SelectedText == "--]")
                {
                    state = EditorState.Code;
                }
                editorTextbox.SelectionStart = currPos;
                editorTextbox.SelectionLength = 0;
                editorTextbox.SelectionColor = textColor;
            }
            else if (Regex.Match(key, "[:]").Success && state == EditorState.Code)
            {
                // Limit listbox to sub-namespace symbols
                string prevWord = previousWord();
                scriptingSymbolListbox.LimitToNamespace(prevWord);
            }
            else if (Regex.Match(key, "[\\(]").Success && state == EditorState.Code)
            {
                // Move suggestion box to current position
                Point pos = new Point((CursorPosition.X + 1) * charWidth + 15, CursorPosition.Y * lineHeight + lineHeight / 2);
                parameterSuggestionBox.Location = pos;

                // Display function arguments
                parameterSuggestionBox.LoadFunction("Double getValueAtTime(String SubsystemKey, Double time)");
                parameterSuggestionBox.CurrentParameter = 0;
                parameterSuggestionBox.Visible = true;

                // Hide symbol box
                scriptingSymbolListbox.Visible = false;

                // Reset symbol namespace
                scriptingSymbolListbox.ResetNamespace();

                // Shade previous word
                shadePreviousWord(' ');
            }
            else if (Regex.Match(key, "[,]").Success && state == EditorState.Code)
            {
                if (parameterSuggestionBox.Visible)
                {
                    // If in function parameter, advance suggestion index and position
                    parameterSuggestionBox.CurrentParameter++;
                    Point pos = new Point((CursorPosition.X + 1) * charWidth + 15, CursorPosition.Y * lineHeight + lineHeight / 2);
                    parameterSuggestionBox.Location = pos;
                }
                else
                {
                    // Otherwise, reset and hide
                    scriptingSymbolListbox.Visible = false;
                    scriptingSymbolListbox.ResetNamespace();
                }
            }
            else if (Regex.Match(key, "[\\)]").Success && state == EditorState.Code)
            {
                // Hide function arguments
                parameterSuggestionBox.Visible = false;

                // Shade previous word
                shadePreviousWord(' ');

                // Reset scope
                scriptingSymbolListbox.ResetNamespace();
            }
            else if (Regex.Match(key, "[\\s]").Success && state == EditorState.Code)
            {
                // Shade previous word
                shadePreviousWord(e.KeyChar);
                ResetBoxes();
            }
            else
            {
                // Reset and hide both boxes
                scriptingSymbolListbox.Visible = false;
                ResetBoxes();
            }
            if (flag) { flag = true; }
        }

        private void shadePreviousWord(char keyPressed)
        {
            // Shades the previous word, including keyPressed if not a space
            int currPos = editorTextbox.SelectionStart;
            selectPreviousWord();
            string word = keyPressed == ' ' ? editorTextbox.SelectedText : editorTextbox.SelectedText + keyPressed;
            SymbolInfo info = scriptingSymbolListbox.FindSymbolInfo(word);
            editorTextbox.SelectionColor = info.Shade;
            editorTextbox.SelectionStart = currPos;
            editorTextbox.SelectionLength = 0;
            editorTextbox.SelectionColor = textColor;
        }

        private void insertText(string txt, int pos)
        {
            // Inserts given string at given location
            if (txt == "") { return; }
            int currPos = editorTextbox.SelectionStart;
            editorTextbox.Select(pos, editorTextbox.Text.Length - pos);
            txt += editorTextbox.SelectedText;
            editorTextbox.SelectedText = "";
            editorTextbox.AppendText(txt);
            editorTextbox.SelectionLength = 0;
            editorTextbox.SelectionStart = currPos + txt.Length;
        }

        private string previousWord()
        {
            // Returns the word before the current position
            if (editorTextbox.SelectionStart == 0) { return ""; }
            bool wasReturn = false;
            int initPos = editorTextbox.SelectionStart;
            int currPos = editorTextbox.SelectionStart - 1;
            editorTextbox.Select(currPos, initPos - currPos);
            string word = editorTextbox.SelectedText;
            if (Regex.Match(word, @"^[\n\r]$").Success)
            {
                // Most recent key was enter / return; look behind that
                wasReturn = true;
                initPos--;
                currPos--;
                editorTextbox.Select(currPos, initPos - currPos);
                word = editorTextbox.SelectedText;
            }
            while (Regex.Match(word, "^[a-zA-Z0-9_]+$").Success && currPos > 0)
            {
                currPos--;
                editorTextbox.Select(currPos, initPos - currPos);
                word = editorTextbox.SelectedText;
            }
            if (currPos > 0)
            {
                currPos++;
                editorTextbox.Select(currPos, initPos - currPos);
                word = editorTextbox.SelectedText;
            }
            if (wasReturn)
            {
                initPos++;
            }
            editorTextbox.SelectionStart = initPos;
            editorTextbox.SelectionLength = 0;
            return word;
        }

        private void selectPreviousWord()
        {
            // Selects the previous typed word
            if (editorTextbox.SelectionStart == 0) { return; }
            int initPos = editorTextbox.SelectionStart;
            int currPos = editorTextbox.SelectionStart - 1;
            editorTextbox.Select(currPos, initPos - currPos);
            string word = editorTextbox.SelectedText;
            if (Regex.Match(word, @"^[\n\r]$").Success)
            {
                // Most recent key was enter / return; look behind that
                initPos--;
                currPos--;
                editorTextbox.Select(currPos, initPos - currPos);
                word = editorTextbox.SelectedText;
            }
            while (Regex.Match(word, "^[a-zA-Z0-9_]+$").Success && currPos > 0)
            {
                currPos--;
                editorTextbox.Select(currPos, initPos - currPos);
                word = editorTextbox.SelectedText;
            }
            if (currPos > 0)
            {
                currPos++;
                editorTextbox.Select(currPos, initPos - currPos);
            }
        }

        public void ResetBoxes()
        {
            // Resets listbox (ie, if user clicks to another location, presses esc, etc)
            scriptingSymbolListbox.UnselectAll();
            scriptingSymbolListbox.ResetNamespace();
            scriptingSymbolListbox.Visible = false;
            parameterSuggestionBox.Visible = false;
        }
    }

    public enum EditorState
    {
        Code,
        SingleComment,
        RangeComment
    }

    public enum EditorStatus
    {
        Green,
        Yellow,
        Red
    }
}
