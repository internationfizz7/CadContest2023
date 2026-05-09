# CadContest 2023 — Problem B（3D IC Floorplanner）

This is the full source code of the **ICCAD 2023 Contest Problem B** entry, which received the **佳作（Honorable Mention）** award.

The tool performs 3D placement for macros and standard cells while minimizing HPWL, with full TSV (Through-Silicon Via) awareness.

Problem spec: https://iccad-contest.org/Problems.html → Problem B

---

## 快速上手（Docker — 推薦給新人）

> 執行檔是 Linux x86-64 binary，在 macOS / Windows 上需要透過 Docker 執行。

### 步驟一：安裝 Docker Desktop

- macOS：`brew install --cask docker`，安裝後從 Applications 開啟 Docker Desktop
- Windows：https://www.docker.com/products/docker-desktop 下載安裝

安裝後，從 **Applications 開啟 Docker Desktop app**，等到 menu bar 的鯨魚 icon 停止轉動，再確認 daemon 是否正常：

```bash
docker ps
```

看到表頭輸出（即使是空的）代表成功：
```
CONTAINER ID   IMAGE   COMMAND   CREATED   STATUS   PORTS   NAMES
```

> 如果看到 `cannot connect` 或 `failed to connect`，表示 Docker Desktop app 還沒開啟或還在啟動中。

### 步驟二：Build Docker 映像檔（只需做一次）

在專案根目錄執行：

```bash
docker build --platform linux/amd64 --load -t ncku-floorplanner .
```

### 步驟三：執行程式

```bash
./run.sh Testcase/case02.txt case02_out.txt
```

或直接用 docker run：

```bash
docker run --rm --platform linux/amd64 \
    -v "$(pwd)":/workspace \
    -w /workspace \
    ncku-floorplanner \
    ./NCKUFplanner Testcase/case02.txt case02_out.txt
```

### 測試案例

| 檔案 | 說明 |
|------|------|
| `Testcase/case02.txt` | 測試案例 02 |
| `Testcase/case03.txt` | 測試案例 03 |
| `Testcase/case04.txt` | 測試案例 04 |

---

## 程式流程

```
Parser → Partitioning (FM) → Global Placement (QP) →
Macro Legalization (ML) → Cell Legalization (Abacus) →
TSV Assignment → Output
```

---

## 檔案結構

```
CadContest2023/
├── NCKUFplanner      # 預編譯執行檔（Linux x86-64）
├── Dockerfile        # Docker 環境定義
├── run.sh            # 一鍵執行腳本
├── Testcase/         # 測試輸入檔
├── src/              # C++ 原始碼
├── include/          # 標頭檔
└── cor/              # 舊版程式碼備份
```

---

## 從原始碼編譯（進階）

> 這個方法會在 Docker 裡重新從 C++ source 編譯，產生新的執行檔。  
> 適合改過程式碼之後重新 build，或執行檔遺失的情況。

### 前置條件

- Docker Desktop 已安裝並開啟（同上方步驟一）
- `package/` 資料夾存在且完整（含 DEFLEF、FLUTE、LASPACK、LEMON 四個子資料夾）

這些函式庫已隨 repo 附上：

| 資料夾 | 函式庫 | 用途 |
|--------|--------|------|
| `package/DEFLEF/` | DEF/LEF parser | 電路格式解析 |
| `package/lemon-1.2.4/` | LEMON 1.2.4 | 網絡流求解 |
| `package/laspack/` | LASPACK | 共軛梯度法（QP solver） |
| `package/flute_for_HIMAX/` | FLUTE | Steiner Tree |

### 步驟：執行 build.sh

在專案根目錄執行：

```bash
./build.sh
```

這個 script 會自動：
1. 啟動 Ubuntu 22.04 (linux/amd64) 容器
2. 在容器內安裝 `g++` 和 `make`
3. 執行 `make clean && make`
4. 產生執行檔 `NCKUFplanner`（在本機專案根目錄）

編譯成功後，terminal 最後會看到：

```
g++ ... -o NCKUFplanner -no-pie ...
```

沒有 `error:` 就代表成功。

### 驗證編譯結果

```bash
ls -lh NCKUFplanner
```

看到 `NCKUFplanner` 存在（約 30–40 MB）就完成了。接著照上方步驟三執行即可。

---

## 執行方式

```bash
./NCKUFplanner <input.txt> <output.txt>
```

範例：

```bash
./run.sh Testcase/case02.txt case02_out.txt
```

測試 benchmark 可至 https://iccad-contest.org/tw/index.html Problem B 下載。
