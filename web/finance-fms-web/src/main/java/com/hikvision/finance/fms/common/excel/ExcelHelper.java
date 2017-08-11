/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-1-21 下午12:14:35
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.excel;

import java.io.File;
import java.io.FileInputStream;
import java.lang.reflect.InvocationTargetException;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang3.StringUtils;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.DateUtil;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.google.common.collect.Multimap;
import com.hikvision.finance.fms.common.excel.model.ExcelColumn;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.excel.util.ExcelBeanUtils;
import com.hikvision.finance.fms.common.excel.util.ExcelUtils;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fwork.exception.ExpectedException;

/**
 * <p>
 * 导入、导出的Excel帮助类
 * </p>
 * 导入数据到相关泛型对象T中
 * @author jinxindong 2016年4月21日 下午1:10:35
 * @version V1.0
 * @param <T>
 */
public class ExcelHelper<T> {
	
	private IPoiExcelOperation excelOperation;
	
	public IPoiExcelOperation getExcelOperation() {
		return excelOperation;
	}
	
	public void setExcelOperation(IPoiExcelOperation excelOperation) {
		this.excelOperation = excelOperation;
	}
	
	public ExcelHelper(IPoiExcelOperation excelOperation) {
		this.excelOperation = excelOperation;
	}
	
	/**
	 * <p>
	 * 将excel中数据导入到list中
	 * </p>
	 * 文件读取失败会抛出ExpectedException
	 * @author jinxindong 2016年4月21日 下午1:05:12
	 * @param head 文件头信息
	 * @param file 导入的数据源
	 * @param cls 保存当前数据的对象
	 * @return
	 */
	public List<T> importToObjectList(ExcelHead head, File file, Class<T> cls) {
		List<T> contents = null;
		FileInputStream fis;
		List<List<?>> rows = null; // 根据excel 每行 生成list类型的数据
		try {
			fis = new FileInputStream(file);
			rows = excelFileConvertToList(fis);
		} catch (Exception ex) {
			ex.printStackTrace();
			throw new ExpectedException("", "读取文件失败");
		}
		// 1.删除头信息
		if (rows != null) {
			for (int i = 0; i < head.getRowCount(); i++) {
				rows.remove(0);
			}
		}
		// 2.将表结构转换成Map
		Map<Integer, String> excelHeadMap = convertExcelHeadToMap(head.getColumns());
		// 3.构建为对象
		if (rows != null) {// coverity检测的空指针问题
			contents = buildDataObject(excelHeadMap, head.getColumnsConvertMap(), rows, cls);
		}
		return contents;
	}
	
	/**
	 * 将Excel文件内容转换为List对象
	 * @author jinxindong 2016年4月21日 下午1:12:38
	 * @param fis
	 * @return
	 * @throws Exception
	 */
	public List<List<?>> excelFileConvertToList(FileInputStream fis) throws Exception {
		Workbook wb = this.excelOperation.readExcel(fis);
		Sheet sheet = wb.getSheetAt(0);
		List<List<?>> rows = new ArrayList<List<?>>();
		if (sheet != null) {
			for (Row row : sheet) {
				if (!ExcelUtils.isBlankRow(row)) {
					List<Object> cells = new ArrayList<Object>();
					for (Cell cell : row) {
						Object obj = null;
						obj = this.getValue(cell);
						cells.add(obj);
					}
					rows.add(cells);
				}
			}
		}
		return rows;
	}
	
	/**
	 * 将Excel中的数据类型进行转换
	 * @author jinxindong 2016年4月21日 下午1:12:45
	 * @param cell
	 * @return
	 */
	private Object getValue(Cell cell) {
		Object value = null;
		if (cell != null) {
			switch (cell.getCellType()) {
				case Cell.CELL_TYPE_STRING:
					value = cell.getRichStringCellValue().getString();
					break;
				case Cell.CELL_TYPE_NUMERIC:
					if (DateUtil.isCellDateFormatted(cell)) {
						value = cell.getDateCellValue();
					} else {
						BigDecimal big = new BigDecimal(cell.getNumericCellValue());
						String strValue = big.toString();
						// 解决1234.0 去掉后面的.0
						if (null != strValue && !"".equals(strValue.trim())) {
							String[] item = strValue.split("[.]");
							if (1 < item.length && "0".equals(item[1])) {
								strValue = item[0];
							}
						};
						value = strValue;
					}
					break;
				case Cell.CELL_TYPE_BOOLEAN:
					value = cell.getBooleanCellValue();
					break;
				case Cell.CELL_TYPE_FORMULA:
					value = String.valueOf(cell.getNumericCellValue()); // 读公式计算值
					if (value.equals("NaN")) { // 如果获取的数据值为非法值,则转换为获取字符串
						value = cell.getStringCellValue().toString();
					}
					break;
				default:
					value = null;
			}
		}
		return value;
	}
	
	/**
	 * 将报表结构转换成Map
	 * @author jinxindong 2016年4月21日 下午1:12:54
	 * @param excelColumns
	 * @return
	 */
	private Map<Integer, String> convertExcelHeadToMap(List<ExcelColumn> excelColumns) {
		Map<Integer, String> excelHeadMap = new HashMap<Integer, String>();
		for (ExcelColumn excelColumn : excelColumns) {
			if (StringUtils.isNotEmpty(excelColumn.getFieldName())) {
				excelHeadMap.put(excelColumn.getIndex(), excelColumn.getFieldName());
			}
		}
		return excelHeadMap;
	}
	
	/**
	 * 根据Excel生成数据对象
	 * @author jinxindong 2016年4月21日 下午1:13:08
	 * @param excelHeadMap 表头信息
	 * @param excelHeadConvertMap 需要特殊转换的单元
	 * @param rows Excel文件中数据的List对象
	 * @param cls 转换为的对象
	 * @return
	 */
	private List<T> buildDataObject(Map<Integer, String> excelHeadMap, Map<String, Map<?, ?>> excelHeadConvertMap,
	        List<List<?>> rows, Class<T> cls) {
		List<T> contents = Lists.newArrayList();
		for (List<?> list : rows) {
			// 1.如果当前第一列中无数据,则忽略当前行的数据
			if (list == null || list.get(0) == null) {
				break;
			}
			// 2.当前行的数据放入map中,生成<fieldName, value>的形式
			Map<String, Object> rowMap = rowListToMap(excelHeadMap, excelHeadConvertMap, list);
			// 3.将当前行转换成对应的对象
			T obj = null;
			try {
				
				obj = cls.newInstance();
			} catch (InstantiationException ex) {
				ex.printStackTrace();
			} catch (IllegalAccessException ex) {
				ex.printStackTrace();
			}
			
			try {
				ExcelBeanUtils.populateBean(obj, rowMap);
			} catch (IllegalAccessException e) {
				e.printStackTrace();
				throw new ExpectedException("", "导入文件内容有误!");
			} catch (InvocationTargetException e) {
				e.printStackTrace();
				throw new ExpectedException("", "导入文件内容有误!");
			}
			
			contents.add(obj);
		}
		return contents;
	}
	
	/**
	 * 将行转行成map,生成<fieldName, value>的形式
	 * @author jinxindong 2016年4月21日 下午1:14:02
	 * @param excelHeadMap 表头信息
	 * @param excelHeadConvertMap 需要转换的信息
	 * @param list excel中的数据
	 * @throws ExpectedException 当导入文件不是按模板定义好的格式时，抛出异常
	 * @return
	 */
	private Map<String, Object> rowListToMap(Map<Integer, String> excelHeadMap, Map<String, Map<?, ?>> excelHeadConvertMap,
	        List<?> list) {
		Map<String, Object> rowMap = new HashMap<String, Object>();
		if (excelHeadMap.size() > list.size()) {
			throw new ExpectedException("", "导入文件的内容格式有误");
		}
		for (int i = 0; i < list.size(); i++) {
			String fieldName = excelHeadMap.get(i);
			if (fieldName != null) {
				// 得到一行数据中每个单元格的value
				Object value = list.get(i);
				if (excelHeadConvertMap != null && excelHeadConvertMap.get(fieldName) != null) {
					value = excelHeadConvertMap.get(fieldName).get(value);
				}
				rowMap.put(fieldName, value);
			}
		}
		return rowMap;
	}
	
	/*--------------------------------导出功能在com.hikvision.finance.fms.common.util下完成-----------------------------------*/
	
	/**
	 * 获取根节点数据
	 * @author jinxindong 2016年4月21日 下午2:18:47
	 * @param fis
	 * @return
	 */
	public Map<String,Object> getRootDate(File orgFile){
		Map<String,Object> map = Maps.newHashMap();
		map.put("errorNo", true);
		Workbook wb;
        FileInputStream fis;
        String nregionName = "";
		try {
			fis = new FileInputStream(orgFile);
	        wb = this.excelOperation.readExcel(fis);
        } catch (Exception e) {
        	e.printStackTrace();
        	throw new ExpectedException("", "读取表格失败");
        }
		Sheet sheet = wb.getSheetAt(0);
		if (!ExcelUtils.isBlankRow(sheet.getRow(1))) {
			Cell cell = sheet.getRow(1).getCell(3);//第2行的第四格
			nregionName = cell.getStringCellValue();
		}else{
			//throw new ExpectedException("", "根节点的父节点在表格中不存在");
			map.put("errorNo", false);
			map.put("errorInfo", "表格中所属组织根节点不存在");
		}
	   map.put("nregionName", nregionName);
       return map;
	}

	
	/**
	 * 将excel中数据导入到list中
	 * @author jinxindong 2016年4月21日 下午2:31:52
	 * @param head
	 * @param file
	 * @param cls
	 * @return
	 */
	public Map<String, Object> validateDate(File file) {
		Multimap<String, RegionInfo> myMultimap = ArrayListMultimap.create();
		Map<String, Object> map = Maps.newHashMap();
		map.put("errorNo", true);
		FileInputStream fis;
		List<Object> cells1 = new ArrayList<Object>();
		List<Object> cells2 = new ArrayList<Object>();
		List<Object> cells3 = new ArrayList<Object>();
		// 判断cells1是否重复和cells2是否重复
		try {
			fis = new FileInputStream(file);
			Workbook wb = this.excelOperation.readExcel(fis);
			Sheet sheet = wb.getSheetAt(0);
			if (sheet != null) {
				for (int i = 1; i <= sheet.getLastRowNum(); i++) {
					Row row = sheet.getRow(i);//迭代每一行Row row = sheet.getRow(i);
					if (row == null) {
					    continue;
					}
					for (int j = 0; j <row.getLastCellNum(); j++) {
						if(j==0){
							Cell cell1 = row.getCell(j);
							if(!cells1.contains(this.getValue(cell1))){
								cells1.add(this.getValue(cell1));
							}else{
								//throw new ExpectedException("", "组织节点列含有重复项");
								map.put("errorNo", false);
								map.put("errorInfo", "所属组织节点列含有重复项");
								return map;
							}
						}
						
						if(j==1){
							Cell cell3 = row.getCell(1);
							if(!cells3.contains(this.getValue(cell3))){
								cells3.add(this.getValue(cell3));
							}else{
								//throw new ExpectedException("", "组织编码含有重复项");
								map.put("errorNo", false);
								map.put("errorInfo", "组织编码含有重复项");
								return map;
							}
						}
						
						if (i > 1 && j==3) {
							Cell cell2 = row.getCell(3);
							cells2.add(this.getValue(cell2));
						}
					}
					//组装Map<父区域名字-区域对象>
					RegionInfo regionnInfo = new RegionInfo();
					regionnInfo.setStrName(row.getCell(0).getStringCellValue());
					regionnInfo.setStrCode(row.getCell(1).getStringCellValue());
					regionnInfo.setStrTxt(row.getCell(2).getStringCellValue());
					regionnInfo.setNparentName(row.getCell(3).getStringCellValue());
					myMultimap.put(regionnInfo.getNparentName(), regionnInfo);
				}
			} else {
				//throw new ExpectedException("", "读取文件内容为空");
				map.put("errorNo", false);
				map.put("errorInfo", "读取文件内容为空");
				return map;
			}
			 // 以及cells2的每个元素是否在cells1中能找到
			for(Object object:cells2){
				if(!cells1.contains(object)){
					//throw new ExpectedException("", "组织父节点列含有未知组织");
					map.put("errorNo", false);
					map.put("errorInfo", "所属组织节点列含有所在列以外的组织");
					return map;
				}
			}
			//校验
		} catch (Exception ex) {
			ex.printStackTrace();
			//throw new ExpectedException("", "读取文件失败");
			map.put("errorNo", false);
			map.put("errorInfo", "读取文件失败");
			return map;
		}
		map.put("myMultimap", myMultimap);
		return map;
	}
	

}
