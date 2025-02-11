# haco3


esp32で動くミニゲーム開発プラットフォームです。

基板を発注して、パーツを組み合わせれば、PICO-8ライクな書き方でミニゲームが作れます。

ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

「haco3editor」

haco3ゲームをプログラミングして下のハードウエアに転送するためのソフト（win,mac両方あります）

https://drive.google.com/drive/folders/1Eny3NMIrRxlyB-WC0IIFq0EagY_Ctn1x?usp=sharing

ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

「てづくりれとろ」
![Image](https://github.com/user-attachments/assets/b0eb5afc-7657-48cf-9c26-ed7b01d68ba6)

haco3をインストールして動かすためのハードウエアです。

tezukuriretoro.zipという基板用のガーバーデータを追加しました。

基板はPLCPCBなどで500~1000円で発注できます。

基本部品(5000~6000円）程度

--

TFT液晶2.8インチタッチパネル付き1個1500円

https://ja.aliexpress.com/item/1005006623369442.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.1.2d47a5Cza5CzBO&gps-id=pcDetailTopMoreOtherSeller&scm=1007.14452.396806.0&scm_id=1007.14452.396806.0&scm-url=1007.14452.396806.0&pvid=3165c3cf-c313-4545-a8cd-5d3b5acdef8c&_t=gps-id:pcDetailTopMoreOtherSeller,scm-url:1007.14452.396806.0,pvid:3165c3cf-c313-4545-a8cd-5d3b5acdef8c,tpp_buckets:668%232846%238114%231999&pdp_npi=4%40dis%21JPY%21859%21859%21%21%2140.56%2140.56%21%402141115b17392709119556174e0a2d%2112000037852145489%21rec%21JP%21178272571%21X&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A&search_p4p_id=202502110248319936289498010760422639_0

タクトスイッチ3個100~300円

https://ja.aliexpress.com/item/4000544090602.html?spm=a2g0o.productlist.main.3.4c8251028zXh88&algo_pvid=6c0a256a-8bf0-4e69-8160-52db26d62564&algo_exp_id=6c0a256a-8bf0-4e69-8160-52db26d62564-1&pdp_ext_f=%7B%22order%22%3A%22884%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21JPY%21451%21451%21%21%212.91%212.91%21%40214100f417392712054285725e42c7%2110000002795928570%21sea%21JP%21178272571%21X&curPageLogUid=6xhgWfRlmfcF&utparam-url=scene%3Asearch%7Cquery_from%3A

Bluetooth開発ボード,wifiモジュール,ttgo,t8,v1.8,ESP32-WROVER, 8mb,psram,tfカード1個2000円

https://ja.aliexpress.com/item/1005006639630413.html?spm=a2g0o.order_list.order_list_main.65.644b585aLsBgGt&gatewayAdapt=glo2jpn

スピーカー1個300円

https://ja.aliexpress.com/item/1005006358049156.html?spm=a2g0o.order_list.order_list_main.53.644b585aLsBgGt&gatewayAdapt=glo2jpn

リミットスイッチ（２回路切替スイッチ）4個で300円

https://ja.aliexpress.com/item/4000638031271.html?spm=a2g0o.order_list.order_list_main.365.644b585aLsBgGt&gatewayAdapt=glo2jpn

アナログジョイスティック1個で100~300円

https://ja.aliexpress.com/item/1005003608244861.html?spm=a2g0o.order_list.order_list_main.5.644b585aLsBgGt&gatewayAdapt=glo2jpn

キャップ1個50円

https://ja.aliexpress.com/item/1005008196773113.html?spm=a2g0o.detail.0.0.6d01l7Gpl7Gpbd&mp=1&_gl=1*cypf7q*_gcl_aw*R0NMLjE3MzkyNzA1NDMuQ2p3S0NBaUFoNnk5QmhCUkVpd0FwQkxIQzF4b2Z0Q19RMzNCcDdqY0JWM1M0NTA1ZVR3TzVmNXpvMjgwcktiNFJGOTBzSW1KdmxqYllSb0NXaWNRQXZEX0J3RQ..*_gcl_au*ODgxMzcxNTE3LjE3MzkyNzA1NDI.*_ga*MTY2MTEwMjA0NS4xNjc2MzgyOTE2*_ga_VED1YSGNC7*MTczOTI3MDU0My4zLjEuMTczOTI3MDYzNy42MC4wLjA.&gatewayAdapt=glo2jpn

ピンヘッダ（オス・メス）500円程度

オス

17列（2個）

14列（1個）

4列（1個）

メス

17列（2個）

14列（1個）

4列（1個）

https://ja.aliexpress.com/item/32724478308.html?spm=a2g0o.order_list.order_list_main.394.644b585aLsBgGt&gatewayAdapt=glo2jpn

抵抗（100〜300円）

２回路切替スイッチ用（４種類）

9.1KΩ

4.7KΩ

2KΩ

1KΩ

タクトスイッチ用

100KΩ（3個）

--

外部出力したい時（はじめは必要ない）

ヘッドホンジャックソケット（音声）

https://ja.aliexpress.com/item/4001044110474.html?spm=a2g0o.order_list.order_list_main.200.644b585aLsBgGt&gatewayAdapt=glo2jpn

RCA-AVソケット（映像の外部出力はプログラミングが必要）

https://ja.aliexpress.com/item/1005006135848173.html?spm=a2g0o.order_list.order_list_main.204.644b585aLsBgGt&gatewayAdapt=glo2jpn

ーーーーーーーーーーーーーーーーーーーーーーーーー

ina_aniさんのo-bakoをlovyanGFXに移植し、タッチパネル対応にするため自作ライブラリDentaroUI（同梱のカスタム仕様）を追加したのがhaco3です。

スプライトも自作です。LovyanGFXもo-bakoも素晴らしいプロジェクトで、この２つをベースにhaco3はできています。

https://inajob.github.io/o-bako-simulator/index.html

Lua5.3が動いています。

platformIOプロジェクトのフォルダをコピーした「コード」だけをzipあげています。

ビルドオプションの設定などが必要なため、インストールはplatformIOでしかできません。（Arduino不可）
