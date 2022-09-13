# 目標

以前作った将棋プログラムの可読性を上げるため"オブジェクト指向"を積極的に取り入れてコーディングする。
  
改定前のプログラムでは保守性が悪くコードの改善がしにくい、そのため　  
当将棋プロジェクトではオブジェクト指向を積極的に取り入れ改善していく。  
この事を留意し作りなおすこと。　

# 基本的な仕様

ボード:  
*上部をUSER1,下部をUSER2とする。  
*サイズは縦横9*9で合計81マスとする。  
表示:  
*成り駒は赤色で表示される。  
*表示される場合背景色を黄色にする。  
*選択された場合選択された駒の背景を緑色にする。  

# 駒の名前とナンバ:

玉将[王]:100　　None　　:000  
飛車[飛]:101　　竜王(成飛車)[龍]:201  
角行[角]:102　　竜馬(成角行)[馬]:202  
金将[金]:103　　None　　:000  
銀将[銀]:104　　成銀[銀]:204  
桂馬[桂]:105　　成桂[桂]:205  
香車[香]:106　　成香[香]:206  
歩兵[歩]:107　　と金[と]:207  
*駒が200番台の場合成りごまとなる。  

# include手順

*...:省略  

NewShogi.cpp　|->time.h  
 　　　　　　　|->stdlib.h  
 　　　　　　　|->board.h---->vector---->...  
　　　　　　　　　　　　　|  
　　　　　　　　　　　　　|->base.h----->stdio.h->...  
　　　　　　　　　　　　　|　　　　　　->wchar.h->...  
　　　　　　　　　　　　　|  
　　　　　　　　　　　　　|->piece.h---->move.h  
　　　　　　　　　　　　　|　　　　　　->base.h  
　　　　　　　　　　　　　|  
　　　　　　　　　　　　　|->SanaeUtil.h->SanaeBase.h->...  