#!/usr/bin/env python3
"""列出某段時間內新增/修改的 NeetCode 繳交檔案（依 git commit 時間）。

用法:
  python3 podcast/find_submissions.py --ref origin/main --since-commit <sha>
  python3 podcast/find_submissions.py --ref origin/main --date 2026-11-19   # 台北時間當天
輸出 JSON：每題的最新繳交、同題的歷次繳交路徑，方便撰寫講稿時比較。
"""
import argparse, json, subprocess, sys
from datetime import datetime, timedelta, timezone

TZ = timezone(timedelta(hours=8))  # Asia/Taipei


def git(*args):
    return subprocess.run(["git", *args], check=True, capture_output=True, text=True).stdout


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--ref", default="origin/main")
    p.add_argument("--since-commit", help="只看這個 commit 之後的變更（不含）")
    p.add_argument("--date", help="YYYY-MM-DD（台北時間），預設今天")
    a = p.parse_args()

    if a.since_commit:
        rng = [f"{a.since_commit}..{a.ref}"]
    else:
        d = datetime.strptime(a.date, "%Y-%m-%d") if a.date else datetime.now(TZ)
        start = datetime(d.year, d.month, d.day, tzinfo=TZ)
        end = start + timedelta(days=1)
        rng = [a.ref, f"--since={start.isoformat()}", f"--until={end.isoformat()}"]

    out = git("log", *rng, "--name-only", "--diff-filter=AM", "--format=@@%H")
    files = []
    for line in out.splitlines():
        if line and not line.startswith("@@") and "/submission-" in line and line not in files:
            files.append(line)

    problems = {}
    for f in files:
        parts = f.split("/")
        if len(parts) < 3:
            continue
        cat, slug = parts[0], parts[1]
        problems.setdefault(slug, {"category": cat, "slug": slug, "today": []})["today"].append(f)

    tree = git("ls-tree", "-r", "--name-only", a.ref).splitlines()
    for slug, info in problems.items():
        prefix = f"{info['category']}/{slug}/"
        info["all_submissions"] = sorted(t for t in tree if t.startswith(prefix))
        info["today"].sort(key=lambda s: int(s.rsplit("-", 1)[-1].split(".")[0]))
        info["latest"] = info["today"][-1]

    json.dump({"head": git("rev-parse", a.ref).strip(), "problems": list(problems.values())},
              sys.stdout, ensure_ascii=False, indent=2)


if __name__ == "__main__":
    main()
