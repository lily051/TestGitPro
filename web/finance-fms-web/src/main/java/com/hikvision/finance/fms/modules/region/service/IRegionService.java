/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:46:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.service;

import java.io.File;
import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.region.qo.RegionQo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p></p>	
 * @author jinxindong 2016年2月25日 上午9:46:48
 * @version V1.0 
 */
public interface IRegionService extends IBaseService<Region,RegionQo> {
	
	/**
	 * 增加组织
	 * @author jinxindong 2016年2月26日 上午9:50:13
	 * @param regionInfo
	 * @return
	 */
	public AjaxData addReg(RegionInfo regionInfo);
	
	/**
	 * 根据父id获取组织的编码
	 * @author jinxindong 2016年2月26日 上午9:23:27
	 * @param nparentId
	 * @return
	 */
	public String getRegSeril(Integer nparentId);
	
	/**
	 * 删除或者批量删除组织
	 * @author jinxindong 2016年2月26日 上午11:20:29
	 * @param ids
	 * @return
	 */
	public AjaxData deleteReg(List<Integer> ids);
	
	/**
	 * 修改组织
	 * @author jinxindong 2016年2月26日 下午3:25:39
	 * @param regionInfo
	 * @return
	 */
	public AjaxData editReg(RegionInfo regionInfo);
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月18日 下午3:23:20
	 * @param limit
	 * @param start
	 * @param regionInfo
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,RegionInfo regionInfo);
	
	/**
	 * 获取匹配strpath的节点id
	 * @author jinchenwu 2016年4月13日 下午2:16:04
	 * @param regionId
	 * @param isFlag
	 * @return
	 */
    public String getLikeStrPathIds(Integer regionId, Integer isFlag);
    
    /**
     * 导出组织表格
     * @author jinxindong 2016年4月19日 下午4:46:25
     * @param regionInfo
     * @param titleName
     * @param attrName
     * @param path
     * @return
     */
	public boolean exportRegionExcel(RegionInfo regionInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 解析添加组织信息的excel文件
	 * @author jinxindong 2016年4月21日 下午1:28:31
	 * @param orgFile
	 * @param orgFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addRegFromExcel(File orgFile, String orgFileFileName, ExcelHead excelHead);
	
	/**
	 * 校验表格数据
	 * @author jinxindong 2016年6月1日 下午6:49:09
	 * @param orgFile
	 * @param orgFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addAndValidateExcel(File orgFile, String orgFileFileName, ExcelHead excelHead);
    
}
