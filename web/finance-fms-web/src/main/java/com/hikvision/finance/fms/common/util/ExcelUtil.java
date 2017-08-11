package com.hikvision.finance.fms.common.util;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;

import com.hikvision.finance.core.util.InvokeUtils;

/**
 * Excel工具类
 * @author jinxindong 2016年2月22日 下午2:42:59
 * @version V1.0
 */
public class ExcelUtil {
	
	private static HSSFWorkbook wb;
	private static CellStyle headStyle; // 表头行样式
	private static Font headFont; // 表头行字体
	private static CellStyle contentStyle; // 内容行样式
	private static Font contentFont; // 内容行字体
	
	/**
	 * 导出Excel文件
	 * @author jinxindong 2016年2月22日 下午2:43:17
	 * @param setInfo 文件内容的字节数组
	 * @param outputExcelFileName 完整文件名
	 * @return
	 * @throws Exception
	 */
	public static boolean export2File(ExcelExportData setInfo, String outputExcelFileName) throws Exception {
		
		// 1，创建多集目录
		File f = new File(outputExcelFileName);
		File parent = f.getParentFile();
		if (parent != null && !parent.exists()) {
			parent.mkdirs();
		}
		// 2，删除原有文件
		if (f.exists()) {
			f.delete();
		}
		// 3，创建文件
		File file = new File(outputExcelFileName);
		FileOutputStream fs = new FileOutputStream(file);
		try {
			fs.write(export2ByteArray(setInfo));
			fs.flush();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			fs.close();
		}
		return true;
	}
	
	/**
	 * 导出字节数组
	 * @author jinxindong 2016年2月22日 下午2:43:44
	 * @param setInfo
	 * @return
	 * @throws Exception
	 */
	public static byte[] export2ByteArray(ExcelExportData setInfo) throws Exception {
		ByteArrayOutputStream outputStream = export2Stream(setInfo);
		byte[] byteArray = outputStream.toByteArray();
		if (outputStream != null) {
			outputStream.close();
		}
		return byteArray;
	}
	
	/**
	 * 导出流
	 * @author jinxindong 2016年2月22日 下午2:43:52
	 * @param setInfo
	 * @return
	 * @throws Exception
	 */
	public static ByteArrayOutputStream export2Stream(ExcelExportData setInfo) throws Exception {
		// 1，初始化
		init();
		// 2，创建Sheet名称
		String[] sheetNames = new String[setInfo.getDataMap().size()];
		int sheetNameNum = 0;
		for (Entry<String, List<?>> entry : setInfo.getDataMap().entrySet()) {
			sheetNames[sheetNameNum++] = entry.getKey();
		}
		HSSFSheet[] sheets = createSheets(sheetNames);
		// 3，创建Sheet内容
		int sheetNum = 0;
		for (Entry<String, List<?>> entry : setInfo.getDataMap().entrySet()) {
			List<?> objs = entry.getValue();
			createHead(setInfo, sheets, sheetNum); // 标题
			String[] fieldNames = setInfo.getFieldNames().get(sheetNum); // 属性
			int rowNum = 1;
			for (Object obj : objs) {
				HSSFRow contentRow = sheets[sheetNum].createRow(rowNum++);
				contentRow.setHeight((short)350);
				HSSFCell[] cells = createContent(contentRow, setInfo.getFieldNames().get(sheetNum).length);
				if (fieldNames != null) {
					for (int num = 0, cellNum = 0; num < fieldNames.length; num++, cellNum++) {
						Object value = InvokeUtils.forceGetProperty(obj, fieldNames[num]);
						cells[cellNum].setCellValue(value == null?"":value.toString());
					}
				}
			}
			if (fieldNames != null) {// Coverity检测到的空指针问题
				adjustColumnSize(sheets, sheetNum, fieldNames); // 自动调整列宽
			}
			sheetNum++;
		}
		// 4，输出流
		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
		wb.write(outputStream);
		return outputStream;
	}
	
	/**
	 * Excel导出数据类
	 * @author jinxindong 2016年2月22日 下午2:44:01
	 * @version V1.0
	 */
	public static class ExcelExportData {
		
		/**
		 * 导出数据， key:String 表示每个Sheet的名称；value:List<?> 表示每个Sheet里的所有数据行
		 */
		private LinkedHashMap<String, List<?>> dataMap;
		
		/**
		 * 单个Sheet里的数据列标题
		 */
		private List<String[]> columnNames;
		
		/**
		 * 单个Sheet里每行数据的列对应的对象属性名称
		 */
		private List<String[]> fieldNames;
		
		public List<String[]> getFieldNames() {
			return fieldNames;
		}
		
		public void setFieldNames(List<String[]> fieldNames) {
			this.fieldNames = fieldNames;
		}
		
		public List<String[]> getColumnNames() {
			return columnNames;
		}
		
		public void setColumnNames(List<String[]> columnNames) {
			this.columnNames = columnNames;
		}
		
		public LinkedHashMap<String, List<?>> getDataMap() {
			return dataMap;
		}
		
		public void setDataMap(LinkedHashMap<String, List<?>> dataMap) {
			this.dataMap = dataMap;
		}
	}
	
	/**
	 * 自动调整列宽
	 * @author jinxindong 2016年2月22日 下午2:44:15
	 * @param sheets
	 * @param sheetNum
	 * @param fieldNames
	 */
	private static void adjustColumnSize(HSSFSheet[] sheets, int sheetNum, String[] fieldNames) {
		for (int i = 0; i < fieldNames.length; i++) {
			sheets[sheetNum].autoSizeColumn(i, true);
		}
	}
	
	/* ------------------------------内容的创建-------------------------------- */
	/**
	 * 创建所有Sheet
	 * @author jinxindong 2016年2月22日 下午2:44:22
	 * @param names
	 * @return
	 */
	private static HSSFSheet[] createSheets(String[] names) {
		HSSFSheet[] sheets = new HSSFSheet[names.length];
		for (int i = 0, len = names.length; i < len; i++) {
			sheets[i] = wb.createSheet(names[i]);
		}
		return sheets;
	}
	
	/**
	 * 创建表头行
	 * @author jinxindong 2016年2月22日 下午2:44:28
	 * @param setInfo
	 * @param sheets
	 * @param sheetNum
	 */
	private static void createHead(ExcelExportData setInfo, HSSFSheet[] sheets, int sheetNum) {
		// 表头
		HSSFRow head = sheets[sheetNum].createRow(0);
		head.setHeight((short)700);
		// 列头名称
		for (int num = 0, len = setInfo.getColumnNames().get(sheetNum).length; num < len; num++) {
			HSSFCell headCell = head.createCell(num);
			headCell.setCellStyle(headStyle);
			headCell.setCellValue(setInfo.getColumnNames().get(sheetNum)[num]);
		}
	}
	
	/**
	 * 创建内容行
	 * @author gengqiaolei 2015年11月18日 下午6:34:03
	 * @param contentRow
	 * @param num
	 * @return
	 */
	public static HSSFCell[] createContent(HSSFRow contentRow, int num) {
		HSSFCell[] cells = new HSSFCell[num];
		for (int i = 0; i < num; i++) {
			cells[i] = contentRow.createCell(i);
			cells[i].setCellStyle(contentStyle);
		}
		return cells;
	}
	
	/* ------------------------------表格的初始化------------------------------- */
	/**
	 * 类的初始化
	 * @author jinxindong 2016年2月22日 下午2:44:44
	 */
	private static void init() {
		wb = new HSSFWorkbook();
		headFont = wb.createFont();
		headStyle = wb.createCellStyle();
		contentFont = wb.createFont();
		contentStyle = wb.createCellStyle();
		initHeadFont();
		initHeadCellStyle();
		initContentFont();
		initContentCellStyle();
	}
	
	/**
	 * 表头行字体的初始化
	 * @author jinxindong 2016年2月22日 下午2:44:52
	 */
	private static void initHeadFont() {
		headFont.setFontName("宋体");
		headFont.setFontHeightInPoints((short)16);
		headFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
		headFont.setCharSet(Font.DEFAULT_CHARSET);
		headFont.setColor(IndexedColors.BLACK.index);
	}
	
	/**
	 * 表头行样式的初始化
	 * @author jinxindong 2016年2月22日 下午2:44:58
	 */
	private static void initHeadCellStyle() {
		headStyle.setAlignment(CellStyle.ALIGN_CENTER);
		headStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
		headStyle.setFont(headFont);
		headStyle.setBorderTop(CellStyle.BORDER_THIN);
		headStyle.setBorderBottom(CellStyle.BORDER_THIN);
		headStyle.setBorderLeft(CellStyle.BORDER_THIN);
		headStyle.setBorderRight(CellStyle.BORDER_THIN);
		headStyle.setTopBorderColor(IndexedColors.GREY_80_PERCENT.index);
		headStyle.setBottomBorderColor(IndexedColors.GREY_80_PERCENT.index);
		headStyle.setLeftBorderColor(IndexedColors.GREY_80_PERCENT.index);
		headStyle.setRightBorderColor(IndexedColors.GREY_80_PERCENT.index);
	}
	
	/**
	 * 内容行字体的初始化
	 * @author jinxindong 2016年2月22日 下午2:45:05
	 */
	private static void initContentFont() {
		contentFont.setFontName("宋体");
		contentFont.setFontHeightInPoints((short)12);
		contentFont.setBoldweight(Font.BOLDWEIGHT_NORMAL);
		contentFont.setCharSet(Font.DEFAULT_CHARSET);
		contentFont.setColor(IndexedColors.BLACK.index);
	}
	
	/**
	 * 内容行样式的初始化
	 * @author jinxindong 2016年2月22日 下午2:45:11
	 */
	private static void initContentCellStyle() {
		// contentStyle.setAlignment(CellStyle.ALIGN_CENTER);
		contentStyle.setAlignment(CellStyle.ALIGN_LEFT);
		contentStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
		contentStyle.setFont(contentFont);
		contentStyle.setBorderTop(CellStyle.BORDER_THIN);
		contentStyle.setBorderBottom(CellStyle.BORDER_THIN);
		contentStyle.setBorderLeft(CellStyle.BORDER_THIN);
		contentStyle.setBorderRight(CellStyle.BORDER_THIN);
		contentStyle.setTopBorderColor(IndexedColors.GREY_80_PERCENT.index);
		contentStyle.setBottomBorderColor(IndexedColors.GREY_80_PERCENT.index);
		contentStyle.setLeftBorderColor(IndexedColors.GREY_80_PERCENT.index);
		contentStyle.setRightBorderColor(IndexedColors.GREY_80_PERCENT.index);
		contentStyle.setWrapText(true); // 字段换行
	}
}
