namespace evrs_php
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tb_Ip = new System.Windows.Forms.TextBox();
            this.lbl_Ip = new System.Windows.Forms.Label();
            this.btn_Query = new System.Windows.Forms.Button();
            this.btn_Login = new System.Windows.Forms.Button();
            this.tb_rpckey = new System.Windows.Forms.TextBox();
            this.lbl_UserName = new System.Windows.Forms.Label();
            this.lbl_Pwd = new System.Windows.Forms.Label();
            this.groupbox_Login = new System.Windows.Forms.GroupBox();
            this.btn_Test = new System.Windows.Forms.Button();
            this.btn_Quit = new System.Windows.Forms.Button();
            this.groupbox_Query = new System.Windows.Forms.GroupBox();
            this.cob_Direct = new System.Windows.Forms.ComboBox();
            this.tb_RemoteCode = new System.Windows.Forms.TextBox();
            this.tb_MinTalkLong = new System.Windows.Forms.TextBox();
            this.tb_MaxTalkLong = new System.Windows.Forms.TextBox();
            this.lbl_Direct = new System.Windows.Forms.Label();
            this.tb_PageLimit = new System.Windows.Forms.TextBox();
            this.lbl_MinTalkLong = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.lbl_MaxTalkLong = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_PageStart = new System.Windows.Forms.TextBox();
            this.tb_LocalCode = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.date_EndDate = new System.Windows.Forms.DateTimePicker();
            this.tb_DeviceId = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.lbl_DeviceId = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.date_StateDate = new System.Windows.Forms.DateTimePicker();
            this.btn_Download = new System.Windows.Forms.Button();
            this.btn_Delete = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.bdnInfo = new System.Windows.Forms.BindingNavigator(this.components);
            this.tslbl_TotalRecords = new System.Windows.Forms.ToolStripLabel();
            this.tslbl_TotalCount = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbtn_Pre = new System.Windows.Forms.ToolStripButton();
            this.tstb_CurrentPage = new System.Windows.Forms.ToolStripTextBox();
            this.tslbl_Split = new System.Windows.Forms.ToolStripLabel();
            this.tslbl_PageCount = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbtn_Next = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbtn_Skip = new System.Windows.Forms.ToolStripButton();
            this.dgv_RecData = new System.Windows.Forms.DataGridView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.dataGridViewCh = new System.Windows.Forms.DataGridView();
            this.bdsInfo = new System.Windows.Forms.BindingSource(this.components);
            this.toolStripMenu_DownLoad = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenu_Delete = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenu_RecData = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tb_Pwd = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btQueryChannel = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.txtLocalCode = new System.Windows.Forms.TextBox();
            this.lable8 = new System.Windows.Forms.Label();
            this.txtChannel = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.txtLocalName = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.txtLocalDepartment = new System.Windows.Forms.TextBox();
            this.btSetChannel = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.tb_LocalName = new System.Windows.Forms.TextBox();
            this.groupbox_Login.SuspendLayout();
            this.groupbox_Query.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.bdnInfo)).BeginInit();
            this.bdnInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgv_RecData)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCh)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.bdsInfo)).BeginInit();
            this.contextMenu_RecData.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tb_Ip
            // 
            this.tb_Ip.Location = new System.Drawing.Point(88, 23);
            this.tb_Ip.Name = "tb_Ip";
            this.tb_Ip.Size = new System.Drawing.Size(95, 21);
            this.tb_Ip.TabIndex = 1;
            this.tb_Ip.Text = "127.0.0.1";
            // 
            // lbl_Ip
            // 
            this.lbl_Ip.AutoSize = true;
            this.lbl_Ip.Location = new System.Drawing.Point(11, 29);
            this.lbl_Ip.Name = "lbl_Ip";
            this.lbl_Ip.Size = new System.Drawing.Size(47, 12);
            this.lbl_Ip.TabIndex = 2;
            this.lbl_Ip.Text = "设备IP:";
            this.lbl_Ip.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btn_Query
            // 
            this.btn_Query.Location = new System.Drawing.Point(527, 112);
            this.btn_Query.Name = "btn_Query";
            this.btn_Query.Size = new System.Drawing.Size(62, 23);
            this.btn_Query.TabIndex = 12;
            this.btn_Query.Text = "查询(&Q)";
            this.btn_Query.UseVisualStyleBackColor = true;
            this.btn_Query.Click += new System.EventHandler(this.btn_Query_Click);
            // 
            // btn_Login
            // 
            this.btn_Login.Location = new System.Drawing.Point(436, 397);
            this.btn_Login.Name = "btn_Login";
            this.btn_Login.Size = new System.Drawing.Size(62, 23);
            this.btn_Login.TabIndex = 4;
            this.btn_Login.Text = "登录(&L)";
            this.btn_Login.UseVisualStyleBackColor = true;
            this.btn_Login.Visible = false;
            this.btn_Login.Click += new System.EventHandler(this.btn_Login_Click);
            // 
            // tb_rpckey
            // 
            this.tb_rpckey.Location = new System.Drawing.Point(88, 60);
            this.tb_rpckey.Name = "tb_rpckey";
            this.tb_rpckey.Size = new System.Drawing.Size(85, 21);
            this.tb_rpckey.TabIndex = 2;
            this.tb_rpckey.Text = "VocLogRpcKey";
            // 
            // lbl_UserName
            // 
            this.lbl_UserName.AutoSize = true;
            this.lbl_UserName.Location = new System.Drawing.Point(11, 60);
            this.lbl_UserName.Name = "lbl_UserName";
            this.lbl_UserName.Size = new System.Drawing.Size(71, 12);
            this.lbl_UserName.TabIndex = 2;
            this.lbl_UserName.Text = "RPCCallKey:";
            // 
            // lbl_Pwd
            // 
            this.lbl_Pwd.AutoSize = true;
            this.lbl_Pwd.Location = new System.Drawing.Point(-375, 1);
            this.lbl_Pwd.Name = "lbl_Pwd";
            this.lbl_Pwd.Size = new System.Drawing.Size(35, 12);
            this.lbl_Pwd.TabIndex = 2;
            this.lbl_Pwd.Text = "密码:";
            this.lbl_Pwd.Visible = false;
            // 
            // groupbox_Login
            // 
            this.groupbox_Login.Controls.Add(this.btn_Test);
            this.groupbox_Login.Controls.Add(this.lbl_Ip);
            this.groupbox_Login.Controls.Add(this.tb_rpckey);
            this.groupbox_Login.Controls.Add(this.tb_Ip);
            this.groupbox_Login.Controls.Add(this.lbl_UserName);
            this.groupbox_Login.Location = new System.Drawing.Point(8, 443);
            this.groupbox_Login.Name = "groupbox_Login";
            this.groupbox_Login.Size = new System.Drawing.Size(255, 150);
            this.groupbox_Login.TabIndex = 6;
            this.groupbox_Login.TabStop = false;
            this.groupbox_Login.Text = "登录";
            // 
            // btn_Test
            // 
            this.btn_Test.Location = new System.Drawing.Point(164, 121);
            this.btn_Test.Name = "btn_Test";
            this.btn_Test.Size = new System.Drawing.Size(62, 23);
            this.btn_Test.TabIndex = 5;
            this.btn_Test.Text = "测试(&T)";
            this.btn_Test.UseVisualStyleBackColor = true;
            this.btn_Test.Click += new System.EventHandler(this.btn_Test_Click);
            // 
            // btn_Quit
            // 
            this.btn_Quit.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btn_Quit.Location = new System.Drawing.Point(510, 397);
            this.btn_Quit.Name = "btn_Quit";
            this.btn_Quit.Size = new System.Drawing.Size(62, 23);
            this.btn_Quit.TabIndex = 5;
            this.btn_Quit.Text = "退出(&X)";
            this.btn_Quit.UseVisualStyleBackColor = true;
            this.btn_Quit.Visible = false;
            this.btn_Quit.Click += new System.EventHandler(this.btn_Quit_Click);
            // 
            // groupbox_Query
            // 
            this.groupbox_Query.Controls.Add(this.cob_Direct);
            this.groupbox_Query.Controls.Add(this.btn_Query);
            this.groupbox_Query.Controls.Add(this.tb_LocalName);
            this.groupbox_Query.Controls.Add(this.tb_RemoteCode);
            this.groupbox_Query.Controls.Add(this.tb_MinTalkLong);
            this.groupbox_Query.Controls.Add(this.tb_MaxTalkLong);
            this.groupbox_Query.Controls.Add(this.lbl_Direct);
            this.groupbox_Query.Controls.Add(this.tb_PageLimit);
            this.groupbox_Query.Controls.Add(this.label10);
            this.groupbox_Query.Controls.Add(this.lbl_MinTalkLong);
            this.groupbox_Query.Controls.Add(this.label1);
            this.groupbox_Query.Controls.Add(this.lbl_MaxTalkLong);
            this.groupbox_Query.Controls.Add(this.label5);
            this.groupbox_Query.Controls.Add(this.tb_PageStart);
            this.groupbox_Query.Controls.Add(this.tb_LocalCode);
            this.groupbox_Query.Controls.Add(this.label4);
            this.groupbox_Query.Controls.Add(this.date_EndDate);
            this.groupbox_Query.Controls.Add(this.tb_DeviceId);
            this.groupbox_Query.Controls.Add(this.label2);
            this.groupbox_Query.Controls.Add(this.lbl_DeviceId);
            this.groupbox_Query.Controls.Add(this.label6);
            this.groupbox_Query.Controls.Add(this.label3);
            this.groupbox_Query.Controls.Add(this.date_StateDate);
            this.groupbox_Query.Location = new System.Drawing.Point(271, 444);
            this.groupbox_Query.Name = "groupbox_Query";
            this.groupbox_Query.Size = new System.Drawing.Size(612, 143);
            this.groupbox_Query.TabIndex = 7;
            this.groupbox_Query.TabStop = false;
            this.groupbox_Query.Text = "查询";
            // 
            // cob_Direct
            // 
            this.cob_Direct.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cob_Direct.FormattingEnabled = true;
            this.cob_Direct.Items.AddRange(new object[] {
            "呼入",
            "呼出"});
            this.cob_Direct.Location = new System.Drawing.Point(513, 88);
            this.cob_Direct.Name = "cob_Direct";
            this.cob_Direct.Size = new System.Drawing.Size(85, 20);
            this.cob_Direct.TabIndex = 13;
            // 
            // tb_RemoteCode
            // 
            this.tb_RemoteCode.Location = new System.Drawing.Point(78, 56);
            this.tb_RemoteCode.Name = "tb_RemoteCode";
            this.tb_RemoteCode.Size = new System.Drawing.Size(125, 21);
            this.tb_RemoteCode.TabIndex = 8;
            // 
            // tb_MinTalkLong
            // 
            this.tb_MinTalkLong.Location = new System.Drawing.Point(127, 114);
            this.tb_MinTalkLong.Name = "tb_MinTalkLong";
            this.tb_MinTalkLong.Size = new System.Drawing.Size(85, 21);
            this.tb_MinTalkLong.TabIndex = 8;
            // 
            // tb_MaxTalkLong
            // 
            this.tb_MaxTalkLong.Location = new System.Drawing.Point(336, 114);
            this.tb_MaxTalkLong.Name = "tb_MaxTalkLong";
            this.tb_MaxTalkLong.Size = new System.Drawing.Size(85, 21);
            this.tb_MaxTalkLong.TabIndex = 8;
            // 
            // lbl_Direct
            // 
            this.lbl_Direct.AutoSize = true;
            this.lbl_Direct.Location = new System.Drawing.Point(446, 92);
            this.lbl_Direct.Name = "lbl_Direct";
            this.lbl_Direct.Size = new System.Drawing.Size(59, 12);
            this.lbl_Direct.TabIndex = 2;
            this.lbl_Direct.Text = "呼叫方向:";
            // 
            // tb_PageLimit
            // 
            this.tb_PageLimit.Location = new System.Drawing.Point(513, 54);
            this.tb_PageLimit.Name = "tb_PageLimit";
            this.tb_PageLimit.Size = new System.Drawing.Size(85, 21);
            this.tb_PageLimit.TabIndex = 12;
            this.tb_PageLimit.Text = "50";
            // 
            // lbl_MinTalkLong
            // 
            this.lbl_MinTalkLong.AutoSize = true;
            this.lbl_MinTalkLong.Location = new System.Drawing.Point(15, 117);
            this.lbl_MinTalkLong.Name = "lbl_MinTalkLong";
            this.lbl_MinTalkLong.Size = new System.Drawing.Size(107, 12);
            this.lbl_MinTalkLong.TabIndex = 2;
            this.lbl_MinTalkLong.Text = "最短通话时长(秒):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 59);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "对方号码:";
            // 
            // lbl_MaxTalkLong
            // 
            this.lbl_MaxTalkLong.AutoSize = true;
            this.lbl_MaxTalkLong.Location = new System.Drawing.Point(227, 117);
            this.lbl_MaxTalkLong.Name = "lbl_MaxTalkLong";
            this.lbl_MaxTalkLong.Size = new System.Drawing.Size(107, 12);
            this.lbl_MaxTalkLong.TabIndex = 2;
            this.lbl_MaxTalkLong.Text = "最长通话时长(秒):";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(436, 57);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 12);
            this.label5.TabIndex = 2;
            this.label5.Text = "每页显示数:";
            // 
            // tb_PageStart
            // 
            this.tb_PageStart.Location = new System.Drawing.Point(513, 25);
            this.tb_PageStart.Name = "tb_PageStart";
            this.tb_PageStart.Size = new System.Drawing.Size(85, 21);
            this.tb_PageStart.TabIndex = 11;
            this.tb_PageStart.Text = "1";
            // 
            // tb_LocalCode
            // 
            this.tb_LocalCode.Location = new System.Drawing.Point(292, 23);
            this.tb_LocalCode.Name = "tb_LocalCode";
            this.tb_LocalCode.Size = new System.Drawing.Size(129, 21);
            this.tb_LocalCode.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(448, 28);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 2;
            this.label4.Text = "起始页:";
            // 
            // date_EndDate
            // 
            this.date_EndDate.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.date_EndDate.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.date_EndDate.Location = new System.Drawing.Point(292, 87);
            this.date_EndDate.Name = "date_EndDate";
            this.date_EndDate.Size = new System.Drawing.Size(150, 21);
            this.date_EndDate.TabIndex = 10;
            // 
            // tb_DeviceId
            // 
            this.tb_DeviceId.Location = new System.Drawing.Point(80, 23);
            this.tb_DeviceId.Name = "tb_DeviceId";
            this.tb_DeviceId.Size = new System.Drawing.Size(123, 21);
            this.tb_DeviceId.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(227, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "本地号码:";
            // 
            // lbl_DeviceId
            // 
            this.lbl_DeviceId.AutoSize = true;
            this.lbl_DeviceId.Location = new System.Drawing.Point(14, 26);
            this.lbl_DeviceId.Name = "lbl_DeviceId";
            this.lbl_DeviceId.Size = new System.Drawing.Size(47, 12);
            this.lbl_DeviceId.TabIndex = 2;
            this.lbl_DeviceId.Text = "设备ID:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(227, 90);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(59, 12);
            this.label6.TabIndex = 2;
            this.label6.Text = "结束日期:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 2;
            this.label3.Text = "开始日期:";
            // 
            // date_StateDate
            // 
            this.date_StateDate.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.date_StateDate.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.date_StateDate.Location = new System.Drawing.Point(80, 87);
            this.date_StateDate.Name = "date_StateDate";
            this.date_StateDate.Size = new System.Drawing.Size(141, 21);
            this.date_StateDate.TabIndex = 9;
            // 
            // btn_Download
            // 
            this.btn_Download.Location = new System.Drawing.Point(867, 415);
            this.btn_Download.Name = "btn_Download";
            this.btn_Download.Size = new System.Drawing.Size(62, 23);
            this.btn_Download.TabIndex = 12;
            this.btn_Download.Text = "下载(&A)";
            this.btn_Download.UseVisualStyleBackColor = true;
            this.btn_Download.Visible = false;
            this.btn_Download.Click += new System.EventHandler(this.btn_Download_Click);
            // 
            // btn_Delete
            // 
            this.btn_Delete.Location = new System.Drawing.Point(762, 373);
            this.btn_Delete.Name = "btn_Delete";
            this.btn_Delete.Size = new System.Drawing.Size(62, 23);
            this.btn_Delete.TabIndex = 12;
            this.btn_Delete.Text = "删除(&D)";
            this.btn_Delete.UseVisualStyleBackColor = true;
            this.btn_Delete.Visible = false;
            this.btn_Delete.Click += new System.EventHandler(this.btn_Delete_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tabControl1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1152, 434);
            this.panel1.TabIndex = 9;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1146, 428);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.bdnInfo);
            this.tabPage1.Controls.Add(this.btn_Delete);
            this.tabPage1.Controls.Add(this.dgv_RecData);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1138, 402);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "通话记录";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // bdnInfo
            // 
            this.bdnInfo.AddNewItem = null;
            this.bdnInfo.CountItem = null;
            this.bdnInfo.DeleteItem = null;
            this.bdnInfo.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.bdnInfo.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tslbl_TotalRecords,
            this.tslbl_TotalCount,
            this.toolStripSeparator2,
            this.tsbtn_Pre,
            this.tstb_CurrentPage,
            this.tslbl_Split,
            this.tslbl_PageCount,
            this.toolStripSeparator1,
            this.tsbtn_Next,
            this.toolStripSeparator3,
            this.tsbtn_Skip});
            this.bdnInfo.Location = new System.Drawing.Point(3, 374);
            this.bdnInfo.MoveFirstItem = null;
            this.bdnInfo.MoveLastItem = null;
            this.bdnInfo.MoveNextItem = null;
            this.bdnInfo.MovePreviousItem = null;
            this.bdnInfo.Name = "bdnInfo";
            this.bdnInfo.PositionItem = null;
            this.bdnInfo.Size = new System.Drawing.Size(1132, 25);
            this.bdnInfo.TabIndex = 3;
            this.bdnInfo.Text = "bindingNavigator1";
            // 
            // tslbl_TotalRecords
            // 
            this.tslbl_TotalRecords.Name = "tslbl_TotalRecords";
            this.tslbl_TotalRecords.Size = new System.Drawing.Size(47, 22);
            this.tslbl_TotalRecords.Text = "总记录:";
            // 
            // tslbl_TotalCount
            // 
            this.tslbl_TotalCount.Name = "tslbl_TotalCount";
            this.tslbl_TotalCount.Size = new System.Drawing.Size(15, 22);
            this.tslbl_TotalCount.Text = "0";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // tsbtn_Pre
            // 
            this.tsbtn_Pre.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.tsbtn_Pre.Image = ((System.Drawing.Image)(resources.GetObject("tsbtn_Pre.Image")));
            this.tsbtn_Pre.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbtn_Pre.Name = "tsbtn_Pre";
            this.tsbtn_Pre.Size = new System.Drawing.Size(48, 22);
            this.tsbtn_Pre.Text = "上一页";
            // 
            // tstb_CurrentPage
            // 
            this.tstb_CurrentPage.Name = "tstb_CurrentPage";
            this.tstb_CurrentPage.Size = new System.Drawing.Size(50, 25);
            this.tstb_CurrentPage.Text = "0";
            // 
            // tslbl_Split
            // 
            this.tslbl_Split.Name = "tslbl_Split";
            this.tslbl_Split.Size = new System.Drawing.Size(13, 22);
            this.tslbl_Split.Text = "/";
            // 
            // tslbl_PageCount
            // 
            this.tslbl_PageCount.Name = "tslbl_PageCount";
            this.tslbl_PageCount.Size = new System.Drawing.Size(15, 22);
            this.tslbl_PageCount.Text = "0";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tsbtn_Next
            // 
            this.tsbtn_Next.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.tsbtn_Next.Image = ((System.Drawing.Image)(resources.GetObject("tsbtn_Next.Image")));
            this.tsbtn_Next.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbtn_Next.Name = "tsbtn_Next";
            this.tsbtn_Next.Size = new System.Drawing.Size(48, 22);
            this.tsbtn_Next.Text = "下一页";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // tsbtn_Skip
            // 
            this.tsbtn_Skip.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.tsbtn_Skip.Image = ((System.Drawing.Image)(resources.GetObject("tsbtn_Skip.Image")));
            this.tsbtn_Skip.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbtn_Skip.Name = "tsbtn_Skip";
            this.tsbtn_Skip.Size = new System.Drawing.Size(36, 22);
            this.tsbtn_Skip.Text = "跳转";
            // 
            // dgv_RecData
            // 
            this.dgv_RecData.AllowUserToAddRows = false;
            this.dgv_RecData.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.dgv_RecData.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dgv_RecData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgv_RecData.Dock = System.Windows.Forms.DockStyle.Top;
            this.dgv_RecData.Location = new System.Drawing.Point(3, 3);
            this.dgv_RecData.MultiSelect = false;
            this.dgv_RecData.Name = "dgv_RecData";
            this.dgv_RecData.ReadOnly = true;
            this.dgv_RecData.RowTemplate.Height = 23;
            this.dgv_RecData.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgv_RecData.Size = new System.Drawing.Size(1132, 366);
            this.dgv_RecData.TabIndex = 2;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.dataGridViewCh);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1138, 402);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "通道";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // dataGridViewCh
            // 
            this.dataGridViewCh.AllowUserToAddRows = false;
            this.dataGridViewCh.AllowUserToResizeRows = false;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.dataGridViewCh.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewCh.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewCh.Dock = System.Windows.Forms.DockStyle.Top;
            this.dataGridViewCh.Location = new System.Drawing.Point(3, 3);
            this.dataGridViewCh.MultiSelect = false;
            this.dataGridViewCh.Name = "dataGridViewCh";
            this.dataGridViewCh.ReadOnly = true;
            this.dataGridViewCh.RowTemplate.Height = 23;
            this.dataGridViewCh.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewCh.Size = new System.Drawing.Size(1132, 366);
            this.dataGridViewCh.TabIndex = 3;
            // 
            // toolStripMenu_DownLoad
            // 
            this.toolStripMenu_DownLoad.Image = global::evrs_php.Properties.Resources.download;
            this.toolStripMenu_DownLoad.Name = "toolStripMenu_DownLoad";
            this.toolStripMenu_DownLoad.Size = new System.Drawing.Size(117, 22);
            this.toolStripMenu_DownLoad.Text = "下载(&A)";
            this.toolStripMenu_DownLoad.Click += new System.EventHandler(this.toolStripMenu_DownLoad_Click);
            // 
            // toolStripMenu_Delete
            // 
            this.toolStripMenu_Delete.Image = global::evrs_php.Properties.Resources.delete;
            this.toolStripMenu_Delete.Name = "toolStripMenu_Delete";
            this.toolStripMenu_Delete.Size = new System.Drawing.Size(117, 22);
            this.toolStripMenu_Delete.Text = "删除(&D)";
            this.toolStripMenu_Delete.Click += new System.EventHandler(this.toolStripMenu_Delete_Click);
            // 
            // contextMenu_RecData
            // 
            this.contextMenu_RecData.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenu_DownLoad,
            this.toolStripMenu_Delete});
            this.contextMenu_RecData.Name = "contextMenu_RecData";
            this.contextMenu_RecData.Size = new System.Drawing.Size(118, 48);
            // 
            // tb_Pwd
            // 
            this.tb_Pwd.Location = new System.Drawing.Point(-322, -2);
            this.tb_Pwd.Name = "tb_Pwd";
            this.tb_Pwd.PasswordChar = '*';
            this.tb_Pwd.Size = new System.Drawing.Size(95, 21);
            this.tb_Pwd.TabIndex = 3;
            this.tb_Pwd.Text = "123456";
            this.tb_Pwd.Visible = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btSetChannel);
            this.groupBox1.Controls.Add(this.btQueryChannel);
            this.groupBox1.Controls.Add(this.tb_Pwd);
            this.groupBox1.Controls.Add(this.lbl_Pwd);
            this.groupBox1.Controls.Add(this.txtLocalDepartment);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.txtLocalName);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.txtChannel);
            this.groupBox1.Controls.Add(this.lable8);
            this.groupBox1.Controls.Add(this.txtLocalCode);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Location = new System.Drawing.Point(889, 444);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(247, 135);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "通道";
            // 
            // btQueryChannel
            // 
            this.btQueryChannel.Location = new System.Drawing.Point(154, 106);
            this.btQueryChannel.Name = "btQueryChannel";
            this.btQueryChannel.Size = new System.Drawing.Size(93, 23);
            this.btQueryChannel.TabIndex = 0;
            this.btQueryChannel.Text = "查询通道信息";
            this.btQueryChannel.UseVisualStyleBackColor = true;
            this.btQueryChannel.Click += new System.EventHandler(this.btQueryChannel_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(104, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(59, 12);
            this.label7.TabIndex = 2;
            this.label7.Text = "本地号码:";
            // 
            // txtLocalCode
            // 
            this.txtLocalCode.Location = new System.Drawing.Point(161, 20);
            this.txtLocalCode.Name = "txtLocalCode";
            this.txtLocalCode.Size = new System.Drawing.Size(76, 21);
            this.txtLocalCode.TabIndex = 7;
            // 
            // lable8
            // 
            this.lable8.AutoSize = true;
            this.lable8.Location = new System.Drawing.Point(11, 23);
            this.lable8.Name = "lable8";
            this.lable8.Size = new System.Drawing.Size(41, 12);
            this.lable8.TabIndex = 2;
            this.lable8.Text = "通道号";
            // 
            // txtChannel
            // 
            this.txtChannel.Location = new System.Drawing.Point(59, 19);
            this.txtChannel.Name = "txtChannel";
            this.txtChannel.Size = new System.Drawing.Size(38, 21);
            this.txtChannel.TabIndex = 7;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(10, 52);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(41, 12);
            this.label8.TabIndex = 2;
            this.label8.Text = "使用者";
            // 
            // txtLocalName
            // 
            this.txtLocalName.Location = new System.Drawing.Point(57, 46);
            this.txtLocalName.Name = "txtLocalName";
            this.txtLocalName.Size = new System.Drawing.Size(94, 21);
            this.txtLocalName.TabIndex = 7;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(10, 79);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(29, 12);
            this.label9.TabIndex = 2;
            this.label9.Text = "部门";
            // 
            // txtLocalDepartment
            // 
            this.txtLocalDepartment.Location = new System.Drawing.Point(57, 73);
            this.txtLocalDepartment.Name = "txtLocalDepartment";
            this.txtLocalDepartment.Size = new System.Drawing.Size(94, 21);
            this.txtLocalDepartment.TabIndex = 7;
            // 
            // btSetChannel
            // 
            this.btSetChannel.Location = new System.Drawing.Point(24, 106);
            this.btSetChannel.Name = "btSetChannel";
            this.btSetChannel.Size = new System.Drawing.Size(93, 23);
            this.btSetChannel.TabIndex = 0;
            this.btSetChannel.Text = "设置通道信息";
            this.btSetChannel.UseVisualStyleBackColor = true;
            this.btSetChannel.Click += new System.EventHandler(this.btSetChannel_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(228, 57);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(47, 12);
            this.label10.TabIndex = 2;
            this.label10.Text = "使用者:";
            // 
            // tb_LocalName
            // 
            this.tb_LocalName.Location = new System.Drawing.Point(292, 54);
            this.tb_LocalName.Name = "tb_LocalName";
            this.tb_LocalName.Size = new System.Drawing.Size(125, 21);
            this.tb_LocalName.TabIndex = 8;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btn_Quit;
            this.ClientSize = new System.Drawing.Size(1152, 608);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btn_Download);
            this.Controls.Add(this.groupbox_Login);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btn_Quit);
            this.Controls.Add(this.btn_Login);
            this.Controls.Add(this.groupbox_Query);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupbox_Login.ResumeLayout(false);
            this.groupbox_Login.PerformLayout();
            this.groupbox_Query.ResumeLayout(false);
            this.groupbox_Query.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.bdnInfo)).EndInit();
            this.bdnInfo.ResumeLayout(false);
            this.bdnInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgv_RecData)).EndInit();
            this.tabPage2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCh)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.bdsInfo)).EndInit();
            this.contextMenu_RecData.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox tb_Ip;
        private System.Windows.Forms.Label lbl_Ip;
        private System.Windows.Forms.Button btn_Query;
        private System.Windows.Forms.Button btn_Login;
        private System.Windows.Forms.TextBox tb_rpckey;
        private System.Windows.Forms.Label lbl_UserName;
        private System.Windows.Forms.Label lbl_Pwd;
        private System.Windows.Forms.GroupBox groupbox_Login;
        private System.Windows.Forms.GroupBox groupbox_Query;
        private System.Windows.Forms.Label lbl_DeviceId;
        private System.Windows.Forms.TextBox tb_DeviceId;
        private System.Windows.Forms.TextBox tb_LocalCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_PageLimit;
        private System.Windows.Forms.TextBox tb_PageStart;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lbl_MaxTalkLong;
        private System.Windows.Forms.TextBox tb_MaxTalkLong;
        private System.Windows.Forms.DateTimePicker date_StateDate;
        private System.Windows.Forms.DateTimePicker date_EndDate;
        private System.Windows.Forms.Button btn_Test;
        private System.Windows.Forms.Button btn_Quit;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.BindingSource bdsInfo;
        private System.Windows.Forms.TextBox tb_RemoteCode;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbl_Direct;
        private System.Windows.Forms.ComboBox cob_Direct;
        private System.Windows.Forms.TextBox tb_MinTalkLong;
        private System.Windows.Forms.Label lbl_MinTalkLong;
        private System.Windows.Forms.Button btn_Download;
        private System.Windows.Forms.Button btn_Delete;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenu_DownLoad;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenu_Delete;
        private System.Windows.Forms.ContextMenuStrip contextMenu_RecData;
        private System.Windows.Forms.TextBox tb_Pwd;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.BindingNavigator bdnInfo;
        private System.Windows.Forms.ToolStripLabel tslbl_TotalRecords;
        private System.Windows.Forms.ToolStripLabel tslbl_TotalCount;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton tsbtn_Pre;
        private System.Windows.Forms.ToolStripTextBox tstb_CurrentPage;
        private System.Windows.Forms.ToolStripLabel tslbl_Split;
        private System.Windows.Forms.ToolStripLabel tslbl_PageCount;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton tsbtn_Next;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton tsbtn_Skip;
        private System.Windows.Forms.DataGridView dgv_RecData;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button btQueryChannel;
        private System.Windows.Forms.DataGridView dataGridViewCh;
        private System.Windows.Forms.TextBox txtChannel;
        private System.Windows.Forms.Label lable8;
        private System.Windows.Forms.TextBox txtLocalCode;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtLocalName;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtLocalDepartment;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button btSetChannel;
        private System.Windows.Forms.TextBox tb_LocalName;
        private System.Windows.Forms.Label label10;
    }
}