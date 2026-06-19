# 背景：なぜ CFR ソルバーを自作するのか

このプロジェクトに至るまでに調べた、ポーカー計算ライブラリの全体像と判断の記録。

## ポーカー計算ライブラリの3カテゴリ

調べた結果、用途で3つに大別できた。

| カテゴリ | 代表 | 言語 | 性質 |
|---------|------|------|------|
| **エクイティ／ハンド評価** | PokerStove, OMPEval, Cactus Kev, 2+2 evaluator | C++ | 確定した数学。速さ・使いやすさで評価（論文不要） |
| **研究フレームワーク** | OpenSpiel, PokerKit | C++コア＋Python / Python | アルゴリズム研究用。論文の裏付けあり。ポーカーは小さなゲーム中心 |
| **GTO ソルバー（製品）** | TexasSolver, PioSOLVER, Desktop Postflop | C++ / Rust | 実戦 NLHE を解く。高度に最適化されたアプリ |

## 調べて分かった重要な区別

### ① アルゴリズム vs ライブラリ vs ツール

- **CFR はアルゴリズム（手法）**であって、ライブラリではない。
  「クイックソート（手法）↔ `std::sort`（実装）」と同じ関係。
- CFR を**実装したもの**が OpenSpiel（ライブラリ）や TexasSolver（ツール）。

### ② エクイティ計算 vs GTO を解く

```
エクイティ計算    … 確定した組合せ数学   → 論文不要（PokerStove / OMPEval）
GTO戦略・均衡を解く … 未解決の研究問題     → 論文の本丸（CFR / OpenSpiel）
```

「AA vs KK は 82%」は全列挙すれば厳密に出るので、エクイティ計算に論文は要らない。
論文が本領を発揮するのは「最適戦略をどう計算するか（GTO を解く）」側。

### ③ 速い C++ は「ソルバーアプリ」の中にある

GTO の重い計算は確かに C++/GPU で最適化されている（TexasSolver, PioSOLVER）。
ただし**それらは「完成品アプリ」として作られており、`#include` して呼ぶ汎用
ライブラリではない**。GTO 解法はゲーム木抽象化・ベット・レンジが一体化した縦に
深い専用システムで、再利用可能な「部品」になりにくいため。

→ 「`std::sort` のように呼べる、速い C++ GTO ライブラリ」というエコシステムは、
実はあまり存在しない。

## OpenSpiel について（研究フレームワークの代表）

- DeepMind 製。コアは C++、利用は主に Python（`pip install open-spiel`）。
- `CFRSolver`・`BestResponse`・exploitability 計算などを備える。
- ポーカーは **Kuhn / Leduc**（おもちゃ）。**実戦 NLHE は対象外**。
- 「GTO/CFR の仕組みを学ぶ」には最適だが、「GTO Wizard 並みの NLHE 解」は得られない。

## なぜ自作するのか

| 動機 | 内容 |
|------|------|
| 速い C++ で書きたい | CFR は Python でしか書けないわけではない。C++ で十分速く書ける |
| ライブラリの作法を学ぶ | エンジンとゲームの分離など、良い設計を実地で学ぶ |
| 車輪の再発明はしない | ハンド評価など確定済みの部分は既存（OMPEval）を流用する |
| 学習リポジトリの趣旨 | 実験 → 面白ければ別リポへ独立、という運用に合う |

「呼べる速い C++ GTO ライブラリ」は期待ほど無いが、**CFR を自分で書く題材は十分ある**。
Kuhn poker の CFR は約100〜200行で、GTO が収束する様子を自分のコードで観察できる。

## 既存の無料 GTO ソルバー（参考）

実戦の GTO 解が安く欲しいだけなら、自作せずこれらを使うのが早い：

- TexasSolver（C++・GPU版あり）
- Desktop Postflop / WASM Postflop（Discounted CFR・ブラウザ版あり）

## 出典

- [TexasSolver (GitHub)](https://github.com/bupticybee/TexasSolver)
- [WASM Postflop (GitHub)](https://github.com/b-inary/wasm-postflop)
- [OpenSpiel (GitHub)](https://github.com/google-deepmind/open_spiel) ／ [論文 arXiv:1908.09453](https://www.arxiv-vanity.com/papers/1908.09453/)
- [PokerKit 論文 (arXiv:2308.07327)](https://arxiv.org/pdf/2308.07327)
