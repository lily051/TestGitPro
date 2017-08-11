/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午2:17:27
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.service;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.modules.record.dto.RecordDto;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.record.qo.ProSaveRecordQo;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;

/**
 * <p>理财录像模块</p>
 * @author jinchenwu 2016年4月6日 下午2:17:27
 * @version V1.0
 */
public interface IRecordService extends IBaseService<Prosaverecord, ProSaveRecordQo> {
	
	/**
	 * 获取理财录像分页数据
	 * @author jinchenwu 2016年4月6日 下午4:16:56
	 * @param recordInfo
	 * @param start
	 * @param limit
	 * @return
	 */
	public Page getDataPage(RecordInfo recordInfo, Integer start, Integer limit);
	
	/**
	 * 获取单条理财录像信息，以及回放控件需要的设备信息，用于录像回放
	 * @author jinchenwu 2016年4月12日 下午12:21:38
	 * @param recordInfo
	 * @return
	 */
	public RecordDto saveLogAndGetData(RecordInfo recordInfo);
	
	/**
	 * 随机查询一条录像数据，用于随机回放
	 * @author jinchenwu 2016年4月7日 下午2:10:08
	 * @param recordInfo
	 * @return
	 */
	public RecordInfo getRandomData(RecordInfo recordInfo);
	
	/**
	 * 获取业务单信息
	 * @author jinchenwu 2016年5月10日 上午10:36:33
	 * @param id
	 * @return
	 */
	public SaleInfo getOrderInfo(SaleInfo saleInfo);
	
	/**
	 * 设置录像合规检查状态
	 * @author jinchenwu 2016年4月7日 下午5:34:22
	 * @param recordInfo
	 */
	public void saveRecordCheck(RecordInfo recordInfo);
	
	/**
	 * 设置录像延迟删除天数
	 * @author jinchenwu 2016年4月11日 下午4:37:23
	 * @param recordInfo
	 */
	public void saveDelayDays(RecordInfo recordInfo);
	
	/**
	 * 导出Excel报表
	 * @author jinchenwu 2016年4月12日 上午9:25:29
	 * @param recordInfo
	 * @param path
	 * @return
	 */
    public boolean saveLogAndexportExcel(RecordInfo recordInfo, String path);
}
