using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using org.phprpc;
using System.Text.RegularExpressions;
using System.Collections;
using org.phprpc.util;
using System.Net;
using System.IO;
using System.Threading;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization;

namespace evrs_php
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        PHPRPC_Client rpc_client=null;
        int nCurrnetPage = 1;
        const string RPC_CALL_KEY = "VocLogRpcKey";
        #region 命令值
        const string CMD_LOGIN_URL = "/friextra/rpc_call.php";
        #endregion

        #region 调用函数返回值
        const string LOGIN_PWD_ERROR = "NO";//登录时，密码错误的返回值

        const string DEL_OK = "OK";//删除录音文件成功

        const string DIRECT_CALL_IN = "0";//呼入
        const string DIRECT_CALL_OUT = "1";//呼出
        #endregion
#region  通话记录查询返回

        public class TalkLogList
        {
            [DataMember(Order = 0)]
            public string talktime { get; set; }
            [DataMember(Order = 1)]
            public string talklogid { get; set; }
            [DataMember(Order = 2)]
            public string channel { get; set; }
            [DataMember(Order = 3)]
            public string localcode { get; set; }
            [DataMember(Order = 4)]
            public string localname { get; set; }
            [DataMember(Order = 5)]
            public string remotecode { get; set; }
            [DataMember(Order = 6)]
            public string calldir { get; set; }
            [DataMember(Order = 6)]
            public string talklong { get; set; }
            [DataMember(Order = 6)]
            public string url { get; set; }
        }
        public class QueryTalkLogRreturn
        {
            [DataMember(Order = 0, IsRequired = true)]
            public string result { get; set; }
            [DataMember(Order = 1)]
            public string totalcount { get; set; }
            [DataMember(Order = 2)]
            public List<TalkLogList> datas;
        }
#endregion

#region  通道信息返回
        public class ChannelList
        {
            [DataMember(Order = 0)]
            public string channel { get; set; }
            [DataMember(Order = 1)]
            public string localcode { get; set; }
            [DataMember(Order = 2)]
            public string localname { get; set; }
            [DataMember(Order = 3)]
            public string localdepartment { get; set; }
        }
        public class QueryChannelRreturn
        {
            [DataMember(Order = 0, IsRequired = true)]
            public string result { get; set; }
            [DataMember(Order = 1)]
            public List<ChannelList> datas;
        }
#endregion


        #region 弹框
        const int MSG_BOX_OK = 0;
        const int MSG_BOX_OKCANCEL = 1;
        const int MSG_BOX_YESNO = 2;
        const int MSG_BOX_ERROR = 3;
        #endregion

        #region 列
        const string COL_NAME_TALKLOGID = "talklogId";
        const string COL_NAME_CHANEL = "chanel";
        const string COL_NAME_RECFILE = "recfile";
        const string COL_NAME_DIRECTION = "direction";
        const string COL_NAME_LOCALCODE = "localcode";
        const string COL_NAME_REMOTECODE = "remotecode";
        const string COL_NAME_TALKLONG = "talklong";
        const string COL_NAME_TALKTIME = "talktime";
        const string COL_NAME_USERNAME = "username";

        const string COL_TEXT_TALKLOGID = "记录ID";
        const string COL_TEXT_CHANEL = "通道号";
        const string COL_TEXT_RECFILE = "路径";
        const string COL_TEXT_DIRECTION = "呼入/呼出";
        const string COL_TEXT_LOCALCODE = "本地号码";
        const string COL_TEXT_REMOTECODE = "对方号码";
        const string COL_TEXT_TALKLONG = "通话时长(秒)";
        const string COL_TEXT_TALKTIME = "开始时间";
        const string COL_TEXT_USERNAME = "用户名";
        #endregion

        public static T JSONparse<T>(string jsonString)
        {
            using (var ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString)))
            {
                return (T)new DataContractJsonSerializer(typeof(T)).ReadObject(ms);
            }
        }

      
      
        private void MainForm_Load(object sender, EventArgs e)
        {
            btn_Login.Focus();
            date_StateDate.Text = DateTime.Now.AddDays(-7).ToString();
            IniDgv();
         
        }

        

        private void IniDgv()
        {
            dgv_RecData.Columns.Add(COL_NAME_TALKLOGID, COL_TEXT_TALKLOGID);
            dgv_RecData.Columns.Add(COL_NAME_TALKTIME, COL_TEXT_TALKTIME);
            dgv_RecData.Columns.Add(COL_NAME_CHANEL, COL_TEXT_CHANEL);
            dgv_RecData.Columns.Add(COL_NAME_USERNAME, COL_TEXT_USERNAME);
            dgv_RecData.Columns.Add(COL_NAME_LOCALCODE, COL_TEXT_LOCALCODE);
            dgv_RecData.Columns.Add(COL_NAME_REMOTECODE, COL_TEXT_REMOTECODE);
            dgv_RecData.Columns.Add(COL_NAME_DIRECTION, COL_TEXT_DIRECTION);
            dgv_RecData.Columns.Add(COL_NAME_TALKLONG, COL_TEXT_TALKLONG);
            dgv_RecData.Columns.Add(COL_NAME_RECFILE, COL_TEXT_RECFILE);
            dgv_RecData.Columns[COL_NAME_TALKLOGID].Width = 100;
         
            dgv_RecData.Columns[COL_NAME_LOCALCODE].Width = 80;
            dgv_RecData.Columns[COL_NAME_REMOTECODE].Width = 80;
            dgv_RecData.Columns[COL_NAME_TALKTIME].Width = 120;
            dgv_RecData.Columns[COL_NAME_DIRECTION].Width = 85;
            dgv_RecData.Columns[COL_NAME_CHANEL].Width = 70;
            dgv_RecData.Columns[COL_NAME_RECFILE].Width = 300;
            dgv_RecData.Columns[COL_NAME_TALKLOGID].DataPropertyName = COL_NAME_TALKLOGID;
            dgv_RecData.Columns[COL_NAME_LOCALCODE].DataPropertyName = COL_NAME_LOCALCODE;
            dgv_RecData.Columns[COL_NAME_REMOTECODE].DataPropertyName = COL_NAME_REMOTECODE;
            dgv_RecData.Columns[COL_NAME_TALKTIME].DataPropertyName = COL_NAME_TALKTIME;
            dgv_RecData.Columns[COL_NAME_TALKLONG].DataPropertyName = COL_NAME_TALKLONG;
            dgv_RecData.Columns[COL_NAME_DIRECTION].DataPropertyName = COL_NAME_DIRECTION;
            dgv_RecData.Columns[COL_NAME_CHANEL].DataPropertyName = COL_NAME_CHANEL;
            dgv_RecData.Columns[COL_NAME_RECFILE].DataPropertyName = COL_NAME_RECFILE;
            dgv_RecData.Columns[COL_NAME_USERNAME].DataPropertyName = COL_NAME_USERNAME;


            dataGridViewCh.Columns.Add("channel", "通道号");
            dataGridViewCh.Columns.Add("localcode", "本地号码");
            dataGridViewCh.Columns.Add("localname", "使用者");
            dataGridViewCh.Columns.Add("localdepartment", "使用部门");
            dataGridViewCh.Columns["channel"].Width = 100;

            dataGridViewCh.Columns["localcode"].Width = 80;
            dataGridViewCh.Columns["localname"].Width = 80;
            dataGridViewCh.Columns["localdepartment"].Width = 120;

            dataGridViewCh.Columns["channel"].DataPropertyName = "channel";
            dataGridViewCh.Columns["localname"].DataPropertyName = "localname";
            dataGridViewCh.Columns["localcode"].DataPropertyName = "localcode";
            dataGridViewCh.Columns["localdepartment"].DataPropertyName = "localdepartment";


            
        }

        #region 登录
        //登录时条件判断
        private bool CreateRpcClient()
        {
            string szIp = "";//设备ip
            szIp = tb_Ip.Text.Trim();//设备ip
            if (rpc_client != null)
                return true;
            try
            {
                rpc_client = new PHPRPC_Client("http://" + szIp + CMD_LOGIN_URL);
                if (rpc_client == null)
                {
                    ShowMessageBox("登录设备" + szIp + "时的PHPRPC_Client对象为空！", "提示", MSG_BOX_OK);
                    return false;
                }
            }
            catch (Exception ex)
            {
                ShowMessageBox("Erorr:" + ex.Message, "提示", MSG_BOX_OK);
            }
            finally
            {
                
            }

            return true;
        }

        //登录
        private void btn_Login_Click(object sender, EventArgs e)
        {
            
        }
        #endregion



        private DialogResult ShowMessageBox(string szMsg, string szCaption, int nType)
        {
            MessageBoxButtons button = MessageBoxButtons.OK;
            MessageBoxIcon icon = MessageBoxIcon.Information;
            switch (nType)
            { 
                case MSG_BOX_OK:
                    button = MessageBoxButtons.OK;
                    icon = MessageBoxIcon.Information;
                    break;
                case MSG_BOX_OKCANCEL:
                    button = MessageBoxButtons.OKCancel;
                    icon = MessageBoxIcon.Information;
                    break;
                case MSG_BOX_YESNO:
                    button = MessageBoxButtons.YesNo;
                    icon = MessageBoxIcon.Information;
                    break;
                case MSG_BOX_ERROR:
                    
                    break;
            }
           return MessageBox.Show(szMsg, szCaption, button, icon);
        }

        #region 查询
        private void SetPageCounts()
        {
            int nPageSize = 0;
            int nPageCount = 0;
            int nTotalCount = 0;
            nPageSize = PHPConvert.ToInt32(tb_PageLimit.Text.Trim());
            nTotalCount = PHPConvert.ToInt32(tslbl_TotalCount.Text.Trim());
            nPageCount = nTotalCount / nPageSize;
            int modVal = nTotalCount % nPageSize;
            if (modVal > 0)
            {
                nPageCount++;
            }
            tslbl_PageCount.Text = nPageCount.ToString();
            tstb_CurrentPage.Text = nCurrnetPage.ToString();
        }

        
        private void btn_Query_Click(object sender, EventArgs e)
        {
            string szDeviceId = "";//设备id
            string szStateDate = "";//开始时间
            string szEndDate = "";//结束时间
            string szLocalCode = "";//本地号码
            string szRemoteCode = "";//对方号码
            string szMinTalkLong = "";//最短通话时长
            string szMaxTalkLong = "";//最长通话时长
            string szCallDirect = "";//呼叫方向
            string szLocalName = "";
            int nPageStart = 0;
            string szPageStart = "";// 起始页
            string szPageLimit = "";//每页显示数目
            int nTalkLogCount = 0;
            string szGetDirectVal = "";//呼叫方向
            List<TalkLogList> Talklog;

            szDeviceId = tb_DeviceId.Text.Trim();//设备id
            szStateDate = date_StateDate.Text.Trim();//开始时间
            szEndDate = date_EndDate.Text.Trim();//结束时间
            szLocalCode = tb_LocalCode.Text.Trim();//本地号码
            szLocalName = tb_LocalName.Text.Trim();
            szRemoteCode = tb_RemoteCode.Text.Trim();//对方号码
            szMinTalkLong = tb_MinTalkLong.Text.Trim();//最短通话时长
            szMaxTalkLong = tb_MaxTalkLong.Text.Trim();//最长通话时长
            szCallDirect = cob_Direct.SelectedIndex.ToString();//呼叫方向
            CreateRpcClient();

            if (szCallDirect == "-1")
            {
                szCallDirect = "";
            }
            nPageStart = PHPConvert.ToInt32(tb_PageStart.Text.Trim());
            if (nPageStart >= 1)
            {
                nCurrnetPage = nPageStart;
                szPageStart = (nPageStart - 1).ToString();// 起始页
            }
            szPageLimit = tb_PageLimit.Text.Trim();//每页显示数目

            tabControl1.SelectedIndex = 0;
            dgv_RecData.Rows.Clear(); 
            try
            {

                Object obj=rpc_client.Invoke("QueryTalklog",
                    new Object[] { tb_rpckey.Text, 
                        szStateDate,
                        szEndDate, 
                        szLocalCode,
                        szLocalName,
                        szRemoteCode,
                        szCallDirect,
                        szMaxTalkLong,
                        szMinTalkLong,szPageStart, szPageLimit });
                List<QueryTalkLogRreturn> ArRet = JSONparse<List<QueryTalkLogRreturn>>("["+PHPConvert.ToString(obj)+"]");
                if (ArRet[0].result != "ok")
                {
                    ShowMessageBox("失败，原因：" + ArRet[0].result, "提示", MSG_BOX_OK);
                }
                tslbl_TotalCount.Text = ArRet[0].totalcount;
                if (Convert.ToInt32(ArRet[0].totalcount) == 0)
                {
                    ShowMessageBox("查询到的录音记录为0！", "提示", MSG_BOX_OK);
                    return;
                }
                Talklog = ArRet[0].datas;
      
                int allCount =Convert.ToInt32(ArRet[0].totalcount);
                int pagesize = Convert.ToInt32(szPageLimit);
                int pagecount = allCount  % pagesize;   //页数 = 总数/显示的行数
  
                //只能显示一页,判断是否是整除
                if (pagecount == 0)
                {
                    pagecount =allCount  / pagesize;
                }
                else
                {
                    pagecount = allCount / pagesize + 1;
                }

                tslbl_PageCount.Text = pagecount.ToString();
                if (tstb_CurrentPage.Text == "0")
                    tstb_CurrentPage.Text = tb_PageStart.Text;

                for(int i=0;i<Talklog.Count;i++)
                {
                    int index = dgv_RecData.Rows.Add();
                    dgv_RecData.Rows[index].Cells[COL_NAME_RECFILE].Value = Talklog[i].url;//录音文件web路径
                    dgv_RecData.Rows[index].Cells[COL_NAME_CHANEL].Value = Talklog[i].channel;//录音通道在设备中的编号
                    szGetDirectVal = Talklog[i].calldir;//通话方向，0表示呼入电话，1表示呼出电话
                    dgv_RecData.Rows[index].Cells[COL_NAME_DIRECTION].Value = "未知";
                    if (szGetDirectVal.Equals(DIRECT_CALL_IN))
                    {
                        dgv_RecData.Rows[index].Cells[COL_NAME_DIRECTION].Value = "呼入";
                    }
                    else if (szGetDirectVal.Equals(DIRECT_CALL_OUT))
                    {
                        dgv_RecData.Rows[index].Cells[COL_NAME_DIRECTION].Value = "呼出";
                    }
                    dgv_RecData.Rows[index].Cells[COL_NAME_TALKLOGID].Value = Talklog[i].talklogid;//通话id
                    dgv_RecData.Rows[index].Cells[COL_NAME_LOCALCODE].Value = Talklog[i].localcode;//被录音话机的号码
                    dgv_RecData.Rows[index].Cells[COL_NAME_REMOTECODE].Value = Talklog[i].remotecode;//呼入呼出时对方号码
                    dgv_RecData.Rows[index].Cells[COL_NAME_TALKLONG].Value = Talklog[i].talklong;//通话时长，单位为秒
                    dgv_RecData.Rows[index].Cells[COL_NAME_TALKTIME].Value = Talklog[i].talktime;//通话开始时间
                    dgv_RecData.Rows[index].Cells[COL_NAME_USERNAME].Value = Talklog[i].localname;//
                    //dgv_RecData.Rows[index].ContextMenuStrip = contextMenu_RecData;
                }
                bdsInfo.DataSource = dgv_RecData.DataSource;
                bdnInfo.BindingSource = bdsInfo;
            }
            catch (Exception ex)
            {
                ShowMessageBox("查询失败，原因：" + ex.Message, "提示", MSG_BOX_OK);
            }
            finally
            {
               
            }
        }
        #endregion

        #region 下载
        private void toolStripMenu_DownLoad_Click(object sender, EventArgs e)
        {
           
        }
        #endregion

        #region 删除
        private void toolStripMenu_Delete_Click(object sender, EventArgs e)
        {
            
        }
        #endregion

        #region 测试
        private void btn_Test_Click(object sender, EventArgs e)
        {
            if (!CreateRpcClient()) //登录时条件判断
            {
                return;
            }
            try
            {
                object obj;
                string szRetVal = "";
                obj = rpc_client.Invoke("Test", new object[] { tb_rpckey.Text});
                if (obj == null)
                {
                    ShowMessageBox("返回的值为空！", "提示", MSG_BOX_OK);
                    return;
                }
                szRetVal = PHPConvert.ToString(obj);
                MessageBox.Show(szRetVal);

            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        //datatable添加列
        private DataTable FillDTColumn()
        {
            DataTable table = new DataTable();
            table.Columns.Add(COL_NAME_TALKLOGID, typeof(string));
            table.Columns.Add(COL_NAME_TALKTIME, typeof(string));
            table.Columns.Add(COL_NAME_LOCALCODE, typeof(string));
            table.Columns.Add(COL_NAME_USERNAME, typeof(string));
            table.Columns.Add(COL_NAME_REMOTECODE, typeof(string));
            table.Columns.Add(COL_NAME_TALKLONG, typeof(string));
            table.Columns.Add(COL_NAME_DIRECTION, typeof(string));
            table.Columns.Add(COL_NAME_CHANEL, typeof(string));
            table.Columns.Add(COL_NAME_RECFILE, typeof(string));
            return table;
        }

        private DataTable FillData()
        {
            DataTable table = FillDTColumn();//datatable添加列

            string szDerict = "0";
            DataRow row = table.NewRow();
            row[COL_NAME_TALKLOGID] = "1";
            row[COL_NAME_LOCALCODE] = "123";
            row[COL_NAME_REMOTECODE] = "456";
            row[COL_NAME_TALKTIME] = DateTime.Now.ToString();
            row[COL_NAME_TALKLONG] = "120";
            row[COL_NAME_DIRECTION] = "未知";
            if (szDerict.Equals(DIRECT_CALL_IN))
            {
                row[COL_NAME_DIRECTION] = "呼入";
            }
            else if (szDerict.Equals(DIRECT_CALL_IN))
            {
                row[COL_NAME_DIRECTION] = "呼出";
            }
            row[COL_NAME_CHANEL] = "1";
            row[COL_NAME_RECFILE] = @"F:\liuh\winformDemo\eVRS服务器接口文档 V5.4.0_20140921.txt";
            row[COL_NAME_USERNAME] = "1";
            table.Rows.Add(row);

            row = table.NewRow();
            row[COL_NAME_TALKLOGID] = "2";
            row[COL_NAME_LOCALCODE] = "876";
            row[COL_NAME_REMOTECODE] = "340";
            row[COL_NAME_TALKTIME] = DateTime.Now.ToString();
            row[COL_NAME_TALKLONG] = "653";
            szDerict = "1";
            row[COL_NAME_DIRECTION] = "未知";
            if (szDerict.Equals(DIRECT_CALL_IN))
            {
                row[COL_NAME_DIRECTION] = "呼入";
            }
            else if (szDerict.Equals(DIRECT_CALL_IN))
            {
                row[COL_NAME_DIRECTION] = "呼出";
            }
            row[COL_NAME_CHANEL] = "3";
            row[COL_NAME_RECFILE] = @"F:\liuh\winformDemo\eVRS服务器接口文档 V5.4.0_20140921.doc";
            row[COL_NAME_USERNAME] = "1";
            table.Rows.Add(row);
            return table;
        }
        #endregion

        #region 退出
        private void btn_Quit_Click(object sender, EventArgs e)
        {
            DialogResult result = ShowMessageBox("您确定要退出？", "提示", MSG_BOX_OKCANCEL);
            if (result == DialogResult.Cancel)
            {
                return;
            }
            this.Close();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = false;
        }
        #endregion

        #region 删除
        private void btn_Delete_Click(object sender, EventArgs e)
        {
            toolStripMenu_Delete_Click(null, null);
        }
        #endregion

        #region 下载
        private void btn_Download_Click(object sender, EventArgs e)
        {
            toolStripMenu_DownLoad_Click(null, null);
        }
        #endregion

        private void bdnInfo_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            int nPageCount = PHPConvert.ToInt32(tslbl_PageCount.Text.Trim());
            if (e.ClickedItem.Text == "上一页")
            {
                if (nCurrnetPage <= 1)
                {
                    ShowMessageBox("已经是第一页，请点击“下一页”查看！", "提示", MSG_BOX_OK);
                    return;
                }
                nCurrnetPage--;
            }
            else if (e.ClickedItem.Text == "下一页")
            {
                if (nCurrnetPage >= nPageCount)
                {
                    ShowMessageBox("已经是最后一页，请点击“上一页”查看！", "提示", MSG_BOX_OK);
                    return;
                }
                nCurrnetPage++;
            }
            else if (e.ClickedItem.Text == "跳转")
            {
                nCurrnetPage = PHPConvert.ToInt32(tstb_CurrentPage.Text.Trim());
                if (nCurrnetPage <= 1)
                {
                    ShowMessageBox("已经是第一页，请点击“下一页”查看！", "提示", MSG_BOX_OK);
                    return;
                }
                if (nCurrnetPage >= nPageCount)
                {
                    ShowMessageBox("已经是最后一页，请点击“上一页”查看！", "提示", MSG_BOX_OK);
                    return;
                }
            }
            tstb_CurrentPage.Text = nCurrnetPage.ToString();
            tb_PageStart.Text = nCurrnetPage.ToString();
            btn_Query_Click(null, null);
        }

        private void dgv_RecData_RowPostPaint(object sender, DataGridViewRowPostPaintEventArgs e)
        {
            using (SolidBrush b = new SolidBrush(Color.Gray))
            {
                e.Graphics.DrawString(Convert.ToString(e.RowIndex + 1),
                e.InheritedRowStyle.Font, b, e.RowBounds.Location.X + 10, e.RowBounds.Location.Y + 4);
                b.Dispose();
            }
        }

        private void btQueryChannel_Click(object sender, EventArgs e)
        {
            string szLocalCode = "";
            string szlocalname = "";
            string szlocadepartment = "";
            string szChannel = "";
         
            List<ChannelList> ChList;

                  
            CreateRpcClient();
            tabControl1.SelectedIndex = 1;
            szLocalCode = txtLocalCode.Text.Trim();
            szChannel = txtChannel.Text.Trim();
            szlocadepartment = txtLocalDepartment.Text.Trim();
            szlocalname = txtLocalName.Text.Trim();

            dataGridViewCh.Rows.Clear();

            try
            {

                Object obj = rpc_client.Invoke("QueryChannel",
                    new Object[] { tb_rpckey.Text, 
                        szChannel,
                        szLocalCode, 
                        szlocalname,
                        szlocadepartment });
                List<QueryChannelRreturn> ArRet = JSONparse<List<QueryChannelRreturn>>("[" + PHPConvert.ToString(obj) + "]");
                if (ArRet[0].result != "ok")
                {
                    ShowMessageBox("失败，原因：" + ArRet[0].result, "提示", MSG_BOX_OK);
                }

                ChList = ArRet[0].datas;

                for (int i = 0; i < ChList.Count; i++)
                {
                    int index = dataGridViewCh.Rows.Add();
                    dataGridViewCh.Rows[index].Cells["channel"].Value = ChList[i].channel;
                    dataGridViewCh.Rows[index].Cells["localcode"].Value = ChList[i].localcode;
                    dataGridViewCh.Rows[index].Cells["localname"].Value = ChList[i].localname;
                    dataGridViewCh.Rows[index].Cells["localdepartment"].Value = ChList[i].localdepartment;
                }
                bdsInfo.DataSource = dgv_RecData.DataSource;
                bdnInfo.BindingSource = bdsInfo;
            }
            catch (Exception ex)
            {
                ShowMessageBox("查询失败，原因：" + ex.Message, "提示", MSG_BOX_OK);
            }
            finally
            {
                
            }

        }

        private void btSetChannel_Click(object sender, EventArgs e)
        {
            string szLocalCode = "";
            string szlocalname = "";
            string szlocadepartment = "";
            string szChannel = ""; 
            CreateRpcClient();
            tabControl1.SelectedIndex = 1;
            szLocalCode = txtLocalCode.Text.Trim();
            szChannel = txtChannel.Text.Trim();
            szlocadepartment = txtLocalDepartment.Text.Trim();
            szlocalname = txtLocalName.Text.Trim();
            if (szLocalCode == "" || szChannel == "")
            {
                ShowMessageBox("通道和本地号码必须填写！", "提示", MSG_BOX_OK);
                return;
            }
            try
            {

                Object obj = rpc_client.Invoke("SetChannel",
                    new Object[] { tb_rpckey.Text, 
                        szChannel,
                        szLocalCode, 
                        szlocalname,
                        szlocadepartment });
                List<QueryChannelRreturn> ArRet = JSONparse<List<QueryChannelRreturn>>("[" + PHPConvert.ToString(obj) + "]");
                if (ArRet[0].result != "ok")
                {
                    ShowMessageBox("失败，原因：" + ArRet[0].result, "提示", MSG_BOX_OK);
                }

            }
            catch (Exception ex)
            {
                ShowMessageBox("失败，原因：" + ex.Message, "提示", MSG_BOX_OK);
            }
            finally
            {
                
            }
        }
    }
}
