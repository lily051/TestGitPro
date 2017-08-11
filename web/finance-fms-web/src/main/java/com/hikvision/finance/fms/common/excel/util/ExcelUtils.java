/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-1-22 下午2:33:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.excel.util;


import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.DateUtil;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.util.CellRangeAddress;


/**
 * Excel辅助类
 * @author jinxindong 2016年4月21日 下午1:11:46
 * @version V1.0
 */
public class ExcelUtils {
	/**
	 * <p>判断Excel数据是否是空行</p>
	 * @param row
	 * @return
	 */ 
	public static boolean isBlankRow(Row row) {
		boolean isBlank = true;
		int cells = row.getPhysicalNumberOfCells();
		for (int i = 0; i < cells; i++) {
			Cell cell = row.getCell(i);
			if (cell != null && cell.getCellType() != Cell.CELL_TYPE_BLANK) { 
				isBlank = false;
				break;
			}
		}
		return isBlank;
	}
	/**
	 * <P>判断指定的单元格是否是合并单元格</P>
	 * @param sheet
	 * @param row 行下标
	 * @param column 列下标
	 * @return
	 */
	public static boolean isMergedRegion(Sheet sheet, int row, int column) {
		int sheetMergeCount = sheet.getNumMergedRegions();
		for (int i = 0; i < sheetMergeCount; i++) {
			CellRangeAddress range = sheet.getMergedRegion(i);
			if(range != null){
				int firstColumn = range.getFirstColumn();
				int lastColumn = range.getLastColumn();
				int firstRow = range.getFirstRow();
				int lastRow = range.getLastRow();
				if (row >= firstRow && row <= lastRow) {
					if (column >= firstColumn && column <= lastColumn) {
						return true;
					}
				}
			}
		}
		return false;
	}
	/**
	 * <p>获取合并单元格</P>
	 * @author maxianming 2016-1-25 上午10:11:09
	 * @param sheet
	 * @param row
	 * @param column
	 * @return 
	 */
	public static Cell getMergedRegionValue(Sheet sheet, int row, int column) {
		int sheetMergeCount = sheet.getNumMergedRegions();
		for (int i = 0; i < sheetMergeCount; i++) {
			CellRangeAddress ca = sheet.getMergedRegion(i);
			if(ca != null){
				int firstColumn = ca.getFirstColumn();
				int lastColumn = ca.getLastColumn();
				int firstRow = ca.getFirstRow();
				int lastRow = ca.getLastRow();
				if (row >= firstRow && row <= lastRow) {
					if (column >= firstColumn && column <= lastColumn) {
						Row fRow = sheet.getRow(firstRow);
						Cell fCell = fRow.getCell(firstColumn);
						return fCell;
					}
				}
			}
		}
		
		return null;
	}
	 /**
	  * <p>判断是否为date形式数据</p>
	  * @author maxianming 2016-1-22 下午12:08:11
	  * @param cell
	  * @return
	  */
	 public static boolean isCellDateFormatted(Cell cell) {
	     if (cell == null)
	         return false;
	     boolean bDate = false;
	     double d = cell.getNumericCellValue();
	     if (DateUtil.isValidExcelDate(d)) {
	         CellStyle style = cell.getCellStyle();
	         if (style == null)
	             return false;
	         String f = style.getDataFormatString();
	         if (f.indexOf("y") > -1 && f.indexOf("m") > -1
	                 && f.indexOf("d") > -1) {
	             return true;
	         }
	     }
	     return bDate;
	 }
	  /**
	   * <p>判断是是否为time形式数据</P>
	   * @author maxianming 2016-1-22 下午12:08:16
	   * @param cell
	   * @return
	   */
	 public static boolean isCellTimeFormatted(Cell cell) {
	     if (cell == null)
	         return false;
	     boolean bDate = false;
	     double d = cell.getNumericCellValue();
	     if (DateUtil.isValidExcelDate(d)) {
	         CellStyle style = cell.getCellStyle();
	         if (style == null)
	             return false;
	         String f = style.getDataFormatString();
	         if (f.indexOf("mm") > -1 && f.indexOf("ss") > -1
	                 && f.indexOf("h") > -1) {
	             return true;
	         }
	     }
	     return bDate;
	 } 
	 
}
