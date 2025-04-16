コンパイルする方法について

【z88dk】
 z88dkを使用しています。まずは下記からDownloadしてください。
  https://z88dk.org/site/

 展開したz88dkを、フォルダごと適当な所にコピーします。
  私は「d:\tmp」の下にコピーしました。


【コマンドプロンプト】
 次に、コマンドプロンプトを開き環境変数にてPATH追加とZCCCFG定義を指定ください。
 PATH=%PATH%;d:\tmp\z88dk\bin set ZCCCFG=d:\tmp\z88dk\lib\config ※d:\tmp\z88dkまでは、z88dkのコピー先に変更ください。

 バッチファイルを作成すると便利です。

 最後に下記コマンドにて、コンパイルを行います。 zcc +zx81 -create-app MoleAttack9.c


【ZX81エミュレータで実行】
 a.Pファイルが出来上がるので、ZX81シミュレータを起動し 「file]-[Open Tape...」にて読み込んで起動します。


【操作】
 起動するとタイトル画面となるので、以下のキーにて操作ください。

 ・ATTACK　…　対応した１～９キー
 ・スタート　…　Enterキー
 ・強制終了　…　スタートキー


【最後に】 プログラムの詳細は、下記のHPにて紹介しています。 https://plaza.rakuten.co.jp/zx81fun/

よかったら参照してみてください。
