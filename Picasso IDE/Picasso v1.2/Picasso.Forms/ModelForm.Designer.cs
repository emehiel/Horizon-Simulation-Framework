namespace Picasso.Forms
{
    partial class ModelForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.luaGroup = new System.Windows.Forms.GroupBox();
            this.luaFilesRemoveButton = new System.Windows.Forms.Button();
            this.luaFilesAddButton = new System.Windows.Forms.Button();
            this.luaFilesAddTextbox = new System.Windows.Forms.TextBox();
            this.luaFilesLabel = new System.Windows.Forms.Label();
            this.luaFilesListbox = new System.Windows.Forms.ListBox();
            this.isScripted = new System.Windows.Forms.CheckBox();
            this.schEvalGroup = new System.Windows.Forms.GroupBox();
            this.evalFuncLabel = new System.Windows.Forms.Label();
            this.evalFunctionTextbox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.typeComboBox = new System.Windows.Forms.ComboBox();
            this.luaGroup.SuspendLayout();
            this.schEvalGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // luaGroup
            // 
            this.luaGroup.Controls.Add(this.luaFilesRemoveButton);
            this.luaGroup.Controls.Add(this.luaFilesAddButton);
            this.luaGroup.Controls.Add(this.luaFilesAddTextbox);
            this.luaGroup.Controls.Add(this.luaFilesLabel);
            this.luaGroup.Controls.Add(this.luaFilesListbox);
            this.luaGroup.Controls.Add(this.isScripted);
            this.luaGroup.Location = new System.Drawing.Point(12, 12);
            this.luaGroup.Name = "luaGroup";
            this.luaGroup.Size = new System.Drawing.Size(318, 154);
            this.luaGroup.TabIndex = 0;
            this.luaGroup.TabStop = false;
            this.luaGroup.Text = "Lua";
            // 
            // luaFilesRemoveButton
            // 
            this.luaFilesRemoveButton.Location = new System.Drawing.Point(52, 62);
            this.luaFilesRemoveButton.Name = "luaFilesRemoveButton";
            this.luaFilesRemoveButton.Size = new System.Drawing.Size(107, 23);
            this.luaFilesRemoveButton.TabIndex = 5;
            this.luaFilesRemoveButton.Text = "Remove Selected";
            this.luaFilesRemoveButton.UseVisualStyleBackColor = true;
            this.luaFilesRemoveButton.Click += new System.EventHandler(this.luaFilesRemoveButton_Click);
            // 
            // luaFilesAddButton
            // 
            this.luaFilesAddButton.Location = new System.Drawing.Point(52, 117);
            this.luaFilesAddButton.Name = "luaFilesAddButton";
            this.luaFilesAddButton.Size = new System.Drawing.Size(108, 23);
            this.luaFilesAddButton.TabIndex = 4;
            this.luaFilesAddButton.Text = "Add Script";
            this.luaFilesAddButton.UseVisualStyleBackColor = true;
            this.luaFilesAddButton.Click += new System.EventHandler(this.luaFilesAddButton_Click);
            // 
            // luaFilesAddTextbox
            // 
            this.luaFilesAddTextbox.Location = new System.Drawing.Point(52, 91);
            this.luaFilesAddTextbox.Name = "luaFilesAddTextbox";
            this.luaFilesAddTextbox.Size = new System.Drawing.Size(107, 20);
            this.luaFilesAddTextbox.TabIndex = 3;
            // 
            // luaFilesLabel
            // 
            this.luaFilesLabel.AutoSize = true;
            this.luaFilesLabel.Location = new System.Drawing.Point(111, 45);
            this.luaFilesLabel.Name = "luaFilesLabel";
            this.luaFilesLabel.Size = new System.Drawing.Size(49, 13);
            this.luaFilesLabel.TabIndex = 2;
            this.luaFilesLabel.Text = "Lua Files";
            // 
            // luaFilesListbox
            // 
            this.luaFilesListbox.FormattingEnabled = true;
            this.luaFilesListbox.Location = new System.Drawing.Point(166, 45);
            this.luaFilesListbox.Name = "luaFilesListbox";
            this.luaFilesListbox.Size = new System.Drawing.Size(120, 95);
            this.luaFilesListbox.TabIndex = 1;
            // 
            // isScripted
            // 
            this.isScripted.AutoSize = true;
            this.isScripted.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.isScripted.Location = new System.Drawing.Point(78, 19);
            this.isScripted.Name = "isScripted";
            this.isScripted.Size = new System.Drawing.Size(103, 17);
            this.isScripted.TabIndex = 0;
            this.isScripted.Text = "Enable Scripting";
            this.isScripted.UseVisualStyleBackColor = true;
            this.isScripted.CheckedChanged += new System.EventHandler(this.isScripted_CheckedChanged);
            // 
            // schEvalGroup
            // 
            this.schEvalGroup.Controls.Add(this.evalFuncLabel);
            this.schEvalGroup.Controls.Add(this.evalFunctionTextbox);
            this.schEvalGroup.Controls.Add(this.label1);
            this.schEvalGroup.Controls.Add(this.typeComboBox);
            this.schEvalGroup.Location = new System.Drawing.Point(12, 172);
            this.schEvalGroup.Name = "schEvalGroup";
            this.schEvalGroup.Size = new System.Drawing.Size(318, 78);
            this.schEvalGroup.TabIndex = 1;
            this.schEvalGroup.TabStop = false;
            this.schEvalGroup.Text = "Schedule Evaluator";
            // 
            // evalFuncLabel
            // 
            this.evalFuncLabel.AutoSize = true;
            this.evalFuncLabel.Location = new System.Drawing.Point(59, 49);
            this.evalFuncLabel.Name = "evalFuncLabel";
            this.evalFuncLabel.Size = new System.Drawing.Size(101, 13);
            this.evalFuncLabel.TabIndex = 3;
            this.evalFuncLabel.Text = "Evaluation Function";
            // 
            // evalFunctionTextbox
            // 
            this.evalFunctionTextbox.Location = new System.Drawing.Point(166, 46);
            this.evalFunctionTextbox.Name = "evalFunctionTextbox";
            this.evalFunctionTextbox.Size = new System.Drawing.Size(121, 20);
            this.evalFunctionTextbox.TabIndex = 2;
            this.evalFunctionTextbox.TextChanged += new System.EventHandler(this.evalFunctionTextbox_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(129, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Type";
            // 
            // typeComboBox
            // 
            this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeComboBox.FormattingEnabled = true;
            this.typeComboBox.Location = new System.Drawing.Point(166, 19);
            this.typeComboBox.Name = "typeComboBox";
            this.typeComboBox.Size = new System.Drawing.Size(121, 21);
            this.typeComboBox.TabIndex = 0;
            this.typeComboBox.SelectedIndexChanged += new System.EventHandler(this.typeComboBox_SelectedIndexChanged);
            // 
            // ModelForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 263);
            this.Controls.Add(this.schEvalGroup);
            this.Controls.Add(this.luaGroup);
            this.Name = "ModelForm";
            this.Text = "ModelForm";
            this.luaGroup.ResumeLayout(false);
            this.luaGroup.PerformLayout();
            this.schEvalGroup.ResumeLayout(false);
            this.schEvalGroup.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox luaGroup;
        private System.Windows.Forms.CheckBox isScripted;
        private System.Windows.Forms.GroupBox schEvalGroup;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox typeComboBox;
        private System.Windows.Forms.Label evalFuncLabel;
        private System.Windows.Forms.TextBox evalFunctionTextbox;
        private System.Windows.Forms.Button luaFilesRemoveButton;
        private System.Windows.Forms.Button luaFilesAddButton;
        private System.Windows.Forms.TextBox luaFilesAddTextbox;
        private System.Windows.Forms.Label luaFilesLabel;
        private System.Windows.Forms.ListBox luaFilesListbox;
    }
}