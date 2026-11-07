# embed-learn 上传 GitHub 实操手册

> 本手册针对的是**这个仓库的真实结构与规则**，命令可以直接照抄。
> 用途有两个：首次把仓库推上 GitHub；以后往仓库里加个人文档（笔记、代码、资料）时，按第三章自检一遍再推。

---

## 一、仓库现状

### 1.1 环境

| 检查项 | 情况 |
| :--- | :--- |
| 系统 / 终端 | Windows + PowerShell |
| Git | 2.50.1 |
| GitHub CLI | 2.93.0，已登录 `oliver2981`（含 `repo` 权限） |
| 凭据 | HTTPS + gh 作为 credential helper，推送不用再输入密码 |
| 提交身份 | `user.name`、`user.email` 已配置（邮箱需与 GitHub 账号已验证的邮箱一致，见 8.4） |
| 本地 `.git` | 尚未初始化 |
| 远程仓库 | 尚未创建 |

### 1.2 目录结构

```
embed-learn/
├─ .gitignore                        190 B    ← 18 行，13 条规则
├─ README.md                         17.7 KB  ← 由 嵌入式学习计划.md 改名而来
├─ 嵌入式学习计划-每日执行版.md          30.8 KB
├─ embed-learn上传GitHub操作指南.md     本文档
├─ 笔记/
│  ├─ day1数据常量.md                 8.7 KB
│  └─ day2数据类型.md                 2.4 KB
└─ code/
   ├─ day1/
   │  ├─ hello.c                      133 B   ← 这一层没有 output/ 目录
   │  └─ number.c                     463 B
   └─ day2/
      ├─ float.c                      278 B
      ├─ scanf.c                      195 B
      ├─ size.c                       287 B
      └─ output/                              ← 被忽略（整棵子树）
         ├─ float.exe                 83.4 KB
         └─ scanf.exe                 83.8 KB
```

### 1.3 进度

| 事项 | 状态 |
| :--- | :--- |
| `.gitignore` 写好并验证 | ✅ 见第二章 |
| `README.md` 就位（复用学习计划） | ✅ |
| 目录按天拆分 | ✅ |
| 上传前隐私自检 | ⬜ 见第三章 |
| `git init` | ⬜ 第四章第 1 步 |
| 建远程仓库并推送 | ⬜ 第四章第 4 步 |

---

## 二、`.gitignore` 说明与验收

### 2.1 当前内容（18 行，13 条规则，190 字节）

```gitignore
# ---- 编译产物 ----
**/output/
*.exe
*.o
*.obj
*.out

# ---- 编辑器 / IDE ----
.vscode/
.vs/
*.lnk
*.suo
*.user

# ---- 系统 ----
Thumbs.db
desktop.ini
.DS_Store
```

格式要点：无 BOM（BOM 会污染第一行规则，导致首条规则失效）、UTF-8 编码、CRLF 换行、无全角字符、文件名必须是 `.gitignore`（不是 `.gitignore.txt`）。这几项任一出问题，规则都会静默失效。

### 2.2 规则的实际覆盖（实测结果）

在仓库副本上跑 `git status --ignored --short`：

```
?? .gitignore
?? README.md
?? code/
?? embed-learn上传GitHub操作指南.md
?? 嵌入式学习计划-每日执行版.md
?? 笔记/
!! code/day2/output/          ← 被忽略，目录折叠成一行
```

逐条核对命中规则（`git check-ignore -v` 输出格式为 `规则文件:行号:模式  路径`）：

| 测试路径 | 命中规则 | 结果 |
| :--- | :--- | :--- |
| `code/day2/output/float.exe` | `.gitignore:2  **/output/` | 忽略 |
| `code/day2/output/scanf.exe` | `.gitignore:2  **/output/` | 忽略 |
| `code/day1/hello.exe`（假设在这层编译） | `.gitignore:3  *.exe` | 忽略 |
| `code/day2/demo.o`（假设产生中间文件） | `.gitignore:4  *.o` | 忽略 |
| `code/day2/output/result.txt`（假设放运行结果） | `.gitignore:2  **/output/` | 忽略（副作用，见 2.4） |
| `embed-learn上传GitHub操作指南.md` | 无命中 | **不忽略，会随仓库上传** |

**首次提交的实际文件清单（11 个）：**

```
.gitignore
README.md
embed-learn上传GitHub操作指南.md
嵌入式学习计划-每日执行版.md
code/day1/hello.c
code/day1/number.c
code/day2/float.c
code/day2/scanf.c
code/day2/size.c
笔记/day1数据常量.md
笔记/day2数据类型.md
```

两个 `.exe` 没有进入这个清单。**当前规则没有漏洞。**

### 2.3 常见误区：`**/output/` 和 `*.exe` 是重叠，不是互补

容易冒出来的想法："`**/output/` 已经把目录挡住了，里面的 exe 当然也不会传，为什么还要写 `*.exe`？"

**这个判断本身是对的**：目录被忽略，整棵子树都不进 Git，`*.exe` 在 output 里面是纯重叠。它的价值只体现在 output **外面**：

| 规则 | 唯一管得住的场景 |
| :--- | :--- |
| `**/output/` | 产物按约定输出到 output/ 目录时 |
| `*.exe` | 产物**没有**落到 output/ 里时，例如直接 `gcc hello.c`，`hello.exe` 落在源码旁边 |

`code/day1` 这一层正好没有 output 子目录，所以这条冗余规则有实际价值 —— 它防"手滑"，不防"日常"。两条一起写是故意的双保险：

- 只写 `**/output/` → 依赖每次都记得 `-o output/xxx.exe`
- 只写 `*.exe` → 中间文件 `.o`、运行结果 `.txt` 仍会漏
- 两条都写 → 习惯怎么变都不会漏

### 2.4 目录忽略的副作用

`**/output/` 忽略的是**整个目录**，不只是 exe。往 `code/day2/output/` 里放任何想保留的东西（运行输出、对比数据、截图），也会被静默忽略，Git 不会有提示。

- 想把运行结果当证据留存 → 放 `笔记/` 或专门的 `results/` 目录
- 想忽略目录里的内容、但保留其中某个文件 → 写 `output/*` + `!output/想保留的文件`（见附录 C.5）

### 2.5 结论

规则不用再改。已覆盖：编译产物（目录 + 类型双保险）、中间文件、IDE 配置、系统垃圾文件。以后出现新类型的产物，追加一行即可。

---

## 三、上传前自检（个人文档必做）

仓库一旦推上 GitHub，**历史里的东西很难彻底删除**（要重写历史 + force push）。所以自检必须在第一次 `git push` 之前做完，以后每加一批个人文档也照这个清单走一遍。

### 3.1 先定可见性

| | Public 公开 | Private 私有 |
| :--- | :--- | :--- |
| 谁可见 | 所有人，搜索引擎可能收录 | 只有你和你邀请的人 |
| 适合 | 学习笔记、练习代码、作品集（求职可展示） | 含个人信息的资料、草稿、公司内部内容 |
| 代价 | 内容永久可被索引 | 不能当作品集链接给面试官 |

本仓库定位是学习笔记 + 练习代码，**公开是合理的**；但公开就意味着第三章下面每一条都要认真过一遍。

### 3.2 敏感信息扫描

推到公开仓库前，先跑一遍关键词扫描。只报位置、不打印内容：

```powershell
$d = "E:\GitHub\embed-learn"
$rules = @{
  "身份证号"   = "\d{17}[\dXx]"
  "手机号"     = "(?<!\d)1[3-9]\d{9}(?!\d)"
  "银行卡号"   = "(?<!\d)\d{16,19}(?!\d)"
  "邮箱地址"   = "[\w.+-]+@[\w-]+\.[\w.]+"
  "密码/密钥"  = "(?i)(password|passwd|密码|口令|secret|api[_-]?key|token)"
}
Get-ChildItem -LiteralPath $d -Recurse -File -Force -Include *.md,*.c,*.txt,*.ps1,*.json |
  ForEach-Object {
    $f = $_.Name
    foreach ($k in $rules.Keys) {
      $h = Select-String -LiteralPath $_.FullName -Pattern $rules[$k] -AllMatches -Encoding utf8 -ErrorAction SilentlyContinue
      if ($h) { "{0,-10} {1}：{2} 处（行 {3}）" -f $k, $f, $h.Count, (($h | Select-Object -First 8 | ForEach-Object { $_.LineNumber }) -join ",") }
    }
  }
```

**本仓库的扫描结果与判读：**

| 命中 | 位置 | 判读 |
| :--- | :--- | :--- |
| "身份证号" 模式 | `float.exe`、`scanf.exe` | **误报** —— 二进制文件里任意 18 位数字串都会命中；这两个文件本身已被忽略，不会上传 |
| "邮箱地址" | 本手册 | 原文写着真实邮箱，**已在本次改写中替换为占位符**，避免公开仓库暴露联系方式 |
| "密码/密钥" 字样 | 本手册 | **误报** —— 只是"不用再输入密码"这类说明文字，不含值 |

判读原则：**扫描只能捞"明显模式"，真正要防的是你心里知道不该公开的东西** —— 个人联系方式、身份证/学生证照片、成绩单、简历原件、公司内部文档、客户或同事信息。这类东西靠正则捞不出来，只能自己逐项确认。

处理方式：不想公开的文件，加进 `.gitignore` 对应位置（写法见附录 C）；已经提交过的，用 `git rm --cached 文件` 取消跟踪再补规则。

### 3.3 确认产物没混进来

```powershell
git status --ignored --short
```

`!!` 列表里应包含 `code/day2/output/`。如果 `??` 里出现 `.exe`、`.o`，说明规则没生效，先查原因再往下走。

### 3.4 确认没有超大文件

GitHub 单文件上限 100 MB，超过会被直接拒收：

```powershell
Get-ChildItem -LiteralPath "E:\GitHub\embed-learn" -Recurse -File -Force |
  Sort-Object Length -Descending | Select-Object -First 5 Name, @{n="MB";e={[math]::Round($_.Length/1MB,2)}}
```

本仓库目前最大的是 84 KB 的 exe（且已被忽略），远低于上限，无需处理。

### 3.5 确认提交身份

```powershell
git config user.name
git config user.email
```

邮箱必须与 GitHub 账号里**已验证**的地址一致（Settings → Emails）。不一致时提交能成功，但不会计入个人主页的贡献格子。

---

## 四、首次上传：方案 A（命令行，推荐）

### 第 1 步：进目录、初始化仓库

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn"
git init -b main
```

`-b main` 直接把默认分支命名为 `main`，省掉后面 `git branch -M main`。

### 第 2 步：确认要提交什么

```powershell
git status --short
git status --ignored --short
```

预期：`??` 共 6 项（`.gitignore`、`README.md`、本文档、`每日执行版.md`、`code/`、`笔记/`），`!!` 只有 `code/day2/output/` 一项。

**如果 `!!` 里没 output，或 `??` 里出现 `.exe`，停在这里先修规则**，用 `git check-ignore -v 文件名` 查是哪条出问题。

中文文件名若显示成 `\346\265\213...` 这类转义：

```powershell
git config --global core.quotepath false
```

只影响显示，不影响提交内容。

### 第 3 步：首次提交

```powershell
git add .
git diff --cached --name-only    # 核对：应该是 2.2 里那 11 个文件
git commit -m "docs: 初始化嵌入式学习笔记与 C 语言练习代码"
```

### 第 4 步：建远程仓库并推送

```powershell
gh repo create embed-learn --public --source=. --remote=origin --push --description "嵌入式 C 语言学习笔记与练习代码（12 周学习计划）"
```

| 参数 | 作用 |
| :--- | :--- |
| `--public` | 公开仓库；不想公开换 `--private` |
| `--source=.` | 用当前目录作为仓库内容 |
| `--remote=origin` | 自动执行 `git remote add origin`，不用手动加 |
| `--push` | 建完立刻推送 |
| `--description` | 仓库简介，显示在 About 区 |

### 第 5 步：验证

```powershell
git remote -v
git log --oneline
gh repo view oliver2981/embed-learn --web
```

浏览器里应看到：`README.md`、`.gitignore`、本文档、`每日执行版.md`、`code/`（day1 两个 .c、day2 三个 .c）、`笔记/`（2 个 md）。**看不到 `output/`** —— 这就是对的。

---

## 五、方案 B：网页建仓库 + 纯 Git

只把"建仓库"换成手动点，其余相同。

1. 打开 <https://github.com/new>
2. Repository name 填 `embed-learn`
3. 可见性选 Public / Private（决策见 3.1）
4. ⚠️ **不要勾** `Add a README file`、`Add .gitignore`、`Choose a license` —— 勾了远程会自带初始提交，本地推送会被拒，得多走一次 `git pull --rebase`
5. Create repository
6. 本地执行：

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn"
git init -b main
git add .
git commit -m "docs: 初始化嵌入式学习笔记与 C 语言练习代码"
git remote add origin https://github.com/oliver2981/embed-learn.git
git push -u origin main
```

---

## 六、日常更新

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn"
git add .
git commit -m "docs: 补充 Day3 运算符与表达式笔记"
git push
```

提交信息带前缀，翻历史一眼看出性质：

| 前缀 | 用在哪 |
| :--- | :--- |
| `docs:` | 笔记、README、计划文档 |
| `feat:` | 新增练习代码 |
| `fix:` | 修正笔记里的错误结论 |
| `chore:` | `.gitignore`、目录调整等杂项 |

建议一天一提交，消息写清"今天学了什么"。12 周下来，commit 时间线本身就是学习进度的证据。

### 编译约定（可选但推荐）

统一产物输出位置，让"源码目录只有源码"：

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn\code\day1"
New-Item -ItemType Directory output -Force
gcc hello.c -o output/hello.exe
```

好处：产物永远在 output 里，`**/output/` 一条规则就够用，`*.exe` 退化为兜底。

---

## 七、常见报错对照

| 报错 | 原因 | 解决 |
| :--- | :--- | :--- |
| `fatal: not a git repository` | 不在仓库目录 | `Set-Location -LiteralPath "E:\GitHub\embed-learn"` |
| `error: remote origin already exists` | 已加过 origin | `git remote set-url origin <新地址>` |
| `Updates were rejected because the remote contains work...` | 远程有本地没有的提交（建仓库时勾了 README） | `git pull --rebase origin main` 后 `git push` |
| `Authentication failed` / `could not read Username` | 凭据失效 | `gh auth login`，必要时 `gh auth setup-git` |
| `error: src refspec main does not match any` | 没有提交，或分支不叫 main | 先 `git commit`；否则 `git branch -M main` |
| `warning: LF will be replaced by CRLF` | Windows 换行符提示 | 警告而已，可忽略 |
| `exceeds GitHub's file size limit of 100.00 MB` | 单文件超 100 MB | 用 Git LFS，或排除该文件 |
| 中文文件名显示成八进制 | Git 默认转义非 ASCII | `git config --global core.quotepath false` |
| `Failed to connect to github.com port 443` | 网络不通，不是配置错 | 见 7.1 |

### 7.1 推送报 `Failed to connect to github.com port 443`

**这是网络问题，不是 Git 配置问题。** 典型现象：`gh repo create` 明明成功了、仓库也出现在 GitHub 上，但点进去是空的 —— 因为 `gh` 走的是 `api.github.com`，而 `git push` 走的是 `github.com:443`，两条链路可以一通一不通。

先分清哪条路断了：

```powershell
Test-NetConnection -ComputerName github.com -Port 443 -InformationLevel Quiet      # git push 用这条
Test-NetConnection -ComputerName api.github.com -Port 443 -InformationLevel Quiet  # gh 用这条
```

`github.com` 返回 `False`、`api.github.com` 返回 `True`，就是本节这种情况。

本机若装了代理（Clash 类通常监听 7890 或 7897），先验证代理本身能不能访问 GitHub：

```powershell
curl.exe -s -o NUL -w "HTTP %{http_code}`n" --max-time 20 -x http://127.0.0.1:7897 https://github.com/
```

返回 `HTTP 200` 说明代理可用。再把它只绑定给 github.com（不影响 gitee 等其他远端）：

```powershell
git config --global http.https://github.com.proxy http://127.0.0.1:7897
git push -u origin main
```

看到 `* [new branch] main -> main` 即成功。事后想撤掉这条配置：

```powershell
git config --global --unset http.https://github.com.proxy
```

**排查顺序记住一句话：`gh` 成功不等于能推送。** 仓库建出来是空的，先怀疑 `github.com:443` 的网络，再查分支名和提交是否存在。

---

## 八、仓库建好之后（半小时可完成）

1. **About 区**：Description 写"嵌入式 C 语言学习笔记与练习代码"；Topics 加 `c`、`embedded`、`learning-notes`，学到单片机再加 `stm32`。招聘方按关键词搜人。
2. **LICENSE**：个人学习仓库用 MIT 最省事。Add file → Create new file，文件名 `LICENSE`，右侧选模板。
3. **Pin 到主页**：Your repositories → Customize your pins，把 `embed-learn` 与 `web-ui-automation` 一起钉上。
4. **确认贡献计数**：见 3.5，邮箱需与 GitHub 已验证邮箱一致，否则有提交但主页不涨绿格子。

---

## 附录 A：`.gitignore` 定稿（18 行）

```gitignore
# ---- 编译产物 ----
**/output/
*.exe
*.o
*.obj
*.out

# ---- 编辑器 / IDE ----
.vscode/
.vs/
*.lnk
*.suo
*.user

# ---- 系统 ----
Thumbs.db
desktop.ini
.DS_Store
```

维护口径：换目录结构不用动它（`**/output/` 任意深度生效）；新增产物类型往后追加一行；新增"不打算公开的个人文档"时，把文件名写进最后一段。

---

## 附录 B：`.gitignore` 到底管什么（原理）

**一句话：`.gitignore` 只对"尚未被 Git 跟踪"的文件生效。**

它不是删除，不是封印。它只告诉 Git：**以后 `git add` 时默认跳过这些文件**。文件仍在硬盘上，编译照常、运行照常，只是 Git 看不见。

| 疑问 | 真实行为 |
| :--- | :--- |
| 加了之后文件会被删吗 | **不会**，磁盘上原样还在 |
| 就永远不能提交了吗 | **默认不提交，但可 `git add -f 文件名` 强制** |
| 已被提交过的文件加进去生效吗 | **不生效**。已跟踪文件不受约束，需 `git rm --cached 文件名` |
| `.gitignore` 自己要提交吗 | **要**，它是普通文件；不提交，换台电脑规则全丢 |
| 能只忽略一部分吗 | 能，用 `!` 反向排除（有前提，见 C.5） |
| 忽略一个目录会怎样 | 整棵子树全挡，不只是某类文件（见 2.4） |

四种典型行为：

| 场景 | 结果 |
| :--- | :--- |
| 先有 `.gitignore` 再 `git add .` | 目标文件不进暂存区，`.gitignore` 自己进去了 |
| **先提交了文件，之后才写 `.gitignore`** | 文件仍在待提交列表，规则完全失效 |
| `git add -f 被忽略的文件` | 强行进入，规则被突破 |
| `*.exe` + `!keep.exe` | `keep.exe` 被重新纳入 |

第二条是核心：**`.gitignore` 必须在第一次 `git add` 之前写好**。本仓库当前还没 `git init`，正处在正确时机。

---

## 附录 C：`.gitignore` 怎么写（语法 + 验证）

### C.1 主干规则

一行一条模式（不能逗号分隔）；`#` 开头是注释；空行只用于分组，不影响规则。

### C.2 通配符只有 4 个

| 符号 | 含义 | 例 |
| :--- | :--- | :--- |
| `*` | 任意字符（不含 `/`），可跨目录层级匹配文件名 | `*.exe` 管所有目录 |
| `?` | 单个字符 | `?tmp.tmp` 匹配 `atmp.tmp`，不匹配 `xtmp2.tmp` |
| `**` | 跨任意多层目录 | `**/output/` |
| `[abc]` | 字符集合 | `file[0-9].c` |

### C.3 别写错的三组对比

| 写法 | 实际匹配 |
| :--- | :--- |
| `output/` | 所有叫 output 的目录（任何深度）—— 纯名字模式默认全目录搜索 |
| `**/output/` | 同上，只是把"任意深度"写明，**效果等价**，可读性更好 |
| `/output` | **只匹配根目录**那一层 |
| `output` | 目录和同名文件都匹配，容易误伤 |

记两件事：**`/` 开头 = 锚定根目录**，**`/` 结尾 = 这是目录**。

### C.4 顺序：后面的覆盖前面的

从第一行读到最后一行，**最后命中的规则说了算**，所以 `!` 必须写在它要推翻的规则之后。

### C.5 `!` 的坑：父目录被忽略就救不回来

```gitignore
# ❌ 错误
output/
!output/keep.exe
```

```
$ git check-ignore -v output/keep.exe
.gitignore:1:output/    output/keep.exe     ← 仍被挡
```

```gitignore
# ✅ 正确
output/*
!output/keep.exe
```

```
$ git check-ignore -v output/keep.exe
.gitignore:2:!output/keep.exe    output/keep.exe   ← 生效
```

记法：**要救回某个文件，父目录本身不能被忽略，只能忽略它里面的 `*`。**

### C.6 验证：`git check-ignore`

判断"到底生不生效"的唯一可靠方法（比 `git status` 准，status 分不清"被忽略"和"不存在"）：

```powershell
git check-ignore -v code/day1/hello.exe
```

- **有输出** → 被忽略了，按 `行号:模式` 核对是不是要的那条
- **没输出** → 没被忽略，会被提交（退出码 `1`）

看全仓库哪些被忽略：

```powershell
git status --ignored --short
```

`??` = 未跟踪待添加，`!!` = 已忽略。

**不想在自己仓库上试的做法**：复制一份到临时目录，在副本上初始化再验证：

```powershell
$tmp = Join-Path $env:TEMP "gi-check"
Copy-Item -LiteralPath "E:\GitHub\embed-learn" -Destination $tmp -Recurse -Force
git -C $tmp init -b main
git -C $tmp status --ignored --short
git -C $tmp check-ignore -v "code/day1/hello.exe"
git -C $tmp add -A; git -C $tmp diff --cached --name-only
Remove-Item -LiteralPath $tmp -Recurse -Force
```

注意 `git init` 必须写成 `git -C $tmp`，否则会在当前目录建仓库。

### C.7 决定写什么：三步

1. **先编译一次，看多出了什么** —— 那些就是产物，要挡
2. **按两条线写**：类型（`*.exe`）+ 目录（`**/output/`），互为兜底
3. **验证**：`git add .` 后看 `git status`，产物不出现即通过；拿不准用 `check-ignore`

不用一次写完，以后出现新垃圾文件回来加一行即可 —— 只要那些文件还没被提交过。

### C.8 五个常见坑

| 现象 | 原因 | 解决 |
| :--- | :--- | :--- |
| 规则不生效 | 文件已被提交过 | `git rm --cached 文件名` |
| 规则不生效 | 文件名变成 `.gitignore.txt`（Windows 隐藏扩展名） | 用 VSCode 新建，别用"新建文本文档"改名 |
| 规则不生效 | 放错位置（在子目录里） | 必须放仓库根目录 |
| `!` 救不回文件 | 父目录被整体忽略 | 改 `目录/*` + `!目录/文件` |
| 误伤想提交的文件 | 模式太宽 | `check-ignore -v` 找出命中行，改成 `目录/` |

---

## 附录 D：一页速查

首次上传：

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn"
git init -b main
git status --short                 # 确认没有 .exe
git status --ignored --short       # 确认 output/ 在 !! 列表里
git add .
git diff --cached --name-only      # 应该是 11 个文件
git commit -m "docs: 初始化嵌入式学习笔记与 C 语言练习代码"
gh repo create embed-learn --public --source=. --remote=origin --push --description "嵌入式 C 语言学习笔记与练习代码（12 周学习计划）"
git remote -v
gh repo view oliver2981/embed-learn --web
```

日常更新：

```powershell
Set-Location -LiteralPath "E:\GitHub\embed-learn"
git add . && git commit -m "docs: 今日内容" && git push
```

---

## 附录 E：提交错了怎么办

| 情况 | 处理 |
| :--- | :--- |
| 刚 commit 发现漏文件（未 push） | `git add .` → `git commit --amend --no-edit` |
| 想撤销最近一次提交、保留改动 | `git reset --soft HEAD~1` |
| 已 push，想改提交信息 | `git commit --amend` → `git push --force-with-lease`（个人仓库可以） |
| 忘了加规则，产物已提交 | `git rm --cached 文件` → 补 `.gitignore` → `git commit -m "chore: 移除编译产物"`（历史仍留 blob，彻底清需 `git filter-repo`，个人仓库一般不必） |
| 误把敏感文件推上公开仓库 | 立刻 `git rm --cached` 提交删除；若担心已被索引，把仓库改 Private 或删库重建。**推上去过就等于泄露过**，最稳妥是重建仓库 |
| 远端地址填错 | `git remote set-url origin <正确地址>` |
| 整个仓库想重来 | 删掉 `.git` 目录后重新 `git init`（提交历史归零） |

---

## 附录 F：`README.md` 三种做法

| 做法 | 说明 | 本仓库 |
| :--- | :--- | :--- |
| 学习计划改名做 README | 首页直接是计划，信息量大 | 采用 |
| 独立写一份项目 README | 首页是"仓库说明 + 目录表 + 进度" | 可选，后面可换 |
| 网页端直接编辑 | 不开本地编辑器，在 GitHub 上改 | 适合临时小改 |

不管哪种，README 必须存在 —— 没有 README 时 GitHub 首页是空的，点进来什么都看不到。
