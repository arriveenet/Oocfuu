# 誕生日おめでとうございます！
![screen_shot](https://i.imgur.com/zz6NoeC.png)
メッセージとBGMが流れるしょうもないアプリです。  
本当な2Dスクロールアクションゲームを作ろうとしたのですが、技術と時間が足りず断念。  
BGMはファミコンと同じ、矩形波2チャンネル、三角波、ノイズで再現しています。  
画面上のスコアボードみたいなやつは時計になってたりします。　

# ビルド方法
- VisualStudio2022がインストールされていること
- ワークロードはC++によるデスクトップ開発がインストールされていること
## GUIによるビルド
1. Oocfuu.slnをVisualStudio2022を起動する
1. プラットフォームをx64、構成をRelease or Debugにする
1. メニューバーのビルド→ソリューションのビルドを実行
## コマンドによるビルド
1. command_build.batのある階層でコマンドプロンプトを開く
1. command_build.bat [-a] [-d] [-r]を実行
* オプション
  * -a リリースモードとデバッグモード両方でビルド
  * -d デバッグモードでビルド
  * -r リリースモードでビルド

# 使用方法 
1. Oocfuu.exeをダブルクリックし、実行します。
2. スペースキーを押してスタートします。

# 参考資料
* 楽譜
  * [楽譜の読み方](https://www.print-gakufu.com/guide/4003/)
  * [Happy Birthday to you](https://www.youtube.com/watch?v=Iokd6iGyQbQ) 
  * [タイフーンパレード](https://www.youtube.com/watch?v=9Cl9OcSgbvE)
  * [VOYAGER〜日付のない墓標](https://www.print-gakufu.com/score/detail/473445/)
* プログラム
  * [OpenGL道場](https://www.youtube.com/watch?v=wtC03LR3VFo&list=PL8_ASIpg7ciG3btmV6RElRjmWrfAL0q2P)
  * [OpenAL道場](https://www.youtube.com/watch?v=bX_Gh5m99Xg&list=PL8_ASIpg7ciEx1nynwoL1EOxpGQ3iBCOf)
  * [スーパーマリオを作ってみた](https://www.youtube.com/watch?v=WPgDIs70Mvg&list=PL8_ASIpg7ciFmpJ84Wcig5Mj28eCMnsfh)
  * [スペースインベーダー作ってみた](https://www.youtube.com/watch?v=J0MA8c0bPHw&list=PL8_ASIpg7ciEKxStzvx0Xg-TF51P1hfto)
