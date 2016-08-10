/******************************************************************************************
 * plati_temp.h
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * 白金薄膜温度センサデータ取得ヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/01/14 || Tetsuya Kaku
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef PLATI_TEMP_H
#define	PLATI_TEMP_H


//static double get_pt(void);        // これはstatic関数なのでいらない

/*=====================================================
 * @brief
 *     温度データ取得関数
 * @param
 *     なし
 * @return
 *     平均温度データ
 * @note
 *     温度を取得する関数
 *===================================================*/
double get_temp(void);



#endif	/* PLATI_TEMP_H */