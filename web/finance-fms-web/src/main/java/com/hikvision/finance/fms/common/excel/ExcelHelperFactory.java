/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-1-22 下午4:28:35
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.excel;


import org.apache.commons.lang3.StringUtils;

import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * 产生一个ExcelHelper工具类
 * @author jinxindong 2016年4月21日 下午1:09:51
 * @version V1.0
 */
public class ExcelHelperFactory {
	
	/**
	 * 根据后缀名类型产生一个ExcelHelper类
	 * @author jinxindong 2016年4月21日 下午1:10:10
	 * @param fileName
	 * @return
	 */
	public static <T> ExcelHelper<T> createExcelHelper(String fileName){
		ExcelHelper<T> excelHelper = null;
	    if(StringUtils.isNotBlank(fileName)){
    		String type = fileName.substring(fileName.lastIndexOf(".") + 1);
    		if("xls".equals(type)){
    			 excelHelper = new ExcelHelper<T>(new HSSFExcel());
    		} else if("xlsx".equals(type)){
    			 excelHelper = new ExcelHelper<T>(new XSSFExcel());
    		} else{
    			throw new ExpectedException("","不支持Excel文件的扩展名【" + type +"】");
    		}
    	}
		return excelHelper;
	}
}
