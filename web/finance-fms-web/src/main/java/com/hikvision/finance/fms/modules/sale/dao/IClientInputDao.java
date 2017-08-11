/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:51:46
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月7日 下午6:51:46
 * @version V1.0 
 */
public interface IClientInputDao extends IBaseDao<Clientinput,SaleQo>{
	
	/**
	 * 获取销售查询模块分页数据
	 * @author jinxindong 2016年4月8日 上午9:34:44
	 * @param limit
	 * @param start
	 * @param saleInfo
	 * @return
	 */
	public Page getSaleDataPage(Integer limit, Integer start, SaleInfo saleInfo);
	
	/**
	 * 获取需要导出的数据
	 * @author jinxindong 2016年4月8日 上午10:44:37
	 * @param saleInfo
	 * @return
	 */
	public List<SaleInfo> getSaleDataList( SaleInfo saleInfo);
	
	/**
	 * 获取业务单信息
	 * @author jinxindong 2016年4月18日 下午2:23:41
	 * @param analysisInfo
	 * @return
	 */
    public List<Clientinput> getSaleByAnalysisInfo(AnalysisInfo analysisInfo);
    
    /**
     * 获取组织节点下的业务单信息
     * @author jinxindong 2016年7月12日 上午10:04:37
     * @param analysisInfo
     * @param regionIds
     * @param status
     * @return
     */
    public Integer getSaleByAnalysis(AnalysisInfo analysisInfo,String regionIds,int status);
    
    /******************************以下为到期管理模块**********************************/
    
	/**
	 * 获取到期信息分页数据
	 * @author jinchenwu 2016年4月7日 下午8:47:55
	 * @param dueRecordInfo
	 * @param start
	 * @param limit
	 * @return
	 */
	public Page getDataPage(DueRecordInfo dueRecordInfo, Integer start, Integer limit);
	
	/**
	 * 获取业务单关联的录像数目
	 * @author jinchenwu 2016年5月30日 下午7:46:34
	 * @param ids
	 * @return
	 */
	public List<DueRecordInfo> getVideoNumList(List<Integer> ids);
	
	/**
	 * 获取到期信息List
	 * @author jinchenwu 2016年4月19日 上午10:52:24
	 * @param dueRecordInfo
	 * @param limit
	 * @return
	 */
	public List<DueRecordInfo> getDataList(DueRecordInfo dueRecordInfo, Integer limit);
	
	/**
	 * 设置录像延迟删除天数
	 * @author jinchenwu 2016年4月12日 下午7:54:32
	 * @param dueRecordInfo
	 */
	public void setDelayDays(DueRecordInfo dueRecordInfo);
	
	/**
	 * 设置录像已读
	 * @author jinchenwu 2016年4月12日 下午8:32:26
	 * @param dueRecordInfo
	 */
	public void setReadStatus(DueRecordInfo dueRecordInfo);
	
	public List<SaleInfo> getHisClientInputList(SaleInfo saleInfo);

}
