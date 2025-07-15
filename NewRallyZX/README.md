コンパイルする方法について<br/>
<br/>
【z88dk】<br/>
 z88dkを使用しています。まずは下記からDownloadしてください。<br/>
 https://z88dk.org/site/<br/>
<br/>
 展開したz88dkを、フォルダごと適当な所にコピーします。<br/>
 私は「d:\tmp」の下にコピーしました。<br/>
<br/>
<br/>
【コマンドプロンプト】<br/>
 次に、コマンドプロンプトを開き環境変数にてPATH追加とZCCCFG定義を指定ください。<br/>
  PATH=%PATH%;d:\tmp\z88dk\bin<br/>
  set ZCCCFG=d:\tmp\z88dk\lib\config<br/>
  ※d:\tmp\z88dkまでは、z88dkのコピー先に変更ください。<br/>
<br/>
 バッチファイルを作成すると便利です。<br/>
<br/>
 最後に下記コマンドにて、コンパイルを行います。<br/>
 zcc +zx81 -create-app car.c scrn.c map.c main.c<br/>
<br/>
<br/>
【ZX81エミュレータで実行】<br/>
 a.Pファイルが出来上がるので、ZX81シミュレータを起動し<br/>
「file]-[Open Tape...」にて読み込んで起動します。<br/>
<br/>
<br/>
【操作】<br/>
<br/>
 起動するとタイトル画面となるので、以下のキーにて操作ください。<br/>
<br/>
・自車移動　…　矢印キー<br/>
・スタート、煙幕　…　スペースキー<br/>
・強制終了　…　Enterキー<br/>
<br/>
<br/>
【最後に】<br/>
 プログラムの詳細は、下記のHPにて紹介しています。<br/>
 https://plaza.rakuten.co.jp/zx81fun/<br/>
<br/>
 よかったら参照してみてください。<br/>
<br/>
<br/>
2025年7月15日追記<br/><br/>
![TITLE画面](RallyZxPlay.png?raw=true)
<br/>
![PLAY画面](RallyZxTitle.png?raw=true)
<br/>



