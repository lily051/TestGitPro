package com.hikvision.finance.fms.common.excel;

import java.io.InputStream;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.DataValidationHelper;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.hikvision.finance.fms.common.excel.model.ExcelHead;
/**
 * 对Excel的操作：如读取、创建、修改进行抽象
 * 使用POI开源框架实现
 * @author jinxindong 2016年4月21日 下午1:07:13
 * @version V1.0
 */
public interface IPoiExcelOperation extends IExcelOperation{
	
	public Workbook createExcel(String excelName,List<Map<Object, Object>> dataList,List<ExcelHead> headList);
	
	/**
	 * 读取Excel文件信息
	 * @param s 输入流
	 * @return
	 * @throws Exception 读取Excel异常信息
	 */
	public Workbook readExcel(InputStream s) throws Exception;
	
	/**
	 * 读取Excel文件信息
	 * @param filePath 文件路径
	 * @return
	 * @throws Exception 读取Excel异常信息
	 */
	public Workbook readExcel(String filePath) throws Exception;
	
	/**
	 * 获取数据有效性设置帮助类，兼容97-03 和 07-10格式
	 * @param sheet
	 * @return
	 */
	public DataValidationHelper getdataValidationHelper(Sheet sheet);
	
	/**
	 * 获取工作表对象
	 * @return
	 */
	public Workbook getWorkbook();
	
}
