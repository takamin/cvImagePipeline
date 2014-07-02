External Reference Specification
================================

## Image Processors

### class AverageMat

直近の入力イメージの画素毎の平均画像を出力する。

平均を行う入力回数は averageCount プロパティで設定する。初期値は30。
つまり、初期状態では、過去30回の平均イメージが出力される。

#### Property

* int averageCount 平均するフレームの数。初期値は30。
