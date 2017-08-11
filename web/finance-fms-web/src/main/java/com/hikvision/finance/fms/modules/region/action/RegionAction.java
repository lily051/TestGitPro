/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:40:58
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.action;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.struts2.ServletActionContext;

import com.google.common.collect.Lists;
import com.hikvision.finance.fms.common.excel.model.ExcelColumn;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.region.service.IRegionService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>组织、区域管理</p>
 * @author jinxindong 2016年2月25日 上午9:40:58
 * @version V1.0
 */
public class RegionAction extends BaseAction<Region> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -4844845931387959504L;
	private IRegionService regionService;
	private RegionInfo regionInfo;
	private List<Integer> ids;
	
	private File regFile;// 导入机构的文件
	private String regFileFileName;// 导入机构的文件名
	private Map<String,Object> resMap; 
	
	
    public Map<String, Object> getResMap() {
    	return resMap;
    }

	
    public void setResMap(Map<String, Object> resMap) {
    	this.resMap = resMap;
    }

	/**
	 * 根据父级ID获取一级组织数据 也就是父id为1
	 * @author jinxindong 2016年2月25日 下午2:22:22
	 * @return
	 */
	public String getDataPage() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			page = regionService.getDataPage(limit, start, regionInfo);
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
		return PAGE;
	}
	
	/**
	 * 弹出新增组织页面
	 * @author jinxindong 2016年2月25日 下午2:23:51
	 * @return
	 */
	public String toAddRegPage() {
		if (regionInfo == null || regionInfo.getNparentId() == null) {
			throw new ExpectedException("", "未获取区域父ID");
		}
		Integer nparentId = regionInfo.getNparentId();
		regionInfo.setStrInterAreaCode(regionService.getRegSeril(nparentId));
		Region region = regionService.get(nparentId);
		
		regionInfo.setNparentId(nparentId);
		regionInfo.setNparentName(region.getStrName());
		
		operPage = "/modules/region/dialogs/org-win.jsp";
		return DISPATCHER;
		
	}
	
	/**
	 * 添加组织
	 * @author jinxindong 2016年2月25日 下午5:47:30
	 * @return
	 */
	public String addReg() {
		if (regionInfo == null || regionInfo.getNparentId() == null) {
			throw new ExpectedException("", "未获取区域父ID");
		}
		ajaxData = regionService.addReg(regionInfo);
		return AJAX;
	}
	
	/**
	 * 删除组织（根据id或者ids）
	 * @author jinxindong 2016年2月25日 下午5:49:06
	 * @return
	 */
	public String deleteReg() {
		if (ids == null) {
			throw new ExpectedException("", "未获取区域ID");
		}
		ajaxData = regionService.deleteReg(ids);
		return AJAX;
	}
	
	/**
	 * 跳转到编辑页面
	 * @author jinxindong 2016年2月25日 下午6:04:12
	 * @return
	 */
	public String toEditRegPage() {
		if (regionInfo == null || regionInfo.getId() == null) {
			throw new ExpectedException("", "未获取区域ID");
		}
		// 根据id查询组织信息
		entity = regionService.get(regionInfo.getId());
		regionInfo.setId(entity.getId());
		regionInfo.setStrCode(entity.getStrCode());
		regionInfo.setStrName(entity.getStrName());
		regionInfo.setStrTxt(entity.getStrTxt());
		
		// 将regionInfo传到前台
		operPage = "/modules/region/dialogs/org-win.jsp";
		return DISPATCHER;
		
	}
	
	/**
	 * 编辑组织（根据id）
	 * @author jinxindong 2016年2月25日 下午5:50:43
	 * @return
	 */
	public String editReg() {
		if (regionInfo == null || regionInfo.getId() == null) {
			throw new ExpectedException("", "未获取区域ID");
		}
		ajaxData = regionService.editReg(regionInfo);
		return AJAX;
	}
	
	/**
	 * 跳转到下载模板界面
	 * @author jinxindong 2016年2月25日 下午6:05:08
	 * @return
	 */
	public String toCheckOutPage() {
		operPage = "/modules/region/dialogs/reg-import.jsp";
		return DISPATCHER;
	}
	
	
	/**
	 * 上传Excel 
	 * @author jinxindong 2016年2月25日 下午5:57:41
	 * 下载模板：/modules/region/doc/RegionImportTemplate.xls
	 * @return
	 */
	public String regionUploadExcel() {
		resMap = new HashMap<String, Object>(); 
		if (this.regFile != null) {
			// 1、excel的每行显示名称-数据库名称
			List<ExcelColumn> excelColumns = Lists.newArrayList();
			excelColumns.add(new ExcelColumn(0, "strName", "组织名称"));
			excelColumns.add(new ExcelColumn(1, "strCode", "组织代码"));
			excelColumns.add(new ExcelColumn(2, "strTxt", "备注"));
			excelColumns.add(new ExcelColumn(3, "nparentName", "所属组织"));
			
			// 4、组装excel信息
			ExcelHead excelHead = new ExcelHead();
			excelHead.setRowCount(1);// 模板头部所占行数
			excelHead.setColumnCount(4);
			excelHead.setColumns(excelColumns);// 列的定义
	/*		ajaxData = regionService.addRegFromExcel(regFile, regFileFileName, excelHead);
			resMap.put("alType", ajaxData.get("alType"));*/
			ajaxData = regionService.addAndValidateExcel(regFile, regFileFileName, excelHead);
			resMap.put("errorNo", ajaxData.getSuccess());
			resMap.put("errorInfo", ajaxData.getMsg());
			if(ajaxData.getData().get("failRegionName")!=null){
				resMap.put("failRegionName", ajaxData.getData().get("failRegionName"));	
			}
		}
		return "upload";
	}
	
	
	/**
	 * 导出Excel
	 * @author jinxindong 2016年4月19日 下午4:45:21
	 * @return
	 */
	public String exportRegionExcel() {
		String[] titleNames = new String[] {"组织名称", "组织代码",  "备注", "所属组织"};
		String[] attrNames = new String[] {"strName", "strCode",  "strTxt", "nparentName"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "regionQuery.xls";
		if(regionService.exportRegionExcel(regionInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	// ==========================getter/setter================================================
	
	public IRegionService getRegionService() {
		return regionService;
	}
	
	public void setRegionService(IRegionService regionService) {
		this.regionService = regionService;
	}
	
	public RegionInfo getRegionInfo() {
		return regionInfo;
	}
	
	public void setRegionInfo(RegionInfo regionInfo) {
		this.regionInfo = regionInfo;
	}
	
	public List<Integer> getIds() {
		return ids;
	}
	
	public void setIds(List<Integer> ids) {
		this.ids = ids;
	}

	
    public File getRegFile() {
    	return regFile;
    }

	
    public void setRegFile(File regFile) {
    	this.regFile = regFile;
    }

	
    public String getRegFileFileName() {
    	return regFileFileName;
    }

	
    public void setRegFileFileName(String regFileFileName) {
    	this.regFileFileName = regFileFileName;
    }
	
}
