# Testcase 格式說明

## 輸入檔：`caseXX.txt`

輸入檔分為四大區段，依序如下：

---

### 區段一：Technology Library（元件庫定義）

定義 standard cell 的幾何尺寸與 pin 位置。

```
NumTechnologies <技術數量>
Tech <技術名稱> <row高度>
  LibCell <類型> <元件名稱> <寬度> <高度> <pin數量>
    Pin <pin名稱> <相對X偏移> <相對Y偏移>
    ...
```

範例：
```
NumTechnologies 1
Tech TA 221
LibCell N MC1 382 33 3
  Pin P1 46 0
  Pin P2 145 0
  Pin P3 49 0
```

---

### 區段二：Die 與佈置限制

定義晶片尺寸、利用率上限、row 規格、使用技術，以及 TSV（矽穿孔）參數。

```
DieSize <lx> <ly> <ux> <uy>

TopDieMaxUtil    <百分比>
BottomDieMaxUtil <百分比>

TopDieRows    <startX> <startY> <endX> <rowHeight> <rowCount>
BottomDieRows <startX> <startY> <endX> <rowHeight> <rowCount>

TopDieTech    <技術名稱>
BottomDieTech <技術名稱>

TerminalSize    <寬> <高>
TerminalSpacing <間距>
TerminalCost    <成本係數>
```

範例：
```
DieSize 0 0 23000 19000

TopDieMaxUtil    80
BottomDieMaxUtil 80

TopDieRows    0 0 23000 33 573
BottomDieRows 0 0 23000 33 573

TopDieTech    TA
BottomDieTech TA

TerminalSize    92 92
TerminalSpacing 92
TerminalCost    10
```

---

### 區段三：Instances（電路元件清單）

```
NumInstances <元件總數>
Inst <元件名稱> <所屬LibCell名稱>
...
```

範例：
```
NumInstances 13907
Inst C1 MC1
Inst C2 MC2
```

---

### 區段四：Nets（連線網表）

```
NumNets <網路總數>
Net <網路名稱> <pin數量>
  Pin <元件名稱>/<pin名稱>
  ...
```

範例：
```
NumNets 19547
Net N1 2
  Pin C1/P1
  Pin C13848/P1
Net N2 3
  Pin C2794/P1
  Pin C88/P1
  Pin C13849/P1
```

---

## 輸出檔：`caseXX_out.txt`

輸出分為三大區段：Top Die 佈置、Bottom Die 佈置、TSV Terminal 位置。

---

### 區段一：Top Die 佈置結果

```
TopDiePlacement <元件數量>
Inst <元件名稱> <x> <y> <方向>
...
```

---

### 區段二：Bottom Die 佈置結果

```
BottomDiePlacement <元件數量>
Inst <元件名稱> <x> <y> <方向>
...
```

- `<x> <y>`：元件左下角座標
- `<方向>`：目前固定為 `R0`（不旋轉）

---

### 區段三：TSV Terminal 位置

跨層連線（Top ↔ Bottom）的矽穿孔座標。

```
NumTerminals <TSV數量>
Terminal <網路名稱> <x> <y>
...
```

範例輸出（case02）：
```
TopDiePlacement 6782
Inst C2 12626 9867 R0
Inst C3 11688 8745 R0
...

BottomDiePlacement 7125
Inst C1 13109 6930 R0
...

NumTerminals 1656
Terminal N2  11659 5290
Terminal N30 17069 9706
...
```

---

## 案例規模（case02）

| 項目 | 數值 |
|------|------|
| Die 尺寸 | 23000 × 19000 |
| 利用率上限 | 80%（Top / Bottom 各） |
| 元件數 | 13,907 |
| 網路數 | 19,547 |
| TSV 數 | 1,656 |
| Top Die 佈置 | 6,782 個 |
| Bottom Die 佈置 | 7,125 個 |
