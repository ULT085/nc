# 每日睡前 Podcast 任務說明

這份文件是給每日排程任務（Claude）照著執行的流程。

## 流程

1. 在 `podcasts` 分支上工作：`git fetch origin main podcasts && git checkout -B podcasts origin/podcasts`。
   繳交紀錄由 NeetCode 同步到 `main`，所以要讀的是 `origin/main`。
2. 確認 TTS 套件：`pip install -q edge-tts`（環境的 setup script 通常已安裝）。
3. 讀 `podcasts/state.json` 拿到 `last_head`，執行
   `python3 podcast/find_submissions.py --ref origin/main --since-commit <last_head>`，取得新的繳交。
4. 選題：
   - 新題目（今天有繳交的）全部優先，每集最多 6 題。超過 6 題時，挑繳交次數最多或最難的 6 題，其餘加到 `backlog` 尾端。
   - 新題目少於 3 題時，從 `backlog` 開頭補到 3 題，並在講稿中標明是「舊題回顧」。
   - 新題目與 backlog 都沒有時：不產生集數，只更新 `last_head`，結束。
5. 用 `git show origin/main:<path>` 讀每題的**所有**繳交版本（`all_submissions`），理解題目與寫法演進。
6. 寫講稿到 `podcasts/<台北日期 YYYY-MM-DD>/script.md`（格式見下），再執行
   `python3 podcast/tts.py podcasts/<日期>/script.md podcasts/<日期>/episode.mp3`。
7. 更新 `podcasts/state.json`：`last_head` 設為 find_submissions 輸出的 `head`，移除已講過的 backlog 題，`episodes` 加一筆。
   更新 `podcasts/README.md` 的集數列表（最新在最上面，附 mp3 連結和題目）。
8. commit 並 `git push origin podcasts`（網路錯誤最多重試 4 次）。

## 講稿寫法

- 語言：繁體中文、口語、溫和的睡前語氣。聽眾閉著眼睛聽，**不要逐行念程式碼**，也不要用 Markdown 表格或程式碼區塊（TTS 會略過）。
- 結構：開場（今天有哪幾題）→ 每題一個 `## 標題` 段落 → 「今晚的重點回顧」→ 一個留給夢裡想的延伸問題 → 晚安。
- 每題涵蓋：
  1. 一兩句講清楚題目。
  2. 暴力解為什麼不夠好（一句即可）。
  3. **使用者自己的解法**的核心思路，用生活化比喻，按執行流程走一遍。
  4. 關鍵細節／容易出錯的地方（例如更新順序、邊界、哨兵節點）。
  5. 時間與空間複雜度。
  6. 有多個繳交版本時，說明版本間的演進（重構、換演算法）。
  7. 真的有 bug、記憶體洩漏或可改進處，溫和地點出來。
  8. 一句好記的「關鍵句」。
- 每題約 400–700 字；整集控制在 5–15 分鐘。
- 符號念法要能被唸出來：寫「O(n)」或「線性時間」，避免 `->`、`[]` 這類符號。
